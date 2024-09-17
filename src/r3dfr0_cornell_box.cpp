
#include "r3df0_varsutil.h"
#include "r3df0_material.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"

using namespace std;
using namespace r3dfrom0;

int main() {

    hittable_list world;

    auto red   = make_shared<lambertian>(pixel_f(.65, .05, .05));
    auto white = make_shared<lambertian>(pixel_f(.73, .73, .73));
    auto green = make_shared<lambertian>(pixel_f(.12, .45, .15));
    auto light = make_shared<diffuse_light>(pixel_f(15, 15, 15));

    world.append(make_shared<quad>(vec3f(555,0,0),      vec3f(0,555,0), vec3f(0,0,555), green));
    world.append(make_shared<quad>(vec3f(0,0,0),        vec3f(0,555,0), vec3f(0,0,555), red));
    world.append(make_shared<quad>(vec3f(343, 554, 332),vec3f(-130,0,0),vec3f(0,0,-105),light));
    world.append(make_shared<quad>(vec3f(0,0,0),        vec3f(555,0,0), vec3f(0,0,555), white));
    world.append(make_shared<quad>(vec3f(555,555,555),  vec3f(-555,0,0),vec3f(0,0,-555),white));
    world.append(make_shared<quad>(vec3f(0,0,555),      vec3f(555,0,0), vec3f(0,555,0), white));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 1000;
    cam.samples_number    = 500;
    cam.max_recursion_depth  = 100;

    cam.vfov      = 40;
    cam.look_from = vec3f(278, 278, -800);
    cam.look_at   = vec3f(278, 278, 0);
    cam.view_up   = vec3f(0,1,0);

    cam.defocus_angle = 0;

    cam.render_png("empty_cornell_box.png",world);
}
