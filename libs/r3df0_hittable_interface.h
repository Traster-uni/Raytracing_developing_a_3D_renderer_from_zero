//
// Created by tommasomarialopedote on 15/08/24.
//

#ifndef R3DFROM0_R3DF0_HITTABLE_INTERFACE_H
#define R3DFROM0_R3DF0_HITTABLE_INTERFACE_H

namespace r3dfrom0{
    class ray;
    class interval;
    class hit_record;
    class axisAlignBbox;

    class hittable{ // interface
        /**
         * All world objects implement this hittable interface
         */
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval i, hit_record& record) const = 0;

        virtual axisAlignBbox bbox() const = 0;
    }; // hittable interface
}

#endif //R3DFROM0_R3DF0_HITTABLE_INTERFACE_H
