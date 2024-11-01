//
// Created by trast on 17/08/2024.
//

#ifndef R3DFROM0_R3DF0_TEXTURE_H
#define R3DFROM0_R3DF0_TEXTURE_H

#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_perlin.h"

namespace r3dfrom0{

    class color_gradiant : public texture {
    public:
        color_gradiant() {};

        pixel_f color(float u, float v, const vec3f& p) const override{
            return color_map(p);
        }

    };

    class spacial_texture : public texture {
    public:
        // constructor
        spacial_texture(const pixel_f& albedo) : albedo(albedo) {}
        spacial_texture(float r, float g, float b) : albedo(pixel_f{r,g,b}) {}

        // methods
        pixel_f color(float u, float v, const vec3f& p) const override{
            return albedo;
        }

    private:
        pixel_f albedo;
    };

    class checker_texture : public texture {
    public:
        // constructors
        checker_texture(float scale, shared_ptr<texture> even_tex, shared_ptr<texture> odd_tex) :
                        scale(1.f/scale), even(even_tex), odd(odd_tex) {};
        checker_texture(float scale, const pixel_f& even_color, const pixel_f& odd_color) :
        checker_texture(scale, make_shared<spacial_texture>(even_color), make_shared<spacial_texture>(odd_color)) {};

        // methods
        pixel_f color(float u, float v, const vec3f& p) const override{
            int _x = int(floor(p.x) * scale);
            int _y = int(floor(p.y) * scale);
            int _z = int(floor(p.z) * scale);
            return (_x + _y + _z) % 2 == 0 ? even->color(u,v,p) : odd->color(u,v,p);
        }

    private:
        float scale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
    }; // checker_texture class

    class image_texture : public texture {
    public:
    // constructor
    image_texture(string file_name) : image_file_name(file_name){
        load_img(tuple_img, image_file_name, width, height);
    }

    pixel_f color(float u, float v, const vec3f& p) const override{
        // check h and w if they are set correctly
        //
        if (height <= 0 || width <= 0 || tuple_img.empty()) {
            return BRIGHT_PURPLE;
        }
        // clamping in 0,1 could hold values that equals to w and h
        u = interval(0.0f,1.0f).clamp(u);
        v = 1.0f - interval(0.0f,1.0f).clamp(v); // v coords are flipped

        auto x = int(u * width);
        auto y = int(v * height);

        if (x < width && y < height) {
            return tuple_img[y][x];
        } else {
            return BRIGHT_PURPLE;
        }
    }

    private:
        int width = 0;
        int height = 0;
        string image_file_name;
        vector<vector<pixel_f>> tuple_img;
        const pixel_f BRIGHT_PURPLE = {1.0f, 0, 1.0f};

    }; // image_texture class

    class perlin_noise_texture : public texture {
    public:
        // constructor
        perlin_noise_texture(float scale) : scale(scale) {}
        // methods
        virtual pixel_f color(float u, float v, const vec3f& p) const override{
            return pixel_f(1.f, 1.f, 1.f) * 0.5 * (1.f + noise.noise(scale * p));
        }
    private:
        float scale;
        perlin_noise noise;
    };
} // namespace r3dfrom0
#endif //R3DFROM0_R3DF0_TEXTURE_H
