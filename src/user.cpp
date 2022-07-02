#include "user.hpp"

////////////////////
neroshop::User::User() : id(0), logged(false), account_type(user_account_type::guest), cart(nullptr), order_list({}), favorites_list({}) {
    cart = std::unique_ptr<Cart>(new Cart());
}
////////////////////
neroshop::User::~User()
{
    // destroy cart
    if(cart.get()) cart.reset();
    // clear orders
    order_list.clear(); // this should reset (delete) all orders
    // clear favorites
    favorites_list.clear(); // this should reset (delete) all favorites
#ifdef NEROSHOP_DEBUG
    std::cout << "user deleted\n";
#endif    
}
////////////////////
////////////////////
////////////////////
// buyers can only rate seller they have purchased from!!
void neroshop::User::rate_seller(unsigned int seller_id, int score, std::string comments) { // perfected 99.9%!!

} 
// int seller_id = 2;
// user->rate_seller(seller_id, 1, "This seller rocks!");
// user->rate_seller(seller_id, 0, "This seller sucks!");
////////////////////
////////////////////
void neroshop::User::rate_item(unsigned int item_id, int stars, std::string comments) { // perfected 99%!!!

} 
// user->rate_item(ball.get_id(), 5, "Very bouncy. I love it!");
////////////////////
////////////////////
// account-related stuff here
////////////////////
void neroshop::User::convert() {
    if(is_guest()) return;
    if(is_seller()) { 
        neroshop::print("You are already a seller", 2); 
        return; 
    }
}
// if(user->is_buyer()) user->convert(); // convert buyer to seller
////////////////////
/*void neroshop::User::revert() {
    // convert user from seller to buyer
    UPDATE users SET account_type_id = 1 WHERE id = $1;
    // remove all items listed by this user
    DELETE FROM inventory WHERE seller_id = $1;
}
*/
////////////////////
void neroshop::User::delete_account() {

} // username of deleted accounts can be reused or nah?? :O
////////////////////
////////////////////
////////////////////
// cart-related stuff here
////////////////////
void neroshop::User::add_to_cart(unsigned int item_id, int quantity) {
    cart->add(item_id, quantity);
}
////////////////////
void neroshop::User::add_to_cart(const neroshop::Item& item, int quantity) {
    add_to_cart(item.get_id(), quantity);
}
////////////////////
void neroshop::User::remove_from_cart(unsigned int item_id, int quantity) {
    cart->remove(item_id, quantity);
}
////////////////////
void neroshop::User::remove_from_cart(const neroshop::Item& item, int quantity) {
    remove_from_cart(item.get_id(), quantity);
}
////////////////////
void neroshop::User::clear_cart() {
    if(is_registered()) cart->empty(this->id);
}
////////////////////
void neroshop::User::load_cart() {
    cart->load_cart(this->id);
}
////////////////////
////////////////////
////////////////////
// order-related stuff here
////////////////////
void neroshop::User::create_order(const std::string& shipping_address, std::string contact) {//const {
    // name(first, last), address1(street, p.o box, company name, etc.), address2(apt number, suite, unit, building, floor, etc.) city, zip/postal_code, state/province/region country, optional(phone, email)
    std::shared_ptr<neroshop::Order> order(std::make_shared<neroshop::Order>());//(new neroshop::Order());
    order->create_order(*cart.get(), shipping_address, contact); // we are using crypto, not debit/credit cards so no billing address is needed
    order_list.push_back(order); // whether an order fails or succeeds, store it regardless
}
// cart->add(ball, 2);
// cart->add(ring);
// user->create_order(shipping_addr);
////////////////////
// put this in Buyer::on_login and Seller::on_login
// Guests orders are not saved to the main database
// orders are never deleted, their statuses just change: rejected, failure, delivered, etc.
void neroshop::User::load_orders() {

}
////////////////////
////////////////////
////////////////////
// favorite-or-wishlist-related stuff
////////////////////
void neroshop::User::add_to_favorites(unsigned int item_id) {

}
////////////////////
void neroshop::User::add_to_favorites(const neroshop::Item& item) {
    add_to_favorites(item.get_id());
}
////////////////////
void neroshop::User::remove_from_favorites(unsigned int item_id) {

}
////////////////////
void neroshop::User::remove_from_favorites(const neroshop::Item& item) {
    remove_from_favorites(item.get_id());
}
////////////////////
void neroshop::User::clear_favorites() {

}
////////////////////
void neroshop::User::load_favorites() {

}
////////////////////
////////////////////
////////////////////
// avatar-related stuff here
////////////////////
void neroshop::User::upload_avatar(const std::string& filename) {

}
////////////////////
void neroshop::User::delete_avatar() {

}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void neroshop::User::set_id(unsigned int id) {
    this->id = id;
}
////////////////////
void neroshop::User::set_name(const std::string& name) {
    this->name = name;
}
////////////////////
void neroshop::User::set_account_type(user_account_type account_type) {
    this->account_type = account_type;
}
////////////////////
void neroshop::User::set_logged(bool logged) { // protected function, so only derived classes can use this
    this->logged = logged;
    if(!logged) logout(); // call on_logout() (callback)
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
unsigned int neroshop::User::get_id() const {
    return id;
}
////////////////////
std::string neroshop::User::get_name() const {
    return name;
}
////////////////////
user_account_type neroshop::User::get_account_type() const {
    return account_type;
}
////////////////////
std::string neroshop::User::get_account_type_string() const {
    switch(this->account_type) {
        case user_account_type::guest: return "Guest"; break;
        case user_account_type::buyer: return "Buyer"; break;
        case user_account_type::seller: return "Seller"; break;
        default: return ""; break;
    }
}
////////////////////
////////////////////
neroshop::Cart * neroshop::User::get_cart() const {
    return cart.get();
}
////////////////////
////////////////////
neroshop::Order * neroshop::User::get_order(unsigned int index) const {
    if(index > (order_list.size() - 1)) throw std::out_of_range("neroshop::User::get_order(): attempt to access invalid index");
    return order_list[index].get();
}
////////////////////
unsigned int neroshop::User::get_order_count() const {
    return order_list.size();
}
////////////////////
std::vector<neroshop::Order *> neroshop::User::get_order_list() const {
    std::vector<neroshop::Order *> orders = {};
    for(const auto & order : order_list) {//for(int o = 0; o < order_list.size(); o++) {
        orders.push_back(order.get());//(order_list[o].get());
    }
    return orders;
}
////////////////////
////////////////////
neroshop::Item * neroshop::User::get_favorite(unsigned int index) const {
    if(index > (favorites_list.size() - 1)) throw std::out_of_range("neroshop::User::get_favorites(): attempt to access invalid index");
    return favorites_list[index].get();
}
////////////////////
unsigned int neroshop::User::get_favorites_count() const {
    return favorites_list.size();
}
////////////////////
std::vector<neroshop::Item *> neroshop::User::get_favorites_list() const {
    std::vector<neroshop::Item *> favorites = {};
    for(const auto & item : favorites_list) {//for(int f = 0; f < favorites_list.size(); f++) {
        favorites.push_back(item.get());//(favorites_list[f].get());
    }
    return favorites;
}
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::User::is_guest() const {
    if(is_logged()) return false;
    return true; // guests (buyers) are not required to register // guests are buyers by default, except their data is not stored
}
////////////////////
bool neroshop::User::is_buyer() const// buyer and guests are not required to register, only sellers
{

    return false;    
}
////////////////////
bool neroshop::User::is_seller() const
{

    return false;    
}
////////////////////
bool neroshop::User::is_online() const // a user is not created until they are logged so this function can only be called when a user is logged // guests can also use this function so its a bad idea to check if user is logged
{
    return Client::get_main_client()->connected();// && is_logged()); // user must be both connected to the network and logged in
}
////////////////////
bool neroshop::User::is_registered() const {

    return false;    
}
////////////////////
bool neroshop::User::is_registered(const std::string& name) { // no need to login to prove user is registered, just need to check the db

    return false;    
}
////////////////////
bool neroshop::User::is_logged() const
{
    return logged;
}
////////////////////
bool neroshop::User::has_email() const {
    if(is_guest()) return false;

    return false;    
}
////////////////////
////////////////////
bool neroshop::User::has_purchased(unsigned int item_id) { // for registered users only//if(!is_logged()) { neroshop::print("You are not logged in", 2); return false; }

    return false;    
}
////////////////////
bool neroshop::User::has_purchased(const neroshop::Item& item) {
    return has_purchased(item.get_id());
}
////////////////////
bool neroshop::User::has_favorited(unsigned int item_id) {
    // since we loaded the favorites into memory when the app launched, we should be able to access the pre-loaded favorites and any newly added favorites in the current session without performing any database queries/operations
    for(const auto & favorites : favorites_list) {
        // if any favorites_list items' ids matches "item_id" then return true
        if(favorites->get_id() == item_id) return true;
    }
    return false;////return (std::find(favorites_list.begin(), favorites_list.end(), item_id) != favorites_list.end()); // this is good for when storing favorites as integers (item_ids)
}
////////////////////
bool neroshop::User::has_favorited(const neroshop::Item& item) {
    return has_favorited(item.get_id());
}
////////////////////
////////////////////
////////////////////
// callbacks
////////////////////
//User * neroshop::User::on_login(const std::string& username) {return nullptr;} // this function does nothing
////////////////////
void neroshop::User::on_order_received() {} // for sellers to implement // this function does nothing
////////////////////
void neroshop::User::logout() {
    //edit: guests can definitely logout too//if(is_guest()) return; // guests don't have an account so therefore they cannot logout
    // do something when logged is set to false ...
    // reset private members to their default values
    this->id = 0; // clear id
    this->name.clear(); // clear name
    this->account_type = user_account_type::guest; // set account type to the default
    this->logged = false; // make sure user is no longer logged in
    // disconnect from server
    // print message    
    neroshop::print("You have logged out");
}
////////////////////
////////////////////
////////////////////
