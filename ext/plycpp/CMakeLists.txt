cmake_minimum_required(VERSION 3.12.0)

add_library(plycpp STATIC
            plycpp/plycpp_runner.h
            plycpp/plycpp.h
            plycpp/plycpp.cpp
)

set_target_properties(plycpp PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(plycpp PUBLIC . plycpp)
add_library(plycpp::plycpp ALIAS plycpp)


