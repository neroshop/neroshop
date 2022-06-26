#include "order.hpp"

////////////////////
neroshop::Order::Order() : id(0) {}
////////////////////
neroshop::Order::Order(unsigned int id) {
    this->id = id; // once an order has a valid id, then it means it is already in the database
}
////////////////////
neroshop::Order::~Order() {
#ifdef NEROSHOP_DEBUG0
    std::cout << "order deleted\n";
#endif    
}
////////////////////
void neroshop::Order::create_order(const neroshop::Cart& cart, const std::string& shipping_address, std::string contact_info) {
    if(cart.get_id() < 1) { // invalid id or zero
        neroshop::print("creating a guest user order ...", 3);
        create_guest_order(cart, shipping_address, contact_info);
        return; // exit function
    }
    neroshop::print("creating a registered user order ...", 4);
    create_registered_user_order(cart, shipping_address, contact_info);
}
////////////////////
void neroshop::Order::create_guest_order(const neroshop::Cart& cart, const std::string& shipping_address, std::string contact_info) // order: order_id, [order_date], product, SKU, quantity, price (subtotal), discount (optional), shipping_cost/estimated_delivery, payment method:monero[xmr], total
{

}
////////////////////
////////////////////
////////////////////
////////////////////
void neroshop::Order::create_registered_user_order(const neroshop::Cart& cart, const std::string& shipping_address, std::string contact_info) {

}
////////////////////
void neroshop::Order::cancel_order()
{
    // cannot cancel order if it has been at least 12 hours or more
    // sellers can request that a buyer cancels an order
    // only a buyer can cancel an order
    set_status( order_status::cancelled );
}
////////////////////
void neroshop::Order::change_order()
{}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void neroshop::Order::set_status(order_status status) { this->status = status;}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
unsigned int neroshop::Order::get_id() const {
    return id;
}
////////////////////
order_status neroshop::Order::get_status() const {return status;}
////////////////////
std::string neroshop::Order::get_status_string() const {
    switch(status) {
        case order_status::incomplete: return "Incomplete";break; // order was interrupted while user was in the process of creating an order
        case order_status::pending   : return "Pending"  ; break;
        case order_status::preparing : return "Preparing"; break;
        case order_status::shipped   : return "Shipped"  ; break;
        case order_status::ready     : return "Ready"    ; break;
        case order_status::done      : return "Delivered"; break;
        case order_status::cancelled : return "Cancelled"; break;
        case order_status::failed    : return "Failed"   ; break;
        case order_status::returned  : return "Returned" ; break;
        //case order_status:: : break;
        default: return "";
    }
}
// if(order->get_status_string() == "pending") {}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::Order::is_cancelled() const {return (status == order_status::cancelled);}
////////////////////
////////////////////
bool neroshop::Order::in_db(unsigned int order_number) // static - can be called without an obj
{

    return false;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
