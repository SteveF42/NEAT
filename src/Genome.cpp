#include "Genome.hpp"
#include "Util.hpp"
#include "random"
#include <stack>
#include <unordered_set>

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
        nodes.push_back(new NodeGene(1, INPUT));
    }
    for (int i = 0; i < outputs; i++)
    {
        nodes.push_back(new NodeGene(0, OUTPUT));
    }

    // initialize links
    for (int i = 0; i < inputs; i++)
    {
        for (int j = 0; j < outputs; j++)
        {
            links.push_back(new LinkGene(nodes[j + inputs], nodes[i], 1));
        }
    }
}

float Genome::getFitness()
{
    return fitness;
}

Genome* Genome::crossGenomes(const Genome &dominant, const Genome &recessive)
{
    Genome *child = new Genome(dominant.inputs, dominant.outputs, false);

    // cross nodes
    for (const auto &dominantNeuron : dominant.nodes)
    {
        int nodeID = dominantNeuron->getID();
        NodeGene *recessiveNeuron = recessive.findNode(nodeID);
        if (recessiveNeuron == nullptr)
        {
            child->nodes.push_back(new NodeGene(*dominantNeuron));
        }
        else
        {
            NodeGene newNode = crossNeurons(*dominantNeuron, *recessiveNeuron);
            child->nodes.push_back(new NodeGene(newNode));
        }
    }

    // cross links
    for (const auto &dominantLink : dominant.links)
    {
        int linkID = dominantLink->getID();
        LinkGene *recessiveLink = recessive.findLink(linkID);
        if (recessiveLink == nullptr)
        {
            child->links.push_back(new LinkGene(*dominantLink));
        }
        else
        {
            LinkGene newLink = crossLinks(*dominantLink, *recessiveLink);
            child->links.push_back(new LinkGene(newLink));
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
    // mutate 50% of the time
    int mutation = randNumber(4);
    if (mutation == 0)
    {
        addNode();
    }
    else if (mutation == 1)
    {
        removeNode();
    }
    else if (mutation == 2)
    {
        addLink();
    }
    else if (mutation == 3)
    {
        removeLink();
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
    NodeGene *newNode = new NodeGene(1, HIDDEN);
    // create two new links
    LinkGene *fromLink = new LinkGene(newNode, link->getFromNode(), link->getWeight());
    LinkGene *toLink = new LinkGene(link->getToNode(), newNode, 1);
    links.push_back(fromLink);
    links.push_back(toLink);
    // add the new node
    nodes.insert(nodes.begin() + idx, newNode);
    newNode->addToLink(*fromLink);
    link->getFromNode()->addToLink(*toLink);
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
        if ((*it)->getFromNode() == nodeToDelete)
        {
            delete *it;
            it = links.erase(it);
        }
        else if ((*it)->getToNode() == nodeToDelete)
        {
            it = links.erase(it);
            (*it)->getFromNode()->removeLink(**it);
            delete *it;
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
    int toNodeIndex = randNumber(outputs) + inputs;

    NodeGene *fromNode = nodes[fromNodeIndex];
    NodeGene *toNode = nodes[toNodeIndex];
    // check if the link already exists
    for (LinkGene *link : links)
    {
        if (link->getFromNode()->getID() == fromNode->getID() && link->getToNode()->getID() == toNode->getID())
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
    LinkGene *newLink = new LinkGene(toNode, fromNode, 1);
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
    link->getFromNode()->removeLink(*link);
    delete links[linkIndex];
    links.erase(links.begin() + linkIndex);
}

NodeGene Genome::crossNeurons(const NodeGene &lhs, const NodeGene &rhs)
{
    int id = lhs.getID();
    double bias = randNumber(2) == 0 ? lhs.getBias() : rhs.getBias();
    return NodeGene(bias, lhs.getType());
}

LinkGene Genome::crossLinks(const LinkGene &lhs, const LinkGene &rhs)
{
    double weight = randNumber(2) == 0 ? lhs.getWeight() : rhs.getWeight();
    bool enabled = randNumber(2) == 0 ? lhs.isEnabled() : rhs.isEnabled();
    LinkGene child(lhs.getFromNode(), lhs.getToNode(), weight);
    child.setEnabled(enabled);
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