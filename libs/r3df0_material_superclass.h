//
// Created by tommasomarialopedote on 15/08/24.
//

#ifndef R3DFROM0_R3DF0_MATERIAL_SUPERCLASS_H
#define R3DFROM0_R3DF0_MATERIAL_SUPERCLASS_H

namespace r3dfrom0{
    class ray;
    class hit_record;
    class pixel_f;

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

        virtual pixel_f emitted(const float& u, const float& v, const vec3f& p) const {
            return {.0f, .0f, .0f};
        }

    }; // material interface
}


#endif //R3DFROM0_R3DF0_MATERIAL_SUPERCLASS_H
