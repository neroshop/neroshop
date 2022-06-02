#include "../include/scrollbar.hpp"

Scrollbar::Scrollbar() : value(0), range(0, 5), color(64, 64, 64, 225), 
// handle
handle_offset(0), // y_position
handle_color(0, 51, 102, 1.0)/*(32, 32, 32, 1.0)*/,
handle_size(40),  // height
handle_radius(50),
// button
button(false),
button_color (96, 96, 96, 1.0),
button_height(20),
// arrow
arrow(false),
arrow_color(32, 32, 32, 1.0),
// outline (or border)
outline (true),
outline_color(0, 0, 0, 1.0),
outline_width(1.0),
outline_antialiased(false),
// gradient
gradient(false),
gradient_color(color),
// border
border(false)
{
	set_position(0, 0);
	set_size(15, 200); // 20, 150
	set_orientation(0);
}
//////////////
Scrollbar::Scrollbar(int x, int y) : Scrollbar()
{
	set_position(x, y);
}
//////////////
Scrollbar::Scrollbar(int x, int y, int width, int height) : Scrollbar()
{
	set_position(x, y);
	set_size(width, height);
}
//////////////
int Scrollbar::new_(lua_State *L)
{
    return 1;
}
//////////////
Scrollbar::~Scrollbar()
{}
//////////////	
void Scrollbar::draw()
{
	on_draw(); // callback for all gui
	if(!is_visible()) return;
	    //double min_val = get_range().x;
	    //double max_val = get_range().y;
		double value = get_value();
	    double x = get_position().x;
		double y = get_position().y;
		double angle = get_angle();
		double scale_x = get_scale().x;
		double scale_y = get_scale().y;
		int width  = get_width();
		int height = get_height();
        int red    = get_color().x;
        int green  = get_color().y;
        int blue   = get_color().z;		
		int alpha  = get_color().w;		
		Renderer::draw_scrollbar(x, y, width, height, angle, scale_x, scale_y, red, green, blue, alpha, (!shader.get()) ? GUI::gui_shader : shader.get(),
            value, range.x, range.y,
			// handle
			handle_offset, handle_size, handle_color,
            // button
			button, button_height, button_color,
            // arrow
			arrow, arrow_color,
            // outline
			outline, outline_width, outline_color, outline_antialiased,
            // border
			border
		);
}
/////////////
void Scrollbar::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Scrollbar::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
//////////////
int Scrollbar::draw(lua_State *L)
{
    return 0;
}
//////////////
//////////////
void Scrollbar::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
}
//////////////
void Scrollbar::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
//////////////
void Scrollbar::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
//////////////
void Scrollbar::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
//////////////
int Scrollbar::set_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	int d = lua_tonumber(L, 5);
	if(d == 0) {dokun::Logger("color.w is 0 ?? | progressbar.cpp (143)\n");d=255;};
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Scrollbar * scrollbar = *static_cast<Scrollbar**>(lua_touserdata(L, -1));
		scrollbar->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), d);
	}
    return 0;	
}
//////////////
void Scrollbar::set_value(double value)
{
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value	
	this->value = value;	
}
//////////////
int Scrollbar::set_value(lua_State *L)
{
    return 0;
}
//////////////
void Scrollbar::set_step(double step)
{
    range.y = step;
} // number of items to scroll
//////////////
// handle properties
void Scrollbar::set_handle_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{}
//////////////
int Scrollbar::set_handle_color(lua_State *L)
{
    return 0;
}
//////////////
//void set_handle_inner_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_handle_inner_color(lua_State *L);// handle and beam parts of slider
//void set_handle_outer_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_handle_outer_color(lua_State *L);// handle and beam parts of slider
void Scrollbar::set_handle_size(int handle_size)
{
    this->handle_size = handle_size; // height
}
//////////////
// scrollbar properties
void Scrollbar::set_radius(double radius)
{}
//////////////
int Scrollbar::set_radius(lua_State *L)
{
    return 0;
}// roundness of beam edges [===========]
//////////////
// button properties
void Scrollbar::set_button(bool button)
{}
//////////////
void Scrollbar::set_button_height(int height)
{}
//////////////
void Scrollbar::set_button_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{}
//////////////
void Scrollbar::set_button_color(const Vector3& color)
{}
//////////////
void Scrollbar::set_button_color(const Vector4& color)
{}
//////////////
// arrow properties
void Scrollbar::set_arrow(bool arrow)
{}
//////////////
void Scrollbar::set_arrow_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{}
//////////////
void Scrollbar::set_arrow_color(const Vector3& color)
{}
//////////////
void Scrollbar::set_arrow_color(const Vector4& color)
{}
//////////////
// outline properties
void Scrollbar::set_outline(bool outline)
{
	this->outline = outline;
}
//////////////
int Scrollbar::set_outline(lua_State *L)
{
    return 0;
}
//////////////
void Scrollbar::set_outline_width(double width)
{
	this->outline_width = width;
}
//////////////
int Scrollbar::set_outline_width(lua_State *L)
{
    return 0;
}
//////////////
void Scrollbar::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	outline_color = Vector4(red, green, blue, alpha);
}
//////////////
int Scrollbar::set_outline_color(lua_State *L)
{
    return 0;
}
//////////////
void Scrollbar::set_outline_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
//////////////
//int set_outline_color(lua_State *L);
void Scrollbar::set_outline_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
//////////////
void Scrollbar::set_outline_antialiased(bool antialised)
{
	this->outline_antialiased = antialised;
}
//////////////
int Scrollbar::set_outline_antialiased(lua_State *L)
{
    return 0;
}		
//////////////
// border properties
void Scrollbar::set_border(bool border)
{
	this->border = border;
}
//////////////
// getters
double Scrollbar::get_value() const
{
	return value;
}
//////////////
int Scrollbar::get_value(lua_State *L)
{
    return 1;
}
//////////////
Vector2 Scrollbar::get_range() const
{
	return range;
}
//////////////
int Scrollbar::get_range(lua_State *L)
{
    return 2;
}
//////////////
double Scrollbar::get_minimum_value() const
{
	return range.x;
}
//////////////
int Scrollbar::get_minimum_value(lua_State *L)
{
    return 1;
}
//////////////
double Scrollbar::get_maximum_value() const
{
	return range.y;
}
//////////////
int Scrollbar::get_maximum_value(lua_State *L)
{
    return 1;
}
//////////////
double Scrollbar::get_step() const
{
    return 10;
} // how many steps the scrollbar can take
//////////////
Vector4 Scrollbar::get_color() const
{
	return color;
}
//////////////
int Scrollbar::get_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Scrollbar * scrollbar = *static_cast<Scrollbar**>(lua_touserdata(L, -1));
		lua_pushnumber(L, scrollbar->get_color().x);
		lua_pushnumber(L, scrollbar->get_color().y);
		lua_pushnumber(L, scrollbar->get_color().z);
		lua_pushnumber(L, scrollbar->get_color().w);
		return 4;
	}		
    lua_pushnil(L);	
	return 4;
}
//////////////
// handle
int Scrollbar::get_handle_x() const
{
    return 0;
}
//////////////
int Scrollbar::get_handle_x(lua_State *L)
{
    return 1;
}
//////////////
int Scrollbar::get_handle_y() const
{
    return 0;
}
//////////////
int Scrollbar::get_handle_y(lua_State *L)
{
    return 1;
}
//////////////
int Scrollbar::get_handle_width() const
{
    return get_width();
}
//////////////
int Scrollbar::get_handle_width(lua_State *L)
{
    return 1;
}
//////////////
int Scrollbar::get_handle_height() const
{
    return handle_size;
}
//////////////
int Scrollbar::get_handle_height(lua_State *L)
{
    return 1;
}
//////////////
// boolean
bool Scrollbar::is_moved() const
{
    return false;
}
//////////////
int Scrollbar::is_moved(lua_State *L)
{
    return 1;
}
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
