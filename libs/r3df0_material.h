//
// Created by tommasomarialopedote on 02/08/24.
//

#ifndef R3DFROM0_R3DF0_MATERIAL_H
#define R3DFROM0_R3DF0_MATERIAL_H

#include "r3df0_hittable.h"

using namespace std;

namespace r3dfrom0{

    class material{ // interface
    public:
        // constructor
        virtual ~material() = default;

        // methods
        virtual bool scatter
            (const ray& ray_in, hit_record& hitRecord, ray& scatter_ray, pixel_f& attenuation)
             const{
            return false;
        }
    }; // material interface

    class lambertian : public material {
    public:
        // constructor
        lambertian(pixel_f albedo) : albedo(albedo) {}

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
            attenuation = albedo;
            return true;
        }

    private:
        pixel_f albedo;
    }; // lambertian class


    class metal : public  material {
    public:
        // constructor
        metal(pixel_f albedo) : albedo(albedo) {}

        // method
        bool scatter
            (const ray& ray_in, hit_record& hitRecord, ray& scatter_ray, pixel_f& attenuation)
            const override{
            auto direction = reflect(ray_in.direction(), hitRecord.normal);
            scatter_ray = ray(hitRecord.position, direction);
            attenuation = albedo;
            return true;
        }
    private:
        pixel_f albedo;
    }; // metal class
}


#endif //R3DFROM0_R3DF0_MATERIAL_H
