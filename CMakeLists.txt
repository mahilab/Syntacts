# MIT License
#
# Syntacts
# Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab - Rice University
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
option(SYNTACTS_BUILD_GUI           "Turn ON to build Syntacts GUI"      ON)
option(SYNTACTS_BUILD_C_DLL         "Turn ON to build Syntacts C DLL"    ON)
option(SYNTACTS_BUILD_EXAMPLES      "Turn ON to build Syntacts examples" ON)
option(SYNTACTS_BUILD_TESTS         "Turn ON to build Syntacts tests"    ON)
option(SYNTACTS_USE_STATIC_STD_LIBS "Turn ON to link Syntacts against static runtime libs 
                                     (i.e. eliminate VCRUNTIME140.dll, etc. dependency" OFF)

# create project
project(Syntacts VERSION 1.3.0)

# add modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(GNUInstallDirs)

# globally set static linkage
if(MSVC AND SYNTACTS_USE_STATIC_STD_LIBS)
    message("Linking Syntacts against STATIC runtime libraries")
    foreach(flag
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if (${flag} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
        endif()
        if (${flag} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
        endif()
    endforeach()
else()
    message("Linking Syntacts against DYNAMIC runtime libraries")
endif()

# disable FH4 for now (removes deps on new (late 2019) VCRUNTIME140_1.dll)
# https://developercommunity.visualstudio.com/content/problem/852548/vcruntime140-1dll-is-missing.html
if (MSVC AND NOT SYNTACTS_USE_STATIC_STD_LIBS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -d2FH4-")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -d2:-FH4-")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -d2:-FH4-")
endif()

# gather public includes
set(SYNTACTS_INCLUDE 
    "include/Tact/Envelope.hpp"
    "include/Tact/Oscillator.hpp"
    "include/Tact/Signal.hpp"
    "include/Tact/Process.hpp"
    "include/Tact/Serialization.hpp"
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

function(download_zip url filename)
if(NOT EXISTS ${filename})
  file(DOWNLOAD ${url} ${filename}
       TIMEOUT 60  # seconds
       )
endif()
endfunction(download_zip)

# get ASIO SDK
if (WIN32)
    set(ASIO_ZIP_FILENAME "${PROJECT_SOURCE_DIR}/3rdparty/asio.zip")
    if (NOT EXISTS ${ASIO_ZIP_FILENAME})
        # download
        file(DOWNLOAD "https://www.steinberg.net/asiosdk" ${ASIO_ZIP_FILENAME} TIMEOUT 60)
        # extract
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${ASIO_ZIP_FILENAME} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/3rdparty/")
    endif()
endif()

# portaudio
set(PA_DLL_LINK_WITH_STATIC_RUNTIME ON CACHE BOOL "" FORCE)
set(PA_BUILD_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/portaudio)
set_target_properties(portaudio_static PROPERTIES DEBUG_POSTFIX -d)

#===============================================================================
# Syntacts Static C++ Library
#===============================================================================

add_library(syntacts "")
add_library(syntacts::syntacts ALIAS syntacts)
set_target_properties(syntacts PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
target_compile_features(syntacts PUBLIC cxx_std_17)
if (MSVC)
    set_target_properties(syntacts PROPERTIES COMPILE_FLAGS "/bigobj")
endif()
target_sources(syntacts PRIVATE ${SYNTACTS_INCLUDE})
target_sources(syntacts PRIVATE ${SYNTACTS_SRC})
target_compile_definitions(syntacts 
    PUBLIC 
        SYNTACTS_STATIC 
    PRIVATE 
        NOMINMAX
)
if (WIN32 AND PA_USE_ASIO)
    message("Building Syntacts with ASIO support")
    target_compile_definitions(syntacts PUBLIC PA_USE_ASIO)
endif()
target_include_directories(syntacts
	PUBLIC
		$<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cereal/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${portaudio_INCLUDE_DIR}
        3rdparty
)
target_link_libraries(syntacts PUBLIC portaudio_static PRIVATE)

#===============================================================================
# Syntacts C Plugin
#===============================================================================

if (SYNTACTS_BUILD_C_DLL)
    add_subdirectory("c")
endif()

#===============================================================================
# Syntacts GUI
#===============================================================================

if (SYNTACTS_BUILD_GUI)
    set(MAHI_GUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(MAHI_GUI_COROUTINES OFF CACHE BOOL "" FORCE)
    set(MAHI_UTIL_DEFAULT_LOG OFF CACHE BOOL "" FORCE)
    set(MAHI_UTIL_EXAMPLES OFF CACHE BOOL "" FORCE)
    if (MSVC AND SYNTACTS_USE_STATIC_STD_LIBS)
        set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
    endif()
    add_subdirectory(3rdparty/mahi-gui EXCLUDE_FROM_ALL)
    add_subdirectory("gui")
endif()

#===============================================================================
# Syntacts Examples
#===============================================================================

if (SYNTACTS_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

#===============================================================================
# Syntacts Test
#===============================================================================

if (SYNTACTS_BUILD_TESTS)
    add_subdirectory("tests")
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
install(DIRECTORY 3rdparty/cereal/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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
       