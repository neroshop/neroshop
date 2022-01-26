#ifndef ENGINE_HPP_DOKUN
#define ENGINE_HPP_DOKUN

/*  Dokun Engine 1.0.0            Copyright (C) 2015-2021 sidpoison (no company or website)
   
    The following rules apply when using this software:
        1. You are free to use, modify, distribute this software and sell the games you make
         with this software without paying any fees or royalties.
        2. You must not misrepresent the origin of this software
         nor claim ownership of this software.
        3. It is not guaranteed that this software works and the creators
         will not take responsibility for any damages or claims in the use of this software.
        4.
*/

#include "build.hpp"
#include "platform.hpp"
#include "types.hpp" 
#include "math.hpp"
#include "vector.hpp"
//#include "matrix_test.hpp"
//#include "quaternion.hpp" // not in using right now
#include "string.hpp"
#include "logger.hpp" 
#include "utility.hpp"
#include "system.hpp"
// containers.
#include "window.hpp"
// graphics.
#include "texture.hpp"
#include "image.hpp"
// input.
#include "mouse.hpp"
#include "keyboard.hpp" //#include "touch.hpp" //  - deprecated
// audio.
//#include "sound.hpp"
//#include "music.hpp"
//#include "voice.hpp"
// font
#include "font.hpp"
// ui
#include "ui.hpp"
#include "box.hpp" 
#include "button.hpp" 
#include "label.hpp"
#include "edit.hpp"
#include "progressbar.hpp"
#include "slider.hpp"
#include "toggle.hpp"
#include "scrollbar.hpp"
#include "list.hpp"
#include "spinner.hpp"
#include "menubar.hpp"
#include "grid.hpp"
#include "combo.hpp"
#include "tab.hpp"
//#include "chat.hpp"//#include "player.hpp" // temp
// network.
//#include "server.hpp"
//#include "client.hpp"
// utilities - the most essential part of the engine
#include "entity.hpp"
#include "component.hpp"
#include "timer.hpp"
#include "shader.hpp"
//#include "resource.hpp"
//#include "db.hpp"
#include "factory.hpp" 
#include "script.hpp"
#include "console.hpp"
//#include "video.hpp"
#include "plugin.hpp"
#include "file.hpp"
//#include "event.hpp"
#include "renderer.hpp"
#include "process.hpp"
#include "browser.hpp"
//#include "player.hpp"
/////////////
#define ENGINE_NAME      "dokun"
#define ENGINE_AUTHOR    "sidtheprince"
#define ENGINE_LICENSE   "MIT License" 
#define ENGINE_COPYRIGHT "Copyright (C) 2015-2021 sidtheprince"
/////////////
#define ENGINE_VERSION_MAJOR "1"
#define ENGINE_VERSION_MINOR "0"
#define ENGINE_VERSION_PATCH "0"
#define ENGINE_VERSION ENGINE_VERSION_MAJOR "." ENGINE_VERSION_MINOR "." ENGINE_VERSION_PATCH

#ifdef __cplusplus
namespace dokun {
class Engine { // The engine initializes the libraries it requires and registers c functions to lua
public:
    Engine(); 
	~Engine();
    // functions
	static bool open();                                           static int open(lua_State *L); // init engine
	static void close();                                          static int close(lua_State *L); // uninitializes engine and closes engine                                                   
	static int test(lua_State *L); // test function : temporary
	static int reg(lua_State *L); // registers the engine's lua module
	// getters
	static Engine    * get_engine(); // returns a singleton object representing the engine
	static bool        get_status(); // returns true if engine is open (initialized), otherwise returns false
private:
    // callbacks
	static bool on_open (); // do stuff on opening the engine
	static void on_close(); // do stuff before closing engine
	// registry (registers functions)
	// singleton
	static dokun::Engine * engine_ptr;
	// status
	static bool status;
	// third_party
	#ifdef DOKUN_VULKAN
	    VkApplicationInfo app_info;
	#endif
	#ifdef DOKUN_WIN32
	    WSADATA wsa_data;
	#endif
};
}
#endif
#endif // rewriting lua : lualib.h (declare engine), linit.c (register engine), engine.cpp(define engine)
