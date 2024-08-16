//
// Created by tommasomarialopedote on 18/07/24.
//

#ifndef R3DFROM0_R3DF0_SHAPES_H
#define R3DFROM0_R3DF0_SHAPES_H

#include "r3df0_varsutil.h"
#include "r3df0_material.h"
#include "r3df0_bvh.h"

using namespace std;

namespace r3dfrom0{
    class sphere : public hittable{
    public:
        // constructors
        sphere() : center{0,0,0}, radius{1.0} {}; // default behavior
        sphere(const vec3f& c, const float& r, shared_ptr<material> mat) :
                center(c), radius(float (fmax(0,r))), material_ptr(mat) {
            auto temp_v = vec3f{radius, radius, radius};
            bbox1 = axisAlignBbox{center - temp_v, center + temp_v};
        };

        // methods
        bool hit(const ray& r, interval i, hit_record& hit_record) const override{
            auto c_q = center - r.origin();
            // optimized the dot function of a vector by noticing that the dot function of a
            // vector with itself is just the sqr_length of said vector.
            auto a = r.direction().sqr_length(); // d^2
            // b in old func is now d * (C - Q)
            auto h = dot(r.direction(), c_q); // d * (C-Q)
            // optimized the dot function of a vector to itself is, sqr_length of said vector
            auto c = c_q.sqr_length() - (radius * radius); // (C - Q)^2 - r^2
            // delta optimized by algebraic simplification
            auto delta = h * h - a * c;
            if (delta < 0) {
                return false;
            }
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
            hit_record.position = r.at(hit_record.t); // at what point the ray intersected the sphere
            auto normal = (hit_record.position - center) / radius; // calc normal on ray hit
            hit_record.set_face_normals(r, normal);
            hit_record.material_ptr = material_ptr;
            return true;
        } // hit method

        axisAlignBbox bbox() const override{
            return bbox1;
        }

    private:
        // attributes
        vec3f center;
        float radius = 1.0;
        shared_ptr<material> material_ptr;
        axisAlignBbox bbox1;
    }; // sphere class
}

#endif //R3DFROM0_R3DF0_SHAPES_H
