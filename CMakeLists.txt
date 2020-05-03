cmake_minimum_required(VERSION 3.17)
project(fuka)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++17")
endif ()

file(GLOB_RECURSE FUKA_SRC
        include/fuka/*.h
        src/*/*.cpp)

add_library(fuka STATIC ${FUKA_SRC})
target_include_directories(fuka PUBLIC
        external/eigen
        include)

add_executable(test_math tests/test_math.cpp)
target_link_libraries(test_math fuka)