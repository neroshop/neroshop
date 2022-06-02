#ifndef TAB_HPP_DOKUN
#define TAB_HPP_DOKUN

#include "ui.hpp"
#include "box.hpp"
#ifdef __cplusplus
class Tab : public GUI
{
public:
    Tab();
    ~Tab();

	void draw();                               static int draw(lua_State * L);
	void draw(double x, double y);
	void draw(const Vector2& position);
    void add(const Box& box);                  static int add(lua_State * L);
	
	void set_text(const std::string& text);    static int set_text(lua_State *L);
	void set_label(const dokun::Label& label);        static int set_label(lua_State *L);
	void set_image(const Image& image);        static int set_image(lua_State *L);
	void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);
	void set_color(const Vector3& color);      static int set_color(lua_State *L);
	void set_color(const Vector3& color, double alpha);
	void set_color(const Vector4& color);
	
    std::string get_text()const;               static int get_text(lua_State *L);
	dokun::Label * get_label()const;                  static int get_label(lua_State *L);
	Image * get_image()const;                  static int get_image(lua_State *L);
	Vector4 get_color()const;                  static int get_color(lua_State *L);
	Vector2 get_body_position() const;         static int get_body_position(lua_State *L);
	Vector2 get_body_size() const;	           static int get_body_size(lua_State *L);
private:
    Box * selection;
    dokun::Label * label;
	Image * image;
    Vector4 color;
    std::vector<Box *> tab_list; // number of tabs	
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;	
};
#endif
#endif
