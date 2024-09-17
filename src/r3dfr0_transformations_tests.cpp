#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"
#include "r3df0_bvh.h"

using namespace std;
using namespace r3dfrom0;

matrix44f inverse(matrix44f& t)
{
    int i, j, k;
    matrix44f s;

    // Forward elimination
    for (i = 0; i < 3 ; i++) {
        int pivot = i;

        auto pivotsize = t[i][i];

        if (pivotsize < 0)
            pivotsize = -pivotsize;

        for (j = i + 1; j < 4; j++) {
            auto tmp = t[j][i];

            if (tmp < 0)
                tmp = -tmp;

            if (tmp > pivotsize) {
                pivot = j;
                pivotsize = tmp;
            }
        }

        if (pivotsize == 0) {
            // Cannot invert singular matrix
            return matrix44f();
        }

        if (pivot != i) {
            for (j = 0; j < 4; j++) {
                float tmp;

                tmp = t[i][j];
                t[i][j] = t[pivot][j];
                t[pivot][j] = tmp;

                tmp = s[i][j];
                s[i][j] = s[pivot][j];
                s[pivot][j] = tmp;
            }
        }

        for (j = i + 1; j < 4; j++) {
            auto f = t[j][i] / t[i][i];

            for (k = 0; k < 4; k++) {
                t[j][k] -= f * t[i][k];
                s[j][k] -= f * s[i][k];
            }
        }
    }

    // Backward substitution
    for (i = 3; i >= 0; --i) {
        float f;

        if ((f = t[i][i]) == 0) {
            // Cannot invert singular matrix
            return matrix44f();
        }

        for (j = 0; j < 4; j++) {
            t[i][j] /= f;
            s[i][j] /= f;
        }

        for (j = 0; j < i; j++) {
            f = t[j][i];

            for (k = 0; k < 4; k++) {
                t[j][k] -= f * t[i][k];
                s[j][k] -= f * s[i][k];
            }
        }
    }

    return s;
}

int main() {
    hittable_list world;

    matrix44f m = {vec4f(0.707107, 0, -0.707107, 0),
                   vec4f(-0.331295, 0.883452, -0.331295, 0),
                   vec4f(0.624695, 0.468521, 0.624695, 0),
                   vec4f(4.000574, 3.00043, 4.000574, 1)};

//    0.707107  -0.331295 0.624695  0
//    0 0.883452 0.468521   0
//    -0.707107 -0.331295 0.624695  0
//    0             0     -6.404043 1
    auto m1 = m;
    auto m2 = m;
    m1 = invert(m1);
    m2 = inverse(m2);
    cout << m1 << endl;
    cout << m2 << endl;
    cout << m << endl;

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
    main_camera.background = pixel_f(0.70, 0.80, 1.00);
//    main_camera.camera_to_world = object_to_world;

    ofstream txt_out;
    if (txt_out.is_open()){
        cout << "file already open" << endl;
        return 1;
    }
    txt_out.open("matrix.txt", std::ios::out);
    txt_out << "triangle coords pre transform:\n["<< v1 << "]\n[" << v2 << "]\n["<< v3 << "]" << endl;
    txt_out << object_to_world << endl;
    auto vv1 = transform_point(v1, object_to_world);
    auto vv2 = transform_point(v2, object_to_world);
    auto vv3 = transform_point(v3, object_to_world);
    txt_out << "triangle coords post transform:\n["<< vv1 << "]\n[" << vv2 << "]\n["<< vv3 << "]" << endl;
    txt_out.close();

//    main_camera.render_png("rotation_test1_50x.png", world);

}