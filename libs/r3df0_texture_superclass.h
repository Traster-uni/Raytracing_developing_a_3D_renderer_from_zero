//
// Created by trast on 17/08/2024.
//

#ifndef R3DFROM0_R3DF0_TEXTURE_SUPERCLASS_H
#define R3DFROM0_R3DF0_TEXTURE_SUPERCLASS_H

#include "r3df0_varsutil.h"

namespace r3dfrom0{
    class texture {  // interface
        /**
         *  A common interface for all texture related classes
         */
    public:
        virtual ~texture() = default;

        virtual pixel_f color(float u, float v, const vec3f& p) const = 0;
    };  // texture interface
}
#endif //R3DFROM0_R3DF0_TEXTURE_SUPERCLASS_H
