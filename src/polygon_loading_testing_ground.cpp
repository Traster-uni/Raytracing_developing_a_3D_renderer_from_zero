//
// Created by trast on 18/09/2024.
//
#include "r3df0_varsutil.h"
#include "r3df0_polygons.h"
#include "r3df0_camera.h"

using namespace std;
using namespace r3dfrom0;

int main(int argc, char* argv[]) {
//    auto filein = string (argv[1]);
    string filein = "C:\\Raytracing_developing_a_3D_renderer_from_zero\\resources\\models\\bunny.ply";
    string fileout = "bunny500.png";
//    auto fileout = string(argv[2]);
    hittable_list world;
    camera cam;
    // materials
    auto bunny_mat = make_shared<lambertian>(pixel_f(255,0,0));
    auto light_mat = make_shared<diffuse_light>(4);
    auto ground_mat = make_shared<lambertian>(pixel_f{100, 100, 100});
    // object
    auto bunny = make_shared<polygon_mesh>(filein, bunny_mat);
    auto light_sphere = make_shared<sphere>(vec3f(0,10,-2), 2, light_mat);
    auto ground = make_shared<sphere>(vec3f(0,-1000,0), 990, ground_mat);

    world.append(light_sphere);
    world.append(ground);
    world.append(bunny);

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.vfov              = 90;

    cam.samples_number = 900;

    cam.look_from = vec3f(0,10,20);
    cam.look_at   = vec3f(0,0,0);
    cam.view_up   = vec3f(0,1,0);
    cam.background = pixel_f(0.70, 0.80, 1.00);
    cam.background = pixel_f(.0f,.0f,.0f);
    cam.render_png(fileout,world);

}
