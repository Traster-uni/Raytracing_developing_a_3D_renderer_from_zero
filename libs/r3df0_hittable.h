//
// Created by tommasomarialopedote on 17/07/24.
//

#ifndef R3DFROM0_R3DF0_HITTABLE_H
#define R3DFROM0_R3DF0_HITTABLE_H

#include <vector>
#include <memory>
#include "r3df0_math.h"
#include "r3df0_rays.h"

using namespace std;

namespace r3dfrom0{
    class hit_record{
        /**
         * A record to save data at ray intersection
         */
    public:
        // attributes
        vec3f position;
        vec3f normal;
        double t;
        bool front_face_hit;

        // constructors
        hit_record() {};

        // methods
        void set_face_normals(const ray& r, const vec3f& outward_normal){
            if (dot(r.direction(), outward_normal) < 0.0){
                // ray hitting from inside
                normal = -outward_normal;
                front_face_hit = false;
            } else {
                // ray hitting from outside
                normal = outward_normal;
                front_face_hit = true;
            }
        } // set_face_normals method
    }; // hit_record class


    class hittable{ // interface
        /**
         * All world objects implement this hittable interface
         */
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval i, hit_record& record) const = 0;
    }; // hittable interface


    class hittable_list : hittable{
    public: // attributes
        vector<shared_ptr<hittable>> objects_list;

        // constructors
        hittable_list() {}; // default constructor
        hittable_list(shared_ptr<hittable> obj) { objects_list.push_back(obj); };

        // methods
        void append(shared_ptr<hittable> obj) {
            objects_list.push_back(obj);
        }

        bool hit(const ray& r, interval i, hit_record& record) const override{
            /**
             * hittable_list has to implement the method hit because it implements the
             * hittable interface. this object is a container, so the function of this
             * hit method is to call the hit method of the actual object in the world
             */
            hit_record temp;
            bool hit_detected = false;

            for (const auto& obj : objects_list){
                if (obj->hit(r, i, temp)){
                    hit_detected = true;
                    // i.max is the closest value so far
                    i.max = temp.t; // <-- may cause errors
                    record = temp;
                }
            }
            return hit_detected;
        } // hit method

    }; // hittable_list class
}

#endif //R3DFROM0_R3DF0_HITTABLE_H
