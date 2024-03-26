#include "Neat.hpp"

Neat::Neat(int input, int output)
{
    this->input = input;
    this->output = output;
    this->population = 0;
}

void Neat::initialize(int population)
{
    for(int i = 0; i < population; i++)
    {
        allGenomes.push_back(new Genome(input, output, true));
    }
}

void Neat::evolve()
{
    speciate();
    kill();
    breed();
    mutate();
}