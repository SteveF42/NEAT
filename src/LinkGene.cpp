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

void LinkGene::setWeight(double weight)
{
    this->weight = weight;
}

bool LinkGene::isEnabled() const
{
    return enabled;
}

int LinkGene::getID() const
{
    return LinkeID;
}

NodeGene* LinkGene::getToNode() const
{
    return toNode;
}

NodeGene* LinkGene::getFromNode() const
{
    return fromNode;
}   
