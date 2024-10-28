
#include "r3df0_varsutil.h"
#include "r3df0_material.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"
#include "r3df0_bvh.h"
#include "r3df0_ftransforms.h"

using namespace std;
using namespace r3dfrom0;

int main() {

    hittable_list world;
//    hittable_list box0;

    auto red   = make_shared<lambertian>(pixel_f(.65f, .05f, .05f));
    auto white = make_shared<lambertian>(pixel_f(.73f, .73f, .73f));
    auto green = make_shared<lambertian>(pixel_f(.12f, .45f, .15f));
    auto metal_mat = make_shared<metal>(pixel_f(.12f,.45f,.15f), .1f);
    auto light = make_shared<diffuse_light>(pixel_f(15, 15, 15));


    world.append(make_shared<quad>(vec3f(555,0,0),      vec3f(0,555,0), vec3f(0,0,555), green));
    world.append(make_shared<quad>(vec3f(0,0,0),        vec3f(0,555,0), vec3f(0,0,555), red));
    world.append(make_shared<quad>(vec3f(343, 554, 332),vec3f(-130,0,0),vec3f(0,0,-105),light));
    world.append(make_shared<quad>(vec3f(0,0,0),        vec3f(555,0,0), vec3f(0,0,555), white));
    world.append(make_shared<quad>(vec3f(555,555,555),  vec3f(-555,0,0),vec3f(0,0,-555),white));
    world.append(make_shared<quad>(vec3f(0,0,555),      vec3f(555,0,0), vec3f(0,555,0), white));

    hittable_list box_bbox1;
    shared_ptr<hittable> b1 = box_quad(vec3f(0, 0, 0), vec3f(165,330,165), white);
    b1 = make_shared<rotate_y>(b1, 15);
    b1 = make_shared<translate>(b1, vec3f(265,0,295));
    box_bbox1.append(b1);

    hittable_list box_bbox2;
    shared_ptr<hittable> b2 = box_quad(vec3f(0,0,0), vec3f(165,165,165), white);
    b2 = make_shared<rotate_y>(b2, -18);
    b2 = make_shared<translate>(b2, vec3f(130,0,65));
    box_bbox2.append(b2);
    // MIRROR PYRAMID
//    hittable_list py_bbox;
//    py_bbox.append(pyramid_sqr_base(vec3f(380, 0, 80), vec3f(100, 0, 0), vec3f(0,0,100), vec3f(0,300,0), metal_mat));
//    world.append(pyramid_sqr_base(vec3f(130, 0, 65), vec3f(200, 0, 0), vec3f(0,0,200), vec3f(0,200,0), metal_mat));

    world.append(make_shared<bvh_node>(box_bbox1));
    world.append(make_shared<bvh_node>(box_bbox2));
//    world.append(make_shared<bvh_node>(py_bbox));
    world.append(make_shared<bvh_node>(world));
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

    cam.render_png("cornell_box.png",world);
}
