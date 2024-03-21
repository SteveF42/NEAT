#ifndef LinkGene_HPP
#define LinkGene_HPP
#include "NodeGene.hpp"

class NodeGene;

class LinkGene
{
public:
    LinkGene(NodeGene* toNode, NodeGene* fromNode, double weight);
    void setEnabled(bool enabled);
    double getWeight() const;
    bool isEnabled() const;
    NodeGene* getToNode() const;
    NodeGene* getFromNode() const;
    
private:
    int LinkeID;
    inline static int nextID = 0;
    NodeGene* toNode;
    NodeGene* fromNode;
    double weight;
    bool enabled;
};

#endif