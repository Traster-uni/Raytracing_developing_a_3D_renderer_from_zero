//
// Created by trast on 16/08/2024.
//

#ifndef R3DFROM0_R3DF0_BVH_H
#define R3DFROM0_R3DF0_BVH_H


#include "r3df0_varsutil.h"
#include "r3df0_hittable.h"
#include <algorithm>

namespace r3dfrom0{

    class bvh_node : public hittable{
    public:
        // constructor
        bvh_node(hittable_list hittableList) :
            bvh_node(hittableList.objects_list, 0, hittableList.objects_list.size()) {};

        bvh_node(vector<shared_ptr<hittable>>& hittableList, size_t start_index, size_t end_index) {
            axisAlignBbox bbx1 = axisAlignBbox::empty;
            for (size_t obj_index = start_index; obj_index < end_index; obj_index++){
                bbx1 = axisAlignBbox(bbx1, hittableList[obj_index]->bbox());
            }

            int rdm_axis = bbx1.longest_axis();
            // comparison function selector used to sort the hittable list
            auto comp = (rdm_axis == 0) ? box_x_compare
                    : (rdm_axis == 1) ? box_y_compare : box_z_compare;

            size_t delta = end_index - start_index;
            if (delta == 1){
                right = left = hittableList[start_index];
                // TODO: error: no match for 'operator=' (operand types are 'std::shared_ptr<r3dfrom0::bvh_node>' and '__gnu_cxx::__alloc_traits<std::allocator<std::shared_ptr<r3dfrom0::hittable> >, std::shared_ptr<r3dfrom0::hittable> >::value_type' {aka 'std::shared_ptr<r3dfrom0::hittable>'})
                //                 right = left = hittableList[start_index];
            } else if (delta == 2) {
                left = hittableList[start_index];
                right = hittableList[start_index + 1]
            } else {
                // sort hittableList by x,y,z axis randomly
                std::sort(begin(hittableList) + start_index, begin(hittableList) + end_index, comp);

                auto m = start + delta/2;
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
            interval reduced_interval = {i.min, hit_record ? record.t : i.max};
            bool hit_left = left->hit(r, reduced_interval, record);
            return hit_right || hit_left;
        } // hit method

        axisAlignBbox bounding_box() {
            return bbox;
        }

    private:
        // private arguments
        shared_ptr<bvh_node> right;
        shared_ptr<bvh_node> left;
        axisAlignBbox bbox;

        // methods
        //      comparing methods
        static inline bool box_compare(shared_ptr<hittable> a, shared_ptr<hittable> b, int axis_index){
            auto a_interval = a->bbox().axis_interval(axis_index);
            auto b_interval = b->bbox().axis_interval(axis_index);
            return a_interval.min < b_interval.min;
        }

        static inline bool box_x_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
            return box_compare(a, b, 0)
        }

        static inline bool box_y_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
            return box_compare(a, b, 1);
        }

        static inline bool box_z_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
            return box_compare(a, b, 2);
        }

    }; // bvh_node definition

}

#endif //R3DFROM0_R3DF0_BVH_H
