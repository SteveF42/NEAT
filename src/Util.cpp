#include<iostream>
#include<random>
#include "Util.hpp"

int randNumber(int range)
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0,range-1);
    int randNum = dist(rd);
    return randNum;
}
