cmake_minimum_required(VERSION 3.17)
project(misaki-utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MSK_CUDA "Enable cuda for tracing jit" ON)
set(MSK_CUDA ON)
if (MSK_CUDA)
        set(CMAKE_CUDA_STANDARD 14)
        set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_50,code=compute_50")
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler \"/wd 4819\"")
        if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.17.0")
                # CMake 3.17.0 introduces CMAKE_CUDA_RUNTIME which must be used instead of explicit -cudart shared flag
                set(CMAKE_CUDA_RUNTIME_LIBRARY Shared)
        else()
                set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -cudart shared")
        endif()
        enable_language(CUDA)
endif()

if (MSK_CUDA)
        add_library(misaki-cuda src/cuda/jit.cu)
        target_include_directories(misaki-cuda PUBLIC include)
        target_compile_features(misaki-cuda PUBLIC cxx_std_14)
        target_link_libraries(misaki-cuda PRIVATE cuda)
endif()

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

add_executable(test_math tests/test_math.cpp)
target_link_libraries(test_math misaki-utils)

add_executable(test_image tests/test_image.cpp)
target_link_libraries(test_image misaki-utils)

add_executable(test_cuda tests/test_cuda.cpp)
target_link_libraries(test_cuda misaki-utils misaki-cuda)