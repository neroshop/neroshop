# neroshop 

An online marketplace for [Monero](https://getmonero.org/) users ("nero" meaning black + shop)
[![alt text](res/neroshop-logo.png)](https://github.com/larteyoh/neroshop "neroshop logo")

# names attempts:
Monazon  (placeholder name)
Monashop (placeholder name)
Neroshop (placeholder name)("nero" meaning black + shop) <3
Neronero (placeholder name)

language: `c++`

# dependencies:
monero     : (MIT)
monero-cpp : https://github.com/monero-ecosystem/monero-cpp (MIT)
bcrypt     : https://github.com/libbcrypt/ | https://www.openwall.com/crypt/ (public domain)
sqlite3    : https://www.sqlite.org/ (public domain)
QR Code generator: https://github.com/nayuki/QR-Code-generator (MIT)
json       : https://github.com/nlohmann/json/ (MIT)
curl       : https://curl.se/libcurl/ (The curl license - inspired by MIT/X)


# features:
* buyers are not required to register
* none of that KYC or AML bull----
* buy and sell products with monero (and other "private by default" crypto)
* generate a subaddress per customer => automatic subaddress generator (a unique monero address is generated from seller's account each time a buyer orders an item)
* address watcher that tracks incoming txs and notifies the both the user and seller
("awaiting payment ..."[red], "payment incoming"[yellow], "payment received!"[green])
* seller reputation system (score from 0-1)
* product rating system (stars from 1-5)
* and more ...

# Building neroshop
```sh
# install neroshop dependencies:
sudo apt install build-essential
sudo apt-get install cmake
sudo apt-get install libx11-dev
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libcurl4-openssl-dev
# install monero dependencies:
sudo apt install git libboost-all-dev cmake g++ make libssl-dev libzmq3-dev libhidapi-dev libudev-dev libusb-1.0-0-dev libfox-1.6-dev
# to build under a different version of monero, follow the instructions here: https://github.com/monero-ecosystem/monero-cpp#using-this-library-in-your-project

cmake -G"Unix Makefiles"
make
```

NOTE:
I am an awful programmer and still consider myself a noob at it since I don't know what I'm doing half of the time, so if there's any out there who can help me bring
my project to life, I'd highly appreciate it! 
I am also willing to compensate any contributors with Monero when I become financially stable.
