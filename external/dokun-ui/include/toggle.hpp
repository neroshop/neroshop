#ifndef TOGGLE_HPP_DOKUN
#define TOGGLE_HPP_DOKUN

#include "ui.hpp"
////#include "label.hpp"

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
		void set_foreground_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_foreground_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
		void set_foreground_color(const Vector3& color);
		void set_foreground_color(const Vector3& color, double alpha);
		void set_foreground_color(const Vector4& color);
		
		void set_background_color(unsigned int red, unsigned int green, unsigned int blue);
        void set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
		void set_background_color(const Vector3& color);
		void set_background_color(const Vector3& color, double alpha);
		void set_background_color(const Vector4& color);
		
		void set_on_color(unsigned int red, unsigned int green, unsigned int blue);		
		void set_on_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
		void set_on_color(const Vector3& color);
		void set_on_color(const Vector3& color, double alpha);
		void set_on_color(const Vector4& color);
		
		void set_off_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_off_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
		void set_off_color(const Vector3& color);
		void set_off_color(const Vector3& color, double alpha);
		void set_off_color(const Vector4& color);			
		// outline
		void set_outline(bool outline);
		void set_outline_width(double width);
		void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
		void set_outline_color(const Vector3& color);
		void set_outline_color(const Vector3& color, double alpha);
		void set_outline_color(const Vector4& color);
		void set_outline_antialiased(bool antialiased);
		// radius
		void set_radius(double radius);
		// getters
		bool get_value()const;                                  static int get_value(lua_State *L);
	    std::string get_type()const;                            static int get_type(lua_State *L);
		Vector4 get_foreground_color()const;
		Vector4 get_on_color()const;
		Vector4 get_off_color()const;
		double get_radius() const;
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
		// callback
		void on_mouse_press();
	private:	
	    std::string type;
	    bool value; // on-off
		Vector4 foreground_color;
		Vector4 background_color;
		Vector4 on_color;
		Vector4 off_color;
		// label
		////dokun::Label * label;
		// handle
		// outline
		bool outline;
		Vector4 outline_color;
		double outline_width;
		bool outline_antialiased;
		Vector4 restore_outline_color;
		// radius
		double radius;
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
