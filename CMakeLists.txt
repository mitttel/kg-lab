cmake_minimum_required(VERSION 3.29)
project(KG)

set(CMAKE_CXX_STANDARD 20)

add_executable(KG main.cpp
        VecFunctions.h
        Vec3.h
        Vec2.h)
