// filename: loader.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef LOADER_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define LOADER_HPP_NEROSHOP

#include <iostream>
#include <script.hpp> // includes both Lua and file.hpp

class Loader {// loads config files (lua), and loads database files (.sql)
public:
    static bool load_config(const std::string& cfgfile);
private:
};
#endif
