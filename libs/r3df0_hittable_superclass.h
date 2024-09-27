//
// Created by tommasomarialopedote on 15/08/24.
//

#ifndef R3DFROM0_R3DF0_HITTABLE_SUPERCLASS_H
#define R3DFROM0_R3DF0_HITTABLE_SUPERCLASS_H

namespace r3dfrom0{
    class ray;
    class interval;
    class hit_record;
    class axisAlignBbox;

    class hittable{ // interface
    public:
        shared_ptr<matrix44f> w2o;   // world to object transform matrix
        shared_ptr<matrix44f> o2w;   // object to world transform matrix, inverse of w2o matrix
        // constructor
        virtual ~hittable() = default;
        // methods
        virtual bool hit(const ray& r, interval i, hit_record& record) const = 0;

        virtual axisAlignBbox bounding_box() const = 0;

        virtual void apply_w2o() const = 0;

        virtual void apply_o2w() const = 0;
    }; // hittable interface
}

#endif //R3DFROM0_R3DF0_HITTABLE_SUPERCLASS_H
