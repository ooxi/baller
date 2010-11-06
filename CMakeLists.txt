# ###############################
# CMake build file for Ballerburg
# ###############################

cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
project (Ballerburg)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckCCompilerFlag)
include(DistClean)

# Set build type to "Release" if user did not specify any build type yet
# Other possible values: Debug, Release, RelWithDebInfo and MinSizeRel
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)


# ####################
# Check for libraries:
# ####################

find_package(SDL REQUIRED)
if(NOT SDL_FOUND)
	message(FATAL_ERROR "SDL library not found!")
endif(NOT SDL_FOUND)

find_package(Math)


# ###############
# Various CFLAGS:
# ###############

if(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "-Wall -Wmissing-prototypes -Wstrict-prototypes ${CMAKE_C_FLAGS}")
        set(CMAKE_C_FLAGS "-fsigned-char ${CMAKE_C_FLAGS}")
        # set(CMAKE_C_FLAGS "-Werror ${CMAKE_C_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCC)


# #########################################
# Create config.h and recurse into subdirs:
# #########################################

# configure_file(${CMAKE_SOURCE_DIR}/cmake/config-cmake.h
#                ${CMAKE_BINARY_DIR}/config.h)

add_subdirectory(src)
