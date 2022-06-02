#ifndef COMBOBOX_HPP_DOKUN
#define COMBOBOX_HPP_DOKUN

#include "list.hpp"
#include "scrollbar.hpp"
#ifdef __cplusplus
class Combobox : public GUI
{
public:
    Combobox();
	~Combobox();
	
	void draw();                               static int draw(lua_State * L);
	void draw(double x, double y);
	void draw(const Vector2& position);
    void add(const Box& widget);            static int add(lua_State * L);
	
	void set_list(const List& list);           static int set_list(lua_State * L);
	void set_scrollbar(const Scrollbar& scrollbar); static int set_scrollbar(lua_State * L);
	void set_text(const std::string& text);    static int set_text(lua_State *L);
	void set_label(const dokun::Label& label);        static int set_label(lua_State *L);
	void set_image(const Image& image);        static int set_image(lua_State *L);
	void set_color(unsigned int red, unsigned int green, unsigned int blue);
	void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
	void set_color(const Vector3& color);      static int set_color(lua_State *L);
	void set_color(const Vector3& color, double alpha);
	void set_color(const Vector4& color);
	
    std::string get_text()const;               static int get_text(lua_State *L);
	dokun::Label * get_label()const;                  static int get_label(lua_State *L);
	Image * get_image()const;                  static int get_image(lua_State *L);
	Vector4 get_color()const;                  static int get_color(lua_State *L);	
	List * get_list() const;                   static int get_list(lua_State * L);
	Scrollbar * get_scrollbar() const;         static int get_scrollbar(lua_State * L);
	// button
	int get_button_x() const;                  static int get_button_x(lua_State *L);
	int get_button_y() const;                  static int get_button_y(lua_State *L);
	int get_button_width() const;              static int get_button_width(lua_State *L);
	int get_button_height() const;             static int get_button_height(lua_State *L);	
private:
    // callback
	void on_button();
	void on_select(); // selection
	Box * selection;
    Vector4 selection_color;
    Vector4 color;
	dokun::Label * label;
	Image * image;
	List * list;
	Scrollbar * scrollbar;
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;
    // button
    Vector4 button_color;
    int button_width;
    bool button_activated;
    // highlight
	bool highlight;
	Vector4 highlight_color;	
	// ...
	bool selected;
};
#endif
#endif
/* EXAMPLE USAGE:
	List * list = new List();
	list->add(*new List());
	// lists with different sizes
	list->add(*new List(0, 0));
	list->add(*new List(0, 0));
	list->add(*new List(0, 0));
	list->add(*new List(0, 0));
	// .. and different colors
	list->get_item(0)->set_color(220,	20,	60);
	list->get_item(1)->set_color(218,	112,	214);
	list->get_item(2)->set_color(65,	105,	225);
	list->get_item(3)->set_color(110,	123,	139);
	list->get_item(4)->set_color(0,	139,	69);
	
	Combobox * combo = new Combobox();
	combo->set_position(500, 120);
	combo->set_list(*list);
	// change sprite color based on combo
	sprite->set_color(combo->get_color());
*/
