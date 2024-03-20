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
    Genome(int inputs, int outputs);
    Genome crossGenomes(const Genome &rhs);
    void mutate();
    void activate();

    vector<LinkGene*> getLinks();
    vector<NodeGene*> getNodes();

    // mutations
    void addNode();
    void removeNode();
    void addLink();
    void removeLink();
    bool isDominant(const Genome &rhs);
private:
    float fitness;
    int inputs;
    int outputs;
    //first n + m nodes are inputs and outputs
    vector<NodeGene*> nodes;
    vector<LinkGene*> links;
    

    NodeGene crossNeurons(const NodeGene &lhs, const NodeGene &rhs);
    LinkGene crossLinks(const LinkGene &lhs, const LinkGene &rhs);

    //util
    bool containsCycle(int fromNode);
    bool cycleUtil(int v, bool visited[], bool *rs);
};

#endif