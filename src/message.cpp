#include "../include/message.hpp"

neroshop::Message::Message() : box(nullptr), button_list({}), edit_list({}) {//, button0(nullptr), button1(nullptr), button2(nullptr), edit0(nullptr), edit1(nullptr) {
    initialize();
    // reserve space in vector
    button_list.reserve(4);
    edit_list.reserve(2);
    if(!first) {first = this; std::cout << "message_box singleton created\n";}
    if((first != this) && !second) {second = this; std::cout << "message_box doubleton created\n";}
}
////////////////////
neroshop::Message::Message(const std::string& text) : neroshop::Message() {
    set_text(text);
}
////////////////////
neroshop::Message::Message(const std::string& text, int red, int green, int blue, double alpha) : neroshop::Message() {
    set_text(text, red, green, blue, alpha);
}
////////////////////
neroshop::Message::Message(const std::string& text, std::string color) : neroshop::Message() {
    set_text(text, color);
}
////////////////////
neroshop::Message::~Message() {
    // delete button(s)
    // delete edit(s)
    destroy_children();
    // delete box
    if(box) {
        delete box;
        box = nullptr;
    }
}
////////////////////
neroshop::Message * neroshop::Message::first(nullptr);
////////////////////
neroshop::Message * neroshop::Message::second(nullptr);
////////////////////
////////////////////
void neroshop::Message::initialize() 
{
    if(box) return; // box must be uninitialized before it can be initialized
    std::cout << "message_box initialized\n";
    // create a message_box - box size set in Message::restore()
    box = new Box();
    //box->set_outline(true);
    box->set_draggable(true);
    box->set_color(167,173,186);//(101,115,126);//(54,69,79);//(112,128,144);//(17,17,24);//(72, 88, 111, 1.0);
    // message_box label - label defaults set in Message::restore()
    dokun::Label * box_label = new dokun::Label();
    box->set_label(*box_label);
    // message_box title bar
    box->set_title_bar(true);
    box->set_title_bar_color(21, 34, 56, 1.0);//(15, 46, 83, 1.0);
    // message_box title bar label
    dokun::Label * title_label = new dokun::Label();
    title_label->set_string("neroshop");
    title_label->set_alignment("center");
    box->set_title_bar_label(*title_label);
    box->hide(); // by default msg_box is hidden
}
////////////////////
void neroshop::Message::show()
{
    if(!box) throw std::runtime_error("message box is not initialized");
    // if message is already being showned, create a second instance of message or ?
    box->show();
    // children have their own visibility so they can be hidden even when box is visible
}
////////////////////
void neroshop::Message::hide()
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->hide();
    // children have their own visibility but are hidden when box is not visible, since they can only appear on top of box
}
////////////////////
void neroshop::Message::draw()
{
    if(!box) throw std::runtime_error("message box is not initialized");
    on_draw();
    box->draw();
    // draw children too
    draw_children();
}
////////////////////
void neroshop::Message::center(unsigned int window_width, unsigned int window_height) 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->set_position((window_width / 2) - (box->get_width() / 2), (window_height / 2) - (box->get_height() / 2));    
}
////////////////////
void neroshop::Message::restore() 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->get_label()->clear();
    box->get_label()->set_color(0, 0, 0, 1.0);//(255, 255, 255, 1.0);//(64, 64, 64, 1.0);//(255, 255, 255, 1.0);// restore original color (on hide)
    box->get_label()->set_alignment("center"); // restore alignment
    box->set_size(1000, 200); // restore original size
    // destroy all buttons, edits, etc. (causes seg fault for some reason :/)
    ////destroy_children();
    // actually, hide them instead (only works now cuz I didn't set_parent to box)
    hide_children();
    //std::cout << "message_box restored\n";       
}
////////////////////
void neroshop::Message::add_button(const std::string& text, int relative_x, int relative_y, int width, int height) {
    if(!box) throw std::runtime_error("message box is not initialized");
    std::shared_ptr<Button> button = std::shared_ptr<Button>(new Button());//Button * button = new Button();
	//std::cout << "button.get() = "<< button.get() << std::endl;
	//std::cout << "button.use_count() = " << button.use_count() << std::endl;    
    //////////////////////////////
    button->set_size(width, height);
    //////////////////////////////
    // button label
    dokun::Label * button_label = new dokun::Label(text);
    button_label->set_alignment("center"); // center
    button->set_label(* button_label); // set
    //////////////////////////////
    // button relative position
    button->set_relative_position(relative_x, relative_y);
    //////////////////////////////
    button_list.push_back(button);
}
////////////////////
void neroshop::Message::add_edit(int relative_x, int relative_y, int width, int height) {
    if(!box) throw std::runtime_error("message box is not initialized");
    std::shared_ptr<Edit> edit = std::shared_ptr<Edit>(new Edit());//Edit * edit = new Edit();
	//std::cout << "edit.get() = "<< edit.get() << std::endl;
	//std::cout << "edit.use_count() = " << edit.use_count() << std::endl;
	//////////////////////////////
	edit->set_character_limit(256);
	edit->set_size(width, height);    
    //////////////////////////////
    // edit label
    dokun::Label * edit_label = new dokun::Label();
	edit_label->set_color(32, 32, 32);
	edit->set_label(* edit_label);
	//////////////////////////////
	// edit relative position
    edit->set_relative_position(relative_x, relative_y);// center the x-axis position
    //////////////////////////////
    // scale to fit
    //if(edit->get_width() > box->get_width()) edit->set_width(box->get_width() - 10);
    //if(edit->get_height() > box->get_height()) edit->set_height(box->get_height() - 10);
    //////////////////////////////
    edit_list.push_back(edit);
}
////////////////////
void neroshop::Message::destroy_children() {
    if(!box) throw std::runtime_error("message box is not initialized");
    if(button_list.empty() && edit_list.empty()) return;
    // SHARED_PTRS ARE AUTOMATICALLY DELETED SO THEY CANNOT BE MANUALLY DELETED
    // its ok to use normal for-loops since I am using less than 10 buttons
    // 1. remove objects from vector
    // 2. delete objects
    /*while(!button_list.empty()) {
        Button * button = button_list.front();
        button_list.erase(button_list.begin());
        if(button) {
            delete button;
            //button = nullptr;
        }
    }*/
    // this causes munmap_chunk(): invalid pointer, segfault, double free or corruption (out), etc.
    /*while(!edit_list.empty()) {
        Edit * edit = edit_list.front();
        edit_list.erase(edit_list.begin());
        if(edit) {
            delete edit;
            edit = nullptr;
        }
    }*/
    /*for(int i = 0; i < edit_list.size(); i++) {
        std::shared_ptr<Edit>//Edit * edit = edit_list[i];
        if(edit) {
            delete edit;
            edit = nullptr;
        }
    }*/
    //button_list.clear();
    //edit_list.clear();
}
////////////////////
void neroshop::Message::draw_children() {
    if(!box) throw std::runtime_error("message box is not initialized");
    if(button_list.empty() && edit_list.empty()) return;
    for(auto buttons : button_list) {
        //if(buttons == nullptr) continue;
        // set position relative to box
        buttons->set_position(box->get_x() + buttons->get_relative_x(), box->get_y() + buttons->get_relative_y());
        // make sure that child cannot go past box bounds
	    if(buttons->get_relative_x() >= (box->get_width() - buttons->get_width())) { 
	        buttons->set_position(box->get_x() + (box->get_width() - buttons->get_width()), buttons->get_y()); 
	        buttons->set_relative_position(box->get_width() - buttons->get_width(), buttons->get_relative_y());
	    }
	    if(buttons->get_x() <= box->get_x()) buttons->set_position(box->get_x(), buttons->get_y());
        if(buttons->get_y() <= box->get_y()) buttons->set_position(buttons->get_x(), box->get_y());
	    if(buttons->get_relative_y() >= (box->get_height() - buttons->get_height())) {
	        buttons->set_position(buttons->get_x(), box->get_y() + (box->get_height() - buttons->get_height()));
	        buttons->set_relative_position(buttons->get_relative_x(), (box->get_height() - buttons->get_height()) );
	    }
        // draw buttons
        buttons->draw();
    }  
    for(auto edits : edit_list) {
        //if(edits == nullptr) continue;
        // set position relative to box
        edits->set_position(box->get_x() + edits->get_relative_x(), box->get_y() + edits->get_relative_y());
        // make sure that child cannot go past box bounds
	    if(edits->get_relative_x() >= (box->get_width() - edits->get_width())) { 
	        edits->set_position(box->get_x() + (box->get_width() - edits->get_width()), edits->get_y());
	        edits->set_relative_position(box->get_width() - edits->get_width(), edits->get_relative_y());
	    }
	    if(edits->get_x() <= box->get_x()) edits->set_position(box->get_x(), edits->get_y());
        if(edits->get_y() <= box->get_y()) edits->set_position(edits->get_x(), box->get_y());
	    if(edits->get_relative_y() >= (box->get_height() - edits->get_height())) {
	        edits->set_position(edits->get_x(), box->get_y() + (box->get_height() - edits->get_height()));
	        edits->set_relative_position(edits->get_relative_x(), (box->get_height() - edits->get_height()));
	    }
        // draw edits        
        edits->draw();
    }
}
////////////////////
void neroshop::Message::hide_children() {
    if(!box) throw std::runtime_error("message box is not initialized");
    if(button_list.empty() && edit_list.empty()) return;
    for(auto buttons : button_list) {
        //if(buttons == nullptr) continue;
        buttons->set_visible(false);
    }
    for(auto edits : edit_list) {
        //if(edits == nullptr) continue;
        edits->set_visible(false);
    }
}
////////////////////
void neroshop::Message::show_children() {
    if(!box) throw std::runtime_error("message box is not initialized");
    if(button_list.empty() && edit_list.empty()) return;
    for(auto buttons : button_list) {
        //if(buttons == nullptr) continue;
        buttons->set_visible(true);
    }
    for(auto edits : edit_list) {
        //if(edits == nullptr) continue;
        edits->set_visible(true);
    }
}
////////////////////
////////////////////
void neroshop::Message::set_text(const std::string& text) 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->get_label()->set_string(text);
    // adjust box width based on label width
    // text_width (based on its length)
    int text_width = box->get_label()->get_string().length() * 10;
    int box_width  = box->get_width();
    // if text_width bypasses box width
    if(text_width >= box->get_width()) {
        // increase box width by (text_width - box_width)
        ////box->set_width(box_width + (text_width - box_width) * 2); // add double the space
        //return; // exit function
    }
    //if(text_width <= box->get_width())
    box->set_width(box->get_label()->get_width() + 100);// * 1.5);//((box->get_label()->get_string().length() / 2) * (10 * 2));//
}
////////////////////
void neroshop::Message::set_text(const std::string& text, int red, int green, int blue, double alpha) 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->get_label()->set_string(text);
    // set text color
    box->get_label()->set_color(red, green, blue, alpha);
    // adjust box width based on label width
    int text_width = box->get_label()->get_string().length() * 10;
    int box_width  = box->get_width();
    // if text_width bypasses box width
    if(text_width >= box->get_width()) {
        // increase box width by (text_width - box_width)
        ////box->set_width(box_width + (text_width - box_width) * 2); // add double the space
        //return; // exit function
    }    
    box->set_width(box->get_label()->get_width() + 100);//* 1.5);
}
////////////////////
void neroshop::Message::set_text(const std::string& text, std::string color) 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->get_label()->set_string(text);
    // set text color
    if(String::lower(color) == "white") box->get_label()->set_color(255, 255, 255, 1.0);
    if(String::lower(color) == "black") box->get_label()->set_color(0, 0, 0, 1.0);
    if(String::lower(color) == "red") box->get_label()->set_color(255, 0, 0, 1.0);
    if(String::lower(color) == "yellow") box->get_label()->set_color(255, 255, 0, 1.0);
    if(String::lower(color) == "lime") box->get_label()->set_color(0, 255, 0, 1.0);
    if(String::lower(color) == "cyan" || String::lower(color) == "aqua") box->get_label()->set_color(0, 255, 255, 1.0);
    if(String::lower(color) == "blue") box->get_label()->set_color(0, 0, 255, 1.0);  
    if(String::lower(color) == "magenta" || String::lower(color) == "fuchsia") box->get_label()->set_color(255, 0, 255, 1.0);
    if(String::lower(color) == "silver") box->get_label()->set_color(192, 192, 192, 1.0);
    if(String::lower(color) == "gray" || String::lower(color) == "grey") box->get_label()->set_color(128, 128, 128, 1.0);
    if(String::lower(color) == "maroon") box->get_label()->set_color(128, 0, 0, 1.0);
    if(String::lower(color) == "olive") box->get_label()->set_color(128, 128, 0, 1.0);
    if(String::lower(color) == "green") box->get_label()->set_color(0, 128, 0, 1.0);
    if(String::lower(color) == "purple" || String::lower(color) == "violet") box->get_label()->set_color(128, 0, 128, 1.0);
    if(String::lower(color) == "teal") box->get_label()->set_color(0, 128, 128, 1.0);
    if(String::lower(color) == "navy") box->get_label()->set_color(0, 0, 128, 1.0);
    if(String::lower(color) == "gold") box->get_label()->set_color(255, 215, 0, 1.0);
    if(String::lower(color) == "royal blue") box->get_label()->set_color(65, 105, 225, 1.0);
    //if(String::lower(color) == "") box->get_label()->set_color(255, 255, 255, 1.0);        
    // adjust box width based on label width
    box->set_width(box->get_label()->get_width() + 100);// * 1.5);
}
////////////////////
void neroshop::Message::set_title(const std::string& title) 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    box->get_title_bar_label()->set_string(title);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
neroshop::Message * neroshop::Message::get_first() {
    return first;
}
////////////////////
neroshop::Message * neroshop::Message::get_second() {
    return second;
}
////////////////////
//Box * neroshop::Message::box() const {
//    return box;
//}
////////////////////
Box * neroshop::Message::get_box() const {
    return box;
}
////////////////////
std::shared_ptr<Button> neroshop::Message::get_button(int index) const {
    if(index > (button_list.size()-1)) throw std::runtime_error(std::string("\033[0;31m") + "neroshop::Message::get_button(int): attempting to access invalid index" + std::string("\033[0m"));
    return button_list[index];
}
////////////////////
std::shared_ptr<Edit> neroshop::Message::get_edit(int index) const {
    if(index > (edit_list.size()-1)) throw std::runtime_error(std::string("\033[0;31m") + "neroshop::Message::get_edit(int): attempting to access invalid index" + std::string("\033[0m"));
    return edit_list[index];//std::move(edit_list[index]);
}
////////////////////
////////////////////
int neroshop::Message::get_button_count() const {
    return button_list.size();
}
////////////////////
int neroshop::Message::get_edit_count() const {
    return edit_list.size();
}
////////////////////
////////////////////
////////////////////
bool neroshop::Message::is_visible() 
{
    if(!box) throw std::runtime_error("message box is not initialized");
    return box->is_visible();
}
////////////////////
void neroshop::Message::on_draw() { // call this function BEFORE calling draw()
	//if(dokun::Keyboard::is_pressed(DOKUN_KEY_ESCAPE) && !Message::is_visible()) {
	    // do you wish to exit the program
		//window.destroy();
	//}		    
	if(dokun::Keyboard::is_pressed(DOKUN_KEY_ESCAPE)) Message::hide();
	if(!Message::is_visible()) Message::restore(); // restore defaults if hidden
}
////////////////////
