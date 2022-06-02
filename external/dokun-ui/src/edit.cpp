#include "../include/edit.hpp"

Edit::Edit() : color(190, 190, 190, 1.0), character_limit(20),
zoom_factor(0), newlines_count(0), start_index(0), multilined(false), readonly(false), sensative(false), scrollable(true), label(nullptr), // cursor_height is ALWAYS the same size as edit's height, with some adjustments to padding
// cursor
cursor(true), cursor_x(0), cursor_y(0), cursor_width(1/*8*/), cursor_height(20), cursor_color(0, 0, 0, 0.8), cursor_space(10), cursor_blink_interval(0.90), cursor_restore_alpha(cursor_color.w),
boundless_cursor_x(0), boundless_cursor_y(0),
// placeholder
placeholder_label(nullptr),
placeholder_image(nullptr),
// outline
outline(false),
outline_width(1.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false),
// gradient
gradient(false),
gradient_color(color)	 
// temporary
//,vao(0), vbo(0), tbo(0), ebo(0)	   
{
	set_position(0, 0);
	set_size(200, 20); // 150, 20 // width MUST always be a factor of cursor_space(10), so we don't run into problems later!
	set_orientation(0);
	///////////////////
    cursor_blink_timer = new Timer();
    cursor_blink_timer->start(); // start the timer before loop
    ///////////////////
    // reserve size to character_limit. The vector will retain its original size (will remain empty)
    character_data.reserve(character_limit);//std::cout << "character_array size: " << character_data.size() << std::endl; std::cout << "character array reserved_size: " << character_data.capacity() << std::endl;
    ///////////////////
    ///////////////////
    // generate vertex array object once
    /*Renderer::generate_vertex_array_edit(vao, vbo, tbo, ebo,
        get_x(), get_y(), get_width(), get_height());*/
    ///////////////////
}
/////////////
Edit::~Edit()
{
    // delete placeholders
    if(placeholder_label) {
        delete placeholder_label;
        placeholder_label = nullptr;
    }
    if(placeholder_image) {
        delete placeholder_image;
        placeholder_image = nullptr;
    }
    // delete label
    if(label) {
        delete label;
        label = nullptr;
    }
    // delete timer
    if(cursor_blink_timer) {
        delete cursor_blink_timer;
        cursor_blink_timer = nullptr;
    }
    std::cout << "edit deleted\n";
}
/////////////
Edit * Edit::focused (nullptr); // temp // focus represents the state when the element is currently selected to receive input// https://stackoverflow.com/questions/1677990/what-is-the-difference-between-focus-and-active
/////////////
void Edit::append(const std::string& text)
{
	set_text(label->get_string() + text);
}
/////////////
int Edit::append(lua_State *L)
{
	return 0;
}
/////////////
void Edit::reset() {
    clear_all();
}
/////////////
void Edit::draw()
{
    on_draw_no_focus(); // without checking or setting focus, since edit has its own focus // sets position relative to parent, regardless of visibility
	if(!is_visible()) return;  // is it visible?
	//////////////////////////
	// this is just a test - edit still works whether it has an odd or even width
	// if width is not an even number but an odd, round to the nearest 10 ?
	// % = returns a remainder, but if there's no remainder, then its a factor of cursor_space(10)
	// width MUST be a factor of cursor_space(10) // reminder: width determines capacity
	/*if (get_width() % static_cast<int>(cursor_space) != 0) { // if width is NOT divisible by cursor_space(10), round it to the nearest 10th
	    std::cout << "width(" << get_width() << ") rounded to nearest tenth: " << Math::round10(get_width()) << std::endl;
	    set_width( Math::round10(get_width()) );
	}*/
	//////////////////////////
	// if edit has resized (smaller), and now the visible text (label) has gone past the capacity
	//int text_width = (label->get_string().length() * cursor_space);
	// edit capacity changes everytime edit is resized
	/*if(label->get_string().length() > get_capacity()) {
	    //std::cout << "label width: " << text_width << std::endl;
	    std::cout << "label length: " << label->get_string().length() << std::endl;
	    std::cout << "characters count: " << character_data.size() << std::endl;
	    std::cout << "edit capacity: " << get_capacity() << std::endl;	    
	    ////std::cout << DOKUN_UI_TAG "\033[0;91m" << "LABEL STRING HAS EXCEEDED EDIT CAPACITY!!" << "\033[0m" << std::endl;
	    // re-adjust the label from its starting_index to cursor_position
	    // FROM WHERE BOUNDLESS_CURSOR IS POSITIONED TO END OF STRING
	    // show only n (min(get_capacity(), text_len)) characters
		int start = get_capacity() * get_string_parts(); // increase by capacity(50) every n parts
		int characters_count = character_data.size() - start; // number of characters being drawn at a time          
        // update cursor positions?
        // update starting index
        //if(get_string_parts() < 1) start_index = 0;
        ////if(get_string_parts() > 0) start_index = character_data.size() - (get_capacity() * splits_count);// - 1;//character_data.size() >= get_capacity()        
        std::cout << "string parts (splits): " << get_string_parts() << std::endl;
        std::cout << "starting index: " << start_index << std::endl;
        std::cout << "cursor_x: " << cursor_x << std::endl;
        std::cout << "boundless cursor x: " << boundless_cursor_x << std::endl; 
        // converting boundless cursor_x to bounded cursor_x
        ////if(cursor_x <= 0)  cursor_x = boundless_cursor_x - (get_width() * splits_count);        
        // get current string index where boundless_cursor is at
        int string_index = (boundless_cursor_x) / cursor_space - 1;
        std::cout << "string index: " << string_index << std::endl;        
        // make it the starting index ??
        // the part that is cut off should become the starting_index ?
        ////int cut_off_text = label->get_string().length() - get_capacity();
        ////std::cout << "cut off from edit: " << cut_off_text << std::endl;
        // NEW STARTING INDEX AT CHARACTER_COUNT - CUTOFF_PART
        ////int new_start_index = character_data.size() - cut_off_text;
        ////std::cout << "starting index (new): " << new_start_index << std::endl;
        // BOUNDLESS CURSOR POSITION?
        // SUBTRACT LABEL_STRING UNTIL IT IS EQUAL TO CAPACITY!!!!!
        // ACTUALLY, REMOVE FIRST n (CUT_OFF) CHARACTERS
        // converting boundless cursor_x to bounded cursor_x
        // so it does not go past edit
        //
        // push cursor to end of edit        
        //boundless_cursor_x = character_data.size() * 10;
        //cursor_x = get_width();
        //cursor_x = fabs((get_width() * splits_count) - boundless_cursor_x);
        //std::cout << "boundless_cursor_x: " << boundless_cursor_x << std::endl;
        //std::cout << "cursor_x: " << cursor_x << std::endl;
        //boundless_cursor_x = (start_index * cursor_space);//(character_data.size() * 10) - (label->get_string().length() * 10);
        //std::cout << "boundless_cursor_x (changed): " << boundless_cursor_x << std::endl;
        ////string_index = (boundless_cursor_x / cursor_space);// - 
        ////std::cout << "string index (changed): " << start_index << std::endl;
        // THE START POSITION DOES NOT CHANGE
        // INSTEAD WE USE IT TO FIX THE TEXT
        ////cursor_x = 
        // fix boundless cursor position
        int boundless_cursor_x_fix = boundless_cursor_x - (start_index * cursor_space);
        std::cout << "should_be boundless_cursor_x: " << boundless_cursor_x_fix << std::endl;
        //-----------------
        std::string text = characters_to_string();//label->get_string();
        int n = get_capacity();//(text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();//std::min<size_t>(get_capacity(), character_data.size());
		// GET LAST n characters up to where the boundless_cursor is at
		// FROM END OF CHARACTER_DATA_STRING TO STRING_INDEX WHERE CURSOR IS AT
		size_t cursor_index = string_index;//character_data.size() - string_index;// cursor index
		size_t end_cursor = character_data.size() - 1;//string_index;
		std::string end_to_cursor = text.substr(character_data.size() - cursor_index);
		std::cout << "from end of character_data to  where boundless_cursor is: "
		<< end_to_cursor << " (" << end_to_cursor.size() << ")" << std::endl;
		// AND THEN GET TEXT FROM START_INDEX TO STRING_INDEX WHERE CURSOR IS AT
		std::string start_to_cursor = text.substr(start_index, cursor_index - start_index);
		std::cout << "from start_index to  where boundless_cursor is: "
		<< start_to_cursor << " (" << start_to_cursor.size() << ")" << std::endl;		
		// SET TEXT
		std::string new_text = String::get_first_n_characters(start_to_cursor, n / 2) + String::get_last_n_characters(end_to_cursor, n / 2);
		//text = get_last_n_characters(n);
		// ACTUALLY, REMOVE FIRST n (CUT_OFF) CHARACTERS
		////text.substr(0, std::min<size_t>(n, label->get_string.length()));
		// GET LAST (CAPACITY) CHARACTERS
		// GOAL: GET TEXT FROM START_INDEX TO CAPACITY (whatever the capacity may be)
		//text = text.substr(start_index - string_index, n);//text.substr(start_index, n);//text.substr(string_index - n, n);//text.substr(label->get_string().length() - n);//text.substr(string_index, n);
		#ifdef DOKUN_DEBUG
		    std::cout << "\033[36mtext shifted (fixed): " << text << " (" << text.size() << ")\033[0m" << std::endl; 
		    //std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;
		    std::cout << "\033[36mn = " << n << "\033[0m" << std::endl;
		#endif  	    
		// update label
		label->set_string(new_text);//(end_to_cursor);//(start_to_cursor);//(text);
	}*/
	//////////////////////////
	// callbacks
	on_hover(); // if mouse over edit, change mouse to I-beam
	on_mouse_press();// edit is pressed, set cursor at position pressed // set focus to edit if mouse is pressed
	cursor = (!has_focus()) ? false : true; // hide cursor if not focused	
    on_key_press();
    on_backspace();
    on_enter();
    on_arrow_keys();
	if(readonly) cursor = false; // no cursor necessary for readonly edits!
	// Draw edit (without cursor)    // text is not really used in Renderer::draw_edit() :/
	Renderer::draw_edit("", get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y,
	    get_color().x, get_color().y, get_color().z, get_color().w, (!shader.get()) ? GUI::gui_shader : shader.get(), multilined, false, cursor_x, cursor_y, cursor_width, cursor_height, cursor_color);//if(label) label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // label position stays the same - on the edit, but the cursor is the only thing that moves
    // ::on_placeholder() was causing a segfault [fixed: placeholder_label was not pre-initialized]
    // Draw children (label, placeholder label, etc.)
    on_placeholder(); // hide or show placeholder (on setting a placeholder text or image)
    if(placeholder_label) placeholder_label->draw();
    if(placeholder_image) placeholder_image->draw();
    if(label) label->draw(); // will be drawn over placeholder
    // Draw cursor (over the placeholder - by drawing cursor after placeholder)
    on_cursor(); // cursor blinking effect (on cursor being true)
    if(cursor) Renderer::draw_cursor(cursor_x, cursor_y, cursor_width, cursor_height, cursor_color, (!shader.get()) ? GUI::gui_shader : shader.get(), get_x(), get_y(), get_width(), get_height());
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
void Edit::copy() // copy to clipboard
{
    if(!label) throw std::runtime_error("label is not initialized");
    if(label->get_string().empty()) return; // cannot copy an empty string
    dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));
#ifdef DOKUN_X11 // relies on <X11/Xlib.h>
#endif    
} 
// https://github.com/exebook/x11clipboard/blob/master/x11copy.c
/////////////
int Edit::copy(lua_State *L)
{
	return 0;
}
/////////////
void Edit::copy_all() // copies all text to clipboard
{
    if(!label) throw std::runtime_error("label is not initialized");
    if(label->get_string().empty()) return; // cannot copy an empty string
#ifdef DOKUN_X11
    dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));
    window->copy_clipboard(label->get_string().c_str()); // sensative texts' (secrets) CANNOT be copied!
#endif
}
/////////////
void Edit::paste()
{
    if(!label) throw std::runtime_error("label is not initialized");
#ifdef DOKUN_WIN32
	if(!OpenClipboard(nullptr)) // open clipboard
		return;
    append( (const char *)GetClipboardData(CF_TEXT) );
#endif
#ifdef DOKUN_X11
    dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));
    set_text(label->get_string() += window->paste_clipboard());
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
    if(!label) throw std::runtime_error("label is not initialized");
	label->clear(); // clear label string
	character_data.clear(); // clear all character information
	secret.clear(); // clear secret too (almost forgot :O!)
	// set cursor_pos back to 0
	cursor_x = 0;
	cursor_y = 0;
    // reset boundless_cursor too
    boundless_cursor_x = 0;
    boundless_cursor_y = 0;	
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
void Edit::lose_focus_all() {
    Edit::focused = nullptr;
}
/////////////
/////////////
// SETTERS
/////////////
void Edit::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
/////////////
void Edit::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) // base_color
{
	color = Vector4(red, green, blue, alpha);
}
/////////////
void Edit::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
/////////////
void Edit::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
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
//void Edit::set_cursor(bool cursor) // cursor is enabled by default and can ONLY be disabled if edit is "readonly"
//{
//	this->cursor = cursor;
//} 
/////////////
//int Edit::set_cursor(lua_State *L)
//{
//	return 0;
//}
/////////////
void Edit::set_cursor_x(double x)
{
    // cannot go past the width of the edit
	if(x >= get_width()) {cursor_x = get_width();return;}//get_capacity() * cursor_space
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
void Edit::set_cursor_width(double cursor_width) {
    this->cursor_width = cursor_width;
}
/////////////
void Edit::set_cursor_color(unsigned int red, unsigned int green, unsigned int blue) {
    cursor_color = Vector4(red, green, blue, cursor_color.w);
    cursor_restore_alpha = cursor_color.w; // save alpha value for later
}
/////////////
void Edit::set_cursor_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    cursor_color = Vector4(red, green, blue, alpha);
    cursor_restore_alpha = alpha; // save alpha value for later
}
/////////////
void Edit::set_cursor_color(const Vector3& cursor_color) {
    set_cursor_color(cursor_color.x, cursor_color.y, cursor_color.z);
}
/////////////
void Edit::set_cursor_color(const Vector4& cursor_color) {
    set_cursor_color(cursor_color.x, cursor_color.y, cursor_color.z, cursor_color.w);
}
/////////////
void Edit::set_cursor_advance(double cursor_advance) {
    this->cursor_space = cursor_advance;
}
/////////////
/////////////
void Edit::set_label(const dokun::Label& label)
{
	this->label = &const_cast<dokun::Label&>(label);
	this->label->set_parent(* this);
	// properly set the text to pre-set label text (which will update the character_data vector and correct the boundless_cursor and cursor positions)
	if(!this->label->is_empty()) set_text(this->label->get_string());
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
    if(!label) throw std::runtime_error("label is not initialized");
    if(text.empty()) return; // exit function if text is empty
	// reset edit's text (label string, character_data) and cursor (position)
	reset();
	// check for any string limitations first
	std::string string0 (text);
	// make sure string length does not exceed character_limit
	if(string0.length() > character_limit) { string0.resize(character_limit); std::cout << DOKUN_UI_TAG "\033[0;33mCharacter limit (" <<  character_limit << ") has been exceeded\033[0m" << std::endl;} // resizes string to x character limit // character limit has been reached. Character limit is user-defined but can NEVER exceed edit capacity
	/*if(string0.length() > get_capacity()) // if text goes past edit // (set_cursor_x() already checks if cursor goes past edit, so we don't have to worry about that)
	{   // keep label from going past edit
	    if(!multilined && !scrolling) string0.resize(get_capacity());*/ // capacity = width / get_cursor_advance() = x characters MAX
		/*if(multilined)  // if multilined=true, create a new line // a multilined editor
		{
	        char last_char = String::get_last_character(string0);
	        // remove last (51th) chatracter
	        string0 = string0.erase(string0.length() - 1);//std::cout << last_char << " removed\n";		
		
		    int char_height = label->get_font()->get_height();
		    // double the height of the edit
			set_height(get_height() + char_height); // GOOD!
			// add a newline in text (basically replaces last_char with a newline)
			string0.append("\n"); newlines_count = newlines_count + 1;
			// also append the last character
			string0.append(String::to_string(last_char)); // finally append last (51th) character to the newline
			// reset cursor x position - try to get location of newline
			set_cursor_x(0);
			// set cursor y position (bring it down)
			set_cursor_y(cursor_y + char_height);
	    }*/
	//}		
	/////////////////////////////////
	// hide characters before setting the label string if sensative
	if(sensative) { // text, string0 may be an asterisk, thats why I get asterisks//if(secret.empty()) 
	    secret = string0; // save *new* secret string
	#ifdef DOKUN_DEBUG0
	    std::cout << "secret saved: " << secret << std::endl;
	#endif    
	    // replace all text characters with asterisks
	    std::fill_n(string0.begin() + 0, string0.length(), '*'); // update secret string (fill with asterisks "*") // https://stackoverflow.com/a/2891308
	#ifdef DOKUN_DEBUG0
	    std::cout << "text set to sensative: " << string0 << std::endl;	    
	#endif
	}
	//////////////////////////////////
	// get number of parts of the string that have exceeded capacity(50) (or number of times the string length exceeded the capacity(50) or number of times we must split the string)
	int splits_count = string0.size() / get_capacity(); // (temp.length() % n) != 0 <= to check if not divisible by n beforehand (n = capacity)
    // do not split until characters reach (capacity + 1)
    if(string0.size() < (get_capacity() * splits_count + 1)) splits_count = splits_count - 1;
	//std::cout << std::endl << "splits_count(set_text): " << splits_count << std::endl;	
	//////////////////////////////////

	//////////////////////////////////
	// set cursor position and store all characters' information (for later use)
	//int cursor_index = ((get_text().length() > character_limit) ? -1 : cursor_x);//cursor_x;	
	for(int i = 0; i < string0.length(); i++) { // store all string chars in vector
	    if(splits_count < 1) {
		    cursor_x = cursor_x + cursor_space;//set_cursor_x(cursor_x + cursor_space); // for each character, move cursor_space(10) units right
            boundless_cursor_x = cursor_x;
            start_index = 0; // start_index reset to zero
	    }
	    if(splits_count > 0) {
	        cursor_x = get_width();
	        boundless_cursor_x = boundless_cursor_x + cursor_space;	        
            if(character_data.size() >= get_capacity()) start_index = start_index + 1; // whenever string goes past the end of the edit or surpasses its capacity, increase start_index by 1, shifting the text to the right and hiding the previous character
        }
        // save each character's data in a vector (for later use)
		int string_index = boundless_cursor_x / cursor_space - 1;//(splits_count == 0) ? cursor_x / cursor_space - 1 : boundless_cursor_x / cursor_space - 1;
		character_data.push_back(std::make_tuple(string0[i], boundless_cursor_x, string_index)); // save character's index in cursor and index in string    
	    // string is already resized beforehand, so there may be no point to the code below //if(character_data.size() > character_limit) { std::cout << "error: character_data size has exceeded character_limit\n. Please tell developer to resize!" << std::endl; } // if character limit has been reached, exit function (so character_data does not push_back more unnecessary characters ...)
	#ifdef DOKUN_DEBUG0 // print some results ...
        std::cout << "\"" << std::get<0>(character_data[i]) << "\" (cursor_x: " << std::get<1>(character_data[i]) << ", string_index: " << std::get<2>(character_data[i]) << ", characters: " << ((character_data.empty()) ? 0 : character_data.size()) << ")" << std::endl; // for debug + testing purposes
	#endif	
	}
	// outside of loop
	//////////////////////////////////
        // we want to hide the previous character when we shift right
        int n = (string0.length() < get_capacity()) ? string0.length() : get_capacity();//get_capacity();
		string0 = string0.substr(start_index, n);//(n < text.length()) ? text.substr(text.length() - get_capacity()) : "blank";//text.substr(start_index, n);// comment this out if using: "text.substr(start, characters_count)" below
		#ifdef DOKUN_DEBUG0
		    std::cout << "\033[36mtext shifted (right): " << string0 << " (" << string0.size() << ")\033[0m" << std::endl; 
		    std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;
		#endif  
		// update the label when cursor position is changed  		
		label->set_string(string0);
		//std::cout << "cursor_x (set_text): " << cursor_x << std::endl;
		//std::cout << "boundless_cursor_x (set_text): " << boundless_cursor_x << std::endl;
	//////////////////////////////////	
}
/////////////
int Edit::set_text(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_font(const dokun::Font& font)
{
    if(!label) throw std::runtime_error("label is not initialized");
	get_label()->set_font(font); // set label font (if already exist)
} 
/////////////
int Edit::set_font(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_color(unsigned int red, unsigned int green, unsigned int blue) {
    if(!label) throw std::runtime_error("label is not initialized");
	label->set_color(red, green, blue, label->get_color().w);
}
/////////////
void Edit::set_text_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    if(!label) throw std::runtime_error("label is not initialized");
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
    if(!label) throw std::runtime_error("label is not initialized");
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
    if(!label) throw std::runtime_error("label is not initialized");
	get_label()->set_style(style);
} 
/////////////
int Edit::set_text_style(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_background_color(unsigned int red, unsigned int green, unsigned int blue) {
    if(!label) throw std::runtime_error("label is not initialized");
	label->set_background_color(red, green, blue, label->get_background_color().w);
}
/////////////
void Edit::set_text_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    if(!label) throw std::runtime_error("label is not initialized");
	label->set_background_color(red, green, blue, alpha);
}
/////////////
int Edit::set_text_background_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_placeholder_text(const std::string& placeholder_text) // text will be gray / almost transparent
{
    if(!placeholder_label) placeholder_label = new dokun::Label();
    if(label) placeholder_label->set_font(*label->get_font()); // placeholder_text will use same font as default label (default)
    // make sure placeholder_text does not exceed the capacity (which is determined by the edit's width)
    if(placeholder_text.length() > get_capacity()) const_cast<std::string&>(placeholder_text).resize(get_capacity());//std::cout << DOKUN_UI_TAG "\033[0;33mPlaceholder text has exceeded capacity (" << get_capacity() << ")\033[0m" << std::endl;//std::cout << DOKUN_UI_TAG "\033[0;97mPlaceholder text resized to " << placeholder_text.length() << "\033[0m" << std::endl;
    placeholder_label->set_string(placeholder_text);
    placeholder_label->set_color(32, 32, 32, 0.6); // grayed-out color to make it look like disabled text (default)
    placeholder_label->set_parent(*this);
	placeholder_label->set_relative_position(placeholder_label->get_relative_x() + 1, (get_height() / 2) - (cursor_space / 2)); // adjust label relative position (center the y, space the x by 1) [2021-10-17]
}
/////////////
void Edit::set_placeholder_text_font(const dokun::Font& font) {
    if(!placeholder_label) throw std::runtime_error("placeholder text has not been set");
    placeholder_label->set_font(font); 
}
/////////////
void Edit::set_placeholder_text_color(unsigned int red, unsigned int green, unsigned int blue) {
    if(!placeholder_label) throw std::runtime_error("placeholder text has not been set");//placeholder_label = new dokun::Label();
    placeholder_label->set_color(red, green, blue, placeholder_label->get_color().w);
}
/////////////
void Edit::set_placeholder_text_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    if(!placeholder_label) throw std::runtime_error("placeholder text has not been set");//placeholder_label = new dokun::Label();
    placeholder_label->set_color(red, green, blue, alpha);
}
/////////////
void Edit::set_placeholder_text_color(const Vector3& color) {
    set_placeholder_text_color(color.x, color.y, color.z);
}
/////////////
void Edit::set_placeholder_text_color(const Vector4& color) {
    set_placeholder_text_color(color.x, color.y, color.z, color.w);
}
/////////////
void Edit::set_placeholder_image(const Image& placeholder_image) {
    this->placeholder_image = &const_cast<Image&>(placeholder_image);
}
/////////////
void Edit::set_character_limit(int character_limit)
{
	this->character_limit = character_limit;
	// reserves space for character vector and leaves it empty, so you can insert (up to) that many characters without it having to reallocate
	character_data.reserve(character_limit);
}
/////////////
int Edit::set_maximum_character(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_multilined(bool multilined)
{
	// multilined edits cannot be scrollable (each line ends at the edit's width)
	if(scrollable) {std::cout << "scrollable edits cannot be multilined (at least not yet)" << std::endl;return;}//return;//scrollable = (multilined) ? false : true;
	this->multilined = multilined;
}
/////////////
void Edit::set_readonly(bool readonly) 
{
    this->readonly = readonly;
}
/////////////
void Edit::set_sensative(bool sensative) {
    this->sensative = sensative;
    on_sensative(); // callback (sets the secret if sensative with secret not set yet with Edit::set_text())
}
/////////////
void Edit::set_scrollable(bool scrollable) {
    // scrollable edits cannot be multilined (only single-lined edits can be scrollable)
    // because when text (multilined) reaches the end of edit or capacity, it can just start on a new line
    // if I do make multilined edits scrollable, then that will increase the width of the edit
    if(multilined) return;
    this->scrollable = scrollable;
}
/////////////
void Edit::set_focus(bool focus) {
    //std::cout << "edit set_focus(" << ((focus) ? "true" : "false") << ") called\n";
    Edit::focused = (focus == true) ? this : nullptr;
}
/////////////
/////////////
/////////////
/////////////
// GETTERS
/////////////
dokun::Label * Edit::get_label()const
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
    if(!label) throw std::runtime_error("label is not initialized");
	return (sensative) ? secret : characters_to_string();//label->get_string(); // label only shows visible characters on the edit, but not the whole text so we'll need to use the characters in the character_data vector instead
}  
/////////////
int Edit::get_text(lua_State *L)
{
	return 0;
} 
/////////////
//std::string Edit::get_sensative_text()const {
//    return secret;
//}
/////////////
std::string Edit::get_last_n_characters(unsigned int n)const {
    if(multilined) return get_text(); // multilined texts are not limited by edit capacity
    if(!label) throw std::runtime_error("label is not initialized");
    std::string text = characters_to_string();//label->get_string();  
	// get last n characters from string (with n being the edit's capacity)// capacity = total number of characters that can be shown on the edit at a time// IF TEXT GOES PAST EDIT'S WIDTH, ONLY SHOW THE LAST CHARACTERS(size: get_capacity()) in character_array vector
	n = (text.length() < n) ? text.length() : n;//(text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();
	std::vector<char> last_n_characters(text.end() - n/*std::min(text.size(), n)*/, text.end()); // Of course this will crash and burn if x.size() < n, which is why we use std::min
	// draw and show only the last n characters in edit (visible text)
	std::string visible_text = std::string(last_n_characters.begin(), last_n_characters.end());// construct a string from a vector of chars (std::vector<char> to std::string)
	/////////////////////////////
#ifdef DOKUN_DEBUG0
	if(cursor_x > get_width()) { // if cursor goes past edit's width, show the last n characters
	    for(int i = 0; i < last_n_characters.size(); i++)
	        std::cout << "last n characters in string (show): " << last_n_characters[i] << std::endl;
	    std::cout << "last n characters size: " << last_n_characters.size() << std::endl;
	}
#endif	
#ifdef DOKUN_DEBUG
	std::cout << "draw_visible_text: " << visible_text << " (length: " << visible_text.length() << ")" << std::endl;
#endif
    return visible_text;
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
double Edit::get_cursor_advance()const {
    return cursor_space;
}
/////////////
Vector4 Edit::get_cursor_color()const {
    return cursor_color;
}
/////////////
dokun::Font * Edit::get_font()const
{
    if(!label) throw std::runtime_error("label is not initialized");
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
    if(!label) throw std::runtime_error("label is not initialized");
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
    if(!label) throw std::runtime_error("label is not initialized");
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
    if(!label) throw std::runtime_error("label is not initialized");
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
    if(!label) throw std::runtime_error("label is not initialized");
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
int Edit::get_capacity() const // returns the total number of characters that can be shown at a time (that an edit is able to hold)
{
    //if(is_multilined()) return (get_width() / get_cursor_advance()) * (get_height() / round(label->get_height()/*get_cursor_advance()*/));
    return (get_width() / cursor_space);
}
/////////////
int Edit::get_capacity(lua_State *L)
{
    return 1;
}
/////////////
int Edit::get_horizontal_capacity() const // returns number of characters a non-multilined edit can hold. A width of 200 would mean a capacity of 20 characters
{
    return (get_width() / get_cursor_advance());
}
/////////////
int Edit::get_horizontal_capacity(lua_State *L)
{
    return 1;
}
/////////////
int Edit::get_vertical_capacity() const // returns number of newlines an edit is cabable of (based on its height and the height of the label)
{
    return (get_height() / round(label->get_height()/*get_cursor_advance()*/)); // same as console and chatbox
}
/////////////
int Edit::get_vertical_capacity(lua_State *L)
{
    return 1;
}
/////////////
int Edit::get_string_parts() const {
	// get number of parts of the string that have exceeded capacity(?) (or number of times the string length exceeded the capacity(?) or number of times we must split the string)
	int splits_count = character_data.size() / get_capacity();
    if(character_data.size() < (get_capacity() * splits_count + 1)) splits_count = splits_count - 1; // do not split until characters reach (capacity + 1)
	std::cout << std::endl << "edit->get_string_parts(): string parts (splits_count): " << splits_count << std::endl;
	return splits_count;
}
/////////////
/////////////
//dokun::Label * Edit::get_placeholder_label()const {
//    return placeholder_label;
//}
/////////////
std::string Edit::get_placeholder_text() const {
     if(!placeholder_label) return "";
     return placeholder_label->get_string();
}
/////////////
Image * Edit::get_placeholder_image() const {return placeholder_image;}
/////////////
/////////////
bool Edit::is_empty() const {
    if(!label) throw std::runtime_error("label is not initialized");
    return label->get_string().empty();
}
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
bool Edit::is_sensative() const {
    return (sensative == true);
}
/////////////
bool Edit::has_focus() const {
    return (Edit::focused == this);
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
/////////////
void Edit::on_hover()
{
    if(readonly) return;
    if(is_disabled() || !is_active()) return;
    if(!has_focus()) return;
	dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));//dokun::Window * window = dokun::Window::get_active();
	//if(window == nullptr) return;    
	if(Mouse::is_over(get_x(), get_y(), get_width(), get_height())) {
	#ifdef DOKUN_WIN32
		HCURSOR ibeam = LoadCursor(nullptr, IDC_IBEAM);//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)ibeam); // DWORD = unsigned long
    #endif
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_xterm); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
	    XFreeCursor(window->get_display(), cursor);
	#endif
	#endif
	    //return; // exit function, so we don't revert the mouse cursor too soon
	}
	// revert back to original mouse cursor
	else {//else if(!Mouse::is_over(get_position(), get_size())) {
	#ifdef DOKUN_WIN32
		HCURSOR arrow = LoadCursor(nullptr, IDC_ARROW);//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)arrow);
	#endif
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_left_ptr); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
		XFreeCursor(window->get_display(), cursor);
	#endif	
	#endif
	    //return;
	}	
}
/////////////
void Edit::on_mouse_press()
{
    if(readonly) return;
    if(is_disabled()) return;
    if(!is_active()) return;
    unsigned int string_length = label->get_string().length();//character_data.size()
    dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));
    //Vector4 old_color = label->get_color(); // save original color
    //if(Mouse::is_over(get_x() - 1, get_y(), (string_length * cursor_space), get_height())) { label->set_color(255, 0, 0); } else label->set_color(0, 0, 0);// the 1 makes mouse position accurate and the cursor_x can be 0 instead of 1 every time :| .
    /////////////////////////////////////////
    //if(Mouse::is_over(get_x() - 1, get_y(), get_width(), get_height()) && Mouse::is_pressed(1)) { // on mouse right click
        //std::cout << DOKUN_UI_TAG << "show_list_options (cut, copy, paste, delete, select_all)" << std::endl; 
    //}
    /////////////////////////////////////////
    if(Mouse::is_over(get_x() - 1, get_y(), get_width(), get_height())) { // the + 1 makes mouse position accurate and the cursor_x can be 0 instead of 1 every time :| .
        if(Mouse::is_pressed(1)) {
            //if(window->has_focus()) {
                // mouse position relative to window
                //std::cout << "Mouse pressed at " << Mouse::get_position(*window) << std::endl;
                // assuming that the mouse pointer is a child of the text edit
                // get the mouse position relative to the text edit
                double mouse_x_relative_to_edit = fabs(get_x() - Mouse::get_position(*window).x);//fabs(get_x() - Mouse::get_position(*window).x);
                // round to the nearest tenth (1->0, 5->10, etc.) since cursor_space is 10
                double rounded = Math::round10(mouse_x_relative_to_edit);//std::cout << "rounded_x: " << rounded << std::endl;//std::cout << "mouse pressed at (" << rounded << ")" << std::endl;
                // set cursor pos (relative to edit) to mouse pos (relative to window)
                // cursor_x cannot be a negative number! - reason for fabs()
                /////////////////////////////
                int splits_count = character_data.size() / get_capacity();
		        // if length is not more than the capacity then we do not have to split the string (unless its greater than the capacity)
		        // and we have not reached the max_character or character_limit
		        if(character_data.size() < (splits_count * get_capacity() + 1)) splits_count -= 1;
		        // split the string by capacity(50), n amount of times (n being "splits_count")
		        int start = get_capacity() * splits_count; // increase by capacity(50) every n parts
		        int characters_count = character_data.size() - start; // number of characters being drawn at a time      
                // CONVERT CURSOR COORDINATES TO FULL_CHARACTER_LENGTH COORDINATES
                // start_x = (character_data.size()*label->get_height()) - (characters_count*label->get_height())
                //std::cout << "mouse pressed at (" << (start * cursor_space) + rounded << ")(using start [of new split])" << std::endl;
                //std::cout << "mouse pressed at (" << (character_data.size() * cursor_space) - (characters_count * cursor_space) + rounded << ")(using character_data.size - character_count)" << std::endl;
                ////////////////////////////
                set_cursor_x(rounded); // set cursor_x before changing it to non-relative coordinates
                if(splits_count > 0) rounded = ((character_data.size() * cursor_space) - (string_length * cursor_space)) + rounded;//(character_data.size() * cursor_space) - (characters_count * cursor_space) + rounded;//(start * cursor_space) + rounded; //<= using start works too
                // CONVERT BOUNDED TO BOUNDLESS // last 50 characters
                ////std::cout << "mouse pressed at (" << rounded << ")(using (character_data - string_length) + rounded)" << std::endl; // (character_data.size() * cursor_space) - rounded                
                ////////////////////////////
                boundless_cursor_x = rounded;//set_cursor_x(rounded);
                // USE BOUNDLESS_CURSOR FOR ACCURATE CALCULATION OF STRING_INDEX AND CURSOR FOR NORMAL (DRAWN) POSITIONING                
                // make sure the cursor does not go past the edit
                if(cursor_x > (string_length * cursor_space)) cursor_x = string_length * cursor_space;
                ////std::cout << "new cursor x (relative to edit): " << cursor_x << std::endl;
                // make sure boundless_cursor_x does not exceed characters_x //std::cout << "CANNOT EXCEED: " << (character_data.size() * cursor_space) << std::endl;
                // if width is an even number this works
                if(boundless_cursor_x > (character_data.size() * cursor_space)) boundless_cursor_x = character_data.size() * cursor_space;//character_data.size() * cursor_space;//if(cursor_x >= (string_length * cursor_space)) cursor_x = string_length * cursor_space;
                ////std::cout << "edit on_mouse_press(): cursor_x (boundless): " << boundless_cursor_x << std::endl;                
                // but if width is an odd number, this boundless_cursor_x is not accurate
                // then set focus to the edit
                set_focus(true);
                //////////////////////////////
                /*// highlight part of text in direction of mouse
                std::cout << "mouse locked to edit\n";
                if(Mouse::is_released(1) && has_focus()) {
                    std::cout << "mouse is unlocked from edit\n";
                }*/
            #ifdef DOKUN_DEBUG0
	            if(Edit::focused) std::cout << DOKUN_UI_TAG + String(String::no_digit(typeid(*this).name())).str() << ":" << String(Edit::focused) << " gained focus" << std::endl; // #include <typeinfo>
            #endif                
            //}
        }
    }
}
/////////////
void Edit::on_key_press()
{
    if(!is_visible() || is_disabled() || !is_active()) return;
    if(readonly) return;
    if(!has_focus()) return;
    if(!label) throw std::runtime_error("label is not initialized");
    // if edit text length exceeds character limit, exit this function (or else program will crash) [2021-09-20]
    if(character_data.size() >= character_limit) return; // number of characters must NOT exceed the character_limit! // if character limit has been reached, exit function (so character_data does not push_back more unnecessary characters ...)
    for(unsigned char key = 0; key < 128; key++) //if(dokun::Keyboard::is_down())
	{
		//char key = static_cast<char>(dokun::Keyboard::key);
		if(dokun::Keyboard::is_pressed(key))
		{
		    //if(isascii(key)) {  set_text(get_text() + std::to_string(int(key))); set_cursor_x(cursor_x + cursor_space); return; }
			// number or letter or punctuation or space
			if(isalnum(key) || ispunct(key) || isspace(key))
			{
				// ONLY the boundless_cursor_x can the determine the true string_index, since it has no upper limits
				// the cursor_x, on the other hand has its limits 0 - (CAPACITY * CURSOR_ADVANCE_SPACE)
				int string_index = (boundless_cursor_x + cursor_space) / cursor_space - 1;
			    // hide characters (for sensative texts like passwords, etc.) by replacing each character with "*"
			    if(sensative) { 
			        secret.insert(string_index, String::to_string(key)); // save actual characters first//	    
			        key = '*';
				    ////std::cout << "secret string: " << secret << std::endl;		
				}
				// store characters information (char, cursor_position, string_index) (for later use)
			    int cursor_index = boundless_cursor_x + cursor_space;
			    // add character to vector (at a specific index, where the cursor is positioned)// to-do: find difference b/t insert vs emplace (emplace_back vs push_back is basically the same situation) // emplace operation avoids unnecessary copy of object and does the insertion more efficiently than insert operation. Insert operation takes a reference to an object
			    // no need to save the string_index now that the characters are inserted into vector in the correct order, but lets save it anyways I guess :?			    
			    character_data.insert(character_data.begin() + string_index, std::make_tuple(static_cast<char>(key), cursor_index, string_index));
			#ifdef DOKUN_DEBUG0 // print some cool stuff here ...
			    std::cout << "\033[0;33;49m" << "Edit::on_keypress() called" << "\033[0m" << std::endl;
                std::cout << "\"" << std::get<0>(character_data[character_data.size() - 1]) << "\" (cursor_x: " << std::get<1>(character_data[character_data.size() - 1]) << ", string_index: " << std::get<2>(character_data[character_data.size() - 1]) << ", characters: " << ((character_data.empty()) ? 0 : character_data.size()) << ")" << std::endl; // for debug + testing purposes//std::cout << std::endl << "\033[0;32mkey (" << (char)key << ") inserted at string_index: " << string_index << " = string_len(" << character_data.size() << ") - 1" << "\033[0m" << std::endl;
			#endif
				/////////////////////////////////
	            // get number of parts of the string (length) that have exceeded capacity(50) (or number of times we must split the string)
	            /*int splits_count = character_data.size() / get_capacity(); // (temp.length() % n) != 0 <= to check if not divisible by n beforehand (n = capacity)
                // do not split until characters reach (capacity + 1)
                if(character_data.size() < (get_capacity() * splits_count + 1)) splits_count = splits_count - 1;*/
		        //////////////////////////////////
			    // string position (relative to edit)
                ////int text_x = (characters_count * cursor_space);// = x(140) = characters(14)
			    ////int borderless_text_x = (character_data.size() * cursor_space);// temp.length() should be same as character_data.size()
			    /////////////////////////////////				
			    // set the text manually, rather than calling set_text, to avoid complications//set_text(temp);
			    // if character limit has been reached, resize to whatever the character_limit is
	            ////if(character_data.size() > character_limit) character_data.resize(character_limit); // resize string to whatever the character limit is - Character limit is user-defined
			    // if string has not gone past edit's bounds (width) then draw it as we normally would
			    if(character_data.size() <= get_capacity()) {//if(splits_count < 1) { // same as: if(character_data.size() <= get_capacity()) // if text goes past edit
			        /*// if no splits left, boundless_cursor_x will follow cursor_x
			        boundless_cursor_x = (character_data.size() * cursor_space) - (characters_count * cursor_space) + cursor_x;//boundless_cursor_x = boundless_cursor_x + cursor_space;//(start * cursor_space) + cursor_x;
                    // make sure boundless_cursor_x does not exceed character_data.size()
                    if(boundless_cursor_x >= (character_data.size() * cursor_space)) boundless_cursor_x = character_data.size() * cursor_space;			    
		            *//////////////////////////////
		            // boundless_cursor is the same value as cursor_x when there are no string splits left
		            // move cursor_x 10 units for each character
		            set_cursor_x(cursor_x + cursor_space);
		            boundless_cursor_x = cursor_x;//(character_data.size() * cursor_space) - (characters_count * cursor_space) + cursor_x;//boundless_cursor_x = boundless_cursor_x + cursor_space;
		            /////////////////////////////
		            // update the label when cursor position is changed
		            // "start" position will be 0, since there are 0 string splits
		            start_index = 0;//start;//start + 1; // start should equal zero (hopefully)
		            // restore original min(text.size(), get_capacity(50)) text
		            // show all min(text.size(), get_capacity(50)) characters
		            std::string text = characters_to_string();
		            int n = (text.length() < get_capacity()) ? text.length() : get_capacity();
		            text = text.substr(start_index, n);
		        #ifdef DOKUN_DEBUG0
		            std::cout << "\033[36mtext shifted (right): " << text << " (" << text.size() << ")\033[0m" << std::endl;		            
		            std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;		        
		        #endif    
		            // update the string
		            // only show last (n) characters
		            label->set_string(text);
		            // print cursor_x and boundless_cursor_x
		            /*std::cout << "cursor_x (current): " << cursor_x << std::endl;
		            std::cout << "boundless_cursor_x (current): " << boundless_cursor_x << std::endl;*/
			        return;
			    }
			    if(character_data.size() > get_capacity()) {//if(splits_count > 0) { // same as: if(character_data.size() > get_capacity()) // if text goes past edit
			        // move boundless_cursor_x 10 units for each character
			        boundless_cursor_x = boundless_cursor_x + cursor_space;//set_cursor_x(boundless_cursor_x + text_x);//(text_x);
                    // make sure boundless_cursor_x does not exceed character_data.size()
                    if(boundless_cursor_x >= (character_data.size() * cursor_space)) boundless_cursor_x = character_data.size() * cursor_space;
                    // make sure the cursor does not go past the edit // multiple splits 
                    ////cursor_x = fabs((get_width() * splits_count) - boundless_cursor_x);
		            // if boundless_cursor_x exceeds (capacity*cursor_space), set cursor_x to width(500)
		            // if you want to insert a character in the middle of text then cursor_x must stay still and NOT move at all
		            //////////////////////////////
		            //std::cout << DOKUN_UI_TAG "\033[0;33mCapacity (" <<  get_capacity() << ") has been surpassed by text\033[0m" << std::endl;
		            //////////////////////////////
		            // update the label when cursor position is changed
                    // shift text right by cursor_space(10) units
                    // starting point will be (start - cursor_space)
                    start_index = start_index + 1;//start - 1;//0 + 
                    //if(start_index > start) start_index = start;
                    // get string from start_index to min(end of string or capacity(50))
                    //int start = get_capacity() * splits_count;
                    //start = start_index - start;
                    //std::cout << "numbers of characters from (start_index to start): " << fabs(start) << std::endl;
                    // we want to hide the first character when we shift right
                    std::string text = characters_to_string();
                    int n = (text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();
		            text = text.substr(start_index, n);//(n < text.length()) ? text.substr(text.length() - get_capacity()) : "blank";//text.substr(start_index, n);// comment this out if using: "text.substr(start, characters_count)" below
		        #ifdef DOKUN_DEBUG
		            std::cout << "\033[36mtext shifted (right): " << text << " (" << text.size() << ")\033[0m" << std::endl; 
		            std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;
		            std::cout << "\033[36mn = " << n << "\033[0m" << std::endl;
		        #endif    
		            // this may work for multilined edits
		            //if(multilined)  // if multilined=true, create a new line // a multilined editor
		            //{}		            
		            //// character_data.size() - (get_capacity() * splits_count); // same as characters_count
		            // only show last (characters_count) characters // only show character_count from original start
                    /*// split the string by capacity(50), n amount of times (n being "splits_count")
                    int start = get_capacity() * splits_count; // increase by capacity(50) every n parts
		            int characters_count = character_data.size() - start; // number of characters being drawn at a time
		            text = text.substr(start, characters_count);//text_split = text_split.substr(start, characters_count);
                    *///std::cout << "\033[1;37;49msplits_count: " << splits_count << " (string_length: " << text.length() << ", start: " << start << ", last: " << characters_count << "):\033[0m" << std::endl << text << std::endl;		            
		            ////std::cout << "last (" << characters_count << ") characters: " << text << std::endl;//<< text.substr(/*start_index*/start, characters_count) << std::endl;
		            ////////////////////////////
		            ////if(character_data.size() > character_limit) character_data.resize(character_limit); // resize string to whatever the character limit is - Character limit is user-defined
		            ////////////////////////////
		            // update the label
		            label->set_string(text);//(text_split);
		            ////////////////////////////
		            // print cursor_x and boundless_cursor_x
		            /*std::cout << "cursor_x (current): " << cursor_x << std::endl;
		            std::cout << "boundless_cursor_x (current): " << boundless_cursor_x << " (cursor_x(0) = starting_index * cursor_space)" << std::endl;*/
			        return; // now secret string works :D
			    }
			}
		}
	}
}
/////////////
void Edit::on_backspace() // bug: crashes everytime backspace is used when cursor position is not at the end of text
{
    if(!is_visible() || is_disabled() || !is_active()) return;
    if(readonly) return;
    if(!has_focus()) return;
    if(character_data.empty()) return; // if there are no characters then there is nothing to erase, so return (so it does not crash engine)
#ifdef DOKUN_WIN32
    if(dokun::Keyboard::is_pressed(0x08))
#endif
#ifdef __gnu_linux__
    if(dokun::Keyboard::is_pressed(0xff08))
#endif
	{
        int string_index = (boundless_cursor_x / cursor_space) - 1; // This will give you the character's position in the string (where cursor_x is at) // before deletion // strings are empty at 0, not -1	
	    if(string_index < 0) return;//{ std::cout <<"\033[31mstring index is negative. Exiting on_backspace() ...\033[0m" << std::endl;return;} // if string_index is negative, exit function since we cannot delete a character from a negative string index. String indices start from 0 // a string index at -1 means that the cursor is at position 0 relative to the edit, so it cannot go back any further or else it will go outside the edit's bounds [2021-09-20]
	    // try to get last character behind cursor
	    char last_char = std::get<0>(character_data[string_index]);//label->get_string()[string_index];//String::get_last_character(label->get_string()); // get last character in text (before erasing it) // make sure text is not empty so you can operate on it (to prevent crash) if(get_text().empty()) return;
		// erase last character in string (at a specific index, where the cursor is positioned)
		#ifdef DOKUN_DEBUG0
		    std::cout << "\033[0;91;49mstring_index of deleted character at cursor_x: \033[0m" << last_char << " (" << string_index << ")\033[0m" << std::endl;
		#endif
	        // remove character from label
	        if(sensative) {
	            secret.erase(secret.begin() + string_index); // erase secret string too //
	            ////std::cout << "secret string: " << secret << std::endl;		
		    }
		    // remove deleted character from vector (at a specific index, where the cursor is positioned)
		    character_data.erase(character_data.begin() + string_index); // if string_index=5. deletes 6th element
			// set the text manually, rather than calling set_text, to avoid complications
	        /////////////////////////////////
	        // this is for updated character_data (which has been erased)
	        // get number of parts of the string (length) that have exceeded capacity(50) (or number of times we must split the string)
            int splits_count = character_data.size() / get_capacity();
            // do not split until characters reach (capacity + 1)
            if(character_data.size() < (get_capacity() * splits_count + 1)) splits_count = splits_count - 1;			
			/////////////////////////////////
			// if character limit has been reached, resize to whatever the character_limit is
	        ////if(character_data.size() > character_limit) temp.resize(character_limit); // resize string to whatever the character limit is - Character limit is user-defined
	        // PUT THE CODE BELOW IN A SEPARATE FUNCTION TO MAKE IT NEATER
		    // move_cursor_up() or move_cursor_down()
		    // bring cursor_x back, on character deletion
		    ////set_cursor_x(cursor_x - cursor_space);
			// do same for boundless_cursor_x
			if(splits_count < 1) { // no more splits left // same as: if(character_data.size() <= get_capacity())
                set_cursor_x(cursor_x - cursor_space);
                // if character_data.size() has reached the capacity(50), move cursor_x to end of string (500)
                ////if(character_data.size() == get_capacity()) cursor_x = get_width(); // "(get_capacity() * cursor_space)" should be the same "get_width()" // the width of the edit determines the capacity
			    // cursor_x may be set back to wrong position so correct it
			    if(character_data.size() == get_capacity()) cursor_x = cursor_x + cursor_space;
			    // if no splits left, boundless_cursor_x will follow cursor_x
			    boundless_cursor_x = cursor_x;//(character_data.size() * cursor_space) - (character_data.size() * cursor_space) + cursor_x;//boundless_cursor_x = boundless_cursor_x + cursor_space;//(start * cursor_space) + cursor_x;
                // make sure boundless_cursor_x is NEVER negative, can NEVER be negative anyways
                ////if(boundless_cursor_x <= 0) boundless_cursor_x = 0;                
                ////////////////////////////
                // reset start_index to 0
                start_index = 0;
                // get string from start_index to min(end of string or 50)
                // show all min(text.size(), get_capacity(50)) characters
                std::string text = characters_to_string();
                int n = (text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();   
		        text = text.substr(start_index, n);                
		    #ifdef DOKUN_DEBUG0
		        std::cout << "\033[36mtext shifted (left): " << text << " (" << text.size() << ")\033[0m" << std::endl; 
		        std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;                
            #endif    
                // update the label
                // show only (n) characters
                label->set_string(text);
                ////////////////////////////        
                /*std::cout << std::endl << "cursor_x (on_backspace): " << cursor_x << std::endl;
                std::cout << "boundless_cursor_x (on_backspace): " << boundless_cursor_x << std::endl;*/
			    return;
			}
			if(splits_count > 0) { // multiple splits // same as: if(character_data.size() > get_capacity())
			    boundless_cursor_x = boundless_cursor_x - cursor_space;//set_cursor_x(boundless_cursor_x + text_x);//(text_x);
                // make sure boundless_cursor_x is NEVER negative, can NEVER be negative anyways
                if(boundless_cursor_x <= 0) boundless_cursor_x = 0;
                // assume that the cursor_x has reached 0 or its starting_pos
                // converting boundless cursor_x to bounded cursor_x
                ////if(cursor_x <= 0)  cursor_x = boundless_cursor_x - (get_width() * splits_count);// - cursor_space;//fabs(cursor_x - boundless_cursor_x);//(get_width() * splits_count) - boundless_cursor_x;
                // if you want to delete at middle of string
                // cursor will stay still and NOT move
                ////////////////////////////
                // shift text left by cursor_space(10) units
                // starting point will be (start - cursor_space)
                start_index = start_index - 1;//start - 1;//0 + 
                // get string from start_index to min(end of string or 50)
                // we want to show the previous character when we shift left
                std::string text = characters_to_string();
                int n = (text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();   
		        text = text.substr(start_index, n);
		    #ifdef DOKUN_DEBUG0
		        std::cout << "\033[36mtext shifted (left): " << text << " (" << text.size() << ")\033[0m" << std::endl; 
		        std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;		                          
            #endif    
                ////////////////////////////
                // this may work for multilined edits
                // show only last (character_count) characters
                /*int start = get_capacity() * splits_count; // increase by capacity(50) every n parts
		        int characters_count = character_data.size() - start; // number of characters being drawn at a time
		        // draw text on the newline
		        text = text.substr(start, characters_count);//text_split = text_split.substr(start, characters_count);
                *///std::cout << "\033[1;37;49msplits_count: " << splits_count << " (string_length: " << text.length() << ", start: " << start << ", last: " << characters_count << "):\033[0m" << std::endl << text << std::endl;		            
		        //std::cout << "last (" << characters_count << ") characters: " << text << std::endl;//<< text.substr(/*start_index*/start, characters_count) << std::endl;                
                ////////////////////////////
                // update the label
                label->set_string(text);
                ////////////////////////////
                std::cout << std::endl << "cursor_x (on_backspace2): " << cursor_x << std::endl;
                std::cout << "boundless_cursor_x (on_backspace2): " << boundless_cursor_x << std::endl;         			
			    return;
			}			
	    #ifdef DOKUN_DEBUG0
		    std::cout << "\"" << last_char << "\" (\033[0;91;49mdeleted" << "\033[0m" << " at index: " << string_index << ", ";
		    std::cout << "cursor is now at index: " << (cursor_x / cursor_space) - 1 << ", "; // after deletion // returns "-1" if last_character does not exist (string is empty)
		    std::cout << "characters left: " << ((character_data.empty()) ? 0 : character_data.size()) << ")" << std::endl;
	    #endif
	    ////////////////////////////////////////////////////////////// temp
	    // This deals with cursor_y which is ONLY for multilined edit!
		// if a newline is removed, decrease the size of the edit
		if(last_char == '\n' && multilined) {
		    int char_height = label->get_font()->get_height();
		    set_height(get_height() - char_height);
		    // decrease newlines_count
		    newlines_count = newlines_count - 1;
		    // bring cursor_y back up (if erasing newline)
		    set_cursor_y(cursor_y - char_height);
		}
		// reset cursor position if text is empty
		if(character_data.empty()) set_cursor_position(0, 0);
		// clear character_position_data if text is empty - this may no longer be necessary
		if(character_data.empty()) character_data.clear   ();
	}	
}
/////////////
void Edit::on_enter()
{
    if(!is_visible() || is_disabled() || !is_active()) return;
    if(readonly) return;
    if(!has_focus()) return;    
#ifdef DOKUN_WIN32
    if(dokun::Keyboard::is_pressed(0x0D))
#endif	
#ifdef __gnu_linux__	
	if(dokun::Keyboard::is_pressed(0xff0d))
#endif		
	{
		if(is_multilined()) // a multilined editor
		{
		    int char_height = label->get_font()->get_height();
		    // double the height of the edit
			set_height(get_height() + char_height); // GOOD!
			// add a newline in text
			set_text(label->get_string() + "\n");
			newlines_count = newlines_count + 1;
			// reset cursor x position - try to get location of newline
			set_cursor_x(0);
			// set cursor y position (bring it down)
			set_cursor_y(cursor_y + char_height);
		}
	}	
}
/////////////
// mouse stays within edit's bounds, but keyboard can go past edit's bounds
void Edit::on_arrow_keys() // will shift the text via "start_index"
{
    if(!is_visible() || is_disabled() || !is_active()) return;
    if(readonly) return;
    if(!has_focus()) return;
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_LEFT )) {
        set_cursor_x(cursor_x - cursor_space);
        // IF USER TRIES TO MOVE CURSOR TO -1 OR LESS, IT IS SAFE TO
        // ASSUME THAT THE USER WANTS TO DRAW THE PREVIOUS CHARACTERS (THAT ARE NOT CURRENTLY BEING DRAWN) - ** APPLY THIS TO "on_mouse_press()" AS WELL **
        // move cursor_x to 0 (default position) // move cursor to end of string
        int splits_count = character_data.size() / get_capacity();
        // do not split until characters reach (capacity + 1)
        if(character_data.size() < (get_capacity() * splits_count + 1)) splits_count = splits_count - 1;
        int start = get_capacity() * splits_count;
		int characters_count = character_data.size() - start; // number of characters being drawn at a time
		//std::string temp(characters_to_string());
		//std::string temp_split = temp.substr(start, characters_count);
        // whenever you decrease or increase cursor_x, do the same with boundless_cursor_x
        //if(cursor_x <= 0) {// && boundless_cursor_x > 0) {//cursor_x = 0;//cursor_x = string_length;//{std::cout << "on_arrow_keys(): attempt to reduce cursor_x to negative number (-1)\n";cursor_x = string_length;}
        // cursor_x(0), boundless_cursor_x(500)
        // cursor_x(500), boundless_cursor_x(490)
        // when there are no string splits, then start will be 0
        if(splits_count < 1) { //cursor_x = 0; // no more strings left. This is the final one
            // no more splits left
            // if character_data.size() has reached the capacity(50), cursor should be at end of string (500)
            ////if(character_data.size() == get_capacity()) cursor_x = get_width(); // "(get_capacity() * cursor_space)" should be the same "get_width()" // the width of the edit determines the capacity            
            // if start or splits_count is zero, then boundless_cursor_x should be the same value as cursor_x
            boundless_cursor_x = (character_data.size() * cursor_space) - (characters_count * cursor_space) + cursor_x;//(start * cursor_space) + cursor_x;//cursor_x;
            // make sure boundless_cursor_x is NEVER negative, can NEVER be negative anyways
            if(boundless_cursor_x <= 0) boundless_cursor_x = 0;
            ////////////////////////////        
            std::cout << std::endl << "cursor_x (on_arrow_keys:left): " << cursor_x << std::endl;
            std::cout << "boundless_cursor_x (on_arrow_keys:left): " << boundless_cursor_x << std::endl;            
            return;// exit function so we don't execute another if scope in this function
        }
        // if the string is split into parts or the start is not 0 then it means: 
        // 1. boundless_cursor_x > cursor_x
        // so we will need to reduce the boundless_cursor_x by cursor_space(10)
        if(splits_count > 0) { // there's still more strings left to draw
            // DON'T MOVE THE NORMAL CURSOR_X, KEEP IT AT 0
            // ONLY MOVE THE BORDERLESS_X
            // if(boundless_cursor_x < (start * cursor_space)) {
            // show previous string
            //label->set_string(temp_split);
            boundless_cursor_x = boundless_cursor_x - cursor_space;//(get_capacity() * cursor_space) - cursor_space;//(character_data.size() * cursor_space) - cursor_space;// boundless_cursor_x = temp_split.size() * cursor_space;//0 * cursor_space
            // make sure boundless_cursor_x is NEVER negative, can NEVER be negative anyways
            ////if(boundless_cursor_x <= 0) boundless_cursor_x = 0;                
            // make sure boundless_cursor_x is NEVER less than starting_index
            if(boundless_cursor_x <= (start_index * cursor_space)) boundless_cursor_x = start_index * cursor_space;
            // reset cursor_x (relative to edit) to 500
            // if boundless_cursor_x is less than start(50)
            // assume that the cursor_x has reached 0 or its starting_pos
            // converting boundless to bounded: ??
            ////if(cursor_x <= 0) { cursor_x = boundless_cursor_x - (get_width() * splits_count);//if(cursor_x == 0) cursor_x = fabs(cursor_x - boundless_cursor_x);//(get_width() * splits_count) - boundless_cursor_x;
                // shift text by cursor_space(10) units
                // starting point will be (start - cursor_space)
            ////}
            /*if(start_index <= 0) {start_index = 0; return;}
            ////////////////////////////
            if(cursor_x <= 0) { 
            if(cursor_x == 0) std::cout << std::endl << std::endl << "we've reached the beginning\n";
            // shift the text to the left
            ////////////////////////////
                // shift text left by cursor_space(10) units
                // starting point will be (start - cursor_space)
                start_index = start_index - 1;//start - 1;//0 + 
                // make sure start_index is NEVER negative!
                //if(start_index <= 0) start_index = 0;
                // get string from start_index to min(end of string or 50)
                // we want to show the previous character when we shift left
                std::string text = characters_to_string();
                int n = (text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();   
		        text = text.substr(start_index, n);
		    #ifdef DOKUN_DEBUG    
		        std::cout << "\033[36mtext shifted (left): " << text << " (" << text.size() << ")\033[0m" << std::endl; 
		        std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;		                          
            #endif
                // update text
                label->set_string(text);    
                ////////////////////////////            
            }*/
            ////////////////////////////
            std::cout << std::endl << "cursor_x (on_arrow_keys:left): " << cursor_x << std::endl;
            std::cout << "boundless_cursor_x (on_arrow_keys:left): " << boundless_cursor_x << std::endl;            
            return;
        }
    }
    // right key pressed
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_RIGHT)) {
        set_cursor_x(cursor_x + cursor_space);
        // make sure the cursor does not go past the label// prevent cursor_x from going beyond "end of string"
        int string_length = label->get_string().length();
        ////////////////////////////
        int splits_count = character_data.size() / get_capacity();
        // do not split until characters reach (capacity + 1)
        if(character_data.size() < (get_capacity() * splits_count + 1)) splits_count = splits_count - 1;
        int start = get_capacity() * splits_count;
		int characters_count = character_data.size() - start; // number of characters being drawn at a time
		//std::string temp(characters_to_string());
		//std::string temp_split = temp.substr(start, characters_count);
        // whenever you decrease or increase cursor_x, do the same with boundless_cursor_x
        ////////////////////////////        
        //if(cursor_x <= 0) {// && boundless_cursor_x > 0) {//cursor_x = 0;//cursor_x = string_length;//{std::cout << "on_arrow_keys(): attempt to reduce cursor_x to negative number (-1)\n";cursor_x = string_length;}
        // cursor_x(0), boundless_cursor_x(500)
        // cursor_x(500), boundless_cursor_x(490)
        // when there are no string splits left, then start will be 0
        if(splits_count < 1) { //cursor_x = 0; // no more strings left. This is the final one
            // if start or splits_count is zero, then boundless_cursor_x should be the same value as cursor_x
            boundless_cursor_x = boundless_cursor_x + cursor_space;////boundless_cursor_x = (character_data.size() * cursor_space) - (characters_count * cursor_space) + cursor_x;//(start * cursor_space) + cursor_x;//cursor_x;
            // make sure boundless_cursor_x does not exceed character_data.size()
            if(boundless_cursor_x >= (character_data.size() * cursor_space)) boundless_cursor_x = character_data.size() * cursor_space;
            // make sure the cursor does not go past the edit // zero splits
            if(cursor_x > (string_length * cursor_space)) cursor_x = string_length * cursor_space;
            ////////////////////////////    
            //start_index = start_index + 1;
            //if(start_index >= (character_data.size() - 1)) start_index = character_data.size() - 1;
            ////////////////////////////    
            std::cout << std::endl << "cursor_x (on_arrow_keys:right): " << cursor_x << std::endl;
            std::cout << "boundless_cursor_x (on_arrow_keys:right): " << boundless_cursor_x << std::endl;        
            return; // exit function so we don't execute another if scope of the function
        }
        // if the string is split into parts or the start is not 0 then it means: 
        // 1. boundless_cursor_x > cursor_x
        // so we will need to reduce the boundless_cursor_x by cursor_space(10)
        if(splits_count > 0) { // there's still more strings left to draw
            // show previous string
            //label->set_string(temp_split);
            //cursor_x = get_capacity() * cursor_space;//end_of_character_data * cursor_size 
            boundless_cursor_x = boundless_cursor_x + cursor_space;//(get_capacity() * cursor_space) - cursor_space;//(character_data.size() * cursor_space) - cursor_space;// boundless_cursor_x = temp_split.size() * cursor_space;//0 * cursor_space
            // make sure boundless_cursor_x does not exceed character_data.size()
            if(boundless_cursor_x >= (character_data.size() * cursor_space)) boundless_cursor_x = character_data.size() * cursor_space;
            // make sure the cursor does not go past the edit // multiple splits 
            ////cursor_x = fabs((get_width() * splits_count) - boundless_cursor_x);
            /*////////////////////////////
            if(start_index >= (character_data.size() - 1)) { start_index = character_data.size() - 1;
                return;
            }
            ////////////////////////////
            if(cursor_x >= get_width()) {
                if(cursor_x == get_width()) std::cout << std::endl << std::endl << "we've reached the end\n";
                // shift text 10 units to the right
                start_index = start_index + 1;
                //if(start_index >= (character_data.size() - 1)) start_index = character_data.size() - 1;   
                // get string from start_index to min(end of string or 50)
                // we want to show the previous character when we shift left
                std::string text = characters_to_string();
                int n = (text.length() < get_capacity()) ? text.length() : get_capacity();//get_capacity();   
		        text = text.substr(start_index, n);
		    #ifdef DOKUN_DEBUG    
		        std::cout << "\033[36mtext shifted (right): " << text << " (" << text.size() << ")\033[0m" << std::endl; 
		        std::cout << "\033[36mstarting index: " << start_index << "\033[0m" << std::endl;		                          
            #endif
                // update text
                label->set_string(text);    
                ////////////////////////////              
            }*/
            ////////////////////////////     
            std::cout << std::endl << "cursor_x (on_arrow_keys:right): " << cursor_x << std::endl;
            std::cout << "boundless_cursor_x (on_arrow_keys:right): " << boundless_cursor_x << std::endl;        
            return; // exit function
        }
    }
    //if(dokun::Keyboard::is_pressed(DOKUN_KEY_UP))
    //if(dokun::Keyboard::is_pressed(DOKUN_KEY_DOWN))
}
/////////////
// void Edit::on_left_mouse()
//{
//    // show list
//}
/////////////
void Edit::on_cursor() { // blinking cursor effect
    if(readonly) return; // cursor is disabled for readonly edits
    // we might be able to achieve this by making it fully transparent
    // place this inside loop
    cursor_color.w = cursor_restore_alpha;// restore back to original alpha value //std::cout << "original alpha: " << cursor_restore_alpha << std::endl;
    //std::cout << "Blink timer: " << cursor_blink_timer->increment() << "s" << std::endl;
    if(cursor_blink_timer->increment() >= cursor_blink_interval) // update every x seconds
    {
        //std::cout << cursor_blink_interval << " seconds has passed" << std::endl;
        // hide cursor (by changing cursor transparency)
        cursor_color.w = 0.0;
        // reset timer if x seconds has passed
        cursor_blink_timer->reset();
        // and start the cycle all over again ...
        cursor_blink_timer->start();
    }
}
/////////////
void Edit::on_placeholder() { 
    on_placeholder_text();
    on_placeholder_image();
}
/////////////
void Edit::on_placeholder_text() { // placeholder visibility toggle
    // Draw placeholder_label (whenever edit's primary label string is empty)
	if(!placeholder_label) return; // if no placeholder_text is set, exit function
	if(!label) throw std::runtime_error("label is not initialized");
	if(!label->get_string().empty()) placeholder_label->hide();
	else placeholder_label->show();
}
/////////////
void Edit::on_placeholder_image() {///*
    if(!placeholder_image) return; // if no placeholder_image is set, exit function
    if(!label) throw std::runtime_error("label is not initialized"); // label must be initialized
    int image_width  = placeholder_image->get_width();
	int image_height = placeholder_image->get_height();
	if(placeholder_image->get_alignment() == "left"  ) placeholder_image->set_relative_position(0, (get_height() - image_height) / 2); // center y
	if(placeholder_image->get_alignment() == "center") placeholder_image->set_relative_position((get_width() - image_width) / 2, (get_height() - image_height) / 2); // center both x and y
	if(placeholder_image->get_alignment() == "right" ) placeholder_image->set_relative_position(get_width() - image_width, (get_height() - image_height) / 2); // center y
	placeholder_image->set_position(get_x() + placeholder_image->get_relative_x(), get_y() + placeholder_image->get_relative_y());
	// If placeholder_image is larger than edit then
	// Scale placeholder_image to fit edit
	if(image_width > get_width () || image_height > get_height()) placeholder_image->scale_to_fit(get_width(), get_height());
	// Show or hide placeholder_image
	if(!label->get_string().empty()) placeholder_image->set_visible(false);
	else placeholder_image->set_visible(true);
	//placeholder_image->draw(); // draw in Edit::draw() function instead //*/
}
/////////////
void Edit::on_sensative() { 
    // If not sensative, restore original text and clear secret then exit function
    if(!sensative) {
        if(secret.empty()) return; // was not previously sensative but set_sensative(false) was called :|  (edit set_sensative(false) is called when it is already false)
        // at this point, secret is still normal text
        // sensative is false so get_text will return character_data vector
        // BUT character_data has already changed to asterisks *****
        // replace all asterisks with characters first
        for(int i = 0; i < secret.size(); i++) std::get<0>(character_data[i]) = secret[i];
        label->set_string(secret);// update the label too
        // finally, clear the secret
        secret.clear(); //std::cout << "edit set_sensative(false) called - edit on_sensative(): secret cleared" << std::endl;
        return;
    }
    // If sensative, set the secret and save (store) it only once
    if(sensative) { 
        if(character_data.empty()) return; // If label is empty. No secret to be set, so exit function (a non-empty label means that set_text() was called before set_sensative())
        if(!secret.empty()) return; // secret is not empty, so set_text() might have been called to set the secret instead
        secret = characters_to_string();//get_text();//std::cout << "edit set_sensative(true) called - edit on_sensative(): secret saved: " << secret << std::endl;
        // replace all characters with an asterisk
        std::string temp(secret);
        std::fill_n(temp.begin() + 0, temp.length(), '*');
        // update character information and label
        for(auto& characters : character_data) std::get<0>(characters) = '*';
	    label->set_string(temp); // set the text manually rather than calling set_text, to prevent secret from being saved twice // no need to check the character_limit or set cursor_position since set_text() already did that for us
	    return;
	}
}
/////////////
/////////////
std::string Edit::characters_to_string() const {
    if(character_data.empty()) return ""; // exit, if empty
    std::string text;
    for (auto& characters : character_data) // range-based loops are faster // pass by reference instead of by value (since we are not modifying it)
        text += std::get<0>(characters); //for(int i = 0; i < character_data.size(); i++) full_text += std::get<0>(character_data[i]);
    return text;
}
/////////////
/////////////
// other functions
void Edit::move_cursor_up() {}
/////////////
void Edit::move_cursor_down() {}
/////////////
void Edit::move_cursor_left() {
    
}
/////////////
void Edit::move_cursor_right() {
    
}
/////////////
/////////////
/////////////   
/////////////   
/*void Edit::resize(const Vector2& size)
{
    // everytime the edit's size changes, the cursor positions gets messed up and everything is ruined
    // maybe I should update the text and cursor positions whenever the edit is resized
    int last_width = get_width();
    
    std::string original_text = get_text();
	set_size(size.x, size.y);
	set_text(original_text);
}*/
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

