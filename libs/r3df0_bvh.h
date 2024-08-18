//
// Created by trast on 16/08/2024.
//

#ifndef R3DFROM0_R3DF0_BVH_H
#define R3DFROM0_R3DF0_BVH_H


#include "r3df0_varsutil.h"
#include "r3df0_hittable.h"
#include <algorithm>

using namespace std;

namespace r3dfrom0{

    class bvh_node : public hittable{
    public:
        // constructors
        bvh_node(hittable_list hittableList) :
            bvh_node(hittableList.objects_list, 0, hittableList.objects_list.size()) {};

        bvh_node(vector<shared_ptr<hittable>>& hittableList, size_t start_index, size_t end_index) {
            bbox = axisAlignBbox::empty;
            for (size_t obj_index = start_index; obj_index < end_index; obj_index++){
                // for each object in hittableList define bbox from the previous bbox and the one of the current object
                bbox = axisAlignBbox(bbox, hittableList[obj_index]->bounding_box());
            }

            int longestAxis = bbox.longest_axis();
            // comparison function selector used to sort the hittable list
            auto comp =
                    (longestAxis == 0) ? box_x_compare: (longestAxis == 1) ? box_y_compare : box_z_compare;

            size_t delta = end_index - start_index;
            if (delta == 1){
                right = left = hittableList[start_index];

            } else if (delta == 2) {
                left = hittableList[start_index];
                right = hittableList[start_index + 1];

            } else {
                // sort hittableList by x,y,z axis randomly
                sort(begin(hittableList) + start_index, begin(hittableList) + end_index, comp);

                auto m = start_index + delta/2;
                left = make_shared<bvh_node>(hittableList, start_index, m);
                right = make_shared<bvh_node>(hittableList, m, end_index);
            }
        }

        // methods
        bool hit(const ray& r, interval i, hit_record& record) const override{

            if (!bbox.hit(r, i)) {
                return false;
            }

            bool hit_right = right->hit(r, i, record);
            // if right hit was a success reduce interval for hit recording, no need to record two time the same spot
            interval reduced_interval = {i.min, hit_right ? record.t : i.max};
            bool hit_left = left->hit(r, reduced_interval, record);
            return hit_right || hit_left;
        } // hit method

        axisAlignBbox bounding_box() const override{
            return bbox;
        }

    private:
        // private arguments
        shared_ptr<hittable> right;
        shared_ptr<hittable> left;
        axisAlignBbox bbox;

        // methods
        //      comparing methods
        static inline bool box_compare(shared_ptr<hittable>& a, shared_ptr<hittable>& b, int axis_index){
            auto a_interval = a->bounding_box()[axis_index];
            auto b_interval = b->bounding_box()[axis_index];
            return a_interval.min < b_interval.min;
        }

        static inline bool box_x_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
            return box_compare(a, b, 0);
        }

        static inline bool box_y_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
            return box_compare(a, b, 1);
        }

        static inline bool box_z_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
            return box_compare(a, b, 2);
        }

    };
    }

#endif //R3DFROM0_R3DF0_BVH_H
