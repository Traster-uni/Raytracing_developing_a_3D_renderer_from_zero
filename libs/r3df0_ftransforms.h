//
// Created by trast on 08/10/2024.
//

#ifndef R3DFROM0_R3DF0_FTRANSFORMS_H
#define R3DFROM0_R3DF0_FTRANSFORMS_H

#include "r3df0_varsutil.h"
#include "r3df0_hittable.h"
#include "r3df0_axisAlignBBoxes.h"

using namespace std;
namespace r3dfrom0{
    class translate : public hittable {
    public:
        translate(shared_ptr<hittable> object, const vec3f& offset)
                : object(object), offset(offset)
        {
            bbox = object->bounding_box() + offset;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // Move the ray backwards by the offset
            ray offset_r(r.origin() - offset, r.direction());

            // Determine whether an intersection exists along the offset ray (and if so, where)
            if (!object->hit(offset_r, ray_t, rec))
                return false;

            // Move the intersection point forwards by the offset
            rec.position += offset;

            return true;
        }

        axisAlignBbox bounding_box() const override { return bbox; }

        void apply_o2w() const override {};
        void apply_w2o() const override {};

    private:
        shared_ptr<hittable> object;
        vec3f offset;
        axisAlignBbox bbox;
    };

    class rotate_y : public hittable {
    public:
        rotate_y(shared_ptr<hittable> object, double angle) : object(object) {
            auto radians = degrees_to_radians(angle);
            sin_theta = std::sin(radians);
            cos_theta = std::cos(radians);
            bbox = object->bounding_box();

            vec3f min( infinity,  infinity,  infinity);
            vec3f max(-infinity, -infinity, -infinity);

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        auto x = i*bbox.x.max + (1-i)*bbox.x.min;
                        auto y = j*bbox.y.max + (1-j)*bbox.y.min;
                        auto z = k*bbox.z.max + (1-k)*bbox.z.min;

                        auto newx =  cos_theta*x + sin_theta*z;
                        auto newz = -sin_theta*x + cos_theta*z;

                        vec3f tester(newx, y, newz);

                        for (int c = 0; c < 3; c++) {
                            min[c] = std::fmin(min[c], tester[c]);
                            max[c] = std::fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = axisAlignBbox(min, max);
        }


        bool hit(const ray& r, interval i, hit_record& hitRecord) const override {

            // Transform the ray from world space to object space.

            auto origin = vec3f(
                    (cos_theta * r.origin().x) - (sin_theta * r.origin().z),
                    r.origin().y,
                    (sin_theta * r.origin().x) + (cos_theta * r.origin().z)
            );

            auto direction = vec3f(
                    (cos_theta * r.direction().x) - (sin_theta * r.direction().z),
                    r.direction().y,
                    (sin_theta * r.direction().x) + (cos_theta * r.direction().z)
            );

            ray rotated_r(origin, direction); // time()?

            // Determine whether an intersection exists in object space (and if so, where).

            if (!object->hit(rotated_r, i, hitRecord))
                return false;

            // Transform the intersection from object space back to world space.

            hitRecord.position = vec3f(
                    (cos_theta * hitRecord.position.x) + (sin_theta * hitRecord.position.z),
                        hitRecord.position.y,
                        (-sin_theta * hitRecord.position.x) + (cos_theta * hitRecord.position.z)
                    );

            hitRecord.normal = vec3f(
                    (cos_theta * hitRecord.normal.x) + (sin_theta * hitRecord.normal.z),
                    hitRecord.normal.y,
                    (-sin_theta * hitRecord.normal.x) + (cos_theta * hitRecord.normal.z)
            );

            return true;
        }

        axisAlignBbox bounding_box() const override { return bbox; }
        void apply_o2w() const override {};
        void apply_w2o() const override {};


    private:
        shared_ptr<hittable> object;
        double sin_theta;
        double cos_theta;
        axisAlignBbox bbox;
    };
}

#endif //R3DFROM0_R3DF0_FTRANSFORMS_H
