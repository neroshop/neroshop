#include "../include/catalog.hpp"

////////////////////
neroshop::Catalog::Catalog() : width(1000), height(800) {//, columns(1), rows(1) { // list views ALWAYS have 1 column, grid views can have multiple rows and columns
    initialize();
}
////////////////////
neroshop::Catalog::~Catalog() {
}
////////////////////
////////////////////
void neroshop::Catalog::initialize() {
    ////////////////////
    // create box
    std::shared_ptr<Box> box = std::shared_ptr<Box>(new Box());
    // store box
    box_list.push_back(box); // box 0
    ////////////////////
    // set initial box properties
    box_list[0]->set_size(width / box_list.size(), height);//height / box_list.size());
    ////////////////////
    
    ////////////////////
}
////////////////////
void neroshop::Catalog::draw() {
    ////////////////////
    // update positions
    for(int i = 0; i < box_list.size(); i++) {//for(auto boxes : box_list) {
        if(box_list[i] == box_list[0]) continue; // skip first box
        Box * prev_box = box_list[i - 1].get(); // get last box
        box_list[i]->set_position(prev_box->get_x() + prev_box->get_width() + 1, prev_box->get_y()); // column
        //if i == max_columns
        //dokun::Window * window = dokun::Window::get_active();
        // int splits_count = width / window->get_client_width() //- box_list[0]->get_x();
        //if(splits_count > 0) start_a_new_row;
        //box_list[i]->set_position(prev_box->get_x(), prev_box->get_y() + prev_box->get_height() + 1); // row            
        // height / rows
    }    
    ////////////////////
    for(auto boxes : box_list) {
        // update sizes - division by 0 causes error: Floating point exception (core dumped)
        int box_width = width / box_list.size(); // single box width (be sure box_list.size() is not zero or you'll get floating point exception error)
        int box_height = height;//height / box_list.size(); // single box height            
        boxes->set_size(box_width, box_height);//std::cout << "boxes heights: " << boxes->get_height() << std::endl;
        // draw box
        boxes->draw();
    }
}
////////////////////
void neroshop::Catalog::draw(int x, int y) {
    set_position(x, y);
    draw();
}
////////////////////
void neroshop::Catalog::center(int window_width, int window_height) {
    if(box_list.empty()) { neroshop::print("The catalog is empty!", 1); return; }
    box_list[0]->set_position((window_width / 2) - (box_list[0]->get_width() / 2), (window_height / 2) - (box_list[0]->get_height() / 2));
}
////////////////////
void neroshop::Catalog::add_box() {
    if(box_list.empty()) { neroshop::print("The catalog is empty!", 1); return; }
    ////////////////////
    // create box
    std::shared_ptr<Box> box = std::shared_ptr<Box>(new Box());
    ////////////////////
    // set box properties
    box->set_size(width / box_list.size(), height);//height / box_list.size());
    // update other boxes too?? - will do this in draw call/loop
    ////////////////////
    //box->set_position()
    ////////////////////
    // store box
    box_list.push_back(box);
}
////////////////////
////////////////////
void neroshop::Catalog::set_position(int x, int y) {
    if(box_list.empty()) { neroshop::print("The catalog is empty!", 1); return; }
    // each box will follow the first box or previous boxes
    box_list[0]->set_position(x, y);
}
////////////////////
void neroshop::Catalog::set_width(int width) {
    this->width = width;
}
////////////////////
void neroshop::Catalog::set_height(int height) {
    this->height = height;
}
////////////////////
void neroshop::Catalog::set_size(int width, int height) {
    if(box_list.empty()) { neroshop::print("The catalog is empty!", 1); return; }
    // make sure width is not greater than window's width//if(width > window_width) width = window_width;
    this->width = width;
    this->height = height;
}
////////////////////
////////////////////
Box * neroshop::Catalog::get_box(int index) const {
    if(index > (box_list.size()-1)) throw std::runtime_error(std::string("\033[0;91m") + "neroshop::Catalog::get_box(int): attempting to access invalid index" + std::string("\033[0m"));
    return box_list[index].get();
}
////////////////////
int neroshop::Catalog::get_x() const {
    if(box_list.empty()) throw std::runtime_error("The catalog is empty!");
    return box_list[0]->get_x();
}
////////////////////
int neroshop::Catalog::get_y() const {
    if(box_list.empty()) throw std::runtime_error("The catalog is empty!");
    return box_list[0]->get_y();
}
////////////////////
Vector2i neroshop::Catalog::get_position() const {
    if(box_list.empty()) throw std::runtime_error("The catalog is empty!");
    return Vector2i(box_list[0]->get_position().x, box_list[0]->get_position().y);
}
////////////////////
int neroshop::Catalog::get_width() const {
    return width;
}
////////////////////
int neroshop::Catalog::get_height() const {
    return height;
}
////////////////////
Vector2i neroshop::Catalog::get_size() const {
    return Vector2i(width, height);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
