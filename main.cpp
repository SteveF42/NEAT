#include <iostream>
#include <vector>
#include "Genome.hpp"
#include "XOR.hpp"
#include "DisplayNetwork.hpp"
#include <thread>

int main()
{
    Xor xorGame;
    DisplayNetwork displayNetwork(xorGame, 800, 800);
    thread displayThread = displayNetwork.displayThread();
    xorGame.initialize(500);
    xorGame.train(100);
    xorGame.test();
    displayThread.join();

    return 0;
}