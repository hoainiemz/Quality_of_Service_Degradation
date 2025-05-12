//
// Created by hoainiem on 5/12/25.
//

#include "Utils.h"

long long Utils::POW(long long x, long long y) {
    long long res = 1;
    for (long long i = 1; i < y; i <<= 1, x = x * x) {
        if (y & i) {
            res *= x;
        }
    }
    return res;
}
