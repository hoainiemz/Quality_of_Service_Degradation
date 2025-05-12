//
// Created by hoainiem on 5/7/25.
//

#ifndef RANDOM_H
#define RANDOM_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

class MyRandom {
public:
    static std::mt19937 rng; // random seed
    static std::uniform_real_distribution<> bernouliDist;

    static bool bernouli(double p);
    static int randInt(int l, int r);
};



#endif //RANDOM_H
