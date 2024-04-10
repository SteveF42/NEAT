#include "Neat.hpp"
#include "Util.hpp"

using std::make_unique;

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
        allGenomes.push_back(make_unique<Genome>(input, output, true));
    }
    LinkGene::setNextID(input * output);
    NodeGene::setNextID(input + output);
}

void Neat::train(int genCount)
{
    bestGenome.setFitness(INT32_MIN);
    for (int i = 0; i < genCount; i++)
    {
        this->genCount++;
        trainGeneration();
        evolve();
    }
}

void Neat::test()
{
    testNetwork(&bestGenome);
}

const vector<GenomePtr> &Neat::getGenomes() const
{
    return allGenomes;
}

const vector<SpeciesPtr> &Neat::getSpecies() const
{
    return allSpecies;
}

Genome &Neat::getBestGenome()
{
    std::shared_lock lock(mutex_);
    return bestGenome;
}

int Neat::getPopulation() const
{
    return population;
}

std::ostream &operator<<(std::ostream &os, const Neat &other)
{
    os << "Population: " << other.population << '\n';
    os << "Genomes: " << other.allGenomes.size() << '\n';
    for (const auto &species : other.allSpecies)
    {
        os << "Species size: ";
        os << other.allSpecies.size() << '\n';
    }
    return os;
}

void Neat::trainGeneration()
{
    double avgFitness = 0;
    for (const auto &genome : allGenomes)
    {
        genome->setFitness(playGame(genome.get()));
        avgFitness += genome->getFitness();
        std::unique_lock lock(mutex_);
        if (genome->getFitness() > bestGenome.getFitness())
        {
            bestGenome = *genome;
        }
        lock.unlock();
    }
    std::cout << "Generation: " << genCount
              << " Average Fitness: " << avgFitness / population
              << " Best Fitness: " << bestGenome.getFitness() << '\n';
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
    for (const auto &genome : allGenomes)
    {
        bool found = false;
        for (const auto &species : allSpecies)
        {
            const Genome *leader = species->getLeader();
            if (genome->distance(*leader) <= Config::speciationThreshold)
            {
                species->addMember(genome.get());
                found = true;
                break;
            }
        }
        if (!found)
        {
            SpeciesPtr newSpecies = make_unique<Species>();
            newSpecies->setLeader(genome.get());
            allSpecies.push_back(std::move(newSpecies));
        }
    }

    for (const auto &species : allSpecies)
    {
        species->evaluateScore();
    }
}

void Neat::kill()
{
    for (auto it = allSpecies.begin(); it != allSpecies.end();)
    {
        Species *species = it->get();
        species->kill(Config::deathRate);
        if (species->getSpeciesSize() < 2)
        {
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
    vector<GenomePtr> newGenomes;
    int size = allSpecies.size();
    int children = population / size;
    int remainder = population - children * size;

    for (const auto &species : allSpecies)
    {
        for (int i = 0; i < children; i++)
        {
            newGenomes.push_back(GenomePtr(species->breed()));
        }
    }

    for (int i = 0; i < remainder; i++)
    {
        int index = randNumber(size);
        newGenomes.push_back(GenomePtr(allSpecies[index]->breed()));
    }
    allGenomes.clear();
    this->allGenomes = std::move(newGenomes);
    allSpecies.clear();
}

void Neat::mutate()
{
    for (const auto &genome : allGenomes)
    {
        genome->mutate();
    }
}