#include "../include/toggle.hpp"

Toggle::Toggle() : value (false), foreground_color(255, 255, 255, 1.0), background_color(160, 160, 160, 1.0), on_color(0, 51, 102, 1.0), off_color(background_color/*64, 64, 64, 1.0*/), type("switch"),
    // label
    ////label(nullptr),
    // handle
	// outline (or border)
    outline(false),
	outline_width(1.0),
	outline_color(0, 0, 0, 1.0),
	outline_antialiased(false),
	restore_outline_color(outline_color),
	// radius
	radius(0.0),//(80.0), // an ideal radius for a switch toggle would be 80 %
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
{
    /*// delete label
    if(label) {
        delete label;
        label = nullptr;
    }*/
}
/////////////		
void Toggle::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(!is_visible()) return; // if not visible, exit function
	if(is_active()) {}// is it disabled?
	// mouse is over toggle and pressed, change the value
	on_mouse_press();
    if(is_switch()) {
            background_color = (value == 0) ? off_color : on_color;//Vector4(64, 64, 64, 1.0) : Vector4(0, 51, 102, 1.0);
		    Renderer::draw_switch(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y,
		        foreground_color.x, foreground_color.y, foreground_color.z, foreground_color.w, // handle (foreground - white)
			    (!shader.get()) ? GUI::gui_shader : shader.get(),
			    value, background_color, 
			    outline, outline_width, outline_color, outline_antialiased,
			    radius
			);
	}
    if(is_radio()) {
            //outline_color = (value == 0) ? Vector4(64, 64, 64, 1.0) : restore_outline_color;
		    ////radius = 100.0; // for a perfect circle, w and h should be equal //std::cout << "radio_size: " << get_size() << std::endl;//20, 20
		    Renderer::draw_radio(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y,
		        background_color.x, background_color.y, background_color.z, background_color.w, 
			    (!shader.get()) ? GUI::gui_shader : shader.get(),
			    value, foreground_color, // inner color (dokun_blue)
			    outline, outline_width, outline_color, outline_antialiased,
			    radius
			);
	}
    if(is_checkbox()) {
            background_color = (value == 0) ? off_color : on_color;
            Renderer::draw_checkbox(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, background_color.x, background_color.y, background_color.z, background_color.w,
	            (!shader.get()) ? GUI::gui_shader : shader.get(),
	            value, foreground_color, // value, mark_color (white)
		        outline, outline_width, outline_color, outline_antialiased, radius); // outline
	}
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
	// same foreground color as both checkmark and radio (checkbox)
	// same background color as both checkmark and radio (checkbox)
	// background color changes
	radius = 0.0;//80.0;
}
/////////////
void Toggle::set_radio()
{
	type = "radio";
	set_size(20, 20);
	set_foreground_color(on_color); // inner box // default foreground color
	// same background color as both switch and checkbox
	// foreground color changes	
	// background color stays the same
	radius = 100.0;
}
/////////////
void Toggle::set_checkbox()
{
	type = "check";
	set_size(20, 20);
	// same foreground color as both switch and radio
	// same background color as both switch and radio
	// background color changes
	radius = 0.0;
}
/////////////
void Toggle::set_value(bool value)
{
	//if(value >= 1) {this->value = 1;return;} // an attempt to exceed max_value
	//if(value <= 0) {this->value = 0;return;} // an attempt to go below min_value	
	this->value = value;
}
/////////////
int Toggle::set_value_lua(lua_State *L)
{
	return 0;
}
//////////////
//////////////
void Toggle::set_foreground_color(unsigned int red, unsigned int green, unsigned int blue) {
    foreground_color = Vector4(red, green, blue, foreground_color.w);
}
//////////////
void Toggle::set_foreground_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}
//////////////
void Toggle::set_foreground_color(const Vector3& color)
{
	set_foreground_color(color.x, color.y, color.z);
}
//////////////
void Toggle::set_foreground_color(const Vector3& color, double alpha) {
    set_foreground_color(color.x, color.y, color.z, alpha);
}
//////////////
void Toggle::set_foreground_color(const Vector4& color)
{
	set_foreground_color(color.x, color.y, color.z, color.w);
}
//////////////
//////////////
//////////////
void Toggle::set_background_color(unsigned int red, unsigned int green, unsigned int blue) {
    background_color = Vector4(red, green, blue, background_color.w);
}
void Toggle::set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
/////////////
void Toggle::set_background_color(const Vector3& color)
{
	set_background_color(color.x, color.y, color.z); 
}
/////////////
void Toggle::set_background_color(const Vector3& color, double alpha) {
    set_background_color(color.x, color.y, color.z, alpha);
}
/////////////
void Toggle::set_background_color(const Vector4& color)
{
	set_background_color(color.x, color.y, color.z, color.w); 
}
//////////////
//////////////
//////////////
void Toggle::set_on_color(unsigned int red, unsigned int green, unsigned int blue) {
    on_color = Vector4(red, green, blue, on_color.w);
}
//////////////
void Toggle::set_on_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	on_color = Vector4(red, green, blue, alpha);
}
//////////////
void Toggle::set_on_color(const Vector3& color)
{
	set_on_color(color.x, color.y, color.z);
}
//////////////
void Toggle::set_on_color(const Vector4& color)
{
	set_on_color(color.x, color.y, color.z, color.w);
}
//////////////
//////////////
//////////////
void Toggle::set_off_color(unsigned int red, unsigned int green, unsigned int blue) {
    off_color = Vector4(red, green, blue, off_color.w);
}
//////////////
void Toggle::set_off_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	off_color = Vector4(red, green, blue, alpha);
}
//////////////
void Toggle::set_off_color(const Vector3& color)
{
	set_off_color(color.x, color.y, color.z);
}
//////////////
void Toggle::set_off_color(const Vector4& color)
{
	set_off_color(color.x, color.y, color.z, color.w);
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
//void Toggle::set_outline_color(unsigned int red, unsigned int green, unsigned int blue) {
//    outline_color = Vector4(red, green, blue, outline_color.w);
//    restore_outline_color = outline_color;
//}
void Toggle::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	outline_color = Vector4(red, green, blue, alpha);
	restore_outline_color = outline_color;
}
void Toggle::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z, outline_color.w);
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
void Toggle::set_radius(double radius) {
    this->radius = radius;
}
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
Vector4 Toggle::get_on_color()const
{
	return on_color;
}
Vector4 Toggle::get_off_color()const
{
	return off_color;
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
double Toggle::get_radius() const {
    return radius;
}
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
void Toggle::on_mouse_press() {
	    // mouse is over toggle
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
    if(Mouse::is_over(get_x(), get_y(), get_width(), get_height()) ) {
	    if(Mouse::is_pressed(1) && value == 0) {
			set_value(1);//( (value == 0) ? 1 : 0 );// find a way to detect when the value has changed, then trigger an event when we get a specific value
		#ifdef DOKUN_DEBUG
			std::cout << DOKUN_UI_TAG "Toggle value has changed: \033[0;32m" << value << "\033[0m" << std::endl;
		#endif //trigger_event(eat(), value); // trigger an event based on the value received
		    return; // exit function so it doesn't change the toggle value twice
		}
		if(Mouse::is_pressed(1) && value == 1) {
			set_value(0);
		#ifdef DOKUN_DEBUG
			std::cout << DOKUN_UI_TAG "Toggle value has changed: \033[0;91m" << value << "\033[0m" << std::endl;
		#endif
		    return; // exit function
		}			
    }	
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
