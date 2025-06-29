cmake_minimum_required(VERSION 3.22)

project(rpixel)

add_library(rpixel STATIC
    "src/rpixel.cpp"
)

target_include_directories(rpixel
    PUBLIC
        "include"
        "src"
)

target_link_libraries(rpixel
    PUBLIC
        pico_stdlib
        hardware_pio
)
