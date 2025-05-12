//
// Created by hoainiem on 5/9/25.
//

#include "IterativeGreedy.h"

#include "IterativeSolution.h"

std::vector<int> IterativeGreedy::greedyBlockingPath(Grapher &graph, std::vector<Path> &pathSet) {
    std::vector<int> x(graph.getM(), 0);
    std::vector<bool> checked(graph.getM(), false);
    for (Path &path : pathSet) {
        for (int i : path) {
            checked[i] = true;
        }
    }
    long long obj = 1LL * Constants::T * pathSet.size();
    long long pot = graph.budgetFunction(pathSet, x);
    while (obj != pot) {
        long long res = 0;
        int pos = 0;
        for (int i = graph.getM() - 1; i >= 0; i--) {
            if (!checked[i] || x[i] == graph.getEdge(i).getWeight().size() - 1) {
                continue;
            }
            long long newpot = graph.budgetFunction(pathSet, addition(x, i, 1));
            if (newpot == pot) {
                checked[i] = false;
            }
            else {
                if (res < newpot - pot) {
                    res = newpot - pot;
                    pos = i;
                }
            }
        }
        x = addition(x, pos, 1);
        pot += res;
    }
    return x;
}

std::vector<int> IterativeGreedy::solve(Grapher &graph, int type) {
    return IterativeSolution::solve(graph, greedyBlockingPath, type);
}
