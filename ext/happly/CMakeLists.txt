cmake_minimum_required(VERSION 3.12.0)

add_library(happly STATIC happly.h)

set_target_properties(happly PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(happly PUBLIC . happly)
add_library(happly::happly ALIAS happly)