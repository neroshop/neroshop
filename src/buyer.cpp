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
//void Buyer::convert() {
//    if(is_guest()) {neroshop::print("Guests must create an account in order to convert to a Seller"); return;}
    //if(!db.table_exists("users")) {
        //db.column("users", "ADD", "role_id", "INTEGER");
        //db.column("users", "ADD", "role_name", "TEXT");
    //}
    // update role_id or role
    //db.update("users", "role", "seller", "id = " + std::string(get_id()));
//}
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
