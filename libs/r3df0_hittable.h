//
// Created by tommasomarialopedote on 17/07/24.
//

#ifndef R3DFROM0_R3DF0_HITTABLE_H
#define R3DFROM0_R3DF0_HITTABLE_H

#include <vector>
#include <memory>
#include "r3df0_axisAlignBBoxes.h"

using namespace std;

namespace r3dfrom0{

    class hit_record{
        /**
         * A record to save miscellaneous data at ray intersection
         */
    public:
        // attributes
        vec3f position;                     // hit vector on the surface
        vec3f normal;                       // normal calculated on ray hit aka shading normal
        shared_ptr<material> material_ptr;  // pointer to material instance
        float t;                            // hit on geometry
        float u;                            // horizontal z, x texture component (u,v)
        float v;                            // vertical y texture component (u,v)
        bool front_face_hit;                // answers the question: Is the object being hit on the front?

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


    class hittable_list : public hittable{
    public:
        // public attributes
        vector<shared_ptr<hittable>> objects_list;
        vector<shared_ptr<material>> material_list;

        // constructors
        hittable_list() {}; // default constructor
        hittable_list(shared_ptr<hittable> obj) {
            objects_list.push_back(obj);
        };

        // methods
        void append(shared_ptr<hittable> obj) {
            objects_list.push_back(obj);
            bbox = axisAlignBbox(bbox, obj->bounding_box());
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

        axisAlignBbox bounding_box() const override{
            return bbox;
        }

        void apply_w2o() const override {
            for (const auto& obj : objects_list) {
                obj->apply_w2o();
            }
        }

        void apply_o2w() const override {
            for (const auto& obj : objects_list) {
                obj->apply_o2w();
            }
        }

    private:
        axisAlignBbox bbox;
    }; // hittable_list class
}

#endif //R3DFROM0_R3DF0_HITTABLE_H
