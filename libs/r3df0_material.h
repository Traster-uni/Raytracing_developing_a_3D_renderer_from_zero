//
// Created by tommasomarialopedote on 02/08/24.
//

#ifndef R3DFROM0_R3DF0_MATERIAL_H
#define R3DFROM0_R3DF0_MATERIAL_H

#include "r3df0_varsutil.h"
#include "r3df0_hittable.h"
#include "r3df0_texture.h"

using namespace std;

namespace r3dfrom0{
    class lambertian : public material {
    public:
        // constructor
        lambertian(shared_ptr<texture> tex) : albedo_texture(tex) {}
        lambertian(pixel_f albedo) : albedo_texture(make_shared<spacial_texture>(albedo)) {}

        // method
        bool scatter
            (const ray& ray_in, hit_record& hitRecord, ray& scatter_ray, pixel_f& attenuation)
            const override{
            auto direction = hitRecord.normal + random_unit_vector();
            if (direction.near_zero()){
                // if random is near zero, return the normal
                direction = hitRecord.normal;
            }
            scatter_ray = ray(hitRecord.position, direction);
            attenuation = albedo_texture->color(hitRecord.u, hitRecord.v, hitRecord.position);
            return true;
        }

    private:
        shared_ptr<texture> albedo_texture;
    }; // lambertian class


    class metal : public  material {
    public:
        // constructor
        metal(pixel_f albedo, float fuzz) : albedo(albedo), fuzziness(fuzz <= 1 ? fuzz : 1)  {}

        // method
        bool scatter
            /**
             * Fuzzy metal, fuzz values are real an in the interval [0,1] where 0 is mirrorball like finish
             * and 1 is as fuzzy as it can get, almost looks plasticky
             */
            (const ray& ray_in, hit_record& hitRecord, ray& scatter_ray, pixel_f& attenuation)
            const override{
            auto reflected_ray = reflect(ray_in.direction(), hitRecord.normal);
            auto reflected_fuzzy_ray = unit(reflected_ray) + (fuzziness * random_unit_vector());
            scatter_ray = ray(hitRecord.position, reflected_fuzzy_ray);
            attenuation = albedo;
            return dot(reflected_fuzzy_ray, hitRecord.normal) > 0;
            // if dot(N,R) < 0 the ray would be clipping into the ball
        }
    private:
        pixel_f albedo;
        float fuzziness;
    }; // metal class

    class dielectric : public material{
    public:
        dielectric(float rf_index) : refraction_index(rf_index) {}

        bool scatter
        (const ray& ray_in, hit_record& hitRecord, ray& scatter_ray, pixel_f& attenuation)
        const override{
            attenuation = pixel_f{1.0, 1.0, 1.0};
            float ri = refraction_index;
            if (hitRecord.front_face_hit){
                // invert coefficients if ray hits from the inverse direction
                ri = 1.0f/refraction_index;
            }
            vec3f uv_ray_in = unit(ray_in.direction());
            auto cos_theta = fmin(dot(-uv_ray_in, hitRecord.normal), 1.0f);
            auto sin_theta = sqrt(1.0f - cos_theta * cos_theta);
            vec3f direction;
            // you want glass behavior? throw in there the schlick approximation
            if (ri * sin_theta > 1.0f|| schlick_approx(cos_theta, ri) > random_float()){
                direction = reflect(uv_ray_in, hitRecord.normal);
            } else {
                direction = refract(uv_ray_in, hitRecord.normal, ri);
            }

            scatter_ray = ray(hitRecord.position, direction);
            return true;
        }
    private:
        /**
         * Refraction indexes List
         * 1.0f transparent
         * higher then 1.0f, eg 1.5f make refraction apparent, but you can't see the Total initial reflection
         * to see the Total initial reflection use the air ball
         * 1.0f/1.33f
         */
        float refraction_index; // eta on eta1
    }; // dielectric class

    class diffuse_light : public material {
    public:
        // constructor
        diffuse_light(shared_ptr<texture> tex) : emission_tex(tex) {};
        diffuse_light(const pixel_f& col ) : emission_tex(make_shared<spacial_texture>(col)) {};
        diffuse_light(const float& intensity ) : emission_tex(make_shared<spacial_texture>(pixel_f{intensity, intensity, intensity})) {};
        // methods

        pixel_f emitted(const float& u, const float& v, const vec3f& p) const override {
            return emission_tex->color(u, v, p);
        }
    private:
        shared_ptr<texture> emission_tex;

    }; // class material
}


#endif //R3DFROM0_R3DF0_MATERIAL_H
