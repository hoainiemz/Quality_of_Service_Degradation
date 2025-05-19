//
// Created by hoainiem on 5/7/25.
//

#include <string>
#include <vector>
#include <assert.h>
#include "Constants.h"
#include "MyRandom.h"

#ifndef GRAPHER_H
#define GRAPHER_H

typedef std::vector<int> Path;

class Edge {
    int u, v;
    std::vector<int> weight;
public:

    void makeWeighted();

    Edge() {
        u = -1;
        v = -1;
    }

    Edge(int id, int u, int v) {
        this->u = u;
        this->v = v;
        makeWeighted();
    }

    int getU() {
        return u;
    }

    int getV() {
        return v;
    }

    std::vector<int> &getWeight() {
        return weight;
    }

    int getWeight(int i) {
        return weight[i];
    }

    double getLinearWeight(double x) {
        return x * (weight[1] - weight[0]) + weight[0];
    }

    int getLinearTan() {
        return weight[1] - weight[0];
    }

    int from(int x);

    friend std::istream& operator>>(std::istream& in, Edge& p);
    friend std::ostream& operator<<(std::ostream& os, const Edge& p);
};

class Grapher {
private:
    int n, m;
    std::vector<std::vector<int> > l; // edgeset
    std::vector<std::pair<int, int> > entries; // Arces
    std::vector<Edge> edges; // edges
    std::vector<std::pair<int, int>> pathQueries; // the S set

    void makeWeighted();

    int maximumDegree();
public:
    Grapher() {
        n = 0;
        m = 0;
    }
    std::pair<std::vector<int>, std::vector<int> > dijkstra(int x, std::vector<int> cost = std::vector<int>());

    std::pair<std::vector<double>, std::vector<int> > dijkstra(double x, std::vector<double> cost = std::vector<double>());

    Path getUnblockedPath(std::vector<int> &cost);

    std::vector<int> getB() {
        std::vector<int> res(m, 0);
        for (int i = 0; i < m; i++) {
            res[i] = edges[i].getWeight().size() - 1;
        }
        return res;
    }

    void genPathQueries();

    std::pair<int, int> read(std::string &filename);

    int write(std::string filename);

    std::pair<int, int> randomize(int n = 240, double density = Constants::edgeDensity);

    Path getPotentialPath(int u, int v, std::vector<int> &cost);

    std::vector<Path> getPotentialPaths(std::vector<int> &cost);

    Path getUnblockedPath(std::vector<double> &cost);

    int getLinearMaxSlope();

    bool isFeasible(std::vector<int> &cost);

    int getM() {
        return m;
    }

    int getN() {
        return n;
    }

    int getH() {
        return Constants::T;
    }

    Edge &getEdge(int i) {
        return edges[i];
    }

    std::vector<std::pair<int, int> > getPathQueries() {
        return pathQueries;
    }

    int getNorm(std::vector<int> x);

    long long budgetFunction(std::vector<Path> &paths, std::vector<int> cost);

    long long budgetFunction(Path path, std::vector<int> cost);

    std::vector<std::pair<Path, double> > samplingPath(std::vector<int> &cost);

    int getNumSampling();

    friend std::istream& operator>>(std::istream& in, Grapher& obj);
    friend std::ostream& operator<<(std::ostream& os, const Grapher& obj);
};

std::ostream& operator<<(std::ostream& os, const Edge& p);

std::ostream& operator<<(std::ostream& os, const Grapher& grapher);

std::istream& operator>>(std::istream& in, Edge& p);

std::istream& operator>>(std::istream& in, Grapher& grapher);

std::vector<int> addition(std::vector<int> a, int idx, int val = 1);

std::ostream& operator<<(std::ostream& os, const std::vector<int>& p);

#endif //GRAPHER_H