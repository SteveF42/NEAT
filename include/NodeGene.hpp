#ifndef Node_HPP
#define Node_HPP

#include "ActivationRelu.hpp"

enum NodeType{
    INPUT,
    HIDDEN,
    OUTPUT
};

class NodeGene{
public:
    NodeGene(int id, double bias);
    int getID();
    float getOutput();
    
private:
    float output;
    int id;
    NodeType type;
    double bias;
    ActivationRelu activation;
};



#endif