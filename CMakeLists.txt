cmake_minimum_required(VERSION 3.13.0)

# create project
project(TactorFX VERSION 0.1.0 LANGUAGES CXX)

# find packages
find_package(portaudio REQUIRED)
find_package(MEL REQUIRED)

# portaudio doesn't export their include directory...ugh!
set(portaudio_INCLUDE_DIR ${portaudio_DIR})
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
get_filename_component(portaudio_INCLUDE_DIR ${portaudio_INCLUDE_DIR} DIRECTORY)
set(portaudio_INCLUDE_DIR "${portaudio_INCLUDE_DIR}/include")

# include directories
include_directories("include" ${portaudio_INCLUDE_DIR})

add_compile_definitions(PA_USE_ASIO) # needed for devs

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

# make library
add_library(tfx SHARED ${INCLUDE_FILES} ${SRC_FILES})
target_link_libraries(tfx portaudio_static)
target_compile_definitions(tfx PRIVATE TFX_EXPORTS)
#===============================================================================
# EXAMPLES
#===============================================================================

add_subdirectory("examples")
add_subdirectory("gui")

#===============================================================================
# EXAMPLES
#===============================================================================

set(BUILD_TESTS OFF)
if (BUILD_TESTS)
    add_subdirectory("tests")
endif()