# Build neroshop third-party Libraries

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(Neroshop-external C CXX)
include(ExternalProject)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Disallow in-source builds
include(DisallowInSourceBuilds)
# Set CMAKE_BUILD_TYPE
include(BuildType)
# Detect operating system type and version
include(DetectOS)
# Set BUILD_SHARED_LIBS
include(BuildShared)
# Sets CMAKE_INSTALL_{BIN,LIB}DIR needed for multilib support
include(GNUInstallDirs)
# Detect compiler version
include(DetectCompilerVersion)
# Get compiler flags
include(get_compiler_flags)
# Query target architecture
include(TargetArch)

# Set compiler id
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMP)

# Set install directory based on compiler ID if was not given on command line
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/${COMP}-${ARCH}"
      CACHE STRING "Third-party libraries install prefix." FORCE)
  message(STATUS "CMAKE_INSTALL_PREFIX not specified, setting to '${PROJECT_SOURCE_DIR}/install/${COMP}-${ARCH}'")
else()
  message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
endif()

# Echo number of PEs found
include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
if(PROCESSOR_COUNT EQUAL 0)     # if ncpus could not be determined, assign 1
  set(PROCESSOR_COUNT "1")
endif()
message(STATUS "Found ${PROCESSOR_COUNT} PEs")

# Configure build for libraries

# QrCodeGen
ExternalProject_Add(
  qrcodegen
  PREFIX qrcodegen
  GIT_REPOSITORY https://github.com/nayuki/QR-Code-generator
  GIT_TAG v1.8.0
  GIT_SHALLOW 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${CMAKE_COMMAND} -E chdir cpp make
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_BINARY_DIR}/qrcodegen/src/qrcodegen/cpp
                  ${CMAKE_INSTALL_PREFIX}/qrcodegen
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
)

# monero
ExternalProject_Add(
  monero
  PREFIX monero
  GIT_REPOSITORY https://github.com/monero-project/monero
  GIT_TAG v0.17.3.2
  GIT_SHALLOW 1
  PATCH_COMMAND patch -p1 -t -i ${PROJECT_SOURCE_DIR}/../cmake/monero.patch
  CMAKE_ARGS
     -D CMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
     -D CMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
     -D BUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
     -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
     -D BUILD_SHARED_LIBS:BOOL=false
     -D BUILD_GUI_DEPS:BOOL=true
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)

# monero-cpp
ExternalProject_Add(
  monerocpp
  PREFIX monerocpp
  DEPENDS monero
  GIT_REPOSITORY https://github.com/monero-ecosystem/monero-cpp
  GIT_TAG v0.7.1
  GIT_SHALLOW 1
  PATCH_COMMAND patch -p1 -t -i ${PROJECT_SOURCE_DIR}/../cmake/monero-cpp.patch
  CMAKE_ARGS
    -D CMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
    -D CMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
    -D BUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -D MONERO_PROJECT:STRING=${PROJECT_BINARY_DIR}/monero/src/monero
    -D MONERO_PROJECT_BUILD:STRING=${PROJECT_BINARY_DIR}/monero/src/monero-build
    -D BUILD_SCRATCHPAD:BOOL=off
    -D BUILD_SAMPLE:BOOL=off
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)

# rapidjson from monero
ExternalProject_Add(
  rapidjson
  PREFIX rapidjson
  DEPENDS monero
  SOURCE_DIR ${PROJECT_BINARY_DIR}/monero/src/monero/external/rapidjson
  CMAKE_ARGS
     -D CMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
     -D CMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
     -D CMAKE_CXX_FLAGS:STRING=-w
     -D BUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
     -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
     -D RAPIDJSON_BUILD_TESTS:BOOL=false
     -D RAPIDJSON_BUILD_EXAMPLES:BOOL=false
  DOWNLOAD_COMMAND ""
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)

# json
ExternalProject_Add(
  json
  PREFIX json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.10.5
  GIT_SHALLOW 1
  CMAKE_ARGS
    -D CMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
    -D CMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
    -D BUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -D JSON_BuildTests:BOOL=off
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)

# bcrypt
ExternalProject_Add(
  bcrypt
  PREFIX bcrypt
  GIT_REPOSITORY https://github.com/rg3/libbcrypt
  GIT_SHALLOW 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_BINARY_DIR}/bcrypt/src/bcrypt
                  ${CMAKE_INSTALL_PREFIX}/bcrypt
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)

# dokun-ui
ExternalProject_Add(
  dokunui
  PREFIX dokunui
  GIT_REPOSITORY https://github.com/larteyoh/dokun-ui
  GIT_TAG main
  GIT_SHALLOW 1
  PATCH_COMMAND patch -p1 -t -i ${PROJECT_SOURCE_DIR}/../cmake/dokun-ui.patch
  CMAKE_ARGS
    -D CMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
    -D CMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
    -D BUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -D CMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
)
