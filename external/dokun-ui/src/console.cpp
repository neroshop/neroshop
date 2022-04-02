#include "../include/console.hpp"

Console::Console(void) // 0=terminal_mode (.sh), 1=script_mode (.lua)
{
    mode = 0;
}
/////////////
Console::~Console(void)
{}
/////////////
#ifdef DOKUN_WIN32
HANDLE Console::rHnd (GetStdHandle(STD_INPUT_HANDLE));
/////////////
HANDLE Console::wHnd (GetStdHandle(STD_OUTPUT_HANDLE));
#endif
/////////////
Console * Console::console_ptr (nullptr);
/////////////
void Console::destroy() // destroys console
{
	#ifdef DOKUN_WIN32
        FreeConsole();
    #endif
}
/////////////
int Console::destroy(lua_State *L)
{
	Console::destroy();
	return 0;
}
/////////////
void Console::create() // creates a new console
{
	#ifdef DOKUN_WIN32
	    // don't do anything if we already have a console
    if (GetStdHandle(STD_OUTPUT_HANDLE))
        return;
	    AllocConsole();
	#endif
}
/////////////
int Console::create(lua_State *L)
{
	Console::create();
	return 0;
}
/////////////
void Console::write(const std::string& text) // writes to console window
{
	#ifdef DOKUN_WIN32
	//wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
	//////////////////////////////////////////
    Console::init();
	//////////////////////////////////////////
    // if text (in edit) is empty, exit function (cannot submit empty text)_
    if(text.empty()) return;
    // add / append new label to chat_box (with a copy of edit_label's string)
    if(console_ptr->label_list.size() < console_ptr->label_list.capacity())  // if capacity has not yet been reached
    {
        console_ptr->label_list.push_back(new dokun::Label(text));
    } // label_less
    else if(console_ptr->label_list.size() >= console_ptr->label_list.capacity()) {// capacity has been reached
        // store string that will be discarded (or hidden from chatbox) for later use
        //std::cout << "discarded from chatbox: " << label_list[0]->get_string() << std::endl;
        console_ptr->discarded_string_list.push_back(console_ptr->label_list[0]->get_string());
        // try to change the label string without removing anything
        console_ptr->label_list[0]->set_string( console_ptr->label_list[1]->get_string() ); // first element copies second element's string
        console_ptr->label_list[1]->set_string( console_ptr->label_list[2]->get_string() ); // second element copies third element's string and so on ...
        console_ptr->label_list[2]->set_string( console_ptr->label_list[3]->get_string() );
        console_ptr->label_list[3]->set_string( console_ptr->label_list[4]->get_string() );
        // last element becomes/copies the edit_label's string
        console_ptr->label_list[4]->set_string( text );
        // scrollbar_handle goes the opposite way of the text (text goes up, scrollbar_handle goes down)
        // TIP: scrollbars always start from the top
        //if(discarded_string_list.size() > 0)//if(label_list.size() > label_list.capacity()) // if label capacity is surpassed // lets say the chatbox's height increases
        //{
            //scrollbar->set_handle_size( box->get_height() - edit->get_label()->get_height() ); // reduce handle size by label_height
            //scrollbar->set_value( scrollbar->get_value() + 1 ); // bring scrollbar down as text goes up (increase scrollbar_value) // when a new label is appended to chat box, the scrollbar comes down (but you can scroll up to see the label at the top of the chatbox)  
        //}    
    } // label_greater_or_equal	
}
/////////////
std::string Console::on_enter()
{
    Console::init();
	//////////////////////////////////////////
	std::string output = ""; // command output
    // if text (in edit) is empty, exit function (cannot submit empty text)_
    if(console_ptr->edit->is_empty()) return "";
    // if the "Enter" key is pressed
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_RETURN)) {
         // add / append new label to chat_box (with a copy of edit_label's string)
        if(console_ptr->label_list.size() < console_ptr->label_list.capacity())  // if capacity has not yet been reached
        {
            console_ptr->label_list.push_back(new dokun::Label(console_ptr->edit->get_text()));
            // execute the command
            output = execute(console_ptr->edit->get_text());//console_ptr->edit->get_label()->get_string());
            // save last string in text edit
            console_ptr->cache = console_ptr->edit->get_text();//console_ptr->edit->get_label()->get_string();
            // clear edit_label, and reset cursor_x
            console_ptr->edit->reset();
            // no need to change scrollbar if label has not yet reached its capacity
        } // label_less
        else if(console_ptr->label_list.size() >= console_ptr->label_list.capacity()) {// capacity has been reached
            // store string that will be discarded (or hidden from chatbox) for later use
            //std::cout << "discarded from chatbox: " << label_list[0]->get_string() << std::endl;
            console_ptr->discarded_string_list.push_back(console_ptr->label_list[0]->get_string());
            // try to change the label string without removing anything
            console_ptr->label_list[0]->set_string( console_ptr->label_list[1]->get_string() ); // first element copies second element's string
            console_ptr->label_list[1]->set_string( console_ptr->label_list[2]->get_string() ); // second element copies third element's string and so on ...
            console_ptr->label_list[2]->set_string( console_ptr->label_list[3]->get_string() );
            console_ptr->label_list[3]->set_string( console_ptr->label_list[4]->get_string() );
             // last element becomes/copies the edit_label's string
            console_ptr->label_list[4]->set_string( console_ptr->edit->get_text() );
            // execute the command
            output = execute(console_ptr->edit->get_text());//console_ptr->edit->get_label()->get_string());
            // save last string in text edit
            console_ptr->cache = console_ptr->edit->get_text();//console_ptr->edit->get_label()->get_string();                        
            // clear edit_label, and reset cursor_x
            console_ptr->edit->reset();
            // scrollbar_handle goes the opposite way of the text (text goes up, scrollbar_handle goes down)
            // TIP: scrollbars always start from the top
            if(console_ptr->discarded_string_list.size() > 0)//if(label_list.size() > label_list.capacity()) // if label capacity is surpassed // lets say the chatbox's height increases
            {
                //scrollbar->set_handle_size( box->get_height() - edit->get_label()->get_height() ); // reduce handle size by label_height
                //scrollbar->set_value( scrollbar->get_value() + 1 ); // bring scrollbar down as text goes up (increase scrollbar_value) // when a new label is appended to chat box, the scrollbar comes down (but you can scroll up to see the label at the top of the chatbox)  
            }    
       } // label_greater_or_equal  
   } // enter pressed 
   return output;
}
/////////////
std::string Console::on_enter_code(lua_State *L)
{
    Console::init();
	//////////////////////////////////////////
	std::string output = ""; // command output
    // if text (in edit) is empty, exit function (cannot submit empty text)_
    if(console_ptr->edit->is_empty()) return "";
    // if the "Enter" key is pressed
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_RETURN)) {
         // add / append new label to chat_box (with a copy of edit_label's string)
        if(console_ptr->label_list.size() < console_ptr->label_list.capacity())  // if capacity has not yet been reached
        {
            console_ptr->label_list.push_back(new dokun::Label(console_ptr->edit->get_text()));
            // execute the lua code
            output = code(L, console_ptr->edit->get_text());
            // save last string in text edit
            console_ptr->cache = console_ptr->edit->get_text();//console_ptr->edit->get_label()->get_string();
            // clear edit_label, and reset cursor_x
            console_ptr->edit->reset();
            // no need to change scrollbar if label has not yet reached its capacity
        } // label_less
        else if(console_ptr->label_list.size() >= console_ptr->label_list.capacity()) {// capacity has been reached
            // store string that will be discarded (or hidden from chatbox) for later use
            //std::cout << "discarded from chatbox: " << label_list[0]->get_string() << std::endl;
            console_ptr->discarded_string_list.push_back(console_ptr->label_list[0]->get_string());
            // try to change the label string without removing anything
            console_ptr->label_list[0]->set_string( console_ptr->label_list[1]->get_string() ); // first element copies second element's string
            console_ptr->label_list[1]->set_string( console_ptr->label_list[2]->get_string() ); // second element copies third element's string and so on ...
            console_ptr->label_list[2]->set_string( console_ptr->label_list[3]->get_string() );
            console_ptr->label_list[3]->set_string( console_ptr->label_list[4]->get_string() );
             // last element becomes/copies the edit_label's string
            console_ptr->label_list[4]->set_string(console_ptr->edit->get_text());//console_ptr->edit->get_label()->get_string() );
            // execute the command
            output = execute(console_ptr->edit->get_text());//console_ptr->edit->get_label()->get_string());
            // save last string in text edit
            console_ptr->cache = console_ptr->edit->get_text();//console_ptr->edit->get_label()->get_string();                        
            // clear edit_label, and reset cursor_x
            console_ptr->edit->reset();
            // scrollbar_handle goes the opposite way of the text (text goes up, scrollbar_handle goes down)
            // TIP: scrollbars always start from the top
            if(console_ptr->discarded_string_list.size() > 0)//if(label_list.size() > label_list.capacity()) // if label capacity is surpassed // lets say the chatbox's height increases
            {
                //scrollbar->set_handle_size( box->get_height() - edit->get_label()->get_height() ); // reduce handle size by label_height
                //scrollbar->set_value( scrollbar->get_value() + 1 ); // bring scrollbar down as text goes up (increase scrollbar_value) // when a new label is appended to chat box, the scrollbar comes down (but you can scroll up to see the label at the top of the chatbox)  
            }    
       } // label_greater_or_equal  
   } // enter pressed 
   return output;
}
/////////////
int Console::write(lua_State *L)
{
    //luaL_checktype(L, 1, LUA_TSTRING);
    if(lua_type(L, -1) != LUA_TSTRING) Console::write("");//return;
    Console::write(lua_tostring(L, -1));
	return 0;
}
/////////////
void Console::read()
{
	#ifdef DOKUN_WIN32
	//rHnd = GetStdHandle(STD_INPUT_HANDLE);
	#endif
}
/////////////
int Console::read(lua_State *L)
{
	return 0;
}
/////////////
std::string Console::execute(const std::string& cmd) // executes a command on the terminal, and sends the result to a txt file
{///*
    std::string file_name = ".output.txt";  // file is hidden with "."
    std::system( (cmd + " 2>&1 | tee " + file_name).c_str() ); // redirect output to file // https://askubuntu.com/questions/420981/how-do-i-save-terminal-output-to-a-file : David Foerster
    // open file for input, return string containing characters in the file
    std::ifstream file(file_name);
    return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() } ;
}
/////////////
int Console::execute(lua_State *L)
{
    std::string cmd = lua_tostring(L, -1);
    lua_pushstring(L, Console::execute(cmd).c_str());
    return 1;
}
/////////////
std::string Console::color(const std::string& text, const std::string& foreground_color, const std::string& background_color, std::string format_attribute, std::string reset_attribute)
{   // "\033[0;33;42mI am a Banana!\033[0m" // [0=bold;33=text_color;42m=backgound_color
#ifdef DOKUN_LINUX
    unsigned int foreground_number = 39; // 39=Default
    unsigned int background_number = 49; // 49=Default
    unsigned int format_attribute_number = 0; // 0=Default
    unsigned int reset_attribute_number  = 0; // 0=All
    // format attribute
    if(String::lower(format_attribute) == "default") format_attribute_number = 0;
    if(String::lower(format_attribute) == "bold") format_attribute_number = 1;
    if(String::lower(format_attribute) == "dim") format_attribute_number = 2;
    if(String::lower(format_attribute) == "underlined") format_attribute_number = 3;
    if(String::lower(format_attribute) == "blink") format_attribute_number = 5;
    if(String::lower(format_attribute) == "reverse") format_attribute_number = 7;
    if(String::lower(format_attribute) == "hidden") format_attribute_number = 8;
    // foreground
    if(String::lower(foreground_color) == "default") foreground_number = 39;
    if(String::lower(foreground_color) == "black") foreground_number = 30;
    if(String::lower(foreground_color) == "red") foreground_number = 31;
    if(String::lower(foreground_color) == "green") foreground_number = 32;
    if(String::lower(foreground_color) == "yellow") foreground_number = 33;
    if(String::lower(foreground_color) == "blue") foreground_number = 34;
    if(String::lower(foreground_color) == "magenta") foreground_number = 35;
    if(String::lower(foreground_color) == "cyan") foreground_number = 36;
    if(String::lower(foreground_color) == "light gray") foreground_number = 37;
    if(String::lower(foreground_color) == "dark gray") foreground_number = 90;
    if(String::lower(foreground_color) == "light red") foreground_number = 91;
    if(String::lower(foreground_color) == "light green") foreground_number = 92;
    if(String::lower(foreground_color) == "light yellow") foreground_number = 93;
    if(String::lower(foreground_color) == "light blue") foreground_number = 94;
    if(String::lower(foreground_color) == "light magenta") foreground_number = 95;
    if(String::lower(foreground_color) == "light cyan") foreground_number = 96;
    if(String::lower(foreground_color) == "white") foreground_number = 97;
    // background
    if(String::lower(background_color) == "default") background_number = 49;
    if(String::lower(background_color) == "black") background_number = 40;
    if(String::lower(background_color) == "red") background_number = 41;
    if(String::lower(background_color) == "green") background_number = 42;
    if(String::lower(background_color) == "yellow") background_number = 43;
    if(String::lower(background_color) == "blue") background_number = 44;
    if(String::lower(background_color) == "megenta") background_number = 45;
    if(String::lower(background_color) == "cyan") background_number = 46;
    if(String::lower(background_color) == "light gray") background_number = 47;
    if(String::lower(background_color) == "dark gray") background_number = 100;
    if(String::lower(background_color) == "light red") background_number = 101;
    if(String::lower(background_color) == "light green") background_number = 102;
    if(String::lower(background_color) == "light yellow") background_number = 103;
    if(String::lower(background_color) == "light blue") background_number = 104;
    if(String::lower(background_color) == "light magenta") background_number = 105;
    if(String::lower(background_color) == "light cyan") background_number = 106;
    if(String::lower(background_color) == "white") background_number = 107;
    // reset attribute
    if(String::lower(reset_attribute) == "all") reset_attribute_number = 0;
    if(String::lower(reset_attribute) == "bold") reset_attribute_number = 21;
    if(String::lower(reset_attribute) == "dim") reset_attribute_number = 22;
    if(String::lower(reset_attribute) == "underlined") reset_attribute_number = 24;
    if(String::lower(reset_attribute) == "blink") reset_attribute_number = 25;
    if(String::lower(reset_attribute) == "reverse") reset_attribute_number = 27;
    if(String::lower(reset_attribute) == "hidden") reset_attribute_number = 28;    

    return "\033[" + std::to_string(format_attribute_number) + ";" + std::to_string(foreground_number) + ";" + std::to_string(background_number) + "m" + text + "\033[" + std::to_string(reset_attribute_number) + "m";
#endif    
    return text;
}
/////////////
void Console::init()
{
    if(console_ptr == nullptr) 
    {   // console
        console_ptr = new Console();
        // box
        console_ptr->box = new Box();
        console_ptr->box->set_size(800, 400); // update size in loop (in case of changes to edit size) // 800 is perfect for width
        console_ptr->box->set_color(64, 64, 64, 255);
        console_ptr->box->set_gradient(true);
        // title_bar
        console_ptr->box->set_title_bar(true);
        console_ptr->box->set_title_bar_size(30);
        console_ptr->box->set_title_bar_color(console_ptr->box->get_color());
        // title_bar : labels
        console_ptr->box->set_title_bar_label(*new dokun::Label("Console"));
        console_ptr->box->get_title_bar_label()->set_alignment("center");
        // title_bar : buttons
        console_ptr->box->set_title_bar_button_iconify(true);
        console_ptr->box->set_title_bar_button_maximize(true);
        // outline and other properties
        console_ptr->box->set_outline(true);
        //console_ptr->box->set_draggable(true);
        console_ptr->box->set_position(500/*console_ptr->box->get_x()*/, 720 - console_ptr->box->get_height() - 50 );//console_ptr->box->get_y() + 30);
        // edit
	    console_ptr->edit = new Edit();
	    console_ptr->edit->set_character_limit(console_ptr->box->get_width() / 10);//(50); // dimensions for label is estimated to be 10 //std::cout << "Character limit for console is: " << console_ptr->box->get_width() / 10 << std::endl;
 	    console_ptr->edit->set_size(console_ptr->box->get_width(), 20);
	    //edit->set_position(700, 600);
	    dokun::Label * edit_label = new dokun::Label();
	    // font
	    dokun::Font * font = new dokun::Font(DOKUN_DEFAULT_FONT_PATH);
	    edit_label->set_font(*font); // font will be stored in a unique_ptr
	    edit_label->set_color(49, 39, 19, 255);
	    edit_label->set_relative_position(0, 4);
	    console_ptr->edit->set_label(* edit_label); 
	    // push_back label[0]
	    //console_ptr->label_list.push_back(edit_label);
	    //std::cout << "CONSOLE LABEL_LIST SIZE: " << console_ptr->label_list.size() << std::endl;
        // label_list
        console_ptr->label_list.reserve(console_ptr->box->get_height() / round(console_ptr->edit->get_label()->get_height())); // can only hold (chat_height / height_of_each_label)
        //std::cout << "Labels reserved for chat_box: " << label_list.capacity() << std::endl;
        // scrollbar
        //scrollbar = new Scrollbar();
        //scrollbar->set_size(10, box->get_height());
        //scrollbar->set_handle_size(box->get_height()); // scrollbar is same height as chatbox        
    }
}
/////////////
void Console::draw()
{   
    Console::init();
    // callbacks
    std::string output = Console::on_enter();//(console_ptr->mode == 0) ? Console::on_enter() : Console::on_enter_code(L);
    std::vector<std::string> output_split = String::split(output, "\n"); // split output line by line
    if(!output.empty()) // if output is not empty
    {
        for(int i = 0; i < output_split.size(); i++) 
        {
            Console::write( output_split[i] );
        }
    }    
    // update label_list reserve (in case chatbox's height increases, which means more labels can be appended, or added to it)
    console_ptr->label_list.reserve(console_ptr->box->get_height() / round(/*edit->get_label()->get_height()*/20)  ); //std::cout << "edit_label->get_height() = " << edit->get_label()->get_height() << std::endl; // 19=each glyphs height    
    // update geometry
    console_ptr->edit->set_size(console_ptr->box->get_width(), console_ptr->edit->get_height()); // update size in loop (in case of changes to edit size)
    console_ptr->edit->set_position(console_ptr->box->get_x(), console_ptr->box->get_y() + console_ptr->box->get_height()); // update position in loop (in case of changes to edit position)    
    //scrollbar->set_size(scrollbar->get_width(), box->get_height());
    //scrollbar->set_position(box->get_x() + (box->get_width() - scrollbar->get_width()), box->get_y());//scrollbar->set_relative_position(box->get_width() - scrollbar->get_width(), 0);//scrollbar->set_position(box->get_x() + box->get_width(), 0);
    //scrollbar->set_step( label_list.capacity() + discarded_string_list.size()); // 5 steps    std::cout << "Scrollbar step increased" << ". Total steps: " << scrollbar->get_maximum_value() << std::endl;std::cout << "scrollbar value " << scrollbar->get_value() << std::endl;
    // update character limit    (in case chatbox edit size changes)
    console_ptr->edit->set_character_limit(console_ptr->box->get_width() / 10);
    // hide or show all children depending on box's visibility
    if(!console_ptr->edit_hidden) console_ptr->edit->set_visible( (console_ptr->box->is_iconified() == false) && (console_ptr->box->is_visible() == true) ); // hide edit (if box is iconified or hidden) //console_ptr->scrollbar->set_visible((console_ptr->box->is_iconified() == false));
    // draw edit and box and scrollbar
    console_ptr->edit->draw();
    console_ptr->box->draw (); //if(discarded_string_list.size() > 0) scrollbar->draw(); // draw scrollbar only when box is full of text
    // draw labels
    for(int i = 0; i < console_ptr->label_list.size(); i++) 
    {
        if(i == 0) console_ptr->label_list[0]->set_position( console_ptr->box->get_x() + 1, console_ptr->box->get_y() + (console_ptr->box->get_height() - (console_ptr->label_list[0]->get_height() * console_ptr->label_list.size()) ) ); // first label in list will determine the position of the rest
        if(i != 0) { console_ptr->label_list[i]->set_position( console_ptr->label_list[i - 1]->get_x(), console_ptr->label_list[i - 1]->get_y() + console_ptr->label_list[i - 1]->get_height() ); } // set label position to the position of its previous - height    
        //console_ptr->label_list[i]->set_color(32, 32, 32);
        console_ptr->label_list[i]->set_visible( (console_ptr->box->is_iconified() == false) && (console_ptr->box->is_visible() == true) ); // hide labels (if box is iconified)
        console_ptr->label_list[i]->draw();
    }
}
/////////////
int Console::draw(lua_State *L)
{
    Console::init(); // be sure console has been initialized
    if(console_ptr->mode == 1) Console::draw(L); else Console::draw();
    return 0;
}
/////////////
/////////////
void Console::set_title(std::string title)
{
	#ifdef DOKUN_WIN32
	    SetConsoleTitle(TEXT(title.c_str()) );
	#endif
}
/////////////
int Console::set_title(lua_State *L)
{
	Console::set_title(lua_tostring(L, -1));
	return 0;
}
/////////////
void Console::set_size(int width, int height)
{
#ifdef DOKUN_WIN32
    HWND console = GetConsoleWindow();
    RECT console_rect;
    GetWindowRect(console, &console_rect); //stores the console's current dimensions
    MoveWindow(console, console_rect.left, console_rect.top, width, height, TRUE);	
        //SMALL_RECT console_size = {0, 0, static_cast<SHORT>(width), static_cast<SHORT>(height)};
        //SetConsoleWindowInfo(wHnd, TRUE, &console_size);
#endif	
    Console::init();
    console_ptr->box->set_size(width, height);
}
/////////////
int Console::set_size(lua_State *L)
{
	Console::set_size(static_cast<int>(lua_tonumber(L, -2)), static_cast<int>(lua_tonumber(L, -1)));
	return 0;
}
/////////////
/////////////
std::string Console::code(lua_State *L, const std::string& command) // works
{
	if(luaL_dostring(L, command.c_str()) != 0) lua_error(L);
    return command.c_str();
}
int Console::code(lua_State *L)
{
    lua_pushstring(L, Console::code(L, lua_tostring(L, -1)).c_str());
    return 1;
}
/////////////
void Console::set_mode(int mode) // crashes for some reason
{
    Console::init();
    console_ptr->mode = mode;//this->mode = mode;
}
int Console::set_mode(lua_State *L)
{
    Console::set_mode(lua_tonumber(L, -1));//console_ptr->mode = lua_tonumber(L, -1);//
    return 0;
}
/////////////
void Console::hide_edit()
{
    Console::init();
    console_ptr->edit->set_visible(false);
    console_ptr->edit_hidden = true;
}
int Console::hide_edit(lua_State *L)
{
    Console::hide_edit();
    return 0;
}
/////////////
void Console::show_edit()
{
    Console::init();
    console_ptr->edit->set_visible(true);
    console_ptr->edit_hidden = false;
}
int Console::show_edit(lua_State *L)
{
    Console::show_edit();
    return 0;
}
/////////////
/////////////
void Console::set_position(double x, double y)
{
    Console::init();
    console_ptr->box->set_position(x, y);
}
int Console::set_position(lua_State *L)
{
    Console::set_position(static_cast<int>(lua_tonumber(L, -2)), static_cast<int>(lua_tonumber(L, -1)));
    return 0;
}
/////////////
