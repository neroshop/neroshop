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
if (EXISTS ${TPL_DIR}/include)
  set(TPL_INCLUDE_DIR ${TPL_DIR}/include)
endif()
message(STATUS "Third-party (external) libraries install directory: " ${TPL_DIR})

find_package(CURL REQUIRED)
set(nlohmann_json_DIR ${TPL_DIR}/lib/cmake/nlohmann_json)
find_package(nlohmann_json REQUIRED)

#### Neroshop

# configure neroshop daemon executable
set(DAEMON_EXECUTABLE "neromon")
add_executable(${DAEMON_EXECUTABLE}
               src/daemon.cpp
               src/server.cpp
)
target_include_directories(${DAEMON_EXECUTABLE} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  # TODO: should not depend on GUI
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/include
)
target_link_libraries(${DAEMON_EXECUTABLE}
)
INSTALL(TARGETS ${DAEMON_EXECUTABLE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
message(STATUS "Target ${DAEMON_EXECUTABLE} configured")

# configure neroshop gui executable
set(GUI_CLIENT_EXECUTABLE "neroshop-gui")
add_executable(${GUI_CLIENT_EXECUTABLE}
               src/buyer.cpp
               src/cart.cpp
               src/catalog.cpp
               src/client.cpp
               src/converter.cpp
               src/database.cpp
               src/encryptor.cpp
               src/icon.cpp
               src/item.cpp
               src/main.cpp
               src/message.cpp
               src/order.cpp
               src/qr.cpp
               src/seller.cpp
               src/server.cpp
               src/user.cpp
               src/validator.cpp
               src/wallet.cpp
               # TODO: remove this ->
               #external/sqlite/sqlite3.c
               #external/libbcrypt/crypt_blowfish/crypt_blowfish.c external/libbcrypt/crypt_blowfish/crypt_gensalt.c external/libbcrypt/crypt_blowfish/wrapper.c external/libbcrypt/bcrypt.c
)
target_include_directories(${GUI_CLIENT_EXECUTABLE} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/database/include
  ${TPL_INCLUDE_DIR}
  ${CURL_INCLUDE_DIRS}
  ${QRCODEGEN_INCLUDE_DIRS}
  ${MONEROCPP_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/external/libbcrypt
  ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite
  ${CMAKE_CURRENT_SOURCE_DIR}/external/json/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/lua/lua-5.4.1/src
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/png
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/jpeg
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/gif/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/tiff/libtiff
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/freetype/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/external//external/monero-cpp/external/monero-project/wallet
  ${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/external/monero-project/external/rapidjson/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/external/monero-project/contrib/epee/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/external/monero-project/external/easylogging++
  ${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/external/monero-project/src
  ${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/external/monero-project/external
)
#add_dependencies(${GUI_CLIENT_EXECUTABLE})
target_link_libraries(${GUI_CLIENT_EXECUTABLE}
                      ${CURL_LIBRARIES}
                      ${QRCODEGEN_LIBRARIES}
                      ${MONEROCPP_LIBRARIES}
                      ${DOKUNUI_LIBRARIES}
                      #libpthread.so libcrypto.so
)
message(STATUS "Target ${GUI_CLIENT_EXECUTABLE} configured")
