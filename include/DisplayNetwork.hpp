#ifndef DISPLAYNETWORK_HPP
#define DISPLAYNETWORK_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <atomic>
#include <thread>
#include "Neat.hpp"

using std::thread;
using std::unique_ptr;
typedef unique_ptr<sf::RenderWindow> WindowPtr;
typedef unique_ptr<thread> ThreadPtr;

struct point
{
    double x;
    double y;
};

class DisplayNetwork
{
private:
    WindowPtr window;
    Neat *neatEngine;
    ThreadPtr windowThread;
    std::mutex mtx;
    int TARGET_WIDTH = 800;
    int TARGET_HEIGHT = 800;
    bool running = false;
    void draw();
    void update();
    void drawLayer(networkLayer &layer, int layerIdx, int offsetX);
    void drawLinks();
    void run();
    map<int, point> nodePositions;

public:
    ~DisplayNetwork();
    DisplayNetwork(Neat &neatEngine);
    DisplayNetwork(Neat &neatEngine, int width, int height);
    void createWindowThread();
    void killWindowThread();
    void joinWindow();
};

#endif