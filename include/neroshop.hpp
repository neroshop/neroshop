// filename: neroshop.hpp
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

#include "debug.hpp"
#include "wallet.hpp" // causes error depending on where you place this header
#include "config.hpp"
#include "database.hpp"
#include "qr.hpp"
#include "icon.hpp"
#include "validator.hpp"
#include "converter.hpp"
#include "user.hpp"
#include "buyer.hpp"
#include "seller.hpp"
#include "item.hpp"
#include "cart.hpp"
#include "order.hpp"
#include "message.hpp"
#include "catalog.hpp"
//#include "server.hpp" // for neroshop daemon to use
#include "client.hpp"
#include "encryptor.hpp"
//#include "carrier.hpp" // not currently in use
#include <cassert> //all of these are for the "switch replacement" below
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#endif

template<typename Key_t>
void switch2(const Key_t& key,
    const std::vector<std::pair<Key_t, std::function<void()>>>& pairs,
    const std::function<void()>& def)
{
    std::unordered_map<Key_t, std::function<void()>> dict;
    for (const auto& entry : pairs)
        dict.insert(entry);
    assert(dict.size() == pairs.size());
    const auto it = dict.find(key);
    if (it != dict.end())
        it->second();
    else
        def();
}

template<typename F, typename... Args>
std::function<void()> defer(F f, Args ... args)
{
    return [f, args...]()
    {
        f(args...);
    };
}

void say(const std::string& text)
{
    std::cout << text << std::endl;
}

void quit(const std::string& msg, bool* running_flag)
{
    if (running_flag)
        *running_flag = false;
    say(msg);
}
