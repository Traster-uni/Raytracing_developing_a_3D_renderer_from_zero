//
// Created by tommasomarialopedote on 12/07/24.
//

#ifndef R3DFROM0_R3DF0_VARSUTIL_H
#define R3DFROM0_R3DF0_VARSUTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


// C++ Std Usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers
#include "r3df0_color.h"
#include "r3df0_rays.h"
#include "r3df0_math.h"


#endif //R3DFROM0_R3DF0_VARSUTIL_H
