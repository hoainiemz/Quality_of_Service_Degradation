//
// Created by hoainiem on 5/9/25.
//

#ifndef ITERATIVEGREEDY_H
#define ITERATIVEGREEDY_H
#include "Grapher.h"


class IterativeGreedy {
    static std::vector<int> greedyBlockingPath(Grapher &graph, std::vector<Path> &pathSet);

public:
    static std::vector<int> solve(Grapher &graph, int type);
};



#endif //ITERATIVEGREEDY_H
