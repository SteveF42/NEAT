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
    thread displayThread = thread([&xorGame]()
    {
        DisplayNetwork display(xorGame);
        display.run();
    });
    xorGame.initialize(500);
    xorGame.train(100);
    xorGame.test();
    displayThread.join();

    return 0;
}