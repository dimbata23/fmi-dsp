#include "Utils.hpp"
#include <cmath>

int square(int a) {
	return a * a;
}

float distance(int x, int y, int x2, int y2) {
	return std::sqrt(square(x - x2) + square(y - y2));
}
