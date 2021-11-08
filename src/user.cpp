#include "../include/user.hpp"

////////////////////
User::User() : logged(false), id(0), role(user_role::guest) // name is an empty string by default
{}
////////////////////
User::~User()
{
    for(int i = 0; i < order_list.size(); i++) {
        if(order_list[i]) 
            delete order_list[i];
        order_list[i] = nullptr;
    }
}
////////////////////
////////////////////
// buyers can only rate seller they have purchased from!!
void User::rate_seller(unsigned int seller_id, unsigned int score, std::string comments) { // perfected 99.9%!!
    // seller_id cannot be 0 (0 = invalid id)
    if(seller_id <= 0) return;
    // score must be between 0 and 1
    if(score >= 1) score = 1;
    if(score <= 0) score = 0;
    DB db("neroshop.db");
    // create table seller_ratings if it does not yet exist
    if(!db.table_exists("seller_ratings")) {
        db.table("seller_ratings");
        db.column("seller_ratings", "ADD", "seller_id", "INTEGER");
        db.column("seller_ratings", "ADD", "score", "INTEGER"); // 0=bad, 1=good
        db.column("seller_ratings", "ADD", "user_id", "INTEGER");
        db.column("seller_ratings", "ADD", "comments", "TEXT");
    }
    if(db.table_exists("seller_ratings")) {
        // seller_id must be an actual seller and not just a normal user (buyer)
        int role_id = db.get_column_integer("users", "role_id", "id = " + std::to_string(seller_id));//std::string role = db.get_column_text("users", "role", "id = " + std::to_string(seller_id));
        if(role_id != 2) {neroshop::print("This user (id: " + std::to_string(seller_id) + ") is not a seller, so they cannot be rated", 2); return;}//if(String::lower(role) != "seller") {neroshop::print("You cannot rate a non-seller");return;}
        // prevent seller from rating him/herself
        if(seller_id == get_id()) {
            neroshop::print("You cannot rate yourself", 2);
            return; // exit function
        }
        // to prevent duplicating seller_id that is has already been rated by this user_id (a user cannot rate the same seller twice, except update his or her score rating for a specific seller_id
        int rated_seller = db.get_column_integer("seller_ratings", "seller_id", 
            "seller_id = " + std::to_string(seller_id) + " AND user_id = " + std::to_string(get_id()));
	    if(rated_seller == seller_id) { 
	        neroshop::print("You have previously rated this seller (id: " + std::to_string(seller_id) + ")", 2);
	        db.update("seller_ratings", "score", std::to_string(score), "seller_id = " + std::to_string(seller_id) + " AND user_id = " + std::to_string(get_id()));
	        db.update("seller_ratings", "comments", DB::to_sql_string(comments), "seller_id = " + std::to_string(seller_id) + " AND user_id = " + std::to_string(get_id()));
	        neroshop::print("Your rating for seller (id: " + std::to_string(seller_id) + ") has been updated to a score of " + ((score != 0) ? "\033[1;32m" : "\033[1;91m") + std::to_string(score) + "\033[0m");
	        return; // exit function
	    }
        // insert initial values
        db.insert("seller_ratings", "seller_id, score, user_id, comments",
            std::to_string(seller_id) + ", " +
            std::to_string(score) + ", " +
            std::to_string(get_id()) + ", " +
            DB::to_sql_string(comments)// + ", " +
            //std::to_string() + ", " +
        );
        neroshop::print("You have rated seller (id: " + std::to_string(seller_id) + ") with a score of " + ((score != 0) ? "\033[1;32m" : "\033[1;91m") + std::to_string(score) + "\033[0m");
    #ifdef NEROSHOP_DEBUG        
        // get number of seller ratings (for a specific seller_id)
        unsigned int total_seller_ratings = db.get_column_integer("seller_ratings", "COUNT(*)", "seller_id = " + std::to_string(seller_id));
        std::cout << "total ratings for seller (id: " << seller_id << "): \033[1;93m" << total_seller_ratings << "\033[0m" << std::endl;
        // get seller rating from user (0 = bad, 1 = good)
        unsigned int good_ratings = db.get_column_integer("seller_ratings", "COUNT(score)", "seller_id = " + std::to_string(seller_id) + " AND score = " + std::to_string(1));
        std::cout << "number of good ratings for seller (id: " << seller_id << "): \033[1;32m" << good_ratings << "\033[0m" << std::endl;
        unsigned int bad_ratings  = db.get_column_integer("seller_ratings", "COUNT(score)", "seller_id = " + std::to_string(seller_id) + " AND score = " + std::to_string(0));                
        std::cout << "number of bad ratings for seller  (id: " << seller_id << "): \033[1;91m" << bad_ratings << "\033[0m" << std::endl;
        // calculate seller reputation
        double reputation = (good_ratings / static_cast<double>(total_seller_ratings)) * 100;
        std::cout << "reputation of seller (id: " << seller_id << "):\033[0;93m " << static_cast<int>(reputation) << "%\033[0m" << std::endl;
    #endif
    }
    //////////
    db.close();
} 
// int seller_id = 2;
// user->rate_seller(seller_id, 1, "This seller rocks!");
// user->rate_seller(seller_id, 0, "This seller sucks!");
////////////////////
////////////////////
void User::rate_item(unsigned int item_id, unsigned int stars, std::string comments) { // perfected 99%!!!
    // user_id (maybe allow unregistered users to review/rate items)
    // if user is a guest, generate an id for it? basically, a guest is a session
    // if item is not registered
    if(item_id <= 0) return; // exit function
    // star ratings must be between 1 and 5
    if(stars >= 5) stars = 5;
    if(stars <= 1) stars = 1;
    DB db("neroshop.db");
    // create table item_ratings if it does not yet exist
    if(!db.table_exists("item_ratings")) {
        // create table item_ratings if it does not yet exist
        db.table("item_ratings");
        db.column("item_ratings", "ADD", "item_id", "INTEGER");
        db.column("item_ratings", "ADD", "stars", "INTEGER"); // number of stars that user_id has given to this item_id
        db.column("item_ratings", "ADD", "user_id", "INTEGER");
        //db.column("item_ratings", "ADD", "purchased", "BOOLEAN"); // verified_purchase
        db.column("item_ratings", "ADD", "comments", "TEXT");
    }    
    // if table item_ratings does exist, insert data
    if(db.table_exists("item_ratings")) {
        // check if user has previously ordered this item
        bool purchased = false;
        for(int i = 0; i < get_order_count(); i++) {
            int ordered_item = db.get_column_integer("order_item", "item_id", "item_id = " + std::to_string(item_id) + " AND order_id = " + std::to_string(get_order(i)->get_id()));
            if(ordered_item == item_id) {
                //std::cout << "You previously ordered this item \033[0;35m(order_id: " << get_order(i)->get_id() << "\033[0m, contains \033[0;93mitem_id: " << item_id << "\033[0m)" << std::endl;
                purchased = true; // set purchased to true since user has previously purchased this item
                break; // break from the for-loop so we don't stay stuck in loop forever
            }
        }
        if(!purchased) {neroshop::print("You must purchase this item (id: " + std::to_string(item_id) + ") before rating it"); return;}
        // to prevent duplicating item_id that is has already been rated by this user_id (a user cannot rate (insert star ratings) for the same item twice, except update his or her star rating for a specific item)
        int rated_item = db.get_column_integer("item_ratings", "item_id", 
            "item_id = " + std::to_string(item_id) + " AND user_id = " + std::to_string(get_id()));
	    if(rated_item == item_id) { 
	        neroshop::print("You have previously rated this item (id: " + std::to_string(item_id) + ")", 2);
	        // if user has previous rated this item, update the user's rating
	        db.update("item_ratings", "stars", std::to_string(stars), "item_id = " + std::to_string(item_id) + " AND user_id = " + std::to_string(get_id()));
	        db.update("item_ratings", "comments", DB::to_sql_string(comments), "item_id = " + std::to_string(item_id) + " AND user_id = " + std::to_string(get_id()));
	        neroshop::print("Your star rating on item (id: " + std::to_string(item_id) + ") has been updated to \033[1;33m" + std::to_string(stars) + "\033[1;37m stars\033[0m");
	        return; // exit function
	    }        
        // insert initial values
        db.insert("item_ratings", "item_id, stars, user_id, comments",
            std::to_string(item_id) + ", " +
            std::to_string(stars) + ", " +
            std::to_string(get_id()) + ", " + // guests don't have ids
            //std::to_string(purchased) + ", " +
            DB::to_sql_string(comments)// + ", " +
        );
        //std::cout << "You have rated item (id: " << item_id << ") with " << ((stars >= 3) ? "\033[1;32m" : "\033[1;91m") << stars << " \033[0mstars" << std::endl;
        neroshop::print("You have rated item (id: " + std::to_string(item_id) + ") with " + ((stars >= 3) ? "\033[1;32m" : "\033[1;91m") + std::to_string(stars) + " \033[0mstars");
    #ifdef NEROSHOP_DEBUG        
        // get number of star ratings (for a specific item_id)
        unsigned int total_star_ratings = db.get_column_integer("item_ratings", "COUNT(*)", "item_id = " + std::to_string(item_id));
        std::cout << "# star ratings for item (id: " << item_id << "): \033[1;33m" << total_star_ratings << "\033[0m" << std::endl;
        // get number of 1, 2, 3, 4, and 5 star_ratings   
        int one_star_count = db.get_column_integer("item_ratings", "COUNT(stars)", "item_id = " + std::to_string(item_id) + " AND stars = " + std::to_string(1));
        std::cout << "# of 1 star ratings for item (id: " << item_id << "):\033[0;93m " << one_star_count << "\033[0m" << std::endl;
        int two_star_count = db.get_column_integer("item_ratings", "COUNT(stars)", "item_id = " + std::to_string(item_id) + " AND stars = " + std::to_string(2));
        std::cout << "# of 2 star ratings for item (id: " << item_id << "):\033[0;93m " << two_star_count << "\033[0m" << std::endl;
        int three_star_count = db.get_column_integer("item_ratings", "COUNT(stars)", "item_id = " + std::to_string(item_id) + " AND stars = " + std::to_string(3)); 
        std::cout << "# of 3 star ratings for item (id: " << item_id << "):\033[0;93m " << three_star_count << "\033[0m" << std::endl;
        int four_star_count = db.get_column_integer("item_ratings", "COUNT(stars)", "item_id = " + std::to_string(item_id) + " AND stars = " + std::to_string(4));
        std::cout << "# of 4 star ratings for item (id: " << item_id << "):\033[0;93m " << four_star_count << "\033[0m" << std::endl;
        int five_star_count = db.get_column_integer("item_ratings", "COUNT(stars)", "item_id = " + std::to_string(item_id) + " AND stars = " + std::to_string(5));        
        std::cout << "# of 5 star ratings for item (id: " << item_id << "):\033[0;93m " << five_star_count << "\033[0m" << std::endl;        
        // SELECT COUNT(stars) FROM item_ratings WHERE item_id = # AND stars = #
        // now calculate average stars
        // 3 total star ratings:(5, 3, 4) // average = (5 + 3 + 4) / 3 = 4 stars        
        double average_stars = (
            (1 * static_cast<double>(one_star_count)) + 
            (2 * static_cast<double>(two_star_count)) + 
            (3 * static_cast<double>(three_star_count)) + 
            (4 * static_cast<double>(four_star_count)) + 
            (5 * static_cast<double>(five_star_count))) / total_star_ratings;
        std::cout << "calculated average stars for item (id: " << item_id << "):\033[1;33m " << average_stars << "\033[0m" << std::endl;
        // test average at: https://calculator.academy/average-rating-calculator-star-rating/#f1p1|f2p0
    #endif
    }
    //////////
    db.close();
} 
// user->rate_item(ball.get_id(), 5, "Very bouncy. I love it!");
////////////////////
////////////////////
void User::convert() {
    if(is_guest()) return;
    if(is_seller()) { neroshop::print("You are already a seller", 2); return; }
    DB db("neroshop.db");
    db.update("users", "role_id", std::to_string(static_cast<int>(user_role::seller)), "id = " + std::to_string(get_id()));
    neroshop::print("You have converted from a buyer to a seller");
    db.close();
}
// if(user->is_buyer()) user->convert(); // convert buyer to seller
////////////////////
void User::delete_account() {
    if(!is_logged()) {neroshop::print("You are not logged in", 2);return;} // must be logged in to delete your account
    DB db("neroshop.db");
    db.drop("users", "id = " + std::to_string(get_id()) + " AND name = " + DB::to_sql_string(get_name()));
    neroshop::print("Your account has been permanently deleted", 1);
    // send account to graveyard
    if(!db.table_exists("deleted_users")) {
        db.table("deleted_users");
        db.column("deleted_users", "ADD", "name", "TEXT");
        db.index("idx_deleted_users", "deleted_users", "name"); // graveyard names must be unique
    }
    db.insert("deleted_users", "name", DB::to_sql_string(name)); // graveyard only stores deleted usernames and no other information
    // reset user info and then logout user
    set_id(0);
    name.clear();
    set_role(user_role::guest);
    set_logged(false); // logout here (will call on_logout callback, if logged is false)
    db.close();
} // username of deleted accounts cannot be reused ?? :O
////////////////////
////////////////////
////////////////////
////////////////////
void User::add_to_cart(const Item& item, unsigned int quantity) {
    Cart::get_singleton()->add(item, quantity);
}
////////////////////
void User::remove_from_cart(const Item& item, unsigned int quantity) {
    Cart::get_singleton()->remove(item, quantity);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
Order * User::create_order(const std::string& shipping_address) {//const {
    // name[first and last], address1(street, p.o box, company name, etc.), address2(apt number, suite, unit, building, floor, etc.) city, zip/postal_code, state/province/region country, opt:[phone, email]
    Order * order = new Order();
    order->create_order(get_id(), shipping_address); // we are using crypto, not debit/credit cards, so no billing address is needed
    order_list.push_back(order); // whether an order fails or succeeds, store it regardless
    return order;
}
// cart->add(ball, 2);
// cart->add(ring);
// user->create_order(shipping_addr);
////////////////////
////////////////////
// put this in Buyer::on_login and Seller::on_login
// Guests orders are not saved to the main database
// orders are never deleted, their statuses just change: rejected, failure, delivered, etc.
void User::load_orders() {
    DB db("neroshop.db");
    if(!db.table_exists("orders")) return; // user probably has no order history
    // create orders based on user order_ids stored in orders
    // get last inserted order
    int last_order = db.get_column_integer("orders ORDER BY id DESC LIMIT 1", "*");//int orders_count = db.row_count("orders");
    int user_order_count = db.get_column_integer("orders", "COUNT(*)", "user_id = " + std::to_string(get_id()));// get number of orders this user has made so far
    if(user_order_count < 1) neroshop::print("No orders found"); // for this user
    if(user_order_count > 0) 
    {   // if user has any orders, load them
        for(unsigned int i = 1; i <= last_order; i++) { // i is wrong because ids can start from 24, but i starts from 1
            unsigned int order_id = db.get_column_integer("orders", "id", "id = " + std::to_string(i) + " AND user_id = " + std::to_string(get_id()));
            if(order_id == 0) continue; // skip 0's
            Order * order = new Order(order_id);
            order_list.push_back(order); // store orders for later use
            neroshop::print("Order (id: " + std::to_string(order->get_id()) + ") has been loaded");
        }
    }
    db.close();
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
////////////////////
void User::set_id(unsigned int id) {
    this->id = id;
}
////////////////////
void User::set_name(const std::string& name) {
    this->name = name;
}
////////////////////
void User::set_role(user_role role) {
    this->role = role;
}
////////////////////
void User::set_logged(bool logged) { // protected function, so only derived classes can use this
    this->logged = logged;
    if(!logged) on_logout(); // call on_logout() (callback)
}
////////////////////
////////////////////
////////////////////
unsigned int User::get_id() const {
    return id;
}
////////////////////
std::string User::get_name() const {
    return name;
}
////////////////////
user_role User::get_role() const {
    return role;
}
////////////////////
std::string User::get_role_string() const {
    switch(this->role) {
        case user_role::guest: return "Guest"; break;
        case user_role::buyer: return "Buyer"; break;
        case user_role::seller: return "Seller"; break;
        default: return ""; break;
    }
}
////////////////////
////////////////////
////////////////////
Cart * User::get_cart() const {
    return Cart::get_singleton();
}
////////////////////
Order * User::get_order(unsigned int index) const {
    if(index > (order_list.size()-1)) throw std::runtime_error("(User::get_order): attempt to access invalid index");
    return order_list[index];
}
////////////////////
unsigned int User::get_order_count() const {
    return order_list.size();
}
////////////////////
////////////////////
////////////////////
////////////////////
bool User::is_guest() const {
    if(is_logged()) return false;
    return true; // guests (buyers) are not required to register // guests are buyers by default, except their data is not stored
}
////////////////////
bool User::is_buyer() const// buyer and guests are not required to register, only sellers
{
    if(id <= 0) return false;
    DB db("neroshop.db");
    int role_id = db.get_column_integer("users", "role_id", "id = " + std::to_string(this->id));//std::string role = db.get_column_text("users", "role", "id = " + std::to_string(this->id));
    if(role_id <= 0) return false;//if(String::lower(role) != "buyer" || role.empty()) return false;
    if(role_id != 1) return false; // 1 = buyer
    db.close();
    return true;
}
////////////////////
bool User::is_seller() const
{
    if(id <= 0) return false;//if(String::lower(this->name) == "guest") return false; // reserved name "Guest" for guests only
    DB db("neroshop.db");
    int role_id = db.get_column_integer("users", "role_id", "id = " + std::to_string(this->id));//std::string role = db.get_column_text("users", "role", "id = " + std::to_string(this->id));
    if(role_id <= 0) return false;//if(role.empty() || String::lower(role) != "seller") return false;
    if(role_id != 2) return false; // 2 = seller
    db.close();
    return true;
}
////////////////////
bool User::is_online() const
{
    return false;
}
////////////////////
bool User::is_registered() const {
    DB db("neroshop.db");
	// if table Users does not exist, that means no accounts are registered
	if(!db.table_exists("users")) return false; // if table Users does not exist, no accounts are registered
	// confirm that this user's id is in the db (to further prove that they are registered)
	int id = db.get_column_integer("users", "id", "id = " + std::to_string(get_id()));
	if(id <= 0) return false;
	db.close(); // always remember to close db when done :)
    return true;
}
////////////////////
bool User::is_registered(const std::string& name) { // no need to login to prove user is registered, just need to check the db
    // an empty username is ALWAYS invalid
    if(name.empty()) return false;
    DB db("neroshop.db");
	// if table Users does not exist, that means no accounts are registered
	if(!db.table_exists("users")) return false;
	// confirm that this user's name is in the db (to prove that they are registered)
	std::string user = db.get_column_text("users", "name", "name = " + DB::to_sql_string(String::lower(name)));
	if(user.empty()) return false;
	if(user != String::lower(name)) return false;
	db.close(); // always remember to close db when done :)
    return true;
}
////////////////////
bool User::is_logged() const
{
    return logged;
}
////////////////////
bool User::has_email() const {
    if(is_guest()) return false;
    DB db("neroshop.db");
    std::string email_hash = db.get_column_text("users", "email_hash", "id = " + std::to_string(get_id()));
    if(email_hash.empty()) return false;
    db.close();
    return true;
}
////////////////////
////////////////////
////////////////////
// callbacks
////////////////////
//User * User::on_login(const std::string& username) {return nullptr;} // this function does nothing
////////////////////
void User::on_order_received() {} // for sellers to implement // this function does nothing
////////////////////
void User::on_logout() {
    if(is_guest()) return; // guests don't have an account so therefore they cannot logout
    // do something when logged is set to false ...
    neroshop::print("You have logged out");
}
////////////////////
////////////////////
////////////////////
