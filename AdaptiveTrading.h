//
// Created by hoainiem on 5/9/25.
//

#ifndef ADAPTIVETRADING_H
#define ADAPTIVETRADING_H
#include "Grapher.h"


class AdaptiveTrading {
    static std::vector<int> greedyBlockingPath(Grapher &graph, std::vector<Path> &pathSet);

public:
    static std::vector<int> solve(Grapher &graph, int type);
};

#endif //ADAPTIVETRADING_H
