//
// Created by trast on 18/09/2024.
//
#include "r3df0_varsutil.h"
#include "r3df0_polygons.h"
#include "r3df0_camera.h"

using namespace std;
using namespace r3dfrom0;

int main() {
    hittable_list world;
    camera cam;

    auto bunny_mat = make_shared<lambertian>(pixel_f(255,0,0));

    auto bunny = make_shared<polygon_mesh>("../resources/models/bunny.ply", bunny_mat);
    world.append(bunny);
    // TODO: stack overflow
    cam.aspect_ratio      = 1.0;
    cam.image_width       = 1000;
    cam.vfov              = 90;

    cam.look_from = vec3f(0,2,20);
    cam.look_at   = vec3f(0,0,0);
    cam.view_up   = vec3f(0,1,0);

    cam.render_png("bunny.png",world);

}
