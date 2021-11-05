#include "../include/inventory.hpp"
////////////////////
neroshop::Inventory::Inventory() 
{}
////////////////////
neroshop::Inventory::~Inventory() 
{}
////////////////////
////////////////////
bool neroshop::Inventory::load() {
    // get number of items in table Inventory
    //std::cout << "number of items in table (Item): " << db->get_column_integer("Item", "COUNT(*)") << std::endl;
    //std::cout << "number of items in table (Inventory): " << db->get_column_integer("Inventory", "COUNT(*)") << std::endl;
    return true;
}
////////////////////
//bool in_inventory(unsigned int item_id, unsigned int inventory_id) {}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
