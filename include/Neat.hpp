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
    void train(int genCount);
    void evolve();
    void clear();
    void test();

    vector<Genome *> getGenomes() const;
    vector<Species *> getSpecies() const;
    int getPopulation() const;
    friend std::ostream &operator<<(std::ostream &os, const Neat &other);

protected:
    int input;
    int output;
    int population;
    int genCount = 0;
    Genome bestGenome;
    vector<Genome *> allGenomes;
    vector<Species *> allSpecies;
    vector<NodeGene *> allNodes;
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
