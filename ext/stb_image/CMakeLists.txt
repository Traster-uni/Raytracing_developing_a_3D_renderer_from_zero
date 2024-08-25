cmake_minimum_required(VERSION 3.12.0)
#
#project(stb_image)

add_library(stb_image STATIC
            stb_image/stb_image_runner.cpp
            stb_image/stb_image.h
            stb_image/stb_image_write.h
            stb_image/stb_image_resize2.h
)
set_target_properties(stb_image PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_include_directories(stb_image PUBLIC . stb_image)

add_library(stb_image::stb_image ALIAS stb_image)
