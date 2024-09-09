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
        r3df0_perlin.h
)

set_target_properties(r3dfrom0 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
# ADDITIVE INSTRUCTIONS FOR EXT LIBS
target_include_directories(r3dfrom0 PRIVATE ext/)
target_include_directories(r3dfrom0 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(r3dfrom0 PRIVATE stb_image::stb_image)