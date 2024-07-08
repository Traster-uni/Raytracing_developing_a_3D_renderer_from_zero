//
// Created by trast on 01/07/2024.
//

#ifndef _R3DFROM0_MATH_H
#define _R3DFROM0_MATH_H

// INCLUDES
#include <cmath>

namespace r3dfrom0 {

    // floats
    struct pixel_f { // by default, it's black
        float r = 0;
        float g = 0;
        float b = 0;
    };

    struct vec3f { // by default, born at the origin of the axis
        float x = 0;
        float y = 0;
        float z = 0;
    };

    //integers
    struct pixel_i { // by default, it's black
        int r = 0;
        int g = 0;
        int b = 0;
    };

    struct vec3i {
        int x = 0;
        int y = 0;
        int z = 0;
    };


} // namespace r3dfrom0

#endif //_R3DFROM0_MATH_H
