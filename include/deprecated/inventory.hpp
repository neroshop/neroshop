// filename: inventory.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead

#ifndef INVENTORY_HPP_NEROSHOP
#define INVENTORY_HPP_NEROSHOP

#include "item.hpp"
#include "db.hpp"

namespace neroshop {
class Inventory {
    Inventory();
    ~Inventory();
    static bool load(); // loads all items in inventory   
    // getters 
    //std::vector<unsigned int> get_items(unsigned int seller_id);
    //std::vector<unsigned int> get_item();
    //static std::string get_item_name_by_id(unsigned int item_id);
    // boolean
    static bool in_stock(unsigned int item_id);
    static bool has_db();
    // friend
    friend class Seller; // seller can access inventory's private classes
private:
    static void register_item(); // registers an item to the database    
    void add(const Item& item, unsigned int stock_qty, double sales_price); // adds item to the inventory
};
}
#endif
