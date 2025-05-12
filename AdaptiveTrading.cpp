//
// Created by hoainiem on 5/9/25.
//

#include "AdaptiveTrading.h"

#include "IterativeSolution.h"

std::vector<int> AdaptiveTrading::greedyBlockingPath(Grapher &graph, std::vector<Path> &pathSet) {
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
        long long res_nu = 0, res_de = 1;
        long long e = -1;
        for (int i = graph.getM() - 1; i >= 0; i--) {
            for (int j = 0; j + x[i] < graph.getEdge(i).getWeight().size(); j++) {
                long long newpot = graph.budgetFunction(pathSet, addition(x, i, j));
                if (newpot == pot) {
                    if (j == 0) {
                        checked[i] = false;
                    }
                    else {
                        break;
                    }
                }
                else {
                    if (res_nu * j < (newpot - pot) * res_de) {
                        res_nu = newpot - pot;
                        res_de = j;
                        e = i;
                    }
                }
            }
        }
        x = addition(x, e, res_de);
        pot += res_nu;
    }
    return x;
}

std::vector<int> AdaptiveTrading::solve(Grapher &graph, int type) {
    return IterativeSolution::solve(graph, greedyBlockingPath, type);
}