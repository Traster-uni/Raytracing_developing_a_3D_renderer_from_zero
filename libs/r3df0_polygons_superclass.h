//
// Created by trast on 16/09/2024.
//

#ifndef R3DFROM0_R3DF0_POLYGONS_SUPERCLASS_H
#define R3DFROM0_R3DF0_POLYGONS_SUPERCLASS_H

#include "r3df0_varsutil.h"
#include "r3df0_hittable_superclass.h"
#include "plycpp/plycpp/plycpp_runner.h"

using namespace std;

namespace r3dfrom0 {
    // As per described in: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-polygon-mesh/polygon-mesh-file-formats.html
    // plus adaptations made to make it compatible with the current implementation of the renderer
    class polygon_mesh : public hittable {
    public:
        // constructor
        polygon_mesh() {
            // TODO: FIGURE OUT HOW TO SET UP INSTANCE
            // TODO: OPEN FILE FOR SPECIFIC OBJ FORMAT
        };

        void load_mesh(const string fname) {

        }

        // public methods
    private:
        int num_vertex;
        int num_faces;          // you may need to consider using vectors
        int face_index_array;   // NEEDS DIMENSION
        int vertex_index_array; // NEEDS DIMENSION
        vec3f vertex_array;     // NEEDS DIMENSION
        vec3f uvs_array;        // NEEDS DIMENSION
    }; // class polygon_mesh
} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_POLYGONS_SUPERCLASS_H
