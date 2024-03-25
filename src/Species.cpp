#include "Species.hpp"
#include "Util.hpp"
#include <algorithm>

double Species::getScore()
{
    return score;
}

int Species::getSpeciesSize()
{
    return members.size();
}

void Species::setLeader(Genome *leader)
{
    this->leader = leader;
}

void Species::setScore(double score)
{
    this->score = score;
}

void Species::addMember(Genome *member)
{
}

void Species::kill(double percentage)
{
    int numToKill = static_cast<int>(percentage * members.size());
    // Sort members based on fitness
    std::sort(members.begin(), members.end(), [](Genome *a, Genome *b)
              { return a->getFitness() > b->getFitness(); });
    // Remove the lowest performing genomes
    members.erase(members.begin() + numToKill, members.end());
}

void Species::evaluateScore()
{
    double total = 0;
    for (Genome *g : members)
    {
        total += g->getFitness();
    }
    score = total / members.size();
}

Genome *Species::breed()
{
    int size = members.size();
    Genome *g1 = members[randNumber(size)];
    Genome *g2 = members[randNumber(size)];
    return Genome::crossGenomes(*g1, *g2);
}

void Species::clear()
{
    members.clear();
}