//
// Created by hoainiem on 5/12/25.
//

#ifndef SA_H
#define SA_H
#include "Grapher.h"


class SamplingApproach {
    static void greedyAlgorithm(Grapher &graph, std::vector<std::pair<Path, long double> > &P, std::vector<int> &x, int q);
public:
    static std::vector<int> solve(Grapher &graph);
};



#endif //SA_H
