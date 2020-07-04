#include "Utils.hpp"
#include <cmath>

inline int square(int a) {
    return a * a;
}

inline float distance(int x, int y, int x2, int y2) {
    return std::sqrt(square(x - x2) + square(y - y2));
}
