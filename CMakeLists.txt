cmake_minimum_required(VERSION 3.17)
project(misaki-utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++17")
        add_compile_options("/wd4819") # Eigen half
endif ()

file(GLOB_RECURSE MSK_SRC
        include/misaki/utils/*.h
        include/misaki/utils/*.hpp
        src/*/*.cpp)

add_library(misaki-utils STATIC ${MSK_SRC})
target_include_directories(misaki-utils PUBLIC
        external/eigen
        include)