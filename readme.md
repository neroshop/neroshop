# neroshop 

An attempt to create an online marketplace for [Monero](https://getmonero.org/) users, which is likely to fail

[![alt text](res/neroshop-logo.png)](https://github.com/larteyoh/neroshop "neroshop logo")


## Table of contents
- [The history behind neroshop](#about)
- [Features](#features) <!-- - [Documentation](#documentation)-->
- [Building neroshop](#building-neroshop)
  - [Dependencies](#dependencies)
  - [Compiling neroshop from source](#compiling-neroshop-from-source)
  - [Setting up PostgreSQL](#setting-up-postgresql) <!-- - [License](#license)-->
- [Contact information](#contact)


## About
```
This is currently a work in progress. There is nothing to see here ¯\_(ツ)_/¯
```


## Features
* pseudonymous economy (sellers are only identified by their usernames and trusted based on their reputation)
* no registration required (for buyers only)
* no KYC or AML
* no listing fees
* no sales tax (transaction fees will be paid to miners in place of sales tax. I mean, this is crypto LOL)
* no other fees (except for miner transaction fees and shipping costs)
* buy and sell products with monero (and possibly other private-by-default crypto in the near future)
* seller reputation system
* product rating system
* centralized database (for now, since I don't know how to implement a decentralized database or p2p network)
* and much more ...


## Building neroshop

### Dependencies
|      Library                                                       | Minimum Ver.    | Package                | License                            |         Purpose                                                        |
|--------------------------------------------------------------------|-----------------|------------------------|------------------------------------|------------------------------------------------------------------------|
| [monero-cpp](https://github.com/monero-ecosystem/monero-cpp)       | latest          |                        | MIT                                | monero wallet and payment system                                       |
| [libbcrypt](https://github.com/rg3/libbcrypt)                      | ?               |                        | public domain (CC0-1.0)            | password hashing                                                       |
| [sqlite3](https://sqlite.org/)                                     | ?               |                        | public domain                      | database management                                                    |
| [QR Code generator](https://github.com/nayuki/QR-Code-generator)   | ?               |                        | MIT                                | qr code generation                                                     |
| [json](https://github.com/nlohmann/json/)                          | ?               |                        | MIT                                | json parsing                                                           |
| [curl](https://github.com/curl/curl)                               | ?               | `libcurl4-openssl-dev` | curl                               | currency conversion                                                    |
| [openssl](https://github.com/openssl/openssl)                      | 1.1.1           | `libssl-dev`           | OpenSSL-SSLeay or Apache-2.0       | for curl, sha256 sum and message encryption                            |
| [postgresql](https://www.postgresql.org/)                          | ?               | `libpq-dev`            | PostgreSQL                         | server-based database management                                       |
| [dokun-ui](external/dokun-ui)                                      | —               |                        | MIT                                | graphical user interface                                               |


### Compiling neroshop from source
1. Install dependencies

Debian/Ubuntu
```sh
sudo -s -- << EOF
# prerequisites
sudo apt install build-essential cmake git
# neroshop, dokun-ui
sudo apt install libx11-dev libgl1-mesa-dev libglu1-mesa-dev libssl-dev libpq-dev postgresql
# monero-cpp (monero)
sudo apt update && sudo apt install pkg-config libssl-dev libzmq3-dev libunbound-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev libpgm-dev qttools5-dev-tools libhidapi-dev libusb-1.0-0-dev libprotobuf-dev protobuf-compiler libudev-dev libboost-chrono-dev libboost-date-time-dev libboost-filesystem-dev libboost-locale-dev libboost-program-options-dev libboost-regex-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev python3 ccache doxygen graphviz
EOF
```
Arch
```sh
# prerequisites
sudo pacman -Sy --needed base-devel cmake git
# neroshop, dokun-ui
sudo pacman -Sy --needed libx11 lib32-mesa lib32-glu openssl postgresql
# monero-cpp (monero)
sudo pacman -Syu --needed boost openssl zeromq libpgm unbound libsodium libunwind xz readline ldns expat gtest python3 ccache doxygen graphviz qt5-tools hidapi libusb protobuf systemd
```

2. Clone submodules and nested submodules
```sh
cd external
git clone --recurse-submodules https://github.com/monero-ecosystem/monero-cpp.git
git clone --recurse-submodules https://github.com/rg3/libbcrypt.git
git clone --recurse-submodules https://github.com/nayuki/QR-Code-generator.git
git clone --recurse-submodules https://github.com/curl/curl.git
cd ../
```

3. Modify external/monero-cpp/external/monero-project/CMakeLists.txt:
`option(BUILD_GUI_DEPS "Build GUI dependencies." ON)`
<!-- Step 2 is probably not necessary :U -->

4. Build monero-project twice to create libwallet_merged.a and other .a libraries
```sh
cd external/monero-cpp/external/monero-project && make release-static && make release-static
cd ../../../../
```

5. Build libcurl (skip this step if curl is already installed on your system)
```sh
cd external/curl
# build with ./configure (libcurl.a will be installed in both: neroshop/external/curl/lib/.libs/ and /usr/local/lib/)
autoreconf -fi
./configure --with-openssl --disable-shared
make
sudo make install
cd ../../
```

6. Build neroshop (along with dokun-ui)
```sh
# Build dokun-ui
# make sure CMakeCache.txt, cmake_install.cmake, and Makefile have all been deleted if not
cd external/dokun-ui
cmake -G"Unix Makefiles"
make
cd ../../
# Build neroshop
cmake -G"Unix Makefiles" -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/
make
```


### Setting up PostgreSQL
```
# Open the configuration file (change the "14" to your current postgresql version)
sudo gedit /etc/postgresql/14/main/pg_hba.conf

# Edit "pg_hba.conf" (change authentication methods from "peer" to "trust"):
"local   all             postgres                                trust"

# Restart the server for the changes in "pg_hba.conf" to take effect: 
sudo service postgresql restart

# Login to the psql shell as user postgres:
psql user=postgres
    
# Change the password of postgres (within the psql shell)
ALTER USER postgres PASSWORD 'postgres';

# Create a test database
CREATE DATABASE neroshoptest;
    
# Leave the psql shell
\q

# You may restore the "pg_hba.conf" values back to the defaults if you wish
```


## Contact
> larteyoh@pm.me


[//]: # (git add CMakeLists.txt external/ include/ readme.md res/neroshop-logo.png res/wallets src/ todo.txt res/ss res/tmp_images)
[//]: # (git commit -m"empty commit")
[//]: # (git push -u origin main)
