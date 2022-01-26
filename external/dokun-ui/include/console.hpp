#ifndef CONSOLE_HPP_DOKUN
#define CONSOLE_HPP_DOKUN
#include "platform.hpp"
#include "string.hpp"
#include "box.hpp"
#include "edit.hpp"
#include "scrollbar.hpp"

#ifdef __cplusplus
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <lua.hpp>

class Console // static class
{
	public:
	    Console();
		~Console();
		
		static void set_title(std::string title);     static int set_title(lua_State *L);// SetConsoleTitle(TEXT("Win32 Console Control Demo"));
	    static void set_size(int width, int height);  static int set_size(lua_State *L);
	    static void set_position(double x, double y);  static int set_position(lua_State *L);
		static void set_mode(int mode);               static int set_mode(lua_State *L); // terminal mode=0 or script mode=1
		static std::string color(const std::string& text, const std::string& foreground_color, const std::string& background_color, std::string format_attribute = "Default", std::string reset_attribute = "All");		
	    static void draw();                           static int draw(lua_State *L);// draws box with console messages
	    static void init();
	
		static void create();                         static int create(lua_State *L);
		static void destroy();                        static int destroy(lua_State *L); 	
		static void write(const std::string& text);   static int write(lua_State *L);
		static void read();                           static int read(lua_State *L);
		static std::string execute(const std::string& cmd);  static int execute(lua_State *L);
		static std::string code(lua_State *L, const std::string& cmd);  static int code(lua_State *L);
		static void hide_edit();                    static int hide_edit(lua_State *L);
		static void show_edit();                    static int show_edit(lua_State *L);
	private: // 
	    #ifdef DOKUN_WIN32
		    static HANDLE rHnd;
			static HANDLE wHnd;
		#endif // 735, 345
		static std::string on_enter();
		static std::string on_enter_code(lua_State *L);
		static Console * console_ptr; // singleton
		unsigned int mode; // 0=terminal mode (.sh), 1=script mode (.lua)
		bool edit_hidden;
		Box * box;
		Edit * edit;
		Scrollbar * scrollbar;
		std::vector<dokun::Label *> label_list;
		std::vector<std::string> discarded_string_list;
		std::string cache; // last saved string in edit
};
#endif
#endif

/*
Usage:

    std::string colored_text = Console::color("Hello there Sid", "Yellow", "Blue");
    std::cout << colored_text << std::endl;
    
    // place this in loop ...
    Console::draw()

*/
