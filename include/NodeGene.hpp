#ifndef Node_HPP
#define Node_HPP

#include "ActivationRelu.hpp"

class NodeGene{
public:
    NodeGene(int id, double bias);
    int getID();
    double getBias();
    void setBias(double bias);
private:
    int id;
    double bias;
    ActivationRelu activation;
};



#endif