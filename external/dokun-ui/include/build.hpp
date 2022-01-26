//#include "platform.hpp"

#ifndef BUILD_HPP_DOKUN
#define BUILD_HPP_DOKUN
/////////////
// Default - build default
#ifndef DOKUN_BUILD_EXECUTABLE  // no executable
#ifndef DOKUN_BUILD_LIBRARY     // no library
#ifndef DOKUN_BUILD_MODULE      // no module
#ifndef DOKUN_BUILD_CONSOLE     // no console
    #define DOKUN_BUILD_DEFAULT 
    #define DOKUN_BUILD_CONSOLE // define console (default build)
#endif
#endif
#endif
#endif
/////////////
// Executable - build an executable (.exe, etc.)
#ifdef DOKUN_BUILD_EXECUTABLE
#endif
/////////////
// Library - build a library (.lib or .a)
#ifdef DOKUN_BUILD_LIBRARY 
#endif
/////////////
// Module - build a module (.dll or .so)
#ifdef DOKUN_BUILD_MODULE
#ifdef _WIN32
    #define DOKUN_MODULE __declspec(dllexport)
#endif
#ifdef __gnu_linux__
    #define DOKUN_MODULE __attribute__((visibility("default")))
#endif	
#endif
/////////////
// Console - Build an interpreter (lua.exe)
#ifdef DOKUN_BUILD_CONSOLE
#endif 
/////////////
// Header
#define DOKUN_HEADER \
 "dokun_ui.hpp" // 'include'
#define DOKUN_FHEADER \
 "dokun/dokun_ui.hpp" // folder in 'include'
/////////////
#endif
