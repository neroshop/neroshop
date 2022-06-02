#ifndef SCROLLBAR_HPP_DOKUN
#define SCROLLBAR_HPP_DOKUN

#include "ui.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Scrollbar : public GUI // a bit like a handle and beam
{
public:
	Scrollbar();           static int new_(lua_State *L);
	Scrollbar(int x, int y);
	Scrollbar(int x, int y, int width, int height);
	~Scrollbar();
		
	void draw();    static int draw(lua_State *L);
	void draw(double x, double y);
	void draw(const Vector2& position);
		
	void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0); static int set_color(lua_State *L);// beam color
	void set_color(const Vector3& color);
	void set_color(const Vector3& color, double alpha);
	void set_color(const Vector4& color);
	void set_value(double value);static int set_value(lua_State *L);
	void set_step(double step); // number of items to scroll
	// handle properties
	void set_handle_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_handle_color(lua_State *L);// handle color; 0 = inner, 1 = outer
	void set_handle_inner_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0); static int set_handle_inner_color(lua_State *L);// handle and beam parts of slider
	void set_handle_outer_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0); static int set_handle_outer_color(lua_State *L);// handle and beam parts of slider
	void set_handle_size(int handle_size);
	// scrollbar properties
	void set_radius(double radius); static int set_radius(lua_State *L);// roundness of beam edges [===========]
	// button properties
	void set_button(bool button);
	void set_button_height(int height);
	void set_button_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);
	void set_button_color(const Vector3& color);
	void set_button_color(const Vector3& color, double alpha);
	void set_button_color(const Vector4& color);
	// arrow properties
	void set_arrow(bool arrow);
	void set_arrow_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);
	void set_arrow_color(const Vector3& color);
	void set_arrow_color(const Vector3& color, double alpha);
	void set_arrow_color(const Vector4& color);
	// outline properties
	void set_outline(bool outline); static int set_outline(lua_State *L);
	void set_outline_width(double width); static int set_outline_width(lua_State *L);
	void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0); static int set_outline_color(lua_State *L);
	void set_outline_color(const Vector3& color);// static int set_(lua_State *L);
	void set_outline_color(const Vector3& color, double alpha);// static int set_(lua_State *L);
	void set_outline_color(const Vector4& color);// static int set_(lua_State *L);
	void set_outline_antialiased(bool antialised); static int set_outline_antialiased(lua_State *L);		
	// border properties
	void set_border(bool border);
	// getters
	double get_value()const;static int get_value(lua_State *L);
	Vector2 get_range()const;static int get_range(lua_State *L);
	double get_minimum_value()const;static int get_minimum_value(lua_State *L);
	double get_maximum_value()const;static int get_maximum_value(lua_State *L);
	double get_step()const; // how many steps the scrollbar can take
	Vector4 get_color() const; static int get_color(lua_State *L);
	// handle
	int get_handle_x() const;static int get_handle_x(lua_State *L);
	int get_handle_y() const;static int get_handle_y(lua_State *L);
	int get_handle_width() const; static int get_handle_width(lua_State *L);
	int get_handle_height() const;static int get_handle_height(lua_State *L);
	// boolean
	bool is_moved() const;static int is_moved(lua_State *L); // handle on slider is moved(value changed)
	//static int (lua_State *L);
private:
	// beam
	double value;
	Vector2 range;
	Vector4 color;
	// handle
	double handle_position;
	Vector4 handle_color;
	Vector4 handle_color_inner;
	Vector4 handle_color_outer;
	int handle_size; // height
	double handle_offset; // y
	double handle_radius;
	// button
	bool button;
	Vector4 button_color;
	int button_height;
	// arrow
	bool arrow;
	Vector4 arrow_color;
	// outline
	bool outline;
	Vector4 outline_color;
	double outline_width;
	bool outline_antialiased;
	// gradient
	bool gradient;
	Vector4 gradient_color;		
	// border
	bool border;
};
#endif
#endif
