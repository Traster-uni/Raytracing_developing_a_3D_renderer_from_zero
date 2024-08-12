//
// Created by tommasomarialopedote on 06/08/24.
//
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"


using namespace std;
using namespace r3dfrom0;


int main(){
    // Initialize world
    hittable_list world;
    auto R = cos(pi/4);

    auto material_left  = make_shared<lambertian>(pixel_f(0,0,1));
    auto material_right = make_shared<lambertian>(pixel_f(1,0,0));

    world.append(make_shared<sphere>(vec3f(-R, 0, -1), R, material_left));
    world.append(make_shared<sphere>(vec3f( R, 0, -1), R, material_right));

    // Initialize camera
    camera main_camera;
    main_camera.vfov = 90;
    main_camera.render("new_camera_tech_test_90.pmm", world);
}
