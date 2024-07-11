//
// Created by tommasomarialopedote on 10/07/24.
//

#ifndef R3DFROM0_R3DF0_RAYS_H
#define R3DFROM0_R3DF0_RAYS_H

#include "r3df0_math.h"
using namespace std;

namespace r3dfrom0{

    class ray{
    private: // attributes
        vec3f _origin;
        vec3f _dir;
    public:
        // constructors
        ray() {} // default constructor defines a null ray
        ray(const vec3f origin, const vec3f direction) : _origin{origin}, _dir{direction} {}

        // getters
        const vec3f& origin() const { return _origin; }
        const vec3f& direction() const { return _dir; }

        // methods
        vec3f at(const float t) const {
            return _origin + (t * _dir);
        }

    };

    inline const ostream& operator<< (ostream& out, const ray& r) {
        return out << r.origin() << " " << r.direction();
    }
}

#endif //R3DFROM0_R3DF0_RAYS_H
