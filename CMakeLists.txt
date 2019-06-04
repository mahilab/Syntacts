# MIT License
#
# TactorFX
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
option(TFX_STATIC   "Turn ON to build TFX as a static library"                  ON )
option(TFX_ANSI_C   "Turn ON to build TFX DLL with ANSI C exports"              OFF)
option(TFX_GUI      "Turn ON to build TFX GUI (Carnot Engine must be installed)" ON )
option(TFX_EXAMPLES "Turn ON to build TFX example(s)"                           ON)
option(TFX_TEST     "Turn ON to build TFX test(s)"                              ON)

# check for incompatible options
if (TFX_STATIC AND TFX_ANSI_C)
    message(FATAL_ERROR "The ANSI C version of TFX must be built as a shared libary (.dll). Turn TFX_STATIC OFF.")
endif()

if (NOT TFX_STATIC AND (TFX_GUI OR TFX_EXAMPLES))
    message(FATAL_ERROR "The TFX examples cannot be built with the ANSI C exports. Turn TFX_ANSI_C OFF.")
endif()

# create project
project(TactorFX VERSION 0.1.0 LANGUAGES CXX)

#===============================================================================
# TACTORFX LIBRARY
#===============================================================================

# gather public includes
set(INCLUDE_FILES 
    "include/TactorFX/TactorFX.hpp"
    "include/TactorFX/Cue.hpp"
    "include/TactorFX/Envelope.hpp"
    "include/TactorFX/Oscillator.hpp"
    "include/TactorFX/Generator.hpp"
    "include/TactorFX/Config.hpp"
)

# gather private sources
set(SRC_FILES 
    "src/TactorFX/TactorFX.cpp"
    "src/TactorFX/Cue.cpp"
    "src/TactorFX/Envelope.cpp"
    "src/TactorFX/Oscillator.cpp"
    "src/TactorFX/Helpers.hpp"
    "src/TactorFX/Generator.cpp"
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
if (TFX_STATIC AND NOT TFX_ANSI_C)
    add_library(tfx STATIC ${INCLUDE_FILES} ${SRC_FILES})
    target_compile_definitions(tfx PUBLIC -DTFX_STATIC)
else()
    add_library(tfx SHARED ${INCLUDE_FILES} ${SRC_FILES})
    target_compile_definitions(tfx PRIVATE -DTFX_EXPORTS)
    if (TFX_ANSI_C)
        target_compile_definitions(tfx PUBLIC -DTFX_ANSI_C)
    endif()
endif()

# includes
target_include_directories(tfx PRIVATE  ${portaudio_INCLUDE_DIR})
target_include_directories(tfx PUBLIC "include")

# link to portaudio
target_compile_definitions(tfx PUBLIC PA_USE_ASIO)
target_link_libraries(tfx PUBLIC portaudio_static)

#===============================================================================
# APLLICATIONS
#===============================================================================

if (TFX_EXAMPLES)
    add_subdirectory("examples")
endif()

if (TFX_GUI)
    add_subdirectory("gui")
endif()

if (TFX_TEST)
    add_subdirectory("tests")
endif()