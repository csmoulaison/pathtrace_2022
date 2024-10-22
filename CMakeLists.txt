#Minimum version of CMake that can run this script
cmake_minimum_required(VERSION 3.12)

# Project name and version
project(pathtrace VERSION 0.1)

find_package(SDL2 REQUIRED)

# Request c++11 standard features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Export compile commands for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set up configuration header file
# configure_file(include/Config.h.in
#	"${CMAKE_CURRENT_SOURCE_DIR}/include/Config.h")

# Compile the passed source files into an executable
add_executable(pathtrace
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/camera.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/cm_math.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/cm_math.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/color.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/platform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/sphere.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/sphere.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tracer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/tracer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/vec3.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/vec3.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/world.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/world.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/hitinfo.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/material.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/random.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ray.h"
	)

# Reference include directory
target_include_directories(pathtrace PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${SDL2_INCLUDE_DIRS}"
	)

target_link_libraries(pathtrace ${SDL2_LIBRARIES})

# Set the output directory for executables
set_target_properties(pathtrace
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
