cmake_minimum_required(VERSION 3.4...3.18)
project(pyiohook)

add_subdirectory(extern/pybind11)
add_subdirectory(extern/libuiohook)

pybind11_add_module(pyiohook src/main.cpp src/uiohook_worker.c)
target_link_libraries(pyiohook PRIVATE uiohook)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a

# define (VERSION_INFO) here.
target_compile_definitions(pyiohook
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})