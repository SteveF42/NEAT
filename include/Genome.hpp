#ifndef GENOME_HPP
#define GENOME_HPP
#include <iostream>
#include <vector>
#include "NodeGene.hpp"

struct LinkID
{
    int inNode;
    int outNode;
};

struct LinkGene
{
    LinkID id;
    double weight;
    bool enabled;
};

class Genome
{
public:
    Genome(int genomeID, int inputs, int outputs);
    Genome crossGenomes(const Genome &rhs);
    void mutate();

private:
    // mutations
    LinkGene crossLinks(const LinkGene &rhs);
    NodeGene crossNeurons(const NodeGene &rhs);
    void addNode();
    void removeNode(int nodeID);
    void addLink();
    void removeLink();
    bool isDominant(const Genome &rhs);
    int genomeID;
    float fitness;
    int inputs;
    int outputs;
    std::vector<NodeGene> nodes;
    std::vector<LinkGene> links;

    NodeGene crossNeurons(const NodeGene &rhs);
    LinkGene crossLinks(const LinkGene &rhs);
};

#endif