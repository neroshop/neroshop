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

# Set compiler var (will be exported to the source)
set(COMPILER "${CMAKE_CXX_COMPILER}")

# set version
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Set the requirement for the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Required language standard: C++${CMAKE_CXX_STANDARD}")

# Set build type (will be exported to the source)
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
string(TOLOWER "${BUILD_TYPE}" BUILD_TYPE)

# Echo install prefix
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

# Echo number of PEs found
include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
if(PROCESSOR_COUNT EQUAL 0)     # if ncpus could not be determined, assign 1
  set(PROCESSOR_COUNT "1")
endif()
message(STATUS "Found ${PROCESSOR_COUNT} PEs")

set(NEROSHOP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
                     ${TPL_DIR}/include/cli
                     ${TPL_DIR}/include/crypto
                     ${TPL_DIR}/include/storages)
message(STATUS "Third-party (external) libraries install directory: " ${TPL_DIR})

# Find daniele77/cli
set(cli_DIR ${TPL_DIR}/lib/cmake/cli)
find_package(cli REQUIRED)

# Find QrCodeGen
find_path(QRCODEGEN_INCLUDE_DIR NAMES qrcodegen.hpp PATHS ${TPL_DIR}/qrcodegen)
find_library(QRCODEGEN_LIBRARY libqrcodegencpp.a ${TPL_DIR}/qrcodegen)

# Find Bcrypt
find_path(BCRYPT_INCLUDE_DIR NAMES bcrypt.h PATHS ${TPL_DIR}/bcrypt)
find_library(BCRYPT_LIBRARY bcrypt.a ${TPL_DIR}/bcrypt)

# Find Lua
find_package(Lua REQUIRED)

# Find FreeType
find_package(Freetype REQUIRED)

# Find Curl
find_package(CURL REQUIRED)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Find OpenGL
set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

# Find dokun-ui
find_path(DOKUNUI_INCLUDE_DIR NAMES image.hpp PATHS ${TPL_DIR}/include/dokun-ui)
find_library(DOKUNUI_LIBRARY libdokun-ui.a ${TPL_DIR}/lib)

# Find Monero
find_library(MONEROCPP_LIBRARIES libmonero-cpp.so ${TPL_DIR}/lib)

# Find Boost
find_package(Boost REQUIRED COMPONENTS serialization program_options)

# Find ZeroMQ
find_package(ZMQ REQUIRED)

# Find Xapian
find_package(Xapian REQUIRED)

# Find easylogging++
find_package(EASYLOGGINGPP REQUIRED)
add_definitions(-DAUTO_INITIALIZE_EASYLOGGINGPP -DELPP_FEATURE_CRASH_LOG -DELPP_THREAD_SAFE -DELPP_NO_CHECK_MACROS -DELPP_NO_DEBUG_MACROS)

#### Neroshop

# server library
add_library(server ${NEROSHOP_SOURCE_DIR}/server.cpp)
target_include_directories(server PUBLIC ${NEROSHOP_SOURCE_DIR})
set_target_properties(server PROPERTIES LIBRARY_OUTPUT_NAME neroshop_server)
install(TARGETS server
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
)

# client library
add_library(client ${NEROSHOP_SOURCE_DIR}/client.cpp)
target_include_directories(client PUBLIC ${NEROSHOP_SOURCE_DIR})
set_target_properties(client PROPERTIES LIBRARY_OUTPUT_NAME neroshop_client)
install(TARGETS client
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
)

add_library(db ${NEROSHOP_SOURCE_DIR}/db.cpp)
target_include_directories(db PUBLIC ${NEROSHOP_SOURCE_DIR})
set_target_properties(db PROPERTIES LIBRARY_OUTPUT_NAME neroshop_db)
install(TARGETS db
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
)

# configure neroshop daemon executable
set(DAEMON_EXECUTABLE "neromon")
add_executable(${DAEMON_EXECUTABLE}
               ${CMAKE_BINARY_DIR}/project_config.cpp
               ${NEROSHOP_SOURCE_DIR}/daemon.cpp
               ${EASYLOGGINGPP_INCLUDE_DIR}/easylogging++.cc)
target_include_directories(${DAEMON_EXECUTABLE} PUBLIC
                           ${NEROSHOP_SOURCE_DIR}
                           ${PROJECT_BINARY_DIR})
target_link_libraries(${DAEMON_EXECUTABLE}
                      db
                      ${Boost_LIBRARIES}
                      ${XAPIAN_LIBRARIES}
                      ${ZMQ_LIBRARIES}
                      Threads::Threads)
install(TARGETS ${DAEMON_EXECUTABLE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
message(STATUS "Target ${DAEMON_EXECUTABLE} configured")

# configure neroshop cli executable
set(CLI_EXECUTABLE "neroshop-cli")
add_executable(${CLI_EXECUTABLE}
               ${CMAKE_BINARY_DIR}/project_config.cpp
               ${NEROSHOP_SOURCE_DIR}/cli.cpp
               ${EASYLOGGINGPP_INCLUDE_DIR}/easylogging++.cc)
target_include_directories(${CLI_EXECUTABLE} PUBLIC
                           ${NEROSHOP_SOURCE_DIR}
                           ${PROJECT_BINARY_DIR})
target_link_libraries(${CLI_EXECUTABLE} PRIVATE
                      cli::cli
                      ${ZMQ_LIBRARIES})
install(TARGETS ${CLI_EXECUTABLE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
message(STATUS "Target ${CLI_EXECUTABLE} configured")

# configure neroshop cli executable
set(GUI_EXECUTABLE "neroshop-gui")
add_executable(${GUI_EXECUTABLE}
               ${CMAKE_BINARY_DIR}/project_config.cpp
               ${NEROSHOP_SOURCE_DIR}/qr.cpp
               ${NEROSHOP_SOURCE_DIR}/buyer.cpp
               ${NEROSHOP_SOURCE_DIR}/cart.cpp
               ${NEROSHOP_SOURCE_DIR}/catalog.cpp
               ${NEROSHOP_SOURCE_DIR}/converter.cpp
               ${NEROSHOP_SOURCE_DIR}/database.cpp
               ${NEROSHOP_SOURCE_DIR}/encryptor.cpp
               ${NEROSHOP_SOURCE_DIR}/icon.cpp
               ${NEROSHOP_SOURCE_DIR}/item.cpp
               ${NEROSHOP_SOURCE_DIR}/main.cpp
               ${NEROSHOP_SOURCE_DIR}/message.cpp
               ${NEROSHOP_SOURCE_DIR}/order.cpp
               ${NEROSHOP_SOURCE_DIR}/qr.cpp
               ${NEROSHOP_SOURCE_DIR}/seller.cpp
               ${NEROSHOP_SOURCE_DIR}/server.cpp
               ${NEROSHOP_SOURCE_DIR}/user.cpp
               ${NEROSHOP_SOURCE_DIR}/validator.cpp
               ${NEROSHOP_SOURCE_DIR}/wallet.cpp)
target_include_directories(${GUI_EXECUTABLE} PUBLIC
                           ${NEROSHOP_SOURCE_DIR}
                           ${PROJECT_BINARY_DIR}
                           ${TPL_INCLUDE_DIRS}
                           ${QRCODEGEN_INCLUDE_DIR}
                           ${BCRYPT_INCLUDE_DIR}
                           ${DOKUNUI_INCLUDE_DIR}
                           ${LUA_INCLUDE_DIR}
                           ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${GUI_EXECUTABLE}
                      client
                      ${QRCODEGEN_LIBRARY}
                      ${CURL_LIBRARIES}
                      ${DOKUNUI_LIBRARY}
                      ${OPENSSL_CRYPTO_LIBRARIES}
                      ${X11_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${MONEROCPP_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${BCRYPT_LIBRARY}
                      Threads::Threads dl)
install(TARGETS ${GUI_EXECUTABLE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
message(STATUS "Target ${GUI_EXECUTABLE} configured")

#### Testing

option(ENABLE_TESTS "Enable tests" ON)
if (NOT ENABLE_TESTS)
  message(STATUS "Tests disabled")
else()
  enable_testing()
  add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()

# Grab current date to stick into the executables
execute_process(COMMAND "date" "--utc" OUTPUT_VARIABLE BUILD_DATE)
string(REGEX REPLACE "[\r\n]" "" BUILD_DATE "${BUILD_DATE}")

# Configure cmake variables to pass to the build
configure_file( "${NEROSHOP_SOURCE_DIR}/project_config.hpp.in"
                "${PROJECT_BINARY_DIR}/project_config.hpp" )
configure_file( "${NEROSHOP_SOURCE_DIR}/project_config.cpp.in"
                "${PROJECT_BINARY_DIR}/project_config.cpp" )
