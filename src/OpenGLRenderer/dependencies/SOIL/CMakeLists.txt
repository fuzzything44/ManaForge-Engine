cmake_minimum_required(VERSION 3.0)

project(SOIL)


include_directories("../../include/SOIL/")
include_directories("../../include/")

add_library(SOIL "image_DXT.c" "image_helper.c" "SOIL.c" "stb_image_aug.c")

add_definitions("-DSOIL_Source")

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(SOIL ${OPENGL_LIBRARIES})

message(${OPENGL_LIBRARIES})


