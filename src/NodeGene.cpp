#include "NodeGene.hpp"

NodeGene::NodeGene(int id, double bias, NodeType type)
{
    this->id = id;
    this->bias = bias;
    this->type = type;
}

void NodeGene::addToLink(const LinkGene &link)
{
    toLinks.push_back(&link);
}

void NodeGene::removeLink(const LinkGene &link)
{
    for (int i = 0; i < toLinks.size(); i++)
    {
        if (toLinks[i]->getFromNode()->getID() == link.getFromNode()->getID() && toLinks[i]->getToNode()->getID() == link.getToNode()->getID())
        {
            toLinks.erase(toLinks.begin() + i);
            return;
        }
    }
}
int NodeGene::getID() const
{
    return id;
}

double NodeGene::getBias() const
{
    return bias;
}

NodeType NodeGene::getType() const
{
    return type;
}

vector<const LinkGene*> NodeGene::getToLinks() const
{
    return toLinks;
}