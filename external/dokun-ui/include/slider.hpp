#ifndef SLIDER_HPP_DOKUN
#define SLIDER_HPP_DOKUN

#include "ui.hpp"
#include "label.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Slider : public GUI // a bit like a ball and beam
{
public:
	    Slider();                                                                 static int new_(lua_State *L);
		Slider(int x, int y);
		Slider(int x, int y, int width, int height);
		~Slider();
		
		void draw(); 
	    void draw(double x, double y);
	    void draw(const Vector2& position);		                                                             static int draw(lua_State *L);
		void reset();                                                             static int reset(lua_State *L);
		
		void set_foreground_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_foreground_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);            static int set_foreground_color(lua_State *L);// beam color
		void set_foreground_color(const Vector3& color);
		void set_foreground_color(const Vector3& color, double alpha);
		void set_foreground_color(const Vector4& color);
		void set_background_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);            static int set_background_color(lua_State *L);
		void set_background_color(const Vector3& color);
		void set_background_color(const Vector3& color, double alpha);
		void set_background_color(const Vector4& color);
		void set_range(double max_val, double min_val);                           static int set_range(lua_State *L);
		void set_minimum_value(double min_val);                                   static int set_minimum_value(lua_State *L);
		void set_maximum_value(double max_val);                                   static int set_maximum_value(lua_State *L);
		void set_value(double value);                                             static int set_value(lua_State *L);
		void set_decimals(int decimals);
		// ball properties
		void set_ball_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);             static int set_ball_color(lua_State *L);// sets overall ball color for both inner and outer
		void set_ball_inner_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_ball_inner_color(lua_State *L);// ball and beam parts of slider
		void set_ball_outer_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_ball_outer_color(lua_State *L);// ball and beam parts of slider
		void set_ball_size(int ball_size);
		void set_ball_radius(float ball_radius);
		// slider (beam) properties
		void set_radius(float radius);                                           static int set_radius(lua_State *L);// roundness of beam edges [===========]
		// label
		void set_label(const dokun::Label& label);                                       static int set_label(lua_State *L);
		// outline properties
		// outline
		void set_outline(bool outline);                                           static int set_outline(lua_State *L);
		void set_outline_width(double width);                                     static int set_outline_width(lua_State *L);
		void set_outline_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);    static int set_outline_color(lua_State *L);
		void set_outline_color(const Vector3& color);                             // static int set_(lua_State *L);
		void set_outline_color(const Vector3& color, double alpha);                             // static int set_(lua_State *L);
		void set_outline_color(const Vector4& color);                             // static int set_(lua_State *L);
		void set_outline_antialiased(bool antialised);                            static int set_outline_antialiased(lua_State *L);
		// border properties
		void set_border(bool border);
		// getters
		double get_value() const;                                                 static int get_value(lua_State *L);
		Vector2 get_range() const;                                                static int get_range(lua_State *L);
		double get_minimum_value() const;                                         static int get_minimum_value(lua_State *L);
		double get_maximum_value()const;                                          static int get_maximum_value(lua_State *L);
		unsigned int get_decimals() const;
		Vector4 get_color() const;                                                static int get_color(lua_State *L);
		dokun::Label * get_label() const;                                                static int get_label(lua_State *L);
		float get_radius() const;
		// ball
		int get_ball_x();                                                         static int get_ball_x(lua_State *L);
		int get_ball_y();                                                         static int get_ball_y(lua_State *L);
		int get_ball_width();                                                     static int get_ball_width(lua_State *L);
		int get_ball_height();                                                    static int get_ball_height(lua_State *L);
		Vector4 get_ball_color(int layer = 0) const;
		float get_ball_radius() const;
		// boolean
		bool is_moved();                                                          static int is_moved(lua_State *L); // ball on slider is moved(value changed)
		//static int (lua_State *L);
	// callbacks
	void on_mouse_move_ball();	
	void on_keyboard_test(); //temp	
private:
	    // beam
	    double value;
		Vector2 range;
		unsigned int decimals;
		double step; // this isn't actually being used except maybe for testing purposes	
		Vector4 foreground_color;
		Vector4 background_color;
		float radius;
		// ball
		double ball_position;
		Vector4 ball_color;
		Vector4 ball_color_inner;
		Vector4 ball_color_outer;
		int ball_size;
		double ball_radius;
		bool ball_locked;
		// label
		std::unique_ptr<dokun::Label> label;
		// outline
		bool outline;
		Vector4 outline_color;
		double outline_width;
		bool outline_antialiased;
		// gradient
		bool gradient;
		Vector4 gradient_color;		
};
/*
// slider example:
[----0---]

// Checking if mouse is over pivot (horizontal slider)
double ball_y        = slider->get_y();
double ball_width    = slider->get_ball_width();
double ball_x        = slider->get_x() + slider->get_value() * 2;
double ball_height   = slider->get_height();
if(Mouse::is_over(ball_x, ball_y, ball_width, ball_height))
{
	if(Mouse::is_down()) // drag the pivot
	{
		slider->set_value();
	}
}	
	Slider * slider = new Slider();
	slider->set_position(10, 200);
	
	
*/
#endif
#endif
