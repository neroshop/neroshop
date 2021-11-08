#include "../include/edit.hpp"

Edit::Edit() : color(160, 160, 160, 255), character_limit(20),
    cursor(true), cursor_x(0), cursor_y(0), cursor_height(20), cursor_space(10), zoom_factor(0), multilined(false), readonly(false), num_newlines(0), label(nullptr),
// outline
    outline(false),
	outline_width(1.0),
	outline_color(0, 0, 0, 255),
	outline_antialiased(false),
// gradient
    gradient(false),
	gradient_color(color)	    
{
	set_position(0, 0);
	set_size(200, 20); // 150, 20
	set_orientation(0);
}
/////////////
Edit::~Edit()
{}
/////////////
void Edit::append(const std::string& text)
{
	set_text(get_text() + text);
}
/////////////
int Edit::append(lua_State *L)
{
	return 0;
}
/////////////
void Edit::draw()
{
    if(this == nullptr) throw std::runtime_error("calling Edit::draw() with a nullptr object"); // cannot draw a nullptr object
    on_draw_before(); // set position relative to parent and watch for any triggers (presses) ***
	if(!is_visible()) return;  // is it visible? 
	if(is_active()) // is it disabled?
	{}
	// callbacks
	    on_hover();// if mouse over edit, change mouse to I-beam
	//on_mousepress();// edit is pressed, set cursor at position_pressed
    //if(is_focused()) {
        on_keypress();
        on_backspace();
        on_enter();
        on_arrow_keys();
    //}
	double x = get_position().x;
	double y = get_position().y;
	double angle = get_angle();
	double scale_x = get_scale().x;
	double scale_y = get_scale().y;
	int width  = get_width();
	int height = get_height();
    int red    = get_color().x;
    int green  = get_color().y;
    int blue   = get_color().z;		
	int alpha  = get_color().w;
    void * font = (label ? label->get_font()->get_data() : nullptr);		
	Vector4 text_color = get_text_color();
	std::string text = (label ? label->get_string() : "");
	// if edit's width increases, the character limit should also increase? - this is up to the user to decide
	//character_limit = width / get_cursor_space(); // get_cursor_space() is the space each character in the edit takes up 
	if(readonly) cursor = false; // no cursor necessary for readonly edits!
	// Draw edit
	Renderer::draw_edit(text, x, y, width, height, angle, scale_x, scale_y,
	    red, green, blue, alpha, multilined, cursor, cursor_x, cursor_y, cursor_height);// Draw text//if(label) label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // label position stays the same - on the edit, but the cursor is the only thing that moves
    // Callback
    on_draw_after(); // draw children ***
}
/////////////
void Edit::draw(double x, double y) 
{
    set_position(x, y);
    draw();
}
/////////////
void Edit::draw(const Vector2& position) 
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int Edit::draw(lua_State *L)
{
	return 0;
}
/////////////
void Edit::cut() 
{}
/////////////
int Edit::cut(lua_State *L)
{
	return 0;
}
/////////////
void Edit::copy() 
{}
/////////////
int Edit::copy(lua_State *L)
{
	return 0;
}
/////////////
void Edit::paste()
{
#ifdef __windows__
	if(!OpenClipboard(nullptr)) // open clipboard
	{
		return;
	}
    append( (const char *)GetClipboardData(CF_TEXT) );
#endif
}
/////////////
int Edit::paste(lua_State *L)
{
	return 0;
}
/////////////
void Edit::undo() 
{}
/////////////
int Edit::undo(lua_State *L)
{
	return 0;
}
/////////////
void Edit::redo() 
{}
/////////////
int Edit::redo(lua_State *L)
{
	return 0;
}
/////////////
void Edit::select() 
{}
/////////////
int Edit::select(lua_State *L)// (highlights specific text) or all
{
	return 0;
}
/////////////
void select_all()
{} 
/////////////
int select_all(lua_State *L)
{
	return 0;
}
/////////////
void Edit::clear() 
{}
/////////////
int Edit::clear(lua_State *L)// (std::string selected_text) or all
{
	return 0;
}
/////////////
void Edit::clear_all() 
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	label->set_string("");
	character_data.clear();
	// set cursor_pos back to 0
	cursor_x = 0;
	cursor_y = 0;
}
/////////////
int Edit::clear_all(lua_State *L)// (std::string selected_text) or all
{
	return 0;
}
/////////////
void Edit::zoom(int value) 
{}
/////////////
int Edit::zoom(lua_State *L)// -1 (zoom out), +1 (zoom in) 
{
	return 0;
}
/////////////
// SETTERS
/////////////
void Edit::set_color(int red, int green, int blue, int alpha) // base_color
{
	color = Vector4(red, green, blue, alpha);
}
/////////////
void Edit::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
/////////////
void Edit::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int Edit::set_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_cursor(bool cursor)
{
	(this)->cursor = cursor;
} 
/////////////
int Edit::set_cursor(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_cursor_x(double x)
{
    // cannot go past the width of the edit
	if(x >= get_width()) {cursor_x = get_width();return;}
	if(x <= 0)           {cursor_x = 0;return;          }
	cursor_x = x;	
}
/////////////
void Edit::set_cursor_y(double y)
{
	cursor_y = y;
}
/////////////
void Edit::set_cursor_position(double x, double y)
{
    set_cursor_x(x);
	set_cursor_y(y);
} 
/////////////
void Edit::set_cursor_position(const Vector2& position)
{
	set_cursor_position(position.x, position.y);
}
/////////////
/////////////
int Edit::set_cursor_position(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
	this->label->set_parent(* this);
	// adjust label relative position (center the y, space the x by 1) [2021-10-17]
	this->label->set_relative_position(this->label->get_relative_x() + 1, (get_height() / 2) - (cursor_space / 2));
}
/////////////
int Edit::set_label(lua_State *L)
{
	return 0;
}
/////////////
// TEXT-RELATED
/////////////
void Edit::set_text(const std::string& text)
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	std::string string0 (text);
	if(string0.length() > get_character_limit()) { string0.resize(character_limit); std::cout << "Character limit (" <<  character_limit << ") has been exceeded!" << std::endl;} // resizes string to x character limit // character limit has been reached. Character limit is user-defined but can NEVER exceed edit capacity
	if(cursor_x > get_width()) // cursor cannot go past edit
	{   // keep label from going past edit
	    if(!is_multilined()) { string0.resize(get_capacity());} // capacity = width / get_cursor_space() = x characters MAX
		/*if(is_multilined())  // if multilined=true, create a new line // a multilined editor
		{
	        char last_char = String::get_last_character(string0);
	        // remove last (51th) chatracter
	        string0 = string0.erase(string0.length() - 1);//std::cout << last_char << " removed\n";		
		
		    int char_height = label->get_font()->get_height();
		    // double the height of the edit
			set_height(get_height() + char_height); // GOOD!
			// add a newline in text (basically replaces last_char with a newline)
			string0.append("\n"); num_newlines = num_newlines + 1;
			// also append the last character
			string0.append(String::to_string(last_char)); // finally append last (51th) character to the newline
			// reset cursor x position - try to get location of newline
			set_cursor_x(0);
			// set cursor y position (bring it down)
			set_cursor_y(cursor_y + char_height);
	    }*/
	}		
	label->set_string(string0);
    /////////////////////////////////
    // if cursor has not moved from its initial position and the text in the edit is not empty
    if(cursor_x <= 0 && !string0.empty()) {
	    if(character_data.size() >= get_character_limit()) return; // character_position_data must NOT exceed the number of characters in string! // if character limit has been reached, exit function (so character_data does not push_back more unnecessary characters ...)
		//int cursor_index = ((get_text().length() > get_character_limit()) ? -1 : cursor_x);//cursor_x;
		for(int i = 0; i < string0.length(); i++) { // store all string chars in vector
		    set_cursor_x(cursor_x + get_cursor_space()); // for each character, move 10 units right
		    int string_index = (cursor_x / get_cursor_space()) - 1;
            // save each character in a vector (for later use)
		    character_data.push_back(std::make_tuple(string0[i], cursor_x, string_index)); // save character's index in cursor and index in string
	    #ifdef DOKUN_DEBUG0 // print some results ...
            std::cout << "\"" << std::get<0>(character_data[i]) << "\" (cursor_x: " << std::get<1>(character_data[i]) << ", string_index: " << std::get<2>(character_data[i]) << ", characters: " << ((character_data.empty()) ? 0 : character_data.size()) << ")" << std::endl; // for debug + testing purposes
	    #endif	    
	    }
	#ifdef DOKUN_DEBUG0    
	    std::cout << "\033[0;33;49m" << "Edit::set_text() called outside of Edit::on_keypress()" << "\033[0m" << std::endl;
	#endif
	    return; // exit function
    }
}
/////////////
int Edit::set_text(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_font(const FONT& font)
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	get_label()->set_font(font); // set label font (if already exist)
} 
/////////////
int Edit::set_font(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_color(int red, int green, int blue, int alpha)
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	label->set_color(red, green, blue, alpha);
} 
/////////////
void Edit::set_text_color(const Vector3& color)
{
	set_text_color(color.x, color.y, color.z);
}
/////////////
void Edit::set_text_color(const Vector4& color)
{
	set_text_color(color.x, color.y, color.z, color.w);
}
/////////////
int Edit::set_text_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_size(int width, int height)
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	get_label()->set_size(width, height);
}
/////////////
int Edit::set_text_size(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_style(int style)
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	get_label()->set_style(style);
} 
/////////////
int Edit::set_text_style(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_background_color(int red, int green, int blue, int alpha)
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	get_label()->set_background_color(red, green, blue, alpha);
}
/////////////
int Edit::set_text_background_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_placeholder_text(const std::string& placeholder_text) // text will be gray / almost transparent
{}
/////////////
void Edit::set_character_limit(int limit)
{
	this->character_limit = limit;
}
/////////////
int Edit::set_maximum_character(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_multilined(bool multilined)
{
	this->multilined = multilined;
}
/////////////
void Edit::set_readonly(bool readonly) 
{
    this->readonly = readonly;
}
/////////////
/////////////
/////////////
/////////////
// GETTERS
/////////////
Label * Edit::get_label()const
{
	return label;
}    
/////////////
int Edit::get_label(lua_State *L)
{
	return 0;
}
/////////////
std::string Edit::get_text()const
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	return get_label()->get_string();
}  
/////////////
int Edit::get_text(lua_State *L)
{
	return 0;
} 
/////////////
Vector4 Edit::get_color()const
{
	return color;
}
///////////// 
int Edit::get_color(lua_State *L)
{
	return 0;
}
/////////////
int Edit::get_cursor_x() const
{
	return cursor_x;
}
/////////////
int Edit::get_cursor_y() const
{
	return cursor_y;
}		
/////////////
Vector2 Edit::get_cursor_position()const
{
	return Vector2(cursor_x, cursor_y);
}  
/////////////
int Edit::get_cursor_position(lua_State *L)
{
	return 0;
}
/////////////
double Edit::get_cursor_space()const {
    return cursor_space;
}
/////////////
FONT * Edit::get_font()const
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
    return get_label()->get_font();
}
/////////////
int Edit::get_font(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Edit::get_text_color()const
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	return get_label()->get_color();
}
/////////////
int Edit::get_text_color(lua_State *L)
{
	return 4;
}
/////////////
Vector2 Edit::get_text_size()const
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	return get_label()->get_size();
}
/////////////
int Edit::get_text_size(lua_State *L)
{
	return 2;
}
/////////////
int Edit::get_text_style()const
{
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	return get_label()->get_style();
}    
/////////////
int Edit::get_text_style(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Edit::get_text_background_color()const
{	
    if(label == nullptr) throw std::runtime_error("label is not initialized");
	return get_label()->get_background_color();
} 
/////////////
int Edit::get_text_background_color(lua_State *L)
{
	return 4;
}
/////////////
int Edit::get_character_limit()const
{
	return character_limit;
}
/////////////
int Edit::get_capacity() const // returns the total number of characters an edit is able to hold
{
    //if(is_multilined()) return (get_width() / get_cursor_space()) * (get_height() / round(label->get_height()/*get_cursor_space()*/));
    return (get_width() / get_cursor_space());
}
/////////////
int Edit::get_capacity(lua_State *L)
{
    return 1;
}
/////////////
int Edit::get_horizontal_capacity() const // returns number of characters a non-multilined edit can hold. A width of 200 would mean a capacity of 20 characters
{
    return (get_width() / get_cursor_space());
}
/////////////
int Edit::get_horizontal_capacity(lua_State *L)
{
    return 1;
}
/////////////
int Edit::get_vertical_capacity() const // returns number of newlines an edit is cabable of (based on its height and the height of the label)
{
    return (get_height() / round(label->get_height()/*get_cursor_space()*/)); // same as console and chatbox
}
/////////////
int Edit::get_vertical_capacity(lua_State *L)
{
    return 1;
}
/////////////
/////////////
/////////////
bool Edit::is_multilined() const
{
	return (multilined == true);
}
/////////////
bool Edit::is_readonly() const {
    return (readonly == true);
}
/////////////
bool Edit::is_empty() const {
    if(label == nullptr) throw std::runtime_error("label is not initialized");
    return label->get_string().empty();
}
/////////////
bool Edit::is_edit() const
{
    return ((this != 0) && (dokun::instanceof<Edit>(this) != 0));
}
////////////
int Edit::is_edit(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<Edit *>(gui)->is_edit());
		return 1;
	}
    lua_pushboolean(L, false);
    return 1;	
}
/////////////
/////////////
void Edit::on_hover()
{
	dokun::Window * window = dokun::Window::get_active();
	if(window == nullptr) return;    
	if(Mouse::is_over(get_position(), get_size()))
	{
	#ifdef __windows__
		HCURSOR ibeam = LoadCursor(nullptr, IDC_IBEAM);//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)ibeam); // DWORD = unsigned long
    #endif
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_xterm); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
	    XFreeCursor(window->get_display(), cursor);
	#endif
	#endif
	}
	else {//if(!Mouse::is_over(get_position(), get_size()))   
	   // revert back to original mouse cursor
	#ifdef __windows__
		HCURSOR arrow = LoadCursor(nullptr, IDC_ARROW);//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)arrow);
	#endif
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_left_ptr); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
		XFreeCursor(window->get_display(), cursor);
	#endif	
	#endif
	}
}
/////////////
void Edit::on_mousepress()
{
    if(readonly) return;
    if(!Mouse::is_over(get_rect())) return; // mouse_not_over_edit, return
	if(Mouse::is_over(get_position(), (get_text().size() * get_cursor_space())+get_cursor_space())) // mouse_over_text
	{
		if(Mouse::is_pressed(1))
		{
		    dokun::Window * window = dokun::Window::get_active();
		    if(window == nullptr) return;
		    double x = fabs((double)Mouse::get_position(*window).x - (double)(get_x() + cursor_x)); // abs() for int, fabs() for decimals//if(x < 0) x = -x; = makes negative numbers into a positive
		    set_cursor_position(round(x), cursor_y);//round to the nearest tenth//get_y() - Mouse::get_position(*window).y);//cursor_changed_by_mouse = true; // add chars to location of cursor : get_text().size() / cursor_x
		}
	}
}
/////////////
void Edit::on_keypress()
{
    if(readonly) return;
    // if edit text length exceeds character limit, exit this function (or else program will crash) [2021-09-20]
    if(character_data.size() >= get_character_limit()) return;//if(get_text().length() >= get_character_limit()) return;
    for(unsigned char key = 0; key < 128; key++) //if(Keyboard::is_down())
	{
		//char key = static_cast<char>(Keyboard::key);
		if(Keyboard::is_pressed(key))
		{   
		#ifdef DOKUN_DEBUG0
		    std::cout << key << " pressed\n";
		#endif
		    //if(isascii(key)) {  set_text(get_text() + std::to_string(int(key))); set_cursor_x(cursor_x + 10); return; }
			// number or letter or punctuation or space
			if(isalnum(key) ||  ispunct(key) || isspace(key))
			{   
                // increment by 10 for each character	
				set_cursor_x(cursor_x + get_cursor_space()); // label_width does not give accurate results so DO NOT use! Use 10 instead.			
			    // insert character in string    //set_text(get_text() + String::to_string(key)); // old way of appending characters
				std::string temp(get_text());
				int string_index = (cursor_x / get_cursor_space()) - 1;//((cursor_x + get_cursor_space()) / get_cursor_space()) - 1;     // get character at string_index (where cursor_x is) // add cursor_x to increment (10) before we increase cursor_x by 10.
			    temp.insert(string_index, String::to_string(key)); // insert character in between string (+10=advance_to_next_spot)
			    set_text(temp);                                    // set text
				if(cursor_x > 0 && !temp.empty()) {
				    // store character position (for later use)
	                if(character_data.size() >= get_character_limit()) return; // character_position_data must NOT exceed the number of characters in string! // if character limit has been reached, exit function (so character_data does not push_back more unnecessary characters ...)
			        int cursor_index = ((get_text().length() > get_character_limit()) ? -1 : cursor_x);//cursor_x;
			        character_data.push_back(std::make_tuple((char)key, cursor_index, string_index)); // save character's index in cursor and index in string			
			    #ifdef DOKUN_DEBUG0 // print some cool stuff here ...
                    std::cout << "\"" << std::get<0>(character_data[character_data.size() - 1]) << "\" (cursor_x: " << std::get<1>(character_data[character_data.size() - 1]) << ", string_index: " << std::get<2>(character_data[character_data.size() - 1]) << ", characters: " << ((character_data.empty()) ? 0 : character_data.size()) << ")" << std::endl; // for debug + testing purposes
			        std::cout << "\033[0;33;49m" << "Edit::on_keypress() called" << "\033[0m" << std::endl;
			    #endif
			    }
				return;
			}
		}
	}
}
/////////////
void Edit::on_backspace() // bug: crashes everytime backspace is used when cursor position is not at the end of text
{
    if(readonly) return;
    if(get_text().empty()) return; // if empty string, there is nothing to delete, so return (so it does not crash engine)
    int string_index = (cursor_x / get_cursor_space()) - 1; // This will give you the character's position in the string (where cursor_x is at) // before deletion // strings are empty at 0, not -1
    if(string_index < 0) return; // a string index at -1 means that the cursor is at position 0 relative to the edit, so it cannot go back any further or else it will go outside the edit's bounds [2021-09-20]
#ifdef __windows__
    if(Keyboard::is_pressed(0x08))
#endif
#ifdef __gnu_linux__
    if(Keyboard::is_pressed(0xff08))
#endif
	{
	    // try to get last character behind cursor
	    char last_char = label->get_string()[string_index];//String::get_last_character(label->get_string()); // get last character in text (before erasing it) // make sure text is not empty so you can operate on it (to prevent crash) if(get_text().empty()) return;
		// erase last character in string    //if(get_text().length() > 0) set_text(get_text().erase(get_text().length() - 1));	// old way of deleting characters
		if(get_text().length() > 0) 
		{
		#ifdef DOKUN_DEBUG
		    //std::cout << "string_index of deleted character at cursor_x: " << string_index << std::endl;
		#endif
		    std::string temp(get_text());            // copy of label_string
	        temp.erase(temp.begin() + string_index); // erase character at string_index
	        set_text(temp);                          // set text
		    // bring cursor_x back, on character deletion
		    set_cursor_x(cursor_x - get_cursor_space());
		    // remove deleted character from data
		    character_data.erase(character_data.begin() + string_index); // if string_index=5. deletes 6th element
	    #ifdef DOKUN_DEBUG0
		    std::cout << "\"" << last_char << "\" (\033[0;91;49mdeleted" << "\033[0m" << " at index: " << string_index << ", ";
		    std::cout << "cursor is now at index: " << (cursor_x / get_cursor_space()) - 1 << ", "; // after deletion // returns "-1" if last_character does not exist (string is empty)
		    std::cout << "characters: " << ((character_data.empty()) ? 0 : character_data.size()) << ")" << std::endl;
	    #endif
	    }
	    ////////////////////////////////////////////////////////////// temp
	    // This deals with cursor_y which is ONLY for multilined edit!
		// if a newline is removed, decrease the size of the edit
		if(last_char == '\n' && multilined) {
		    int char_height = label->get_font()->get_height();
		    set_height(get_height() - char_height);
		    // decrease num_newlines
		    num_newlines = num_newlines - 1;
		    // bring cursor_y back up (if erasing newline)
		    set_cursor_y(cursor_y - char_height);
		}
		// reset cursor position if text is empty
		if(get_text ().empty()) set_cursor_position(0, 0);
		// clear character_position_data if text is empty
		if(get_text ().empty()) character_data.clear   ();
	}	
}
/////////////
void Edit::on_enter()
{
    if(readonly) return;
#ifdef __windows__
    if(Keyboard::is_pressed(0x0D))
#endif	
#ifdef __gnu_linux__	
	if(Keyboard::is_pressed(0xff0d))
#endif		
	{
		if(is_multilined()) // a multilined editor
		{
		    int char_height = label->get_font()->get_height();
		    // double the height of the edit
			set_height(get_height() + char_height); // GOOD!
			// add a newline in text
			set_text(get_text() + "\n");
			num_newlines = num_newlines + 1;
			// reset cursor x position - try to get location of newline
			set_cursor_x(0);
			// set cursor y position (bring it down)
			set_cursor_y(cursor_y + char_height);
		}
	}	
}
/////////////
void Edit::on_arrow_keys()
{
    if(readonly) return;
    if(Keyboard::is_pressed(DOKUN_KEY_LEFT)) set_cursor_x(cursor_x - get_cursor_space());
    // prevent cursor_x from going beyond "end of string"
    if(cursor_x < (label->get_string().length() * get_cursor_space())) { if(Keyboard::is_pressed(DOKUN_KEY_RIGHT)) set_cursor_x(cursor_x + get_cursor_space()); } // <= don't change this. It's good! Keeps cursor from going to empty white space area
    //if(Keyboard::is_pressed(DOKUN_KEY_UP))
    //if(Keyboard::is_pressed(DOKUN_KEY_DOWN))
}
/////////////
// void Edit::on_left_mouse()
//{
//    // show list
//}
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
int Edit::new_(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}
/////////////
// to-do:
// - add unicode support
// - get key combination input (e.g. shift + A)
// - fix focused_gui()
// - fix multi-line label
