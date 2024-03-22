#include <iostream>
#include "Genome.hpp"
#include "catch_amalgamated.hpp"

TEST_CASE("Genome links initialized correctly")
{
    Genome genome(2, 1);
    REQUIRE(genome.getLinks().size() == 2);
}

TEST_CASE("Add/remove node mutation")
{
    Genome genome(2, 1);

    genome.removeNode();
    REQUIRE(genome.getNodes().size() == 3);

    genome.addNode();
    REQUIRE(genome.getNodes().size() == 4);
    REQUIRE(genome.getLinks().size() == 4);

    genome.addNode();
    genome.addNode();
    genome.addNode();
    REQUIRE(genome.getNodes().size() == 7);
    REQUIRE(genome.getLinks().size() == 10);

    // its hard to tell how many links will be removed since its random
    // one node could have multiple connections
    // however removing all the nodes we added should result in the original number of connections
    genome.removeNode();
    REQUIRE(genome.getNodes().size() == 6);
    genome.removeNode();
    genome.removeNode();
    genome.removeNode();
    REQUIRE(genome.getNodes().size() == 3);
    REQUIRE(genome.getLinks().size() == 2);
}

TEST_CASE("Removing a link/ adding a link mutation")
{
    Genome genome(3, 2);
    REQUIRE(genome.getLinks().size() == 6);
    genome.removeLink();
    REQUIRE(genome.getLinks().size() == 5);
    genome.removeLink();
    genome.removeLink();
    genome.removeLink();
    genome.removeLink();
    genome.removeLink();
    REQUIRE(genome.getLinks().size() == 0);
    genome.addLink();
    REQUIRE(genome.getLinks().size() == 1);
}

TEST_CASE("Genome activation")
{
    Genome genome1(3, 2);
    Genome genome2(5, 2);
    Genome genome3(9, 4);

    vector<double> inputs1 = {1, 2, 3};
    vector<double> inputs2 = {1, 2, 3, 4, 5};
    vector<double> inputs3 = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto outputs1 = genome1.activate(inputs1);
    auto outputs2 = genome2.activate(inputs2);
    auto outputs3 = genome3.activate(inputs3);

    REQUIRE(outputs1.size() == 2);
    REQUIRE(outputs2.size() == 2);
    REQUIRE(outputs3.size() == 4);

    try
    {
        auto outputs4 = genome1.activate(inputs2);
        REQUIRE(false);
    }
    catch (const std::exception &e)
    {
        REQUIRE(true);
    }
}

TEST_CASE("Genome crossover")
{
    Genome genome1(3, 2);
    Genome genome2(3, 2);

    Genome genome3 = Genome::crossGenomes(genome1, genome2);

    REQUIRE(genome3.getNodes().size() == 5);
    REQUIRE(genome3.getLinks().size() == 6);
}