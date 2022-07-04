#include "cart.hpp"

////////////////////
neroshop::Cart::Cart() : id(0), max_items(10), max_quantity(100) {} // cart can only hold up to 10 unique items // cart items can only add up to 100 qty
////////////////////
neroshop::Cart::~Cart() {
    contents.clear(); // this should reset (delete) all cart items
#ifdef NEROSHOP_DEBUG
    std::cout << "cart deleted\n";
#endif
}
////////////////////
////////////////////
////////////////////
// normal
////////////////////
void neroshop::Cart::add(unsigned int item_id, int quantity) {

}
////////////////////
void neroshop::Cart::add(const neroshop::Item& item, int quantity) {
    add(item.get_id(), quantity);
}
////////////////////
void neroshop::Cart::remove(unsigned int item_id, int quantity) {

}
////////////////////
void neroshop::Cart::remove(const neroshop::Item& item, int quantity) {
    remove(item.get_id(), quantity);
}
////////////////////
////////////////////
////////////////////
bool neroshop::Cart::open() const {

    return true;        
}
////////////////////
void neroshop::Cart::load(const neroshop::Item& item, unsigned int quantity) { // load pre-existing cart data
    // be sure item is in stock
    if(!item.in_stock()) 
    {   // remove item from cart if it is no longer in stock
        NEROSHOP_TAG_OUT std::cout << "\033[1;91m" << item.get_name() << " is out of stock" << "\033[0m" << std::endl; // An item that was in your cart is now out of stock
        const_cast<neroshop::Item&>(item).set_quantity(0, 0); // set quantity to 0
        remove_db(item.get_id()); // remove item_id from table Cart
        NEROSHOP_TAG_OUT std::cout << "\033[1;91m" << item.get_name() << " (" << quantity << ") removed from cart" << "\033[0m" << std::endl;
        return; // exit function
    }
    unsigned int stock_qty = item.get_stock_quantity(); // get stock quantity
    if(quantity >= stock_qty) quantity = stock_qty; // quantity cannot be more than what is in stock
    // load the item and quantity ...
    std::shared_ptr<neroshop::Item> cart_item(&const_cast<neroshop::Item&>(item));
    this->contents.push_back(cart_item); // store existing item
    const_cast<neroshop::Item&>(item).set_quantity(quantity, 0); // save existing item_qty
    NEROSHOP_TAG_OUT std::cout << "\033[1;32m" << item.get_name() << " (id: " << item.get_id() << ", qty: " << quantity << ") has been loaded into cart" << "\033[0m" << std::endl;
}
////////////////////
void neroshop::Cart::add_to_guest_cart(unsigned int item_id, int quantity) {

}
////////////////////
void neroshop::Cart::add_to_guest_cart(const neroshop::Item& item, int quantity) { // good!
    add_to_guest_cart(item.get_id(), quantity);
}
//////////////////// blue: << "\033[0;34m"
void neroshop::Cart::remove_from_guest_cart(unsigned int item_id, int quantity) {

}
////////////////////
void neroshop::Cart::remove_from_guest_cart(const neroshop::Item& item, int quantity) {
    // if item is not in cart, exit function since there is nothing to remove
    if(!in_cart(item.get_id())) return;
    // make sure quantity is not more than item_quantity (cannot remove more than that which is in the cart)
    unsigned int item_qty = item.get_quantity(0);
    if(quantity >= item_qty) quantity = item_qty;
    // if item is already in cart
    // if item_quantity is more than zero (0), reduce item_quantity by 1 or whatever the specified quantity is
    if(item_qty > 0) 
    {
        const_cast<neroshop::Item&>(item).set_quantity(item_qty - quantity, 0);//(item.get_quantity() - quantity);
        item_qty = item.get_quantity(0); // item_qty has been updated so get the updated item_qty
        NEROSHOP_TAG_OUT std::cout << "\033[0;91m" << item.get_name() << " (" << quantity << ") removed from cart" << "\033[0m" << std::endl;
    }
    // if item_quantity is zero (0), delete item from cart
    if(item_qty <= 0) 
    {
        const_cast<neroshop::Item&>(item).set_quantity(0, 0); // first, make sure item quantity is exactly zero (0)
        // now remove it from cart.contents
        for(int i = 0; i < contents.size(); i++) {
            if(item.get_id() == contents[i]->get_id()) {contents.erase(contents.begin()+i);}//std::cout << "cart [" << i << "]: " << contents[i]->get_name() << std::endl;
        }
        // remove the item from the database as well
        remove_db(item.get_id());
    }
    // if cart is empty, clear the contents: if(is_empty()) empty();
#ifdef NEROSHOP_DEBUG
    NEROSHOP_TAG_OUT std::cout << "\033[0;97m" << "Total items in cart: " << get_items_count() << "\033[0m" << std::endl;
#endif            
    if(is_empty()) neroshop::print("Cart is empty");
}
////////////////////
/*void neroshop::Cart::remove_from_guest_cart(unsigned int index, int quantity) {
    if(index > (contents.size()-1)) throw std::runtime_error(std::string("\033[0;31m") + "neroshop::Cart::remove(unsigned int, unsigned int): attempting to access invalid index" + std::string("\033[0m"));
    remove_from_guest_cart(*contents[index], quantity); // remove item at an index
}*/
////////////////////
void neroshop::Cart::empty() {

    contents.clear();
}
////////////////////
////////////////////
////////////////////
void neroshop::Cart::change_quantity(const neroshop::Item& item, int quantity) {
    if(!in_cart(item.get_id())) return; // you can only change the qty of items that are already inside of the cart    
    // check if item is in stock
    if(!item.in_stock()) {NEROSHOP_TAG_OUT std::cout << "\033[1;91m" << item.get_name() << " is out of stock" << "\033[0m" << std::endl; return;}
    int stock_qty = item.get_stock_quantity(); // get stock quantity
    if(quantity >= stock_qty) {quantity = stock_qty; NEROSHOP_TAG_OUT std::cout << "Only " << "\033[1;91m" << stock_qty << "\033[0m " << item.get_name() << "s left in stock" << std::endl;} // make sure quantity is not more than the amount in stock
    // if item quantity is the same as the specified amount, exit function
    unsigned int item_qty = item.get_quantity(0);
    if(quantity == item_qty) return; // there is really nothing to change here since item_qty and specified quantity are both the same value
    // if cart is full, but user still wants to increase the item_qty
    if(is_full() && (quantity > 0)) {NEROSHOP_TAG_OUT std::cout << "\033[0;33m" << "Cart is full, and you still want to add more shit to it, bruh?" << "\033[0m" << std::endl;return;}
    if(quantity < 0) quantity = 0; // quantity cannot be negative
    if(quantity > max_quantity) quantity = max_quantity; // quantity cannot exceed max_quantity
    // if cart_qty added with the new quantity exceeds max_quantity (cart does not have to be full)
    unsigned int cart_qty = get_total_quantity();
    if((cart_qty + quantity) > max_quantity)
    {
        // adjust the quantity so it can fit
        // (100+1)-2=99 ((max_quantity+item_qty)-items_count=quantity)(you need at least a quantity of 99 to fit everything into the cart)
        quantity = (max_quantity + item_qty) - cart_qty;
        // cart can no longer fit any more items
        NEROSHOP_TAG_OUT std::cout << "\033[0;33m" << "Cart is full (cannot fit any more items)" << "\033[0m" << std::endl;
        // cart is full but the cart item's previous quantity is higher than the new quantity
        //if(item.get_quantity() > quantity) {return;} // cart is already full, so no need to increase the qty. Exit the function!
        //return; // exit the function
    }
    // if quantity of item is set to 0 or less, remove the item from the cart (remove() will auto set the quantity to 0)
    if(quantity <= 0) { remove_from_guest_cart(item, max_quantity); return;} // remove all of it from the cart
    // with all obstacles out of the way, change the qty of the item
    const_cast<neroshop::Item&>(item).set_quantity(quantity, 0); // changes quantity in database as well
#ifdef NEROSHOP_DEBUG
    neroshop::print(item.get_name() + " quantity has changed to \033[0;36m" + std::to_string(quantity) + "\033[0m");
    NEROSHOP_TAG_OUT for(int i = 0; i < contents.size(); i++) {
        item_qty = neroshop::Item::get_quantity(contents[i]->get_id(), 0);
        std::string item_name = contents[i]->get_name();
        std::cout << "Cart [" << i << "]: " << item_name << " (" << item_qty << ")" << std::endl;
    }
    //NEROSHOP_TAG_OUT std::cout << "\033[0;97m" << "Total items in cart: " << get_items_count() << "\033[0m" << std::endl;
#endif
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
double neroshop::Cart::get_seller_subtotal_price(unsigned int seller_id) const { 
    double seller_price = 0.00;
    for(int i = 0; i < contents.size(); i++) {
        seller_price += contents[i]->get_quantity(0) * contents[i]->get_seller_price(seller_id); // if seller_id is 0, it will return a random seller's price
    }
    return seller_price;
}
////////////////////
double neroshop::Cart::get_subtotal_price() const {
    double items_price = 0.00; // price of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_price += contents[i]->get_quantity(0) * contents[i]->get_price();
    }
    return items_price;
}
////////////////////
////////////////////
unsigned int neroshop::Cart::get_total_quantity() const {

    return 0;
}
////////////////////
unsigned int neroshop::Cart::get_total_quantity_db() { // faster than neroshop::Cart::get_total_quantity() since this does not open the db file multiple times, but only one time
    
    return 0;
}
////////////////////
double neroshop::Cart::get_total_weight() const {
    ///////////////////////////// https://stackoverflow.com/questions/30909020/postgresql-how-to-multiply-two-columns-and-display-result-in-same-query    // https://www.alphacodingskills.com/postgresql/notes/postgresql-operator-multiply.php
    double items_weight = 0.0; // weight of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_weight += contents[i]->get_quantity(0) * contents[i]->get_weight(); // weight should be determined by (items_weight * quantity)  // the more the quantity, the bigger the weight
    }
    //std::cout << "items_weight: " << items_weight << std::endl;
    return items_weight;
}
////////////////////
double neroshop::Cart::get_seller_total_discount(unsigned int seller_id) const {
    double seller_discount = 0.00;
    for(int i = 0; i < contents.size(); i++) {
        seller_discount += contents[i]->get_seller_discount(seller_id);
    }
    return seller_discount;
}
////////////////////
double neroshop::Cart::get_total_discount() const { // ?
    double items_discount = 0.00; // discount of all cart items combined
    for(int i = 0; i < contents.size(); i++) {
        items_discount += 0.00;//contents[i]->get_discount(i);//discounts are determined by sellers
    }
    return items_discount;
}
////////////////////
////////////////////
unsigned int neroshop::Cart::get_items_count() const {
    return get_total_quantity();
}
////////////////////
unsigned int neroshop::Cart::get_contents_count() const {
    return contents.size();
}
////////////////////
unsigned int get_contents_count_db() { // returns number of rows in table Cart

    return 0;
}
////////////////////
////////////////////
////////////////////
neroshop::Item * neroshop::Cart::get_item(unsigned int index) const {
    if(index > (contents.size() - 1)) throw std::out_of_range(std::string(std::string("\033[0;31m") + "(neroshop::Cart::get_item): attempting to access invalid index" + std::string("\033[0m")).c_str());//std::cerr << "\033[0;31m" << "neroshop::Cart::get_item(): attempting to access invalid index" << "\033[0m" << std::endl;//return nullptr;
    return contents[index].get();
}
////////////////////
std::vector<std::shared_ptr<neroshop::Item>> neroshop::Cart::get_contents_list() const {
    return contents;
}
////////////////////
unsigned int neroshop::Cart::get_id() const {
    return id;
}
////////////////////
unsigned int neroshop::Cart::get_owner_id() const {
    if(this->id == 0) return 0; // invalid cart_id means invalid user_id

    return 0;
}
////////////////////
unsigned int neroshop::Cart::get_owner_id(unsigned int cart_id) {
    if(cart_id == 0) return 0;

    return 0;
}
////////////////////
unsigned int neroshop::Cart::get_max_items() const {
    return max_items;
}
////////////////////
unsigned int neroshop::Cart::get_max_quantity() const {
    return max_quantity;
}
////////////////////
////////////////////
////////////////////
bool neroshop::Cart::is_empty() const {
    return contents.empty();//return (db.row_count("cart") == 0);
}
////////////////////
bool neroshop::Cart::is_full() const {
    return ((contents.size() >= max_items) || (get_total_quantity() >= max_quantity)); // either cart.contents has reached its capacity (max_items:10) or all items have a combined quantity of 100 (max_quantity:100)
}
////////////////////
bool neroshop::Cart::in_cart(unsigned int item_id) const {//(const neroshop::Item& item) const {

    return true;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::Cart::create_guest_cart() {

    return true;
}
////////////////////
////////////////////
void neroshop::Cart::add_db(unsigned int item_id) {

}
////////////////////
void neroshop::Cart::remove_db(unsigned int item_id) {

} 
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// checks whether cart exists or not - should be changed to has_cart(int)
bool neroshop::Cart::load_cart(int user_id) {

    return true;
}
////////////////////
////////////////////
void neroshop::Cart::add_to_registered_user_cart(int user_id, unsigned int item_id, int quantity) {

}
////////////////////
void neroshop::Cart::add_to_registered_user_cart(int user_id, const neroshop::Item& item, int quantity) {
    add_to_registered_user_cart(user_id, item.get_id(), quantity);
}
////////////////////
////////////////////
void neroshop::Cart::remove_from_registered_user_cart(int user_id, unsigned int item_id, int quantity) {
}
////////////////////
void neroshop::Cart::remove_from_registered_user_cart(int user_id, const neroshop::Item& item, int quantity) {
    remove_from_registered_user_cart(user_id, item.get_id(), quantity);
}
////////////////////
////////////////////
void neroshop::Cart::create_cart_item_table() {

}
////////////////////
void neroshop::Cart::create_table() {
    create_cart_item_table();
}
////////////////////
void neroshop::Cart::insert_into(int user_id, int item_id, int item_qty, double item_price, float item_weight) {

}
////////////////////
void neroshop::Cart::delete_from(int user_id, int item_id) {

}
////////////////////
////////////////////
void neroshop::Cart::empty(int user_id) {

    contents.clear();
}
////////////////////
////////////////////
int neroshop::Cart::get_total_quantity(int user_id) const {

    return 0;
}
////////////////////
double neroshop::Cart::get_subtotal_price(int user_id) const {

    return 0.0;
}
////////////////////
float neroshop::Cart::get_total_weight(int user_id) const {

    return 0.0f;
}
////////////////////
////////////////////
////////////////////
bool neroshop::Cart::is_empty(int user_id) const {

    return false;
}
////////////////////
bool neroshop::Cart::is_full(int user_id) const {

    return false;
}
////////////////////
bool neroshop::Cart::in_cart(int user_id, unsigned int item_id) const {

    return false;
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
