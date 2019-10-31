# MIT License
#
# Syntacts
# Copyright (c) 2019 Mechatronics and Haptic Interfaces Lab - Rice University
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Author(s): Evan Pezent, Brandon Cambio

cmake_minimum_required(VERSION 3.13.0)

# user options
option(SYNTACTS_EXAMPLES "Turn ON to build Syntacts example(s)"       ON )
option(SYNTACTS_TEST     "Turn ON to build Syntacts test(s)"          ON )
option(SYNTACTS_ANSI_C   "Turn ON to build Syntacts ANSI C interface" OFF)

# create project
project(syntacts VERSION 1.0.0 LANGUAGES CXX)

# add modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(GNUInstallDirs)

# gather public includes
set(SYNTACTS_INCLUDE 
    "include/Syntacts/Syntacts.hpp"
    "include/Syntacts/Cue.hpp"
    "include/Syntacts/Envelope.hpp"
    "include/Syntacts/Oscillator.hpp"
    "include/Syntacts/Source.hpp"
    "include/Syntacts/Config.hpp"
    "include/Syntacts/AudioFile.hpp"
    "include/Syntacts/Serialization.hpp"
    "include/Syntacts/Tween.hpp"
    "include/Syntacts/Memory.hpp"
)

# gather private sources
set(SYNTACTS_SRC 
    "src/Syntacts/Syntacts.cpp"
    "src/Syntacts/Cue.cpp"
    "src/Syntacts/Envelope.cpp"
    "src/Syntacts/Oscillator.cpp"
    "src/Syntacts/Source.cpp"
    "src/Syntacts/Helpers.hpp"
    "src/Syntacts/AudioFile.cpp"
    "src/Syntacts/Serialization.cpp"
    "src/Syntacts/Tween.cpp"
    # "src/Syntacts/Detail/pink_noise.hpp"
    # "src/Syntacts/Detail/pink_noise.cpp"
)

# find portaudio
find_package(portaudio REQUIRED)
set(portaudio_INCLUDE_DIR ${portaudio_DIR})
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
set(portaudio_INCLUDE_DIR "${portaudio_INCLUDE_DIR}/include")

#===============================================================================
# Syntacts Static C++ Library
#===============================================================================

add_library(syntacts "")
add_library(syntacts::syntacts ALIAS syntacts)
set_target_properties(syntacts PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
set_target_properties(syntacts PROPERTIES COMPILE_FLAGS "/bigobj")
target_sources(syntacts PRIVATE ${SYNTACTS_INCLUDE})
target_sources(syntacts PRIVATE ${SYNTACTS_SRC})
target_compile_definitions(syntacts PUBLIC SYNTACTS_STATIC PA_USE_ASIO PRIVATE NOMINMAX)
target_include_directories(syntacts
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${portaudio_INCLUDE_DIR}
        3rdparty
)
target_link_libraries(syntacts PUBLIC portaudio_static)

#===============================================================================
# Syntacts Dynamic ANSI C Library
#===============================================================================

if (SYNTACTS_ANSI_C)
    add_library(syntacts-c SHARED "")
    add_library(syntacts::syntacts-c ALIAS syntacts-c)
    set_target_properties(syntacts-c PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
    set_target_properties(syntacts-c PROPERTIES COMPILE_FLAGS "/bigobj")
    target_sources(syntacts-c PRIVATE ${SYNTACTS_INCLUDE})
    target_sources(syntacts-c PRIVATE ${SYNTACTS_SRC})
    target_compile_definitions(syntacts-c PRIVATE -DSYNTACTS_EXPORTS)
    target_compile_definitions(syntacts-c PUBLIC -DSYNTACTS_ANSI_C PA_USE_ASIO PRIVATE NOMINMAX)
    target_include_directories(syntacts-c
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${portaudio_INCLUDE_DIR}
    )
    target_link_libraries(syntacts-c PUBLIC portaudio_static)
endif()

#===============================================================================
# Syntacts GUI
#===============================================================================

find_package(carnot)
if (carnot_FOUND) 
    add_subdirectory("gui")
endif()

#===============================================================================
# Syntacts Examples/Tests
#===============================================================================

if (SYNTACTS_EXAMPLES)
    add_subdirectory("examples")
endif()

if (SYNTACTS_TEST)
    add_subdirectory("tests")
endif()

#===============================================================================
# Install
#===============================================================================

# install the library
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/syntacts)
install(TARGETS syntacts
	EXPORT syntacts-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(syntacts PROPERTIES EXPORT_NAME syntacts)
set_target_properties(syntacts PROPERTIES FOLDER "syntacts")

# install headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# set where we want to install our config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/syntacts)

# export the targets to a script
install(EXPORT syntacts-targets
	FILE
		SyntactsTargets.cmake
	# NAMESPACE
	# 	syntacts::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# include helper functions for creating config files that can be included by other projects to find and use a package
include(CMakePackageConfigHelpers)

# generate a package configuration file and a package version file
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/SyntactsConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/SyntactsConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/SyntactsConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# install the config and configversion modules
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/SyntactsConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/SyntactsConfigVersion.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)

# export from the build tree
export(EXPORT syntacts-targets
	#    NAMESPACE syntacts::
	   FILE ${CMAKE_CURRENT_BINARY_DIR}/SyntactsTargets.cmake)

