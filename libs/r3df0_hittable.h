//
// Created by tommasomarialopedote on 17/07/24.
//

#ifndef R3DFROM0_R3DF0_HITTABLE_H
#define R3DFROM0_R3DF0_HITTABLE_H

#include <vector>
#include <memory>

using namespace std;

namespace r3dfrom0{
    class material;

    class hit_record{
        /**
         * A record to save miscellaneous data at ray intersection
         */
    public:
        // attributes
        vec3f position;                     // hit vector on the surface
        vec3f normal;                       // normal on the surface
        shared_ptr<material> material_ptr;  // pointer to material data of geometry
        float t;                            // hit on surface (roots of quadratic equation)
        bool front_face_hit;                // Is the object being hit on the front?

        // constructors
        hit_record() {};

        // methods
        void set_face_normals(const ray& r, const vec3f& outward_normal){
            if (dot(r.direction(), outward_normal) < 0.0){
                // ray hitting from inside
                normal = outward_normal;
                front_face_hit = true;
            } else {
                // ray hitting from outside
                normal = -outward_normal;
                front_face_hit = false;
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
        vector<shared_ptr<material>> material_list;

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
            auto closest_hit_so_far = i.max;

            for (const auto& obj : objects_list){
                // update intersect interval to correctly calculate intersection order
                // of hittable objects
                auto updated_i = interval(i.min, closest_hit_so_far);
                if (obj->hit(r, updated_i, temp)){
                    hit_detected = true;

                    if (temp.t < closest_hit_so_far){
                        closest_hit_so_far = temp.t; // update on closest so far
                    }
                    record = temp; // copies hit record from object to hit record of list.
                }
            }
            return hit_detected;
        } // hit method
    }; // hittable_list class
}

#endif //R3DFROM0_R3DF0_HITTABLE_H
