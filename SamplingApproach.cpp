//
// Created by hoainiem on 5/12/25.
//

#include "SamplingApproach.h"

std::vector<int> SamplingApproach::solve(Grapher &graph)  {
    std::vector<int> x(graph.getM(), 0);
    std::vector<std::pair<Path, long double> > P;
    while (true) {
        P = graph.samplingPath(x);
        if (P.empty()) {
            break;
        }
        greedyAlgorithm(graph, P, x, Constants::q);
    }
    return x;
}

void SamplingApproach::greedyAlgorithm(Grapher &graph, std::vector<std::pair<Path, long double> > &P, std::vector<int> &x, int q) {
    std::vector<bool> checked(graph.getM(), false);
    for (int i = 0; i < P.size(); i++) {
        for (int j : P[i].first) {
            checked[j] = true;
        }
    }
    while (q--) {
        int pos = -1;
        long double res = 0;
        for (int i = graph.getM() - 1; i >= 0; i--) {
            if (!checked[i] || x[i] == graph.getEdge(i).getWeight().size() - 1) {
                continue;
            }
            long double pot = 0;
            std::vector<int> newx = addition(x, i, 1);
            for (int j = 0; j < P.size(); j++) {
                int tmp = graph.budgetFunction(P[j].first, newx);
                pot += tmp / (P[j].second);
            }
            if (pot > res) {
                res = pot;
                pos = i;
            }
        }
        if (pos < 0) {
            break;
        }
        x = addition(x, pos, 1);
    }
}

