cmake_minimum_required(VERSION 3.17)
project(misaki-utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++17")
endif ()

add_subdirectory(ext/fmt)

file(GLOB_RECURSE MSK_SRC
        include/misaki/utils/*.h
        include/misaki/utils/*.hpp
        src/*/*.cpp)

if(NOT WIN32)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
			set(LINKER_FLAGS "-lc++fs -pthread")
		else()
			set(LINKER_FLAG "-lstdc++fs -pthread")
		endif()
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(LINKER_FLAG "-lstdc++fs -pthread")
	endif()
endif()

add_library(misaki-utils STATIC ${MSK_SRC})
target_include_directories(misaki-utils PUBLIC
        include)
target_link_libraries(misaki-utils PUBLIC fmt::fmt)