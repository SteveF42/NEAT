#ifndef Node_HPP
#define Node_HPP

#include "ActivationRelu.hpp"
#include "LinkGene.hpp"
#include <vector>

using std::vector;
class LinkGene;

enum NodeType
{
    INPUT,
    HIDDEN,
    OUTPUT
};

class NodeGene
{
public:
    NodeGene(int id, double bias, NodeType type);
    void addToLink(const LinkGene &link);
    void removeLink(const LinkGene &link);
    int getID() const;
    double getBias() const;
    NodeType getType() const;
    vector<const LinkGene*> getToLinks() const;

private:
    int id;
    NodeType type;
    double bias;
    ActivationRelu activation;
    vector<const LinkGene*> toLinks;
};

#endif