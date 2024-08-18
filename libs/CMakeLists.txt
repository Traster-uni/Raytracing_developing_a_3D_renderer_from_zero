add_library(r3dfrom0 STATIC
        r3df0_math.h
        r3df0_image.h
        r3df0_color.h
        r3df0_rays.h
        r3df0_varsutil.h
        r3df0_camera.h
        r3df0_camera.h
        r3df0_hittable.h
        r3df0_shapes.h
        r3df0_material.h
        r3df0_axisAlignBBoxes.h
        r3df0_hittable_interface.h
        r3df0_material_interface.h
        r3df0_texture_interface.h
        r3df0_bvh.h
        r3df0_texture.h
)

set_target_properties(r3dfrom0 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)