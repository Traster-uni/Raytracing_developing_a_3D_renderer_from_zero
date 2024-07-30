//
// Created by tommasomarialopedote on 13/07/24.
//

#ifndef R3DFROM0_R3DF0_CAMERA_H
#define R3DFROM0_R3DF0_CAMERA_H

#include <string>
#include "r3df0_varsutil.h"
#include "r3df0_math.h"
#include "r3df0_hittable.h"

namespace r3dfrom0{

    class camera{
    public: // public attributes
        int image_width = 1920; // defaults to fullHD, 1920w x 1080h
        float aspect_ratio = 1.777778; // defaults to 16:9
        int samples_number = 10; // number of sampling points within a single pixel

        // constructor
        camera() {}

        // public methods
        pixel_f ray_color(const ray& r, hittable_list& worldList){
            //initialize hit record
            hit_record hitRecord;
            interval i(0, infinity);
            if (worldList.hit(r, i, hitRecord)){
                return color_map(hitRecord.normal);
            }

            auto unit_vec = 0.5f * (unit(r.direction()).y + 1.0f);
            return lerp_color(unit_vec, pixel_f{0.0,0.0,0.0}, pixel_f{0.5, 0.7, 1.0});
        }

        void render(const string& fname, hittable_list& world, const vec3f& ep = {0,0,0}, const float& fl = 1.0){
            initialize(ep, fl);

            ofstream out;
            out.open(fname);
            // ppm header
            out << "P3\n" << image_width << " " << image_height << "\n255\n" <<endl;
            // ppm body render
            for (int i = 0; i < image_height; i++){
                clog << "\rScanlines remaining: " << i << "/" << image_height << flush;
                for (int j = 0; j < image_width; j++){

//                    pixel_f color_sum(0.0f,0.0f,0.0f);
//                    for (int s = 0; s < samples_number; s++){
//                        ray r = get_ray(i,j);
//                        color_sum += ray_color(r, world);
//                    }
//                    auto mean_color = color_sum * mean_factor; // (sum(colors rays))/number_of_rays)
//                    cout << "color_sum: " << color_sum << " | " << "mean_color: " << mean_color << endl;

                    // color sampling without antialiasing
                    auto d = pixel_center(i, j) - eye_point;
                    auto r = ray(eye_point, d);
                    auto mean_color = ray_color(r, world);
                    cout << mean_color << endl;
                    auto p_i = convert_pixel_i(mean_color); // clamps colors and converts into integers
                    cout << p_i << endl;
                    out << p_i << " ";
                }
                out << endl;
            }
            out.close();
            clog << "\r< -------------- Done -------------- >\n" << flush;
        }

    private: // private attributes
        int image_height;
        vec3f eye_point;            // origin of the camera
        float focal_length;         // distance from origin to viewport
        float viewport_width;
        float viewport_height;
        vec3f viewport_upper_left;  // coordinates of the most upper left corner
        vec3f viewport_u;           // viewport vector, x axis, left to right
        vec3f viewport_v;           // viewport vector, -y axis, top to bottom
        vec3f pixel00_location;     // origin of the pixel grid
        vec3f pixel_delta_u;        // pixel offset to the right
        vec3f pixel_delta_v;        // pixel offset down
        float mean_factor;

        // private methods
        void initialize(const vec3f& ep = {0,0,0}, const float& fl = 1.0){
            eye_point = ep;
            focal_length = fl;

            mean_factor = 1.0f / samples_number;

            // define image resolution
            auto h = int(float (image_width) / aspect_ratio); // h = w * (1/aspect_ratio)
            image_height = h < 1 ? 1 : h; // image_height must be at least 1

            // define viewport dimension
            viewport_height = 2.0f;  // TODO: MAKE VIEWPORT DIMENSIONS FULLY COMPUTABLE
            viewport_width = viewport_height * (float(image_width) / float (image_height));

            // viewport vectors
            viewport_u = {viewport_width, 0, 0};
            viewport_v = {0, -viewport_height, 0};

            // pixel deltas, small vector that define a step to the next pixel of the viewport
            pixel_delta_u = viewport_u / float (image_width);
            pixel_delta_v = viewport_v / float (image_height);

            // location of upper left corner pixel
            viewport_upper_left = eye_point - vec3f{0,0,focal_length} - viewport_u/2 - viewport_v / 2;
            pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(const int& i, const int& j) {
            auto offset = sample_square();
            // pixel_center with added random offsets
            auto sampled_pixel = pixel00_location + ((i * offset.x) * pixel_delta_u) + ((j * offset.y) * pixel_delta_v);
            auto ray_origin = eye_point;
            auto ray_direction = sampled_pixel - ray_origin;
            return {ray_origin, ray_direction};
        }

        vec3f pixel_center(const int& i, const int& j){
            // returns coordinates for center of pixel, used to cast rays from
            // said pixel
            return pixel00_location + (i * pixel_delta_v) + (j * pixel_delta_u);
        }

        vec3f sample_square(){
            // returns a random point in the unit square(pixel) in bounds [-0.5, 0.5]x and
            // [-0.5, 0.5]y
            return vec3f{random_float() - 0.5f, random_float() - 0.5f, 0};
        }
    };
} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_CAMERA_H
