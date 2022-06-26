#include "item.hpp"

////////////////////
neroshop::Item::Item() : id(0)//, quantity(0), price(0.00), weight(1.0), size(std::make_tuple<double, double, double>(0.0, 0.0, 0.0)), discount(0.0), category("unspecified")/* or none */, condition("new") {}// name and desc and empty strings by default
{}
////////////////////
neroshop::Item::Item(unsigned int id) {//: Item() { // for registered items that already have an id
    set_id(id);
}
////////////////////
neroshop::Item::Item(const std::string& name, const std::string& desc, double price, double weight, double length, double width, double height, const std::string& condition, const std::string& product_code) : Item() // quantity is set by cart
{
    register_item(name, desc, price, weight, length, width, height, condition, product_code);
}
////////////////////
neroshop::Item::Item(const std::string& name, const std::string& desc, double price, double weight, const std::tuple<double, double, double>& size, const std::string& condition, const std::string& product_code) : Item() // quantity is set by cart
{
    register_item(name, desc, price, weight, std::get<0>(size), std::get<1>(size), std::get<2>(size), condition, product_code);  
}        
////////////////////
////////////////////
////////////////////
neroshop::Item::~Item() {
#ifdef NEROSHOP_DEBUG0
    std::cout << "item deleted\n";
#endif    
}
////////////////////
void neroshop::Item::register_item(const std::string& name, const std::string& description, double price, double weight, double length, double width, double height, const std::string& condition, const std::string& product_code) {

}
////////////////////
void neroshop::Item::deregister_item() {
    ////delete_all_upload_images();//delete_upload_image();
}
////////////////////
void neroshop::Item::create_table(void) {
    create_categories_and_subcategories_table();
    create_item_table();
}
////////////////////
void neroshop::Item::create_categories_and_subcategories_table(void) {

}
////////////////////
void neroshop::Item::create_item_table(void) {
}
////////////////////
// DELETE THIS FUNCTION AS WE NO LONGER HAVE A USE FOR IT
void neroshop::Item::register_item(const neroshop::Item& item) { 

}
////////////////////
// seller should have the option to upload videos too (when videos are supported)
void neroshop::Item::upload(const std::string& filename) { 

}    
////////////////////
// the first image of an item will be used as the thumbnail
Image * neroshop::Item::get_upload_image(int index) const {

    return nullptr;
}
////////////////////
void neroshop::Item::delete_upload_image(int index) {

}
////////////////////
void neroshop::Item::delete_all_upload_images(void) {
    
}
////////////////////
int neroshop::Item::get_image_count() const {

    return 0;    
}
////////////////////
////////////////////
////////////////////
void neroshop::Item::set_id(unsigned int id) {
    this->id = id;
}
////////////////////
void neroshop::Item::set_quantity(unsigned int quantity, unsigned int cart_id) {

}
////////////////////
void neroshop::Item::set_quantity(unsigned int item_id, unsigned int quantity, unsigned int cart_id) {

}
////////////////////
void neroshop::Item::set_name(const std::string& name) {

}
////////////////////
void neroshop::Item::set_name(unsigned int item_id, const std::string& name) {

}
////////////////////
void neroshop::Item::set_description(const std::string& description) {

}
////////////////////
void neroshop::Item::set_description(unsigned int item_id, const std::string& description) {

}
////////////////////
void neroshop::Item::set_price(double price) {

}
////////////////////
void neroshop::Item::set_price(unsigned int item_id, double price) {

}
////////////////////
////////////////////
void neroshop::Item::set_weight(double weight) {

}
////////////////////
void neroshop::Item::set_weight(unsigned int item_id, double weight) {

}
////////////////////
void neroshop::Item::set_size(double l, double w, double h) {

}
////////////////////
void neroshop::Item::set_size(unsigned int item_id, double l, double w, double h) {

}
////////////////////
////////////////////
void neroshop::Item::set_size(const std::tuple<double, double, double>& size) {

}
////////////////////
void neroshop::Item::set_size(unsigned int item_id, const std::tuple<double, double, double>& size) {

}
////////////////////
void neroshop::Item::set_discount(double discount) {

}
////////////////////
void neroshop::Item::set_discount(unsigned int item_id, double discount) {

}
////////////////////
void neroshop::Item::set_discount_by_percentage(double percent) { // converts a percentage to a price then sets the price as the discount

}
////////////////////
void neroshop::Item::set_discount_by_percentage(unsigned int item_id, double percent) {

}
////////////////////
void neroshop::Item::set_condition(const std::string& condition) {

}
////////////////////
void neroshop::Item::set_condition(unsigned int item_id, const std::string& condition) {

}
////////////////////
void neroshop::Item::set_product_code(const std::string& product_code) {

}
////////////////////
void neroshop::Item::set_product_code(unsigned int item_id, const std::string& product_code) {

}
////////////////////
////////////////////
// getters
////////////////////
////////////////////
unsigned int neroshop::Item::get_id() const {
    return id; // should be 0 by default for unregistered items
}
////////////////////
std::string neroshop::Item::get_name() const {

    return "";    
}
////////////////////
std::string neroshop::Item::get_name(unsigned int item_id) {

    return "";    
}
////////////////////
std::string neroshop::Item::get_description() const {

    return "";
}
////////////////////
std::string neroshop::Item::get_description(unsigned int item_id) {

    return "";
}
////////////////////
double neroshop::Item::get_price() const {

    return 0.0;
}
////////////////////
double neroshop::Item::get_price(unsigned int item_id) { // original price (list price)

    return 0.0;
}
////////////////////
unsigned int neroshop::Item::get_quantity(unsigned int cart_id) const {

    return 0;    
}
////////////////////
unsigned int neroshop::Item::get_quantity(unsigned int item_id, unsigned int cart_id) {

    return 0;
}
////////////////////
////////////////////
////////////////////
double neroshop::Item::get_weight() const {

    return 0.0;    
}
////////////////////
double neroshop::Item::get_weight(unsigned int item_id) {

    return 0.0;//0.0f;    
}
////////////////////
std::tuple<double, double, double> neroshop::Item::get_size() const {

    return std::make_tuple(0, 0, 0);
}
////////////////////
std::tuple<double, double, double> neroshop::Item::get_size(unsigned int item_id) {

    return std::make_tuple(0, 0, 0);
}
////////////////////
double neroshop::Item::get_discount(unsigned int seller_id) const {
    return get_seller_discount(seller_id); // item discount is determined by seller
}
////////////////////
double neroshop::Item::get_discount(unsigned int item_id, unsigned int seller_id) {
    return get_seller_discount(item_id, seller_id); // item discount is determined by seller
}
////////////////////
std::string neroshop::Item::get_condition() const {

    return "";
}
////////////////////
std::string neroshop::Item::get_condition(unsigned int item_id) {

    return "";
}
////////////////////
//std::string get_subcategory() const {/*return subcategory;*/}
////////////////////
std::string neroshop::Item::get_product_code() const {

    return "";
}
////////////////////
std::string neroshop::Item::get_product_code(unsigned int item_id) {

    return "";
}
////////////////////
////////////////////
// ratings
////////////////////
int neroshop::Item::get_ratings_count() const {

    return 0;    
}
////////////////////
int neroshop::Item::get_ratings_count(unsigned int item_id) {

    return 0;  
}
////////////////////
int neroshop::Item::get_star_count(int star_number) {

    return 0;    
}
////////////////////
int neroshop::Item::get_star_count(unsigned int item_id, int star_number) {

    return 0;    
}
////////////////////
float neroshop::Item::get_average_stars() const {

    return 0.0f;
}
////////////////////
float neroshop::Item::get_average_stars(unsigned int item_id) {

    return 0.0f;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// seller
////////////////////
////////////////////
double neroshop::Item::get_seller_price(unsigned int seller_id) const {

    return 0.0;
}
////////////////////
double neroshop::Item::get_seller_price(unsigned int item_id, unsigned int seller_id) {// seller price (sales price)

    return 0.0;
}
////////////////////
double neroshop::Item::get_seller_discount(unsigned int seller_id) const {

    return 0.0;
}
////////////////////
double neroshop::Item::get_seller_discount(unsigned int item_id, unsigned int seller_id) {

    return 0.0;
}
////////////////////
std::string neroshop::Item::get_seller_condition(unsigned int seller_id) const {

    return "";
}
////////////////////
std::string neroshop::Item::get_seller_condition(unsigned int item_id, unsigned int seller_id) {

    return "";
}
////////////////////
unsigned int neroshop::Item::get_stock_quantity() const {

    return 0;    
}
//////////////////// 
unsigned int neroshop::Item::get_stock_quantity(unsigned int item_id) {

    return 0;    
}
////////////////////
unsigned int neroshop::Item::get_seller_count() const {

    return 0;    
}
////////////////////
unsigned int neroshop::Item::get_seller_count(unsigned int item_id) {

    return 0;    
}
////////////////////
unsigned int neroshop::Item::get_seller_id() const { // might need a little more work

    return 0;    
}
////////////////////
unsigned int neroshop::Item::get_seller_id(unsigned int item_id) { // might need a little more work

    return 0;    
}
////////////////////
std::vector<unsigned int> neroshop::Item::get_seller_ids() const {

    return {};    
}
////////////////////
std::vector<unsigned int> neroshop::Item::get_seller_ids(unsigned int item_id) {

    return {};    
}
////////////////////
////////////////////
////////////////////
bool neroshop::Item::is_registered() const {

    return false;    
}
////////////////////
bool neroshop::Item::is_registered(unsigned int item_id) {

    return false;    
}
////////////////////
////////////////////
bool neroshop::Item::in_stock() const {

    return false;    
}
////////////////////
bool neroshop::Item::in_stock(unsigned int item_id) {

    return false;    
}
////////////////////
////////////////////
bool neroshop::Item::in_cart(/*unsigned int cart_id*/) const {

    return true;
}
////////////////////
bool neroshop::Item::in_cart(unsigned int item_id/*, unsigned int cart_id*/) {

    return true;
}
////////////////////
////////////////////
////////////////////
////////////////////
void neroshop::Item::show_info() {
    std::cout << "Item name: " << get_name() << std::endl;
    std::cout << "id: " << get_id() << std::endl;
    std::cout << "desc: " << get_description() << std::endl;
    //std::cout << "quantity: " << get_quantity() << std::endl;
    std::cout << "price: " << get_price() << std::endl;
    std::cout << "weight: " << get_weight() << std::endl;
    //std::cout << "size: " << item.get_size() << std::endl;
    //std::cout << "discount: " << get_discount() << std::endl;
    std::cout << "condition: " << get_condition() << std::endl;
    std::cout << "product_code: " << get_product_code() << std::endl;
    /*std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;
    std::cout << ": " << item.get_() << std::endl;*/
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
