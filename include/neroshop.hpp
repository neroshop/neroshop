// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef NEROSHOP_HPP
#define NEROSHOP_HPP
#define APP_NAME      "neroshop"
#define APP_AUTHOR    "larteyoh"
#define APP_LICENSE   "MIT License" 
#define APP_COPYRIGHT "Copyright (C) 2021-present larteyoh@protonmail.com"
#define APP_VERSION_MAJOR "0"
#define APP_VERSION_MINOR "1"
#define APP_VERSION_PATCH "0"
#define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_VERSION_PATCH
// https://web.getmonero.org/legal/#copyright
// the neroshop team are in no way affiliated with the monero team
#include "debug.hpp"
#include "wallet.hpp" // causes error depending on where you place this header
#include "config.hpp"
#include "db.hpp"
#include "qr.hpp"
#include "icon.hpp"
#include "validator.hpp"
#include "converter.hpp"
#include "user.hpp"
#include "buyer.hpp"
#include "seller.hpp"
#include "item.hpp"
#include "cart.hpp"
#include "order.hpp" //#include ".hpp"
#include "message.hpp"
#include "catalog.hpp"
//#include "server.hpp" // for daemon to use
#include "client.hpp"

#endif
