#ifndef GENOME_HPP
#define GENOME_HPP
#include <iostream>
#include <vector>
#include "NodeGene.hpp"
#include "LinkGene.hpp"
using std::vector;

class Genome
{
public:
    Genome(int inputs, int outputs, bool initInputs = true);
    void initialize();
    double getFitness();
    void mutate();
    double distance(const Genome &other);
    static Genome *crossGenomes(const Genome &dominant, const Genome &recessive);

    vector<float> activate(vector<double> inputs);
    vector<LinkGene *> getLinks();
    vector<NodeGene *> getNodes();

    // mutations
    void addNode();
    void removeNode();
    void addLink();
    void removeLink();
    void weightRandom();
    void toggleWeight();
    void weightShift();
    void shiftBias();

    bool operator<(const Genome &other);
    bool operator>(const Genome &other);

private:
    double fitness;
    int inputs;
    int outputs;
    // first n + m nodes are inputs and outputs
    vector<NodeGene *> nodes;
    vector<LinkGene *> links;

    static NodeGene* crossNeurons(const NodeGene &lhs, const NodeGene &rhs);
    static LinkGene* crossLinks(const LinkGene &lhs, const LinkGene &rhs);

    // util
    bool containsCycle(int fromNode);
    NodeGene* getRandomNode();
    LinkGene* getRandomLink();
    NodeGene *findNode(int nodeID) const;
    LinkGene *findLink(int linkID) const;
};

#endif