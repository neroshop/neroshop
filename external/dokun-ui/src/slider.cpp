#include "../include/slider.hpp"

Slider::Slider() : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 1.0), background_color(160, 160, 160, 1.0),
    ball_size(10), ball_color(64, 64, 64, 0.9), ball_radius(50), ball_locked(false), label(nullptr),
	// outline
	outline (false),
    outline_color(0, 0, 0, 1.0),
    outline_width(1.0),
    outline_antialiased(false),
    // gradient
    gradient(false),
	gradient_color(background_color) // gradient.color1 = backgound_color, gradient.color0 = foreground_color
{
	set_position(0, 0);
	set_size(200, 20);
	set_orientation(0);
}
/////////////            
Slider::Slider(int x, int y) : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 1.0), background_color(160, 160, 160, 1.0),
    ball_size(10), ball_color(64, 64, 64, 0.9), ball_radius(50), label(nullptr),
	// outline
	outline (false),
    outline_color(0, 0, 0, 1.0),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(x, y);
	set_size(200, 20);
	set_orientation(0);
}
/////////////
Slider::Slider(int x, int y, int width, int height) : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 1.0), background_color(160, 160, 160, 1.0),
    ball_size(10), ball_color(64, 64, 64, 0.9), ball_radius(50), label(nullptr),
	// outline
	outline (false),
    outline_color(0, 0, 0, 1.0),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
}
/////////////
Slider::~Slider()
{}
/////////////		
void Slider::draw()
{
	if(!is_visible()) return;  // is it visible? //if(!is_active())
	    double min_val = get_range().x;
	    double max_val = get_range().y;
		double angle = get_angle();
		//on_keyboard_test();//temp
		on_mouse_move_ball();
		// Draw slider				
		if(get_orientation() == 0) { // horizontal
		    int ball_width = ball_size;
		    Renderer::draw_slider(get_x(), get_y(), get_width(), get_height(), 0, get_scale().x, get_scale().y, foreground_color.x, foreground_color.y, foreground_color.z, foreground_color.w,
			// beam properties
			    min_val, max_val, value, background_color,
			// ball properties
			    ball_width, ball_color
			// outline
			    );
		}
		if(get_orientation() != 0) { // vertical 
		    int ball_height = ball_size;
		    Renderer::draw_slider_vertical(get_x(), get_y(), get_width(), get_height(), 0, get_scale().x, get_scale().y, foreground_color.x, foreground_color.y, foreground_color.z, foreground_color.w,
			    min_val, max_val, value, background_color, ball_height, ball_color);
		}	
	// Label	
	if(!label) return;// as long as label is not nullptr. Does not matter if text is empty, still need to set proper x and y positions
    // set label_position relative to progressbar_position
    if(label->get_alignment() == "left"  ) label->set_relative_position(0, (get_height() / 2) - (10 / 2)); // left will remain 0, y will be centered
	if(label->get_alignment() == "center") label->set_relative_position((get_width() / 2) - ((10 * label->get_string().length()) / 2), (get_height() / 2) - (10 / 2)); // both x and y will be centered; adjusted based on label string's length
	if(label->get_alignment() == "right" ) label->set_relative_position(get_width() - (10 * label->get_string().length()), (get_height() / 2) - (10 / 2)); // right will be move to the far right (width), y will be centered; adjusted based on label string's length
	if(label->get_alignment() == "none"  ) label->set_relative_position(label->get_relative_x(), label->get_relative_y()); // nothing is changed here
	label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y());
    // NO need to draw label since child GUI are automatically drawn //label->draw();//Renderer::draw_label(get_text(), x, y, angle, 0.5, 0.5, get_label()->get_font()->get_data(), get_label()->get_color().x, get_label()->get_color().y, get_label()->get_color().z, get_label()->get_color().w);
    on_draw(); // callback for all gui	
}    
/////////////
void Slider::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Slider::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
///////////// 
int Slider::draw(lua_State *L)
{
	return 0;
}
/////////////
void Slider::reset()
{
    value = get_minimum_value();//0;
}
/////////////
int Slider::reset(lua_State *L)
{
	return 0;
}
/////////////
/////////////
// SETTERS
/////////////		
void Slider::set_foreground_color(unsigned int red, unsigned int green, unsigned int blue) {
	foreground_color = Vector4(red, green, blue, foreground_color.w);
}
void Slider::set_foreground_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}  
void Slider::set_foreground_color(const Vector3& color) {
    set_foreground_color(color.x, color.y, color.z);
}
void Slider::set_foreground_color(const Vector4& color) {
    set_foreground_color(color.x, color.y, color.z, color.w);
}
/////////////
int Slider::set_foreground_color(lua_State *L) {
	return 0;
}
/////////////	
void Slider::set_background_color(unsigned int red, unsigned int green, unsigned int blue) {
	background_color = Vector4(red, green, blue, background_color.w);
}	
void Slider::set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
void Slider::set_background_color(const Vector3& color) {
    set_background_color(color.x, color.y, color.z);
}
void Slider::set_background_color(const Vector4& color) {
    set_background_color(color.x, color.y, color.z, color.w);
}
/////////////
int set_foreground_color(lua_State *L) {
    return 0;
}
/////////////
void Slider::set_range(double max, double min)
{
	range = Vector2(min, max);
} 
/////////////
int Slider::set_range(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_minimum_value(double minimum_value)
{
	range.x = minimum_value;
} 
/////////////
int Slider::set_minimum_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_maximum_value(double maximum_value)
{
    range.y = maximum_value;
} 
/////////////
int Slider::set_maximum_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_value(double value)
{
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value	
	this->value = value;
} 
/////////////
int Slider::set_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_ball_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    ball_color = Vector4(red, green, blue, alpha); // outer
    //ball_color_alt = Vector4(red, green, blue, alpha); // inner
}
/////////////
int Slider::set_ball_color(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_ball_inner_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    //ball_color_alt = Vector4(red, green, blue, alpha); // inner
}
/////////////
int Slider::set_ball_inner_color(lua_State *L)
{
	return 0;
}// ball and beam parts of slider
/////////////
void Slider::set_ball_outer_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    ball_color = Vector4(red, green, blue, alpha); // outer
}
/////////////
int Slider::set_ball_outer_color(lua_State *L)
{
	return 0;
}// ball and beam parts of slider
/////////////
void Slider::set_ball_size(int ball_size)
{
	this->ball_size = ball_size;
}
/////////////
void Slider::set_radius(double radius)
{
	//(this)->radius = radius;
}  
/////////////
int Slider::set_radius(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_label(const dokun::Label& label)
{
    this->label = &const_cast<dokun::Label&>(label);
    this->label->set_parent(* this);
}
/////////////
int Slider::set_label(lua_State *L)
{
    return 0;
}
/////////////
/////////////
// GETTERS
/////////////
double Slider::get_value() const
{
	return value;
} 
/////////////
int Slider::get_value(lua_State *L)
{
	return 1;
}
/////////////
Vector2 Slider::get_range() const
{
	return range;
} 
/////////////
int Slider::get_range(lua_State *L)
{
	return 2;
}
/////////////
double Slider::get_minimum_value() const
{
	return range.x;
} 
/////////////
int Slider::get_minimum_value(lua_State *L)
{
	return 1;
}
/////////////
double Slider::get_maximum_value() const
{
	return range.y;
} 
/////////////
int Slider::get_maximum_value(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Slider::get_color() const
{
	return foreground_color;
}
int Slider::get_color(lua_State *L)
{
	return 1;
}
/////////////
dokun::Label * Slider::get_label() const
{
    return label;
}
/////////////
int Slider::get_label(lua_State *L)
{
    return 1;
}
/////////////
/////////////
/////////////
/////////////
int Slider::get_ball_x()
{
    if(get_orientation() != 0) return get_x() - (get_width() - get_width() / 2) + 0; // vertical
	return (get_x() + (get_value() / get_maximum_value()) * static_cast<float>(get_width())) - ball_size; // horizontal (default)
}
int Slider::get_ball_x(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_y() // should return the ball's actual (real) y_position // y=-5, h=1.5  or  y=-10, h=2
{
    if(get_orientation() != 0) return get_y() + (get_value() / get_maximum_value()) * static_cast<float>(get_height()); // vertical
	return get_y() - (get_height() - get_height() / 2) + 0;//get_y() + 1; // horizontal (default)
}
int Slider::get_ball_y(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_width()
{
    if(get_orientation() != 0) return (get_width() * 2) - 0; // vertical
	return ball_size; // horizontal (default)
}
int Slider::get_ball_width(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_height() // ball_height is half of double the beam's height (height x 1.5) minus the bottom_padding (2) // y=-5, h=1.5  or  y=-10, h=2
{
    if(get_orientation() != 0) return ball_size; // vertical
	return (get_height() * 2) - 0;//get_height() - 2; // horizontal (default)
}
int Slider::get_ball_height(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Slider::get_ball_color(int layer) const { //0=outer=default, 1=inner
    return ball_color;
}
/////////////
/////////////
/////////////
/////////////
// BOOLEAN
/////////////		
bool Slider::is_moved()
{
	return false;
} 
int Slider::is_moved(lua_State *L)
{
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/////////////
void Slider::on_keyboard_test() {
    if(!is_focused()) return;
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_LEFT)) 
        set_value(value - 1);
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_RIGHT)) 
        set_value(value + 1);
#ifdef DOKUN_DEBUG0
    std::cout << "value changed: " << value << " (ball_x: " << get_ball_x() << ")" << std::endl;
#endif    
}
/////////////
/////////////
void Slider::on_mouse_move_ball() { // horz slider
    dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));
    ///////////////////
    // is ball/handle is pressed
    //if(Mouse::is_over(get_ball_x(), get_ball_y(), get_ball_width(), get_ball_height()) && Mouse::is_pressed(1)) {}
    // if slider is pressed (at a specific part) // or if ball is moved
    if(Mouse::is_over(get_x()-1, get_y(), get_width(), get_height()) && Mouse::is_pressed(1)) {
        // make sure mouse is not over ball while pressing on the slider (doesnt even work since you are still on slider)
        //std::cout << "Slider pressed at: " << Mouse::get_position(*window) << " (mouse_pos)" << std::endl;
        double mouse_x_relative_to_slider = fabs(get_x() - Mouse::get_position(*window).x); // we use fabs here to turn a negative number to a positive number//double mouse_x_relative_to_ball = fabs(get_ball_x() - Mouse::get_position(*window).x);
        // move ball to where mouse was pressed
        double max_value = range.y;
        // we are unable to set the ball position manually since it is controlled by the renderer   
        //double ball_x = (value / max_value) * get_width();// + ball_size; // to get ball_x from the value
        // only thing we can do is change the value
        // we know how to get ball_x from the value
        // but how do we get value from ball_x ????????
        // move ball by changing the value
        // we are setting the value, so no need to use increment + or decrement -
        set_value ((mouse_x_relative_to_slider * max_value) / (get_width() - ball_size));
        // print some results
        //std::cout << "value: " << value << std::endl; // ball_x at width_of_slider means the value is 100%
        //std::cout << "ball_x: " << get_ball_x() << std::endl;//std::cout << "ball_x_calculated: " << ball_x << std::endl;
        //return;
    }
}
/////////////
/////////////
int Slider::new_(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}
