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
    NodeGene(int id, NodeType type);
    NodeGene(NodeType type, double bias);
    NodeGene(int id, NodeType type, double bias);
    NodeGene(NodeGene &other);
    void addToLink(const LinkGene &link);
    void addFromLink(const LinkGene &link);
    void removeLink(const LinkGene &link);
    void removeFromLink(const LinkGene &link);

    int getID() const;
    double getBias() const;
    void setBias(double bias);
    NodeType getType() const;
    vector<const LinkGene *> getToLinks() const;
    vector<const LinkGene *> getFromLinks() const;

    void addAccumalator(double value);
    void resetAccumalator();
    double activate();
    double output;

    static void setNextID(int id);
    static NodeGene *getNewNode();
    inline static int nextID = 0;
private:
    double bias;
    double accumalator;
    int id;
    NodeType type;
    ActivationRelu activationRelu;
    vector<const LinkGene *> fromLinks;
    vector<const LinkGene *> toLinks;
};

#endif