#include "Genome.hpp"
#include "random"
#include <stack>
#include <unordered_set>

Genome::Genome(int inputs, int outputs)
{
    this->inputs = inputs;
    this->outputs = outputs;
    this->fitness = 0;
    // initialize inputs and outputs
    for (int i = 0; i < inputs; i++)
    {
        nodes.push_back(new NodeGene(i, 0, INPUT));
    }
    for (int i = 0; i < outputs; i++)
    {
        nodes.push_back(new NodeGene(i + inputs, 0, OUTPUT));
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

Genome Genome::crossGenomes(const Genome &rhs)
{
    Genome child(inputs, outputs);

    return child;
}

vector<LinkGene*> Genome::getLinks()
{
    return links;
}

vector<NodeGene*> Genome::getNodes()
{
    return nodes;
}

void Genome::mutate()
{
    // mutate 50% of the time
    int mutation = rand() % 4;
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
    int linkIndex = rand() % (links.size() - 1);
    LinkGene *link = links[linkIndex];
    // disable the link
    link->setEnabled(false);
    // create a new node
    int nodeIdx = 0;
    for(int i = 0; i < nodes.size(); i++)
    {
        if(link->getToNode()->getID() == nodes[i]->getID())
        {
            nodeIdx = i;
            break;
        }
    }
    int idx = std::min(nodeIdx, (int)(nodes.size() - outputs));
    NodeGene *newNode = new NodeGene(nodes.size(), 1, HIDDEN);
    // create two new links
    LinkGene *fromLink = new LinkGene(newNode, link->getFromNode(), 1);
    LinkGene *toLink = new LinkGene(link->getToNode(), newNode, link->getWeight());
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
    int nodeIndex = rand() % (nodes.size() - outputs - inputs);
    nodeIndex += inputs;

    NodeGene *nodeToDelete = nodes[nodeIndex];

    // remove all links to and from the node
    for (auto it = links.begin(); it != links.end();)
    {
        if ((*it)->getFromNode() == nodeToDelete)
        {
            it = links.erase(it);
        }
        else if ((*it)->getToNode() == nodeToDelete)
        {
            it = links.erase(it);
            (*it)->getFromNode()->removeLink(**it);
        }
        else
        {
            it++;
        }
    }
    // remove the node
    std::cout << "Link Size: " << links.size() << "\n";
    nodes.erase(nodes.begin() + nodeIndex);
}

void Genome::addLink()
{
    // from node can be inputs or hidden. to nodes can be hidden or outputs
    int fromNodeIndex = rand() % (nodes.size() - outputs);
    int toNodeIndex = (rand() % outputs) + inputs;

    NodeGene *fromNode = nodes[fromNodeIndex];
    NodeGene *toNode = nodes[toNodeIndex];
    // check if the link already exists
    for (LinkGene* link : links)
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
    int linkIndex = rand() % links.size();
    LinkGene *link = links[linkIndex];
    link->getFromNode()->removeLink(*link);
    links.erase(links.begin() + linkIndex);
}

NodeGene Genome::crossNeurons(const NodeGene &lhs, const NodeGene &rhs)
{
    int id = lhs.getID();
    double bias = rand() % 2 == 0 ? lhs.getBias() : rhs.getBias();
    return NodeGene(id, bias, lhs.getType());
}

LinkGene Genome::crossLinks(const LinkGene &lhs, const LinkGene &rhs)
{
    double weight = rand() % 2 == 0 ? lhs.getWeight() : rhs.getWeight();
    bool enabled = rand() % 2 == 0 ? lhs.isEnabled() : rhs.isEnabled();
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