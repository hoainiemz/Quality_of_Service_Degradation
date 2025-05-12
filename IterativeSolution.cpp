//
// Created by hoainiem on 5/9/25.
//

#include "IterativeSolution.h"

#include <vector>

int IterativeSolution::updatePotentialPaths(Grapher &graph, std::vector<Path> &pathSet, std::vector<int> cost, int type) {
    int res = 0;
    if (cost.empty()) {
        cost.resize(graph.getM(), 0);
    }
    if (type == 1) {
        for (auto [u, v] : graph.getPathQueries()) {
            Path path = graph.getPotentialPath(u, v, cost);
            if (path.size()) {
                pathSet.push_back(path);
                res++;
            }
        }
    }
    else {
        std::vector<Path> paths = graph.getPotentialPaths(cost);
        for (Path path : paths) {
            if (path.size()) {
                pathSet.push_back(path);
                res++;
            }
        }
    }
    return res;
}

Path IterativeSolution::solve(Grapher &graph, Path (*algorithm) (Grapher &graph, std::vector<Path> &pathSet), int type) {
    std::vector<Path> pathSet;
    std::vector<int> x(graph.getM(), 0);
    while (updatePotentialPaths(graph, pathSet, x, type)) {
        x = algorithm(graph, pathSet);
    }
    return x;
}
