#include <vector>
#include "r3df0_math.h"
#include "r3df0_image.h"
#include "r3df0_color.h"
#include "r3df0_rays.h"
#include "r3df0_camera.h"
using namespace std;
using namespace r3dfrom0;

bool hit_sphere(const vec3f& sphere_center, const float& sphere_radius, const ray& r){
    auto c_q = sphere_center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = 2 * dot(r.direction(),c_q);
    auto c = dot(c_q, c_q) - (sphere_radius * sphere_radius);
    auto delta = b * b - 4 * (a * c);
    return delta >= 0;
}
pixel_f ray_color(const ray& r){
    /**
     * test ray intersection and ray sampling code
     */
    auto c = vec3f{0,0,-1};
    float d = 5.0;
    if (hit_sphere(c, d, r)){
        return {1,0,0};    // returns red pixel
    }

    auto unit_vec = unit(r.direction()).y;
    return lerp_color(unit_vec, pixel_f{0.0,0.0,0.0}, pixel_f{0.5, 0.7, 1.0});
}

int main(){
    camera main_camera;
    main_camera

    vector<vector<pixel_f>> img = {};
    for (int i = 0; i < 256; i++){
        vector<pixel_f> row = {};
        for (int j = 0; j<256; j++){
            pixel_f p(double(i) / (255), double(j) / (255), 0.0);
            row.push_back(p);
//          cout << "(" << p.r << " " << p.g << " " << p.b << ")" << " ";
        }
//        cout << "\n" << endl;
        img.push_back(row);
    }
    ppm_out(img, "hello_sphere.ppm");
}

