#include <iostream>
#include <vector>
#include "Genome.hpp"
#include "XOR.hpp"

int main()
{
    Xor xorGame;
    xorGame.initialize(100);
    xorGame.train(150);
    xorGame.test();
    return 0;
}