#pragma once
#include <cstdlib>

int Random(int lo, int hi) {
    int r = rand() % (hi - lo + 1) + lo;

    return r;
}