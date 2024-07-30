//
// Created by tommasomarialopedote on 12/07/24.
//

#ifndef R3DFROM0_R3DF0_VARSUTIL_H
#define R3DFROM0_R3DF0_VARSUTIL_H

#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * (pi / 180.0);
}


inline double radians_to_degrees(double radians) {
    return radians * (180.0 / pi);
}


inline float random_float(){
    // return a random float in [0, 1[
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    generator.seed(std::rand());
    return distribution(generator);
}

inline float random(float min, float max){
    // returns a random in [min, max]
    return min + (max - min) * random_float();
}

#endif //R3DFROM0_R3DF0_VARSUTIL_H
