#ifndef PROGRESSBAR_HPP_DOKUN
#define PROGRESSBAR_HPP_DOKUN

#include "ui.hpp"
#include "label.hpp"

#ifdef __cplusplus
#include <iostream>
#include <lua.hpp>

class Progressbar : public GUI{
public:
	    Progressbar();               static int new_(lua_State *L);
		Progressbar(int x, int y);
		Progressbar(int x, int y, int width, int height);
		~Progressbar();
		
	void draw();  static int draw(lua_State *L);
	void draw(double x, double y);
	void draw(const Vector2& position);
		
		void set_text(const std::string& text);    static int set_text(lua_State *L);
		void set_label(const dokun::Label& label);       static int set_label(lua_State *L);
		void set_foreground_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_foreground_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_foreground_color(lua_State *L);
		void set_foreground_color(const Vector3& color);
		void set_foreground_color(const Vector3& color, double alpha);
		void set_foreground_color(const Vector4& color);
		void set_background_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_background_color(lua_State *L);
		void set_background_color(const Vector3& color);
		void set_background_color(const Vector3& color, double alpha);
		void set_background_color(const Vector4& color);
		void set_range(double min, double max); static int set_range(lua_State *L);
		void set_range(const Vector2& range);
		void set_minimum_value(double min); static int set_minimum_value(lua_State *L);
		void set_maximum_value(double max); static int set_maximum_value(lua_State *L);
		void set_value(double value); static int set_value(lua_State *L); // current value
		void set_direction(int direction); static int set_direction(lua_State *L); // bar fill direction
		void set_text_direction(int direction); static int set_text_direction(lua_State *L);// center, left, or right
		void reset(); static int reset(lua_State *L);// on_value_changed(int value)
		// outline
		void set_outline(bool outline); static int set_outline(lua_State *L);
		void set_outline_width(double width); static int set_outline_width(lua_State *L);
		void set_outline_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_outline_color(lua_State *L);
		void set_outline_color(const Vector3& color);// static int set_(lua_State *L);
		void set_outline_color(const Vector3& color, double alpha);// static int set_(lua_State *L);
		void set_outline_color(const Vector4& color);// static int set_(lua_State *L);
		void set_outline_antialiased(bool antialised); static int set_outline_antialiased(lua_State *L);
		// border - coming soon
		void set_border(bool border);
		void set_radius(double radius);
		// getters
		std::string get_text()const;    static int get_text(lua_State *L);
		dokun::Label * get_label()const;    static int get_label(lua_State *L);
		Vector4 get_color()const;       static int get_color(lua_State *L);
        Vector4 get_foreground_color()const;   static int get_foreground_color(lua_State *L);
        Vector4 get_background_color()const;   static int get_background_color(lua_State *L);
		Vector2 get_range()const;       static int get_range(lua_State *L);
		double get_minimum_value()const;       static int get_minimum_value(lua_State *L);
		double get_maximum_value()const;       static int get_maximum_value(lua_State *L);
		double get_value()const;       static int get_value(lua_State *L);
		int get_direction()const;       static int get_direction(lua_State *L); // fill_direction : 0 = right(default), 1 = left
		int get_text_direction()const;       static int get_text_direction(lua_State *L); // 0 = center(default), left = 1, right = 2
	    double get_radius() const;
	private:
	    double value; 
        Vector2 range;
		unsigned int direction;
		Vector4 foreground_color; // moving bar color (forground)
		Vector4 background_color; // (background)
		// text
		std::unique_ptr<dokun::Label> label;
		unsigned int text_direction;
		// outline
		bool outline;
		double outline_width;
		bool outline_antialiased; // removes staircase effect from line and straightens the line (can lower performance)
		Vector4 outline_color;
		// border
		bool border;
		// radius
		double radius;		
		// gradient
		bool gradient;
		Vector4 gradient_color;
};
#endif
#endif
