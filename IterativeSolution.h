//
// Created by hoainiem on 5/9/25.
//

#ifndef ITERATIVESOLUTION_H
#define ITERATIVESOLUTION_H
#include <vector>

#include "Grapher.h"

class IterativeSolution {
    static int updatePotentialPaths(Grapher &graph, std::vector<Path> &pathSet, std::vector<int> cost, int type);
public:

    static Path solve(Grapher &graph, std::vector<int> (*algorithm) (Grapher &graph, std::vector<Path> &pathSet), int type);
};



#endif //ITERATIVESOLUTION_H
