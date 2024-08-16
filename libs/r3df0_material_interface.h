//
// Created by tommasomarialopedote on 15/08/24.
//

#ifndef R3DFROM0_R3DF0_MATERIAL_INTERFACE_H
#define R3DFROM0_R3DF0_MATERIAL_INTERFACE_H

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
    }; // material interface
}


#endif //R3DFROM0_R3DF0_MATERIAL_INTERFACE_H
