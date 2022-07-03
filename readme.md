# neroshop 

An attempt to create a P2P marketplace for [Monero](https://getmonero.org/) users, which is likely to fail

[![alt text](res/neroshop-logo.png)](https://github.com/larteyoh/neroshop "neroshop logo")


## Table of contents
- [The history behind neroshop](#about)
- [Features](#features) <!-- - [Documentation](#documentation)-->
- [Building neroshop](#building-neroshop)
  - [Dependencies](#dependencies)
  - [Compiling neroshop from source](#compiling-neroshop-from-source) <!-- - [License](#license)-->
- [Contributions](#contributions)  <!-- - [Bug Bounty Program]-->
- [Contact information](#contact)

## About
This is currently a work in progress. There is nothing to see here :shrug:



## Features
* Pseudonymous identities (sellers are only identified by their usernames and trusted based on their reputation)
* No registration required (for buyers only)
* No KYC or AML
* No listing fees
* No sales tax
* No other fees (except for miner transaction fees and shipping costs)
* Buy and sell products with Monero [XMR] (and Wownero [WOW] in the near future)
* Seller reputation system
* Product rating system
* Wishlist feature
* End-to-end encrypted messaging system for communications between sellers and buyers
* Distributed p2p network
* Subaddress generator (a unique subaddresses will be generated from a seller's synced wallet account for each order placed by a customer)
* Option to run a local Monero node or connect to remote Monero nodes (so that sellers will not have to sync the entire blockchain)
* Option to choose between sending funds directly to a seller or using a multisignature escrow.
* Tor integration (Internet traffic can be optionally routed through tor for more added privacy)
* And much more ...


## Building neroshop

### Dependencies
|      Library                                                       | Minimum Ver.    | Package                | License                            |         Purpose                                                        |
|--------------------------------------------------------------------|-----------------|------------------------|------------------------------------|------------------------------------------------------------------------|
| [monero-cpp](https://github.com/monero-ecosystem/monero-cpp)       | latest          |                        | MIT                                | monero wallet and payment system                                       |
| [lmdb](https://github.com/LMDB/lmdb)                               | ?               |                        | OpenLDAP Public                    | database management                                                    |
| [libbcrypt](https://github.com/rg3/libbcrypt)                      | ?               |                        | public domain (CC0-1.0)            | password hashing                                                       |
| [QR Code generator](https://github.com/nayuki/QR-Code-generator)   | ?               |                        | MIT                                | qr code generation                                                     |
| [json](https://github.com/nlohmann/json/)                          | ?               |                        | MIT                                | json parsing                                                           |
| [curl](https://github.com/curl/curl)                               | ?               | `libcurl4-openssl-dev` | curl                               | currency conversion                                                    |
| [openssl](https://github.com/openssl/openssl)                      | 1.1.1           | `libssl-dev`           | OpenSSL-SSLeay or Apache-2.0       | for curl, sha256 sum and message encryption                            |
| [dokun-ui](external/dokun-ui)                                      | n/a             |                        | MIT                                | graphical user interface                                               |


### Compiling neroshop from source
1. Install dependencies

Debian/Ubuntu
```sh
sudo -s -- << EOF
# prerequisites
sudo apt install build-essential cmake git
# neroshop, dokun-ui
sudo apt install libx11-dev libgl1-mesa-dev libglu1-mesa-dev libcurl4-openssl-dev libssl-dev
# monero-cpp (monero)
sudo apt update && sudo apt install pkg-config libssl-dev libzmq3-dev libunbound-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev libpgm-dev qttools5-dev-tools libhidapi-dev libusb-1.0-0-dev libprotobuf-dev protobuf-compiler libudev-dev libboost-chrono-dev libboost-date-time-dev libboost-filesystem-dev libboost-locale-dev libboost-program-options-dev libboost-regex-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev python3 ccache doxygen graphviz
EOF
```
Arch
```sh
# prerequisites
sudo pacman -Sy --needed base-devel cmake git
# neroshop, dokun-ui
sudo pacman -Sy --needed libx11 lib32-mesa lib32-glu curl openssl
# monero-cpp (monero)
sudo pacman -Syu --needed boost openssl zeromq libpgm unbound libsodium libunwind xz readline ldns expat gtest python3 ccache doxygen graphviz qt5-tools hidapi libusb protobuf systemd
```
Fedora
```sh
# prerequisites
sudo dnf install gcc gcc-c++ make cmake git
# neroshop, dokun-ui
sudo dnf install mesa-libGL-devel mesa-libGLU-devel libcurl-devel openssl-devel
# monero-cpp (monero)
sudo dnf install boost-static libstdc++-static pkgconf boost-devel openssl-devel zeromq-devel openpgm-devel unbound-devel libsodium-devel libunwind-devel xz-devel readline-devel ldns-devel expat-devel gtest-devel ccache doxygen graphviz qt5-linguist hidapi-devel libusbx-devel protobuf-devel protobuf-compiler systemd-devel
```


2. Clone submodules and nested submodules
```sh
cd external
git clone --recurse-submodules https://github.com/monero-ecosystem/monero-cpp.git
git clone --recurse-submodules https://github.com/rg3/libbcrypt.git
git clone --recurse-submodules https://github.com/nayuki/QR-Code-generator.git
git clone --recurse-submodules https://github.com/nlohmann/json.git
cd ../
```

3. Modify external/monero-cpp/external/monero-project/CMakeLists.txt:
`option(BUILD_GUI_DEPS "Build GUI dependencies." ON)`

4. Build monero-project twice to create libwallet_merged.a and other .a libraries
```sh
cd external/monero-cpp/external/monero-project && make release-static && make release-static
cd ../../../../
```

5. Build neroshop (along with dokun-ui)
```sh
# Build dokun-ui
cd external/dokun-ui
cmake -G"Unix Makefiles"
make
cd ../../
# Build neroshop
mkdir build && cd build
cmake ..
make
```


## Contributions
I can't do everything on my own with my current knowledge and programming skills, so I will need help with some things. Though I am poor as shit (recently got out of NEETdom so yeah ...), I will open a paid bounty program to encourage individuals who have at least some experience in c++ programming to contribute to my project and help bring this project to life.

This is a one-time pay for each task. Payments will be in XMR.

We can negotiate payment if you feel the bounty price is too low.
```
Details: Implement peer discovery system that broadcasts/multicasts messages to other peers, connects to other peers, stores their IP addresses and shares it with other neroshop nodes.
Bounty Reward: $1000-$3000
Files: src/daemon.cpp; src/server.cpp; src/client.cpp
Libraries/Tools: System network libraries (Linux Sockets, Winsock2, etc.)
Difficulty: Hard
Status: Not Filled


Details: Create an advance search engine using built-in LMDB database. More details will be discussed.
Bounty Reward: $2000-$5000
Files: src/database/lmdb.cpp; src/database/lmdb.hpp;
Libraries/Tools: LMDB
Difficulty: Extreme
Status: Not Filled


Details: Fix font loading issue where loading some fonts cause a crash
Bounty Reward: $100
Files: external/dokun-ui/src/font.cpp; external/dokun-ui/src/font.hpp
Libraries/Tools: FreeType2
Difficulty: Easy-Intermediate
Status: Not Filled


Details: Fix issue where Mouse presses affect all overlapping GUIs when it should ONLY affect the GUI on top
Bounty Reward: $100
Files: 
Libraries/Tools: 
Difficulty: Easy-Intermediate
Status: Not Filled


Details: Improve Input functions (Keyboard input is received/triggered multiple times even with a single key press on X11/Linux); Replace Mouse presses with complete clicks (Mouse press and release)
Bounty Reward: $250
Files: external/dokun-ui/src/mouse.cpp; external/dokun-ui/src/keyboard.cpp; external/dokun-ui/src/window.cpp
Libraries/Tools: 
Difficulty: Intermediate
Status: Not Filled


Details: Fix and optimize graphics and add graphical user interface features (such as scrollbar and scrollarea); Improve current GUI elements (specifically the text edit)
Bounty Reward: $1000-$2000
Files: external/dokun-ui/src/renderer.cpp; external/dokun-ui/include/renderer.hpp; 
Libraries/Tools: OpenGL, Vulkan (optional)
Difficulty: Intermediate-Hard
Status: Not Filled


Details: Fix decoding error in functions using EVP_PKEY_encrypt() and EVP_PKEY_decrypt()
Bounty Reward: $100
Files: src/encryptor.cpp; include/encryptor.hpp
Libraries/Tools: OpenSSL
Difficulty: Easy-Intermediate
Status: Not Filled
```


## Contact
> larteyoh@pm.me

[//]: # (rm -rf external/dokun-ui/CMakeFiles; rm -rf external/dokun-ui/CMakeCache.txt; rm -rf external/dokun-ui/cmake_install.cmake; rm -rf external/dokun-ui/Makefile; rm -rf external/dokun-ui/src/libdokun-ui.a)
[//]: # (git checkout -b feature)
[//]: # (git add CMakeLists.txt external/ include/ readme.md res/neroshop-logo.png res/ss res/tmp_images res/wallets src/ todo.txt .gitignore .gitmodules)
[//]: # (git commit -m"")
[//]: # (git push -u origin feature)
