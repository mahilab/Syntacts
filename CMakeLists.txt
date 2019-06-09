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
# Author(s): Evan Pezent, Brandom Cambio

cmake_minimum_required(VERSION 3.13.0)

# options
option(SYNTACTS_STATIC   "Turn ON to build Syntacts as a static library"                   ON )
option(SYNTACTS_ANSI_C   "Turn ON to build Syntacts DLL with ANSI C exports"               OFF)
option(SYNTACTS_GUI      "Turn ON to build Syntacts GUI (Carnot Engine must be installed)" ON )
option(SYNTACTS_EXAMPLES "Turn ON to build Syntacts example(s)"                            ON)
option(SYNTACTS_TEST     "Turn ON to build Syntacts test(s)"                               ON)

# check for incompatible options
if (SYNTACTS_STATIC AND SYNTACTS_ANSI_C)
    message(FATAL_ERROR "The ANSI C version of Syntacts must be built as a shared libary (.dll). Turn SYNTACTS_STATIC OFF.")
endif()

if (NOT SYNTACTS_STATIC AND (SYNTACTS_GUI OR SYNTACTS_EXAMPLES))
    message(FATAL_ERROR "The Syntacts examples cannot be built with the ANSI C exports. Turn SYNTACTS_ANSI_C OFF.")
endif()

# create project
project(Syntacts VERSION 0.1.0 LANGUAGES CXX)

#===============================================================================
# Syntacts LIBRARY
#===============================================================================

# gather public includes
set(INCLUDE_FILES 
    "include/Syntacts/Syntacts.hpp"
    "include/Syntacts/Cue.hpp"
    "include/Syntacts/Envelope.hpp"
    "include/Syntacts/Oscillator.hpp"
    "include/Syntacts/Generator.hpp"
    "include/Syntacts/Config.hpp"
)

# gather private sources
set(SRC_FILES 
    "src/Syntacts/Syntacts.cpp"
    "src/Syntacts/Cue.cpp"
    "src/Syntacts/Envelope.cpp"
    "src/Syntacts/Oscillator.cpp"
    "src/Syntacts/Helpers.hpp"
    "src/Syntacts/Generator.cpp"
)

# find packages
find_package(portaudio REQUIRED)

# portaudio doesn't export their include directory...ugh!
set(portaudio_INCLUDE_DIR ${portaudio_DIR})
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
set(portaudio_INCLUDE_DIR "${portaudio_INCLUDE_DIR}/include")

# make library
if (SYNTACTS_STATIC AND NOT SYNTACTS_ANSI_C)
    add_library(syntacts STATIC ${INCLUDE_FILES} ${SRC_FILES})
    target_compile_definitions(syntacts PUBLIC -DSYNTACTS_STATIC)
else()
    add_library(syntacts SHARED ${INCLUDE_FILES} ${SRC_FILES})
    target_compile_definitions(syntacts PRIVATE -DSYNTACTS_EXPORTS)
    if (SYNTACTS_ANSI_C)
        target_compile_definitions(syntacts PUBLIC -DSYNTACTS_ANSI_C)
    endif()
endif()

# includes
target_include_directories(syntacts PRIVATE  ${portaudio_INCLUDE_DIR})
target_include_directories(syntacts PUBLIC "include")

# link to portaudio
target_compile_definitions(syntacts PUBLIC PA_USE_ASIO)
target_link_libraries(syntacts PUBLIC portaudio_static)

#===============================================================================
# APLLICATIONS
#===============================================================================

if (SYNTACTS_EXAMPLES)
    add_subdirectory("examples")
endif()

if (SYNTACTS_GUI)
    add_subdirectory("gui")
endif()

if (SYNTACTS_TEST)
    add_subdirectory("tests")
endif()