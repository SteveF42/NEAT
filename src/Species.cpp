#include "Species.hpp"
#include "Util.hpp"
#include <algorithm>

Genome *Species::getLeader() const
{
    return leader;
}
double Species::getScore()
{
    return score;
}

int Species::getSpeciesSize()
{
    return members.size();
}

vector<Genome *> Species::getMembers() const
{
    return members;
}

void Species::setLeader(Genome *leader)
{
    members.push_back(leader);
    this->leader = leader;
}

void Species::setScore(double score)
{
    this->score = score;
}

void Species::addMember(Genome *member)
{
    members.push_back(member);
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
    int numTopPerformers = static_cast<int>(members.size() * Config::topPerformerPercentage);
    Genome *g1 = members[randNumber(numTopPerformers)];
    Genome *g2 = members[randNumber(numTopPerformers)];

    if (g1->getFitness() > g2->getFitness())
        return Genome::crossGenomes(*g1, *g2);
    else
        return Genome::crossGenomes(*g2, *g1);
}

void Species::clear()
{
    members.clear();
}

void Species::setNumOfChildren(int num)
{
    numOfChildren = num;
}

int Species::getNumOfChildren()
{
    return numOfChildren;
}