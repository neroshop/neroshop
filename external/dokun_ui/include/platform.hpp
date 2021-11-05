#ifndef _PLATFORM
#define _PLATFORM

#ifdef  _WIN32 // for both 32-bit and 64-bit environments
#define __windows__
#define DOKUN_WINDOWS
#define DOKUN_WIN32
#endif
#ifdef _WIN64
#define DOKUN_WIN64
#endif

#ifdef __gnu_linux__
#define __ubuntu__ 
#define DOKUN_LINUX
#endif

#ifdef __APPLE__
#ifdef __MACH__
#define __macosx__
#define DOKUN_MACOSX
#endif
#endif

#ifdef __ANDROID__
#define __android__
#define DOKUN_ANDROID
#endif

#ifdef __gnu_linux__
#ifndef DOKUN_X11 
#ifndef DOKUN_WAYLAND
#ifndef DOKUN_MIR
#ifndef DOKUN_SDL2
#ifndef DOKUN_GLFW
	#define DOKUN_X11
#endif	
#endif	
#endif
#endif
#endif
#endif

#ifdef DOKUN_OPENGL
#define DOKUN_USE_OPENGL
#endif
#ifdef DOKUN_VULKAN
#define DOKUN_USE_VULKAN
#endif
////////////
#if defined(__cplusplus) && (__cplusplus >= 199711L) // 98
#define DOKUN_CPP98_SUPPORTED
#endif
#if defined(__cplusplus) && (__cplusplus >= 201103L) // 11
#define DOKUN_CPP11_SUPPORTED
#endif
#if defined(__cplusplus) && (__cplusplus >= 201402L) // 14
#define DOKUN_CPP14_SUPPORTED
#endif
#if defined(__cplusplus) && (__cplusplus >= 201703L) // 17
#define DOKUN_CPP17_SUPPORTED
#endif
////////////
#ifdef __windows__
    #define __desktop__ // on a desktop
    #if defined(_MSC_VER) && (_MSC_VER >= 1020)
        #pragma warning(disable:4003)
		#pragma warning(disable:4018)
        #pragma warning(disable:4047) 
		#pragma warning(disable:4101) 
		#pragma warning(disable:4133)
		#pragma warning(disable:4244)
        #pragma warning(disable:4267)
        #pragma warning(disable:4273)
		#pragma warning(disable:4305)
		#pragma warning(disable:4311)
        #pragma warning(disable:4312)
        #pragma warning(disable:4390)
        #pragma warning(disable:4700)
        #pragma warning(disable:4715)
        #pragma warning(disable:4724)
		#pragma warning(disable:4800)
		#pragma warning(disable:4996) 
	#endif
	#if defined(__GNUC__) || defined(__GNUG__) // check for both gcc and g++
	    #pragma GCC diagnostic ignored "-Wwrite-strings"
	    #pragma GCC diagnostic ignored "-Wpsabi"
	#endif
	#if defined(__clang__)
	#endif
	#ifdef DOKUN_OPENGL               // use opengl (OPTIONAL), glew (REQUIRED IF USING OPENGL)
		#define GLEW_STATIC
        #include <GL/glew.h>
		#include <GL/wglew.h>
        #include <GL/gl.h>
        #include <GL/glu.h>
	#endif
	#ifdef DOKUN_VULKAN               // use vulkan (OPTIONAL)
	    #define VK_USE_PLATFORM_WIN32_KHR
	    #include <vulkan/vulkan.h>
	#endif
		
	#include "win32_header.hpp"
#endif
////////////
#ifdef __linux__ 
////////////
#ifdef __gnu_linux__              // GNU/Linux
    #define __desktop__           // on a desktop
    #if defined(__GNUC__) || defined(__GNUG__) && !defined(__llvm__) && !defined(__INTEL_COMPILER) // check for both gcc and g++; clang pretends to be gcc so check that compiler is not clang
    #endif
	#if defined(__clang__) //#if defined(__clang_major__ >= 3) && defined(__clang_minor__ >= 3) // clang 3.3 required to use all C++ 11 features
	#endif 
	#ifdef DOKUN_OPENGL           // opengl (OPTIONAL), glew (REQUIRED If using OpenGL and must be included before gl.h and glu.h)
		#define GLEW_STATIC
        #include <GL/glew.h>
	    #ifdef DOKUN_X11
			#include <GL/glxew.h>
		#endif	
		#ifdef DOKUN_WAYLAND
		    #include <GL/eglew.h>    // glew for EGL, #define GLEW_EGL if not defined
		    #include <wayland-egl.h> // Wayland EGL MUST be included before EGL headers
            #include <EGL/egl.h>
		#endif
		#include <GL/gl.h>       				
		#include <GL/glu.h>	
		#ifdef DOKUN_X11
            #include <GL/glx.h>
            #define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
            #define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
            typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
        #endif			
	#endif
		
	#ifdef DOKUN_VULKAN
	    #ifdef DOKUN_X11
	        #define VK_USE_PLATFORM_XLIB_KHR
		#ifdef DOKUN_XCB
			#define VK_USE_PLATFORM_XCB_KHR
		#endif
		#endif
		#ifdef DOKUN_WAYLAND
		    #define VK_USE_PLATFORM_WAYLAND_KHR
		#endif
		#ifdef DOKUN_MIR
		    #define VK_USE_PLATFORM_MIR_KHR
		#endif
		#include <vulkan/vulkan.h>
	#endif
		
	#ifdef DOKUN_X11 
		#include <X11/Xlib.h>       // includes X11/X.h
		#include <X11/Xcms.h>
		#include <X11/Xutil.h>
		#include <X11/Xresource.h>
		#include <X11/Xatom.h>
		#include <X11/cursorfont.h>
		#include <X11/keysym.h>     // includes X11/keysymdef.h;  for keyboard codes
	    //#include <X11/Xlibint.h>    // includes X11/Xproto.h which includes <X11/Xprotostr.h>
        // x keyboard ext : https://www.x.org/releases/X11R7.7/doc/libX11/XKB/xkblib.html#ftn.id2522146
        #include <X11/XKBlib.h> // automatically includes: X11/extensions/XKBstr.h which auto includes X11/extensions/XKB.h, 
        #include <X11/extensions/XKBgeom.h>
	#ifdef DOKUN_XCB	
	    #include <X11/Xlib-xcb.h>
		#include <xcb/xcb.h>
	#endif	
	#endif
	#ifdef DOKUN_WAYLAND
        #include <wayland-client.h>
        #include <wayland-server.h>
        #include <wayland-client-protocol.h>
	#endif
	#ifdef DOKUN_MIR
	#endif
	// glfw
    #ifdef DOKUN_GLFW
        //#define _GLFW_X11
        //#define _GLFW_GLX
        #include <GLFW/glfw3.h>
    #endif			
	#include "linux_header.hpp"
#endif
////////////
#ifdef __android__                 // Android - based on linux kernel
	#define __mobile__             // on a mobile device
    #ifdef DOKUN_VULKAN            // use vulkan (OPTIONAL)
	    #define VK_USE_PLATFORM_ANDROID_KHR
		#include <vulkan/vulkan.h> 
	#endif
	#include "android_header.hpp"
#endif
////////////
#endif
////////////
#ifdef __macosx__               // Mac OSX - I never used a mac before
	#define __desktop__         // on a desktop
	#ifdef DOKUN_OPENGL         // use opengl (OPTIONAL)
	    #include <OpenGL/gl.h>
	    #include <OpenGL/glu.h>
    #endif
	#ifdef __OBJC__
        #import <Cocoa/Cocoa.h> // use cocoa (REQUIRED)
	#endif
#endif
//////////////
#endif
