#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"


using namespace std;
using namespace r3dfrom0;

float hit_sphere_old(const vec3f& sphere_center, const float& sphere_radius, const ray& r){
    /**
     * First implementation of the sphere hit function as described in the book checks only
     * if the ray is into the sphere
     * P = Q - d*t with Q ray origin, d ray direction and t step
     */
    auto c_q= sphere_center - r.origin(); // C - Q
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0f * dot(r.direction(),c_q);
    auto c = dot(c_q, c_q) - (sphere_radius * sphere_radius);
    auto delta = b * b - 4 * (a * c);
    if (delta < 0){
        return -1.0;
    }else{
        return (-b - sqrt(delta)) / float(2.0 * a);
    }
}

float hit_sphere(const vec3f& sphere_center, const float& sphere_radius, const ray& r){
    /**
     * Same then before but the formula has been optimized by reducing factors in
     * the quadratic equation.
     * Final implementation in the sphere class in r3df0_shapes
     */
    auto c_q = sphere_center - r.origin();
    // optimized the dot function of a vector to itself is, sqr_length of said vector
    auto a = r.direction().sqr_length();
    // b in old func is now d * (C - Q)
    auto h = dot(r.direction(), c_q);
    // optimized the dot function of a vector to itself is, sqr_length of said vector
    auto c = c_q.sqr_length() - (sphere_radius * sphere_radius);
    // delta optimized by algebraic simplification
    auto delta = h * h - a * c;
    if (delta < 0){
        return -1.0;
    }else{
        return (h - sqrt(delta)) / a;
    }
}

pixel_f ray_color_oldest(const ray& r){
    /**
     * test ray intersection and ray sampling code
     * before hittable_list implementation
     */
    auto c = vec3f{0,0,-1};
    float d = 5.0;
    auto sph = hit_sphere(c, d, r);
    if (sph > 0.0){
        auto uv_length = unit(r.at(sph) - vec3f(0,0,-1.0));
        return color_map(uv_length);
    }

    auto unit_vec = unit(r.direction()).y;
    return lerp_color(unit_vec, pixel_f{0.0,0.0,0.0}, pixel_f{0.5, 0.7, 1.0});
}

pixel_f ray_color_old(const ray& r, hittable_list& worldList){
    //initialize hit record
    r3dfrom0::hit_record hitRecord;
    interval i(0, infinity);
    if (worldList.hit(r, i, hitRecord)){
        return color_map(hitRecord.normal);
    }

    auto unit_vec = unit(r.direction()).y;
    return lerp_color(unit_vec, pixel_f{0.0,0.0,0.0}, pixel_f{0.5, 0.7, 1.0});
}


int main(){
    // Initialize world
    hittable_list world;
//    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto material_ground = make_shared<lambertian>(pixel_f(0.8, 0.8, 0.0)); // test here
    //pixel_f(0.8, 0.8, 0.0)
    auto material_center = make_shared<lambertian>(pixel_f(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50f);     // air bubble! 1.0f theta of air, 1.33f theta of water
    auto material_bubble = make_shared<dielectric>(1.0 / 1.3333);
    auto material_right      = make_shared<metal>(pixel_f(0.8, 0.6, 0.2), 0.7);

    world.append(make_shared<sphere>(vec3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.append(make_shared<sphere>(vec3f( 0.0,    0.0, -1.2),   0.5, material_center));
    world.append(make_shared<sphere>(vec3f(-1.0,    0.0, -1.0),   0.5, material_left));
    world.append(make_shared<sphere>(vec3f(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.append(make_shared<sphere>(vec3f( 1.0,    0.0, -1.0),   0.5, material_right));

    // Initialize camera
    camera main_camera;
    main_camera.defocus_angle = 10;
    main_camera.image_width   = 1920;
    main_camera.focus_dist    = 3.4;
    main_camera.vfov = 20;
    main_camera.look_from   = vec3f(-2,2,1);
    main_camera.look_at     = vec3f(0,0,-1);
    main_camera.view_up     = vec3f(0,1,0);
    main_camera.render_png("png_write_test.png", world);
}

