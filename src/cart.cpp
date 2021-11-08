#include "../include/cart.hpp"

////////////////////
Cart::Cart() {
    max_items = 10; // cart can only hold up to 10 unique items
    max_quantity = 100; // cart items can only add up to 100 qty
}
////////////////////
Cart::~Cart() {}
////////////////////
Cart * Cart::cart_obj (nullptr);
////////////////////
// normal
////////////////////
bool Cart::open() const {
    // cart data is stored locally on a user's device
    // a cart tied to an account (user_id) should be named: cart_user.db (e.g cart_layter.db) or??
    // cart.db - should not be loaded until user logs in or??
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    std::ifstream file(cart_file.c_str()); // no need to use f.close() as the file goes out of scope at the end of the function
    // if cart.db already exists        
    if(file.good()) {
        std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
        NEROSHOP_TAG std::cout << "\033[1;36m" << "cart found: \"" << cart_file << "\"\033[0m" << std::endl;
        // open cart db
        DB db;if(!db.open(cart_file)) {neroshop::print(SQLITE3_TAG + std::string("could not open cart.db")); return false;}
        if(!db.table_exists("Cart")) { // table Cart is missing either due to corruption or something
            neroshop::print(SQLITE3_TAG + std::string("table Cart is missing (Please delete cart.db and then restart the application)"));
            db.close();
            return false;
        }
        // create items based on item_ids stored in cart.db
        int cart_items_count = db.row_count("Cart"); // numbers of items currently in cart
        unsigned int last_cart_item = db.get_column_integer("Cart ORDER BY id DESC LIMIT 1", "*"); // number of inserted rows in table cart //std::cout << "last inserted row in table cart: " << last_cart_item << std::endl;
        if(cart_items_count < 1) neroshop::print("Cart is empty");
        if(cart_items_count > 0) 
        {   // load items into the cart
            for(unsigned int i = 1; i <= last_cart_item; i++) {//for(unsigned int i = 1; i <= cart_items_count; i++) {
                unsigned int cart_item = db.get_column_integer("Cart", "item_id", "id = " + std::to_string(i));
                if(cart_item == 0) continue; // item not in cart, so skip
                unsigned int item_qty = db.get_column_integer("Cart", "item_qty", "item_id = " + std::to_string(cart_item));//neroshop::print("loaded cart item (id: " + std::to_string(cart_item) + ", qty: " + std::to_string(item_qty) + ")");// << std::endl;//std::cout << "found cart item with id: " << i << " (qty: " << item_qty + ")" << std::endl;
                Item * item = new Item(cart_item);//Item item(cart_item);//item created on stack dies at end of for loop that is why it returns invalid values
                // if the cart is not empty, just load it, no need to add more of the same items again
                Cart::get_singleton()->load(*item, item_qty);
            }
        }
        db.close();
    }
    if(!file.good()) { // if cart.db does not exist, create it
        if(!Cart::create_db()) {neroshop::print(SQLITE3_TAG + std::string("failed to create cart.db")); return false;}
    }
    return true;        
}
////////////////////
void Cart::load(const Item& item, unsigned int quantity) { // load pre-existing cart data
    // be sure item is in stock
    if(!item.in_stock()) 
    {   // remove item from cart if it is no longer in stock
        NEROSHOP_TAG std::cout << "\033[1;91m" << item.get_name() << " is out of stock" << "\033[0m" << std::endl; // An item that was in your cart is now out of stock
        const_cast<Item&>(item).set_quantity(0); // set quantity to 0
        remove_db(item.get_id()); // remove item_id from table Cart
        NEROSHOP_TAG std::cout << "\033[1;91m" << item.get_name() << " (" << quantity << ") removed from cart" << "\033[0m" << std::endl;
        return; // exit function
    }
    unsigned int stock_qty = item.get_stock_quantity(); // get stock quantity
    if(quantity >= stock_qty) quantity = stock_qty; // quantity cannot be more than what is in stock
    // load the item and quantity ...
    cart_obj->contents.push_back(&const_cast<Item&>(item)); // store existing item
    const_cast<Item&>(item).set_quantity(quantity); // save existing item_qty
    NEROSHOP_TAG std::cout << "\033[1;32m" << item.get_name() << " (id: " << item.get_id() << ", qty: " << quantity << ") has been loaded into cart" << "\033[0m" << std::endl;
}
////////////////////
void Cart::add(const Item& item, unsigned int quantity) { // good!
    // check if item is in database
    if(!item.is_registered()) {NEROSHOP_TAG std::cout << "\033[1;91m" << item.get_name() << " is not registered in the database" << "\033[0m" << std::endl;return;}
    // check if the item is in stock
    if(!item.in_stock()) {NEROSHOP_TAG std::cout << "\033[1;91m" << item.get_name() << " is out of stock" << "\033[0m" << std::endl; return;}
    unsigned int item_qty  = item.get_quantity(); // get item_qty
    unsigned int stock_qty = item.get_stock_quantity(); // get stock_qty
    if(quantity >= stock_qty) quantity = stock_qty - item_qty; // make sure quantity is not more than the amount in stock
    // if item_qty is more than stock amount
    if(item_qty >= stock_qty) {
        const_cast<Item&>(item).set_quantity(stock_qty);//update(item.get_id(), stock_qty); // set item_qty to stock_qty
        NEROSHOP_TAG std::cout << "Only " << "\033[1;91m" << stock_qty << "\033[0m " << item.get_name() << "s left in stock" << std::endl;//NEROSHOP_TAG std::cout << "\033[0;33mYou have exceeded the amount in stock\033[0m" << std::endl;
        return; // exit function since item_qty has surpassed the amount in stock
    }
    // make sure cart is not full, else exit function
    if(is_full()) {
        NEROSHOP_TAG std::cout << "\033[0;33m" << "Cart is full";
        std::string reason = (contents.size() >= max_items) ? "you can only add 10 items (unique) to cart" : "max_quantity (100) has been exceeded";
        std::cout << " (" <<  reason << ")" << "\033[0m" << std::endl; 
        return; // exit function if full
    }
    // quantity cannot be less than one
    if(quantity <= 0) quantity = 1;    
    // quantity cannot exceed max_quantity, so exit function (lets say that the quantity is 4294967280 - this is not acceptable)
    if(quantity > max_quantity) {NEROSHOP_TAG std::cout << "Quantity is too high (max_quantity (" << max_quantity << ") has been exceeded)" << "\033[0m" << std::endl; return;}
    // if cart_qty added with the new quantity exceeds max_quantity (cart does not have to be full)
    unsigned int cart_qty = get_total_quantity();//_db();
    if((cart_qty + quantity) > max_quantity)
    {
        NEROSHOP_TAG std::cout << "\033[0;33m" << "Cart is full (max_quantity (" << max_quantity << ") has been reached)" << "\033[0m" << std::endl;// : " << quantity << " " << const_cast<Item&>(item).get_name() << "s ("<< get_quantity() << "+" << quantity << "=" << (get_quantity() + quantity) << ")" << std::endl; //         std::cout << "You can only have a quantity of " << max_quantity << " items in your cart" << std::endl;
        // adjust the quantity so it can fit
        // 100-10=90 (max_quantity-items_count=quantity)(you need at least a quantity of 90 to fit everything into the cart)
        quantity = max_quantity - cart_qty;
    }
    // if item is already in cart
    if(in_cart(item.get_id()))//if(in_cart(item)) 
    {
        const_cast<Item&>(item).set_quantity(item_qty + quantity);// increase item quantity by a user-specified amount//update(item.get_id(), item_qty + quantity);
        item_qty = item.get_quantity(); // get updated quantity
        NEROSHOP_TAG std::cout << "\033[0;32m" << "Already in cart: " << item.get_name() << " +" << quantity << " (" << item_qty << ")" << "\033[0m" << std::endl;
    }
    // if item is not already in cart
    if(!in_cart(item.get_id()))//if(!in_cart(item)) 
    {
        add_db(item.get_id()); // add item to table Cart
        contents.push_back(&const_cast<Item&>(item));// add item to cart.contents
        const_cast<Item&>(item).set_quantity(item_qty + quantity);// increase item quantity by a user-specified amount
        NEROSHOP_TAG std::cout << "\033[1;32m" << item.get_name() << " (" << quantity << ") added to cart" << "\033[0m" << std::endl;
    }
#ifdef NEROSHOP_DEBUG
    NEROSHOP_TAG std::cout << "\033[0;97m" << "Total items in cart: " << get_items_count() << "\033[0m" << std::endl;
#endif    
}
//////////////////// blue: << "\033[0;34m"
void Cart::remove(const Item& item, unsigned int quantity) {
    // if item is not in cart, exit function since there is nothing to remove
    if(!in_cart(item.get_id())) return;
    // make sure quantity is not more than item_quantity (cannot remove more than that which is in the cart)
    unsigned int item_qty = item.get_quantity();
    if(quantity >= item_qty) quantity = item_qty;
    // if item is already in cart
    // if item_quantity is more than zero (0), reduce item_quantity by 1 or whatever the specified quantity is
    if(item_qty > 0) 
    {
        const_cast<Item&>(item).set_quantity(item_qty - quantity);//(item.get_quantity() - quantity);
        item_qty = item.get_quantity(); // item_qty has been updated so get the updated item_qty
        NEROSHOP_TAG std::cout << "\033[0;91m" << item.get_name() << " (" << quantity << ") removed from cart" << "\033[0m" << std::endl;
    }
    // if item_quantity is zero (0), delete item from cart
    if(item_qty <= 0) 
    {
        const_cast<Item&>(item).set_quantity(0); // first, make sure item quantity is exactly zero (0)
        // now remove it from cart.contents
        for(int i = 0; i < contents.size(); i++) {
            if(item.get_id() == contents[i]->get_id()) {contents.erase(contents.begin()+i);}//std::cout << "cart [" << i << "]: " << contents[i]->get_name() << std::endl;
        }
        // remove the item from the database as well
        remove_db(item.get_id());
    }
    // if cart is empty, clear the contents: if(is_empty()) empty();
#ifdef NEROSHOP_DEBUG
    NEROSHOP_TAG std::cout << "\033[0;97m" << "Total items in cart: " << get_items_count() << "\033[0m" << std::endl;
#endif            
    if(is_empty()) neroshop::print("Cart is empty");
}
////////////////////
void Cart::remove(unsigned int index, unsigned int quantity) {
    if(index > (contents.size()-1)) throw std::runtime_error(std::string("\033[0;31m") + "Cart::remove(unsigned int, unsigned int): attempting to access invalid index" + std::string("\033[0m"));
    remove(*contents[index], quantity); // remove item at an index
}
////////////////////
void Cart::empty() {
    contents.clear();
    // clear the sqlite Cart table as well
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db(cart_file);
    if(!db.table_exists("Cart")) {db.close(); return;}
    db.truncate("Cart");
    //db.vacuum(); // probably not necessary since cart.db is smaller than the main db and is stored locally on the user's device
    db.close();
}
////////////////////
////////////////////
////////////////////
void Cart::change_quantity(const Item& item, unsigned int quantity) {
    if(!in_cart(item.get_id())) return; // you can only change the qty of items that are already inside of the cart    
    // check if item is in stock
    if(!item.in_stock()) {NEROSHOP_TAG std::cout << "\033[1;91m" << item.get_name() << " is out of stock" << "\033[0m" << std::endl; return;}
    int stock_qty = item.get_stock_quantity(); // get stock quantity
    if(quantity >= stock_qty) {quantity = stock_qty; NEROSHOP_TAG std::cout << "Only " << "\033[1;91m" << stock_qty << "\033[0m " << item.get_name() << "s left in stock" << std::endl;} // make sure quantity is not more than the amount in stock
    // if item quantity is the same as the specified amount, exit function
    unsigned int item_qty = item.get_quantity();
    if(quantity == item_qty) return; // there is really nothing to change here since item_qty and specified quantity are both the same value
    // if cart is full, but user still wants to increase the item_qty
    if(is_full() && (quantity > 0)) {NEROSHOP_TAG std::cout << "\033[0;33m" << "Cart is full, and you still want to add more shit to it, bruh?" << "\033[0m" << std::endl;return;}
    if(quantity < 0) quantity = 0; // quantity cannot be negative
    if(quantity > max_quantity) quantity = max_quantity; // quantity cannot exceed max_quantity
    // if cart_qty added with the new quantity exceeds max_quantity (cart does not have to be full)
    unsigned int cart_qty = get_total_quantity();
    if((cart_qty + quantity) > max_quantity)
    {
        // adjust the quantity so it can fit
        // (100+1)-2=99 ((max_quantity+item_qty)-items_count=quantity)(you need at least a quantity of 99 to fit everything into the cart)
        quantity = (max_quantity + item_qty) - cart_qty;
        // cart can no longer fit any more items
        NEROSHOP_TAG std::cout << "\033[0;33m" << "Cart is full (cannot fit any more items)" << "\033[0m" << std::endl;
        // cart is full but the cart item's previous quantity is higher than the new quantity
        //if(item.get_quantity() > quantity) {return;} // cart is already full, so no need to increase the qty. Exit the function!
        //return; // exit the function
    }
    // if quantity of item is set to 0 or less, remove the item from the cart (remove() will auto set the quantity to 0)
    if(quantity <= 0) { remove(item, max_quantity); return;} // remove all of it from the cart
    // with all obstacles out of the way, change the qty of the item
    const_cast<Item&>(item).set_quantity(quantity); // changes quantity in database as well
#ifdef NEROSHOP_DEBUG
    neroshop::print(item.get_name() + " quantity has changed to \033[0;36m" + std::to_string(quantity) + "\033[0m");
    NEROSHOP_TAG for(int i = 0; i < contents.size(); i++) {
        item_qty = Item::get_quantity(contents[i]->get_id());
        std::string item_name = contents[i]->get_name();
        std::cout << "Cart [" << i << "]: " << item_name << " (" << item_qty << ")" << std::endl;
    }
    //NEROSHOP_TAG std::cout << "\033[0;97m" << "Total items in cart: " << get_items_count() << "\033[0m" << std::endl;
#endif
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
double Cart::get_seller_subtotal_price(unsigned int seller_id) const { 
    double seller_price = 0.00;
    for(int i = 0; i < contents.size(); i++) {
        seller_price += contents[i]->get_quantity() * contents[i]->get_seller_price(seller_id); // if seller_id is 0, it will return a random seller's price
    }
    return seller_price;
}
////////////////////
double Cart::get_subtotal_price() const {
    double items_price = 0.00; // price of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_price += contents[i]->get_quantity() * contents[i]->get_price();
    }
    return items_price;
}
////////////////////
////////////////////
unsigned int Cart::get_total_quantity() const {
    unsigned int items_count = 0; // quantity of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_count += contents[i]->get_quantity();
    }
    return items_count;
}
////////////////////
unsigned int Cart::get_total_quantity_db() { // faster than Cart::get_total_quantity() since this does not open the db file multiple times, but only one time
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {NEROSHOP_TAG std::cout << "Could not open sql database - " << cart_file << std::endl; return 0;}
    if(!db.table_exists("Cart")) {db.close(); return 0;}
    ///////
    unsigned int row_count = db.row_count("Cart");
    unsigned int items_count = 0;
    if(row_count < 1) return 0;
    for(unsigned int i = 1; i <= row_count; i++) {
        unsigned int item_qty = db.get_column_integer("Cart", "item_qty", "item_id = " + std::to_string(i));
        items_count += item_qty;
    }
    db.close();
    return items_count;
}
////////////////////
double Cart::get_total_weight() const {
    double items_weight = 0.0; // weight of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_weight += contents[i]->get_quantity() * contents[i]->get_weight(); // weight should be determined by (items_weight * quantity)  // the more the quantity, the bigger the weight
    }
    return items_weight;
}
////////////////////
double Cart::get_seller_total_discount(unsigned int seller_id) const {
    double seller_discount = 0.00;
    for(int i = 0; i < contents.size(); i++) {
        seller_discount += contents[i]->get_seller_discount(seller_id);
    }
    return seller_discount;
}
////////////////////
double Cart::get_total_discount() const { // ?
    double items_discount = 0.00; // discount of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_discount += 0.00;//contents[i]->get_discount(i);//discounts are determined by sellers
    }
    return items_discount;
}
////////////////////
////////////////////
unsigned int Cart::get_items_count() const {
    return get_total_quantity();
}
////////////////////
unsigned int Cart::get_contents_count() const {
    return contents.size();
}
////////////////////
unsigned int get_contents_count_db() { // returns number of rows in table Cart
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db(cart_file);
    if(!db.table_exists("Cart")) {db.close(); return 0;}
    int row_count = db.row_count("Cart");
    db.close();
    return row_count;
}
////////////////////
////////////////////
////////////////////
Item * Cart::get_item(unsigned int index) const {
    if(index > (contents.size()-1)) throw std::runtime_error(std::string(std::string("\033[0;31m") + "(Cart::get_item): attempting to access invalid index" + std::string("\033[0m")).c_str());//std::cerr << "\033[0;31m" << "Cart::get_item(): attempting to access invalid index" << "\033[0m" << std::endl;//return nullptr;
    return contents[index];
}
////////////////////
//unsigned int Cart::get_id() const {
//    return id;
//}
////////////////////
////////////////////
Cart * Cart::get_singleton() {
    if(cart_obj == nullptr) cart_obj = new Cart();
    return cart_obj;
}
////////////////////
////////////////////
////////////////////
bool Cart::is_empty() const {
    return contents.empty();//return (db.row_count("Cart") == 0);
}
////////////////////
bool Cart::is_full() const {
    return ((contents.size() >= max_items) || (get_total_quantity() >= max_quantity)); // either cart.contents has reached its capacity (max_items:10) or all items have a combined quantity of 100 (max_quantity:100)
}
////////////////////
bool Cart::in_cart(unsigned int item_id) const {//(const Item& item) const {
    //return (std::find(contents.begin(), contents.end(), &const_cast<Item&>(item)) != contents.end());
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {NEROSHOP_TAG std::cout << "Could not open sql database - " << cart_file << std::endl; return false;}
    if(!db.table_exists("Cart")) {db.close(); return false;}
    int cart_item = db.get_column_integer("Cart", "item_id", "item_id = " + std::to_string(item_id));
    if(cart_item == 0) return false; // 0 means item was never registered
    if(cart_item != item_id) return false; // if cart_item_id does not match item_id
    //std::cout << "item with id: " << item_id << " is inside cart" << std::endl;
    db.close();
    return true;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
bool Cart::create_db() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d");
	std::string date = ss.str();
    ////////////////////////////
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {NEROSHOP_TAG std::cout << "Could not open sql database - " << cart_file << std::endl; return false;}
    NEROSHOP_TAG std::cout << "\033[1;97m" << "created " << cart_file << "\033[0m" << std::endl;
    db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked) // https://www.sqlite.org/pragma.html#pragma_journal_mode
    // table: Cart - one cart per user on each device
    if(!db.table_exists("Cart")) {
        db.table("Cart"); // auto generate cart_id (primary key) //db.column("Cart", "ADD", "session_id", "INTEGER"); // combine name+date+time//db.column("Cart", "ADD", "cart_id", "INTEGER"); // the id of the cart this item is in //db.column("Cart", "ADD", "user_id", "INTEGER"); // the id of the (logged) user whose the cart item belongs to
        db.column("Cart", "ADD", "item_id", "INTEGER"); // the id of the item that is in the cart
        db.column("Cart", "ADD", "item_qty", "INTEGER");
        //db.column("Cart", "ADD", "item_price", "INTEGER"); // opt //db.column("Cart", "ADD", "", "TEXT");//db.column("Cart", "ADD", "checked", "BOOLEAN");//NUMERIC // purchased or discarded//db.index("idx_session_ids", "Cart", "session_id");// enforce that the user names are unique, in case there is an attempt to insert a new "name" of the same value// jack_20211021//db.execute("CREATE UNIQUE INDEX idx_user_ids ON Cart (user_id);");//user_id should not be unique since user can have multiple items on cart
        db.index("idx_item_ids", "Cart", "item_id");//db.execute("CREATE UNIQUE INDEX idx_item_ids ON Cart (item_id);");//std::string session_id = DB::to_sql_string(user + "_" + date);//bool checked = false;
    }
    db.close();
    return true;
}
////////////////////
void Cart::add_db(unsigned int item_id) {
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {NEROSHOP_TAG std::cout << "Could not open sql database - " << cart_file << std::endl; return;}
    if(!db.table_exists("Cart")) {db.close(); return;}
    db.insert("Cart", "item_id, item_qty",
        std::to_string(item_id) + ", " + std::to_string(0) //+ ", " + std::to_string(item_price) + ", " + //std::to_string() + ", " + 
    );
    db.close();
}
////////////////////
void Cart::remove_db(unsigned int item_id) {
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {NEROSHOP_TAG std::cout << "Could not open sql database - " << cart_file << std::endl; return;}
    if(!db.table_exists("Cart")) {db.close(); return;}
    db.drop("Cart", "item_id = " + std::to_string(item_id)); // deletes column where id=2
    //db.vacuum();
    //NEROSHOP_TAG std::cout << "item " << item_id << " has been removed from Cart.db" << std::endl;
    db.close();
} 
////////////////////
////////////////////
////////////////////
////////////////////
