#include "../include/slider.hpp"

Slider::Slider() : value(0), range(0, 100), decimals(2), step(1), foreground_color(0, 51, 102, 1.0), background_color(160, 160, 160, 1.0),
    radius(0.0),
    ball_size(10), ball_color(64, 64, 64, 0.9), ball_radius(0.0), ball_locked(false), label(nullptr),
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
	// temporary ====== I think that users should set the label manually thats why I'm making this temp
    label = std::unique_ptr<dokun::Label>(new dokun::Label(String::to_string_with_precision(value, decimals)));//std::make_shared<dokun::Label>(String::to_string_with_precision(value, decimals));
    label->set_parent(*this);
    label->set_alignment("center"); // label will ALWAYS be fixed at the center of its parent (the slider)
    //=================
}
/////////////            
Slider::Slider(int x, int y) : Slider()
{
	set_position(x, y);
}
/////////////
Slider::Slider(int x, int y, int width, int height) : Slider()
{
	set_position(x, y);
	set_size(width, height);
}
/////////////
Slider::~Slider()
{
    if(label.get()) {
        label.reset();//if(!label.get()) std::cout << "slider label deleted\n";
    }    
    std::cout << "slider deleted\n";
}
/////////////		
void Slider::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(!is_visible()) return;  // is it visible? //if(!is_active())
		double angle = get_angle();
		//on_keyboard_test();//temp
		on_mouse_move_ball();
		// Draw slider				
		if(get_orientation() == 0) { // horizontal
		    int ball_width = ball_size;
		    Renderer::draw_slider(get_x(), get_y(), get_width(), get_height(), 0, get_scale().x, get_scale().y, foreground_color.x, foreground_color.y, foreground_color.z, foreground_color.w,
			// shader    
			    (!shader.get()) ? GUI::gui_shader : shader.get(),
			// beam properties
			    get_range().x, get_range().y, value, background_color, radius,
			// ball properties
			    ball_width, ball_color, ball_radius
			// outline
			    );
		}
		if(get_orientation() != 0) { // vertical 
		    int ball_height = ball_size;
		    Renderer::draw_slider_vertical(get_x(), get_y(), get_width(), get_height(), 0, get_scale().x, get_scale().y, foreground_color.x, foreground_color.y, foreground_color.z, foreground_color.w,
			    (!shader.get()) ? GUI::gui_shader : shader.get(),
			    get_range().x, get_range().y, value, background_color, ball_height, ball_color);
		}	
	// Label
	if(!label) return;
    // set label_position relative to progressbar_position
    if(label->get_alignment() == "left"  ) label->set_relative_position(0, (get_height() / 2) - (label->get_height() / 2)); // left will remain 0, y will be centered
	if(label->get_alignment() == "center") label->set_relative_position((get_width() / 2) - (label->get_width() / 2), (get_height() / 2) - (label->get_height() / 2)); // both x and y will be centered; adjusted based on label string's length
	if(label->get_alignment() == "right" ) label->set_relative_position(get_width() - label->get_width(), (get_height() / 2) - (label->get_height() / 2)); // right will be move to the far right (width), y will be centered; adjusted based on label string's length
	if(label->get_alignment() == "none"  ) {} // nothing is changed here
    // update label string as the slider's value changes
    label->set_string(String::to_string_with_precision(value, decimals));
    // draw label manually since there can only be one label for slider
    label->draw();
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
void Slider::set_foreground_color(const Vector3& color, double alpha) {
    set_foreground_color(color.x, color.y, color.z, alpha);
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
void Slider::set_background_color(const Vector3& color, double alpha) {
    set_background_color(color.x, color.y, color.z, alpha);
}
void Slider::set_background_color(const Vector4& color) {
    set_background_color(color.x, color.y, color.z, color.w);
}
/////////////
int set_foreground_color(lua_State *L) {
    return 0;
}
/////////////
void Slider::set_range(double minimum_value, double maximum_value)
{
	range = Vector2(minimum_value, maximum_value);
	// to make sure value does not go below min_val or surpass max_val
	if(value < minimum_value) value = minimum_value;
	if(value > maximum_value) value = maximum_value;
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
void Slider::set_decimals(int decimals) {
    this->decimals = decimals;
}
/////////////
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
void Slider::set_ball_radius(float ball_radius) {
    this->ball_radius = ball_radius;
}
/////////////
void Slider::set_radius(float radius)
{
	this->radius = radius;
}  
/////////////
int Slider::set_radius(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_label(const dokun::Label& label) // we no longer need this function since slider will have its own label by default, eh, but I'll keep this function anyways ...
{
    std::unique_ptr<dokun::Label> slider_label(&const_cast<dokun::Label&>(label));
    this->label = std::move(slider_label); // unique pointers cannot be copied, but can only be moved // "std::unique_ptr::release()" is a similar function but "std::move()" is better of the two
    this->label->set_parent(*this);
    this->label->set_string(String::to_string_with_precision(value, decimals)); // we will be updating this value in the draw call too which is why I added this here
    this->label->set_alignment("center"); // label will ALWAYS be fixed at the center of its parent (the slider)
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
unsigned int Slider::get_decimals() const {
    return decimals;
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
    return label.get();
}
/////////////
int Slider::get_label(lua_State *L)
{
    return 1;
}
/////////////
float Slider::get_radius() const {
    return radius;
}
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
float Slider::get_ball_radius() const {
    return ball_radius;
}
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
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	///////////////////
    if(!has_focus()) return;
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_LEFT)) 
        set_value(value - step);
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_RIGHT)) 
        set_value(value + step);
#ifdef DOKUN_DEBUG0
    std::cout << "value changed: " << value << " (ball_x: " << get_ball_x() << ")" << std::endl;
#endif    
}
/////////////
/////////////
void Slider::on_mouse_move_ball() { // horz slider
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	///////////////////
    if(!has_focus()) return;
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
