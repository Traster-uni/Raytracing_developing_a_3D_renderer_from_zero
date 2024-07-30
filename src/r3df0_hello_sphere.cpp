#include <vector>
#include "r3df0_math.h"
#include "r3df0_image.h"
#include "r3df0_color.h"
#include "r3df0_rays.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"

using namespace std;
using namespace r3dfrom0;

float hit_sphere_old(const vec3f& sphere_center, const float& sphere_radius, const ray& r){
    /**
     * First implementation of the sphere hit function as described in the book checks only
     * if the ray is into the sphere
     */
    auto c_q = sphere_center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = 2 * dot(r.direction(),c_q);
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
        return (-h - sqrt(delta)) / a;
    }
}

pixel_f ray_color_old(const ray& r){
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

pixel_f ray_color(const ray& r, hittable_list& worldList){
    //initialize hit record
    hit_record hitRecord;
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
    auto sph1 = sphere(vec3f{0,0,-1}, 1);
//    auto sph1 = sphere(vec3f{-0.6,0,-3}, 1.5);
//    auto sph2 = sphere(vec3f(100.5,0,-1), 100);
//    auto sph2 = sphere(vec3f{0.2,0,-1}, 0.3);
    // make_shared is a method to create a pointer of shared pointer array
    world.append(make_shared<sphere>(sph1));
//    world.append(make_shared<sphere>(sph2));

    // Initialize camera
    camera main_camera;
    main_camera.render("hello_sphere.pmm", world);
}

