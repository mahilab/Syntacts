#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "portaudio" for configuration ""
set_property(TARGET portaudio APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(portaudio PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "/System/Library/Frameworks/CoreAudio.framework;/System/Library/Frameworks/AudioToolbox.framework;/System/Library/Frameworks/AudioUnit.framework;/System/Library/Frameworks/Carbon.framework;m;pthread"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libportaudio.dylib"
  IMPORTED_SONAME_NOCONFIG "@rpath/libportaudio.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS portaudio )
list(APPEND _IMPORT_CHECK_FILES_FOR_portaudio "${_IMPORT_PREFIX}/lib/libportaudio.dylib" )

# Import target "portaudio_static" for configuration ""
set_property(TARGET portaudio_static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(portaudio_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "/System/Library/Frameworks/CoreAudio.framework;/System/Library/Frameworks/AudioToolbox.framework;/System/Library/Frameworks/AudioUnit.framework;/System/Library/Frameworks/Carbon.framework;m;pthread"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libportaudio.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS portaudio_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_portaudio_static "${_IMPORT_PREFIX}/lib/libportaudio.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
