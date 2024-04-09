#ifndef DISPLAYNETWORK_HPP
#define DISPLAYNETWORK_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <thread>
#include "Neat.hpp"

using std::thread;
using std::unique_ptr;
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
    void update() const;
    void drawLayer(networkLayer &layer, int layerIdx, int offsetX) const;

public:
    DisplayNetwork(Neat &neatEngine);
    DisplayNetwork(Neat &neatEngine, int width, int height);
    void run();
};

#endif