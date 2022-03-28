#ifndef MOUSE_HPP_DOKUN
#define MOUSE_HPP_DOKUN

#include "platform.hpp"// specify the platform
#include "window.hpp"// allows the mouse to move within the window instead of the desktop
#include "image.hpp" // to apply to cursor
extern "C" {
//#include <xdo.h>
}
//#include <X11/extensions/XTest.h>
#define DOKUN_MOUSE_LEFT   1
#define DOKUN_MOUSE_MIDDLE 2 
#define DOKUN_MOUSE_RIGHT  3 
#define DOKUN_MOUSE_WUP    4 
#define DOKUN_MOUSE_WDOWN  5 
#define DOKUN_MOUSE_THUMB  8

#ifdef DOKUN_LINUX
    #define DOKUN_MOUSE_DEVICE_FILE "/dev/input/mice"
#endif

#ifdef __cplusplus
#include <iostream>

class Mouse {
    public:
        Mouse();                                       
        ~Mouse();
		// normal
        static void hide();                                         static int hide(lua_State *L);
	    static void show();                                         static int show(lua_State *L);
        static void restore();                                      static int restore(lua_State *L);
		// setters
        static void set_position(int x, int y);                     static int set_position(lua_State *L);
		static void set_position(int x, int y, const dokun::Window& window); 
		static void set_position(const Vector2& position);
        static void set_position(const Vector2& position, const dokun::Window& window); 
        static void set_cursor(unsigned long cursor);                                   static int set_cursor(lua_State * L);// new july 12 2017!
        static void set_cursor(const Image& cursor);
		static void set_cursor(const std::string& cursor_file, int width=0, int height=0);
		// getters
        static Vector2 get_position();                              static int get_position(lua_State *L);
        static Vector2 get_position(const dokun::Window& window);  /* local position(on window) */		
		static Vector3 get_normalized_position(const dokun::Window& window);
		
		static int get_delta();                                     static int get_delta(lua_State *L); // returns mouse wheel delta
		static Vector3i get_color();                     static int get_color(lua_State *L); // returns rgb color in a position in the active window	
		static Vector2i get_size() /*const*/;                            static int get_size(lua_State * L);
		// boolean
		static bool is_moved();                                     static int is_moved(lua_State *L);
		static bool is_scrolled();                                  static int is_scrolled(lua_State *L);        
		static bool is_pressed(int button, int times_pressed = 1);  static int is_pressed(lua_State *L);              
        static bool is_released(int button);                        static int is_released(lua_State *L);
        static bool is_clicked(int button); // when mouse is both pressed and released
        static bool is_down();                                      static int is_down(lua_State *L);
        static bool is_up();                                        static int is_up(lua_State *L);
		static bool is_over(double x, double y);                             static int is_over(lua_State *L);
        static bool is_over(const Vector2& position);  
    	static bool is_over(const dokun::Window& window);	 
		// 2d object
		static bool is_over(double x, double y, int width, int height);
		static bool is_over(const Vector4& rect);
		static bool is_over(const Vector2& position, const Vector2& size);
		// bonus functions
		static double  rotate(double x, double y);
		static Vector3 rotate(double x, double y, double z);
		#ifdef DOKUN_WIN32
		#endif
		static void get_input(int button, int pressed, int released);
    private:
        static int x; 
        static int y;
}; 
#endif
#endif
