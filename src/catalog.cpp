#include "../include/catalog.hpp"

////////////////////
neroshop::Catalog::Catalog() : view(nullptr), current(nullptr), tooltip(nullptr) {// list views ALWAYS have 1 column, grid views can have multiple rows and columns
    initialize();
}
////////////////////
neroshop::Catalog::~Catalog() {
    // delete sort_box
    if(sort_box.get()) {
        sort_box.reset();
        if(!sort_box.get()) std::cout << "catalog sort_box deleted\n";
    }
    // delete tooltip
    if(tooltip.get()) { 
        tooltip.reset();
        if(!tooltip.get()) std::cout << "catalog tooltip deleted\n"; // confirm deletion
    }
    // delete page
    if(current.get()) { 
        //delete_page_children();
        current.reset();
        if(!current.get()) std::cout << "catalog page deleted\n"; // confirm deletion
    }    
    // delete view
    if(view.get()) { 
        //delete_view_children();
        view.reset();
    }    
    std::cout << "catalog deleted\n";
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
    view = std::unique_ptr<Grid>(new Grid());
    std::cout << "catalog view initialized\n";
    // set initial values // size=400,300;rows=2;columns=3 OR 
    view->set_rows(2);//(3);//(6)//(1); // we cannot have columns without rows
    view->set_columns(3);//(5)//(1);
    view->set_gap(5);//view->set_horizontal_gap(5);//view->set_vertical_gap(50);
    view->set_color(160, 160, 160, 1.0);
    view->set_highlight(true); // for testing purposes ////view->set_outline(false);
    set_box_size(220, 220);//(220, 250);//(250, 300);//(215, 210);// also applies size to grid (view) boxes
    ////////////////////
    // initialize current page (product page)
    if(current.get()) {neroshop::print("catalog is already initialized", 2);return;}
    current = std::unique_ptr<Box>(new Box());
    setup_page();
    current->hide(); // hide product page by default
    ////////////////////
    // initialize tooltip
    if(tooltip.get()) {neroshop::print("catalog is already initialized", 2);return;}
    tooltip = std::unique_ptr<Box>(new Box());
    tooltip->set_tooltip(true);
    tooltip->set_color(10, 10, 10);//, 1.0);//tooltip->set_tooltip_arrow_direction("down");
    // outline won't show unless it has a radius :U
    //tooltip->set_radius(10);
    //tooltip->set_outline(true); // hint is a box, not a tooltip so why is the outline missing?
    //tooltip->set_outline_color(255, 255, 255, 1.0);
    tooltip->add_label(*new dokun::Label("")); // font will be auto set when label is initialized with a string
    tooltip->hide(); // hide tooltip by default
    ////////////////////
    // initialize sorting and filtering box
    if(sort_box.get()) {neroshop::print("catalog is already initialized", 2);return;}
    sort_box = std::unique_ptr<Box>(new Box());
    // add labels, check boxes, radios, etc.
    // department or category
    // release date: newer, older
    // brand: nintendo, playstation, xbox
    // [BOLD]color[BOLD]
      // [] red
      // [] green
      // [] blue
      // [] any
    // size
    // type
    // ratings: 5, 4, 3, 2, 1 stars
    // condition: new, used
    // price: <$10, <$20, <$30, <$40
    // deals
    // availability: include out of stock [x]
    sort_box->hide(); // only show when user looks up an item using the search bar
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
void neroshop::Catalog::delete_view_children() {
    //std::vector<std::vector<std::shared_ptr<Box>>> box_list = view->get_box_list_2d();
	if(view->get_box_list_2d().empty()) return;//if(box_list.empty()) return;// if no rows, exit function
	for(auto rows : view->get_box_list_2d()) {
	    for(auto boxes : rows) {
	        // delete (reset) box children
	        for(auto box_guis : boxes->get_gui_list()) {
	            if(box_guis.use_count() > 0) box_guis.reset();
	        }
	        for(auto box_images : boxes->get_image_list()) {
	            if(box_images.use_count() > 0) box_images.reset();
	        }
	        ////if(boxes.use_count() > 0) boxes.reset(); // delete (reset) boxes (will also reset children)
	    }
	}
}
////////////////////
void neroshop::Catalog::delete_page_children() {
    // delete (reset) page children
    for(auto page_guis : current->get_gui_list()) {
        if(page_guis.use_count() > 0) page_guis.reset();
    }
    for(auto page_images : current->get_image_list()) {
        if(page_images.use_count() > 0) page_images.reset();
    }   
    // delete (reset) the page itself (will also reset children)
    ////if(current.get()) current.reset();
}
////////////////////
void neroshop::Catalog::draw() {
    //std::cout << "view_pos: " << view->get_position() << std::endl;
    tooltip->hide(); // hide tooltip by default, unless mouse is over icon
    // put this in "on_draw" function
	std::vector<std::vector<std::shared_ptr<Box>>> box_list = view->get_box_list_2d();
	if(box_list.empty()) return;// if no rows, exit function
	for(auto rows : box_list) {
	    for(auto boxes : rows) {
	        // so long as product page is visible, grid boxes cannot be active or maybe I could hide the grid boxes?
	        if(current->is_visible()) { /*view->hide();*/ boxes->set_active(false); } else { /*view->show();*/ boxes->set_active(true); }
	        // update relative positions of box contents
	        // images
	        if(boxes->get_image_count() < 9) continue; // skip boxes with less than 3 image(s)
	        //std::cout << "box image count: " << boxes->get_image_count() << std::endl;
	        Image * verified_purchase_icon = boxes->get_image(0); // image 1
	        Image * heart_icon = boxes->get_image(1); // image 2
	        ////Image * product_image = boxes->get_image(2); // image 3
	        ////Image * product_star_0 = boxes->get_image(3); // image 4
	        ////Image * product_star_1 = boxes->get_image(4); // image 5
	        ////Image * product_star_2 = boxes->get_image(5); // image 6
	        ////Image * product_star_3 = boxes->get_image(6); // image 7
	        ////Image * product_star_4 = boxes->get_image(7); // image 8
	        ////Image * monero_ticker = boxes->get_image(8); // image 9
	        ///////////////////// temporary code below
	        // WORKS! :D
	        // set stars' color on mouse hover
	        /*std::vector<Image *> product_stars = {boxes->get_image(3),boxes->get_image(4),boxes->get_image(5),boxes->get_image(6),boxes->get_image(7)};
	        for(int i = 0; i < 5; i++) {
	        if(Mouse::is_over(product_stars[i]->get_x(), product_stars[i]->get_y(), product_stars[i]->get_width(),  product_stars[i]->get_height() )) {
                  std::cout << "mouse is pointing at star: " << i << std::endl;
                  int selected_star_index = i;
                  // set color of current star
                  product_stars[i]->set_color(255, 179, 68, 1.0);
                  // then set the color of the stars before it
                  for(int j = i; j >= 0; j--) {
                      std::cout << "j: " << j << std::endl;
                      product_stars[j]->set_color(255, 179, 68, 1.0);
                  }
              } else product_stars[i]->set_color(255, 255, 255, 1.0); // restore all stars original color
              } // for loop*/
              ////////////////////// temporary code above
	        // gui elements (labels included)
	        if(boxes->get_gui_count() < 4) continue; // skip boxes with less than 2 gui(s)
	        //std::cout << "box gui count: " << boxes->get_gui_count() << std::endl;
	        // product name label
	        ////GUI * product_name_label = boxes->get_gui(0);
	        ////product_name_label->set_relative_position(10, product_image->get_relative_y() + product_image->get_height() + 10);
	        // ratings / reviews label (index 1)
	        ////GUI * ratings_count_label = boxes->get_gui(1);
	        // price label
	        ////GUI * price_label = boxes->get_gui(2);
	        ////price_label->set_relative_position(10, product_star_0->get_relative_y() + product_star_0->get_height() + 10);
	        // monero ticker
	        ////monero_ticker->set_relative_position(10, price_label->get_relative_y() + price_label->get_height() + 8);
	        // price label xmr
	        ////GUI * price_label_xmr = boxes->get_gui(2); // third gui
	        ////price_label_xmr->set_relative_position();
	        // if mouse is over favorite button, show tooltip ...
	        if(Mouse::is_over(verified_purchase_icon->get_rect()) && verified_purchase_icon->is_visible() && boxes->is_active()) { // && verified_purchase_icon->is_active()) {
	            std::string message = "You've previously purchased this item";
	            tooltip->get_label(0)->set_string(message);
	            tooltip->get_label(0)->set_alignment("center");
	            tooltip->set_size(tooltip->get_label(0)->get_width() + 20, 50);
	            tooltip->set_position(verified_purchase_icon->get_x() + (verified_purchase_icon->get_width() - tooltip->get_width()) / 2, verified_purchase_icon->get_y() - (tooltip->get_height() + tooltip->get_tooltip_arrow_height()) - 5/*, verified_purchase_icon->get_y() + verified_purchase_icon->get_height()*/);//above icon//under icon
	            tooltip->show();
	        }
	        // if mouse is over verified_purchase icon, show tooltip
	        if(Mouse::is_over(heart_icon->get_rect()) && heart_icon->is_visible() && boxes->is_active()) {
	            std::string message = "You've favorited this item";
	            tooltip->get_label(0)->set_string(message);
	            tooltip->get_label(0)->set_alignment("center");
	            tooltip->set_size(tooltip->get_label(0)->get_width() + 20, 50);
	            tooltip->set_position(heart_icon->get_x() + (heart_icon->get_width() - tooltip->get_width()) / 2, heart_icon->get_y() - (tooltip->get_height() + tooltip->get_tooltip_arrow_height()) - 5);	            
	            tooltip->show();
	        }
	        // on box product_image press, show current page
	        if(Mouse::is_over(boxes->get_image(2)->get_rect()) && Mouse::is_pressed(1)) {//if(boxes->is_pressed()) {
	            // populate page with box item details before we show it
	            int item_id = boxes->get_component("item_id")->to_integer();
	            std::cout << "box with item_id(" << item_id << ") is pressed\n";
	            update_page(item_id); // will update product_image too
	            current->show();
	        }
	    }
	}    
    /////////////////////////////////
    // set filter box size and position
    sort_box->set_size(sort_box->get_width(), view->get_full_height());
    sort_box->set_position(50, view->get_y());
    /////////////////////////////////
    int window_width = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0))->get_client_width();
    int window_height = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0))->get_client_height();
    current->set_width(window_width - 40); // edge must line up with cart_button's edge
    current->set_height(window_height - 100 - 10); // - 10 to not block the date display and sync status label
    current->set_position((window_width / 2) - (current->get_width() / 2), view->get_y() - 10);//90// - 10 to cover the grid outline
    // if ESCAPE is pressed, hide the product page
    if(dokun::Keyboard::is_pressed(DOKUN_KEY_C/*DOKUN_KEY_ESCAPE*/)) current->hide();
    /////////////////////////////////
    view->draw();
    tooltip->draw();
    sort_box->draw();
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
void neroshop::Catalog::setup_page() {
    // 3 pages: 
    // 1. catalog view (contains only the product image, name, and price)
    // 2. product page (contains product image, name, price, stars, favorite_button and info_iconcart_add_button, quantity_spinner, etc.)
    // 3. cart / checkout page
    // product_page ------------------------------------------------------------------
    // PRODUCT IMAGE BORDER BOX
    /*Box * product_image_border_box = new Box();
    product_image_border_box->set_outline(true);
    product_image_border_box->set_size(500, 500);
    product_image_border_box->set_relative_position(10, 10);
    // PRODUCT IMAGE
    Image * product_image = new Image(Icon::get["image_gallery"]->get_data(), 64, 64, 1, 4);
    ////product_image->set_relative_position((product_image_border_box->get_width() - product_image->get_width()) / 2, (product_image_border_box->get_height() - product_image->get_height()) / 2);//product_image->set_relative_position((current->get_width() - product_image->get_width()) / 2, 10);//(10, 10);
    // dont resize product_image unless it exceeds maximum image size limits or is below minimum image size limits
    product_image_border_box->add_image(*product_image);
    current->add_gui(*product_image_border_box);/////current->add_image(*product_image);
    // MORE PRODUCT IMAGE BORDER BOXES
    std::vector<Box *> product_image_border_boxes = {};
    product_image_border_boxes.push_back(new Box());
    product_image_border_boxes.push_back(new Box());
    int box_gap = 10;
    // MORE PRODUCT IMAGES 
    //std::vector<Image *> product_images;
    //product_images.push_back(new Image());
    ////product_image_border_boxes[0]->set_relative_position(10, product_image_border_box->get_relative_y() + product_image_border_box->get_height() + box_gap);
    for(int i = 0; i < product_image_border_boxes.size(); i++) {
        product_image_border_boxes[i]->set_outline(true);
        product_image_border_boxes[i]->set_size((product_image_border_box->get_width() - box_gap) / product_image_border_boxes.size(), 128);
    
        current->add_gui(*product_image_border_boxes[i]);
        if(i == 0) continue; // skip first product image border sub box
        ////product_image_border_boxes[i]->set_relative_position(product_image_border_boxes[i - 1]->get_relative_x() + product_image_border_boxes[i - 1]->get_width() + box_gap, product_image_border_boxes[0]->get_relative_y());
    }*/
    // NAVIGATION ARROWS (add to main product_image_border_box)
    /*// PRODUCT NAME LABEL
    dokun::Label product_name_label("Monero Ball"); // place at bottom of image or next to image
    product_name_label.set_color(0, 0, 0, 1.0);
    product_name_label.set_relative_position(10, product_image->get_relative_y() + product_image->get_height() + 10);//((current->get_width() - product_name_label.get_width()) / 2, product_image->get_relative_y() + product_image->get_height() + 10);//(product_image->get_relative_x(), product_image->get_relative_y() + product_image_height + 10);
    //////current->add_gui(product_name_label);
    // BRAND NAME LABEL
    // ...
    // STARS (FROM THE CALCULATED AVERAGE STARS - 5)
    std::vector<std::shared_ptr<Image>> product_stars; // size will be 5    //std::cout << "number of stars: " << product_stars.size() << std::endl;
    product_stars.push_back(std::make_shared<Image>()); // 0
    product_stars.push_back(std::make_shared<Image>()); // 1
    product_stars.push_back(std::make_shared<Image>()); // 2
    product_stars.push_back(std::make_shared<Image>()); // 3
    product_stars.push_back(std::make_shared<Image>()); // 4
    for(int i = 0; i < 5; i++) {
        // load stars
        product_stars[i]->load(Icon::get["star"]->get_data(), 64, 64, 1, 4);//"star_half"
        product_stars[i]->resize(20, 20);//(16, 16);
        product_stars[i]->set_color(255, 179, 68, 1.0);
        product_stars[i]->set_outline(true); // gives the star an illusion of depth
        product_stars[i]->set_outline_thickness(0.6);//(1.0);
        product_stars[i]->set_outline_color(230, 136, 0);// shades = rgb(230, 136, 0) = THE perfect outline color, rgb(179, 106, 0) = looks really bad//product_stars[i]->set_outline_threshold(0.0);
        //////current->add_image(*product_stars[i].get());// same as: current->set_image(*product_stars[0].get(), i); // except that Box::set_image uses insert() rather than push_back(). This is the only difference between Box::add_image and Box::set_image
        if(i == 0) { 
            product_stars[0]->set_relative_position(product_name_label.get_relative_x(), product_name_label.get_relative_y() + product_name_label.get_height() + 10);//5); // set position of the first star (other stars will follow it)
            continue; // skip the first star for now
        }
        // update positions of stars
        product_stars[i]->set_relative_position(product_stars[i - 1]->get_relative_x() + product_stars[i - 1]->get_width() + 1, product_stars[0]->get_relative_y()); // same y_rel_pos as first star
    }
    // STARS LABEL (FROM THE CALCULATED AVERAGE STARS)
    dokun::Label product_reviews_label("(0 ratings)");//(star_ratings > 0) ? std::to_string(star_ratings)+" ratings" : "No ratings yet");
    product_reviews_label.set_color(16, 16, 16, 1.0);
    //int last_star_width = (product_stars[product_stars.size() - 1]->is_resized()) ? product_stars[product_stars.size() - 1]->get_width() : product_stars[product_stars.size() - 1]->get_width();
    //int star_height = (product_stars[0]->is_resized()) ? product_stars[0]->get_height() : product_stars[0]->get_height();//std::cout << "star_height: " << star_height << std::endl;
    product_reviews_label.set_relative_position(product_stars[product_stars.size() - 1]->get_relative_x() + product_stars[product_stars.size() - 1]->get_width() + 1, product_stars[product_stars.size() - 1]->get_relative_y() + (product_stars[0]->get_height() - product_reviews_label.get_height()) / 2);
    //////current->add_gui(product_reviews_label);
    // PRICE LABEL
    dokun::Label price_label("$" + String::to_string_with_precision(0.00, 2)); // size should be made large to emphasize price
    price_label.set_color(32, 32, 32, 1.0);
    price_label.set_scale(1.2, 1.2);
    price_label.set_relative_position(product_stars[0]->get_relative_x(), product_stars[0]->get_relative_y() + product_stars[0]->get_height() + 10);
    //////current->add_gui(price_label);//add_gui//add_label    
    // COLOR PALETTE - UP TO 12
    // TYPE/MODEL, SIZE COMBOBOX
    // PACKAGING/QUANTITY - SPINNER BOX - THIS SHOULD BE FOR WHEN CHECKING OUT AS WELL
    dokun::Label quantity_spinner_label;
    quantity_spinner_label.set_font(*dokun::Font::get_system_font());
    quantity_spinner_label.set_color(0, 0, 0, 1.0);
    Spinner quantity_spinner;
    quantity_spinner.set_range(1, 100);
    quantity_spinner.set_color(128, 128, 128, 0.0); // full transparency
    quantity_spinner.set_button_color(64, 64, 64, 1.0);
    //quantity_spinner.set_shape_color(255, 102, 0, 1.0); // cyan :O
    //quantity_spinner.set_separator(true);
    //quantity_spinner.set_separator_size(5);        
    quantity_spinner.set_label(quantity_spinner_label);
    //////current->add_gui(quantity_spinner);
    // TRASHCAN ICON + BUTTON - FOR REMOVING ITEM FROM CART (this will only be shown in the cart menu, not in the catalogue)
    // INFO ICON - TO GET DETAILS ABOUT A PRODUCT (place at left side of box opposite to heart icon)
    Image info_icon(Icon::get["info_colored"]->get_data(), 64, 64, 1, 4);
    info_icon.resize(24, 24);//(32, 32);
    //info_icon.set_color(30, 80, 155); //155 or 255
    info_icon.set_relative_position(10, 10);
    //////current->add_image(info_icon);
    // HEART ICON + BUTTON (FAVORITES / WISHLIST) - add white outline to button 
    Image heart_icon(Icon::get["heart"]->get_data(), 64, 64, 1, 4); // CRASHES HERE
    heart_icon.set_color(128, 128, 128, 1.0);//(224, 93, 93, 1.0);
    heart_icon.resize(24, 24);//(16, 16);
    heart_icon.set_alignment("center");*/
    /////*Button*/Box favorite_button; // place at top right of box
    /*favorite_button.set_size(32, 32);//(24, 24);
    favorite_button.set_color(current->get_color());//(128, 128, 128, 0.5);//1.0);//(255, 255, 255, 1.0)
    favorite_button.set_outline(true);
    favorite_button.set_outline_color(255, 255, 255, 1.0);
    favorite_button.set_relative_position(current->get_width() - favorite_button.get_width() - 10, 10);
    favorite_button.set_image(heart_icon);
    //////current->add_gui(favorite_button);//current->set_();    
    // BELL ICON + BUTTON - FOR PRICE-RELATED (SALES AND DEALS) NOTIFICATIONS OR SET REMINDER FOR WHEN ITEM IS BACK IN STOCK
    Image bell_icon(Icon::get["bell"]->get_data(), 64, 64, 1, 4);
    bell_icon.set_color(255, 182, 77);
    bell_icon.resize(24, 24);
    bell_icon.set_alignment("center");
    Button notif_button;
    notif_button.set_size(32, 32);
    notif_button.set_color(current->get_color());
    notif_button.set_outline(true); // white by default
    notif_button.set_relative_position(favorite_button.get_relative_x() - favorite_button.get_width() - 5, favorite_button.get_relative_y());
    notif_button.set_image(bell_icon);
    //////current->add_gui(notif_button);
    // ADD TO CART BUTTON
    Button cart_add_button("Add to cart");
    cart_add_button.set_width(current->get_width());//(200);
    cart_add_button.set_color(82, 70, 86);*/ //(55, 25, 255);//bluish-purple//(42, 25, 255);//(50, 25, 255);//(30, 30, 255);
    //////current->add_gui(cart_add_button);    
    // place all set_pos() and set_rel_pos() functions in loop for it to be frequently updated in real-time!
    // cart_button position
    //////cart_add_button.set_relative_position((current->get_width() - cart_add_button.get_width()) / 2, current->get_height() - cart_add_button.get_height()/* - 10*/); // center x, lower y
    //std::cout << "catalog size: " << catalog->get_size() << std::endl;
    //std::cout << "catalog box size: " << current->get_size() << std::endl;
    // qty_spinner position
    //////quantity_spinner.set_relative_position((current->/*cart_add_button.*/get_width() / 2) - (quantity_spinner.get_full_width() / 2), current->get_height() - cart_add_button.get_height() - quantity_spinner.get_height() - 10);
}
////////////////////
void neroshop::Catalog::update_page(int item_id) {
    /*Item product(item_id); // temporary (will die at end of scope)
    // get product_image_border_box's image
    Box * product_image_border_box = static_cast<Box *>(current->get_gui(0));
    Image * product_image_0 = static_cast<Box *>(current->get_gui(0))->get_image(0);//current->get_image(0)->
    // delete old page image and assign it to the grid box image
    std::shared_ptr<Image> product_image_0_copy(product_image_0);//std::cout << "use_count(before reset): " << product_image_0_copy.use_count() << std::endl;
    product_image_0_copy.reset();//std::cout << "use_count(after reset): " << product_image_0_copy.use_count() << std::endl;    
    // get product thumbnail image
    Image product_image_temp = *product.get_upload_image(1);*/ // first image in array
    /*if(!product_image_temp) {
        product_image_0 = new Image(Icon::get["image_gallery"]->get_data(), 64, 64, 1, 4);
    }*/    
    // copy grid box image to page image
    /*std::cout << "image buffer: " << static_cast<Box *>(current->get_gui(0))->get_image(0)->get_buffer() << std::endl;
    if(product_image_border_box->get_image_count() > 0) {
    product_image_0->copy((&product_image_temp) ? product_image_temp : *Icon::get["image_gallery"] );//// (product_image_temp) ? *product_image_temp//load(product_image.get_data(), product_image.get_width(), product_image.get_height(), product_image.get_depth(), product_image.get_channel());
    // if image size is greater than border size, scale the image
    if(product_image_0->get_width() > product_image_border_box->get_width() || product_image_0->get_height() > product_image_border_box->get_height()) {
        std::cout << "image is too large, but we will scale it for you!\n";
        product_image_0->resize(128, 128);
        // "corrupted double-linked list. Aborted(core dumped)" error somewhere around here
    }
    // set relative position
    product_image_0->set_relative_position((product_image_border_box->get_width() - product_image_0->get_width()) / 2, (product_image_border_box->get_height() - product_image_0->get_height()) / 2);
  }
  //--------------------------------------------------
  int box_gap = 10;
  Box * product_image_border_boxes_1 = static_cast<Box *>(current->get_gui(1));
  product_image_border_boxes_1->set_relative_position(10, product_image_border_box->get_relative_y() + product_image_border_box->get_height() + box_gap);
  Box * product_image_border_boxes_2 = static_cast<Box *>(current->get_gui(2));
  product_image_border_boxes_2->set_relative_position(product_image_border_boxes_1->get_relative_x() + product_image_border_boxes_1->get_width() + box_gap, product_image_border_boxes_1->get_relative_y());
  ////product_image_border_boxes_?->set_relative_position(product_image_border_boxes_?->get_relative_x() + product_image_border_boxes_?->get_width() + 10, product_image_border_boxes_1->get_relative_y());
  //--------------------------------------------------
  //--------------------------------------------------
*/  
}
////////////////////
////////////////////
////////////////////
// This function will change overtime and will populate the catalog with products such as best sellers, most wished for, etc.
void neroshop::Catalog::populate() {
    //fetch_items();
    fetch_inventory();
    //fetch_best_sellers();
    //-----------------------------------
    neroshop::print("catalog populated");
}
////////////////////
void neroshop::Catalog::fetch_items() {
    std::string command = "SELECT * FROM item ORDER BY id ASC LIMIT $1;"; // "ORDER BY id ASC" = place in ascending order by id (oldest to latest items). DESC would be from the latest to the oldest items
    int box_count = view->get_row_count() * view->get_column_count(); // rows x columns
    std::vector<const char *> param_values = { std::to_string(box_count).c_str() };
    PGresult * result = PQexecParams(DB::Postgres::get_singleton()->get_handle(), command.c_str(), 1, nullptr, param_values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print("Catalog::fetch(): No registered items found", 2);        
        PQclear(result);//DB::Postgres::get_singleton()->finish();//exit(1);
        return; // exit so we don't double free "result"
    }
    int rows = PQntuples(result); // max_rows is limited to number of boxes in the grid//std::cout << "number of rows (items): " << rows << std::endl;
    for(int i = 0; i < std::min<size_t>(rows, box_count); i++) {
        Box * box = view->get_box(i);//(r, c);
        int item_id = std::stoi(PQgetvalue(result, i, 0));
        std::string item_name = DB::Postgres::get_singleton()->get_text_params("SELECT name FROM item WHERE id = $1", { std::to_string(item_id) });
        std::cout << "registered item_ids: " << item_id << " (" << item_name << ")" << std::endl;        
        // draw contents here ...
    }
    ////////////////////
    PQclear(result); // free result when done using it       
}
////////////////////
void neroshop::Catalog::fetch_inventory() {
    std::string command = "SELECT * FROM inventory ORDER BY item_id ASC LIMIT $1;"; //WHERE stock_qty > 0;";// "ORDER BY item_id ASC" = place in ascending order by item_id (lowest to highest). DESC would be from highest to lowest
    int box_count = view->get_row_count() * view->get_column_count(); // rows x columns
    std::vector<const char *> param_values = { std::to_string(box_count).c_str() };
    PGresult * result = PQexecParams(DB::Postgres::get_singleton()->get_handle(), command.c_str(), 1, nullptr, param_values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print("Catalog::fetch(): No inventory items found", 2);        
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
            // verified_purchase_icon - maybe replace this with tickers (e.g "recommended", "new", "best seller"), etc. (verified purchase icon cannot be pressed but is only shown to let the user know whether they have previously purchased this item or not)
            Image * verified_purchase_icon = new Image(Icon::get["paid"]->get_data(), 64, 64, 1, 4); // image is still alive outside of scope :D
            //std::shared_ptr<Image> verified_purchase_icon = std::shared_ptr<Image>(new Image(Icon::get["paid"]->get_data(), 64, 64, 1, 4)); // causes "Floating point exception (core dumped)" error
            //std::shared_ptr<Image> verified_purchase_icon = std::make_shared<Image>(Icon::get["paid"]->get_data(), 64, 64, 1, 4); // image is dead at end of scope :(
            verified_purchase_icon->resize(24, 24);//(32, 32);
            verified_purchase_icon->set_color(128, 128, 128, 1.0);//(30, 80, 155); //155 or 255
            verified_purchase_icon->set_relative_position(10, 10);
            verified_purchase_icon->set_visible(false); // hide by default and show only if user has purchased the item (guests purchases are not stored which is another reason why verified_purchase_icon is hidden)
            box->add_image(*verified_purchase_icon);
            // heart_icon (favorites or wishlist)
            Image * heart_icon = new Image(Icon::get["heart"]->get_data(), 64, 64, 1, 4);
            heart_icon->set_color(128, 128, 128, 1.0);//(224, 93, 93, 1.0);
            heart_icon->resize(verified_purchase_icon->get_width(), verified_purchase_icon->get_height());//(24, 24);//(16, 16);
            heart_icon->set_relative_position(box->get_width() - heart_icon->get_width() - 10, 10);
            heart_icon->set_visible(false); // hide by default and show only if user has favorited the item (guests cannot heart items which is another reason why heart_icon is hidden)
            box->add_image(*heart_icon);
            // product_image (thumbnail)     
            Item item(item_id); // temporary (will die at end of scope)
            Image * product_image = item.get_upload_image(1); // first image is thumbnail
            if(!product_image) product_image = new Image(Icon::get["image_gallery"]->get_data(), 64, 64, 1, 4); // if no image uploaded for this item, use a placeholder image instead
            product_image->resize(128, 128);//scale_to_fit(view->get_size());
            product_image->set_relative_position(box->get_x() + (box->get_width() - product_image->get_width()) / 2, heart_icon->get_relative_y() + heart_icon->get_height() + 10);//44//or 50);//box->get_y() + (box->get_height() - product_image->get_height()) / 2);//set_alignment("center"); // actual relative position is set in the draw call
            box->add_image(*product_image);
            // save the item_id as a component so we know which item_id belongs to which box
            box->add_component(* new Component("item_id", static_cast<int>(item_id))); // will be stored as shared_ptr by the entity class
            std::cout << "component item_id(" << box->get_component("item_id") << ") added to box " << i << std::endl;
            //////////////////////////////////////////////////
            // product name
            std::string product_name = DB::Postgres::get_singleton()->get_text_params("SELECT name FROM item WHERE id = $1;", { std::to_string(item_id) });//std::cout << "product_name: " << product_name << std::endl;
            dokun::Label * product_name_label = new dokun::Label(product_name);
            product_name_label->set_color(32, 32, 32, 1.0);
            product_name_label->set_relative_position(10, product_image->get_relative_y() + product_image->get_height() + 10);
            box->add_gui(*product_name_label);
            //////////////////////////////////////////////////
            // stars (just for display, users cannot rate products in the catalog_view, but they can on the product_page)
            float average_stars = item.get_average_stars();
            //std::cout << "average_stars for item(id: " << item_id << "): " << average_stars << std::endl;
            std::vector<Image *> product_stars; // size will be 5    //std::cout << "number of stars: " << product_stars.size() << std::endl;
            for(int i = 0; i < 5; i++) {
                product_stars.push_back(new Image());                
                product_stars[i]->load(Icon::get["star"]->get_data(), 64, 64, 1, 4);//"star_half"
                product_stars[i]->resize(20, 20);//(16, 16);
                product_stars[i]->set_color((average_stars > 0) ? 255, 179, 68, 1.0 : 255, 255, 255, 1.0);
                ////product_stars[i]->set_outline(true); // gives the star an illusion of depth
                ////product_stars[i]->set_outline_thickness(0.6);//(1.0);
                ////product_stars[i]->set_outline_color(230, 136, 0);// shades = rgb(230, 136, 0) = THE perfect outline color, rgb(179, 106, 0) = looks really bad//product_stars[i]->set_outline_threshold(0.0);
                product_stars[i]->set_visible(false);
                box->add_image(*product_stars[i]);// same as: current->set_image(*product_stars[0].get(), i); // except that Box::set_image uses insert() rather than push_back(). This is the only difference between Box::add_image and Box::set_image
                if(i == 0) { 
                    product_stars[0]->set_relative_position(10, product_name_label->get_relative_y() + product_name_label->get_height() + 10);//(box->get_width() - (product_stars[0]->get_width() * 5) - 10, product_name_label->get_relative_y() + (product_name_label->get_height() - product_stars[0]->get_height()) / 2); // set position of the first star (other stars will follow it)
                    continue; // skip the first star for now
                }
                // update positions of stars
                product_stars[i]->set_relative_position(product_stars[i - 1]->get_relative_x() + product_stars[i - 1]->get_width() + 1, product_stars[0]->get_relative_y()); // same y_rel_pos as first star
            }            
            // draw yellow stars on top of gray/white stars or ?
            // ...
            // product review label
            int product_ratings_count = DB::Postgres::get_singleton()->get_integer_params("SELECT COUNT(*) FROM item_ratings WHERE item_id = $1", { std::to_string(item_id) });//item.get_ratings_count();
            dokun::Label * product_reviews_label = new dokun::Label("("+std::to_string(product_ratings_count)+")");//(star_ratings > 0) ? std::to_string(star_ratings)+" ratings" : "No ratings yet");
            product_reviews_label->set_color(16, 16, 16, 1.0);
            product_reviews_label->set_visible(product_stars[0]->is_visible());
            product_reviews_label->set_relative_position(product_stars[product_stars.size() - 1]->get_relative_x() + product_stars[product_stars.size() - 1]->get_width() + 1, product_stars[product_stars.size() - 1]->get_relative_y() + (product_stars[0]->get_height() - product_reviews_label->get_height()) / 2);
            box->add_gui(*product_reviews_label);            
            //////////////////////////////////////////////////
            // price display
            double sales_price = std::stod(PQgetvalue(result, i, 4));//std::cout << "product_price: " << sales_price << std::endl;
            std::string currency = PQgetvalue(result, i, 5);//std::cout << "seller currency: " << currency << std::endl;
            std::string currency_symbol = Converter::get_currency_symbol(currency);
            //double sales_price_xmr = Converter::to_xmr(sales_price, currency); // requires libcurl + internet connection (may fail at times) and also slows app launch
            double sales_price_xmr = 0.0; // this is only temporary
            //std::cout << "product_price_xmr: " << String::to_string_with_precision(sales_price_xmr, 12) << std::endl;
            dokun::Label * price_label = new dokun::Label(currency_symbol + String::to_string_with_precision(sales_price, 2) + " " + String::upper(currency));
            price_label->set_color(0, 0, 0, 1.0);
            if(product_stars[0]->is_visible()) price_label->set_relative_position(10, product_stars[0]->get_relative_y() + product_stars[0]->get_height() + 10);//box->get_height() - price_label->get_height() - 10);
            if(!product_stars[0]->is_visible()) price_label->set_relative_position(10, product_name_label->get_relative_y() + product_name_label->get_height() + 10);//15);
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
            if(!monero_ticker->is_visible()) price_label_xmr->set_relative_position(10, price_label->get_relative_y() + price_label->get_height() + 10 );//(monero_ticker->get_relative_x() + monero_ticker->get_width() + 5, monero_ticker->get_relative_y() + (monero_ticker->get_height() - price_label->get_height()) / 2);// <= use this if monero_symbol is visible
            if(monero_ticker->is_visible()) price_label_xmr->set_relative_position(monero_ticker->get_relative_x() + monero_ticker->get_width() + 5, monero_ticker->get_relative_y() + (monero_ticker->get_height() - price_label->get_height()) / 2);
            price_label_xmr->set_color(0, 0, 0, 1.0);
            price_label_xmr->set_visible(false);
            box->add_gui(*price_label_xmr);
            //////////////////////////////////////////////////
            // tooltip
            //If(mouse_is_over(heart_icon)) tooltip->show("This item is in your favorites");
            //If(mouse_is_over(verified_purchase_icon)) tooltip->show("You've previously purchased this item");
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
void neroshop::Catalog::fetch_best_sellers() {
    // if an item has never been ordered at least once, then it will not appear in best-sellers
    std::string command = "SELECT item_id FROM order_item GROUP BY item_id ORDER BY SUM(item_qty) DESC LIMIT $1;"; // DESC (from highest to lowest sum of item_qty)
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
        Box * box = view->get_box(i);//(r, c);
        int item_id = std::stoi(PQgetvalue(result, i, 0));
        std::string item_name = DB::Postgres::get_singleton()->get_text_params("SELECT name FROM item WHERE id = $1", { std::to_string(item_id) });
        std::cout << "best-selling item_ids: " << item_id << " (" << item_name << ")" << std::endl;
        // draw contents here ...
        // best_seller ticker
        /*Box * best_seller_ticker = new Box();
        best_seller_ticker->set_size(120, 24);
        best_seller_ticker->set_color(255, 82, 82, 1.0);//https://www.color-hex.com/color-palette/2539
        best_seller_ticker->set_radius(10);////dokun::Font * font = new dokun::Font(); font->set_pixel_size(0, 14); font->load(DOKUN_DEFAULT_FONT_PATH);////best_seller_ticker->set_size(110 - 20, 17 + 3);//std::cout << "text size(based on font calc): " << Vector2(font->get_width("best seller"), font->get_height("best seller")) << std::endl;
        dokun::Label * best_seller_label = new dokun::Label("best seller");//new dokun::Label(*font, "best seller");
        best_seller_label->set_alignment("center");//best_seller_label->set_relative_position((best_seller_ticker->get_width() - font->get_width("best seller")) / 2, (best_seller_ticker->get_height() - font->get_height("best seller")) / 2);
        best_seller_ticker->set_label(*best_seller_label);//or add_gui//best_seller_ticker->set_size(best_seller_label->get_width() + 20, 24);
        //best_seller_ticker->set_visible(true);
        best_seller_ticker->set_relative_position(40, 10); // center the x or nah?
        box->add_gui(*best_seller_ticker);*/        
    }
    ////////////////////
    PQclear(result); // free result when done using it
}
////////////////////
// I don't know how to get the mode of unnest(item_ids) in table favorites :/
void neroshop::Catalog::fetch_most_favorited() {
    // if an item has never been favorited at least once, then it will not appear in most favorited
    std::string command = "";//"SELECT unnest(item_ids) FROM favorites;";
    /*SELECT count(*), UNNEST(item_ids) as item_id
    FROM favorites GROUP BY item_id;*/
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
        Box * box = view->get_box(i);//(r, c);
        int item_id = std::stoi(PQgetvalue(result, i, 0));
        std::string item_name = DB::Postgres::get_singleton()->get_text_params("SELECT name FROM item WHERE id = $1", { std::to_string(item_id) });
        std::cout << "most favorited item_ids: " << item_id << " (" << item_name << ")" << std::endl;
        // draw contents here ...
    }
    ////////////////////
    PQclear(result); // free result when done using it
}
////////////////////
// refresh content without allocating new objects
void neroshop::Catalog::refresh() {
    std::vector<std::vector<std::shared_ptr<Box>>> box_list = view->get_box_list_2d();
	if(box_list.empty()) return;// if no rows, exit function
	for(auto rows : box_list) {
	    for(auto boxes : rows) {
	        if(!boxes->has_component("item_id")) continue; // ignore boxes without products
	        unsigned int item_id = boxes->get_component("item_id")->to_integer();
            // check if user has previously purchased this item
            // verified purchase image - change color if previously purchased
            Image * verified_purchase_icon = boxes->get_image(0);
            bool purchased = User::get_singleton()->has_purchased(item_id); // won't work with guests since their purchases are not stored
            verified_purchase_icon->set_color((purchased) ? Vector4(30, 80, 155, 1.0) : Vector4(128, 128, 128, 1.0));//(30, 80, 155); //155 or 255    
            verified_purchase_icon->set_visible((purchased) ? true : false); // show, since it is hidden by default
            // check if this item is in user's favorites list (heart icon cannot be pressed but is only shown to let the user know whether the item is in their favorites/wishlist/saved_items or not)
            // heart icon - change color if item is in favorites
            Image * heart_icon = boxes->get_image(1);
            bool favorited = User::get_singleton()->has_favorited(item_id);
            heart_icon->set_color((favorited) ? Vector4(224, 93, 93, 1.0) : Vector4(128, 128, 128, 1.0));//;        
            heart_icon->set_visible((favorited) ? true : false); // show if hidden by default
            // check whether image has been loaded or not
            // product_image - load if not yet loaded
        }
    }
    //-----------------------------------
    neroshop::print("catalog refreshed");    
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
    std::vector<Box *> box_list_1d = view->get_box_list_1d();
    if(box_list_1d.empty()) throw std::runtime_error("The catalog is empty!");
    return box_list_1d[index];
}
////////////////////
Grid * neroshop::Catalog::get_view() const {
    return get_grid();
}
////////////////////
Box * neroshop::Catalog::get_page() const {
    return current.get();
}
////////////////////
Box * neroshop::Catalog::get_tooltip() const {
    return tooltip.get();
}
////////////////////
Box * neroshop::Catalog::get_sort_box() const {
    return sort_box.get();
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
