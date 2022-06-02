#ifndef BUTTON_HPP_DOKUN
#define BUTTON_HPP_DOKUN

#include "ui.hpp"
#include "label.hpp"
#include "image.hpp"
#include "renderer.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Button : public GUI
{
public:
	// Constructor.
    Button();   static int new_(lua_State *L);
	Button(const std::string& text);
	Button(const std::string&, int x, int y);
	Button(const std::string&, int x, int y, int width, int height);		
	Button(int x, int y);
    Button(int x, int y, int width, int height); 
    // Destructor.
	~Button(); 
	// Normal functions.
	void draw();   static void draw(Button & button); static int draw(lua_State *L);
	void draw(double x, double y);
	void draw(const Vector2& position);	
	// Setters.
	void set_text(const std::string& text); static int set_text(lua_State *L);// generates a label for button
    void set_image(const Image& image); static int set_image(lua_State *L); // GUI::add()
	void set_label(const dokun::Label& label); static int set_label(lua_State *L); // dokun::Label::set_text
	void set_color(unsigned int red, unsigned int green, unsigned int blue);
	void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_color(lua_State *L);
	void set_color(const Vector3& color);
	void set_color(const Vector3& color, double alpha);
	void set_color(const Vector4& color);
	void set_alpha(double alpha);   static int set_alpha(lua_State *L);
	void set_outline(bool outline);
	void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);
	void set_outline_color(const Vector3& color);
	void set_outline_color(const Vector3& color, double alpha);
	void set_outline_color(const Vector4& color);	
	void set_radius(double radius);
	void set_fill(bool fill); static int set_fill(lua_State *L);
	void set_shadow(bool shadow); static int set_shadow(lua_State *L);
	void set_gradient(bool gradient); static int set_gradient(lua_State *L);                             //  static int set_(lua_State *L);
	void set_gradient_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0); static int set_gradient_color(lua_State *L);
	void set_gradient_color(const Vector3& color);
	void set_gradient_color(const Vector3& color, double alpha);
	void set_gradient_color(const Vector4& color);
    // border functions - deprecate this since outline is the same as border
	void set_border(bool has_border);
	void set_border_width(int width, int side = 0); // thickness
	void set_border_size(int width, int height);
	void set_border_color(int r, int g, int b, int a = 1);
	void set_border_style(int style, int side = 0);
	void set_border_radius(int radius, int side = 0); // roundness
    // Getters
	dokun::Label * get_label()const; static int get_label(lua_State *L); // accessing label functions = (e.g. get_label()->get_string())
	Image * get_image()const; static int get_image(lua_State *L); // accessing image functions = (e.g. get_image()->get_width() )
	std::string get_text()const; static int get_text(lua_State *L);
	Vector4 get_color()const; static int get_color(lua_State *L);
	Vector4 get_hover_color()const; static int get_hover_color(lua_State *L);
	Vector4 get_press_color()const; static int get_press_color(lua_State *L);
	Vector4 get_disabled_color()const; static int get_disabled_color(lua_State *L);
	Vector4 get_active_color()const; static int get_active_color(lua_State *L);
	double get_radius() const;
	// Boolean
	bool has_border();
	bool has_image();
	bool has_label();
	bool is_text_empty();
	bool has_shadow();
	bool is_filled();
	
	bool is_image()const; // checks if button type is image
	// other
	void on_hover();
	void on_press();
	void on_mouse_interact();
private:
	Vector4 color;
    Vector4 hover_color;
	Vector4 press_color;
	Vector4 old_color; // might not need
	// content
	std::unique_ptr<dokun::Label> label;
	std::unique_ptr<Image> image;
	//////////////
	double tint_factor; // 0.25, 0.5, 0.75, 1.0 (100% tint = white)
	double shade_factor; // 0.25, 0.5, 0.75, 1.0 (100% shade = black)
	int depth ; // press depth
	std::string type;
	double radius;
	// boolean (haves and don't-haves)
	bool fill; // is filled(with color)?
	bool shadow; // has shadow?
	bool border; // has border?
	//////////////
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;
	//////////////
	// border properties
	Vector3 border_color;
	Vector2 border_size;
	int border_thickness;
	int border_style;
	double border_radius;
	// shadow properties
	//////////////
	// highlight
	bool highlight;
	Vector4 highlight_color;
	//////////////
	bool gradient;
	Vector4 gradient_color;		
};
#endif
#endif
