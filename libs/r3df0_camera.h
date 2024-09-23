//
// Created by tommasomarialopedote on 13/07/24.
//

#ifndef R3DFROM0_R3DF0_CAMERA_H
#define R3DFROM0_R3DF0_CAMERA_H

#include "r3df0_varsutil.h"
#include "r3df0_hittable.h"
#include "r3df0_material.h"
#include "r3df0_image.h"


namespace r3dfrom0{

    class camera{
    public:
        // public attributes
        int image_width = 1920;         // defaults to fullHD, 1920w x 1080h
        float aspect_ratio = 1.777776;  // defaults to 16:9
        // added 6/08: vertical fov, camera_frame(u,v,w), looking_frame
        int vfov = 90;
        // ray sampling parameters
        int samples_number = 100;       // number of sampling points within a single pixel
                                        // when using lights, you should use at least 500 samples per pixel.
        int max_recursion_depth = 50;   // limits the number of recursive calls
        // looking frame - camera orientation
        vec3f look_from;    // camera reference
        vec3f look_at;      // objective reference
        vec3f view_up;      // vertical reference
        // lens
        float focus_dist = 10.0f;       // distance between camera eye and focus plane
        float defocus_angle = 0;        // angle of variation from each pixels
        // transform
        matrix44f camera_to_world;
        // background fake light
        pixel_f background = pixel_f(.0f, .0f,.0f);

        // constructor
        camera() {}

        // public methods
        pixel_f ray_color_old(const ray& r, hittable_list& worldList, const int& recursive_depth){
            // ray_color loop not accounting for light emitting materials
            if (recursive_depth <= 0){ // end of recursion check
                return {0,0,0};
            }

            hit_record hitRecord;                  // initialize hit record
            interval i(0.01f, infinity);    // clamp rays too close to object surface
            if (worldList.hit(r, i, hitRecord)){
                pixel_f attenuation;
                ray scatter;
                if (hitRecord.material_ptr->scatter(r, hitRecord, scatter, attenuation)){
                    // attenuation is used to reduce the light reflected by said object, related to albedo
                    return attenuation * ray_color_old(scatter, worldList, recursive_depth-1);
                }
                return {0,0,0};
            }

            auto unit_vec = unit(r.direction()).y;
            return lerp_color(unit_vec, pixel_f{1.0,1.0,1.0}, pixel_f{0.5, 0.7, 1.0});
        }


        pixel_f ray_color(const ray& r, hittable_list& worldList, const int& recursive_depth){
            if (recursive_depth <= 0){ // end of recursion check
//                return {0,0,0};
                return background;
            }

            hit_record hitRecord;                  // initialize hit record
            interval i(0.01f, infinity);    // clamp rays too close to object surface
            if (!worldList.hit(r, i, hitRecord)) { // if ray doesn't hit geometry
                return background;                  // return background color
            }
            // else
            ray scatter;
            pixel_f attenuation;

            // calculate material emission
            auto color_emission = hitRecord.material_ptr->emitted(hitRecord.u, hitRecord.v, hitRecord.position);
            if (!hitRecord.material_ptr->scatter(r, hitRecord, scatter, attenuation)){
                // object didn't scatter ray, so it emits light: pixel takes color of emission
                return color_emission;
            }

            // attenuation is used to reduce the light reflected by said object, related to albedo
            pixel_f color_scatter = attenuation * ray_color(scatter, worldList, recursive_depth-1);
            return color_scatter + color_emission;
        }


        void render_ppm(const string& fname, hittable_list& world){
            initialize();

            auto const start_timer{steady_clock::now()};
            ofstream out;
            out.open(fname);
            // ppm header
            out << "P3\n" << image_width << " " << image_height << "\n255\n" <<endl;
            // ppm body render
            for (int i = 0; i < image_height; i++){ // for each row
                clog << "\rScanlines remaining: " << i << "/" << image_height << flush;
                for (int j = 0; j < image_width; j++){ // for each column
                    pixel_f color_sum(0.0f,0.0f,0.0f);
                    for (int s = 0; s < samples_number; s++){   // for random squares in pixel
                        ray r = get_ray(i,j);
                        color_sum += ray_color(r, world, max_recursion_depth);
                    }
                    auto mean_color = color_sum * mean_factor; // (sum(colors rays))/number_of_rays)
                    write_color_ppm(out, mean_color); // gamma, clamp and write on file
                }
                out << endl;
            }
            out.close();
            // clock
            auto const end_timer {steady_clock::now()};
            const duration<float> elapsed_time {end_timer - start_timer};
            clog << "\rRENDERING DONE IN: " << int(elapsed_time.count()) << " seconds" << flush;
        }

        void render_png(const string& fname, hittable_list& world) {
            initialize();

            auto const start_timer{steady_clock::now()};
            vector<vector<pixel_i>> tuple_image = {};
            for (int i = 0; i < image_height; i++){ // for each row
                clog << "\rScanlines remaining: " << i << "/" << image_height << flush;
                vector<pixel_i> scanline = {};
                tuple_image.push_back(scanline);
                for (int j = 0; j < image_width; j++){ // for each column
                    pixel_f color_sum(0.0f,0.0f,0.0f);
                    for (int s = 0; s < samples_number; s++){   // for random squares in pixel
                        ray r = get_ray(i,j);
                        color_sum += ray_color_old(r, world, max_recursion_depth);
                    }
                    auto mean_color = color_sum * mean_factor; // (sum(colors rays))/number_of_rays)
                    auto correct_color = correct_color_gamma(mean_color);
                    tuple_image[i].push_back(correct_color);
                }
            }
            write_png(fname, image_width, image_height, tuple_image);
            // clock
            auto const end_timer {std::chrono::steady_clock::now()};
            const duration<float> elapsed_time {end_timer - start_timer};
            clog << "\rRENDERING DONE IN: " << int(elapsed_time.count()) << " seconds" << flush;
        }

    private: // private attributes
        int image_height;
        vec3f eye_point;            // origin of the camera
//        float focal_length;         // REDACTED: distance from origin to viewport

        float viewport_width;
        float viewport_height;

        vec3f viewport_upper_left;  // coordinates of the most upper left corner
        vec3f viewport_u;           // viewport vector, x axis, left to right
        vec3f viewport_v;           // viewport vector, -y axis, top to bottom

        vec3f pixel00_location;     // origin of the pixel grid
        vec3f pixel_delta_u;        // pixel offset to the right
        vec3f pixel_delta_v;        // pixel offset down

        vec3f defocus_disk_u;       // defocus disk vector horizontal
        vec3f defocus_disk_v;       // defocus disk vector vertical
        float mean_factor;
        // camera frame
        vec3f u, v, w;

        // private methods
        void initialize(){
            eye_point = look_from;
//            focal_length = (look_from - look_at).length(); // REDACTED

            mean_factor = 1.0f / samples_number;

            // invert transform matrix
            camera_to_world = invert(camera_to_world);

            // define image resolution
            auto img_h = int(float (image_width) / aspect_ratio); // h = w * (1/aspect_ratio)
            image_height = img_h < 1 ? 1 : img_h; // image_height must be at least 1

            // define viewport dimensions
            float vfov_rad = degrees_to_radians(vfov);
            float h = tan(vfov_rad/2);
            viewport_height = 2.0f * h * focus_dist;
            viewport_width = viewport_height * (float(image_width) / float (image_height));

            // define camera frame
            w = unit(look_from-look_at);        // z axis
            u = unit(cross(view_up, w));  // y axis
            v = cross(w,u);                  // x axis

            // viewport vectors
            viewport_u = viewport_width * u;
            viewport_v = viewport_height * -v;

            // pixel deltas, small vector that define a step to the next pixel of the viewport
            pixel_delta_u = viewport_u / float (image_width);
            pixel_delta_v = viewport_v / float (image_height);

            // location of upper left corner pixel
            viewport_upper_left = eye_point - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // calculate the defocus disk dimensions;
            float defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle/2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        ray get_ray(const int& i, const int& j) {
            auto offset = sample_square();
            // pixel_center with added random offsets
            auto sampled_pixel = pixel00_location + ((i+offset.y) * pixel_delta_v) + ((j+offset.x) * pixel_delta_u);
            // origin sampling for fringe lens effect
            auto ray_origin = defocus_angle <= 0 ? eye_point : defocus_disk_sample();
            auto ray_direction = sampled_pixel - ray_origin;
            return {ray_origin, ray_direction};
        }

        vec3f defocus_disk_sample(){
            // https://www.reddit.com/r/GraphicsProgramming/comments/c35jmw/ray_tracing_in_a_weekend_depth_of_field/
            // DEFOCUS BLUR: http://users.eecs.northwestern.edu/~ollie/eecs395/HW6/HW6.htm
            auto rdm = random_in_unit_disk();
            return eye_point + (rdm.x * defocus_disk_u) + (rdm.y * defocus_disk_v);
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
