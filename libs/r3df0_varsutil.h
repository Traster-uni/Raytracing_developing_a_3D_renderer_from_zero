//
// Created by tommasomarialopedote on 12/07/24.
//

#ifndef R3DFROM0_R3DF0_VARSUTIL_H
#define R3DFROM0_R3DF0_VARSUTIL_H

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

#endif //R3DFROM0_R3DF0_VARSUTIL_H
