//
// Created by tommasomarialopedote on 25/08/24.
//
#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"
#include "r3df0_bvh.h"

using namespace std;
using namespace r3dfrom0;

int main(){
    hittable_list world;
    // RED = pixel_f(1.0, 0.2, 0.2)
    // GREEN = pixel_f(0.2, 1.0, 0.2)
    // BLUE = pixel_f(0.2, 0.2, 1.0)
    // ORANGE = pixel_f(1.0, 0.5, 0.0)
    // LIGHT BLUE = pixel_f(0.2, 0.8, 0.8)
    // Materials
    auto left_red     = make_shared<lambertian>(pixel_f(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(pixel_f(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(pixel_f(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(pixel_f(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(pixel_f(0.2, 0.8, 0.8));

    // Quads
//    world.append(make_shared<quad>(vec3f(-3,-2, 5), vec3f(0, 0,-4), vec3f(0, 4, 0), left_red));
//    world.append(make_shared<quad>(vec3f(-2,-2, 0), vec3f(4, 0, 0), vec3f(0, 4, 0), back_green));
//    world.append(make_shared<quad>(vec3f( 3,-2, 1), vec3f(0, 0, 4), vec3f(0, 4, 0), right_blue));
//    world.append(make_shared<quad>(vec3f(-2, 3, 1), vec3f(4, 0, 0), vec3f(0, 0, 4), upper_orange));
//    world.append(make_shared<quad>(vec3f(-2,-3, 5), vec3f(4, 0, 0), vec3f(0, 0,-4), lower_teal));
    // Triangles
//    world.append(make_shared<triangle>(vec3f(-2.5,-2, 0), vec3f(2.5,-2,0), vec3f(1,1, 0), back_green));
//    world.append(make_shared<triangle>(vec3f(-3,-2, 1), vec3f(-3, -2,5), vec3f(-3, 1, 4), left_red));
//    world.append(make_shared<triangle>(vec3f( 3,-2, 1), vec3f(3, -2, 5), vec3f(3, 1, 4), right_blue));
//    world.append(make_shared<triangle>(vec3f(-2.5, -2, 1), vec3f(2.5, -2, 1), vec3f(0, -2, 5), lower_teal));
//    world.append(make_shared<triangle>(vec3f(-2.5, 2, 1), vec3f(2.5, 2, 1), vec3f(0, 2, 5), upper_orange));
    // pyramid
    world.append(pyramid_sqr_base(vec3f(-2,-3, 5), vec3f(4, 0, 0), vec3f(0, 0,-4), vec3f(0,1,0), upper_orange));

    camera cam;
    cam.image_width     = 400;
    cam.aspect_ratio    = 1.0;
    cam.vfov            = 80;
    cam.look_from = vec3f(-3,4,9);
    cam.look_at   = vec3f(0,0,0);
    cam.view_up   = vec3f(0,1,0);
    cam.background = pixel_f(0.70, 0.80, 1.00);

    cam.render_png("triangleBox_test.png",world);
}
