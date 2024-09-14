//
// Created by trast on 21/08/2024.
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

int main() {
    hittable_list world;
//    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_texture = make_shared<spacial_texture>(convert_pixel_f_clamp(pixel_i{235, 231, 16}));
    // wrong color sampling
    // TODO: image_texture DOESN'T WORK, LOOK INTO IT
    auto earth_surface = make_shared<lambertian>(pixel_f{0.4,0.2,0.1});
    auto globe = make_shared<sphere>(vec3f(50,50,50), 20, earth_surface);
    world.append(globe);

    camera main_camera;

    main_camera.vfov     = 20;
    main_camera.look_from = vec3f(0,0,12);
    main_camera.look_at   = vec3f(50,50,50);
    main_camera.view_up   = vec3f(0,1,0);
    main_camera.background = pixel_f(0.70, 0.80, 1.00);
    main_camera.defocus_angle = 0;

    main_camera.render_png("wtt.png", world);
}

