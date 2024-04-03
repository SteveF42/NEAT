#ifndef Neat_HPP
#define Neat_HPP
#include "Genome.hpp"
#include "Species.hpp"
#include <iostream>
#include <memory>

using std::unique_ptr;

typedef unique_ptr<Genome> GenomePtr;
typedef unique_ptr<Species> SpeciesPtr;

class Neat
{
public:
    Neat(int input, int output);
    void initialize(int population = 500);
    void train(int genCount);
    void evolve();
    void clear();
    void test();

    const vector<GenomePtr> &getGenomes() const;
    const vector<SpeciesPtr> &getSpecies() const;
    int getPopulation() const;
    friend std::ostream &operator<<(std::ostream &os, const Neat &other);

protected:
    int input;
    int output;
    int population;
    int genCount = 0;
    Genome bestGenome;
    vector<GenomePtr> allGenomes;
    vector<SpeciesPtr> allSpecies;
    // inherited class will implement playGame
    virtual double playGame(Genome *genome) = 0;
    virtual void testNetwork(Genome *genome) = 0;
    void trainGeneration();

private:
    void speciate();
    void kill();
    void breed();
    void mutate();
};

#endif
