#ifndef LinkGene_HPP
#define LinkGene_HPP

class LinkGene
{
public:
    LinkGene(int inNode, int outNode, double weight);

private:
    int LinkeID;
    int fromNode;
    int toNode;
    double weight;
    bool enabled;
};

#endif