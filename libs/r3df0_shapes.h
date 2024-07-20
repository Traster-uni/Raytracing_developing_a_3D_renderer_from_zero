//
// Created by tommasomarialopedote on 18/07/24.
//

#ifndef R3DFROM0_R3DF0_SHAPES_H
#define R3DFROM0_R3DF0_SHAPES_H

#include "r3df0_varsutil.h"
#include "r3df0_math.h"
#include "r3df0_hittable.h"

using namespace std;

namespace r3dfrom0{
    class sphere : hittable{
    public:
        // attributes
        vec3f center;
        float radius = 1.0;

        // constructors
        sphere() : center{0,0,0}, radius{1.0} {}; // default behavior
        sphere(const vec3f& c, const float& r ) : center{c}, radius{float (fmax(0,r))} {}; // if attrs are defined

        bool hit(const ray& r, interval i, hit_record& hit_record) const override{
            auto c_q = center - r.origin();
            // optimized the dot function of a vector by noticing that the dot function of
            // a vector with itself is just the sqr_length of said vector.
            auto a = r.direction().sqr_length();
            // b in old func is now d * (C - Q)
            auto h = dot(r.direction(), c_q);
            // optimized the dot function of a vector to itself is, sqr_length of said vector
            auto c = c_q.sqr_length() - (radius * radius);
            // delta optimized by algebraic simplification
            auto delta = h * h - a * c;
            auto sqrt_delta = sqrt(delta);
            // roots of the quadratic equation
            auto roots = (h - sqrt_delta) / a;
            if (i.excludes(roots)){
                roots = (h + sqrt_delta) / a;
                if (i.excludes(roots)) {
                    return false;
                }
            }

            // adding tuple to save the hit to the sphere object
            hit_record.t = roots;
            hit_record.position = r.at(hit_record.t);
            auto n = (hit_record.position - center) / radius;
            hit_record.set_face_normals(r, n);
            return true;
        }
    };
}

#endif //R3DFROM0_R3DF0_SHAPES_H
