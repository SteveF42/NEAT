#ifndef NETWORKLAYER_HPP
#define NETWORKLAYER_HPP
#include <vector>
#include "NodeGene.hpp"
using std::vector;

class Networklayer
{
    Networklayer(NodeType type = HIDDEN) : type(type) {}
    void addNode(NodeGene *node)
    {
        nodes.push_back(node);
    }
    void removeNodeFromLayer(NodeGene* node);
    int findNodeLayer(int nodeID);
    void addNodeToLayer(NodeGene *node);

private:
    vector<NodeGene *> nodes;
    NodeType type;
};

#endif