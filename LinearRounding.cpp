//
// Created by hoainiem on 5/16/25.
//

#include "LinearRounding.h"

std::vector<int> LinearRounding::solve(Grapher &graph) {
    std::vector<int> x(graph.getM(), 0);
    std::vector<double> re = EllipsoidAlgorithm(graph);
    int beta = graph.getLinearMaxSlope();
    double eta = 1.0 * beta / (1 - exp(-beta)) * (graph.getH() * log(graph.getN()) - log(0.5) + 1);
    while (true) {
        for (int i = 0; i < re.size(); i++) {
            int j = re[i];
            if (abs(j - re[i]) < Constants::eps) {
                x[i] = j;
            }
            else {
                double rho = re[i] - j;
                double p = rho * eta;
                if (p >= 1) {
                    x[i] = j + 1;
                }
                else {
                    x[i] = j + MyRandom::bernouli(p);
                }
            }
            if (x[i] > graph.getEdge(i).getWeight().size() - 1) {
                x[i] = graph.getEdge(i).getWeight().size() - 1;
            }
        }
        if (graph.isFeasible(x)) {
            break;
        }
    }
    return x;
}

int Grapher::getLinearMaxSlope()  {
    int res = 0;
    for (int i = 0; i < m; i++) {
        res = std::max(res, edges[i].getLinearTan());
    }
    return res;
}


std::vector<double> LinearRounding::EllipsoidAlgorithm(Grapher &graph) {
    int l = 1, mid, r = graph.getNorm(graph.getB());
    std::vector<double> kq;
    while (l < r) {
        mid = (l + r) / 2;
        std::vector<double> re = LinearRounding::EllipsoidStep(graph, mid);
        if (re.empty()) {
            l = mid + 1;
        }
        else {
            r = mid;
            auto re_copy = std::vector<double>(re.begin(), re.end());
            kq = std::move(re_copy);
        }
    }
    return kq;
}

std::vector<double> LinearRounding::EllipsoidStep(Grapher &graph, double mid) {
    Ellipsoid ellipsoid(graph.getB(), mid);
    std::vector<double> re;
    while (true) {
        re = ellipsoid.getCenter();
        double norm = ellipsoid.getCenterNorm();
        std::vector<int> path;
        if (abs(norm - mid) < Constants::eps || norm > mid) {
            path.resize(graph.getM());
            fill(path.begin(), path.end(), -1);
        }
        else {
            path = graph.getUnblockedPath(re);
        }
        if (path.empty()) {
            return re;
        }
        if (ellipsoid.volumeCheck() || ellipsoid.longestAxisLength() < Constants::eps) {
            return {};
        }
        // std::cout << ellipsoid.getLogVolume() << ' ' << ellipsoid.longestAxisLength() << std::endl;
        ellipsoid.update(path);
    }
    return {};
}

void Ellipsoid::update(std::vector<int> av) {
    torch::Tensor a = -torch::tensor(av).to(torch::kFloat64).requires_grad_(false).view({-1, 1});
    torch::Tensor Ha = torch::matmul(H, a);
    torch::Tensor g = a / torch::sqrt(torch::matmul(a.t(), Ha));
    u = u - 1.0 / (n + 1) * torch::matmul(H, g);
    H = 1.0 * (n * n) / (n * n - 1) * (H - torch::matmul(torch::matmul(H, torch::matmul(g, g.t())) , H)* 2.0 / (n + 1));
    u = torch::max(u, torch::zeros_like(u));
}

double Ellipsoid::getLogVolume() {
    // torch::Tensor cur = pot + torch::log(torch::det(H)) / 2;
    return (pot + std::get<1>(torch::linalg_slogdet(H)) / 2).item<double>();
}

double Ellipsoid::getCenterNorm() {
    return u.sum().item<double>();
}

double Ellipsoid::logeps = torch::tensor(1.0).to(torch::kFloat64).requires_grad_(false).item<double>();

bool Ellipsoid::volumeCheck() {
    return getLogVolume() < logeps;
}

double Ellipsoid::longestAxisLength() {
    // Compute eigenvalues of H
    torch::Tensor eigvals = std::get<0>(torch::linalg_eigh(H));  // ascending order

    // Get the largest eigenvalue (last element)
    double lambda_max = eigvals[-1].item<double>();

    // Longest axis length = 2 * sqrt(largest eigenvalue)
    return std::sqrt(lambda_max);
}