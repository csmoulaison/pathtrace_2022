#pragma once
#include <random>

// Returns a random double within the range (0.0,1.0)
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random double within the range (min,max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}
