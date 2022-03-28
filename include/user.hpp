// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef USER_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define USER_HPP_NEROSHOP

#include <iostream>
#include "validator.hpp" // also includes db.hpp
#include "order.hpp"
#include "client.hpp"
//namespace neroshop { // https://stackoverflow.com/questions/623903/defining-a-class-within-a-namespace
enum class user_account_type : unsigned int { guest, buyer, seller }; // guest is 0 by default // enum classes help avoid polluting the scope (either global or namespace) //https://stackoverflow.com/a/32953854 // https://stackoverflow.com/questions/9630744/should-you-declare-enums-inside-or-outside-a-class // user_account_type ut = user_account_type::buyer;

namespace neroshop {
class User { // base class of seller and buyer
public: // bool is_buyer, bool is_seller, bool is_registered, bool is_logged_in, etc.
    User();
    virtual ~User(); // by making this virtual both base and derived class destructors will be called instead of just the base destructor alone
    // only sellers can both sell and buy; 
    // all users (guests, buyers, and sellers) can buy
    // account-related stuff
    void rate_seller(unsigned int seller_id, int score, std::string comments = ""); // seller score (0-1) // use int and NOT unsigned int 'cause unsigned int assumes the arg will never be negative number, but when arg is negative, it converts it to some random positive number
    void rate_item(unsigned int item_id, int stars, std::string comments = ""); // star ratings (1-5)    
    void convert(); // converts buyer to seller //revert(); // I don't see the reason for degrading your account after an upgrade hehe ...
    //void report_user(const User& user, const std::string& reason); // report a user
    void delete_account();
    // favorite-or-wishlist-related stuff
    void add_to_favorites(unsigned int item_id);
    void add_to_favorites(const neroshop::Item& item);
    void remove_from_favorites(unsigned int item_id);
    void remove_from_favorites(const neroshop::Item& item);
    // cart-related stuff
    void add_to_cart(const neroshop::Item& item, int quantity = 1); // use int and NOT unsigned int 'cause unsigned int assumes the arg will never be negative number, but when arg is negative, it converts it to some random positive number
    void remove_from_cart(const neroshop::Item& item, int quantity = 1);
    // order-related stuff
    neroshop::Order * create_order(/*unsigned int seller_id, */const std::string& shipping_address);// const;//void create_order();
    // avatar-related stuff
    void upload_avatar(const std::string& filename);
    void delete_avatar();
    // setters
    // getters
    unsigned int get_id() const;
    std::string get_name() const;
    user_account_type get_account_type() const;
    std::string get_account_type_string() const;
    // buyer functions (getters)
    neroshop::Cart * get_cart() const; // uses default cart (singleton)
    neroshop::Order * get_order(unsigned int index) const;
    unsigned int get_order_count() const;
    // boolean
    bool is_guest() const; // return (user_account_type == "guest") // 
    bool is_buyer() const; // return (user_account_type == "buyer") // 
    bool is_seller() const;  // return (user_account_type == "seller")
    bool is_online() const; // online does not mean logged_in
    bool is_registered() const;
    static bool is_registered(const std::string& name);
    bool is_logged() const; // the same for every derived class // user has entered their login information
    bool has_email() const;
    // boolean (item and order related)
    bool has_purchased(unsigned int item_id); // checks if an item was previously purchased or not
    bool has_favorited(unsigned int item_id); // checks if an item is in a user's favorites or wishlist
    // a virtual function can be overridden in a derived class
    // a pure virtual function means derived class must have its own definition
    // final can only be used on a virtual function, even one that is inherited // https://en.cppreference.com/w/cpp/language/final
    //enum user_account_type {buyer, seller};  
    // singleton
    static User * get_singleton();      
    // callbacks
    void on_registration(const std::string& name); // on registering an account
    //virtual User * on_login(const std::string& username);// = 0; // load all data: orders, reputation/ratings, settings // for all users
    void on_checkout();//(const neroshop::Order& order); // for all users
    virtual void on_order_received(); // for sellers only
    void on_logout();
    // friends
    // ...
protected: // can only be accessed by classes that inherit from class User (even instants of the bass class User cannot call these functions unless you dynamically cast them into a derived class)
    void set_id(unsigned int id);
    void set_name(const std::string& name); // the same for every derived class 
    void set_account_type(user_account_type account_type); // either buyer or seller // the same for every derived class 
    void set_logged(bool logged); // the same for every derived class
    void set_online(bool online);
    // loading into memory so we don't always have to fetch from the database within the same session
    void load_orders(); // on login, load all orders this user has made so far
    void load_favorites(); // on login, load favorites
    static void set_singleton(const User& user);
private:
    unsigned int id;
    std::string name;
    user_account_type account_type; // seller, buyer (guest)
    bool logged; // determines whether user is logged in or not//bool online;
    std::vector<neroshop::Order *> order_list;
    std::vector<unsigned int> favorites_list;
    static neroshop::User * user; // singleton object
};
}
#endif // check if user has internet, and user is logged_in
/*
// converting a user to a seller
        db->column("Users", "account_type", "TEXT", "ADD");
        db->update("Users", "account_type", "'Seller'", "id = 3");
 
                //std::cout << "is_user_logged_in: " << user->is_logged() << std::endl;
                //std::cout << "is_user_registered: " << user->is_registered() << std::endl;
                //std::cout << "is_user_guest: " << user->is_guest() << std::endl;
                //std::cout << "is_user_buyer: " << user->is_buyer() << std::endl;
                //std::cout << "is_user_seller: " << user->is_seller() << std::endl;           
*/
