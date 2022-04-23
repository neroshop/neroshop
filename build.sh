# Install dependencies
#```sh
#sudo apt install build-essential cmake pkg-config            # required by both monero and neroshop (min_version: 5, 3.5, any)
# install neroshop (and dokun-ui) dependencies:
sudo apt install libx11-dev libgl1-mesa-dev libglu1-mesa-dev # required by dokun-ui (which neroshop depends on) (min_version: ?, 3.3)
sudo apt install postgresql                                  # required by neroshop (min_version: ?)
# install monero and monero-cpp dependencies (on Debian/Ubuntu):
sudo apt install git libboost-all-dev cmake g++ make libssl-dev libzmq3-dev libhidapi-dev libudev-dev libusb-1.0-0-dev libfox-1.6-dev # copied from https://github.com/monero-ecosystem/monero-cpp#using-this-library-in-your-project
sudo apt update && sudo apt install build-essential cmake pkg-config libssl-dev libzmq3-dev libunbound-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev libpgm-dev qttools5-dev-tools libhidapi-dev libusb-1.0-0-dev libprotobuf-dev protobuf-compiler libudev-dev libboost-chrono-dev libboost-date-time-dev libboost-filesystem-dev libboost-locale-dev libboost-program-options-dev libboost-regex-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev ccache doxygen graphviz # copied from https://github.com/monero-project/monero#dependencies
#```
# Install git (sudo apt install git) then clone this repository (git clone https://github.com/larteyoh/neroshop.git) 
# assuming you have already cloned this repository locally using git ...
cd external
# then clone the dependencies :
git clone --recurse-submodules https://github.com/monero-ecosystem/monero-cpp.git
git clone --recurse-submodules https://github.com/rg3/libbcrypt.git
git clone --recurse-submodules https://github.com/nayuki/QR-Code-generator.git
git clone --recurse-submodules https://github.com/curl/curl.git
#git clone --recurse-submodules https://github.com/nlohmann/json.git
#git clone --recurse-submodules https://github.com/sqlite/sqlite.git
#git clone --recurse-submodules https://github.com/postgres/postgres
#git clone --recurse-submodules https://github.com/?/dokun-ui.git
# update cloned repositories
# git submodule update --init --recursive

# git clone --recurse-submodules --remote-submodules https://github.com/monero-ecosystem/monero-cpp.git
# git clone --recurse-submodules --remote-submodules = clones and updates in one command
# With version 1.9 of Git up until version 2.12 (-j flag only available in version 2.8+)
# --init = initializes the submodule # --recursive = updates not only the submodules, but also updates any nested submodules inside those submodules (and so on) # --recursive and --recurse-submodules are the same
# adding a new submodule to the project:
# git submodule add https://github.com/rg3/libbcrypt.git external/libbcrypt
cd ../
# Building libcurl - https://curl.se/docs/install.html 
cd external/curl
# build with cmake (libcurl.so will be installed in: neroshop/external/curl/lib/)
#cmake -G"Unix Makefiles"
#make
# build with ./configure (libcurl.a will be installed in both: neroshop/external/curl/lib/.libs/ and /usr/local/lib/)
#sudo apt install dh-autoreconf
autoreconf -fi    # or "autoreconf --install" to create configure
./configure --with-openssl
make
sudo make install
cd ../../
# libcurl.a will be installed in both: neroshop/external/curl/lib/.libs/ and /usr/local/lib/

# Building neroshop
#```sh
# modify "external/monero-cpp/external/monero-project/CMakeLists.txt":
# option(BUILD_GUI_DEPS "Build GUI dependencies." ON) # this is probably not necessary at all, I think :O
# build monero-project twice to create libwallet_merged.a and other .a libraries
cd external/monero-cpp/external/monero-project && make release-static -j$(nproc) && make release-static -j$(nproc)
cd ../../../../
# Build dokun-ui (static library)
# make sure CMakeCache.txt, cmake_install.cmake, and Makefile have all been deleted if not
cd external/dokun-ui
cmake -G"Unix Makefiles"
make
cd ../../
# Build neroshop (executable)
cmake -G"Unix Makefiles" -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/
make
#```


#```sh
# To install glxinfo (which is used for checking opengl version):
#sudo apt install mesa-utils
# then use the following command:
#glxinfo | grep 'version' # glxinfo | grep "OpenGL version"
# the actual version is presented by the "core profile version" (4.x)
# Be sure you have OpenGL version 3.3 or higher!
#```
