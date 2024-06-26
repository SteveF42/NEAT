#include <iostream>
#include <vector>
#include "Genome.hpp"
#include "XOR.hpp"
#include "DisplayNetwork.hpp"
#include <thread>
#include <atomic>

int main()
{
    Xor xorGame;
    DisplayNetwork displayThread(xorGame);
    displayThread.createWindowThread();
    
    xorGame.initialize(5000);
    xorGame.train(100);
    xorGame.test();

    return 0;
}