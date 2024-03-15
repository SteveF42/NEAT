#ifndef LinkGene_HPP
#define LinkGene_HPP

class LinkGene
{
public:
    LinkGene(int inNode, int outNode, double weight);

private:
    int LinkeID;
    int inNode;
    int outNode;
    double weight;
    bool enabled;
};

#endif