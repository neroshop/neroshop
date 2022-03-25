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
    view->set_rows(2/*3*/);//(6)//(1); // we cannot have columns without rows
    view->set_columns(3);//(5)//(1);
    view->set_gap(5);//view->set_horizontal_gap(5);//view->set_vertical_gap(50);
    view->set_color(160, 160, 160, 1.0);
    view->set_highlight(true); // for testing purposes ////view->set_outline(false);
    set_box_size(250, 300);// also applies size to grid (view) boxes //view->set_size(215, 210/*240, 280*/);////297, 305//smaller: 231 157//247, 223=4x4////view->set_width(200, 100);
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
    // update box sizes on call to Catalog::set_box_width and Catalog::set_box_height and Catalog::set_box_size
	std::vector<std::vector<std::shared_ptr<Box>>> box_list = view->get_box_list_2d();
	if(box_list.empty()) return;// if no rows, exit function
	for(auto rows : box_list) {
	    for(auto boxes : rows) {
	        boxes->set_size(view->get_width(), view->get_height());
	    }
	}
}
////////////////////
void neroshop::Catalog::draw() {
    //std::cout << "view_pos: " << view->get_position() << std::endl;
    // put this in "on_draw" function
	std::vector<std::vector<std::shared_ptr<Box>>> box_list = view->get_box_list_2d();
	if(box_list.empty()) return;// if no rows, exit function
	for(auto rows : box_list) {
	    for(auto boxes : rows) {
	        // update relative positions of box contents
	        // images
	        if(boxes->get_image_count() < 9) continue; // skip boxes with less than 3 image(s)
	        //std::cout << "box image count: " << boxes->get_image_count() << std::endl;
	        ////Image * heart_icon = boxes->get_image(1); // image 2
	        ////Image * product_image = boxes->get_image(2); // image 3
	        ////Image * product_star_0 = boxes->get_image(3); // image 4
	        ////Image * monero_ticker = boxes->get_image(8); // image 9
	        // gui elements (labels included)
	        if(boxes->get_gui_count() < 4) continue; // skip boxes with less than 2 gui(s)
	        //std::cout << "box gui count: " << boxes->get_gui_count() << std::endl;
	        // product name label
	        ////GUI * product_name_label = boxes->get_gui(0);
	        ////product_name_label->set_relative_position(10, product_image->get_relative_y() + product_image->get_height_scaled() + 10);
	        // ratings / reviews label (index 1)
	        ////GUI * ratings_count_label = boxes->get_gui(1);
	        // price label
	        ////GUI * price_label = boxes->get_gui(2);
	        ////price_label->set_relative_position(10, product_star_0->get_relative_y() + product_star_0->get_height_scaled() + 10);
	        // monero ticker
	        ////monero_ticker->set_relative_position(10, price_label->get_relative_y() + price_label->get_height() + 8);
	        // price label xmr
	        ////GUI * price_label_xmr = boxes->get_gui(2); // third gui
	        ////price_label_xmr->set_relative_position();
	        // if favorite button pressed, add item to favorites ...
	        // ...
	        // if item was previously purchased, change verified_purchase icon color
	        // ...
	        // on box press, show current page
	        /*if(boxes->is_pressed()) {
	            // populate page with box item details before we show it
	            int item_id = boxes->get_component("item_id");
	            current->show();
	        }*/
	    }
	}    
    ////////////////
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
// This function will change overtime and will populate the catalog with products such as best sellers, most wished for, etc.
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
    int rows = PQntuples(result); // max_rows is limited to number of boxes in the grid//std::cout << "number of rows (items): " << rows << std::endl;
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
	////for(int r = 0; r < view->get_box_list_2d().size(); r++) // block.size() = rows
	////{
		////for(int c = 0; c < view->get_box_list_2d()[r].size(); c++) { // block[r] = items in row r	
            Box * box = view->get_box(i);//(r, c);
            int item_id = std::stoi(PQgetvalue(result, i, 1)); //std::cout << "item_ids: " << item_id << std::endl;       
            // I guess we dont need to use smart pointers all the time, but when creating an object with "new", it should be immediately stored in a smart pointer so that it will be automatically deleted at the appropriate time: https://stackoverflow.com/questions/26473733/using-smart-pointers-as-a-class-member#comment41585405_26473733
            // verified_purchase_icon - maybe replace this with tickers (e.g "recommended", "new", "best seller"), etc.
            Image * verified_purchase_icon = new Image(Icon::get["paid"]->get_data(), 64, 64, 1, 4); // image is still alive outside of scope :D
            //std::shared_ptr<Image> verified_purchase_icon = std::shared_ptr<Image>(new Image(Icon::get["paid"]->get_data(), 64, 64, 1, 4)); // causes "Floating point exception (core dumped)" error
            //std::shared_ptr<Image> verified_purchase_icon = std::make_shared<Image>(Icon::get["paid"]->get_data(), 64, 64, 1, 4); // image is dead at end of scope :( // "std::make_shared" recommended
            verified_purchase_icon->resize(24, 24);//(32, 32);//
            // check if user has previously purchased this item
            // ...
            verified_purchase_icon->set_color(128, 128, 128, 1.0);//(30, 80, 155); //155 or 255
            verified_purchase_icon->set_relative_position(10, 10);
            box->add_image(*verified_purchase_icon);
            // heart_icon (favorites or wishlist)
            Image * heart_icon = new Image(Icon::get["heart"]->get_data(), 64, 64, 1, 4); // CRASHES HERE
            // check if this item is in user's favorites list
            // ...
            heart_icon->set_color(128, 128, 128, 1.0);//(224, 93, 93, 1.0);
            heart_icon->resize(24, 24);//(16, 16);
            heart_icon->set_relative_position(box->get_width() - heart_icon->get_width_scaled() - 10, 10);
            box->add_image(*heart_icon);
            // product_image (thumbnail)     
            Item item(item_id); // temporary (will die at end of scope)
            Image * product_image = item.get_upload_image(1); // first image is thumbnail
            if(!product_image) product_image = new Image(Icon::get["image_gallery"]->get_data(), 64, 64, 1, 4); // if no image uploaded for this item, use a placeholder image instead
            product_image->resize(128, 128);//scale_to_fit(view->get_size());
            product_image->set_relative_position(box->get_x() + (box->get_width() - product_image->get_width_scaled()) / 2, 50);//box->get_y() + (box->get_height() - product_image->get_height_scaled()) / 2);//set_alignment("center"); // actual relative position is set in the draw call
            box->add_image(*product_image);
            // save the item_id as a component so we know which item_id belongs to which box
            box->add_component(* new Component("item_id", static_cast<int>(item_id))); // will be stored as shared_ptr by the entity class
            std::cout << "component item_id(" << box->get_component("item_id") << ") added to box " << i << std::endl;
            //////////////////////////////////////////////////
            // product name
            std::string product_name = DB::Postgres::get_singleton()->get_text_params("SELECT name FROM item WHERE id = $1;", { std::to_string(item_id) });//std::cout << "product_name: " << product_name << std::endl;
            dokun::Label * product_name_label = new dokun::Label(product_name);
            product_name_label->set_color(32, 32, 32, 1.0);
            product_name_label->set_relative_position(10, product_image->get_relative_y() + product_image->get_height_scaled() + 10);
            box->add_gui(*product_name_label);
            //////////////////////////////////////////////////
            // stars
            int average_stars = item.get_average_stars();
            //std::cout << "average_stars for item(id: " << item_id << "): " << average_stars << std::endl;
            std::vector<Image *> product_stars; // size will be 5    //std::cout << "number of stars: " << product_stars.size() << std::endl;
            for(int i = 0; i < 5; i++) {
                product_stars.push_back(new Image());
                product_stars[i]->load(Icon::get["star"]->get_data(), 64, 64, 1, 4);//"star_half"
                product_stars[i]->resize(20, 20);//(16, 16);
                product_stars[i]->set_color((average_stars > 0) ? 255, 179, 68, 1.0 : 255, 255, 255, 1.0);
                ////product_stars[i]->set_outline(true); // gives the star an illusion of depth
                ////product_stars[i]->set_outline_thickness(0.6);//(1.0);
                product_stars[i]->set_outline_color(230, 136, 0);// shades = rgb(230, 136, 0) = THE perfect outline color, rgb(179, 106, 0) = looks really bad//product_stars[i]->set_outline_threshold(0.0);
                box->add_image(*product_stars[i]);// same as: catalog->get_current()->set_image(*product_stars[0].get(), i); // except that Box::set_image uses insert() rather than push_back(). This is the only difference between Box::add_image and Box::set_image
                if(i == 0) { 
                    product_stars[0]->set_relative_position(10, product_name_label->get_relative_y() + product_name_label->get_height() + 10);//(box->get_width() - (product_stars[0]->get_width_scaled() * 5) - 10, product_name_label->get_relative_y() + (product_name_label->get_height() - product_stars[0]->get_height_scaled()) / 2); // set position of the first star (other stars will follow it)
                    continue; // skip the first star for now
                }
                // update positions of stars
                product_stars[i]->set_relative_position(product_stars[i - 1]->get_relative_x() + product_stars[i - 1]->get_width_scaled() + 1, product_stars[0]->get_relative_y()); // same y_rel_pos as first star
            }            
            // product review label
            int product_ratings_count = DB::Postgres::get_singleton()->get_integer_params("SELECT COUNT(*) FROM item_ratings WHERE item_id = $1", { std::to_string(item_id) });//item.get_ratings_count();
            dokun::Label * product_reviews_label = new dokun::Label("("+std::to_string(product_ratings_count)+")");//(star_ratings > 0) ? std::to_string(star_ratings)+" ratings" : "No ratings yet");
            product_reviews_label->set_color(16, 16, 16, 1.0);
            product_reviews_label->set_relative_position(product_stars[product_stars.size() - 1]->get_relative_x() + product_stars[product_stars.size() - 1]->get_width_scaled() + 1, product_stars[product_stars.size() - 1]->get_relative_y() + (product_stars[0]->get_height_scaled() - product_reviews_label->get_height()) / 2);
            box->add_gui(*product_reviews_label);            
            //////////////////////////////////////////////////
            // price display
            double sales_price = std::stod(PQgetvalue(result, i, 4));//std::cout << "product_price: " << sales_price << std::endl;
            std::string currency = PQgetvalue(result, i, 5);//std::cout << "seller currency: " << currency << std::endl;
            std::string currency_symbol = Converter::get_currency_symbol(currency);
            double sales_price_xmr = Converter::to_xmr(sales_price, currency); // requires libcurl + internet connection (may fail at times) and also slows app launch
            //std::cout << "product_price_xmr: " << String::to_string_with_precision(sales_price_xmr, 12) << std::endl;
            dokun::Label * price_label = new dokun::Label(currency_symbol + String::to_string_with_precision(sales_price, 2) + " " + String::upper(currency));
            price_label->set_color(0, 0, 0, 1.0);
            price_label->set_relative_position(10, product_stars[0]->get_relative_y() + product_stars[0]->get_height_scaled() + 10);//(10, product_name_label->get_relative_y() + product_name_label->get_height() + 15);//box->get_height() - price_label->get_height() - 10);
            box->add_gui(*price_label);
            // cryptocurrency ticker (symbol)
            Image * monero_ticker = new Image(Icon::get["monero_symbol_white"]->get_data(), 64, 64, 1, 4);
            monero_ticker->set_outline(true);
            monero_ticker->set_outline_thickness(0.2);//(1.2);
            monero_ticker->resize(16, 16);
            monero_ticker->set_visible(false);
            monero_ticker->set_relative_position(10, price_label->get_relative_y() + price_label->get_height() + 8);
            box->add_image(*monero_ticker);
            // price display xmr
            dokun::Label * price_label_xmr = new dokun::Label(String::to_string_with_precision(sales_price_xmr, 12) + " XMR");
            if(!monero_ticker->is_visible()) price_label_xmr->set_relative_position(10, price_label->get_relative_y() + price_label->get_height() + 10 );//(monero_ticker->get_relative_x() + monero_ticker->get_width_scaled() + 5, monero_ticker->get_relative_y() + (monero_ticker->get_height_scaled() - price_label->get_height()) / 2);// <= use this if monero_symbol is visible
            if(monero_ticker->is_visible()) price_label_xmr->set_relative_position(monero_ticker->get_relative_x() + monero_ticker->get_width_scaled() + 5, monero_ticker->get_relative_y() + (monero_ticker->get_height_scaled() - price_label->get_height()) / 2);
            price_label_xmr->set_color(0, 0, 0, 1.0);
            box->add_gui(*price_label_xmr);
            //////////////////////////////////////////////////
            // show_product or buy button (not neccessary, but optional since user can just click the box to view the item)
            // ...
        ////} // grid (view) columns
    } // grid (view) rows
    ////////////////////
    // in most cases, the number of rows (products) is greater than the box_count so this "if" scope will be ignored 99% of the time but in case all boxes are not filled with product images, fill the ones that need it
    if(rows < box_count) { // if there are less items than catalog boxes, get the boxes that do need an item and fill them up with placeholder product images
        for(int i = 0; i < box_count; i++) {//std::cout << "box_index(i): " << i << std::endl;
            Box * box = view->get_box(i);
            if(box->get_image_count() > 0) continue; // skip boxes that already have product images
            // product_image (thumbnail)
            Image * product_image = new Image(Icon::get["image_gallery"]->get_data(), 64, 64, 1, 4);
            product_image->resize(128, 128);
            product_image->set_alignment("center");
            box->add_image(*product_image);
        }
    }
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
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    view->set_size(box_width, box_height);
    update(); // update ONLY once after setting both the width and height
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
    if(view->get_box_list_2d().empty()) throw std::runtime_error("The catalog is empty!");
    return view->get_box(row, column);
}
////////////////////
Box * neroshop::Catalog::get_box(int index) const {
    if(!view.get()) throw std::runtime_error("catalog view is not initialized");
    std::vector<std::shared_ptr<Box>> box_list_1d = view->get_box_list_1d();
    if(box_list_1d.empty()) throw std::runtime_error("The catalog is empty!");
    return box_list_1d[index].get();
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
