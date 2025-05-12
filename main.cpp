
#include <iostream>
#include <fstream>

#include "AdaptiveTrading.h"
#include "Grapher.h"
#include "IterativeGreedy.h"

Grapher graph;

int main() {;
    // graph.read("../data/p2p-Gnutella04.mtx", false);
    std::pair<int, int> gr = graph.randomize(30, Constants::edgeDensity);
    graph.write("../output/test.txt");
    // std::ifstream file("../output/test.txt");
    // file >> graph;

    std::vector<int> vt1 = IterativeGreedy::solve(graph, 1);
    std::cout << "Iterative Greedy type 1: " << vt1 << '\n' << "Norm = " << graph.getNorm(vt1) << '\n';
    std::vector<int> vt2 = IterativeGreedy::solve(graph, 2);
    std::cout << "Iterative Greedy type 2: " << vt2 << '\n' << "Norm = " << graph.getNorm(vt2) << '\n';


    return 0;
}