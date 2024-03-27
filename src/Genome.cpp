#include "Genome.hpp"
#include "Util.hpp"
#include "random"
#include <stack>
#include <unordered_set>
#include <unordered_map>

Genome::Genome(int inputs, int outputs, bool initInputs)
{
    this->inputs = inputs;
    this->outputs = outputs;
    this->fitness = 0;

    if (initInputs)
    {
        initialize();
    }
    // initialize inputs and outputs
}

void Genome::initialize()
{
    for (int i = 0; i < inputs; i++)
    {
        nodes.push_back(new NodeGene(i, INPUT));
    }
    for (int i = 0; i < outputs; i++)
    {
        nodes.push_back(new NodeGene(i + inputs, OUTPUT));
    }

    // initialize links
    int idx = 0;
    for (int i = 0; i < inputs; i++)
    {
        for (int j = 0; j < outputs; j++)
        {
            LinkGene *newLink = new LinkGene(nodes[j + inputs], nodes[i], randDouble(-1, 1), idx++);
            nodes[i]->addToLink(*newLink);
            links.push_back(newLink);
        }
    }
}

double Genome::getFitness()
{
    return fitness;
}

double Genome::distance(const Genome &other)
{
    // calculate the distance between two genomes
    int s1 = this->links.size();
    int s2 = other.links.size();

    int p1 = 0;
    int p2 = 0;

    int excess = 0;
    int similar = 0;
    int disjoint = 0;
    double weightDiff = 0;

    if (s1 > 0 && s2 > 0)
    {
        while (p1 < s1 && p2 < s2)
        {
            LinkGene *conn1 = this->links[p1];
            LinkGene *conn2 = other.links[p2];

            if (conn1->getID() == conn2->getID())
            {
                similar++;
                weightDiff += std::abs(conn1->getWeight() - conn2->getWeight());
                p1++;
                p2++;
            }
            else if (conn1->getID() < conn2->getID())
            {
                disjoint++;
                p1++;
            }
            else
            {
                disjoint++;
                p2++;
            }
        }

        excess = std::max(s1 - p1, s2 - p2);
        weightDiff /= similar;
    }
    else if (s1 > 0)
    {
        excess = s1;
    }
    else
    {
        excess = s2;
    }

    double N = std::max(s1, s2);
    double distance = ((Config::excessCoefficient * excess + N + Config::disjointCoefficient * disjoint) / N) + Config::weightDiffCoefficient * weightDiff;
    return distance;
}

Genome *Genome::crossGenomes(const Genome &dominant, const Genome &recessive)
{
    Genome *child = new Genome(dominant.inputs, dominant.outputs, false);
    std::unordered_map<int, NodeGene *> nodeMap;

    // cross nodes
    for (const auto &dominantNeuron : dominant.nodes)
    {
        int nodeID = dominantNeuron->getID();
        NodeGene *recessiveNeuron = recessive.findNode(nodeID);
        if (recessiveNeuron == nullptr)
        {
            NodeGene *newNode = new NodeGene(*dominantNeuron);
            child->nodes.push_back(newNode);
            nodeMap.insert({nodeID, newNode});
        }
        else
        {
            NodeGene *newNode = crossNeurons(*dominantNeuron, *recessiveNeuron);
            child->nodes.push_back(newNode);
            nodeMap.insert({nodeID, newNode});
        }
    }

    // cross links
    for (const auto &dominantLink : dominant.links)
    {
        int linkID = dominantLink->getID();
        LinkGene *recessiveLink = recessive.findLink(linkID);
        if (recessiveLink == nullptr)
        {
            int id1 = dominantLink->getFromNode()->getID();
            int id2 = dominantLink->getToNode()->getID();
            NodeGene *fromNode = nodeMap[id1];
            NodeGene *toNode = nodeMap[id2];

            LinkGene *newLink = new LinkGene(toNode, fromNode, dominantLink->getWeight(), linkID);
            fromNode->addToLink(*newLink);
            child->links.push_back(newLink);
        }
        else
        {
            NodeGene *fromNode = nodeMap[dominantLink->getFromNode()->getID()];
            NodeGene *toNode = nodeMap[dominantLink->getToNode()->getID()];

            double weight = randNumber(2) == 0 ? dominantLink->getWeight() : recessiveLink->getWeight();
            bool enabled = randNumber(2) == 0 ? dominantLink->isEnabled() : recessiveLink->isEnabled();
            LinkGene *newLink = new LinkGene(toNode, fromNode, weight, linkID);
            newLink->setEnabled(enabled);
            fromNode->addToLink(*newLink);
            child->links.push_back(newLink);
        }
    }
    return child;
}

vector<float> Genome::activate(vector<double> inputs)
{
    vector<float> genomeOutputs;
    int outIdx = 0;
    if (inputs.size() != this->inputs)
    {
        throw std::runtime_error("Invalid input size " + std::to_string(inputs.size()) + " expected " + std::to_string(this->inputs));
    }

    // idk I saw other people do this
    for (auto node : nodes)
    {
        node->resetAccumalator();
    }

    for (int i = 0; i < this->nodes.size(); i++)
    {
        NodeGene *node = this->nodes[i];
        if (this->nodes[i]->getType() == INPUT)
        {
            // set activation to input
            node->output = inputs[i];
        }
        else if (node->getType() == HIDDEN)
        {
            // activate the node
            node->activate();
        }

        for (auto link : node->getToLinks())
        {
            NodeGene *toNode = link->getToNode();
            toNode->addAccumalator(link->getWeight() * node->output);
        }

        if (node->getType() == OUTPUT)
        {
            genomeOutputs.push_back(node->activate());
        }
    }
    return genomeOutputs;
}

vector<LinkGene *> Genome::getLinks()
{
    return links;
}

vector<NodeGene *> Genome::getNodes()
{
    return nodes;
}

void Genome::mutate()
{
    if (randDouble(0, 1) < Config::mutateNodeProbability)
    {
        addNode();
    }
    else if (randDouble(0, 1) < Config::mutateNodeProbability)
    {
        removeNode();
    }
    if (randDouble(0, 1) < Config::mutateLinkProbability)
    {
        addLink();
    }
    else if (randDouble(0, 1) < Config::mutateLinkProbability)
    {
        removeLink();
    }
    if (randDouble(0, 1) < Config::mutateEnableLinkProbability)
    {
        toggleWeight();
    }
    if (randDouble(0, 1) < Config::mutateWeightShiftProbability)
    {
        weightShift();
    }
    if (randDouble(0, 1) < Config::mutateWeightRandomProbability)
    {
        weightRandom();
    }
    if (randDouble(0, 1) < Config::mutateBiasShiftProbability)
    {
        shiftBias();
    }
}

void Genome::addNode()
{
    // get a random link
    int linkIndex = randNumber(links.size() - 1);
    LinkGene *link = links[linkIndex];
    // disable the link
    link->setEnabled(false);
    // create a new node
    int nodeIdx = 0;
    for (int i = 0; i < nodes.size(); i++)
    {
        if (link->getToNode()->getID() == nodes[i]->getID())
        {
            nodeIdx = i;
            break;
        }
    }
    int idx = std::min(nodeIdx, (int)(nodes.size() - outputs));
    NodeGene *newNode = new NodeGene(HIDDEN, 1);
    // create two new links
    LinkGene *toNewNode = new LinkGene(newNode, link->getFromNode(), link->getWeight());
    LinkGene *fromNewNode = new LinkGene(link->getToNode(), newNode, 1);
    links.push_back(toNewNode);
    links.push_back(fromNewNode);
    // add the new node
    nodes.insert(nodes.begin() + idx, newNode);
    newNode->addToLink(*fromNewNode);
    link->getFromNode()->addToLink(*toNewNode);
}

void Genome::removeNode()
{
    // get a random node
    if (nodes.size() <= inputs + outputs)
    {
        return;
    }

    // node index can only be hidden
    int nodeIndex = randNumber(nodes.size() - outputs - inputs);
    nodeIndex += inputs;

    NodeGene *nodeToDelete = nodes[nodeIndex];

    // remove all links to and from the node
    for (auto it = links.begin(); it != links.end();)
    {
        LinkGene *link = *it;
        if (link->getFromNode() == nodeToDelete)
        {
            it = links.erase(it);
            delete link;
        }
        else if (link->getToNode() == nodeToDelete)
        {
            it = links.erase(it);
            link->getFromNode()->removeLink(*link);
            delete link;
        }
        else
        {
            it++;
        }
    }
    // remove the node
    delete nodeToDelete;
    nodes.erase(nodes.begin() + nodeIndex);
}

void Genome::addLink()
{
    // from node can be inputs or hidden. to nodes can be hidden or outputs
    int fromNodeIndex = randNumber(nodes.size() - outputs);
    int toNodeIndex = randNumber(nodes.size() - inputs) + inputs;

    NodeGene *fromNode = nodes[fromNodeIndex];
    NodeGene *toNode = nodes[toNodeIndex];
    // check if the link already exists
    for (LinkGene *link : links)
    {
        if (link->getFromNode() == fromNode && link->getToNode() == toNode)
        {
            link->setEnabled(true);
            return;
        }
    }

    if (containsCycle(fromNode->getID()))
    {
        return;
    }

    // create the link
    LinkGene *newLink = new LinkGene(toNode, fromNode, randDouble(Config::min, Config::max));
    fromNode->addToLink(*newLink);
    links.push_back(newLink);
}

void Genome::removeLink()
{
    // get a random link
    if (links.size() == 0)
    {
        return;
    }
    int linkIndex = randNumber(links.size());
    LinkGene *link = links[linkIndex];

    //prevents input and output links from being removed
    if (link->getFromNode()->getType() == INPUT && link->getToNode()->getType() == OUTPUT)
    {
        return;
    }
    link->getFromNode()->removeLink(*link);
    delete links[linkIndex];
    links.erase(links.begin() + linkIndex);
}

void Genome::weightShift()
{
    LinkGene *link = getRandomLink();
    double adjustWeight = randDouble(-1, 1) + link->getWeight();
    adjustWeight = std::min(adjustWeight, Config::max);
    adjustWeight = std::max(adjustWeight, Config::min);
    link->setWeight(adjustWeight);
}

void Genome::toggleWeight()
{
    LinkGene *link = getRandomLink();
    link->setEnabled(!link->isEnabled());
}

void Genome::weightRandom()
{
    LinkGene *link = getRandomLink();
    link->setWeight(randDouble(-1, 1));
}

void Genome::shiftBias()
{
    NodeGene *node = getRandomNode();
    double adjustBias = (randDouble(-1, 1) * Config::weightShiftStrength) + node->getBias();
    adjustBias = std::min(adjustBias, Config::max);
    adjustBias = std::max(adjustBias, Config::min);
    node->setBias(adjustBias);
}

NodeGene *Genome::crossNeurons(const NodeGene &lhs, const NodeGene &rhs)
{
    int id = lhs.getID();
    double bias = randNumber(2) == 0 ? lhs.getBias() : rhs.getBias();

    auto newNode = new NodeGene(id, lhs.getType(), bias);
    return newNode;
}

// CHANGE ALL NODES VECTOR TO AN UNORDERED MAP FOR FASTER ACCESS TIMES AND EASIER NODE DELETION
LinkGene *Genome::crossLinks(const LinkGene &lhs, const LinkGene &rhs)
{
    double weight = randNumber(2) == 0 ? lhs.getWeight() : rhs.getWeight();
    bool enabled = randNumber(2) == 0 ? lhs.isEnabled() : rhs.isEnabled();
    auto child = new LinkGene(lhs.getFromNode(), lhs.getToNode(), weight, lhs.getID());
    child->setEnabled(enabled);
    return child;
}

bool Genome::containsCycle(int fromNode)
{
    std::stack<NodeGene *> dfs;
    std::unordered_set<NodeGene *> visited;
    dfs.push(nodes[0]);

    while (dfs.size() > 0)
    {
        NodeGene *current = dfs.top();
        dfs.pop();
        if (visited.find(current) != visited.end())
            continue;

        visited.insert(current);
        for (const LinkGene *neighbor : current->getToLinks())
        {
            if (neighbor->getToNode()->getID() == fromNode)
            {
                return true;
            }
            dfs.push(neighbor->getToNode());
        }
    }
    return false;
}

NodeGene *Genome::getRandomNode()
{
    int range = nodes.size() - inputs - outputs;
    return nodes[randNumber(range) + inputs];
}

LinkGene *Genome::getRandomLink()
{
    return links[randNumber(links.size())];
}

NodeGene *Genome::findNode(int nodeID) const
{
    for (const auto &node : nodes)
    {
        if (node->getID() == nodeID)
        {
            return node;
        }
    }
    return nullptr;
}

LinkGene *Genome::findLink(int linkID) const
{
    for (const auto &link : links)
    {
        if (link->getID() == linkID)
        {
            return link;
        }
    }
    return nullptr;
}

bool Genome::operator<(const Genome &other)
{
    return this->fitness < other.fitness;
}

bool Genome::operator>(const Genome &other)
{
    return this->fitness > other.fitness;
}