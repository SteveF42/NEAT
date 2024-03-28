#include "Neat.hpp"
#include "Util.hpp"

Neat::Neat(int input, int output)
{
    this->input = input;
    this->output = output;
    this->population = 0;
}

void Neat::initialize(int population)
{
    this->population = population;
    for (int i = 0; i < population; i++)
    {
        allGenomes.push_back(new Genome(input, output, true));
    }
    LinkGene::setNextID(input * output);
    NodeGene::setNextID(input + output);
}

void Neat::train(int genCount)
{
    for (int i = 0; i < genCount; i++)
    {
        this->genCount++;
        trainGeneration();
        evolve();
    }
}

void Neat::test()
{
    testNetwork(bestGenome);
}

vector<Genome *> Neat::getGenomes() const
{
    return allGenomes;
}

vector<Species *> Neat::getSpecies() const
{
    return allSpecies;
}

int Neat::getPopulation() const
{
    return population;
}

std::ostream &operator<<(std::ostream &os, const Neat &other)
{
    os << "Population: " << other.population << '\n';
    os << "Genomes: " << other.allGenomes.size() << '\n';
    for (auto species : other.allSpecies)
    {
        os << "Species size: ";
        os << other.allSpecies.size() << '\n';
    }
    return os;
}

void Neat::trainGeneration()
{
    double avgFitness = 0;
    double bestFitness = INT32_MIN;
    for (auto genome : allGenomes)
    {
        genome->setFitness(playGame(genome));
        avgFitness += genome->getFitness();
        if (genome->getFitness() > bestFitness)
        {
            bestFitness = genome->getFitness();
            bestGenome = genome;
        }
    }
    std::cout << "Generation: " << genCount
              << " Average Fitness: " << avgFitness / population
              << " Best Fitness: " << bestFitness << '\n';
}

void Neat::evolve()
{
    speciate();
    kill();
    breed();
    mutate();
}

void Neat::speciate()
{
    allSpecies.clear();
    for (auto genome : allGenomes)
    {
        bool found = false;
        for (auto species : allSpecies)
        {
            const Genome *leader = species->getLeader();
            if (genome->distance(*leader) <= Config::speciationThreshold)
            {
                species->addMember(genome);
                found = true;
                break;
            }
        }
        if (!found)
        {
            Species *newSpecies = new Species();
            newSpecies->setLeader(genome);
            allSpecies.push_back(newSpecies);
        }
    }

    for (auto species : allSpecies)
    {
        species->evaluateScore();
    }
}

void Neat::kill()
{
    for (auto it = allSpecies.begin(); it != allSpecies.end();)
    {
        Species *species = *it;
        species->kill(Config::deathRate);
        if (species->getSpeciesSize() < 2)
        {
            delete species;
            it = allSpecies.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Neat::breed()
{
    vector<Genome *> newGenomes;
    int size = allSpecies.size();
    int children = population / size;
    int remainder = population - children * size;

    for (auto species : allSpecies)
    {
        for (int i = 0; i < children; i++)
        {
            newGenomes.push_back(species->breed());
        }
    }

    for (int i = 0; i < remainder; i++)
    {
        int index = randNumber(size);
        newGenomes.push_back(allSpecies[index]->breed());
    }

    for (auto genome : allGenomes)
    {
        delete genome;
    }

    allGenomes = newGenomes;

    for (auto species : allSpecies)
    {
        delete species;
    }

    allSpecies.clear();
}

void Neat::mutate()
{
    for (auto genome : allGenomes)
    {
        genome->mutate();
    }
}