#include "../include/seller.hpp"

Seller::Seller() : wallet(nullptr) {//, inventory(nullptr) {  // not used
	// add to registry
    //Registry::get_seller_registry()->add(this);				 
}
////////////////////
////////////////////
Seller::Seller(const std::string& name) : Seller() {
    set_name(name);
    //on_create(name);//should be only called when registering an account, not when logging in
}
////////////////////
////////////////////
//Seller::Seller(const std::string& name, float reputation, unsigned int total_ratings) : Seller() { // delegate constructor - inherits default constructor
//    set_name(name);
//}
////////////////////
// destructor
////////////////////
Seller::~Seller() {
    //Registry::get_seller_registry()->remove(this);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void Seller::list_item(unsigned int item_id, unsigned int stock_qty, double sales_price, std::string currency)
{}
////////////////////
void Seller::list_item(const Item& item, unsigned int stock_qty, double sales_price, std::string currency, 
    std::string condition/*, double discount = 0.0, discounted_items = 0*/) { // ex. 5% off 10 balls
    // seller must be logged in
    if(!is_logged()) {neroshop::print("You must be logged in to list an item", 2); return;}
    // user must be an actual seller, not a buyer
    if(!is_seller()) {neroshop::print("Must be a seller (id: " + std::to_string(get_id()) + ") to list an item (id: " + std::to_string(item.get_id()) + ")", 2); return;}
    // a seller can create an item and then register it to the database
    // but if the item is not registered then it cannot be listed
    if(!item.is_registered()) {NEROSHOP_TAG std::cout << "\033[0;91m" << "This item is not registered (invalid Item id)" << "\033[0m" << std::endl; return;}
    // make sure currency is supported
    if(!Converter::is_supported_currency(currency)) {neroshop::print(String::upper(currency) + " is not a supported currency", 2); return;}
    // store item in database
    DB db("neroshop.db");
	//db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked) // https://www.sqlite.org/pragma.html#pragma_journal_mode
    // check if item is already in db
	// create Inventory table if it does not yet exist
	if(!db.table_exists("inventory")) { // if(!db.table_exists(""))
	    db.table("inventory"); // inventory_id will be auto generated (primary key)
	    db.column("inventory", "ADD", "item_id", "INTEGER");
	    db.column("inventory", "ADD", "seller_id", "INTEGER"); // store_id or seller_id or vendor_id
	    db.column("inventory", "ADD", "stock_qty", "INTEGER");//db.column("inventory", "ADD", "stock_available", "TEXT"); // or in_stock
	    db.column("inventory", "ADD", "seller_price", "REAL");//db.index("idx_item_ids", "inventory", "item_id"); actually, don't make item_id unique as multiple sellers can be selling the same item // every item in the inventory must have a unique id //db.execute("CREATE UNIQUE INDEX idx_seller_ids ON Inventory (seller_id);"); // sellers can have multiple items so seller_id should not be unique
	    db.column("inventory", "ADD", "currency", "TEXT"); // seller's local currency, which will be converted to xmr
	    //db.column("inventory", "ADD", "seller_condition", "");
	    //db.column("inventory", "ADD", "seller_discount", "");
	    //db.column("inventory", "ADD", "", "");
	}
    // to prevent duplicating item_id that is being sold by the same seller_id (a seller cannot list the same item twice, except change the stock amount)
    int item_id = db.get_column_integer("inventory", "item_id", 
        "item_id = " + std::to_string(item.get_id()) + " AND seller_id = " + std::to_string(get_id()));
	if(item_id == item.get_id()) { neroshop::print("\033[1;33mYou have already listed this item (id: " + std::to_string(item_id) + ")\033[0m");return;}
	//if(condition.empty()) {neroshop::print("no condition set. setting default item condition ...");condition = item.get_condition();}
	// insert item in inventory
	db.insert("inventory", "item_id, seller_id, stock_qty, seller_price, currency", 
	    std::to_string(item.get_id()) + ", "+ 
	    std::to_string(get_id())      + ", " + 
	    std::to_string(stock_qty)     + ", " + 
	    std::to_string(sales_price)   + ", " +  // per unit (item)
	    DB::to_sql_string(currency)   //+ ", "
	);// + ", ");//DB::to_sql_string(
	NEROSHOP_TAG std::cout << "\033[1;37m" << item.get_name() << " (id: " << item.get_id() << ", stock_qty: " << stock_qty << ") has been listed by seller \033[1;34m" << get_name() << " (id: " << get_id() << ")" << "\033[0m" << std::endl;
	db.close();
}
// static_cast<Seller *>(user)->list_item(ball, 10, Converter::usd_to_xmr(8.50));
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void Seller::set_stock_quantity(unsigned int item_id, unsigned int stock_qty) {
    // seller must be logged in
    if(!is_logged()) {NEROSHOP_TAG std::cout << "\033[0;91m" << "You must be logged in to list an item" << "\033[0m" << std::endl; return;}
    // a seller can create an item and then register it to the database
    if(item_id <= 0) {NEROSHOP_TAG std::cout << "\033[0;91m" << "Could not set stock_qty (invalid Item id)" << "\033[0m" << std::endl; return;}
    // update stock_qty in database
    DB db("neroshop.db");
	//db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked)
	if(db.table_exists("inventory"))
	    db.update("inventory", "stock_qty", std::to_string(stock_qty), "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(get_id()));
	db.close();
}
////////////////////
void Seller::set_stock_quantity(const Item& item, unsigned int stock_qty) {
    // seller must be logged in
    if(!is_logged()) {NEROSHOP_TAG std::cout << "\033[0;91m" << "You must be logged in to list an item" << "\033[0m" << std::endl; return;}
    // a seller can create an item and then register it to the database
    if(!item.is_registered()) {NEROSHOP_TAG std::cout << "\033[0;91m" << "Could not set stock_qty (invalid Item id)" << "\033[0m" << std::endl; return;}
    // update stock_qty in database
    DB db("neroshop.db");
	//db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked)
	if(db.table_exists("inventory"))
	    db.update("inventory", "stock_qty", std::to_string(stock_qty), "item_id = " + std::to_string(item.get_id()) + " AND seller_id = " + std::to_string(get_id()));
	db.close();
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void Seller::set_wallet(const Wallet& wallet) {
    // delete old wallet first (as seller cannot use two wallets at once, for now)
    if(this->wallet) {
        delete this->wallet;
        this->wallet = nullptr;
    }
    this->wallet = &const_cast<Wallet&>(wallet);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// getters
////////////////////
////////////////////
////////////////////
unsigned int Seller::get_good_ratings() const {
    DB db("neroshop.db");
    if(db.table_exists("seller_ratings")) {
        unsigned int good_ratings_count = db.get_column_integer("seller_ratings", "COUNT(score)", "seller_id = " + std::to_string(get_id()) + " AND score = " + std::to_string(1));
        return good_ratings_count;
    }
    db.close();
    return 0;
}
unsigned int Seller::get_bad_ratings() const {
    DB db("neroshop.db");
    if(db.table_exists("seller_ratings")) {
        unsigned int bad_ratings_count = db.get_column_integer("seller_ratings", "COUNT(score)", "seller_id = " + std::to_string(get_id()) + " AND score = " + std::to_string(0));
        return bad_ratings_count;
    }
    db.close();
    return 0;    
}
////////////////////
unsigned int Seller::get_ratings_count() const {
    DB db("neroshop.db");
    if(db.table_exists("seller_ratings")) {
        unsigned int ratings_count = db.get_column_integer("seller_ratings", "COUNT(*)", "seller_id = " + std::to_string(get_id()));
        return ratings_count;
    }
    db.close();
    return 0;
}
////////////////////
unsigned int Seller::get_total_ratings() const {
    return get_ratings_count();
}
////////////////////
unsigned int Seller::get_reputation() const {
    DB db("neroshop.db");
    if(db.table_exists("seller_ratings")) {
        unsigned int ratings_count = db.get_column_integer("seller_ratings", "COUNT(*)", "seller_id = " + std::to_string(get_id()));
        if(ratings_count == 0) return 0; // seller has not yet been rated so his/her reputation will be 0%
        // get seller's good ratings
        unsigned int good_ratings = db.get_column_integer("seller_ratings", "COUNT(score)", "seller_id = " + std::to_string(get_id()) + " AND score = " + std::to_string(1));
        // calculate seller reputation
        double reputation = (good_ratings / static_cast<double>(ratings_count)) * 100;
        return static_cast<int>(reputation); // convert reputation to an integer (basically rounding it)
    }
    db.close();
    return 0;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
Wallet * Seller::get_wallet() const {
    return wallet;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
bool Seller::has_listed(unsigned int item_id) const {
    if(item_id <= 0) {NEROSHOP_TAG std::cout << "\033[0;91m" << "invalid Item id" << "\033[0m" << std::endl; return false;}
    DB db("neroshop.db");
	//db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked)
	if(!db.table_exists("inventory")) return false;
	int id = db.get_column_integer("inventory", "item_id", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(get_id()));
	db.close();
	return (id > 0);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
User * Seller::on_login(const std::string& username) { // assumes user data already exists in database
    // get user_data from db and set them
    DB db("neroshop.db");
    User * user = new Seller(username);
    dynamic_cast<Seller *>(user)->set_logged(true); // protected, so can only be accessed by child class obj   // if validator::login(user, pw) returns true, then set User::logged to true    
    dynamic_cast<Seller *>(user)->set_id(db.get_column_integer("users", "id", "name = " + DB::to_sql_string(username)));
    dynamic_cast<Seller *>(user)->set_role(user_role::seller);
#ifdef NEROSHOP_DEBUG
    std::cout << "\033[1;34m(role: " << String::lower(user->get_role_string()) << ", id: " << user->get_id() << ", reputation: " << static_cast<Seller *>(user)->get_reputation() << ")\033[0m" << std::endl;
#endif    
    ///////////
    // load orders
    dynamic_cast<Seller *>(user)->load_orders();
    // load wishlists
    // ...
    ///////////
    db.close(); // don't forget to close the db
    return user;          
}
////////////////////
void Seller::on_order_received() {
    if(wallet == nullptr) throw std::runtime_error("wallet has not been initialized");
    if(!wallet->get_monero_wallet()) throw std::runtime_error("monero_wallet_full is not opened");
    // if wallet is not properly synced with the daemon, you can only generate used addresses
    // unless wallet is synced to a daemon, you will not be able to generate any unique addresses
    if(!wallet->get_monero_wallet()->is_synced()) throw std::runtime_error("wallet is not synced with a daemon"); // Indicates if the wallet is synced with the daemon.
    // generate 10 new subaddress after each order (just to be sure there are enough unused subaddresses to choose from)
    for(int i = 0; i < 10; i++) wallet->address_new();
    // get a list of all unused subaddresses
    std::vector<std::string> unused_subaddresses = wallet->address_unused();
    // now pick from a list of unused subaddresses (random)
	std::random_device rd; // Generating random numbers with C++11's random requires an engine and a distribution.
    std::mt19937 mt(rd()); // This is an engine based on the Mersenne Twister 19937 (64 bits):
    std::uniform_real_distribution<double> dist(0, unused_subaddresses.size() - 1);
    std::string random_subaddress = unused_subaddresses[static_cast<int>(dist(mt))];
#ifdef NEROSHOP_DEBUG    
    std::cout << std::endl << "random_subaddress: " << random_subaddress << "\n";
#endif    
    // also, generate a qrcode too
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
