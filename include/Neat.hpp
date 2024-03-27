#ifndef Neat_HPP
#define Neat_HPP
#include "Genome.hpp"
#include "Species.hpp"
#include <iostream>

class Neat
{
public:
    Neat(int input, int output);
    void initialize(int population = 500);
    void evolve();
    void clear();

    vector<Genome *> getGenomes() const
    {
        return allGenomes;
    }
    vector<Species *> getSpecies() const
    {
        return allSpecies;
    }
    int getPopulation() const
    {
        return population;
    }

    friend std::ostream& operator<<(std::ostream& os, const Neat &other)
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

private:
    int input;
    int output;
    int population;
    vector<Genome *> allGenomes;
    vector<Species *> allSpecies;
    vector<NodeGene *> allNodes;

    void speciate();
    void kill();
    void breed();
    void mutate();
};

#endif
