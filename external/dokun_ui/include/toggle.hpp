#ifndef _TOGGLE
#define _TOGGLE

#include "ui.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Toggle : public GUI { // includes check_box, radio, and switch
    public:
	    Toggle();                                          static int new_(lua_State *L);
		~Toggle();
		// functions
		void draw();                               static int draw(lua_State *L); /* can be overriden */
		void draw(double x, double y);
		void draw(const Vector2& position);
		static void group(const std::vector<Toggle*>& toggles); // put a bunch of toggles in the same group // added 2021-11-05
		// setters
		void set_value(bool value);                        static int set_value_lua(lua_State *L);
		void set_foreground_color(int red, int green, int blue, int alpha=255);
		void set_foreground_color(const Vector3& color);
		void set_foreground_color(const Vector4& color);
        void set_background_color(int red, int green, int blue, int alpha=255);
		void set_background_color(const Vector3& color);
		void set_background_color(const Vector4& color);		
		void set_background_color_on(int red, int green, int blue, int alpha=255);
		void set_background_color_on(const Vector3& color);
		void set_background_color_on(const Vector4& color);
		void set_background_color_off(int red, int green, int blue, int alpha=255);
		void set_background_color_off(const Vector3& color);
		void set_background_color_off(const Vector4& color);			
		// outline
		void set_outline(bool outline);
		void set_outline_width(double width);
		void set_outline_color(int red, int green, int blue, int alpha=255);
		void set_outline_color(const Vector3& color);
		void set_outline_color(const Vector4& color);
		void set_outline_antialiased(bool antialiased);
		// getters
		bool get_value()const;                                  static int get_value(lua_State *L);
	    std::string get_type()const;                            static int get_type(lua_State *L);
		Vector4 get_foreground_color()const;
		Vector4 get_background_color_on()const;
		Vector4 get_background_color_off()const;
		// handle
		double get_handle_x()const;
		double get_handle_y()const;
		int get_handle_width()const;
		int get_handle_height()const;
		Vector4 get_handle_color()const;
		// boolean
		bool is_checkbox() const;                                   static int is_check(lua_State *L);
		bool is_radio() const;                                   static int is_radio(lua_State *L);
		bool is_switch() const;                                  static int is_switch(lua_State *L);
	    void set_switch();
		void set_radio();
		void set_checkbox();
	private:	
	    std::string type;
	    bool value; // on-off
		Vector4 foreground_color;
		Vector4 background_color;
		Vector4 background_color_on;
		Vector4 background_color_off;
		// handle
		// outline
		bool outline;
		Vector4 outline_color;
		double outline_width;
		bool outline_antialiased;
		// gradient
		bool gradient;
		Vector4 gradient_color;		
};
/////////
/*
if(Keyboard::is_pressed(KEY_LEFT))
{
	toggle->set_value(0);
}	
if(Keyboard::is_pressed(KEY_RIGHT))
{
	toggle->set_value(1);
}	
*/
#endif
/////////
/*
switch:
[=[]]
check box:
[✔]
radio:
(O)

USAGE:
    Toggle * toggle = new Toggle();
	toggle->set_position(10, 500);
*/
#endif
