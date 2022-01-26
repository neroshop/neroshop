# neroshop 

An attempt to create an online marketplace for [Monero](https://getmonero.org/) users, which is likely to fail

[![alt text](res/neroshop-logo.png)](https://github.com/larteyoh/neroshop "neroshop logo")

language: `c++`

### naming: 
The name neroshop comes from "nero", meaning black combined with the word "shop"

## dependencies:
* [monero-cpp](https://github.com/monero-ecosystem/monero-cpp) (MIT) <!-- * [monero](https://github.com/monero-project/monero) (MIT) -->
* [bcrypt](https://github.com/libbcrypt/) (public domain)
* [sqlite3](https://www.sqlite.org/) (public domain)
* [QR Code generator](https://github.com/nayuki/QR-Code-generator) (MIT)
* [json](https://github.com/nlohmann/json/) (MIT)
* [curl](https://curl.se/libcurl/) (The curl license - inspired by MIT/X)
<!-- [dokun-ui](custom library) (MIT) -->

### features:
* no registration required (for buyers)
* no KYC or AML
* buy and sell products with monero (and possibly other private by default crypto in the near future)
* automatic subaddress generator (a unique monero address is generated from seller's account each time a buyer orders an item)
* address watcher that tracks incoming txs and notifies both the user and seller
("awaiting payment ..."[red], "payment incoming"[yellow], "payment received!"[green])
* seller reputation system (score from 0-1)
* product rating system (stars from 1-5)
* and much more ...

## Building neroshop
```sh
# before you start, install ccache for faster rebuilds: sudo apt install ccache
# install essential build tools:
sudo apt install build-essential                      # required by both monero and neroshop (g++, make)(min_version: 5)
sudo apt install cmake                                # required by both monero and neroshop (min_version: 3.5)
sudo apt install git                                  # required by both monero and neroshop (min_version: ?)

# install neroshop dependencies:
sudo apt install libx11-dev                       # required by both dokun-ui and neroshop (min_version: ?)
sudo apt install libgl1-mesa-dev libglu1-mesa-dev # required by both dokun-ui and neroshop (min_version: 3.3)
sudo apt install libcurl4-openssl-dev             # required by neroshop (min_version: ?)

#install monero dependencies (required):
sudo apt install pkg-config             #  (min_version: any)

# install monero and monero-cpp dependencies (required):
sudo apt install libboost-all-dev libssl-dev libzmq3-dev             # (min_version_Boost: 1.58, min_version_OpenSSL: any, min_version_libzmq: 4.2.0)

# install monero dependencies (required, but not listed by monero-cpp):
# sudo apt install libpgm-dev libunbound-dev libsodium-dev             # (min_version_OpenPGM: ?, min_version_libunbound: 1.4.16, min_version_libsodium: ?)

# install hw dependencies (for hardware wallet support):
sudo apt install libhidapi-dev libprotobuf-dev libudev-dev libusb-1.0-0-dev protobuf-compiler             # (min_version: ?, min_version: ?, min_version: ?, min_version: ?, min_version: ?)

# install monero-cpp dependencies (required - for GUI):
# sudo apt install libfox-1.6-dev             # x11, opengl, png, etc. (min_version: )

# to build the latest version of monero-cpp, follow the instructions here: https://github.com/monero-ecosystem/monero-cpp#using-this-library-in-your-project
# or https://github.com/monero-project/monero#compiling-monero-from-source
# update submodules (dependencies)
##################################
# git clone --recurse-submodules https://github.com/monero-ecosystem/monero-cpp.git
# cd ./monero-cpp
##                              ##
#!/usr/bin/env bash

# initialize submodules recursively
git submodule update --init --force --recursive

# update monero-project
cd ./external/monero-project
git checkout master
git pull --ff-only origin master
cd ../../
##################################
# modify "external/monero-cpp/external/monero-project/CMakeLists.txt"
option(BUILD_GUI_DEPS "Build GUI dependencies." ON)
# build monero-project twice to create libwallet_merged.a and other .a libraries
cd external/monero-cpp/external/monero-project && make release-static -j8 && make release-static -j8

# next, build dokun-ui as static library (.a)
cd external/dokun-ui
cmake -G"Unix Makefiles"
make
# finally, we can build neroshop
cd ../../
cmake -G"Unix Makefiles"
make
```

### NOTE:
I am an awful programmer and still consider myself a noob at it since I don't know what I'm doing half of the time, so if there's anyone out there who can help me bring
my project to life, I'd highly appreciate it! 
I am also willing to compensate any contributors with Monero when I become financially stable.

[//]: # (git add CMakeLists.txt external/ include/ readme.md res/neroshop-logo.png res/wallets src/ todo.txt)
[//]: # (git commit -m"empty commit")
[//]: # (git push -u origin main)
