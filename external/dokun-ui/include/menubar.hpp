#ifndef MENUBAR_HPP_DOKUN
#define MENUBAR_HPP_DOKUN

#include "ui.hpp"
#include "label.hpp"
#include "image.hpp"
#include "box.hpp"
#ifdef __cplusplus
class Menubar : public GUI
{
public:
    Menubar();                             static int menu_new(lua_State *L);
	Menubar(int x, int y);
	Menubar(int x, int y, int width, int height);
	~Menubar();
		
	void draw();                           static int draw(lua_State *L);
	void draw(double x, double y);
	void draw(const Vector2& position);	
	void add(const Box& menu);          static int add(lua_State * L);
	void add(const std::string& menu);
	void sub(const Box& sub_menu, int index);
	void sub(const std::string& menu, const std::string& sub_menu);
    // setters
	void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);
	void set_color(const Vector3& color);
	void set_color(const Vector3& color, double alpha);
	void set_color(const Vector4& color);
	void set_submenu_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);
	void set_submenu_color(const Vector3& color);
	void set_submenu_color(const Vector4& color);
	void set_fill(bool fill); static int set_fill(lua_State *L);
	// outline
	void set_outline(bool outline); static int set_outline(lua_State *L);
	void set_outline_width(double width); static int set_outline_width(lua_State *L);
	void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0); static int set_outline_color(lua_State *L);
	void set_outline_color(const Vector3& color);// static int set_(lua_State *L);
	void set_outline_color(const Vector3& color, double alpha);// static int set_(lua_State *L);
	void set_outline_color(const Vector4& color);// static int set_(lua_State *L);
	void set_outline_antialiased(bool antialised); static int set_outline_antialiased(lua_State *L);	
	// getters
	Box * get_menu(int index);     static int get_menu(lua_State * L); // by index
	Box * get_menu(const std::string& menu_name); // by name
	Box * get_submenu(int menu_index, int sub_index);     static int get_submenu(lua_State * L);
	int get_menu_count(); static int get_menu_count(lua_State * L);
	int get_submenu_count(int index); static int get_submenu_count(lua_State * L);
	Vector4 get_color();              static int get_color(lua_State *L);
	// static int (lua_State * L);
	// boolean
	bool has_sub(int menu_index);// static int (lua_State * L);
private:
    // callback
	void on_hover(void);
	void on_menu_press(Box * menu, int index);
    Box * bar;
	std::vector<Box *> menu_list;
	std::vector< std::vector<Box *> > sub_list;//std::vector<Box *> sub_menu;
	Vector4 color; // applies to all menus
	Vector4 submenu_color; // color for all submenus
	// selected menu
	static Box * selected_menu;	
	// filling the entire menubar space if there are some left
	bool fill;
	// spacing
	double spacing;
	// outline
	bool outline;
	double outline_width;
	bool outline_antialiased; // removes staircase effect from line and straightens the line (can lower performance)
	Vector4 outline_color;	
	// highlight
	bool highlight;
	Vector4 highlight_color;
	// gradient
	bool gradient;
	Vector4 gradient_color;
};
#endif
#endif
/*
USAGE:
	Menubar * menubar = new Menubar();
	menubar->set_position(600, 20);
	menubar->add(*new Box());
	menubar->add(*new Box());
	menubar->add(*new Box());
	menubar->sub(*new Box(), 0); // add submenu to menu 1
	menubar->sub(*new Box(), 0); // add submenu to menu 1
	menubar->sub(*new Box(), 1); // add submenu to menu 2
	menubar->sub(*new Box(), 2); // add submenu to menu 3

*/
