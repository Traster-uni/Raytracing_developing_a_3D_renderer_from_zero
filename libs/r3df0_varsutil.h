//
// Created by tommasomarialopedote on 12/07/24.
//

#ifndef R3DFROM0_R3DF0_VARSUTIL_H
#define R3DFROM0_R3DF0_VARSUTIL_H

#include <random>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * (pi / 180.0);
}


inline double radians_to_degrees(double radians) {
    return radians * (180.0 / pi);
}

inline float random_float(){
    static mt19937 generator;
    // return a random float in [0, 1[
    static uniform_real_distribution<float> distribution(0.0,1.0f);
    return distribution(generator);
}

inline float random_float(float min, float max){
    // returns a random in [min, max]
    return min + (max - min) * random_float();
}

inline int random_int(int min, int max){
    return int(random_float(min, max+1));
}

#include "r3df0_math.h"
#include "r3df0_rays.h"
#include "r3df0_hittable_interface.h"
#include "r3df0_material_interface.h"

#endif //R3DFROM0_R3DF0_VARSUTIL_H
