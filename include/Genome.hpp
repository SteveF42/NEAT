#ifndef GENOME_HPP
#define GENOME_HPP
#include <iostream>
#include <vector>
#include "NodeGene.hpp"
#include "LinkGene.hpp"
using std::vector;

struct Config
{
    double initMean = 0.0; // mean of initial weight distribution
    double initStddev = 1.0; // standard deviation of initial weight distribution
    double min = -20.0; // minimum weight value
    double max = 20.0; // maximum weight value
    double mutationRate = 0.8; // probability of mutation
    double mutatePower = 0.5; // power of mutation
    double replaceRate = 0.1; // probability of replacing weights
};

class Genome
{
public:
    Genome(int inputs, int outputs, bool initInputs = true);
    void initialize();
    double getFitness();
    void mutate();
    static Genome *crossGenomes(const Genome &dominant, const Genome &recessive);

    vector<float> activate(vector<double> inputs);
    vector<LinkGene *> getLinks();
    vector<NodeGene *> getNodes();

    // mutations
    void addNode();
    void removeNode();
    void addLink();
    void removeLink();
    bool isDominant(const Genome &rhs);

private:
    double fitness;
    int inputs;
    int outputs;
    // first n + m nodes are inputs and outputs
    vector<NodeGene *> nodes;
    vector<LinkGene *> links;

    static NodeGene crossNeurons(const NodeGene &lhs, const NodeGene &rhs);
    static LinkGene crossLinks(const LinkGene &lhs, const LinkGene &rhs);

    // util
    bool containsCycle(int fromNode);
    NodeGene *findNode(int nodeID) const;
    LinkGene *findLink(int linkID) const;
};

#endif