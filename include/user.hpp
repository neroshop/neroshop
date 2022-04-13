// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef USER_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define USER_HPP_NEROSHOP

#include <iostream>
#include "validator.hpp" // db.hpp included here
#include "order.hpp"
#include "client.hpp"

enum class user_account_type : unsigned int { guest, buyer, seller }; // guest is 0 by default // enum classes help avoid polluting the scope (either global or namespace) //https://stackoverflow.com/a/32953854 // https://stackoverflow.com/questions/9630744/should-you-declare-enums-inside-or-outside-a-class // user_account_type ut = user_account_type::buyer;

namespace neroshop {
class User { // base class of seller and buyer // sellers can buy and sell while buyers (including guests) can only buy but cannot sell
public: 
    User();
    virtual ~User(); // by making this virtual both base and derived class destructors will be called instead of just the base destructor alone
    // account-related stuff (will continue adding more account features)
    void rate_seller(unsigned int seller_id, int score, std::string comments = ""); // seller score (0-1) // use int and NOT unsigned int 'cause unsigned int assumes the arg will never be negative number, but when arg is negative, it converts it to some random positive number
    void rate_item(unsigned int item_id, int stars, std::string comments = ""); // star ratings (1-5)    
    void convert(); // converts buyer to seller //void revert(); // I don't see the reason for degrading your account after an upgrade hehe ... //void report_user(const User& user, const std::string& reason); // report a user
    void delete_account();
    void logout();
    // cart-related stuff (50% complete - cart class still needs some more work)
    void add_to_cart(unsigned int item_id, int quantity = 1);
    void add_to_cart(const neroshop::Item& item, int quantity = 1); // use int and NOT unsigned int 'cause unsigned int assumes the arg will never be negative number, but when arg is negative, it converts it to some random positive number
    void remove_from_cart(unsigned int item_id, int quantity = 1);
    void remove_from_cart(const neroshop::Item& item, int quantity = 1);
    void clear_cart();
    // order-related stuff (50% complete - order class still needs some more work)
    void create_order(const std::string& shipping_address, std::string contact = "");// const;//void create_order();
    // favorite-or-wishlist-related stuff (100% complete)
    void add_to_favorites(unsigned int item_id);
    void add_to_favorites(const neroshop::Item& item);
    void remove_from_favorites(unsigned int item_id);
    void remove_from_favorites(const neroshop::Item& item);
    void clear_favorites();
    // avatar-related stuff (10% complete)
    void upload_avatar(const std::string& filename);
    void delete_avatar();
    // setters
    // getters
    // account-related stuff - getters
    unsigned int get_id() const;
    std::string get_name() const;
    user_account_type get_account_type() const;
    std::string get_account_type_string() const;
    // buyer-related stuff - getters
    neroshop::Cart * get_cart() const;
    neroshop::Order * get_order(unsigned int index) const;
    unsigned int get_order_count() const;
    neroshop::Item * get_favorites(unsigned int index) const;
    unsigned int get_favorites_count() const;
    // boolean
    bool is_guest() const;
    bool is_buyer() const;
    bool is_seller() const;
    bool is_online() const; // online does not mean logged in
    bool is_registered() const;
    static bool is_registered(const std::string& name);
    bool is_logged() const; // the same for every derived class // user has entered their login information
    bool has_email() const;
    // item-related stuff - boolean
    bool has_purchased(unsigned int item_id); // checks if an item was previously purchased or not
    bool has_purchased(const neroshop::Item& item); // checks if an item was previously purchased or not
    bool has_favorited(unsigned int item_id); // checks if an item is in a user's favorites or wishlist
    bool has_favorited(const neroshop::Item& item); // checks if an item is in a user's favorites or wishlist
    // a virtual function can be overridden in a derived class
    // a pure virtual function means derived class must have its own definition
    // final can only be used on a virtual function, even one that is inherited // https://en.cppreference.com/w/cpp/language/final
    // singleton
    static User * get_singleton();      
    // callbacks
    void on_registration(const std::string& name); // on registering an account
    //virtual User * on_login(const std::string& username);// = 0; // load all data: orders, reputation/ratings, settings // for all users
    void on_checkout();//(const neroshop::Order& order); // for all users
    virtual void on_order_received(); // for sellers only
    // friends
    // ...
    static void set_singleton(const User& user); // should be under protected:
protected: // can only be accessed by classes that inherit from class User (even instants of the bass class User cannot call these functions unless you dynamically cast them into a derived class)
    void set_id(unsigned int id);
    void set_name(const std::string& name); // the same for every derived class 
    void set_account_type(user_account_type account_type); // either buyer or seller // the same for every derived class 
    void set_logged(bool logged); // the same for every derived class
    void set_online(bool online);
    // loading into memory so we don't always have to fetch from the database within the same session
    void load_cart();
    void load_orders(); // on login, load all orders this user has made so far (this function is called only once per login)
    void load_favorites(); // on login, load favorites (this function is called only once per login)
private:
    unsigned int id;
    std::string name;
    user_account_type account_type; // seller, buyer (guest)
    bool logged; // determines whether user is logged in or not//bool online;
    std::unique_ptr<Cart> cart;
    std::vector<std::shared_ptr<neroshop::Order>> order_list;
    std::vector<std::shared_ptr<neroshop::Item>> favorites_list; // I get errors while trying to use unqieu_ptr so I'm stuck with a shared_ptr container
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
