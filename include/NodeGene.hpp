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
    NodeGene(double bias, NodeType type);
    void addToLink(const LinkGene &link);
    void removeLink(const LinkGene &link);
    int getID() const;
    double getBias() const;
    NodeType getType() const;
    vector<const LinkGene*> getToLinks() const;
    void addAccumalator(double value);
    void resetAccumalator();
    double activate();
    double output;

private:
    double bias;
    double accumalator;
    inline static int nextID = 0;
    int id;
    NodeType type;
    ActivationRelu activationRelu;
    vector<const LinkGene*> toLinks;
};

#endif