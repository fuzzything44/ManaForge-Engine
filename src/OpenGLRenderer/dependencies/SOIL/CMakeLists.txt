cmake_minimum_required(VERSION 3.0)

project(SOIL)


include_directories("../../include/SOIL/")
include_directories("../../include/")

add_library(SOIL "image_DXT.c" "image_helper.c" "SOIL.c" "stb_image_aug.c")
