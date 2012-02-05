# ###############################
# CMake build file for Ballerburg
# ###############################

cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (Ballerburg C)

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
# Paths configuration:
# ####################

if(NOT BINDIR)
	set(BINDIR bin)
endif()

if(NOT DATADIR)
	set(DATADIR share/ballerburg)
endif()

if(NOT BIN2DATADIR)
	set(BIN2DATADIR "../share/ballerburg"
	    CACHE STRING "Relative path from bindir to datadir")
	mark_as_advanced(BIN2DATADIR)
endif()

if(NOT BIN2LOCALEDIR)
	set(BIN2LOCALEDIR "../share/locale"
	    CACHE STRING "Relative path from bindir to locale directory")
	mark_as_advanced(BIN2LOCALEDIR)
endif()

if(NOT MANDIR)
	set(MANDIR share/man/man1)
endif()

if(NOT DOCDIR)
	set(DOCDIR share/doc/ballerburg)
endif()


# ####################
# Check for libraries:
# ####################

find_package(SDL REQUIRED)
find_package(SDL_gfx REQUIRED)
find_package(Math REQUIRED)

find_package(Gettext)
if(GETTEXT_FOUND)
	set(HAVE_GETTEXT 1)
endif(GETTEXT_FOUND)


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

configure_file(${CMAKE_SOURCE_DIR}/cmake/config-cmake.h
               ${CMAKE_BINARY_DIR}/config.h)

add_subdirectory(src)
add_subdirectory(doc)


add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/Uninstall.cmake)
