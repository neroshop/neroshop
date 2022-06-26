# Set default value for building shared libs if none was specified

if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries. Possible values: ON | OFF")
    message(STATUS "BUILD_SHARED_LIBS not specified, setting to 'ON'")
endif()

message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
