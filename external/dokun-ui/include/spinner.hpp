#ifndef _SPINNER
#define _SPINNER

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
	void set_color(int red, int green, int blue, int alpha = 255);  static int set_color(lua_State * L);
	void set_color(const Vector3& color);
	void set_color(const Vector4& color);
	void set_label(const Label& label);                             static int set_label(lua_State * L);
	
	double get_value() const;                                       static int get_value(lua_State * L);
	double get_minimum_value() const;                               static int get_minimum_value(lua_State * L);	
	double get_maximum_value() const;                               static int get_maximum_value(lua_State * L);
	Vector2 get_range() const;                                      static int get_range(lua_State * L);
	Vector4 get_color() const;                                      static int get_color(lua_State * L);
	Label * get_label() const;                                      static int get_label(lua_State * L);
	int get_top_button_x() const;                                   //static int get_button_(lua_State * L);
	int get_top_button_y() const;
	int get_top_button_width() const;
	int get_top_button_height() const;
	int get_bottom_button_x() const;
	int get_bottom_button_y() const;
	int get_bottom_button_width() const;
	int get_bottom_button_height() const;	
private:
    double value;
	Vector2 range;
	Vector4 color;
	double step; // how much the value can go up or down
	// button
	int button_width;
	Vector4 button_color;
	// label
	Label * label;
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;
	// gradient
	bool gradient;
	Vector4 gradient_color;	
    // callbacks
    void on_button_press();	
};
#endif
#endif
/*
Usage:
	Spinner * spinner = new Spinner();
	spinner->set_position(79, 105);
	Label * spinner_label = new Label();
	spinner_label->set_alignment("center");
	spinner_label->set_color(49, 39, 19, 255);
	spinner->set_label(* spinner_label);
*/
