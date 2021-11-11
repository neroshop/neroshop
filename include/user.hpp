// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef USER_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define USER_HPP_NEROSHOP

#include <iostream>
#include "validator.hpp" // also includes db.hpp
#include "order.hpp"
#include "client.hpp"
//namespace neroshop { // https://stackoverflow.com/questions/623903/defining-a-class-within-a-namespace
enum class user_role : unsigned int {guest, buyer, seller}; // enum classes help avoid polluting the scope (either global or namespace) //https://stackoverflow.com/a/32953854 // https://stackoverflow.com/questions/9630744/should-you-declare-enums-inside-or-outside-a-class // user_role ut = user_role::buyer;

namespace neroshop {
class User { // base class of seller and buyer
public: // bool is_buyer, bool is_seller, bool is_registered, bool is_logged_in, etc.
    User();
    virtual ~User(); // by making this virtual both base and derived class destructors will be called instead of just the base destructor alone
    // only sellers can both sell and buy; 
    // all users (guests, buyers, and sellers) can buy
    void rate_seller(unsigned int seller_id, unsigned int score, std::string comments = ""); // seller score (0-1)
    void rate_item(unsigned int item_id, unsigned int stars, std::string comments = ""); // star ratings (1-5)    
    void convert(); // converts buyer to seller //revert();
    void add_to_cart(const neroshop::Item& item, unsigned int quantity = 1);
    void remove_from_cart(const neroshop::Item& item, unsigned int quantity = 1);
    neroshop::Order * create_order(/*unsigned int seller_id, */const std::string& shipping_address);// const;//void create_order();
    //void report_user(const User& user, const std::string& reason); // report a user
    void delete_account();
    // setters
    // getters
    unsigned int get_id() const;
    std::string get_name() const;
    user_role get_role() const;
    std::string get_role_string() const;
    // buyer functions (getters)
    neroshop::Cart * get_cart() const; // uses default cart (singleton)
    neroshop::Order * get_order(unsigned int index) const;
    unsigned int get_order_count() const;
    // boolean
    bool is_guest() const; // return (user_role == "guest") // 
    bool is_buyer() const; // return (user_role == "buyer") // 
    bool is_seller() const;  // return (user_role == "seller")
    bool is_online() const; // online does not mean logged_in
    bool is_registered() const;
    static bool is_registered(const std::string& name);
    bool is_logged() const; // the same for every derived class // user has entered their login information
    bool has_email() const;
    // a virtual function can be overridden in a derived class
    // a pure virtual function means derived class must have its own definition
    // final can only be used on a virtual function, even one that is inherited // https://en.cppreference.com/w/cpp/language/final
    //enum user_role {buyer, seller};    
    // callbacks
    void on_registration(const std::string& name); // on registering an account
    //virtual User * on_login(const std::string& username);// = 0; // load all data: orders, reputation/ratings, settings // for all users
    void on_checkout();//(const neroshop::Order& order); // for all users
    virtual void on_order_received(); // for sellers only
    void on_logout();
    // friends
protected: // can only be accessed by classes that inherit from class User (even instants of the bass class User cannot call these functions unless you dynamically cast them into a derived class)
    void set_id(unsigned int id);
    void set_name(const std::string& name); // the same for every derived class 
    void set_role(user_role role); // either buyer or seller // the same for every derived class 
    void set_logged(bool logged); // the same for every derived class
    void set_online(bool online);
    void load_orders(); // on login, load all orders this user has made so far
private:
    unsigned int id;
    std::string name;
    user_role role; // seller, buyer (guest)
    bool logged; // determines whether user is logged in or not//bool online;
    std::vector<neroshop::Order *> order_list;
};
}
#endif // check if user has internet, and user is logged_in
/*
// converting a user to a seller
        db->column("Users", "role", "TEXT", "ADD");
        db->update("Users", "role", "'Seller'", "id = 3");
 
                //std::cout << "is_user_logged_in: " << user->is_logged() << std::endl;
                //std::cout << "is_user_registered: " << user->is_registered() << std::endl;
                //std::cout << "is_user_guest: " << user->is_guest() << std::endl;
                //std::cout << "is_user_buyer: " << user->is_buyer() << std::endl;
                //std::cout << "is_user_seller: " << user->is_seller() << std::endl;           
*/
