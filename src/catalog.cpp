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
    std::shared_ptr<Box> box = std::make_shared<Box>();
    // store box
    box_list.push_back(box); // box 0
    ////////////////////
    // set initial box properties
    box_list[0]->set_size(width / box_list.size(), height);//height / box_list.size());
    ////////////////////
    
    ////////////////////
}
////////////////////
void neroshop::Catalog::update() {
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
    }
}
////////////////////
void neroshop::Catalog::draw() {
    update();
    for(auto boxes : box_list) {
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
    std::shared_ptr<Box> box = std::make_shared<Box>();
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
void neroshop::Catalog::add_contents(int box_index) {
    // first, we must retrieve item information from the database
    /////std::string product_name;
    ////fetch_name(product_name); // maybe set a limit on number of names we can fetch and maybe fetch only the bestselling items
    // then we will add the item's contents (image, name, price, star ratings, etc.) to the catalog
    /*for(auto boxes : box_list) {
        // create content here
        // ...
        // then add the contents to the boxes
        boxes->add_();
    }*/
}
////////////////////
void neroshop::Catalog::fetch_product_data() {
    std::string command = "SELECT * FROM inventory;"; //WHERE stock_qty > 0;";
    std::vector<const char *> param_values = { /*nothing here*/ };
    PGresult * result = PQexec(DB::Postgres::get_singleton()->get_handle(), command.c_str());//PQexecParams(DB::Postgres::get_singleton()->get_handle(), command.c_str(), 1, nullptr, param_values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print("Catalog::fetch(): No items found", 2);        
        PQclear(result);//DB::Postgres::get_singleton()->finish();//exit(1);
        return; // exit so we don't double free "result"
    }
    int rows = PQntuples(result);
    for(int i = 0; i < rows; i++) {
        std::cout << PQgetvalue(result, i, 0) << std::endl; // inventory_id
        std::cout << PQgetvalue(result, i, 1) << std::endl; // item_id
        std::cout << PQgetvalue(result, i, 2) << std::endl; // seller_id
        std::cout << PQgetvalue(result, i, 3) << std::endl; // stock_qty
        std::cout << PQgetvalue(result, i, 4) << std::endl; // seller_price or sales_price (not the same as list_price)
        std::cout << PQgetvalue(result, i, 5) << std::endl; // seller's currency of choice (which will be converted to buyer's native currency at checkout)
        std::cout << PQgetvalue(result, i, 6) << std::endl; // seller_discount
        std::cout << PQgetvalue(result, i, 7) << std::endl; // discount_qty
        std::cout << PQgetvalue(result, i, 8) << std::endl; // discount_times
        std::cout << PQgetvalue(result, i, 9) << std::endl; // discount_expiry
        std::cout << PQgetvalue(result, i, 10) << std::endl; // item_condition 
        //std::cout << PQgetvalue(result, i, 11) << std::endl; //std::cout << PQgetvalue(result, i, 12) << std::endl; // //std::cout << PQgetvalue(result, i, 0) << std::endl; // 
        std::cout << std::endl;
        /*PQgetvalue(result, i, 0); // 
        PQgetvalue(result, i, 0); // 
        PQgetvalue(result, i, 0); // 
        PQgetvalue(result, i, 0); // 
        PQgetvalue(result, i, 0); // */
    }
    PQclear(result); // free result
}
////////////////////
////////////////////
void neroshop::Catalog::set_position(int x, int y) {
    if(box_list.empty()) { neroshop::print("The catalog is empty!", 1); return; }
    // each box will follow the first box or previous boxes
    box_list[0]->set_position(x, y);
    update();
}
////////////////////
void neroshop::Catalog::set_width(int width) {
    this->width = width;
    update();
}
////////////////////
void neroshop::Catalog::set_height(int height) {
    this->height = height;
    update();
}
////////////////////
void neroshop::Catalog::set_size(int width, int height) {
    if(box_list.empty()) { neroshop::print("The catalog is empty!", 1); return; }
    // make sure width is not greater than window's width//if(width > window_width) width = window_width;
    set_width(width); // will also call update()
    set_height(height); // will also call update()
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
