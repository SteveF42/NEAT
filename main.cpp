#include <iostream>
#include <vector>
#include "Genome.hpp"
#include "XOR.hpp"

int main()
{
    Xor xorGame;
    xorGame.initialize(500);
    xorGame.train(100);
    xorGame.test();
    return 0;
}