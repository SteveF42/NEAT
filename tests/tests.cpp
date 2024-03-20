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