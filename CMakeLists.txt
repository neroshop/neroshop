cmake_minimum_required(VERSION 3.4.1)

project(neroshop) # tested on Ubuntu 20.04 LTS (focal) with gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
######################################
if(WIN32)
    set(WINDOWS TRUE)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
    option(NEROSHOP_CLANG "use clang" OFF) # clang
    option(NEROSHOP_GCC   "use gcc"   ON ) # gcc   - shipped with Ubuntu by default    
    set(CMAKE_C_COMPILER   /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
    set(CMAKE_CXX_FLAGS    "-no-pie"   ) # no position independent executable (PIE) # NEROSHOP_GCC 
    include_directories(/usr/include/          )
    link_directories(/usr/lib/x86_64-linux-gnu/)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(ANDROID TRUE)
endif()
######################################
if(LINUX) # if user chooses to compile with clang
if(NEROSHOP_CLANG) # cmake -DNEROSHOP_CLANG=ON -G"Unix Makefiles"
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # check if clang is set as the current compiler
    set(NEROSHOP_GCC OFF )  
    set(NEROSHOP_CLANG ON) # use clang (if not using gcc)
    set(CMAKE_C_COMPILER   /usr/bin/clang  )
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    set(CMAKE_CXX_FLAGS    "-w"            ) #-w=ignores all warnings
    set(CMAKE_CXX_FLAGS    "-fno-PIE"      ) #-fPIE, -fno-PIE, -fPIC, -fno-PIC, -nopie, -no-pie
    message("using clang")
    include_directories(/usr/include/          )
    link_directories(/usr/lib/x86_64-linux-gnu/)    
else()
    message(WARNING "clang is not set as the default compiler!")
    message("using gcc/g++")
endif()
endif()
endif()
######################################
# use C++11
set(CMAKE_CXX_STANDARD 11)
######################################
######################################
set(MONERO_PROJECT "${CMAKE_SOURCE_DIR}/external/monero-cpp/external/monero-project")
set(MONERO_PROJECT_SRC "${MONERO_PROJECT}/src")

# header includes
include_directories(src/)
include_directories("${MONERO_PROJECT}/contrib/epee/include")
include_directories("${MONERO_PROJECT}/external/")
include_directories("${MONERO_PROJECT}/external/easylogging++")
include_directories("${MONERO_PROJECT}/external/rapidjson/include")
include_directories("${MONERO_PROJECT_SRC}/")
include_directories("${MONERO_PROJECT_SRC}/wallet")
include_directories("${MONERO_PROJECT_SRC}/wallet/api")
include_directories("${MONERO_PROJECT_SRC}/crypto/crypto_ops_builder/include/")

set(EXTERNAL_LIBS_DIR ${CMAKE_SOURCE_DIR}/monero-cpp/external-libs)
message(STATUS EXTERNAL_LIBS_DIR : ${EXTERNAL_LIBS_DIR})
######################################
# Extra libraries
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

message(STATUS EXTRA_LIBRARIES: ${EXTRA_LIBRARIES})

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
# Boost - sudo apt install libboost-dev, sudo apt-get install libboost-all-dev
set(Boost_NO_BOOST_CMAKE 1)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.58 QUIET REQUIRED COMPONENTS chrono date_time filesystem program_options regex serialization wserialization system thread)
message(STATUS "Using Boost include dir at ${Boost_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS) # to ignore warnings

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(pthread_src /usr/lib/x86_64-linux-gnu/libpthread.so) # has to be linked dynamically or will cause "undefined reference to `_dl_stack_flags'" error
    set(boost_src 
        /usr/lib/x86_64-linux-gnu/libboost_chrono.a 
        /usr/lib/x86_64-linux-gnu/libboost_date_time.a 
        /usr/lib/x86_64-linux-gnu/libboost_filesystem.a 
        /usr/lib/x86_64-linux-gnu/libboost_program_options.a 
        /usr/lib/x86_64-linux-gnu/libboost_regex.a 
        /usr/lib/x86_64-linux-gnu/libboost_serialization.a 
        /usr/lib/x86_64-linux-gnu/libboost_wserialization.a 
        /usr/lib/x86_64-linux-gnu/libboost_system.a 
        /usr/lib/x86_64-linux-gnu/libboost_thread.a 
        ${pthread_src}) # /usr/lib/ also contains these files
endif()
######################################
# OpenSSL - sudo apt-get install libssl-dev
if (APPLE AND NOT IOS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64 -fvisibility=default -std=c++11")
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
#[[include_directories(monero-cpp/external/libsodium/include/sodium) # export.h
set(SODIUM_LIBRARIES monero-cpp/external/libsodium/src/crypto_verify/verify.c)]]
#set(sodium_src /usr/lib/x86_64-linux-gnu/libsodium.so)
######################################
# HIDAPI - https://github.com/libusb/hidapi/releases / sudo apt-get install libhidapi-dev / sudo apt-get install libusb-1.0-0-dev, sudo apt-get install libudev-dev (libudev.h)
if(APPLE)
  include_directories(SYSTEM /usr/include/malloc)
  if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
  endif()
endif()

if (APPLE AND NOT IOS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64 -fvisibility=default -std=c++11")
  if (NOT OPENSSL_ROOT_DIR)
      EXECUTE_PROCESS(COMMAND brew --prefix openssl
        OUTPUT_VARIABLE OPENSSL_ROOT_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using OpenSSL found at ${OPENSSL_ROOT_DIR}")
  endif()
endif()

#find_package(HIDAPI REQUIRED)

message(STATUS "Using HIDAPI include dir at ${HIDAPI_INCLUDE_DIR}")
add_definitions(-DHAVE_HIDAPI)
include_directories(${HIDAPI_INCLUDE_DIR})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(udev_src /usr/lib/x86_64-linux-gnu/libudev.so) # only dynamic library is available
    set(hidapi_src ${udev_src} /usr/lib/x86_64-linux-gnu/libhidapi-libusb.so) # has to be dynamically linked or you'll get "undefined reference to `libusb_get_device_address'" error # /usr/lib/x86_64-linux-gnu/libhidapi-hidraw.so
endif()
######################################
# Monero (monero-0.17.2.3 contents pasted in "monero-cpp/external/monero-project")
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
set(monero_src wallet_merged wallet_crypto #[[wallet_api]] wallet lmdb epee unbound easylogging cryptonote_core cryptonote_basic cryptonote_format_utils_basic mnemonics ringct ringct_basic common cncrypto blockchain_db blocks checkpoints device device_trezor multisig version randomx 
${boost_src} ${Protobuf_LIBRARY} ${LibUSB_LIBRARIES} ${OPENSSL_LIBRARIES} ${SODIUM_LIBRARY} ${hidapi_src} ${EXTRA_LIBRARIES}) #${Boost_LIBRARIES} #${HIDAPI_LIBRARIES}
######################################
# monero-cpp (wrapper) v0.5.1 => v0.5.5 => v0.5.10 => v0.6.4
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/libbcrypt) # (wrapper for https://www.openwall.com/crypt/)
set(bcrypt_src external/libbcrypt/crypt_blowfish/crypt_blowfish.c external/libbcrypt/crypt_blowfish/crypt_gensalt.c external/libbcrypt/crypt_blowfish/wrapper.c external/libbcrypt/crypt_blowfish/x86.S external/libbcrypt/bcrypt.c)
######################################
# sqlite3
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite) # v3.36.0 (https://www.sqlite.org/download.html)
set(sqlite_src ${CMAKE_CURRENT_SOURCE_DIR}/external/sqlite/sqlite3.c)
######################################
# QR-Code-generator
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/QR-Code-generator/cpp) # v1.7.0
set(qr_code_generator_src ${CMAKE_CURRENT_SOURCE_DIR}/external/QR-Code-generator/cpp/qrcodegen.cpp) # or ${CMAKE_CURRENT_SOURCE_DIR}/external/QR-Code-generator/c/qrcodegen.c
######################################
# json
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/json/single_include) # v3.10.3
######################################
# curl
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/curl/include ${CMAKE_CURRENT_SOURCE_DIR}/external/curl/lib) # or include_directories(${CURL_INCLUDE_DIRS}) # where to find curl/curl.h, etc.
set(curl_src /usr/local/lib/libcurl.a) # or ${CMAKE_CURRENT_SOURCE_DIR}/external/curl/lib/.libs/libcurl.a) # or ${CURL_LIBRARIES})
add_definitions(-DCURL_STATICLIB)
######################################
# postgresql
include_directories(${PostgreSQL_INCLUDE_DIRS}) # v14.1
set(postgresql_src ${PostgreSQL_LIBRARIES}) # /usr/lib/x86_64-linux-gnu/libpq.a  OR  /usr/lib/x86_64-linux-gnu/libpq.so
######################################
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
set(opengl_src    opengl32.lib glu32.lib)
set(openal_src    OpenAL32.lib          ) # not currently in use
set(winsocket_src ws2_32.lib            ) # -lglfw3 -lgdi32 -lopengl32 -limm32
endif()
if(LINUX)
set(opengl_src libGL.so libGLU.so                   )
set(OpenGL_GL_PREFERENCE "LEGACY"                   ) # LEGACY(libGL.so) OR GLVND(libOpenGL.so and libGLX.so)
set(openal_src libopenal.so                         ) # not currently in use
set(posix_src  libpthread.so libdl.so libm.so       )
set(x11_src    libX11.a libxcb.a libXau.a libXdmcp.a)
endif() # -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
######################################
# dokun-ui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/include)
set(dokun_ui_src ${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui/src/libdokun-ui.a)#libdokun-ui.a) # definitions added: DOKUN_OPENGL, DOKUN_BUILD_LIBRARY#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/dokun-ui)
######################################
# neroshop
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) # includes (.hpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies) # dependencies (monero-cpp, dokun-ui, etc.)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src) # sources (.cpp, .a)
set(neroshop_src src/buyer.cpp #[[src/carrier.cpp]] src/cart.cpp src/catalog.cpp src/client.cpp src/converter.cpp src/db.cpp src/encryptor.cpp src/icon.cpp src/item.cpp src/main.cpp src/message.cpp src/order.cpp src/qr.cpp src/seller.cpp src/server.cpp src/user.cpp src/validator.cpp src/wallet.cpp)
add_definitions(-DNEROSHOP_DEBUG) # for more detailed debugging, use -DNEROSHOP_DEBUG0
add_executable(neroshop ${neroshop_src} ${monero_cpp_src} ${bcrypt_src} ${sqlite_src} ${qr_code_generator_src})
target_link_libraries(neroshop ${curl_src})
target_link_libraries(neroshop ${monero_src})
target_link_libraries(neroshop ${dokun_ui_src})
######################################
# daemon
add_executable(daemon src/daemon.cpp src/server.cpp)
#target_link_libraries(daemon ${curl_src} ${OPENSSL_LIBRARIES}) # curl requires both openssl(used in monero) and zlib(used in dokun-ui)
target_link_libraries(daemon ${dokun_ui_src})
#set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl") # fixes undefined reference to symbol 'dlsym@@GLIBC_2.2.5' error
# change name to neromon (nero + daemon :D)
set_target_properties(daemon PROPERTIES OUTPUT_NAME "neromon")
######################################
# linking
if(LINUX) 
    target_link_libraries(neroshop ${posix_src})
    target_link_libraries(daemon ${posix_src})
    find_package(X11 REQUIRED)
    if(X11_FOUND)
	    target_link_libraries(neroshop ${x11_src})
	    target_link_libraries(daemon ${x11_src})
    endif()
endif()
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	target_link_libraries(neroshop ${opengl_src})
	target_link_libraries(daemon ${opengl_src})
endif()
find_package(PostgreSQL) # -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/
if(PostgreSQL_FOUND) #message("-- Using PostgreSQL: ${PostgreSQL_LIBRARIES} (v${PostgreSQL_VERSION_STRING})")
    target_link_libraries(neroshop ${PostgreSQL_LIBRARIES})
    target_link_libraries(daemon ${PostgreSQL_LIBRARIES})
endif()
# cmake -G"Unix Makefiles"
# to set the current compiler manually, use the following commands:
# sudo update-alternatives --config cc
# sudo update-alternatives --config c++
