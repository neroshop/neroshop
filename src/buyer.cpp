#include "buyer.hpp"

////////////////////
neroshop::Buyer::Buyer() {} // not used
////////////////////
neroshop::Buyer::Buyer(const std::string& name) : Buyer() {
    set_name(name);
    //std::cout << "buyer created\n";
}
////////////////////
neroshop::Buyer::~Buyer() {
#ifdef NEROSHOP_DEBUG0
    std::cout << "buyer deleted\n";
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
////////////////////
////////////////////
neroshop::User * neroshop::Buyer::on_login(const std::string& username) { // if validator::login(user, pw) returns true, then set User::logged to true
    neroshop::User * user = new Buyer(username); // create seller obj
    dynamic_cast<Buyer *>(user)->set_logged(true); // protected, so only instance of derived class can call this function
    dynamic_cast<Buyer *>(user)->set_account_type(user_account_type::buyer); // set the account_type
    // load orders
    dynamic_cast<Buyer *>(user)->load_orders();
    // load wishlists
    dynamic_cast<Buyer *>(user)->load_favorites();
    // load cart (into memory)
    if(user->is_registered()) {
        user->get_cart()->load_cart(user->get_id());
    }
    return user;
}
////////////////////
////////////////////
