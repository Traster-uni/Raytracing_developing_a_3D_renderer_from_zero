#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"
#include "r3df0_bvh.h"

using namespace std;
using namespace r3dfrom0;

int main() {
    hittable_list world;
//    matrix44f object_to_world = {};
    // make transformation
//    matrix44f object_to_world = make_translation_matrix(vec3f(2,3,0));
    // make scaling
//    auto object_to_world = make_scaling_matrix_xyz(vec3f(2.0f,2.0f,0));
    // make rotation
    auto object_to_world = make_rotation_matrix(50.0f, {1,0,0});
    auto is_matrix = is_rotation_matrix(object_to_world) ? true : false;
    // TODO: TEST WITH AN ACTUAL 3D OBJECT

    auto material1 = make_shared<lambertian>(pixel_f(1.0, 0.5, 0.0));
    auto material2 = make_shared<lambertian>(pixel_f(1.0f, 0.2f, 0.0f));

    auto v1 = vec3f(-2.5f,-2.0f, 0.0f);
    auto v2 = vec3f(2.5f,-2.0f,0.0f);
    auto v3 = vec3f(1.0f,1.0f, 0.0f);
    world.append(make_shared<triangle>(v1, v2, v3, material1, object_to_world));

    camera main_camera;
    main_camera.image_width = 800;
    main_camera.look_from = vec3f(0,0,5);
    main_camera.look_at   = vec3f(0,0,0);
    main_camera.view_up   = vec3f(0,1,0);
//    main_camera.camera_to_world = object_to_world;

    ofstream txt_out;
    if (txt_out.is_open()){
        cout << "file already open" << endl;
        return 1;
    }
    txt_out.open("matrix.txt", std::ios::out);
    txt_out << "triangle coords pre transform:\n["<< v1 << "]\n[" << v2 << "]\n["<< v3 << "]" << endl;
    txt_out << object_to_world << endl;
    auto vv1 = local_to_world(v1, object_to_world);
    auto vv2 = local_to_world(v2, object_to_world);
    auto vv3 = local_to_world(v3, object_to_world);
    txt_out << "triangle coords post transform:\n["<< vv1 << "]\n[" << vv2 << "]\n["<< vv3 << "]" << endl;
    txt_out.close();

    main_camera.render_png("rotation_test1_50x.png", world);

}