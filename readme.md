# neroshop 

_A peer-to-peer marketplace for [monero](https://getmonero.org) users_

This project is work-in-progress. Contact us on
[matrix](https://matrix.to/#/#neroshop:matrix.org).

## Planned Features

* Decentralized distributed peer-to-peer network
* Buy and sell products with monero
* Anonymous payments
* No censorship
* Pseudonymous identities (sellers and buyers are identified by their unique ids)
* Send funds directly to a seller or use a multi-signature escrow
* End-to-end-encrypted messages between sellers and buyers
* Optional Tor integration for added privacy
* No KYC
* No buyer registration for browsing
* No listing fees, sales tax, or any other fees
* The only fees are the very low monero transaction fees and shipping costs
* Reputation system
* Product rating system
* Wishlists
* Built-in monero node with option to connect to remote nodes


## Building neroshop

```sh
# Install system-wide prerequisites on Debian/Ubuntu Linux
sudo apt-get install git cmake g++ pkg-config libssl-dev libminiupnpc-dev libboost-chrono-dev libboost-date-time-dev libboost-filesystem-dev libboost-locale-dev libboost-program-options-dev libboost-regex-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev libzmq3-dev libhidapi-dev libprotobuf-dev libusb-dev libglu1-mesa-dev liblua5.3-dev libfreetype-dev libcurl4-openssl-dev libjpeg-dev libgif-dev libtiff-dev libglm-dev libxapian-dev libeasyloggingpp-dev
# Clone neroshop
git clone https://codeberg.org/neroshop/neroshop.git && cd neroshop
# Build external libraries
mkdir -p external/build && cd external/build && cmake .. && make && cd -
# Build neroshop
mkdir build && cd build && cmake .. && make && cd -
# Test neroshop (optional)
cd build && ctest
```

## Running neroshop
```sh
# Run daemon
cd neroshop/build
./neromon
# Run command-line client (in another terminal)
cd neroshop/build
./neroshop-cli
```

## Contributing

Contact us on [matrix](https://matrix.to/#/#neroshop:matrix.org).
