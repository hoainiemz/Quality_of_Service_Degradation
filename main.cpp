
#include <iostream>
#include <fstream>

#include "AdaptiveTrading.h"
#include "Grapher.h"
#include "IterativeGreedy.h"
#include "SamplingApproach.h"

Grapher graph;

int main() {;
    // graph.read("../data/p2p-Gnutella04.mtx", false);
    std::pair<int, int> gr = graph.randomize(20, Constants::edgeDensity);
    graph.write("../output/test.txt");
    // std::ifstream file("../output/test.txt");
    // file >> graph;

    std::vector<int> vt1 = IterativeGreedy::solve(graph, 1);
    std::cout << "Iterative Greedy type 1: " << vt1 << '\n' << "Norm = " << graph.getNorm(vt1) << '\n';
    std::vector<int> vt2 = IterativeGreedy::solve(graph, 2);
    std::cout << "Iterative Greedy type 2: " << vt2 << '\n' << "Norm = " << graph.getNorm(vt2) << '\n';
    std::vector<int> vt3 = AdaptiveTrading::solve(graph, 1);
    std::cout << "Adaptive Trading type 1: " << vt3 << '\n' << "Norm = " << graph.getNorm(vt3) << '\n';
    std::vector<int> vt4 = AdaptiveTrading::solve(graph, 2);
    std::cout << "Adaptive Trading type 2: " << vt4 << '\n' << "Norm = " << graph.getNorm(vt4) << '\n';

    std::vector<int> vt5 = SamplingApproach::solve(graph);
    std::cout << "Sampling Approach: " << vt5 << '\n' << "Norm = " << graph.getNorm(vt5) << '\n';
    return 0;
}