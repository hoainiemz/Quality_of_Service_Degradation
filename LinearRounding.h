//
// Created by hoainiem on 5/16/25.
//

#ifndef LINEARROUNDING_H
#define LINEARROUNDING_H

#include "pch.h"
#include "Grapher.h"

class Ellipsoid {
public:
    //the center
    torch::Tensor u;

    // the matrix
    torch::Tensor H;

    torch::Tensor pot;

    static double logeps;

    // number of dimensions
    int n;

    Ellipsoid(torch::Tensor u, torch::Tensor H) {
        this->u = u;
        this->H = H;
        this->n = u.size(0);
        torch::Tensor pi = torch::tensor(M_PI).to(torch::kFloat64).requires_grad_(false);
        this->pot = torch::log(pi) * n / 2 - torch::lgamma(torch::tensor(n * 1.0 / 2 + 1).to(torch::kFloat64).requires_grad_(false));
    }

    Ellipsoid(torch::Tensor u, double bound) {
        // std::cout << u << std::endl;
        this->u = torch::zeros_like(u, torch::TensorOptions().dtype(torch::kFloat64).requires_grad(false));
        this->H = torch::eye(u.size(0), torch::TensorOptions().dtype(torch::kFloat64).requires_grad(false)) * (bound * bound);
        n = u.size(0);
        torch::Tensor pi = torch::tensor(M_PI).to(torch::kFloat64).requires_grad_(false);
        this->pot = torch::log(pi) * n / 2 - torch::lgamma(torch::tensor(n * 1.0 / 2 + 1).to(torch::kFloat64).requires_grad_(false));
    }


    Ellipsoid(std::vector<double> u, double bound)
        : Ellipsoid(torch::tensor(u).to(torch::kFloat64).requires_grad_(false).view({-1, 1}), bound) {}

    Ellipsoid(std::vector<int> u, double bound)
        : Ellipsoid(torch::tensor(u).to(torch::kFloat64).requires_grad_(false).view({-1, 1}), bound) {}

    double longestAxisLength();

    double getLogVolume();

    double getCenterNorm();

    bool volumeCheck();

    std::vector<double> getCenter() {
        return std::vector<double>(u.data_ptr<double>(), u.data_ptr<double>() + u.numel());
    }

    void update(std::vector<int> u);
};

class LinearRounding {
    static std::vector<double> EllipsoidAlgorithm(Grapher &graph);
    static std::vector<double> EllipsoidStep(Grapher &graph, double mid);
public:
    static std::vector<int> solve(Grapher &graph);
};



#endif //LINEARROUNDING_H
