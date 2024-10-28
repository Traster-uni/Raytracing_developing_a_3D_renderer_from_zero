//
// Created by trast on 04/10/2024.
//
#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"
#include "r3df0_bvh.h"
#include "r3df0_ftransforms.h"

using namespace std;
using namespace r3dfrom0;

int main(){
    generator();
    hittable_list world;
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(pixel_f(0.48, 0.83, 0.53));

        int boxes_per_side = 8;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -300 + i*w;
            auto z0 = -50 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_float(1,101);
            auto z1 = z0 + w;

            world.append(box_quad(vec3f(x0,y0,z0), vec3f(x1,y1,z1), ground));
        }
    }


//    hittable_list world;
//    world.append(make_shared<bvh_node>(boxes1));
    auto center1 = vec3f(400, 400, 200);


    // textures
    auto earth_tex = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
    auto perlin_tex = make_shared<perlin_noise_texture>(0.2);

    // materials
    auto light = make_shared<diffuse_light>(pixel_f(10, 10, 10));
    auto sphere_material = make_shared<lambertian>(pixel_f(0.7, 0.3, 0.1));

    // static objects
    world.append(make_shared<quad>(vec3f(123,554,147), vec3f(300,0,0), vec3f(0,0,265), light));
//    world.append(make_shared<sphere>(center1, 50, sphere_material));

    world.append(make_shared<sphere>(vec3f(400,250,400), 100, earth_tex));
    world.append(make_shared<sphere>(vec3f(220,280,300), 80, make_shared<lambertian>(perlin_tex)));

    world.append(make_shared<sphere>(vec3f(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.append(make_shared<sphere>(vec3f(0, 150, 145), 49, make_shared<metal>(pixel_f(0.5, 0, 0), .5f)));

    // blue glass
    auto boundary = make_shared<sphere>(vec3f(360,150,145), 70, make_shared<dielectric>(1.5));
    world.append(boundary);
    boundary = make_shared<sphere>(vec3f(360,150,145), 65, make_shared<metal>(pixel_f(0.2, 0.4, 0.9), 0.2f));
    world.append(boundary);


    // box of spheres
    hittable_list boxes2;
    auto white = make_shared<lambertian>(pixel_f(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {

        boxes2.append(make_shared<sphere>(vec3f().random(.0f,165.0f), 10, white));
    }

    world.append(make_shared<translate>(
                      make_shared<rotate_y>(
                              make_shared<bvh_node>(boxes2), 15),
                      vec3f(-100,270,395)
              )
    );

    world.append(make_shared<bvh_node>(world));

    camera cam;
    // int image_width, int samples_per_pixel, int max_depth
    // 400,   250,  4
    // 800, 10000, 40
    cam.image_width = 800;
    cam.samples_number = 300;
    cam.max_recursion_depth = 30;
//    cam.image_width = 400;
//    cam.samples_number = 100;
//    cam.max_recursion_depth = 4;
    cam.aspect_ratio      = 1.0;
    cam.background        = pixel_f(0,0,0);

    cam.vfov        = 40;
    cam.look_from   = vec3f(478, 278, -600);
    cam.look_at     = vec3f(278, 278, 0);
    cam.view_up     = vec3f(0,1,0);


    cam.defocus_angle = 0;

    cam.render_png("complex_final_render.png",world);
}