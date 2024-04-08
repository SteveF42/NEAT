#include "DisplayNetwork.hpp"
#include <memory>

using std::make_unique;

DisplayNetwork::DisplayNetwork(Neat &neatEngine)
{
    this->neatEngine = &neatEngine;
    this->window = make_unique<sf::RenderWindow>(sf::VideoMode(TARGET_WIDTH, TARGET_HEIGHT), "Neat");
}

DisplayNetwork::DisplayNetwork(Neat &neatEngine, int width, int height)
{
    this->neatEngine = &neatEngine;
    this->TARGET_HEIGHT = height;
    this->TARGET_WIDTH = width;
    this->window = make_unique<sf::RenderWindow>(sf::VideoMode(TARGET_WIDTH, TARGET_HEIGHT), "Neat");
}

void DisplayNetwork::draw()
{
    this->window->clear();
    this->window->display();
}

void DisplayNetwork::update()
{
    this->draw();
}

void DisplayNetwork::run()
{

    thread eventThread = thread(
        [this]()
        {
            sf::Event event;
            while (this->window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    this->window->close();
            }
        });
    while (this->window->isOpen())
    {
        this->draw();
    }
}

thread DisplayNetwork::displayThread()
{
    window->setActive(false);
    return thread(&DisplayNetwork::run, this);
}