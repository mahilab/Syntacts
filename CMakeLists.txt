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
option(SYNTACTS_GUI      "Turn ON to build Syntacts GUI"        ON)
option(SYNTACTS_C_DLL    "Turn ON to build Syntacts C DLL"      ON )
option(SYNTACTS_EXAMPLES "Turn ON to build Syntacts example(s)" ON )

# create project
project(syntacts VERSION 1.0.0)

# add modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(GNUInstallDirs)

# gather public includes
set(SYNTACTS_INCLUDE 
    "include/Tact/Envelope.hpp"
    "include/Tact/Oscillator.hpp"
    "include/Tact/Signal.hpp"
    "include/Tact/Process.hpp"
    "include/Tact/Macros.hpp"
    "include/Tact/Session.hpp"
    "include/Tact/Spatializer.hpp"
    "include/Tact/Library.hpp"
    "include/Tact/Operator.hpp"
    "include/Tact/Sequence.hpp"
    "include/Tact/Curve.hpp"
    "include/Tact/Util.hpp"
    "include/Tact/MemoryPool.hpp"
    "include/Tact/General.hpp"
    "include/Tact/Detail/Signal.inl"
    "include/Tact/Detail/Oscillator.inl"
    "include/Tact/Detail/Operator.inl"
)

# gather private sources
set(SYNTACTS_SRC 
    "src/Tact/Envelope.cpp"
    "src/Tact/Oscillator.cpp"
    "src/Tact/Signal.cpp"
    "src/Tact/Process.cpp"
    "src/Tact/Library.cpp"
    "src/Tact/Session.cpp"
    "src/Tact/Spatializer.cpp"
    "src/Tact/Operator.cpp"
    "src/Tact/Sequence.cpp"
    "src/Tact/Curve.cpp"
    "src/Tact/MemoryPool.cpp"
    "src/Tact/Util.cpp"
    "src/Tact/General.cpp"
)

# portaudio
set(PA_DLL_LINK_WITH_STATIC_RUNTIME ON CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/portaudio)
set_target_properties(portaudio_static PROPERTIES DEBUG_POSTFIX -d)

#===============================================================================
# Syntacts Static C++ Library
#===============================================================================

add_library(syntacts "")
add_library(syntacts::syntacts ALIAS syntacts)
set_target_properties(syntacts PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
set_target_properties(syntacts PROPERTIES COMPILE_FLAGS "/bigobj" )
target_sources(syntacts PRIVATE ${SYNTACTS_INCLUDE})
target_sources(syntacts PRIVATE ${SYNTACTS_SRC})
target_compile_definitions(syntacts 
    PUBLIC 
        SYNTACTS_STATIC 
        PA_USE_ASIO 
    PRIVATE 
        NOMINMAX
)
target_include_directories(syntacts
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${portaudio_INCLUDE_DIR}
        3rdparty
        3rdparty/cereal/include
)
target_link_libraries(syntacts PUBLIC portaudio_static PRIVATE)

#===============================================================================
# Syntacts Plugin DLL
#===============================================================================

if (SYNTACTS_C_DLL)
    add_subdirectory("dll")
endif()

#===============================================================================
# Syntacts GUI
#===============================================================================

if (SYNTACTS_GUI)
    set(MAHI_GUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(3rdparty/mahi-gui EXCLUDE_FROM_ALL)
    add_subdirectory("gui")
endif()

#===============================================================================
# Syntacts Examples
#===============================================================================

if (SYNTACTS_EXAMPLES)
    add_subdirectory("examples")
endif()

#===============================================================================
# Install
#===============================================================================

# install the library
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Syntacts)
install(TARGETS syntacts
	EXPORT syntacts-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(syntacts PROPERTIES EXPORT_NAME syntacts)
set_target_properties(syntacts PROPERTIES FOLDER "Syntacts")

# install headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# set where we want to install our config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Syntacts)

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
       