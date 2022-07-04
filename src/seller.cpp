#include "seller.hpp"

neroshop::Seller::Seller() : wallet(nullptr)
{}
////////////////////
////////////////////
neroshop::Seller::Seller(const std::string& name) : Seller() {
    set_name(name);
}
////////////////////
////////////////////
neroshop::Seller::~Seller() {
    // clear customer orders
    customer_order_list.clear(); // will reset (delete) all customer orders
    // destroy wallet
    if(wallet.get()) wallet.reset();
#ifdef NEROSHOP_DEBUG    
    std::cout << "seller deleted\n";
#endif    
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void neroshop::Seller::list_item(unsigned int item_id, unsigned int stock_qty, double sales_price, std::string currency, double discount, unsigned int discounted_items, unsigned int discount_times, std::string discount_expiry, std::string condition)
{

}
////////////////////
void neroshop::Seller::list_item(const neroshop::Item& item, unsigned int stock_qty, double sales_price, std::string currency, double discount, unsigned int discounted_items, unsigned int discount_times, std::string discount_expiry, std::string condition) { // ex. 5% off 10 balls
    list_item(item.get_id(), stock_qty, sales_price, currency, discount, discounted_items, discount_times, discount_expiry, condition);
}
// static_cast<Seller *>(user)->list_item(ball, 50, 8.50, "usd", 0.50, 2, "new"); // $0.50 cents off every 2 balls
////////////////////
void neroshop::Seller::delist_item(unsigned int item_id) {

}
////////////////////
void neroshop::Seller::delist_item(const neroshop::Item& item) {
    delist_item(item.get_id());
}
////////////////////
////////////////////
////////////////////
// the moment the seller logs in, they should be notified that they have a pending transaction from a customer
// and should respond swiftly
// if seller accepts the order, then an address will be generated from seller's wallet and sent to the customer
// if seller rejects the order, their stock_qty is increased by the failed order's qty
void neroshop::Seller::load_customer_orders() {

}
////////////////////
// THIS FUNCTION WILL BE LISTENING FOR ANY NEW (PENDING) ORDERS AT ALL TIMES
void neroshop::Seller::update_customer_orders() { // this function is faster (I think) than load_customer_orders()

}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// setters - item and inventory-related stuff
////////////////////
void neroshop::Seller::set_stock_quantity(unsigned int item_id, unsigned int stock_qty) {

}
////////////////////
void neroshop::Seller::set_stock_quantity(const neroshop::Item& item, unsigned int stock_qty) {
    set_stock_quantity(item.get_id(), stock_qty);
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// setters - wallet-related stuff
////////////////////
void neroshop::Seller::set_wallet(const neroshop::Wallet& wallet) {
    std::unique_ptr<neroshop::Wallet> seller_wallet(&const_cast<neroshop::Wallet&>(wallet));
    this->wallet = std::move(seller_wallet); // unique pointers cannot be copied, but can only be moved // "std::unique_ptr::release()" is a similar function but "std::move()" is better of the two
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
// getters - seller rating system
////////////////////
unsigned int neroshop::Seller::get_good_ratings() const {

    return 0;
}
unsigned int neroshop::Seller::get_bad_ratings() const {

    return 0;	
}
////////////////////
unsigned int neroshop::Seller::get_ratings_count() const {

    return 0;
}
////////////////////
unsigned int neroshop::Seller::get_total_ratings() const {
    return get_ratings_count();
}
////////////////////
unsigned int neroshop::Seller::get_reputation() const {

    return 0;	
}
////////////////////
std::vector<unsigned int> neroshop::Seller::get_top_rated_sellers(unsigned int limit) {

    return {};    
}
////////////////////
////////////////////
////////////////////
// getters - wallet-related stuff
////////////////////
neroshop::Wallet * neroshop::Seller::get_wallet() const {
    return wallet.get();
}
////////////////////
////////////////////
////////////////////
// getters - order-related stuff
////////////////////
unsigned int neroshop::Seller::get_customer_order(unsigned int index) const {
    if(customer_order_list.empty()) return 0;//return nullptr;
    if(index > (customer_order_list.size() - 1)) throw std::out_of_range("neroshop::Seller::get_customer_order(): attempt to access invalid index");
    return customer_order_list[index];
}
////////////////////
unsigned int neroshop::Seller::get_customer_order_count() const {
    return customer_order_list.size();
}
////////////////////
std::vector<int> neroshop::Seller::get_pending_customer_orders() {

    return {};    
}
////////////////////
////////////////////
////////////////////
// getters - sales and statistics-related stuff
////////////////////
unsigned int neroshop::Seller::get_sales_count() const {

    return 0;	
}
////////////////////
unsigned int neroshop::Seller::get_units_sold(unsigned int item_id) const {

    return 0;    
}
////////////////////
unsigned int neroshop::Seller::get_units_sold(const neroshop::Item& item) const {
    return get_units_sold(item.get_id());
}
////////////////////
double neroshop::Seller::get_sales_profit() const {

    return 0.0;
}
////////////////////
double neroshop::Seller::get_profits_made(unsigned int item_id) const {

    return 0.0;
}
////////////////////
double neroshop::Seller::get_profits_made(const neroshop::Item& item) const {
    return get_profits_made(item.get_id());
}
////////////////////
unsigned int neroshop::Seller::get_item_id_with_most_sales() const { // this function is preferred over the "_by_mode" version as it provides the most accurate best-selling item_id result

    return 0;    
}
////////////////////
unsigned int neroshop::Seller::get_item_id_with_most_orders() const {

    return 0;    
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// boolean
////////////////////
bool neroshop::Seller::has_listed(unsigned int item_id) const {

    return false;	
}
////////////////////
bool neroshop::Seller::has_listed(const neroshop::Item& item) const {
    return has_listed(item.get_id());
}
////////////////////
bool neroshop::Seller::has_stock(unsigned int item_id) const {

    return false;    
}
////////////////////
bool neroshop::Seller::has_stock(const neroshop::Item& item) const {
    return has_stock(item.get_id());
}
////////////////////
bool neroshop::Seller::has_wallet() const {
    if(!wallet.get()) return false; // wallet is nullptr
    if(!wallet->get_monero_wallet()) return false; // wallet not opened
    return true;
}
////////////////////
bool neroshop::Seller::has_wallet_synced() const {
    if(!wallet.get()) return false; // wallet is nullptr
    if(!wallet->get_monero_wallet()) return false; // wallet not opened
    if(!wallet->get_monero_wallet()->is_synced()) return false; // wallet not synced to daemon
    return true;
}
////////////////////
////////////////////
////////////////////
// callbacks
////////////////////
neroshop::User * neroshop::Seller::on_login(const std::string& username) { // assumes user data already exists in database
    // create a new user (seller)
    neroshop::User * user = new Seller(username);
    // set user properties retrieved from database
    dynamic_cast<Seller *>(user)->set_logged(true); // protected, so can only be accessed by child class obj   // if validator::login(user, pw) returns true, then set neroshop::User::logged to true
    dynamic_cast<Seller *>(user)->set_account_type(user_account_type::seller);
    // load orders
    dynamic_cast<Seller *>(user)->load_orders();
    // load wishlists
    dynamic_cast<Seller *>(user)->load_favorites();    
    // load customer_orders
    static_cast<Seller *>(user)->load_customer_orders();
    // load cart (into memory)
    if(user->is_registered()) {
        user->get_cart()->load_cart(user->get_id());
    }        
    return user;          
}
////////////////////
void neroshop::Seller::on_order_received(std::string& subaddress) {
    if(!wallet.get()) throw std::runtime_error("wallet has not been initialized");
    if(!wallet->get_monero_wallet()) throw std::runtime_error("monero_wallet_full is not opened");
    // if wallet is not properly synced with the daemon, you can only generate used addresses
    // unless wallet is synced to a daemon, you will not be able to generate any unique addresses
    if(!wallet->get_monero_wallet()->is_synced()) throw std::runtime_error("wallet is not synced with a daemon"); // Indicates if the wallet is synced with the daemon.
    // generate 10 new subaddress after each order (just to be sure there are enough unused subaddresses to choose from)
    for(int i = 0; i < 10; i++) wallet->address_new();
    // get a list of all unused subaddresses
    std::vector<std::string> unused_subaddress_list = wallet->address_unused();
    // now pick from the list of unused subaddresses (random)
	std::random_device rd; // Generating random numbers with C++11's random requires an engine and a distribution.
    std::mt19937 mt(rd()); // This is an engine based on the Mersenne Twister 19937 (64 bits):
    std::uniform_real_distribution<double> dist(0, unused_subaddress_list.size() - 1);
    subaddress = unused_subaddress_list[static_cast<int>(dist(mt))];
    // copy random subaddress
    // USED SUBADDRESS IS NOT REMOVED FROM Wallet::address_unused() UNTIL THE SECOND CONFIRMATION (OUTPUT RECEIVED ...) 
#ifdef NEROSHOP_DEBUG0
    std::cout << std::endl << "subaddress (random): " << subaddress << "\n";
#endif
    // also, generate a qrcode too
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
