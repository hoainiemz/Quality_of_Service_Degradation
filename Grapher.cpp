//
// Created by hoainiem on 5/7/25.
//

#include "Grapher.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <queue>

#include "MyRandom.h"
#include "Utils.h"

int Grapher::write(std::string filename) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Cannot open file.\n";
        return -1;
    }

    file << n << ' ' << m << ' ' << pathQueries.size() << '\n';
    // for (std::pair<int, int> e : entries) {
    //     file << e.first << ' ' << e.second << '\n';
    // }
    for (int i = 0; i < m; i++) {
        file << edges[i] << '\n';
    }
    for (std::pair<int, int> p : pathQueries) {
        file << p.first << ' ' << p.second << '\n';
    }
    return 0;
}

std::pair<int, int> Grapher::read(std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file.\n";
        return {-1, -1};
    }

    std::string line;
    // Bỏ qua phần tiêu đề và dòng comment
    while (getline(file, line)) {
        if (line[0] == '%') continue;
        else break;
    }

    // Đọc kích thước và số phần tử khác 0
    int rows, cols, nonzeros;
    std::stringstream ss(line);
    ss >> rows >> cols >> nonzeros;
    n = rows;
    m = nonzeros;
    l.resize(rows);

    for (int i = 0; i < nonzeros; ++i) {
        int r, c;
        file >> r >> c;
        --r; --c;
        entries.push_back(std::make_pair(r, c));
    }
    makeWeighted();
    genPathQueries();
    return {n, m};
}

std::pair<int, int> Grapher::randomize(int n, double density) {
    this->n = n;
    this->m = 0;
    l.resize(n);
    entries.clear();

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (MyRandom::bernouli(density)) {
                entries.push_back(std::make_pair(u, v));
                ++m;
            }
        }
    }
    makeWeighted();
    genPathQueries();
    return {this->n, this->m};
}

void Grapher::makeWeighted() {
    edges.clear();
    for (int i = 0; i < entries.size(); ++i) {
        Edge edge(i, entries[i].first, entries[i].second);
        edges.push_back(edge);
        l[edge.getU()].push_back(i);
        l[edge.getV()].push_back(i);
    }
}

int Edge::from(int x) {
    // assert(x == u || x == v);
    return (x == u) ? v : u;
}

void Edge::makeWeighted() {
    int type = MyRandom::randInt(0, Constants::edgeType);
    int a, b, c, d, e;
    switch (type) {
        case 0:
            // A linear function: f(x) = ax + b
            a = MyRandom::randInt(3, 10);
            b = 1;
            for (int i = 0; ; i++) {
                weight.push_back(std::min(a * i + b, Constants::T));
                if (weight.back() >= Constants::T) {
                    break;
                }
            }
            break;
        case 1:
            // a squared function: f(x) = ax^2 + bx + c
            a = MyRandom::randInt(1, 5);
            b = MyRandom::randInt(3, 10);
            c = 1;
            for (int i = 0; ; i++) {
                int pot = std::min(a * i * i + b * i + c, Constants::T);
                weight.push_back(pot);
                if (weight.back() >= Constants::T) {
                    break;
                }
            }
            break;
        case 2:
            // A concave function: f(x) = d * ln(ax^2 + bx + c) + e
            a = MyRandom::randInt(3, 20);
            b = MyRandom::randInt(10, 50);
            c = 1;
            d = MyRandom::randInt(5, 20);
            e = MyRandom::randInt(std::max(1, Constants::T - 30), Constants::T);
            if (e != 1) {
                weight.push_back(1);
            }
            for (int i = 0; ; i++) {
                int pot = std::min(d * (int)log(a * i * i + b * i + c) + e, Constants::T);
                if (weight.empty() || weight.back() != pot) {
                    weight.push_back(pot);
                }
                if (weight.back() >= Constants::T) {
                    break;
                }
            }
            break;
        default:
            // A cutting function: f(0) = 1, f(1) = T
            weight.push_back(1);
            weight.push_back(Constants::T);
            break;
    }
    assert(weight[0] == 1 && weight.size() > 1);
}

std::ostream& operator<<(std::ostream& os, const Edge& p) {
    os << p.u << ' ' << p.v << ' ';
    os << p.weight.size() << ' ';
    for (int w : p.weight) {
        os << w << ' ';
    }
    return os;
}

std::pair<std::vector<int>, std::vector<int> > Grapher::dijkstra(int x, std::vector<int> cost) {
    std::vector<int> dist(n, Constants::T), prev(n, -1);
    std::vector<std::queue<int> > queue(Constants::T + 1);
    if (cost.empty()) {
        cost.resize(m, 0);
        fill(cost.begin(), cost.end(), 0);
    }
    dist[x] = 0;
    queue[0].push(x);
    for (int i = 0; i < Constants::T; i++) {
        while (!queue[i].empty()) {
            int u = queue[i].front();
            queue[i].pop();
            if (dist[u] != i) {
                continue;
            }
            for (int id : l[u]) {
                int v = edges[id].from(u), k = std::min(Constants::T, dist[u] + edges[id].getWeight(cost[id]));
                if (dist[v] > k) {
                    dist[v] = k;
                    prev[v] = id;
                    queue[dist[v]].push(v);
                }
            }
        }
    }
    return {dist, prev};
}

std::pair<std::vector<double>, std::vector<int> > Grapher::dijkstra(double x, std::vector<double> cost) {
    assert(Constants::edgeType == 1);
    std::vector<double> dist(n, Constants::T);
    std::vector<int> prev(n, -1);
    std::priority_queue<std::pair<double, int> > queue;
    if (cost.empty()) {
        cost.resize(m, 0);
        fill(cost.begin(), cost.end(), 0);
    }
    dist[x] = 0;
    queue.push({0, x});
    while (!queue.empty()) {
        std::pair<double, int> u = queue.top();
        queue.pop();
        if (abs(dist[u.second] - u.first) > Constants::eps) {
            continue;
        }
        for (int id : l[u.second]) {
            int v = edges[id].from(u.second), k = std::min(Constants::T * 1.0, dist[u.second] + edges[id].getLinearWeight(cost[id]));
            if (dist[v] > k && abs(dist[v] - k) > Constants::eps) {
                dist[v] = k;
                prev[v] = id;
                queue.push({k, v});
            }
        }
    }
    return {dist, prev};
}
std::vector<int> Grapher::getUnblockedPath(std::vector<double> &cost) {
    for (int i = 0; i < pathQueries.size(); i++) {
        int u = pathQueries[i].first, v = pathQueries[i].second;
        std::pair<std::vector<double>, std::vector<int> > p = dijkstra(u, cost);
        if (abs(p.first[v] - Constants::T) < Constants::eps || p.first[v] >= Constants::T) {
            continue;
        }
        std::vector<int> res(m, 0);
        while (v != u) {
            res[p.second[v]] += edges[p.second[v]].getLinearTan();
            v = edges[p.second[v]].from(v);
        }
        return res;
    }
    return {};
}

void Grapher::genPathQueries() {
    std::ofstream file("../output/graph.txt");
    pathQueries.clear();
    int t = Constants::numPathQueries;
    while (t > 0) {
        int u = MyRandom::randInt(0, n - 1);
        std::vector<int> vt, dist = dijkstra(u).first;
        for (int i = 0; i < n; i++) {
            if (dist[i] > 0 && dist[i] < Constants::T) {
                vt.push_back(i);
            }
        }
        if (vt.size()) {
            pathQueries.push_back({u, vt[MyRandom::randInt(0, vt.size())]});
            t--;
            file << u << ' ' << pathQueries.back().second << ' ' << dist[pathQueries.back().second] << '\n';
        }
    }
}

bool Grapher::isFeasible(std::vector<int> &cost) {
    for (int i = 0; i < pathQueries.size(); i++) {
        std::pair<std::vector<int>, std::vector<int> > p = dijkstra(pathQueries[i].first, cost);
        if (p.first[pathQueries[i].second] < Constants::T) {
            return false;
        }
    }
    return true;
}


std::vector<Path> Grapher::getPotentialPaths(std::vector<int> &cost) {
    std::vector<Path> paths;
    std::vector<int> count(m, 0);
    std::vector<std::vector<int> > pl;
    std::vector<std::vector<int> > pr;
    pl.resize(pathQueries.size());
    pr.resize(pathQueries.size());
    for (int i = 0; i < pathQueries.size(); i++) {
        pl[i] = dijkstra(pathQueries[i].first, cost).first;
        pr[i] = dijkstra(pathQueries[i].second, cost).first;
        int distance = pl[i][pathQueries[i].second];
        if (distance == Constants::T) {
            continue;
        }
        for (int j = 0; j < m; j++) {
            int u = edges[j].getU(), v = edges[j].getV(), w = edges[j].getWeight(cost[j]);
            if (distance == pl[i][u] + pr[i][v] + w || distance == pl[i][v] + pr[i][u] + w) {
                count[j]++;
            }
        }
    }
    std::queue<int> que;
    std::vector<int> visited(n);
    for (int i = 0; i < pathQueries.size(); i++) {
        int distance = pl[i][pathQueries[i].second];
        if (distance == Constants::T) {
            continue;
        }

        // binary search

        int le = 1, mid, ri = pathQueries.size() + 1;
        while (le <= ri) {
            mid = (le + ri) / 2;
            fill(visited.begin(), visited.end(), -1);
            while (!que.empty()) {
                que.pop();
            }
            visited[pathQueries[i].first] = true;
            que.push(pathQueries[i].first);
            while (!que.empty()) {
                int u = que.front();
                que.pop();
                for (int id : l[u]) {
                    int v = edges[id].from(u), w = edges[id].getWeight(cost[id]);
                    if (visited[v] == -1 && distance == pl[i][u] + pr[i][v] + w && count[id] <= mid) {
                        visited[v] = id;
                        que.push(v);
                    }
                }
            }

            if (le == ri) {
                break;
            }

            if (visited[pathQueries[i].second] != -1) {
                ri = mid;
            }
            else {
                le = mid + 1;
            }
        }

        Path path;

        int u = pathQueries[i].first, v = pathQueries[i].second;
        while (v != u) {
            path.push_back(visited[v]);
            v = edges[visited[v]].from(v);
        }
        std::reverse(path.begin(), path.end());
        paths.push_back(path);
    }
    return paths;
}

Path Grapher::getPotentialPath(int u, int v, std::vector<int> &cost) {
    if (cost.empty()) {
        cost.resize(m, 0);
    }
    Path path;
    std::pair<std::vector<int>, std::vector<int> > p = dijkstra(u, cost);
    if (p.first[v] >= Constants::T) {
        return {};
    }
    int sus = v;
    while (sus != u) {
        path.push_back(p.second[sus]);
        sus = edges[p.second[sus]].from(sus);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::istream& operator>>(std::istream& in, Edge& p) {
    in >> p.u >> p.v;
    int sz;
    in >> sz;
    p.weight.resize(sz);
    for (int i = 0; i < sz; i++) {
        in >> p.weight[i];
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, const Grapher& grapher) {
    os << grapher.n << ' ' << grapher.m << ' ' << grapher.pathQueries.size() << '\n';
    for (int i = 0; i < grapher.m; i++) {
        os << grapher.edges[i] << '\n';
    }
    for (std::pair<int, int> p : grapher.pathQueries) {
        os << p.first << ' ' << p.second << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& in, Grapher& grapher) {
    int num;
    in >> grapher.n >> grapher.m >> num;
    grapher.entries.clear();
    grapher.l.resize(grapher.n);
    grapher.edges.resize(grapher.m);
    for (int i = 0; i < grapher.m; i++) {
        in >> grapher.edges[i];
        grapher.entries.push_back(std::make_pair(grapher.edges[i].getU(), grapher.edges[i].getV()));
        grapher.l[grapher.edges[i].getU()].push_back(i);
        grapher.l[grapher.edges[i].getV()].push_back(i);
    }
    grapher.pathQueries.clear();
    while (num--) {
        int u, v;
        in >> u >> v;
        grapher.pathQueries.push_back(std::make_pair(u, v));
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& p) {
    for (int i : p) {
        os << i << ' ';
    }
    return os;
}

long long Grapher::budgetFunction(std::vector<Path> &paths, std::vector<int> cost) {
    long long sum = 0;
    for (Path &path : paths) {
        long long pot = 0;
        for (int i : path) {
            pot += edges[i].getWeight(cost[i]);
        }
        sum += std::min(pot, 1LL * Constants::T);
    }
    return sum;
}

std::vector<int> operator+(std::vector<int> a, std::vector<int> &b) {
    assert(a.size() == b.size());
    for (int i = 0; i < a.size(); i++) {
        a[i] += b[i];
    }
    return a;
}

std::vector<int> addition(std::vector<int> a, int idx, int val) {
    a[idx] += val;
    return a;
}

int Grapher::getNorm(std::vector<int> x) {
    int res = 0;
    for (int i : x) {
        res += i;
    }
    return res;
}

std::vector<std::pair<Path, double> > Grapher::samplingPath(std::vector<int> &cost) {
    std::vector<std::pair<std::vector<int>, std::vector<int> > >dist;
    dist.resize(pathQueries.size());
    for (int i = 0; i < pathQueries.size(); i++) {
        dist[i] = dijkstra(pathQueries[i].second, cost);
    }
    std::vector<std::pair<Path, double> > paths;

    int num = getNumSampling();
    while (num--) {
        int id = MyRandom::randInt(0, pathQueries.size());
        double prob = 1.0 / pathQueries.size();
        int s = pathQueries[id].first, t = pathQueries[id].second;
        if (dist[id].first[s] >= Constants::T) {
            continue;
        }
        Path path;
        for (int u = s; u != t;) {
            int i = dist[id].second[u];
            assert(i != -1);
            int v = edges[i].from(u);
            if (MyRandom::bernouli(Constants::alpha)) {
                path.push_back(i);
                prob *= Constants::alpha;
                u = v;
                continue;
            }
            std::vector<int> ds;
            for (int j : l[u]) {
                int x = edges[j].from(u), w = edges[j].getWeight(cost[j]);
                if (dist[id].first[x] <= dist[id].first[u]) {
                    ds.push_back(j);
                }
            }
            if (ds.empty()) {
                path.push_back(i);
                u = v;
                continue;
            }
            int chose = ds[MyRandom::randInt(0, ds.size())];
            path.push_back(chose);
            u = edges[chose].from(u);
            prob *= (1 - Constants::alpha) / ds.size();
        }
        paths.push_back({path, prob});
    }
    return paths;
}

int Grapher::maximumDegree() {
    int res = 0;
    for (int i = 0; i < n; i++) {
        if (res < l[i].size()) {
            res = l[i].size();
        }
    }
    return res;
}

int Grapher::getNumSampling()  {
    return 100000;
}

long long Grapher::budgetFunction(Path path, std::vector<int> cost) {
    long long pot = 0;
    for (int i : path) {
        pot += edges[i].getWeight(cost[i]);
    }
    return std::min(pot, 1LL * Constants::T);
}

