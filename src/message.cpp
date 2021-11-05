#include "../include/message.hpp"

neroshop::Message::Message()
{
    init();
}
////////////////////
neroshop::Message::Message(const std::string& text) : neroshop::Message()
{
    this->set_text(text);
}
////////////////////
neroshop::Message::Message(const std::string& text, int red, int green, int blue, double alpha) : neroshop::Message()
{
    this->set_text(text, red, green, blue, alpha);
}
////////////////////
neroshop::Message::Message(const std::string& text, std::string color) : neroshop::Message()
{
    this->set_text(text, color);
}
////////////////////
neroshop::Message::~Message()
{/*
    delete box->get_label();
    delete box->get_title_bar_label();
    delete box;*/    
}
////////////////////
Box * neroshop::Message::box (nullptr);
////////////////////
Button * neroshop::Message::ok_button (nullptr);
////////////////////
Button * neroshop::Message::cancel_button (nullptr);
////////////////////
Edit * neroshop::Message::edit (nullptr);
////////////////////
////////////////////
////////////////////
void neroshop::Message::init() 
{
    if(box == nullptr) 
    {
        box = new Box();
        box->set_outline(true);
        //box->set_draggable(true);
        box->set_size(200, 50);
        //box->set_position((window.get_width() / 2) - (box->get_width() / 2), 
        //    (window.get_height() / 2) - (box->get_height() / 2));
        Label * box_label = new Label();
        box->set_label(*box_label);
        box->get_label()->set_alignment("center");
        // title bar
        box->set_title_bar(true);
        box->set_title_bar_color(15, 46, 83, 255);
        // title bar label
        Label * title_label = new Label();
        title_label->set_string("neroshop");
        title_label->set_alignment("center");
        box->set_title_bar_label(*title_label);
        box->hide(); // by default msg_box is hidden        
    }
}
////////////////////
void neroshop::Message::show()
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    // if message is already being showned, create a second instance of message or ?
    box->show();
}
////////////////////
void neroshop::Message::hide()
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    box->hide();
}
////////////////////
void neroshop::Message::draw()
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    on_draw();
    box->draw();
}
////////////////////
void neroshop::Message::center(unsigned int window_width, unsigned int window_height) 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    box->set_position((window_width / 2) - (box->get_width() / 2), (window_height / 2) - (box->get_height() / 2));    
}
////////////////////
void neroshop::Message::restore() 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    box->get_label()->set_string("");
    box->get_label()->set_color(255, 255, 255, 1.0);// restore original color (on hide)
    box->get_label()->set_alignment("center"); // restore alignment
    box->set_size(200, 50); // restore original size
    // hide all buttons, edits, etc.
    if(ok_button) ok_button->set_visible(false);
    if(cancel_button) cancel_button->set_visible(false);
    if(edit) edit->set_visible(false);
}
////////////////////
void neroshop::Message::add_button(const std::string& text) {
    if(String::lower(text) == "ok") {
        if(ok_button != nullptr) return; // cannot initialize button more than once
		// ok_button
		ok_button = new Button();// ok button // delete ok_button
		ok_button->set_size(50, 30);//cancel_button=150, 30//ok_button->set_color(192, 192, 192);
		Label * ok_label = new Label("OK"); // delete ok_label
		ok_label->set_alignment("center");
		ok_button->set_label(*ok_label);// ok_button->set_relative_position(message_edit->get_relative_x() + message_edit->get_width() + 10, message_edit->get_relative_y());
		ok_button->hide();
		// add ok_button
        box->add(*ok_button);
    }
    if(String::lower(text) == "cancel" && !edit) { // edit and cancel_button cannot be on message box at same time
        if(cancel_button != nullptr) return; // cannot initialize button more than once
        box->add(*cancel_button);
    }
}
////////////////////
void neroshop::Message::add_edit() {
    if(edit != nullptr) return; // cannot initialize edit more than once
    if(!ok_button) { std::cout << "cannot have edit without an 'ok' or 'submit' button, so call 'Message::add_button(\"ok\")' before calling Message::add_edit()" << std::endl; return;}
	if(cancel_button) cancel_button->hide(); // hide cancel button
	// edit - requires an ok_button as well
	edit = new Edit(); // delete
	edit->set_size(300, 30);
	edit->set_character_limit(256);
	Label * edit_label = new Label(); // delete // free(): invalid pointer - if not on heap
	edit->set_label(*edit_label); // set label to prevent crash
	edit->hide();
	// adjust size of message_box so it can fit everything
	box->set_height(150);
	box->set_alignment("none");
	box->set_relative_position(96, 50); // 96, 70
	// add edit
    box->add(*edit);
}
////////////////////
void neroshop::Message::set_text(const std::string& text) 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    box->get_label()->set_string(text);
    // adjust box width based on label width
    box->set_width(box->get_label()->get_width() * 1.5);
}
////////////////////
void neroshop::Message::set_text(const std::string& text, int red, int green, int blue, double alpha) 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    box->get_label()->set_string(text);
    // set text color
    box->get_label()->set_color(red, green, blue, alpha);
    // adjust box width based on label width
    box->set_width(box->get_label()->get_width() * 1.5);
}
////////////////////
void neroshop::Message::set_text(const std::string& text, std::string color) 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
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
    box->set_width(box->get_label()->get_width() * 1.5);
}
////////////////////
void neroshop::Message::set_title(const std::string& title) 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    box->get_title_bar_label()->set_string(title);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
Box * neroshop::Message::get_box() {
    return box;
}
////////////////////
Button * neroshop::Message::get_button(const std::string& text) {
    if(String::lower(text) == "ok") return ok_button;
    if(String::lower(text) == "cancel") return cancel_button;
    return ok_button;
}
////////////////////
Edit * neroshop::Message::get_edit() {
    return edit;
}
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::Message::is_visible() 
{
    if(box == nullptr) throw std::runtime_error("message box is not initialized");
    return box->is_visible();
}
////////////////////
////////////////////
////////////////////
////////////////////
void neroshop::Message::on_draw() { // call this function BEFORE calling draw()
	//if(Keyboard::is_pressed(DOKUN_KEY_ESCAPE) && !Message::is_visible()) {
	    // do you wish to exit the program
		//window.destroy();
	//}		    
	if(Keyboard::is_pressed(DOKUN_KEY_ESCAPE)) Message::hide();
	if(!Message::is_visible()) Message::restore(); // restore defaults if hidden
}
////////////////////
////////////////////
