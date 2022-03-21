#include "../include/catalog.hpp"

////////////////////
neroshop::Catalog::Catalog() {// list views ALWAYS have 1 column, grid views can have multiple rows and columns
    initialize();
}
////////////////////
neroshop::Catalog::~Catalog() {
}
////////////////////
////////////////////
void neroshop::Catalog::initialize() {
    ////////////////////
    /*// create box
    std::shared_ptr<Box> box = std::make_shared<Box>();
    // store box
    box_list.push_back(box); // box 0
    ////////////////////
    // set initial box properties
    box_list[0]->set_size(width / box_list.size(), height);//height / box_list.size());
    ////////////////////
    */
    // initialize view
    if(view.get()) {neroshop::print("catalog is already initialized", 2);return;} // if view was already initialized then exit function
    view = std::make_shared<Grid>();
    std::cout << "catalog view initialized\n";
    // set initial values // size=400,300;rows=2;columns=3 OR 
    view->set_rows(2);//(6)//(1); // we cannot have columns without rows
    view->set_columns(3);//(5)//(1);
    view->set_gap(5);//view->set_horizontal_gap(5);//view->set_vertical_gap(50);
    view->set_color(160, 160, 160, 1.0);
    view->set_highlight(true); // for testing purposes ////view->set_outline(false);
    view->set_size(215, 210/*240, 280*/);////297, 305//smaller: 231 157//247, 223=4x4////view->set_width(200, 100);
    ////////////////////
    // initialize current page (product page)
    if(current.get()) {neroshop::print("catalog is already initialized", 2);return;}
    current = std::make_shared<Box>();
    current->set_size(500, 500);
    current->hide(); // hide product page by default
    ////////////////////
}
////////////////////
void neroshop::Catalog::update() {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    // update box sizes on call to Catalog::set_box_width and Catalog::set_box_height
	if(view->get_box_list().empty()) return; // 0 rows, exit function
	for(int i = 0; i < view->get_box_list().size(); i++) // block.size() = rows
	{
		for(int j = 0; j < view->get_box_list()[i].size(); j++) {// block[i] = items in block i	
		    view->get_box(i, j)->set_size(view->get_width(), view->get_height());//view->get_box_list()[i][j]->set_(get_());
		}
	}    
}
////////////////////
void neroshop::Catalog::draw() {
    //std::cout << "view_pos: " << view->get_position() << std::endl;
    view->draw();
    current->draw();
}
////////////////////
void neroshop::Catalog::draw(int x, int y) {
    set_position(x, y);
    draw();
}
////////////////////
void neroshop::Catalog::center(int window_width, int window_height) {
    if(!current.get()) throw std::runtime_error("catalog page is not initialized");//set_position((window_width / 2) - (get_width() / 2), (window_height / 2) - (get_height() / 2)); // centers grid
    current->set_position((window_width / 2) - (current->get_width() / 2), (window_height / 2) - (current->get_height() / 2)); // centers current page
}
////////////////////
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
void neroshop::Catalog::populate() {

}
////////////////////
void neroshop::Catalog::fetch_inventory() {
    std::string command = "SELECT * FROM inventory LIMIT $1;"; //WHERE stock_qty > 0;";
    int box_count = view->get_row_count() * view->get_column_count(); // rows x columns
    std::vector<const char *> param_values = { std::to_string(box_count).c_str() };
    PGresult * result = PQexecParams(DB::Postgres::get_singleton()->get_handle(), command.c_str(), 1, nullptr, param_values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print("Catalog::fetch(): No items found", 2);        
        PQclear(result);//DB::Postgres::get_singleton()->finish();//exit(1);
        return; // exit so we don't double free "result"
    }
    int rows = PQntuples(result); // max_rows is limited to number of boxes in the grid
    std::cout << "number of rows (items): " << rows << std::endl;
    for(int i = 0; i < std::min<size_t>(rows, box_count); i++) {
        /*std::cout << PQgetvalue(result, i, 0) << std::endl; // inventory_id
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
        //std::cout << PQgetvalue(result, i, 11) << std::endl; //std::cout << PQgetvalue(result, i, 12) << std::endl;*/
        ///////////////////////////////////////////////////////////////////////////    
	////for(int r = 0; r < view->get_box_list().size(); r++) // block.size() = rows
	////{
		////for(int c = 0; c < view->get_box_list()[r].size(); c++) { // block[r] = items in row r	
            ////std::cout << "grid_box use count: " << view->get_box_list()[r][c].use_count() << std::endl; // 2
            Box * box = view->get_box(i);//(r, c);
            //------------------------------
            // I guess we dont need to use smart pointers all the time, but when creating an object with "new", it should be immediately stored in a smart pointer so that it will be automatically deleted at the appropriate time: https://stackoverflow.com/questions/26473733/using-smart-pointers-as-a-class-member#comment41585405_26473733
            Image * test_image = new Image(Icon::get["paid"]->get_data(), 64, 64, 1, 4); // image is still alive outside of scope :D
            //std::shared_ptr<Image> test_image = std::shared_ptr<Image>(new Image(Icon::get["paid"]->get_data(), 64, 64, 1, 4)); // causes "Floating point exception (core dumped)" error
            //std::shared_ptr<Image> test_image = std::make_shared<Image>(Icon::get["paid"]->get_data(), 64, 64, 1, 4); // image is dead at end of scope :( // "std::make_shared" recommended
            test_image->resize(24, 24);//(32, 32);//
            test_image->set_color(30, 80, 155); //155 or 255
            test_image->set_relative_position(10, 10);
            // heart icon (favorites or wishlist)
            /*Image heart_icon(Icon::get["heart"]->get_data(), 64, 64, 1, 4); // CRASHES HERE
            heart_icon->set_color(128, 128, 128, 1.0);//(224, 93, 93, 1.0);
            heart_icon->resize(24, 24);//(16, 16);
            heart_icon->set_alignment("center");
            Box favorite_button; // place at top right of box
            favorite_button.set_size(32, 32);
            favorite_button.set_color(box->get_color());//(128, 128, 128, 0.5);//1.0);//(255, 255, 255, 1.0)
            favorite_button.set_outline(true);
            favorite_button.set_outline_color(255, 255, 255, 1.0);
            favorite_button.set_relative_position(box->get_width() - favorite_button.get_width() - 10, 10);
            favorite_button.set_image(heart_icon);*/
            //////////////////////////////////////////////////
            // item image
            int item_id = std::stoi(PQgetvalue(result, i, 1)); //std::cout << "item_ids: " << item_id << std::endl;            
            Item item(item_id); // temporary (will die at end of scope)
            Image * item_image = item.get_upload_image();
            if(!item_image) { 
                // try replacing image with a placeholder
                ////item_image = new Image(Icon::get["circle"]->get_data(), 64, 64, 1, 4);
                ////continue; // if no image uploaded for this item, skip it or use a placeholder image?
            }
            if(item_image) {
                //std::cout << "loaded " << item_image->get_file() << std::endl;
                item_image->resize(128, 128);//scale_to_fit(view->get_size());
                item_image->set_alignment("center");
                box->add_image(*item_image);
                // save the item_id as a component so we know which item_id belongs to which box
                box->add_component(* new Component("item_id", static_cast<int>(item_id))); // will be stored as shared_ptr by the entity class
                std::cout << "component item_id(" << box->get_component("item_id") << ") added to box " << i << std::endl;
            } // image not null
            //if(rows < (box_count - 1)) { std::cout << "number of items is less than boxes" << std::endl; break; }
            ////} // db_rows // temp*/
            //------------------------------
            box->add_image(*test_image);
            //box->add_gui(favorite_button);
        ////} // grid (view) columns
    } // grid (view) rows
    ////////////////////
        /*Item item(item_ids[i]); // temporary (will die at end of scope)
            Image * item_image = item.get_upload_image();//.get();
            
    std::cout << "ball file: " << item_image->get_file() << std::endl;
    std::cout << "ball width: " << item_image->get_width() << std::endl;
    std::cout << "ball height: " << item_image->get_height() << std::endl;
    std::cout << "ball channels: " << item_image->get_channel() << std::endl;
        item_image->resize(128, 128);
        std::cout << "ball scaled_size: " << item_image->get_size_scaled() << std::endl;
        item_image->set_alignment("center");
        view->get_box(0, 0)->add_image(*item_image);*/
    ////////////////////
    std::cout << "box_image_count (0, 0): " << view->get_box(0, 0)->get_image_count() << std::endl;
    std::cout << "box_label_count (0, 0): " << view->get_box(0, 0)->get_label_count() << std::endl;
    std::cout << "box_gui_count (0, 0):   " << view->get_box(0, 0)->get_gui_count() << std::endl;
    ////////////////////
    PQclear(result); // free result
}
////////////////////
////////////////////
void neroshop::Catalog::set_position(int x, int y) {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    view->set_position(x, y);
}
////////////////////
void neroshop::Catalog::set_box_width(int box_width) {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    view->set_width(box_width);
    update(); // apply width to all boxes
}
////////////////////
void neroshop::Catalog::set_box_height(int box_height) {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    view->set_height(box_height);
    update(); // apply height to all boxes
}
////////////////////
void neroshop::Catalog::set_box_size(int box_width, int box_height) {
    // make sure width is not greater than window's width//if(width > window_width) width = window_width;
    set_box_width(box_width); // auto updates box sizes
    set_box_height(box_height); // auto updates box sizes
}
////////////////////
void neroshop::Catalog::set_box_size(const Vector2i& box_size) {
    set_box_size(box_size.x, box_size.y); // auto updates box sizes
}
////////////////////
////////////////////
Grid * neroshop::Catalog::get_grid() const {
    return view.get();
}
////////////////////
Box * neroshop::Catalog::get_box(int row, int column) const {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    if(view->get_box_list().empty()) throw std::runtime_error("The catalog is empty!");
    return view->get_block(row, column);
}
////////////////////
Box * neroshop::Catalog::get_current() const {
    return current.get();
}
////////////////////
int neroshop::Catalog::get_x() const {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    return view->get_x();
}
////////////////////
int neroshop::Catalog::get_y() const {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    return view->get_y();
}
////////////////////
Vector2i neroshop::Catalog::get_position() const {
    return Vector2i(get_x(), get_y());
}
////////////////////
int neroshop::Catalog::get_width() const {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    return view->get_full_width();
}
////////////////////
int neroshop::Catalog::get_height() const {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    return view->get_full_height();
}
////////////////////
Vector2i neroshop::Catalog::get_size() const {
    return Vector2i(get_width(), get_height());
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
