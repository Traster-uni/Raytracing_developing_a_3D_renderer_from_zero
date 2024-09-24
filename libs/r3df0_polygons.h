//
// Created by trast on 16/09/2024.
//

#ifndef R3DFROM0_R3DF0_POLYGONS_H
#define R3DFROM0_R3DF0_POLYGONS_H

#include "r3df0_varsutil.h"
#include "r3df0_axisAlignBBoxes.h"
#include "r3df0_hittable_superclass.h"
#include "happly/happly.h"
#include "r3df0_shapes.h"

using namespace std;
using namespace happly;

namespace r3dfrom0 {
    // As per described in: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-polygon-mesh/polygon-mesh-file-formats.html
    // plus adaptations made to make it compatible with the current implementation of the renderer
    class polygon_mesh : public hittable {
    public:
        // public attributes
        int num_vertex = 0;
        int num_faces = 0;
        // constructor
        //// TEST CONSTRUCTOR ////
//        polygon_mesh(shared_ptr<material> mat) {
//            vertex_index_array = {0,1,2,1,3,2}; // bAd
//            vertex_array = {vec3f(0,0,0), vec3f(0,1,-1), vec3f(1,1,-1), vec3f(.5f, 1.5f,-2)};
//            num_faces = 2;
//            num_vertex = 4;
//            for (int i=2; i < vertex_index_array.size(); i+=num_vertex_per_face) {
//                auto v1 = vertex_array[vertex_index_array[i-2]];
//                auto v2 = vertex_array[vertex_index_array[i-1]];
//                auto v3 = vertex_array[vertex_index_array[i]];
//
//                auto tri = make_shared<triangle>(v1, v2, v3, mat);
//                triangles_array.push_back(tri);
//            }
//        }
        //// TEST CONSTRUCTOR ////

        polygon_mesh(const string& fname, const shared_ptr<material> mat) : poly_mat(mat){
            load_mesh(fname);
            set_bounding_box();
        };
        // public methods


        bool hit(const ray& r, interval i, hit_record& record) const override {
            return triangles_array->hit(r,i,record);
        }

        virtual void set_bounding_box() {
            auto max_v = vec3f(0,0,0);
            auto min_v = vec3f(0,0,0);

            for (auto v : vertex_array) {
                max_v = vec3f(fmax(v.x, max_v.x), fmax(v.y, max_v.y), fmax(v.z, max_v.z));
                min_v = vec3f(fmin(v.x, min_v.x), fmin(v.y, min_v.y), fmin(v.z, min_v.z));
            }
            bbox = axisAlignBbox(max_v, min_v);
        }

        axisAlignBbox bounding_box() const override {
            return bbox;
        }

    private:
        int num_vertex_per_face = 3;    // assumes triangle mashes
        vector<vector<int>>     vertex_index_array; // 7000 faces for bunny.ply
        vector<vec3f>   vertex_array; // 3502 vertex for bunny.ply
        vector<vec3f>   uvs_array;
        vector<vec3f>   shading_normals_array;
        shared_ptr<hittable_list> triangles_array;
        shared_ptr<material> poly_mat;
        axisAlignBbox bbox;

        // private methods
        void load_mesh(const string& fname) {
            PLYData plyIn(fname);
            auto varr = plyIn.getVertexPositions("vertex");
            for (auto v : varr){
                vertex_array.emplace_back(v[0], v[1], v[2]);
//                cout << v[0] << " | " << v[1] << " | " << v[2] << endl;
            }
//            cout << vertex_array.size() << endl;
            vertex_index_array = plyIn.getFaceIndices<int>();
//            for (auto idx : vertex_index_array){
//                cout << idx[0] << " | " << idx[1] << " | " << idx[2] << endl;
//            }
//            cout << vertex_index_array.size() << endl;
            for (auto v_tri : vertex_index_array) {
                auto v1 = vertex_array[v_tri[0]];
                auto v2 = vertex_array[v_tri[1]];
                auto v3 = vertex_array[v_tri[2]];
                triangles_array->append(make_shared<triangle>(v1, v2, v3, poly_mat));
            }
        }
    }; // class polygon_mesh
} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_POLYGONS_H
