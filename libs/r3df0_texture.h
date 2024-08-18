//
// Created by trast on 17/08/2024.
//

#ifndef R3DFROM0_R3DF0_TEXTURE_H
#define R3DFROM0_R3DF0_TEXTURE_H

#include "r3df0_varsutil.h"
#include "r3df0_texture_interface.h"

namespace r3dfrom0{
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
    };
}
#endif //R3DFROM0_R3DF0_TEXTURE_H