cmake_minimum_required(VERSION 3.27)
project(Parkour C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(SDL_Tutorial main.c)

target_link_libraries(SDL_Tutorial PRIVATE SDL2::SDL2)
target_link_libraries(SDL_Tutorial PRIVATE SDL2_image::SDL2_image)
target_link_libraries(SDL_Tutorial PRIVATE SDL2_ttf::SDL2_ttf)