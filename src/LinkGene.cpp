#include "Linkgene.hpp"

LinkGene::LinkGene(NodeGene* toNode, NodeGene* fromNode, double weight)
{
    this->LinkeID = nextID++;
    this->fromNode = fromNode;
    this->toNode = toNode;
    this->weight = weight;
    this->enabled = true;
}

void LinkGene::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

double LinkGene::getWeight() const
{
    return weight;
}

bool LinkGene::isEnabled() const
{
    return enabled;
}

NodeGene* LinkGene::getToNode() const
{
    return toNode;
}

NodeGene* LinkGene::getFromNode() const
{
    return fromNode;
}   
