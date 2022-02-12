#include "../include/spinner.hpp"

Spinner::Spinner() : value(0), range(0, 100), step(1), decimal_places(0), color(160, 160, 160, 1.0), disabled(false),
// label
label(nullptr),
// button
button_width(20), // looks better when same as spinner-center height
button_color(0, 51, 102, 1.0)/*(64, 64, 64, 1.0)*/,
// shape (+, -)
shape_size(8),//(10),
shape_color(255, 255, 255, 1.0)/*(0, 255, 255, 1.0)*/,
shape_depth(2.0),
// separator (gap)
separator(false),
separator_size(1),//(5),
// outline
outline(false),
outline_width(1.0),
outline_color(0, 0, 0, 1.0),
outline_antialiased(false),
// gradient
gradient(false),
gradient_color(color)
{
	set_position(0, 0);
	set_size(60, 20);
	set_orientation(0);
}
/////////////
Spinner::Spinner(int x, int y)
{
	set_position(x, y);
	set_size(40/*60*/, 20/*30*/); // ideally, the width of spinner-center should be large enough to fit big numbers
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
{
    // delete label
    if(label) {
        delete label;
        label = nullptr;
    }
}
/////////////		
void Spinner::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility
    if(!is_visible()) return;
    // callback
	on_button_press();
	// Draw spinner
	//Renderer::draw_spinner_old(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, color.x, color.y, color.z, color.w, value, button_width, button_color, outline, outline_width, outline_color, outline_antialiased);
    Renderer::draw_spinner(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, color.x, color.y, color.z, color.w,
        button_width, button_color,
        shape_size, shape_color, shape_depth, 
        separator, separator_size 
    );
    if(!label) return;
    // Increase spinner-center size if necessary (when number gets too big and spinner is not wide enough to hold number)
    // EDIT: User must adjust the width manually
    //if((label->get_string().length() * 10) > get_width()) {
    //    std::cout << "number size has exceeded spinner width\n";
    //    set_width(label->get_string().length() * 10);
    //}
    // Reduce value to 2 decimal places
    if(decimal_places >= std::cout.precision()) decimal_places = std::cout.precision(); // default precision is probably 6 // decimal places cannot be more than 6 (MAX)
    std::string value_str = std::to_string(value); // output: 0.000000
    std::string value_formatted = value_str.erase((value_str.find(".") + 1) + decimal_places);//value_str.substr(0, value_str.find(".") + 1 + decimal_places); // 0.00 //std::cout << "value reduced to 2 decimal places: " << value_truncated << std::endl;
    // Update label string (value)
    label->set_string((decimal_places > 0) ? value_formatted : std::to_string(static_cast<int>(value)));
    // set label_position relative to slider_position - the "10" represents each glyph's width/height in the label
    if(label->get_alignment() == "left"  ) label->set_relative_position(0, (get_height() / 2) - (10 / 2)); // left will remain 0, y will be centered
	if(label->get_alignment() == "center") label->set_relative_position((get_width() / 2) - ((10 * label->get_string().length()) / 2), (get_height() / 2) - (10 / 2)); // both x and y will be centered; adjusted based on label string's length
	if(label->get_alignment() == "right" ) label->set_relative_position(get_width() - (10 * label->get_string().length()), (get_height() / 2) - (10 / 2)); // right will be move to the far right (width), y will be centered; adjusted based on label string's length
	if(label->get_alignment() == "none"  ) label->set_relative_position(label->get_relative_x(), label->get_relative_y()); // nothing is changed here
	label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y());//(get_height() / 4));//label->set_position(get_x() + label->get_x(), get_y() + label->get_y());//label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y());
    //std::cout << "label_relative_pos: " << spinner->get_label()->get_relative_position() << std::endl;
    //std::cout << "label_pos: " << spinner->get_label()->get_position() << std::endl; 		    
	// Draw label
	label->get_font()->generate(); // generate font glpyh in case it has not yet been generated
	Renderer::draw_text2(label->get_string(), label->get_x(), label->get_y(), label->get_width(), label->get_height(), label->get_angle(), label->get_scale().x, label->get_scale().y, *label->get_font(), label->get_color().x, label->get_color().y, label->get_color().z, label->get_color().w);
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
void Spinner::set_step(double step) {
    this->step = fabs(step); // step MUST be a non-negative number!!
}
/////////////
void Spinner::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
void Spinner::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
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
void Spinner::set_label(const dokun::Label& label)
{
	this->label = &const_cast<dokun::Label&>(label);
	this->label->set_parent(* this); // set parent to Spinner
	this->label->set_string(std::to_string(value));
	// set position of label relative to the spinner
	//this->label->set_position(get_x(), get_y() + 5);//get_height());//5);//(get_height() / 2) - (10 / 2));//this->label->set_alignment("left");
}
/////////////
int Spinner::set_label(lua_State * L)
{
    return 0;
}
////////////////
void Spinner::set_disabled(bool disabled) {
    this->disabled = disabled;
    if(disabled) {
        color = Vector4(160, 160, 160, 1.0);
        button_color = Vector4(64, 64, 64, 1.0);
        //shape_color = // keep the same
    }
}
/////////////
// button functions
void Spinner::set_button_color(unsigned int red, unsigned int green, unsigned int blue) {
    button_color = Vector4(red, green, blue, button_color.w);
}
/////////////
void Spinner::set_button_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    button_color = Vector4(red, green, blue, alpha);
}
/////////////
void Spinner::set_button_color(const Vector3& button_color) {
    set_button_color(button_color.x, button_color.y, button_color.z, this->button_color.w);
}
/////////////
void Spinner::set_button_color(const Vector4& button_color) {
    set_button_color(button_color.x, button_color.y, button_color.z, button_color.w);
}
/////////////
void Spinner::set_button_width(unsigned int button_width) {
    this->button_width = button_width;
}
/////////////
/////////////
// shape functions
void Spinner::set_shape_size(unsigned int shape_size) {
    this->shape_size = shape_size;
}
/////////////
void Spinner::set_shape_color(unsigned int red, unsigned int green, unsigned int blue) {
    shape_color = Vector4(red, green, blue, shape_color.w);
}
/////////////
void Spinner::set_shape_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    shape_color = Vector4(red, green, blue, alpha);
}
/////////////
void Spinner::set_shape_color(const Vector3& shape_color) {
    set_shape_color(shape_color.x, shape_color.y, shape_color.z, this->shape_color.w);
}
/////////////
void Spinner::set_shape_color(const Vector4& shape_color) {
    set_shape_color(shape_color.x, shape_color.y, shape_color.z, shape_color.w);
}
/////////////
void Spinner::set_shape_depth(double shape_depth) {
    this->shape_depth = shape_depth;
}
/////////////
/////////////	
// separator functions
void Spinner::set_separator(bool separator) {
    this->separator = separator;
}
void Spinner::set_separator_size(unsigned int separator_size) {
    this->separator_size = separator_size;
}
/////////////
/////////////
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
// full position and width of spinner (with slider and buttons combined)
/////////////
double Spinner::get_full_x() const {
    return get_left_button_x();
}
/////////////
double Spinner::get_full_y() const {
    return get_y();
}
/////////////
Vector2 Spinner::get_full_position() const {
    return Vector2(get_full_x(), get_full_y());
}
/////////////
unsigned int Spinner::get_full_width() const {
    return (!separator) ? get_left_button_width() + get_width() + get_right_button_width() : get_left_button_width() + get_width() + get_right_button_width() + (separator_size * 2); // same as: get_width() + (button_width * 2) + (separator_size * 2);
}
/////////////
unsigned int Spinner::get_full_height() const {
    return get_height();
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
dokun::Label * Spinner::get_label() const
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
/////////////
double Spinner::get_left_button_x() const {
    return (!separator) ? get_x() - button_width : get_x() - button_width - separator_size; // x pos relative to spinner-center
}
/////////////
double Spinner::get_left_button_y() const {
    return get_y(); // same y-axis as spinner-center
}
/////////////
unsigned int Spinner::get_left_button_width() const {
    return button_width; // button can have its own width
}
/////////////
unsigned int Spinner::get_left_button_height() const {
    return get_height(); // same height as spinner-center
}
/////////////
/////////////
double Spinner::get_right_button_x() const {
    return (!separator) ? get_x() + get_width() : get_x() + get_width() + separator_size; // x pos relative to spinner-center
}
/////////////
double Spinner::get_right_button_y() const {
    return get_y(); // same y-axis as spinner-center
}
/////////////
unsigned int Spinner::get_right_button_width() const {
    return button_width; // button can have its own width
}
/////////////
unsigned int Spinner::get_right_button_height() const {
    return get_height(); // same height as spinner-center
}
/////////////
/////////////
/////////////
/////////////
/////////////
void Spinner::on_button_press() 
{
    if(disabled) return; // disabled GUI cannot interact with user
	// Mouse
	/*if(Mouse::is_over(get_top_button_x(), get_top_button_y(), get_top_button_width(), get_top_button_height())) {
		if(Mouse::is_pressed(1)) set_value(value + step);
	}
	if(Mouse::is_over(get_bottom_button_x(), get_bottom_button_y(), get_bottom_button_width(), get_bottom_button_height())) {
		if(Mouse::is_pressed(1)) set_value(value - step);
	}*/
	if(Mouse::is_over(get_left_button_x(), get_left_button_y(), get_left_button_width(), get_left_button_height())) {
	    if(Mouse::is_pressed(1)) set_value(value - step);
	}
	if(Mouse::is_over(get_right_button_x(), get_right_button_y(), get_right_button_width(), get_right_button_height())) {
	    if(Mouse::is_pressed(1)) set_value(value + step);
	}
}
/////////////
/////////////
/////////////
/////////////
/////////////
