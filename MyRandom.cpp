//
// Created by hoainiem on 5/7/25.
//

#include "MyRandom.h"

#include <random>

std::mt19937 MyRandom::rng(static_cast<unsigned>(time(0)));
std::uniform_real_distribution<> MyRandom::bernouliDist(0.0, 1.0);

bool MyRandom::bernouli(double p) {
    return (bernouliDist(rng) < p) ? true : false;
}

int MyRandom::randInt(int l, int r) {
    std::uniform_int_distribution<int> intDist(l, r - 1);
    return intDist(rng);
}