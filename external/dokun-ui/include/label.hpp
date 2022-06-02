#ifndef LABEL_HPP_DOKUN
#define LABEL_HPP_DOKUN

#include "ui.hpp"
#include "font.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>
// dokun::Font : lines 24, 30, 37
namespace dokun {
class Label : public GUI {
    public:
        Label();                                                                              static int label_new(lua_State *L);
		Label(const Label& label);
		Label(const dokun::Font& font);
		Label(const dokun::Font& font, const std::string& text);
		Label(const std::string& text);
		Label(const std::string& text, int x, int y, int width, int height);
        Label(int x, int y);
        Label(int x, int y, int width, int height);
		~Label();
		// normal
		void draw();
		void draw(double x, double y);
		void draw(const Vector2& position);		                                                                           static int draw(lua_State *L);
		void copy(const Label& label);                                                         static int copy(lua_State *L);
		void clear(); // clears the string // 2022-01-04
		// setters
		void set_string(const std::string& text);                                              static int set_string(lua_State *L);                     // void set_(); static int set_(lua_State *L);
		void set_font(const dokun::Font& font);                                                       static int set_font(lua_State *L); // if no font set, use default (from resource)			
		void set_color(unsigned int red, unsigned int green, unsigned int blue);             static int set_color(lua_State *L);
		void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
		void set_color(const Vector3& color);
		void set_color(const Vector3& color, double alpha);
		void set_color(const Vector4& color);
		void set_style(const int style);                                                       static int set_style(lua_State *L);
		void set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha = 1.0);              static int set_background_color(lua_State *L);
		void set_background_color(const Vector3& color);
		void set_background_color(const Vector3& color, double alpha);
		void set_background_color(const Vector4& color);
		void set_alignment(const std::string& alignment);                                      static int set_alignment(lua_State *L);
		// getters
		std::string get_string()const;                                                         static int get_string(lua_State *L);
		dokun::Font * get_font()const;                                                                static int get_font(lua_State *L);
		Vector4 get_color()const;                                                              static int get_color(lua_State *L);
		int get_style()const;                                                                  static int get_style(lua_State *L);
		Vector4 get_background_color()const;  static int get_background_color(lua_State *L);
		std::string get_alignment() const;    static int get_alignment(lua_State *L);
		double get_aspect_ratio_correction(int rect_x, int rect_y) const;                      static int get_aspect_ratio_correction(lua_State * L);
		// override	
		void set_width(int width);                                                             static int set_width(lua_State * L);
		void set_height(int height);                                                           static int set_height(lua_State * L);
		void set_size(int width, int height);                                                  static int set_size(lua_State * L);
		// override (getters)
		int get_width()const;                                                                  static int get_width(lua_State * L);
		int get_height()const;                                                                 static int get_height(lua_State * L);
		Vector2 get_size() const;                                                              static int get_size(lua_State * L);
		Vector4 get_rect() const;                                                              static int get_rect(lua_State * L);
		std::vector<Vector2> get_character_size_array();
		//Vector2 get_scale()const;
		// boolean		
		bool is_empty() const; // 2022-01-04
		bool is_label() const;                                                                 static int is_label(lua_State *L);
        // allow engine to access the default font and other private members of class Label
        friend class Engine;
	private:
	    //void on_parent() final; // re-implementation of a callback // final means GUI::on_parent() is overridden and Label::on_parent() is the final override
	    void generate_shader(void);
	    static Shader * label_shader;
	    void update(void); // updates the width of the label
		std::string string;
		std::unique_ptr<dokun::Font> font;
		Vector4 color;
		unsigned int style;
		Vector4 background_color;
		std::string alignment;
        int width, height;		// original size of label
		//double sx, sy;
};
}
#endif
#endif
