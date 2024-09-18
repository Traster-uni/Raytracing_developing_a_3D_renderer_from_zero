//
// Created by trast on 16/09/2024.
//

#ifndef R3DFROM0_R3DF0_POLYGONS_H
#define R3DFROM0_R3DF0_POLYGONS_H

#include <utility>

#include "r3df0_varsutil.h"
#include "r3df0_axisAlignBBoxes.h"
#include "r3df0_hittable_superclass.h"
#include "plycpp/plycpp/plycpp_runner.h"
#include "r3df0_shapes.h"

using namespace std;
using namespace plycpp;

namespace r3dfrom0 {
    // As per described in: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-polygon-mesh/polygon-mesh-file-formats.html
    // plus adaptations made to make it compatible with the current implementation of the renderer
    class polygon_mesh : public hittable {
    public:
        // public attributes
        int num_vertex = 0;
        int num_faces = 0;
        // constructor
        polygon_mesh(const string& fname, const shared_ptr<material>& mat) : polygon_common_material(mat) {
            load_mesh(fname);
            // todos:
            // call load_mesh() (may want to make it private)
            // create triangles for polygons
            // set up the hit methods as well as bounding boxes.
            for(int i = 0; i < num_faces; i++) {
                for(int j = 2; i < num_vertex; j+=3) {
                    auto v1 = vertex_array[vertex_index_array[j-2]];
                    auto v2 = vertex_array[vertex_index_array[j-1]];
                    auto v3 = vertex_array[vertex_index_array[j]];

                    auto tri = triangle(v1, v2, v3, polygon_common_material, true);
                    triangles_array.push_back(tri);
                }
            }
        };
        // public methods


        bool hit(const ray& r, interval i, hit_record& record) const override {
            for(auto tri : triangles_array){
                tri.hit(r, i, record);
                return true;
            }
            return false;
        }

        virtual void set_bounding_box() {
            auto max_z = vec3f(0,0,0);
            auto max_x = vec3f(0,0,0);
            auto max_y = vec3f(0,0,0);
            for (auto v : vertex_array) {
                if (v.z > max_z.z) max_z = v;
                if (v.x > max_x.x) max_x = v;
                if (v.y > max_y.y) max_y = v;
            }
            auto bbox1 = axisAlignBbox(max_x, max_y);
            auto bbox2 = axisAlignBbox(max_y, max_z);
            auto bbox3 = axisAlignBbox(max_z, max_x);
            auto bbox_sub = axisAlignBbox(bbox1, bbox2);
            bbox = axisAlignBbox(bbox_sub, bbox3);
        }

        axisAlignBbox bounding_box() const override {
            return bbox;
        }

    private:
        int num_vertex_per_face = 3;    // assumes triangle mashes
        vector<int>     vertex_index_array;
        vector<vec3f>   vertex_array;
        vector<vec3f>   uvs_array;
        vector<vec3f>   shading_normals_array;
        vector<triangle>     triangles_array;
        shared_ptr<material> polygon_common_material;
        axisAlignBbox bbox;

        // private methods
        void load_mesh(const string& fname) {
            try {
                PLYData data_buffer;    // dies at the end of call
                load(fname, data_buffer);

                for (const auto& element : data_buffer){

                    if (element.key  == "vertex") {
                        num_vertex = int(element.data->size());
                        float vx = *element.data->properties["x"]->ptr<float>();
                        float vy = *element.data->properties["y"]->ptr<float>();
                        float vz = *element.data->properties["z"]->ptr<float>();
                        vertex_array.emplace_back(vx, vy, vz); // builds vec3f emplace and automatically appends it

                    } else if (element.key == "face") {
                        num_faces = int(element.data->size());
                        for (int i=0; i < element.data->properties["vertex_indices"]->size(); i++ ){
                            int vi = element.data->properties["vertex_indices"]->at<int>(i);
                            vertex_index_array.push_back(vi);
                        }
                    }
                }

            } catch (const Exception& e){
                cout << "An Exception occurred while loading file: " << e.what() << endl;
            }
        }
    }; // class polygon_mesh
} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_POLYGONS_H
