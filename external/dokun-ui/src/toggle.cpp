#include "../include/toggle.hpp"

Toggle::Toggle() : value (false), foreground_color(255, 255, 255, 255), background_color(255, 255, 255, 255), background_color_on(0, 51, 102, 255), background_color_off(64, 64, 64, 255), type("switch"),
    // handle
	// outline (or border)
    outline(false),
	outline_width(1.0),
	outline_color(0, 0, 0, 255),
	outline_antialiased(false),
    // gradient
    gradient(false),
	gradient_color(background_color) // gradient.color1 = backgound_color, gradient.color0 = foreground_color	
{
	set_position(0, 0);
	set_size(50, 20);
	set_orientation(0);
}
/////////////
Toggle::~Toggle()
{}
/////////////		
void Toggle::draw()
{
	if(is_visible())  // is it visible? 
	{
		if(is_active()) // is it disabled?
		{}
		double value = get_value();
	    double x = get_position().x;
		double y = get_position().y;
		double angle = get_angle();
		double scale_x = get_scale().x;
		double scale_y = get_scale().y;
		int width  = get_width();
		int height = get_height();
        int red    = get_foreground_color().x;
        int green  = get_foreground_color().y;
        int blue   = get_foreground_color().z;		
		int alpha  = get_foreground_color().w;	
	    // mouse is over toggle
	    if(Mouse::is_over(get_rect())) {
			if(Mouse::is_pressed(1)) {
				set_value((get_value() == 0) ? 1 : 0);
			}
		}			
        if(is_switch()) {			
		Renderer::draw_switch(x, y, width, height, angle, scale_x, scale_y,
		    red, green, blue, alpha, 
			value, background_color_on, background_color_off, 
			outline, outline_width, outline_color, outline_antialiased
			);
		}
        if(is_radio()) {
		Renderer::draw_radio(x, y, width, height, angle, scale_x, scale_y,
		    red, green, blue, alpha, 
			value, background_color, 
			outline, outline_width, outline_color, outline_antialiased
			);	
		}
        if(is_checkbox())
		{
		    //Renderer::draw_box(x, y, width, height, angle, scale_x, scale_y,
		    //    red, green, blue, alpha, 0, false, false, 0, Vector4(255, 255, 255, 255), false, false, false, Vector4(255, 255, 255, 255), outline, outline_width, outline_color, outline_antialiased, false, 0, Vector4(255, 255, 255, 255), false, Vector4(255, 255, 255, 255), false);
		}
	}	
    on_draw(); // callback for all gui	
}
/////////////
void Toggle::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Toggle::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int Toggle::draw(lua_State *L)
{
	return 0;
}
/////////////
void Toggle::group(const std::vector<Toggle*>& toggles) {
    int toggles_count = toggles.size();
    /*if(toggles_count == 2) {
        if(toggles[0]->get_value()) {
            toggles[1]->set_value(0);
            return;
        }
        else if(toggles[1]->get_value()) {
            toggles[0]->set_value(0);
            return;
        }
    }
    if(toggles_count == 3) {
        if(toggles[0]->get_value() && !toggles[1]->get_value() && !toggles[2]->get_value()) {
            toggles[1]->set_value(0);
            toggles[2]->set_value(0);
            //return;
        }
        if(toggles[1]->get_value() && !toggles[0]->get_value() && !toggles[2]->get_value()) {
            toggles[0]->set_value(0);
            toggles[2]->set_value(0);
            //return;
        }
        if(toggles[2]->get_value() && !toggles[0]->get_value() && !toggles[1]->get_value()) {
            toggles[0]->set_value(0);
            toggles[1]->set_value(0);
            //return;
        }                
    }    */
}
/////////////
/////////////
// SETTERS
/////////////		  
void Toggle::set_switch()
{
	type = "switch";
	set_size(50, 20);
	set_foreground_color(255, 255, 255, 255);
	set_background_color_on(0, 51, 102, 255); 
	set_background_color_off(64, 64, 64, 255);
}  
/////////////
void Toggle::set_radio()
{
	type = "radio";
	set_size(20, 20);
	set_foreground_color(0, 51, 102, 255);
	set_background_color(255, 255, 255, 255);
	set_outline(true);
	set_outline_color(0, 51, 102, 255);
	set_outline_width(2.0);
}
/////////////
void Toggle::set_checkbox()
{
	type = "check";
	set_size(20, 20);
	set_foreground_color(0, 51, 102, 255);
	set_background_color(255, 255, 255, 255);
    set_outline(true);
	set_outline_color(0, 51, 102, 255);
	set_outline_width(2.0);	
}
/////////////
void Toggle::set_value(bool value)
{
	if(value >= 1) {this->value = 1;return;} // an attempt to exceed max_value
	if(value <= 0) {this->value = 0;return;} // an attempt to go below min_value	
	this->value = value;
}
/////////////
int Toggle::set_value_lua(lua_State *L)
{
	return 0;
}
/////////////
void Toggle::set_foreground_color(int red, int green, int blue, int alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}
void Toggle::set_foreground_color(const Vector3& color)
{
	set_foreground_color(color.x, color.y, color.z);
}
void Toggle::set_foreground_color(const Vector4& color)
{
	set_foreground_color(color.x, color.y, color.z, color.w);
}
//////////////
void Toggle::set_background_color_on(int red, int green, int blue, int alpha)
{
	background_color_on = Vector4(red, green, blue, alpha);
}
void Toggle::set_background_color_on(const Vector3& color)
{
	set_background_color_on(color.x, color.y, color.z);
}
void Toggle::set_background_color_on(const Vector4& color)
{
	set_background_color_on(color.x, color.y, color.z, color.w);
}
/////////////
void Toggle::set_background_color_off(int red, int green, int blue, int alpha)
{
	background_color_off = Vector4(red, green, blue, alpha);
}
void Toggle::set_background_color_off(const Vector3& color)
{
	set_background_color_off(color.x, color.y, color.z);
}
void Toggle::set_background_color_off(const Vector4& color)
{
	set_background_color_off(color.x, color.y, color.z, color.w);
}
/////////////
void Toggle::set_background_color(int red, int green, int blue, int alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
/////////////
void Toggle::set_background_color(const Vector3& color)
{
	set_background_color(color.x, color.y, color.z); 
}
/////////////
void Toggle::set_background_color(const Vector4& color)
{
	set_background_color(color.x, color.y, color.z, color.w); 
}
/////////////
/////////////
void Toggle::set_outline(bool outline)
{
	this->outline = outline;
}
void Toggle::set_outline_width(double width)
{
	outline_width = width;
}
void Toggle::set_outline_color(int red, int green, int blue, int alpha)
{
	outline_color = Vector4(red, green, blue, alpha);
}
void Toggle::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z);
}
void Toggle::set_outline_color(const Vector4& color)
{
	set_outline_color(color.x, color.y, color.z, color.w);
}
void Toggle::set_outline_antialiased(bool antialiased)
{
	outline_antialiased = antialiased;
}
/////////////
/////////////
/////////////
/////////////
// GETTERS
/////////////
std::string Toggle::get_type()const // 0 = on-off switch, 1 = radio, 2 = check box
{
	return type;
}             
/////////////
int Toggle::get_type(lua_State *L)
{
	return 0;
}
/////////////
bool Toggle::get_value()const
{
	return value;
}         
/////////////    
int Toggle::get_value(lua_State *L)
{
	return 0;
}
/////////////
Vector4 Toggle::get_foreground_color()const
{
	return foreground_color;
}
Vector4 Toggle::get_background_color_on()const
{
	return background_color_on;
}
Vector4 Toggle::get_background_color_off()const
{
	return background_color_off;
}
/////////////
double Toggle::get_handle_x()const
{
    if(is_switch())
    {
	    if(value == 0) // if off
	        return get_x(); // same x_position as switch
	    if(value == 1) // if on
		    return get_x() + (get_width() / 2); // moved to the other half of the switch
	}	    
    return get_x();
}
double Toggle::get_handle_y()const
{
	return get_y() + 1; // 1 is the top_padding
}
int Toggle::get_handle_width()const
{
	return (get_width() / 2); // handle is half of the switch's width
}
int Toggle::get_handle_height()const
{
	return get_height() - 2; // 2 is the bottom padding
}
Vector4 Toggle::get_handle_color()const
{
	return foreground_color;
}
/////////////
/////////////
/////////////
// BOOLEAN
/////////////
bool Toggle::is_checkbox() const
{
	return (String::lower(get_type()) == "check_box" || String::lower(get_type()) == "checkbox" || String::lower(get_type()) == "check");
}
/////////////
bool Toggle::is_radio() const
{
	return (String::lower(get_type()) == "radio");
}
/////////////
bool Toggle::is_switch() const
{
    return (String::lower(get_type()) == "switch");	
}
/////////////		
int Toggle::new_(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
