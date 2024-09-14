

#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_material.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"

using namespace std;
using namespace r3dfrom0;

int main(){
    hittable_list world;

    auto floor_texture = make_shared<spacial_texture>(.5f, .6f, .7f);
    world.append(make_shared<sphere>(vec3f(0,-1000,0), 1000, make_shared<lambertian>(floor_texture)));
    world.append(make_shared<sphere>(vec3f(0,2,0), 2, make_shared<lambertian>(pixel_f{.5f, .6f, .7f})));
    world.append(make_shared<quad>(vec3f(-6,0,4), vec3f(6,0,4), vec3f(6,10,4), vec3f(-6,10,4), make_shared<lambertian>(pixel_f{.5f, .6f, .7f})));

    auto square_light = make_shared<diffuse_light>(4.0f);
//    world.append(make_shared<quad>(vec3f(3,1,-2), vec3f(2,0,0), vec3f(0,2,0), square_light));
    auto sphere_light = make_shared<diffuse_light>(8.0f);
    world.append(make_shared<sphere>(vec3f(0,10,0), 1, sphere_light));

    camera cam;

    cam.vfov = 30;
    cam.samples_number = 800;
    cam.look_from = vec3f(0,3.5,-20);
    cam.look_at   = vec3f(0,2,0);
    cam.view_up   = vec3f(0,1,0);

    cam.render_png("simple_emission_scene.png",world);
}