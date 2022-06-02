#ifndef EDIT_HPP_DOKUN
#define EDIT_HPP_DOKUN

#include "ui.hpp"
#include "label.hpp"
#include "image.hpp" // for placeholder image (not necessary, but its nice to have)
#include "math.hpp" // round10
#include "timer.hpp" // for blinking cursor effect

#ifdef __cplusplus // if c++
#include <iostream>
#include <tuple>
#include <lua.hpp>

class Edit : public GUI {// can be a line edit or text edit or search field
    public:
	    Edit();                              static int new_(lua_State *L);
		~Edit();
		
		void draw();                   static int draw(lua_State *L);
		void draw(double x, double y); // added 2021-11-04
		void draw(const Vector2& position); // added 2021-11-04
		void append(const std::string& text); static int append(lua_State *L);// appends current text to another text
		void reset(void); // resets edit's string, character_data, and cursor positions // 2022-01-04
		// edit-related stuff here --
		void set_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_color(lua_State *L);// base_color
		void set_color(const Vector3& color);
		void set_color(const Vector3& color, double alpha);
		void set_color(const Vector4& color);
		//void set_cursor(bool cursor); static int set_cursor(lua_State *L);// has text cursor?
		// cursor positioning functions should be made private so that user does not mess up automated cursor positions
		void set_cursor_width(double cursor_width); // added 2021-12-14
		void set_cursor_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_cursor_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); // added 2021-12-14
		void set_cursor_color(const Vector3& cursor_color); // added 2021-12-14
		void set_cursor_color(const Vector3& cursor_color, double alpha);
		void set_cursor_color(const Vector4& cursor_color); // added 2021-12-14
		void set_cursor_advance(double cursor_advance); // sets the num ber of times a cursor should move its position forward
		void set_label(const dokun::Label& label); static int set_label(lua_State *L);
		void set_multilined(bool multilined);
		void set_readonly(bool readonly);
		void set_sensative(bool sensative); // for sensative text like passwords, etc. // 2022-01-03
		void set_scrollable(bool scrollable); // added 2022-01-25
		// text-related stuff here ---
		void set_text(const std::string& text); static int set_text(lua_State *L);// text
		void set_font(const dokun::Font& font); static int set_font(lua_State *L);
        void set_text_color(unsigned int red, unsigned int green, unsigned int blue);
        void set_text_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_text_color(lua_State *L);
	    void set_text_color(const Vector3& text_color);
	    void set_text_color(const Vector3& text_color, double alpha);
	    void set_text_color(const Vector4& text_color);
		void set_text_size(int width, int height); static int set_text_size(lua_State *L);
		void set_text_style(int style); static int set_text_style(lua_State *L); // italic, higlighted, underlined, bold
		void set_text_background_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_text_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); static int set_text_background_color(lua_State *L);
		void set_placeholder_text(const std::string& placeholder_text); // added 2021-11-04
		void set_placeholder_text_font(const dokun::Font& font); // allows user to change placeholder_text's font
		void set_placeholder_text_color(unsigned int red, unsigned int green, unsigned int blue);
		void set_placeholder_text_color(unsigned int red, unsigned int green, unsigned int blue, double alpha); // added 2021-12-17
		void set_placeholder_text_color(const Vector3& color); // added 2021-12-17
		void set_placeholder_text_color(const Vector3& color, double alpha); // added 2021-12-17
		void set_placeholder_text_color(const Vector4& color); // added 2021-12-17
		void set_placeholder_image(const Image& image); // placeholder image
		void set_focus(bool focus); // or set_active(bool active); 
		// getters
		dokun::Label * get_label()const;    static int get_label(lua_State *L);
		Vector4 get_color()const;     static int get_color(lua_State *L);
		int get_cursor_x() const;
		int get_cursor_y() const;
		Vector2 get_cursor_position()const;  static int get_cursor_position(lua_State *L);
		double get_cursor_advance()const; // added 2021-OCT-17 // cursor_advance
		Vector4 get_cursor_color()const; // 2021-12-14
		// text-related getters
		std::string get_text()const;  static int get_text(lua_State *L); 
		dokun::Font * get_font()const;       static int get_font(lua_State *L);
		Vector4 get_text_color()const; static int get_text_color(lua_State *L);
		Vector2 get_text_size()const; static int get_text_size(lua_State *L);
		int get_text_style()const;    static int get_text_style(lua_State *L);
	    Vector4 get_text_background_color()const; static int get_text_background_color(lua_State *L);
		int get_character_limit()const;
		//std::string get_sensative_text()const;
		std::string get_last_n_characters(unsigned int n)const;
		int get_string_parts() const;
		// placeholder_text
		// user should not need access to placeholder_label, only the placeholder_text
		// so Edit::get_placeholder_label() will be marked as deprecated or just set to private
		////dokun::Label * get_placeholder_label()const; // 2021-12-17
		std::string get_placeholder_text() const; // 2021-12-17
		// placeholder image
		Image * get_placeholder_image() const;
		// added - 2019-08-31
		int get_capacity() const;             static int get_capacity(lua_State *L);// returns the number of characters the edit is able to hold (in total)
		int get_horizontal_capacity() const;  static int get_horizontal_capacity(lua_State *L);// returns the number of characters the edit is able to hold (horizontal)
		int get_vertical_capacity  () const;  static int get_vertical_capacity(lua_State *L);// returns the number of "newlines" the edit is able to hold (vertical)
		// boolean
		bool is_multilined() const;
		bool is_readonly() const;
		bool is_sensative() const; // 2022-01-03
		bool is_empty() const;
		bool has_focus() const; // added 2021-12-13
		bool is_edit() const; static int is_edit(lua_State *L);		
		// interactions here --
		void cut();  static int cut(lua_State *L);
		void copy();  static int copy(lua_State *L);
		void copy_all(); // added 2021-12-16
		void paste(); static int paste(lua_State *L);
		void undo(); static int undo(lua_State *L);
		void redo(); static int redo(lua_State *L);
		void select(); static int select(lua_State *L);// (highlights specific text) or all
		void select_all(); static int select_all(lua_State *L);
		void clear(); static int clear(lua_State *L);// (std::string selected_text) or all
		void clear_all(); static int clear_all(lua_State *L);
		void zoom(int value); static int zoom(lua_State *L);// -1 (zoom out), +1 (zoom in)
	    static void lose_focus_all(); // all edits will lose focus		
		// character count
        void set_character_limit(int limit); static int set_maximum_character(lua_State *L);// 0 = unlimited(default)
	    // hiding label =  edit->get_label()->hide() // syntax highlighting???
		// random - syntax highlighting
		////void syntax(std::string keyword, unsigned int red, unsigned int green, unsigned int blue); // if keyword.contains(" \ " ");  if start_of_string = " 
		//void syntax(std::string keyword, const Vector3& color);
	    // friends
	    friend class Console; // console can now access edit's private members
	private:
	    std::string characters_to_string() const; // returns all characters in the character container as a string
		void set_cursor_position(double x, double y); static int set_cursor_position(lua_State *L);// 0, 0 by default0
		void set_cursor_position(const Vector2& position);
		void set_cursor_x(double x);
		void set_cursor_y(double y);	
		// on functions
		void on_hover();
		void on_mouse_press();
		void on_key_press();
		void on_enter();
		void on_backspace();
		void on_arrow_keys();
		void on_cursor(); // cursor callback (blinking cursor effect, etc.)
		void on_placeholder();
		void on_placeholder_text(); // placeholder text callback (show or hide)
		void on_placeholder_image();
		void on_sensative(); // on sensative set to true
		// other functions
		void move_cursor_up();
		void move_cursor_down();
		void move_cursor_left();
		void move_cursor_right();
		// properties
	    Vector4 color;
	    bool   cursor;
		double cursor_x, cursor_y;
		double boundless_cursor_x, boundless_cursor_y;
		int cursor_width;
		int cursor_height;
		Vector4 cursor_color;
		double cursor_space; // space cursor takes up per each character (added 2021-10-17)
		Timer * cursor_blink_timer;
		double cursor_blink_interval;
		double cursor_restore_alpha;
		double zoom_factor; 
		unsigned int character_limit;
		bool multilined;
		bool readonly;
		bool sensative; // passwords, etc.
		bool scrollable; // for scrolling text
		std::string secret; // secret string		
		int newlines_count;
		dokun::Label * label;
		dokun::Label * placeholder_label; // drawn under label (draw before edit)
		std::string selection; // selected string to be copied, cut, cleared, etc.
		unsigned int start_index; // position where the text starts
		// image (icon)
		Image * placeholder_image;
		// scrolling (visible) area
		//double scroll_x, scroll_y;
		// outline
		bool outline;
		double outline_width; // thickness
		Vector4 outline_color;
		bool outline_antialiased;
		// gradient
		bool gradient;
		Vector4 gradient_color;
		// focused edit
		static Edit * focused;
		// maybe add a list later on, with the options: cut, copy, paste, etc.
		// character data
		std::vector< std::tuple<char, unsigned int, int> > character_data;// make a std::tuple, character with cursor_index and string_index // accessing tuple elements: std::get<0>(character_data[0])= first value, std::get<1>(character_data[0])= second value, std::get<3>(character_data[0])=third value // string_index must be an int
		std::vector<char> character_data_in_correct_order;
		// other
		//std::map<std::string, Vector3& color> syntax_list;
		//unsigned int vao, vbo, tbo, ebo;
};
/*
line edit example(single line):
[________]
text edit example(multi-lined):
[________]
[________]
[________]
USAGE:
Edit * edit = new Edit();
	edit->set_position(78, 397);
*/
#endif
#endif
