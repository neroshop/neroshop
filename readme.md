# neroshop 

An attempt to create an online marketplace for [Monero](https://getmonero.org/) users, which is likely to fail

[![alt text](res/neroshop-logo.png)](https://github.com/larteyoh/neroshop "neroshop logo")


### Naming: 
The name neroshop comes from "nero", meaning black combined with the word "shop"

## Dependencies:
<!-- * [monero](https://github.com/monero-project/monero) (MIT) -->
|      Library                                                       | Minimum Ver.    | Package                | License                |         Purpose                                                        |
|--------------------------------------------------------------------|-----------------|------------------------|------------------------|------------------------------------------------------------------------|
| [monero-cpp](https://github.com/monero-ecosystem/monero-cpp)       | latest          |                        | MIT                    | monero wallet and payment system                                       |
| [bcrypt](https://github.com/rg3/libbcrypt.git)                     | ?               |                        | public domain          | password hashing                                                       |
| [sqlite3](https://sqlite.org/)                                     | ?               |                        | public domain          | database management                                                    |
| [QR Code generator](https://github.com/nayuki/QR-Code-generator)   | ?               |                        | MIT                    | qr code generation                                                     |
| [json](https://github.com/nlohmann/json/)                          | ?               |                        | MIT                    | json parsing (used in conjunction with libcurl)                        |
| [curl](https://curl.se/libcurl/)                                   | ?               | `libcurl4-openssl-dev` | curl (inspired by MIT) | multiprotocol file transfer (used to retrieve currency exchange rates) |
| [postgresql](https://www.postgresql.org/)                          | ?               | `postgresql`           | MIT                    | client-server database management                                      |
<!-- [dokun-ui](custom library) (MIT) -->


### Features:
* no registration required (for buyers)
* no KYC or AML
* buy and sell products with monero (and possibly other private by default crypto in the near future)
* automatic subaddress generator (a unique monero address is generated from seller's account each time a buyer orders an item)
* address watcher that tracks incoming txs and notifies both the user and seller
("awaiting payment ..."[red], "payment incoming"[yellow], "payment received!"[green])
* seller reputation system (score from 0-1)
* product rating system (stars from 1-5)
* and much more ...


# Compiling neroshop from source
0. Install dependencies
```sh
sudo -s -- << EOF
# neroshop, dokun-ui
sudo apt install libx11-dev libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libcurl4-openssl-dev postgresql
# monero, monero-cpp
sudo apt install git libboost-all-dev cmake g++ make libssl-dev libzmq3-dev libhidapi-dev libudev-dev libusb-1.0-0-dev libfox-1.6-dev # copied from https://github.com/monero-ecosystem/monero-cpp#using-this-library-in-your-project
sudo apt update && sudo apt install build-essential cmake pkg-config libssl-dev libzmq3-dev libunbound-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev libpgm-dev qttools5-dev-tools libhidapi-dev libusb-1.0-0-dev libprotobuf-dev protobuf-compiler libudev-dev libboost-chrono-dev libboost-date-time-dev libboost-filesystem-dev libboost-locale-dev libboost-program-options-dev libboost-regex-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev ccache doxygen graphviz # copied from https://github.com/monero-project/monero#dependencies
EOF
```

1. Clone submodules and nested submodules
```sh
cd external
git clone --recurse-submodules https://github.com/monero-ecosystem/monero-cpp.git
git clone --recurse-submodules https://github.com/rg3/libbcrypt.git
git clone --recurse-submodules https://github.com/nayuki/QR-Code-generator.git
cd ../
```

2. Modify "external/monero-cpp/external/monero-project/CMakeLists.txt":
`option(BUILD_GUI_DEPS "Build GUI dependencies." ON)`
<!-- Step 2 is probably not necessary :/ -->

3. Build monero-project twice to create libwallet_merged.a and other .a libraries
```sh
cd external/monero-cpp/external/monero-project && make release-static -j8 && make release-static -j8
cd ../../../../
```

4. Build neroshop (along with dokun-ui)
```sh
sudo -s -- << EOF
# Build dokun-ui
# make sure CMakeCache.txt, cmake_install.cmake, and Makefile have all been deleted if not
cd external/dokun-ui
cmake -G"Unix Makefiles"
make
cd ../../
# Build neroshop
cmake -G"Unix Makefiles"
make
EOF
```

### NOTE:
I am an awful programmer and still consider myself a noob at it since I don't know what I'm doing half of the time, so if there's anyone out there who can help me bring
my project to life, I'd highly appreciate it :3 ! 
I am also willing to compensate any contributors with Monero when I become financially stable >.< .

[//]: # (git add CMakeLists.txt external/ include/ readme.md res/neroshop-logo.png res/wallets src/ todo.txt)
[//]: # (git commit -m"empty commit")
[//]: # (git push -u origin main)
