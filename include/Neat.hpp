#ifndef Neat_HPP
#define Neat_HPP
#include "Genome.hpp"
#include "Species.hpp"

class Neat
{
public:
    Neat(int input, int output);
    void initialize(int population = 500);
    void evolve();
    void clear();

private:
    int input;
    int output;
    int population;
    vector<Genome *> allGenomes;
    vector<Species *> allSpecies;

    void speciate();
    void kill();
    void breed();
    void mutate();
};

#endif
