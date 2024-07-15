//
// Created by tommasomarialopedote on 13/07/24.
//

#ifndef R3DFROM0_R3DF0_CAMERA_H
#define R3DFROM0_R3DF0_CAMERA_H

#include "r3df0_varsutil.h"
#include "r3df0_math.h"
namespace r3dfrom0{

    class camera{
    public: // public attributes
        int image_width = 1920; // defaults to fullHD, 1920w x 1080h
        float aspect_ratio = 1.7777777777777777; // defaults to 16:9
        // constructor
        camera() {}

        // public methods
        double fov(){
            return radians_to_degrees(2 * focal_length * atan(image_height / 2));
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
        vec3f pixel_delta_r;        // pixel offset to the right
        vec3f pixel_delta_d;        // pixel offset down

        // private methods
        void initialize(const vec3f& ep = {0,0,0}, const float& fl = 1.0){
            eye_point = ep;
            focal_length = fl;

            // define image resolution
            auto h = int(float (image_width) / aspect_ratio); // h = w * (1/aspect_ratio)
            image_height = h < 1 ? 1 : h; // image_height must be at least 1

            // define viewport dimension
            viewport_height = 2.0;  // TODO: MAKE VIEWPORT DIMENSIONS FULLY COMPUTABLE
            viewport_width = viewport_height * (float(image_width) / float (image_height));

            // viewport vectors
            viewport_u = {viewport_width, 0, 0};
            viewport_v = {0, -viewport_height, 0};

            // pixel deltas
            pixel_delta_r = viewport_u / float (image_width);
            pixel_delta_d = viewport_v / float (image_height);

            // location of upper left corner pixel
            viewport_upper_left = eye_point - vec3f{0,0,focal_length} - viewport_u / 2 - viewport_v / 2;
            pixel00_location = viewport_upper_left - (0.5 * (pixel_delta_r + pixel_delta_d));

        }

        vec3f pixel_center(const int& i, const int& j){
            // returns coordinates for center of pixel, used to cast rays from
            // said pixel
            return pixel00_location + (i * pixel_delta_r) + (j * pixel_delta_d);
        }

    };
} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_CAMERA_H
