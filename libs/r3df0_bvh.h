//
// Created by tommasomarialopedote on 14/08/24.
//

#ifndef R3DFROM0_R3DF0_BVH_H
#define R3DFROM0_R3DF0_BVH_H

#include "r3df0_varsutil.h"

using namespace std;

namespace r3dfrom0{
    class hittable_list;

    class axisAlignBbox{
    public:
        // arguments
        interval x, y, z;

        // constructor
        axisAlignBbox() {};  // this constructor has a reason to exist because, intervals are
                            // defined by default empty
        axisAlignBbox(interval x, interval y, interval z) : x(x), y(y), z(z) {}

        axisAlignBbox(const vec3f& a, const vec3f& b){
            // defines bbox from 2 points
            x = (a.x <= b.x) ? interval(a.x, b.x) : interval(b.x, a.x);
            y = (a.y <= b.y) ? interval(a.y, b.y) : interval(b.y, a.y);
            z = (a.z <= b.z) ? interval(a.z, b.z) : interval(b.z, a.z);
        }

        axisAlignBbox(const axisAlignBbox& bbox1, const axisAlignBbox& bbox2){
            // define bbox from 2 bbox
            x = interval(bbox1.x, bbox2.x);
            y = interval(bbox1.y, bbox2.y);
            z = interval(bbox1.z, bbox2.z);
        }

        // methods
        const interval& axis_interval(int n) const{
            // n should be a number between 0 and 2 inclusive
            if (n == 0) return x;
            else if (n == 1) return y;
            else if (n == 2) return z;
        };

        bool hit(const ray& r, interval i){
            // check for hit, updates intervals
            const vec3f ray_origin = r.origin();
            const vec3f ray_dir = r.direction();

            for (int j=0; j < 3; j++){
                const interval axis = axis_interval(j);
                const double div = 1.0f / ray_dir.vec_iter(j);
                // t0 and t1 are the intersects points with the bbox
                auto t0 = (axis.min - ray_origin.vec_iter(j)) * div;
                auto t1 = (axis.max - ray_origin.vec_iter(j)) * div;

                if (t0 < t1){
                    if (t0 > axis.min) i.min = t0;
                    if (t1 < axis.max) i.max = t1;
                } else {
                    if (t1 < axis.min) i.min = t1;
                    if (t0 > axis.max) i.max = t0;
                }

                if (i.max <= i.min) return false;
            }
            return true;
        }
    }; // class axisAlignBbox

    class bvh_node : public hittable{
    public:
        // constructor
        bvh_node(hittable_list hittableList) : bvh_node(hittableList.list, 0, hittableList.list.size()) {}

        // methods
    private:
        shared_ptr<bvh_node> right;
        shared_ptr<bvh_node> left;
        axisAlignBbox bbox;
    };
}

#endif //R3DFROM0_R3DF0_BVH_H
