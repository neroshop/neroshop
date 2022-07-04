# Set default value for building shared libs if none was specified

if(NOT DEFINED BUILD_SHARED_LIBS)
  option(BUILD_SHARED_LIBS "Build shared libraries" ON)
  message(STATUS "BUILD_SHARED_LIBS not specified, setting to 'ON'")
else()
  message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
endif()
