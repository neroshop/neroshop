// filename: cart.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef CART_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define CART_HPP_NEROSHOP

#include "item.hpp"
#include "db.hpp"
#include <algorithm> // for std::find
#include <chrono>    // std::put_time
#include <sstream>   // std::stringstream
#include <iomanip>   // std::put_time

namespace neroshop {
class Cart {
public: // can be accessed by any class or function 
    Cart();
    ~Cart();
    // normal
    bool open() const;
    void add(const neroshop::Item& item, unsigned int quantity = 1); // quantity is 1 by default // Item * get_item(index);
    //void add_new(unsigned int item_id, unsigned int quantity = 1);
    void remove(const neroshop::Item& item, unsigned int quantity = 1);
    void remove(unsigned int index, unsigned int quantity = 1);
    void empty(); // remove all items from cart
    void move_to_wishlist();
    void save_for_later();
    void change_quantity(const neroshop::Item& item, unsigned int quantity); // set_quantity is private so you can only change item quantity from this function
    void shift_up(const neroshop::Item& item);
    void shift_down(const neroshop::Item& item);
	void swap_positions(const neroshop::Item& item1, const neroshop::Item& item2);
	void checkout(); // user's cart contents impact inventory availability. Only after purchase will actual inventory decrease
	static bool create_db();
	// getters
	double get_seller_subtotal_price(unsigned int seller_id = 0) const;
	double get_subtotal_price() const; // This is the total price for each product in the cart (combined)
	//double get_total_price() const; // This is the total price for all products in cart (combined with shipping and taxes)
	unsigned int get_total_quantity() const; // same as get_items_count; returns the total cart quantity	
	static unsigned int get_total_quantity_db();
	double get_total_weight() const; // returns total weight of all items in cart
	double get_seller_total_discount(unsigned int seller_id = 0) const;
	double get_total_discount() const; // coupons can be applied while item is in cart
	unsigned int get_items_count() const; // number of items in the cart based on quantity
	unsigned int get_contents_count() const; // returns number of items in cart.contents
	neroshop::Item * get_item(unsigned int index) const; //unsigned int get_id() const;//static std::string get_file();
	// singleton
	static Cart * get_singleton();
	// boolean
	bool is_empty() const;
    bool is_full() const; // cart is full (has reached max items)
	bool in_cart(unsigned int item_id) const;//(const neroshop::Item& item) const;
	//bool validate_item(const neroshop::Item& item) const;
	// friends
	friend class Buyer; // buyer can access cart's private members
	friend class Order;
private: // can be accessed by only this class (cannot even be inherited)
    std::vector <neroshop::Item *> contents;//protected: // cannot be accessed outside of class but by a derived class (subclass)
    unsigned int max_items; // cart can only hold up to 10 items
    unsigned int max_quantity; // the max quantity each item can add up to is 100, so 10 items can have 10 quantity each, making the total number of items 100 //unsigned int id;
    static Cart * cart_obj; // singleton object
    static void load(const neroshop::Item& item, unsigned int quantity); // loads cart.db on app start
    static void add_db(unsigned int item_id); // adds item to cart table for first time
    static void remove_db(unsigned int item_id); // removes item from cart table
};
}
#endif

/*
// cart should set the quantity of the item, rather than the item itself
once you "place_order" during checkout, the qr code/ address should pop up and say: "awaiting payment"
// when order is complete, set item quantity back to zero
*/
/*
// example usage:
// create some items
Item * lite = new Item("Nintendo Switch Lite - Blue");
Item * ps5 = new Item("Sony Playstation 5");
// create a cart
Cart * cart = new Cart();
// add items to the cart
cart->add(*lite);
cart->add(*ps5);
cart->add(*lite); // already added to cart, so the quantity will only increase by 1
// add more items
cart->add(*lite);
cart->add(*lite);
cart->add(*lite);
cart->add(*lite);
cart->add(*lite);
cart->add(*lite);
cart->add(*lite);
// remove item from cart
cart->remove(*lite, 3); // will remove 3 of this item, with 7 items remaining
cart->remove(*lite, 6); // remove the rest of the switch lite
cart->remove(*ps5);
// debug - 10 unique items or items with a combined quantity of 100 (max)
std::cout << "Total quantity of cart items: " << cart->get_items_count() << std::endl;
std::cout << "Total number of unique cart items: " << cart->get_contents_count() << std::endl;
*/
