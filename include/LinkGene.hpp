#ifndef LinkGene_HPP
#define LinkGene_HPP
#include "NodeGene.hpp"

class NodeGene;

class LinkGene
{
public:
    LinkGene(NodeGene* inNode, NodeGene* outNode, double weight);
    void setEnabled(bool enabled);
    double getWeight() const;
    bool isEnabled() const;
    NodeGene* getToNode() const;
    NodeGene* getFromNode() const;
    
private:
    int LinkeID;
    NodeGene* toNode;
    NodeGene* fromNode;
    double weight;
    bool enabled;
};

#endif