//
// Created by tommasomarialopedote on 14/08/24.
//

#ifndef R3DFROM0_R3DF0_AXISALIGNBBOXES_H
#define R3DFROM0_R3DF0_AXISALIGNBBOXES_H

#include "r3df0_varsutil.h"

using namespace std;

namespace r3dfrom0{
    class axisAlignBbox{
        /**
         * Bounding volume used to include primitives or simple enough polygons with
         * the function of defining a sub-volume that delimits the would be hitting rays,
         * from the non hitting rays, it is used in tandem with the bvh_node class         *
         */
    public:
        // arguments
        interval x, y, z;

        // constructor
        axisAlignBbox() {};  // this constructor has a reason to exist because, intervals are
                            // defined by default empty
        axisAlignBbox(interval x, interval y, interval z) : x(x), y(y), z(z) {
        }

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
        interval operator[](int n) const {
            if (n == 1) return this->y;
            else if (n == 2) return this->z;
            else return this->x;
        }

        bool hit(const ray& r, interval i) const{
            // check for hit, updates intervals
            const vec3f ray_origin = r.origin();
            const vec3f ray_dir = r.direction();

            for (int j=0; j < 3; j++){
                interval axis = (*this)[j];
                const double axis_div = 1.0f / ray_dir[j];
                // t0 and t1 are the intersects points with the bbox
                auto t0 = (axis.min - ray_origin[j]) * axis_div;
                auto t1 = (axis.max - ray_origin[j]) * axis_div;
                // once t0 and t1 are confirmed hits into bbox, check hit into object interval
                if (t0 < t1){
                    if (t0 > i.min) i.min = t0;
                    if (t1 < i.max) i.max = t1;
                } else {
                    if (t1 > i.min) i.min = t1;
                    if (t0 < i.max) i.max = t0;
                }

                if (i.max <= i.min) return false;
            }
            return true;
        }

        int longest_axis() const{
            // checks which axis is the longest in the bounding box
            // return an index between [0,2]
            if (x.size() > y.size()){
                return x.size() > z.size() ? 0 : 2;
            } else {
                return y.size() > z.size() ? 1 : 2;
            }
        }

        // constants declarations
        static const axisAlignBbox empty, universe;
    }; // class axisAlignBbox

    // constants implementation
    const axisAlignBbox axisAlignBbox::empty    = axisAlignBbox(interval::empty,    interval::empty,    interval::empty);
    const axisAlignBbox axisAlignBbox::universe = axisAlignBbox(interval::universe, interval::universe, interval::universe);
}

#endif //R3DFROM0_R3DF0_AXISALIGNBBOXES_H
