#ifndef DISPLAYNETWORK_HPP
#define DISPLAYNETWORK_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <thread>
#include "Neat.hpp"

using std::unique_ptr;
using std::thread;
typedef unique_ptr<sf::RenderWindow> WindowPtr;

class DisplayNetwork
{
private:
    WindowPtr window;
    Neat *neatEngine;
    thread eventThread;
    int TARGET_WIDTH = 800;
    int TARGET_HEIGHT = 800;
    void draw();
    void update();
    void run();

public:
    DisplayNetwork(Neat &neatEngine);
    DisplayNetwork(Neat &neatEngine, int width, int height);
    thread displayThread();
};

#endif