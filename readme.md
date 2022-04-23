# neroshop 

An attempt to create an online marketplace for [Monero](https://getmonero.org/) users, which is likely to fail

[![alt text](res/neroshop-logo.png)](https://github.com/larteyoh/neroshop "neroshop logo")


## Table of contents
- [The history behind neroshop](#about)
- [Features](#features) <!-- - [Documentation](#documentation)-->
- [Building neroshop](#building-neroshop)
  - [Dependencies](#dependencies)
  - [Compiling neroshop from source](#compiling-neroshop-from-source) <!-- - [Setting up PostgreSQL](#setting-up-postgresql)--> <!-- - [License](#license)-->
- [Contact information](#contact)


## About
See <a href="https://github.com/larteyoh/neroshop/blob/main/about.txt">about.txt</a>.
```
...
```


## Features
* pseudonymous economy (sellers are only identified by their usernames and trusted based on their reputation)
* no registration required (for buyers only)
* no KYC or AML
* no listing fees
* no sales tax (transaction fees will be paid to miners in place of sales tax. I mean, this is crypto LOL)
* no other fees (except for miner transaction fees and shipping costs)
* buy and sell products with monero (and possibly other private-by-default crypto in the near future)
* automatic subaddress generator (a unique monero address is generated from seller's account each time a buyer places an order)
* address watcher that tracks incoming txs and notifies both the user and seller
("awaiting payment ..."[red], "payment incoming"[yellow], "payment received!"[green])
* seller reputation system (score rating ranging from 0-1)
* product rating system (star rating ranging from 1-5)
* centralized database (for now, since I don't know how to implement a decentralized database or p2p network)
* and much more ...


## Building neroshop

### Dependencies
|      Library                                                       | Minimum Ver.    | Package                | License                            |         Purpose                                                        |
|--------------------------------------------------------------------|-----------------|------------------------|------------------------------------|------------------------------------------------------------------------|
| [monero-cpp](https://github.com/monero-ecosystem/monero-cpp)       | latest          |                        | MIT                                | monero wallet and payment system                                       |
| [libbcrypt](https://github.com/rg3/libbcrypt)                      | ?               |                        | public domain (CC0-1.0)            | password hashing                                                       |
| [sqlite3](https://sqlite.org/)                                     | ?               |                        | public domain (Unlicense)          | database management                                                    |
| [QR Code generator](https://github.com/nayuki/QR-Code-generator)   | ?               |                        | MIT                                | qr code generation                                                     |
| [json](https://github.com/nlohmann/json/)                          | ?               |                        | MIT                                | json parsing (used in conjunction with libcurl)                        |
| [curl](https://github.com/curl/curl)                               | ?               | `libcurl4-openssl-dev` | curl (inspired by MIT)             | multiprotocol file transfer (used to retrieve currency exchange rates) |
| [openssl](https://github.com/openssl/openssl)                      | 1.1.1           | `libssl-dev`           | OpenSSL-SSLeay or Apache-2.0       | required by curl for http over ssl (https) connections; email hashing (SHA-2) and message encryption (RSA) |
| [postgresql](https://www.postgresql.org/)                          | ?               | `postgresql`           | PostgreSQL (similar to BSD or MIT) | client-server database management                                      |
| [dokun-ui](external/dokun-ui)                                      | ?               |                        | MIT                                | graphical user interface                                               |


### Compiling neroshop from source
You can either choose to build neroshop by running `./build.sh` or by following the steps below.

0. Install dependencies
```sh
sudo -s -- << EOF
# neroshop, dokun-ui
sudo apt install libx11-dev libgl1-mesa-dev libglu1-mesa-dev
sudo apt install postgresql
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
git clone --recurse-submodules https://github.com/curl/curl.git
cd ../
```

2. Modify "external/monero-cpp/external/monero-project/CMakeLists.txt":
`option(BUILD_GUI_DEPS "Build GUI dependencies." ON)`
<!-- Step 2 is probably not necessary :U -->

3. Build monero-project twice to create libwallet_merged.a and other .a libraries
```sh
cd external/monero-cpp/external/monero-project && make release-static -j$(nproc) && make release-static -j$(nproc)
cd ../../../../
```

4. Build libcurl
```sh
sudo -s -- << EOF
cd external/curl
# build with ./configure (libcurl.a will be installed in both: neroshop/external/curl/lib/.libs/ and /usr/local/lib/)
autoreconf -fi
./configure --with-openssl
make
sudo make install
cd ../../
EOF
```

5. Build neroshop (along with dokun-ui)
```sh
sudo -s -- << EOF
# Build dokun-ui
# make sure CMakeCache.txt, cmake_install.cmake, and Makefile have all been deleted if not
cd external/dokun-ui
cmake -G"Unix Makefiles"
make
cd ../../
# Build neroshop
cmake -G"Unix Makefiles" -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/
make
EOF
```


## Contact
> larteyoh@pm.me


[//]: # (git add build.sh CMakeLists.txt external/ include/ readme.md res/neroshop-logo.png res/wallets src/ todo.txt res/ss res/tmp_images about.txt)
[//]: # (git commit -m"empty commit")
[//]: # (git push -u origin main)
