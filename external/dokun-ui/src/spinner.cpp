#include "../include/spinner.hpp"

Spinner::Spinner() : value(0), range(0, 100), step(1), color(160, 160, 160, 255), 
// label
label(nullptr),
// button
button_width(20),
button_color(0, 51, 102, 255)/*(64, 64, 64, 255)*/,
// outline
outline(false),
outline_width(1.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false),
// gradient
gradient(false),
gradient_color(color)
{
	set_position(0, 0);
	set_size(60, 30);
	set_orientation(0);
}
/////////////
Spinner::Spinner(int x, int y)
{
	set_position(x, y);
	set_size(60, 30);
	set_orientation(0);
}
/////////////
Spinner::Spinner(int x, int y, int width, int height)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
}
/////////////
int spinner_new(lua_State *L)
{
    return 1;
}
/////////////
Spinner::~Spinner()
{}
/////////////		
void Spinner::draw()
{
	if(is_visible())
	{
	    on_button_press();
		Renderer::draw_spinner(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, color.x, color.y, color.z, color.w,
		    value, button_width, button_color, outline, outline_width, outline_color, outline_antialiased);
		// create vertical spinner later ... [-][item_qty][+]
		// Label
        if(label) {
            // update label string (value)
            label->set_string(std::to_string((int)value));
            // set label_position relative to slider_position
		    if(label->get_alignment() == "left"  ) {label->set_relative_position(0                                         , 0  + (get_height() - label->get_height()) );}
		    if(label->get_alignment() == "center") {label->set_relative_position(0 + (get_width() - label->get_width()) / 2, 0  + (get_height() - label->get_height()) );}						
		    if(label->get_alignment() == "right" ) {label->set_relative_position(0 + (get_width() - label->get_width())    , 0  + (get_height() - label->get_height()) );}	
            if(label->get_alignment() == "none"  ) {label->set_relative_position(0 + label->get_relative_x()               , 0  + (get_height() - label->get_height()) );} // only change label_relative_y // default
		    label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y());
		}
	}
	on_draw(); // callback for all gui
}
/////////////
void Spinner::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Spinner::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int Spinner::draw(lua_State *L)
{
    return 0;
}
/////////////
void Spinner::set_value(double value)
{
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value	
	this->value = value;
}
/////////////
int Spinner::set_value(lua_State * L)
{
    return 0;
}
/////////////
void Spinner::set_range(double min_val, double max_val)
{
	range = Vector2(min_val, max_val);
}
/////////////
void Spinner::set_range(const Vector2& range)
{
	set_range(range.x, range.y);
}
/////////////
int Spinner::set_range(lua_State * L)
{
    return 0;
}
/////////////
void Spinner::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
/////////////
void Spinner::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
/////////////
void Spinner::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int Spinner::set_color(lua_State * L)
{
    return 0;
}
/////////////
void Spinner::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
	this->label->set_parent(* this); // set parent to Spinner
	this->label->set_string(std::to_string(value));
	//this->label->set_alignment("left");
}
/////////////
int Spinner::set_label(lua_State * L)
{
    return 0;
}
////////////////
double Spinner::get_value() const
{
	return value;
}
/////////////
int Spinner::get_value(lua_State * L)
{
    return 1;
}
/////////////
double Spinner::get_minimum_value() const
{
	return range.x;
}
/////////////
int Spinner::get_minimum_value(lua_State * L)
{
    return 1;
}
/////////////
double Spinner::get_maximum_value() const
{
	return range.y;
}
/////////////
int Spinner::get_maximum_value(lua_State * L)
{
    return 1;
}
/////////////
Vector2 Spinner::get_range() const
{
	return range;
}
/////////////
int Spinner::get_range(lua_State * L)
{
    return 2;
}
/////////////
Vector4 Spinner::get_color() const
{
	return color;
}
/////////////
int Spinner::get_color(lua_State * L)
{
    return 4;
}
/////////////
Label * Spinner::get_label() const
{
	return label;
}
/////////////
int Spinner::get_label(lua_State * L)
{
    return 1;
}
/////////////
int Spinner::get_top_button_x() const
{
	return get_x() + get_width();
}
/////////////
int Spinner::get_top_button_y() const
{
	return get_y();
}
/////////////
int Spinner::get_top_button_width() const
{
	return button_width;
}
/////////////
int Spinner::get_top_button_height() const
{
	return get_height() / 2;
}
/////////////
int Spinner::get_bottom_button_x() const
{
	return get_x() + get_width();
}
/////////////
int Spinner::get_bottom_button_y() const
{
	return get_y() + (get_height() / 2);
}
/////////////
int Spinner::get_bottom_button_width() const
{
	return button_width;
}
/////////////
int Spinner::get_bottom_button_height() const
{
	return get_height() / 2;
}	
/////////////
void Spinner::on_button_press() 
{
	// Mouse
	if(Mouse::is_over(get_top_button_x(), get_top_button_y(), get_top_button_width(), get_top_button_height()))
	{
		if(Mouse::is_pressed(1)) set_value(value + step);
	}
	if(Mouse::is_over(get_bottom_button_x(), get_bottom_button_y(), get_bottom_button_width(), get_bottom_button_height()))
	{
		if(Mouse::is_pressed(1)) set_value(value - step);
	}
}
/////////////
/////////////
/////////////
/////////////
/////////////
