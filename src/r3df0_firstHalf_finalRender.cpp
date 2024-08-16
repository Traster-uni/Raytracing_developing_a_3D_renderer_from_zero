//
// Created on 12/08/24, code featured as an end-of-book scene
//
#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"


using namespace std;
using namespace r3dfrom0;

int main(){
    hittable_list world;
    // set ground
    auto ground_material = make_shared<lambertian>(pixel_f(0.5,0.5,0.5));
    auto ground = make_shared<sphere>(vec3f(0,-1000,0), 1000, ground_material);
    world.append(ground);

    // set random sphere in world
    for (int i = -11; i < 11; i++){
        for (int j = -11; j < 11; j++){
            auto rdm_selector = random_float();
            vec3f center(i + 0.9*random_float(), 0.2, j + 0.9*random_float());

            if ((center - vec3f(4, 0.2, 0)).length() > 0.9f){
                shared_ptr<material> rdm_material;

                if (rdm_selector < 0.8f){
                    // diffuse
                    pixel_f rdm_albedo = {random_float(), random_float(), random_float()};
                    rdm_material = make_shared<lambertian>(rdm_albedo);
                    world.append(make_shared<sphere>(center, 0.2, rdm_material));
                } else if (rdm_selector < 0.95f){
                    // metal
                    float rdm_fuzz = random_float(0.5, 1);
                    pixel_f rdm_albedo = {random_float(0,0.5), random_float(0,0.5), random_float(0,0.5)};
                    rdm_material = make_shared<metal>(rdm_albedo, rdm_fuzz);
                    world.append(make_shared<sphere>(center, 0.2, rdm_material));
                } else {
                    // glass
                    rdm_material = make_shared<dielectric>(1.5);
                    world.append(make_shared<sphere>(center, 0.2, rdm_material));
                }
            }
        }
    }

    // set fixed elements in world
    auto material1 = make_shared<dielectric>(1.5);
    world.append(make_shared<sphere>(vec3f(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(pixel_f(0.4, 0.2, 0.1));
    world.append(make_shared<sphere>(vec3f(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(pixel_f(0.7, 0.6, 0.5), 0.0);
    world.append(make_shared<sphere>(vec3f(4, 1, 0), 1.0, material3));

    // camera setting
    camera main_camera;

    main_camera.aspect_ratio        = 16.0 / 9.0;
    main_camera.image_width         = 1920;
    main_camera.samples_number      = 500;
    main_camera.max_recursion_depth = 50;

    main_camera.vfov                = 20;
    main_camera.look_from           = vec3f(13,2,3);
    main_camera.look_at             = vec3f(0,0,0);
    main_camera.view_up             = vec3f(0,1,0);

    main_camera.defocus_angle       = 0.6;
    main_camera.focus_dist          = 10.0;

    main_camera.render("final_render.pmm",world);
}
