#ifndef SPINNER_HPP_DOKUN
#define SPINNER_HPP_DOKUN

#include "ui.hpp"
#include "label.hpp"
#ifdef __cplusplus
class Spinner : public GUI
{
public:
    Spinner();                                                      static int spinner_new(lua_State *L);
	Spinner(int x, int y);
	Spinner(int x, int y, int width, int height);
	~Spinner();
		
	void draw();                                                    static int draw(lua_State *L);
	void draw(double x, double y);
	void draw(const Vector2& position);	
	
	void set_value(double value);                                   static int set_value(lua_State * L);
	void set_range(double min_val, double max_val);                 static int set_range(lua_State * L);
	void set_range(const Vector2& range);
	void set_step(double step); // 2021-12-18
	void set_decimals(unsigned int decimals);
	void set_color(unsigned int red, unsigned int green, unsigned int blue);
	void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);  static int set_color(lua_State * L);
	void set_color(const Vector3& color);
	void set_color(const Vector3& color, double alpha);
	void set_color(const Vector4& color);
	void set_label(const dokun::Label& label);                             static int set_label(lua_State * L);
	void set_disabled(bool disabled);
	void set_style(int style);
	// button functions
	void set_button_color(unsigned int red, unsigned int green, unsigned int blue);
	void set_button_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
	void set_button_color(const Vector3& button_color);
	void set_button_color(const Vector4& button_color);
	void set_button_width(unsigned int button_width);
	// shape functions
	void set_shape_size(unsigned int shape_size); // width and height
	void set_shape_color(unsigned int red, unsigned int green, unsigned int blue);
	void set_shape_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
	void set_shape_color(const Vector3& shape_color);
	void set_shape_color(const Vector4& shape_color);
	void set_shape_depth(double shape_depth);
	// separator functions
	void set_separator(bool separator);
	void set_separator_size(unsigned int size);
	
	double get_value() const;                                       static int get_value(lua_State * L);
	double get_minimum_value() const;                               static int get_minimum_value(lua_State * L);	
	double get_maximum_value() const;                               static int get_maximum_value(lua_State * L);
	Vector2 get_range() const;                                      static int get_range(lua_State * L);
	int get_style() const;
	double get_full_x() const; // returns combined position of spinner and buttons
	double get_full_y() const; // returns combined position of spinner and buttons
	Vector2 get_full_position() const; // returns combined position of spinner and buttons // returns the full position of the spinner
	unsigned int get_full_width() const;
	unsigned int get_full_height() const;
	Vector4 get_color() const;                                      static int get_color(lua_State * L);
	dokun::Label * get_label() const;                                      static int get_label(lua_State * L);
	// deprecated functions
	int get_top_button_x() const;                                   //static int get_button_(lua_State * L);
	int get_top_button_y() const;
	int get_top_button_width() const;
	int get_top_button_height() const;
	int get_bottom_button_x() const;
	int get_bottom_button_y() const;
	int get_bottom_button_width() const;
	int get_bottom_button_height() const;
	// left and right button functions
	double get_left_button_x() const;
	double get_left_button_y() const;
	unsigned int get_left_button_width() const;
	unsigned int get_left_button_height() const;
	double get_right_button_x() const;
	double get_right_button_y() const;
	unsigned int get_right_button_width() const;
	unsigned int get_right_button_height() const;
private:
    double value;
	Vector2 range;
	Vector4 color;
	double step; // how much the value can go up or down
	unsigned int decimal_places;
	unsigned int style;
	// button
	int button_width;
	Vector4 button_color;
	// shape (arrows)
	Vector2 arrow_size;
	// shape (+, -)
	unsigned int shape_size;
	Vector4 shape_color;
	double shape_depth; // depth or thickness
	// separator (gap)
	bool separator;
	unsigned int separator_size;
	// label
	std::unique_ptr<dokun::Label> label;
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;
	// gradient
	bool gradient;
	Vector4 gradient_color;	
    // callbacks
    void on_parent() final; // re-implementation of a callback
    void on_button_press();	
};
#endif
#endif
/*
Usage:
	Spinner * spinner = new Spinner();
	spinner->set_position(79, 105);
	dokun::Label * spinner_label = new dokun::Label();
	spinner_label->set_alignment("center");
	spinner_label->set_color(49, 39, 19, 255);
	spinner->set_label(* spinner_label);
	
	// changing the value increment (step), range, and decimal places
	spinner->set_step(0.01);
	spinner->set_range(0.00, 1.00);
	spinner->set_decimals(2); // .00 = two decimal places

	// applying the spinner's value
	box->set_gradient_value(spinner->get_value());
	
	// getting the full width of the spinner + buttons combined:
	if(Mouse::is_over(spinner->get_full_x(), spinner->get_full_y(), spinner->get_full_width(), spinner->get_full_height())) {
	    spinner->set_color(255, 255, 255, 1.0);
	} else spinner->set_color(160, 160, 160, 1.0);
*/
