#include "../include/buyer.hpp"

////////////////////
Buyer::Buyer() {} // not used
////////////////////
Buyer::Buyer(const std::string& name) : Buyer() {
    set_name(name);
    //std::cout << "buyer created\n";
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
////////////////////
User * Buyer::on_login(const std::string& username) { // if validator::login(user, pw) returns true, then set User::logged to true
    // retrieve user data from database
    DB db("neroshop.db");
    User * user = new Buyer(username); // create seller obj
    dynamic_cast<Buyer *>(user)->set_logged(true); // protected, so only instance of derived class can call this function
    dynamic_cast<Buyer *>(user)->set_id(db.get_column_integer("users", "id", "name = " + DB::to_sql_string(username)));
    dynamic_cast<Buyer *>(user)->set_role(user_role::buyer); // set the role
#ifdef NEROSHOP_DEBUG
        std::cout << "\033[1;34m(role: " << String::lower(user->get_role_string()) << ", id: " << user->get_id() << ")\033[0m" << std::endl;
#endif    
    ///////////
    // load orders
    dynamic_cast<Buyer *>(user)->load_orders();
    // load wishlists
    // ...
    ///////////
    db.close(); // don't forget to close the db when done!
    return user;
    // static_cast to convert from User to Buyer (static_cast<Buyer *>(user))
    // dynamic_cast to use a User as if it were a Buyer (dynamic_cast<Sprite *>(entity)->is_sprite())
}
////////////////////
////////////////////
