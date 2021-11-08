// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead

#ifndef SELLER_HPP_NEROSHOP
#define SELLER_HPP_NEROSHOP

#include <cmath> // floor
#include <random>
#include "wallet.hpp" // seller will use wallet to generate a unique subaddress for each tx
#include "user.hpp"
#include "item.hpp"//#include "inventory.hpp" // item.hpp included here//#include "registry.hpp"//deprecated
#include "converter.hpp"

class Seller : public User { // sellers have unlimited access to the inventory
public:
	Seller();
	Seller(const std::string& name);
	~Seller();
	void list_item(unsigned int item_id, unsigned int stock_qty, double sales_price = 0.00, std::string currency = "usd");
	void list_item(const Item& item, unsigned int stock_qty, double sales_price = 0.00, std::string currency = "usd", double discount = 0.00, unsigned int discounted_items = 0, std::string condition = "new"); // adds an item to the inventory
	             // images, price, search_terms
	void check_for_customer_orders();              
	// setters	
	void set_stock_quantity(const Item& item, unsigned int stock_qty);
	void set_stock_quantity(unsigned int item_id, unsigned int stock_qty);
	// create promotions such as: percent_off, free_shipping, giveaway, buy_one_get_one_free, coupon codes(time_limit, expire_date) => https://tinuiti.com/blog/amazon/amazon-promotions-for-sellers/
	// coupon_min_percent = 2%, coupon_max_percent = 90% (amazon is 5%-80%)
	void set_wallet(const Wallet& wallet);// temporary - delete ASAP
	// getters
	unsigned int get_good_ratings() const;
	unsigned int get_bad_ratings() const;
	unsigned int get_ratings_count() const;
	unsigned int get_total_ratings() const;
	unsigned int get_reputation() const; // represents a percentage (of good ratings)
	// get_() const;
	Wallet * get_wallet() const;
	//neroshop::Inventory * get_inventory() const;
	// boolean
	bool is_verified() const; // returns true if seller is verified brand owner
	bool has_listed(unsigned int item_id) const; // returns true if this seller has listed an item
	// callbacks
	static User * on_login(const std::string& username);
	void on_order_received();
private:
    Wallet * wallet;
    //neroshop::Inventory * inventory; // seller will act as the inventory instead
	std::string username;
};

#endif
/*
combining different promotions/coupons:

Lightning Deal Discount: 20%

Coupon Discount: 5%

Total Discount: ($100 * 0.2) + ($100 * 0.05) = $25

*/
/*
	Seller * seller = new Seller();
	std::cout << "good_ratings: " << seller->get_good_ratings() << std::endl;
	std::cout << "bad_ratings: " << seller->get_bad_ratings() << std::endl;
	std::cout << "total_ratings: " << seller->get_total_ratings() << std::endl;
	std::cout << std::endl;
	std::cout << "reputation: " << seller->get_reputation() << "%" << std::endl;
	//std::cout << "good_percentage: " << seller->get_good_percentage() << "%" << std::endl;
	//std::cout << "bad_percentage: " << seller->get_bad_percentage() << "%" << std::endl;
	// total number of sellers
	std::cout << "\nnumber of sellers: " << Registry::get_seller_registry()->get_size() << std::endl;	
*/ // list a product - name, code(upc, ean, isbn, etc.), description/bullet points,
