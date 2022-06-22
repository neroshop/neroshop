cmake_minimum_required(VERSION 3.4.1)

project(neroshop C CXX)

# setup cmake module paths
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/cmake)

# disallow in-source builds
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR "ERROR: In-source builds are not allowed!\n"
  "Please create a separate directory and configure there; for example:\n"
  "  mkdir build\n"
  "  cd build\n"
  "  cmake [OPTIONS] ...\n"
  "NOTE: You must first delete the newly created CMakeCache.txt file and "
  "CMakeFiles directory under the source directory or you will not be able to "
  "configure correctly! For example:\n"
  "  rm -rf CMakeCache.txt CMakeFiles")
endif()

# set version
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# set a default build type
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not specified, setting to 'Release'")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type. Possible values: DEBUG | RELEASE | RELWITHDEBINFO | MINSIZEREL" FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
else()
  message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

# macro to add C++ compiler flags
macro (add_compiler_flag flag)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
endmacro (add_compiler_flag)

# OS-specific setup
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  include_directories(/usr/include/          )
  link_directories(/usr/lib/x86_64-linux-gnu/) # Debian/Ubuntu
  link_directories(/usr/lib/                 ) # Arch
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# ...
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
# ...
endif()

# language standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiler-specific setup
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      #add_compiler_flag("-Weverything")
      add_compiler_flag("-Wno-self-assign-field")
   elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      #add_compiler_flag("-Wall")
      #add_compiler_flag("-no-pie")
      add_compiler_flag("-I'/usr/include/postgresql/'") # Ubuntu 20.04 gcc cannot find libpq-fe.h -.-
  endif()
endif()

# extra libraries
if (APPLE)
  if (DEPENDS)
    list(APPEND EXTRA_LIBRARIES "-framework Foundation -framework IOKit -framework AppKit")
  else()
    find_library(COREFOUNDATION CoreFoundation)
    find_library(IOKIT IOKit)
    find_library(APPKIT AppKit)
    list(APPEND EXTRA_LIBRARIES ${IOKIT})
    list(APPEND EXTRA_LIBRARIES ${COREFOUNDATION})
    list(APPEND EXTRA_LIBRARIES ${APPKIT})
  endif()
endif()
if (WIN32)
  list(APPEND EXTRA_LIBRARIES setupapi)
endif()
if(EXTRA_LIBRARIES)
  message(STATUS EXTRA_LIBRARIES: ${EXTRA_LIBRARIES})
endif()

######################################
# Protobuf (recent)
include(FindProtobuf)
find_package(Protobuf)
message(STATUS "Protobuf lib: ${Protobuf_LIBRARY}, inc: ${Protobuf_INCLUDE_DIR}, protoc: ${Protobuf_PROTOC_EXECUTABLE}")
include_directories(${Protobuf_INCLUDE_DIR})

######################################
# LibUSB (recent)
find_library(usb_LIBRARY NAMES usb-1.0 libusb usb)
set(LibUSB_LIBRARIES ${usb_LIBRARY})

######################################
## Boost
set(Boost_NO_BOOST_CMAKE 1)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.58 QUIET REQUIRED COMPONENTS chrono date_time filesystem program_options regex serialization wserialization system thread)
message(STATUS "Using Boost include dir at ${Boost_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS) # to ignore warnings

######################################
# OpenSSL
if (APPLE AND NOT IOS)
  add_compiler_flag("-march=x86-64")
  add_compiler_flag("-fvisibility=default")
  if (NOT OPENSSL_ROOT_DIR)
      EXECUTE_PROCESS(COMMAND brew --prefix openssl
        OUTPUT_VARIABLE OPENSSL_ROOT_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using OpenSSL found at ${OPENSSL_ROOT_DIR}")
  endif()
endif()
find_package(OpenSSL REQUIRED)
message(STATUS "Using OpenSSL include dir at ${OPENSSL_INCLUDE_DIR}")
include_directories(${OPENSSL_INCLUDE_DIR})

if(STATIC AND NOT IOS)
  if(UNIX)
    set(OPENSSL_LIBRARIES "${OPENSSL_LIBRARIES};${CMAKE_DL_LIBS};${CMAKE_THREAD_LIBS_INIT}")
  endif()
endif()

######################################
# libsodium
include_directories(external/monero-cpp/external/libsodium/include)
include_directories(external/monero-cpp/external/libsodium/include/sodium)
find_library(SODIUM_LIBRARY sodium REQUIRED)
message(STATUS "Using libsodium library at ${SODIUM_LIBRARY}")

######################################
# HIDAPI
if(APPLE)
  include_directories(SYSTEM /usr/include/malloc)
  if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
  endif()
endif()

if (APPLE AND NOT IOS)
  add_compiler_flag("-march=x86-64")
  add_compiler_flag("-fvisibility=default")
  if (NOT OPENSSL_ROOT_DIR)
      EXECUTE_PROCESS(COMMAND brew --prefix openssl
        OUTPUT_VARIABLE OPENSSL_ROOT_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using OpenSSL found at ${OPENSSL_ROOT_DIR}")
  endif()
endif()

find_package(HIDAPI)
if(HIDAPI_FOUND)
    message(STATUS "Using HIDAPI include dir at ${HIDAPI_INCLUDE_DIR}")
    add_definitions(-DHAVE_HIDAPI)
    include_directories(${HIDAPI_INCLUDE_DIR})
else()
    message("HIDAPI libraries not found. Will manually set libraries.")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(udev_src libudev.so)
        set(hidapi_src ${udev_src} libhidapi-libusb.so) # or libhidapi-hidraw.so # has to be dynamically linked or you'll get "undefined reference to `libusb_get_device_address'" error
    endif()
endif()

######################################
# Monero

# header includes
set(MONERO_PROJECT "${CMAKE_SOURCE_DIR}/external/monero-cpp/external/monero-project")
set(MONERO_PROJECT_SRC "${MONERO_PROJECT}/src")
include_directories("${MONERO_PROJECT}/contrib/epee/include")
include_directories("${MONERO_PROJECT}/external/")
include_directories("${MONERO_PROJECT}/external/easylogging++")
include_directories("${MONERO_PROJECT}/external/rapidjson/include")
include_directories("${MONERO_PROJECT_SRC}/")
include_directories("${MONERO_PROJECT_SRC}/wallet")
include_directories("${MONERO_PROJECT_SRC}/wallet/api")
include_directories("${MONERO_PROJECT_SRC}/crypto/crypto_ops_builder/include/")

set(MONERO_PROJECT_BUILD "${MONERO_PROJECT}/build/release")
message(STATUS "Using monero-project build" : ${MONERO_PROJECT_BUILD})

add_library(wallet_merged STATIC IMPORTED)
set_target_properties(wallet_merged PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/lib/libwallet_merged.a)
    
add_library(wallet STATIC IMPORTED)
set_target_properties(wallet PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/lib/libwallet.a)
    
add_library(wallet_crypto STATIC IMPORTED)
set_target_properties(wallet_crypto PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/crypto/wallet/libwallet-crypto.a)
    
add_library(lmdb STATIC IMPORTED)
set_target_properties(lmdb PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/external/db_drivers/liblmdb/liblmdb.a)
    
add_library(epee STATIC IMPORTED)
set_target_properties(epee PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/contrib/epee/src/libepee.a)
    
add_library(unbound STATIC IMPORTED)
set_target_properties(unbound PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/external/unbound/libunbound.a)
    
add_library(easylogging STATIC IMPORTED)
set_target_properties(easylogging PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/external/easylogging++/libeasylogging.a)

add_library(cryptonote_core STATIC IMPORTED)
set_target_properties(cryptonote_core PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/cryptonote_core/libcryptonote_core.a)

add_library(cryptonote_basic STATIC IMPORTED)
set_target_properties(cryptonote_basic PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/cryptonote_basic/libcryptonote_basic.a)
# new addition [2021-10-02]
add_library(cryptonote_format_utils_basic STATIC IMPORTED)
set_target_properties(cryptonote_format_utils_basic PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/cryptonote_basic/libcryptonote_format_utils_basic.a)
    
add_library(mnemonics STATIC IMPORTED)
set_target_properties(mnemonics PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/mnemonics/libmnemonics.a)

add_library(common STATIC IMPORTED)
set_target_properties(common PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/common/libcommon.a)

add_library(cncrypto STATIC IMPORTED)
set_target_properties(cncrypto PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/crypto/libcncrypto.a)

add_library(ringct STATIC IMPORTED)
set_target_properties(ringct PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/ringct/libringct.a)

add_library(ringct_basic STATIC IMPORTED)
set_target_properties(ringct_basic PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/ringct/libringct_basic.a)

add_library(blockchain_db STATIC IMPORTED)
set_target_properties(blockchain_db PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/blockchain_db/libblockchain_db.a)

add_library(blocks STATIC IMPORTED)
set_target_properties(blocks PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/blocks/libblocks.a)

add_library(checkpoints STATIC IMPORTED)
set_target_properties(checkpoints PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/checkpoints/libcheckpoints.a)

add_library(device STATIC IMPORTED)
set_target_properties(device PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/device/libdevice.a)
    
add_library(device_trezor STATIC IMPORTED)
set_target_properties(device_trezor PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/device_trezor/libdevice_trezor.a)

add_library(multisig STATIC IMPORTED)
set_target_properties(multisig PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/multisig/libmultisig.a)

add_library(version STATIC IMPORTED)
set_target_properties(version PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/src/libversion.a)
    
add_library(randomx STATIC IMPORTED)
set_target_properties(randomx PROPERTIES IMPORTED_LOCATION
    ${MONERO_PROJECT_BUILD}/external/randomx/librandomx.a)

######################################
set(MONERO_LIBRARIES wallet_merged wallet_crypto wallet lmdb epee unbound easylogging cryptonote_core cryptonote_basic cryptonote_format_utils_basic mnemonics ringct ringct_basic common cncrypto blockchain_db blocks checkpoints device device_trezor multisig version randomx 
${Boost_LIBRARIES} ${Protobuf_LIBRARY} ${LibUSB_LIBRARIES} ${OPENSSL_LIBRARIES} ${SODIUM_LIBRARY} ${HIDAPI_LIBRARY} ${EXTRA_LIBRARIES})

######################################
# monero-cpp (wrapper)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/src/)
set(monero_cpp_src
    external/monero-cpp/src/utils/gen_utils.cpp
    external/monero-cpp/src/utils/monero_utils.cpp
    external/monero-cpp/src/daemon/monero_daemon_model.cpp
    external/monero-cpp/src/daemon/monero_daemon.cpp
    external/monero-cpp/src/wallet/monero_wallet_model.cpp
    external/monero-cpp/src/wallet/monero_wallet_keys.cpp
    external/monero-cpp/src/wallet/monero_wallet_full.cpp
)

######################################
# libbcrypt
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/libbcrypt)
set(bcrypt_src external/libbcrypt/crypt_blowfish/crypt_blowfish.c external/libbcrypt/crypt_blowfish/crypt_gensalt.c external/libbcrypt/crypt_blowfish/wrapper.c external/libbcrypt/bcrypt.c)

######################################
# sqlite3
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite)
set(sqlite_src ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite/sqlite3.c)

######################################
# QR-Code-generator
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/QR-Code-generator/cpp)
set(qr_code_generator_src ${CMAKE_CURRENT_SOURCE_DIR}/external/QR-Code-generator/cpp/qrcodegen.cpp) # or ${CMAKE_CURRENT_SOURCE_DIR}/external/QR-Code-generator/c/qrcodegen.c

######################################
# json
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/json/single_include)

######################################
# curl
find_package(CURL) # if curl is found on the system, use the system default shared library, otherwise use the curl that we've built from source
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    set(curl_src ${CURL_LIBRARIES})
endif()
if(NOT CURL_FOUND)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/curl/include ${CMAKE_CURRENT_SOURCE_DIR}/external/curl/lib)
    set(curl_src ${CMAKE_CURRENT_SOURCE_DIR}/external/curl/lib/.libs/libcurl.a)
    message("-- Using CURL: ${CURL_LIBRARIES} (v${CURL_VERSION_STRING})")
    add_definitions(-DCURL_STATICLIB)
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -lldap -llber -lnghttp2 -lpsl -lidn2 -lbrotlidec -lzstd -lrtmp") # I guess if we build curl statically we need all libs to also be static, which is unrealistic since most libs on linux-gnu are shared libs # https://stackoverflow.com/questions/68368557/linking-nghttp2-static-library#comment120830125_68368557
endif()

######################################
# lmdb
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/monero-cpp/external/monero-project/external/db_drivers/liblmdb)

######################################
# dokun-ui_dependencies
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/lua/lua-5.4.1/src) # lua
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/glew/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/png ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/zlib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/freetype/include                   # public headers - ft2build.h
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/freetype/include/freetype           # public headers
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/freetype/include/freetype/config    # public config headers
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/freetype/include/freetype/internal) # private headers - added 2019-07-29
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/ogg/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/vorbis/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/vorbis/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/theora/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/utf8/source)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/openal/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/openal-soft/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/jpeg
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/jpeg/jpeg-6b)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/tiff/libtiff)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/gif/lib)
include_directories(#[[${CMAKE_CURRENT_SOURCE_DIR}/external/flac]] # config.h
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/flac/include
   ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/flac/src/libFLAC/include) # for "private/cpu.h"
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/external/glm)

if(WIN32)
set(winsocket_src ws2_32.lib            ) # -lglfw3 -lgdi32 -lopengl32 -limm32
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OpenGL_GL_PREFERENCE "LEGACY"                   ) # LEGACY(libGL.so) OR GLVND(libOpenGL.so and libGLX.so)
set(posix_src  libpthread.so libdl.so libm.so       )
endif() # -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

######################################
# dokun-ui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/include)
set(dokun_ui_src ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/src/libdokun-ui.a) # definitions added: DOKUN_OPENGL, DOKUN_BUILD_LIBRARY#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui)

######################################
# neroshop
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(neroshop_src src/buyer.cpp #[[src/carrier.cpp]] src/cart.cpp src/catalog.cpp src/client.cpp src/converter.cpp src/database.cpp src/encryptor.cpp src/icon.cpp src/item.cpp src/main.cpp src/message.cpp src/order.cpp src/qr.cpp src/seller.cpp src/server.cpp src/user.cpp src/validator.cpp src/wallet.cpp)
add_definitions(-DNEROSHOP_DEBUG) # for more detailed debugging, use -DNEROSHOP_DEBUG0

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/include/postgresql/")
find_package(PostgreSQL)

# daemon executable
set(DAEMON_EXECUTABLE "neromon")
add_executable(${DAEMON_EXECUTABLE}
               src/daemon.cpp
               src/server.cpp
)
target_link_libraries(${DAEMON_EXECUTABLE}
                      ${dokun_ui_src}
                      ${posix_src}
                      ${X11_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${PostgreSQL_LIBRARIES}
)

# neroshop executable
set(NEROSHOP_EXECUTABLE "neroshop")
add_executable(${NEROSHOP_EXECUTABLE}
               ${neroshop_src}
               ${monero_cpp_src}
               ${bcrypt_src}
               ${sqlite_src}
               ${qr_code_generator_src}
)
target_link_libraries(${NEROSHOP_EXECUTABLE}
                      ${curl_src}
                      ${MONERO_LIBRARIES}
                      ${dokun_ui_src}
                      ${posix_src}
                      ${X11_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${PostgreSQL_LIBRARIES}
)

set(EXECUTABLES ${DAEMON_EXECUTABLE}
                ${NEROSHOP_EXECUTABLE})

if(PostgreSQL_FOUND)
    if(DEFINED NEROSHOP_USE_POSTGRESQL) # -DNEROSHOP_USE_POSTGRESQL=1
        message("-- Using PostgreSQL: ${PostgreSQL_LIBRARIES} (v${PostgreSQL_VERSION_STRING})")
        target_compile_definitions(${NEROSHOP_EXECUTABLE} PUBLIC -DNEROSHOP_USE_POSTGRESQL)
    endif() # end of NEROSHOP_USE_POSTGRESQL definition
endif()

message(STATUS "C compiler flags: '${CMAKE_C_FLAGS}'")
message(STATUS "C++ compiler flags: '${CMAKE_CXX_FLAGS}'")
