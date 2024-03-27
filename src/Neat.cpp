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
    LinkGene::setNextID(input + output + 1);
    NodeGene::setNextID(input + output + 1);
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