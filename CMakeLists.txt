cmake_minimum_required (VERSION 3.0.2)
project("PointCarverCpp")

#find_package(OpenGL REQUIRED)
find_package (Threads REQUIRED)

set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_CXX_FLAGS "-std=c++17")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ggdb -Wall -Wextra")

include_directories(
    "./include/"
)

add_executable(carver.out "src/carver.cpp")

install(TARGETS carver.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
