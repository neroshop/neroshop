# Build neroshop

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(neroshop C CXX)
include(ExternalProject)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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

# set version
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Echo install prefix
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

# Echo number of PEs found
include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
if(PROCESSOR_COUNT EQUAL 0)     # if ncpus could not be determined, assign 1
  set(PROCESSOR_COUNT "1")
endif()
message(STATUS "Found ${PROCESSOR_COUNT} PEs")

#### Neroshop external libraries

# Set external libraries install directory
if (NOT TPL_DIR)
  set(REL_TPL_DIR ${PROJECT_SOURCE_DIR}/external/install/${COMP}-${ARCH})
  get_filename_component(ABS_TPL_DIR ${REL_TPL_DIR} ABSOLUTE)
  set(TPL_DIR ${ABS_TPL_DIR} CACHE
      STRING "Third-party (external) libraries install directory.")
endif()
set(TPL_INCLUDE_DIRS ${TPL_DIR}/include
                     ${TPL_DIR}/include/common
                     ${TPL_DIR}/include/utils
                     ${TPL_DIR}/include/crypto)
set(QRCODEGEN_INCLUDE_DIRS ${TPL_DIR}/include/qrcodegen)
set(BCRYPT_INCLUDE_DIRS ${TPL_DIR}/include/bcrypt)
set(DOKUNUI_INCLUDE_DIRS ${TPL_DIR}/include/dokun-ui)
message(STATUS "Third-party (external) libraries install directory: " ${TPL_DIR})

#### Neroshop

# configure neroshop daemon executable
set(DAEMON_EXECUTABLE "neromon")
add_executable(${DAEMON_EXECUTABLE}
               src/daemon.cpp
               src/server.cpp)
target_include_directories(${DAEMON_EXECUTABLE} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${DAEMON_EXECUTABLE})
INSTALL(TARGETS ${DAEMON_EXECUTABLE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
message(STATUS "Target ${DAEMON_EXECUTABLE} configured")
