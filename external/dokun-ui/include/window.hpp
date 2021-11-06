#ifndef _WINDOW
#define _WINDOW

#include "platform.hpp" // specify platform and graphics api
#include "types.hpp"
#include "vector.hpp"
#include "string.hpp" // for unicode support (window title)
//#include "shader.hpp"
#include "factory.hpp" // to store multiple windows
#include "renderer.hpp"
#include "keyboard.hpp"
#include "logger.hpp"
#ifdef __cplusplus
  #include <lua.hpp>
#else
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
#endif

#ifdef __cplusplus
#include <iostream>
// To avoid naming conflictions with X11's "Window" class
#ifndef __gnu_linux__
class Window;
#define WINDOW Window
#endif

#ifdef __gnu_linux__
class XWindow;
#define WINDOW XWindow
#endif

class WINDOW
{
public:
        WINDOW();                             static int window_new(lua_State *L);
		~WINDOW();
        // normal functions
        void create();                            static int create(lua_State *L);  // static int (lua_State *L);
        void create (const std::string& title);    
        void create(int width, int height, int mode = 0);    
        void create(const std::string&, int width, int height, int mode = 0);   
		void create(int x, int y, int left, int top, int mode = 0);
		void create(const std::string&, int x, int y, int left, int top, int mode = 0);	
        void show();                              static int show(lua_State *L); // perfect
        void hide();                              static int hide(lua_State *L); // perfect
		void update();                            static int update(lua_State *L);
        void destroy ();                          static int destroy(lua_State *L);
	    void clear(double red = 0.0, double green = 0.0, double blue = 0.0, double alpha = 1.0, double depth = 1.0, int stencil = 0);   static int clear(lua_State *L);
        void iconify();                           static int iconify(lua_State * L);
		void maximize();                               static int maximize(lua_State * L);
	    void restore();                           static int restore(lua_State * L);
		// NOTE: when using the loop function, you must use window callback functions
		void loop();                              static int loop(lua_State * L); // uses callback functions: dokun.load, dokun.draw, dokun.update, etc.
		// setters
        void set_title(const std::string& title);        static int set_title(lua_State *L);
        void set_width (int width);               static int set_width(lua_State *L);
        void set_height (int height);             static int set_height(lua_State *L);
        void set_size (int width, int height);    static int set_size(lua_State *L);
        void set_mode(int mode);                  static int set_mode(lua_State *L);
	    void set_position(int x, int y);          static int set_position(lua_State *L);
		// appearance and looks
		void set_style(long style);                static int set_style(lua_State *L);
		void set_icon(const std::string& file_name);     static int set_icon(lua_State *L);     
        void set_cursor(int cursor);              static int set_cursor(lua_State *L);	
        void set_cursor(std::string file_name, int width = 0, int height = 0);		
		// vertical synchronization
		void set_vertical_synchronization(bool);  static int set_vertical_synchronization(lua_State *L); // prevents refresh rate from exceeding window's refresh rate
		static void set_viewport(int width, int height); static int set_viewport(lua_State *L);
		// event processing : peek_event or wait_event
        //void set_event(const Event& event);	          static int set_event(lua_State *L); // 0 = peek_msg(returns message, returns nothing if no messages), 1 = get_msg(waits for message) 
		// context creation : for drawing
        void set_context();                       static int set_context(lua_State *L);
        void set_surface();                       static int set_surface(lua_State *L);
		// getters
		std::string get_name();                   static int get_name(lua_State *L);
		int get_id();                             static int get_id(lua_State *L);
        std::string get_title()const;                  static int get_title(lua_State *L);
        int get_width()const;                          static int get_width(lua_State *L);
        int get_height()const;                         static int get_height(lua_State *L);
		Vector2 get_size()const;                       static int get_size(lua_State *L);        // returns window_size; returns size of window including the titlebar (if using win32)
        int get_client_width()const;                   static int get_client_width(lua_State *L);
        int get_client_height()const;                  static int get_client_height(lua_State *L);
		Vector2 get_client_size()const;                static int get_client_size(lua_State *L);        // returns client_size; returns size of window excluding the titlebar (if using win32)		
		
		int get_mode()const;                           static int get_mode(lua_State *L);
		Vector2 get_position()const;                   static int get_position(lua_State *L);
		int get_x()const;                              static int get_x(lua_State *L);
		int get_y()const;                              static int get_y(lua_State *L);
		static WINDOW * get_active();             static int get_active(lua_State *L);
		//Event * get_event()const;                      static int get_event(lua_State *L);
		long get_style()const;                         static int get_style(lua_State *L);
		#ifdef __gnu_linux__
		#ifdef DOKUN_X11
		    Display * get_display()const;             static int get_display(lua_State *L);
		#endif
		#endif
		
		#ifdef __windows__
		#ifndef DOKUN_SDL2
        #ifndef DOKUN_GLFW
		    HINSTANCE get_instance()const;
		    HWND  get_handle ()const;
			HGLRC get_context()const;
		#endif
		#endif
		#endif
		
		#ifdef __gnu_linux__
		#ifdef DOKUN_X11
		#ifndef DOKUN_SDL2
        #ifndef DOKUN_GLFW
		    Window     get_handle ()const;	
		#ifdef DOKUN_OPENGL
			GLXContext get_context()const;
		#endif
		#endif
		#endif
		#endif
		#endif
		// boolean
		bool is_open()const;                           static int is_open(lua_State *L);
		bool is_visible()const;                        static int is_visible(lua_State *L);
		bool is_focused()const;                        static int is_focused(lua_State *L);
		bool is_iconified()const;                      static int is_iconified(lua_State *L);
		bool is_maximized()const;                      static int is_maximized(lua_State *L);
		bool is_window()const;                         static int is_window(lua_State *L);
		bool is_context()const;                        static int is_context(lua_State *L);
		// procedure
		#ifdef __windows__
		    static LRESULT CALLBACK WndProc
			    (HWND handle, UINT mes, WPARAM wParam, LPARAM lParam);
		    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
			DWORD WINAPI MouseLogger(LPVOID lpParm);
			static HHOOK hMouseHook;
		#endif
		// friend
		friend class Mouse;
		friend class Renderer;
		// mouse properties
		static int mouse_button;
		static int mouse_pressed;
		static int mouse_released;
		static int mouse_dragged;
		static int mouse_moved;
		static int mouse_leave;
private:
        std::string title;
        unsigned int width;
        unsigned int height;
        unsigned int mode;
        unsigned int x, y;
        
		//Event * event;
		Vector4 color;
		static std::string name;
        #ifdef __windows__
		    int cursor;
		#endif
		// OpenGL
		#ifdef DOKUN_OPENGL
		    #ifdef __gnu_linux__
		    #ifdef DOKUN_X11
		        GLXContext context;
			    XVisualInfo * visual_info;
			    GLXFBConfig *fbc;
		    #endif
		    #endif
		#endif		
		// Vulkan
        #ifdef DOKUN_VULKAN
		    VkSurfaceKHR surface;
		#endif
        #ifdef DOKUN_GLFW
            GLFWwindow* window;
        #endif
		// WINDOWs (Win32)
        #ifdef __windows__
		    HWND handle;
	        DWORD style;
		    HINSTANCE instance;
			LPCTSTR   lp_cursor_name;
			HDC device;
		#ifndef DOKUN_SDL2
			HGLRC context;
		#endif	
        #endif
		// GNU Linux
		#ifdef __gnu_linux__
		#ifdef DOKUN_X11 // Xlib
		    Display*                       display;
            Window                         window;
			unsigned int style;
			Window root;
			Visual * visual;
			XEvent                  xev; // next_event
			XEvent                  nev; // peek_event
            XEvent                  xevent; // temp
            XSetWindowAttributes    swa;	
            GC gc;		
            Cursor cursor;
		#ifdef DOKUN_XCB
		    xcb_connection_t*             connection;
			xcb_window_t                  window;
		#endif				
		#endif
		#ifdef DOKUN_WAYLAND
		    struct wl_display*                display;
            struct wl_surface*                surface;
            struct wl_shell_surface *         shell_surface;
            struct wl_compositor *compositor = nullptr;
            struct wl_shell *shell = nullptr;
        #ifdef DOKUN_OPENGL
            EGLDisplay egl_display;
        	EGLContext egl_context;
	        struct wl_egl_window *egl_window;
	        EGLSurface egl_surface;
        #endif    
		#endif
		#ifdef DOKUN_MIR
		    MirConnection*                connection;
			MirSurface*                   mirSurface;
		#endif	
		#endif
		// Android
		#ifdef __android__
		    ANativeWindow*                    window;
		#endif
};
#endif /* __cplusplus */

// C implementation (to make code work in c)

#ifdef __cplusplus // if c++
extern "C" {
#endif
	// place c code here
	// example_1
	struct window;
	struct window * window_create();
	void window_destroy(struct window * w);
	// example_2
	/*
	typedef struct {
		const char * title;
		int width;
    	int	height;
        int x, y;
    }        WINDOW;
	
	*/
	
#ifdef __cplusplus
}
#endif
#endif
