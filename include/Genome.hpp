#ifndef GENOME_HPP
#define GENOME_HPP
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include "NodeGene.hpp"
#include "LinkGene.hpp"
using std::map;
using std::unordered_set;
using std::vector;

struct networkLayer
{
    networkLayer(NodeType type = HIDDEN) : type(type) {}
    void addNode(NodeGene *node)
    {
        nodes.insert(node);
    }
    unordered_set<NodeGene *> nodes;
    NodeType type;
};

class Genome
{
public:
    Genome(int inputs, int outputs, bool initInputs = true);
    Genome() {}
    void initialize();
    double getFitness();
    void setFitness(double fitness);
    void mutate();
    double distance(const Genome &other);
    static Genome *crossGenomes(const Genome &dominant, const Genome &recessive);

    vector<double> activate(vector<double> inputs);
    map<int, LinkGene *> getLinks();
    map<int, NodeGene *> getNodes();

    // mutations
    void addNode();
    void removeNode();
    void addLink();
    void removeLink();
    void weightRandom();
    void toggleWeight();
    void weightShift();
    void shiftBias();

    bool operator<(const Genome &other);
    bool operator>(const Genome &other);
    Genome operator=(const Genome &other);

private:
    const int INPUT_LAYER = -2;
    const int OUTPUT_LAYER = -1;
    double fitness;
    int inputs;
    int outputs;
    // first n + m nodes are inputs and outputs
    map<int, NodeGene *> allNodes;
    map<int, LinkGene *> allLinks;
    map<int, networkLayer> layers;

    static NodeGene *crossNeurons(const NodeGene &lhs, const NodeGene &rhs);
    static LinkGene *crossLinks(const LinkGene &lhs, const LinkGene &rhs);

    // util
    bool containsCycle(int fromNode);
    void removeNodeFromLayer(NodeGene *node);
    void buildLayers();
    bool onSameLayer(NodeGene *lhs, NodeGene *rhs);
    void addNodeToLayer(NodeGene *node);
    int findNodeLayer(int nodeID);
    NodeGene *getRandomNode(bool includeOuputs = false, bool includeInputs = false);
    LinkGene *getRandomLink();
    NodeGene *findNode(int nodeID) const;
    LinkGene *findLink(int linkID) const;
    void activationSoftmax(vector<double> &v);
};

#endif