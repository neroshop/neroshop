#include "../include/order.hpp"

////////////////////
Order::Order() : id(0)
{}
////////////////////
Order::Order(unsigned int id) {
    set_id(id); // once an order has a valid id, then it means it is already in the database
}
////////////////////
Order::~Order() 
{}
////////////////////
////////////////////
void Order::create_order(unsigned int user_id, const std::string& shipping_address, std::string contact_info) // order: order_id, [order_date], product, SKU, quantity, price (subtotal), discount (optional), shipping_cost/estimated_delivery, payment method:monero[xmr], total
{
    // check if order is not already in the database
    if(this->id > 0) { neroshop::print("This order (id: " + std::to_string(this->id) + ") already exists"); return; }
    Cart cart = *Cart::get_singleton();
    if(cart.is_empty()) {neroshop::print("You cannot place an order: (Cart is empty)", 1);return;}// if cart is empty, exit function
    // seller_id cannot buy from him or her self
    // if(user_id == seller_id) { neroshop::print("You cannot buy from yourself") return; }
    // seller get all details of an order from buyer
    // get timestamp
	auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S"); // l% = 12h (01-12), %H = 24h (00-23), %p = AM or PM, %F = %Y-%m-%d (YYYY-MM-DD)
	std::string date = ss.str(); // order_date
    DB db("neroshop.db");
    //db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked) // https://www.sqlite.org/pragma.html#pragma_journal_mode
    if(!db.table_exists("orders")) { // ORDER is a keyword in sqlite :O
        db.table("orders"); // a unique id will automatically be made for each order, so long as the same db is being used
        // order-specific
        db.column("orders", "ADD", "date", "TEXT"); // date and time order was created
        db.column("orders", "ADD", "status", "TEXT"); // order status // for each individual item // you don't need all these once you have the item_id (qty, price, weight, sku_code, etc.)
        db.column("orders", "ADD", "user_id", "INTEGER"); // customer that is placing the order
        db.column("orders", "ADD", "weight", "REAL"); // total weight of all items
        db.column("orders", "ADD", "subtotal", "REAL"); // price of all items in cart (combined)
        db.column("orders", "ADD", "discount", "REAL"); // discount from coupon (optional)(price will be reduced by discount)
        //db.column("orders", "ADD", "shipping_address", "TEXT"); // will not be saved in db (for the sake of user's privacy), but will be sent to the seller // shipping_address - name, address, city, region, postal code, country
        //db.column("orders", "ADD", "shipping_method", "TEXT"); // 1 day, 2 day, 3 day, 1 week, etc.
        db.column("orders", "ADD", "shipping_cost", "REAL");
        db.column("orders", "ADD", "total", "REAL"); // total cost of all items plus shipping costs (taxes don't exist in the cryptocurrency world except in the form of tx fees)
        db.column("orders", "ADD", "payment_method", "TEXT"); //  cash, card, crypto, etc.
        db.column("orders", "ADD", "currency", "TEXT"); //  xmr, btc, usd, etc.
        db.column("orders", "ADD", "notes", "TEXT"); // must be encrypted since storing this in db
        //db.column("orders", "ADD", "", ""); // monero_address (of receipient)?
        //db.index("idx_orders_code", "orders", "code");// code can be sku, upc, ean, etc. //db.execute("CREATE UNIQUE INDEX idx_orders_sku ON Orders (sku);");
    }
    double weight = cart.get_total_weight();
    db.insert("orders", "date, status, user_id, weight, subtotal, discount, shipping_cost, total, payment_method, currency, notes",
        DB::to_sql_string(date)           + ", " + 
        DB::to_sql_string("Incomplete")      + ", " + // status
        std::to_string(user_id)           + ", " + 
        std::to_string(weight)            + ", " + 
        std::to_string(0.000000000000)    + ", " + // subtotal
        std::to_string(0.000000000000)    + ", " + // discount (overall)
        std::to_string(0.000000000000)    + ", " + // shipping_cost
        std::to_string(0.000000000000)    + ", " + // total
        DB::to_sql_string("crypto")       + ", " + 
        DB::to_sql_string("monero (xmr)") + ", " + 
        DB::to_sql_string(/*shipping_address*/"") //+ ", " + ", " + // notes
    );
    /////////////////////////////////////////
    if(!db.table_exists("order_item")) {
        db.table("order_item");
        db.column("order_item", "ADD", "order_id", "INTEGER");  // the order id in which the order_item belongs to
        db.column("order_item", "ADD", "item_id", "INTEGER");   // the order_item's item_id
        db.column("order_item", "ADD", "seller_id", "INTEGER"); // the seller that is selling the item_id                
        db.column("order_item", "ADD", "item_qty", "INTEGER");  // the quantity of the item the buyer wishes to buy
        db.column("order_item", "ADD", "item_price", "REAL");   // might not need the item price at all //db.column("order_item", "ADD", "", "");
    }
    // get the most recent order_id (last record from table orders)
    unsigned int order_id = db.get_column_integer("orders ORDER BY id DESC LIMIT 1", "*");//db.execute("SELECT * FROM orders ORDER BY id DESC LIMIT 1;");
    set_id(order_id); // save the order_id
    double subtotal = 0.00, discount = 0.00, shipping_cost = 0.00;
    std::string seller_currency;
    //if(!Converter::is_supported_currency(currency)) currency = "usd"; // default
    for(int i = 0; i < cart.get_contents_count(); i++) {
        Item * item = cart.get_item(i);
        std::string item_name = cart.get_item(i)->get_name();
        unsigned int item_id  = cart.get_item(i)->get_id();
        unsigned int item_qty = cart.get_item(i)->get_quantity();
        // if seller_id is not specified (0), then choose a random seller who is selling the same product
        int seller_id = db.get_column_integer("inventory", "seller_id", "item_id = " + std::to_string(item_id));
        if(seller_id <= 0) { std::cout << "item seller not found"; return; }     
        // if the buyer is also the seller XD
        if(user_id == seller_id) {neroshop::print("You cannot buy from yourself", 1); return;}
        // get the currency that item is priced in
        seller_currency = db.get_column_text("inventory", "currency", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
        // get seller_price
        // if seller does not specify a price for their item, set the item price to its original price
        double seller_price = db.get_column_real("inventory", "seller_price", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));//NEROSHOP_TAG std::cout << "Seller's price for item (id: " << item_id << ") per unit is: " << seller_price << " " << seller_currency << std::endl;
        double item_price = (seller_price > 0.00) ? seller_price : item->get_price(); // set the item price to the seller's price
        // calculate subtotal (price of all items combined)
        subtotal += item_qty * item_price;
        // get seller discount
        // if seller does not specify a discount for their item, set the item discount to its original discount which is 0
        double seller_discount = db.get_column_real("inventory", "seller_discount", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
        unsigned int discounted_items = db.get_column_integer("inventory", "discount_qty", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
        if(seller_discount > 0.00) {
            discount += (item_qty / discounted_items) * seller_discount;
            NEROSHOP_TAG std::cout << "\033[1;37m" << "for every " << discounted_items << " " << item_name << "s, you get " << Converter::get_currency_symbol(seller_currency) << std::fixed << std::setprecision(2) << seller_discount << " off (since you have x" << item_qty << ", total discount is: " << Converter::get_currency_symbol(seller_currency) << ((item_qty / discounted_items) * seller_discount) << ")\033[0m" << std::endl;
        }
        // get condition of item based on seller
        std::string item_condition = db.get_column_text("inventory", "condition", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
        // check again if item is still in stock
        if(!item->in_stock()) {
            neroshop::print("[error]: order failed  [reason]: The following item is out of stock: " + item_name);
            set_status(order_status::failed); // set order status to failed
            db.update("orders", "status", DB::to_sql_string(get_status_string()), "id = " + std::to_string(order_id));
            return; // exit function //continue; // skip this item
        }
        // add each item to the same order_id
        db.insert("order_item", "order_id, item_id, seller_id, item_qty, item_price",
            std::to_string(order_id) + ", " + std::to_string(item_id) + ", " + std::to_string(seller_id) + ", " + std::to_string(item_qty) + ", " + std::to_string(item_price));
        // reduce stock_qty of each purchased item (subtract stock_qty by item_qty that buyer is purchasing)
        int stock_qty = db.get_column_integer("inventory", "stock_qty", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));//std::cout << "stock_qty of item BEFORE deletion: " << stock_qty << std::endl; 
        db.update("inventory", "stock_qty", std::to_string(stock_qty - item_qty), "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));//std::cout << "stock_qty of item AFTER deletion: " << db.get_column_integer("inventory", "stock_qty", "item_id = " + std::to_string(item->get_id()) + " AND seller_id = " + std::to_string(seller_id)) << std::endl;
        // If stock qty goes to 0, delete this item from inventory (row) - bad idea because once an item is listed by a seller then the listing is permanent unless the seller's account is deleted or the manufacture of the item has discontinued
        //stock_qty = db.get_column_integer("inventory", "stock_qty", "item_id = " + std::to_string(item->get_id()) + " AND seller_id = " + std::to_string(seller_id));
        //if(stock_qty <= 0) db.drop("inventory", "stock_qty = " + std::to_string(0));
    }
    // print order message
    neroshop::print("Thank you for using neroshop.");
    neroshop::io_write("You have ordered: ");
    for(int i = 0; i < cart.get_contents_count(); i++) {
        Item * item = cart.get_item(i);
        std::cout << "\033[0;94m" + item->get_name() << " (x" << item->get_quantity() << ")\033[0m" << std::endl;
        item->set_quantity(0); // reset all item quantity to 0 (now that order has been completed)
    }
    // empty cart after completing order
    cart.empty();
    // if a user loses internet connection, libcurl cannot get the exchange rate so the order will fail
    // todo: find a way to check if user has internet connection
    set_status(order_status::failed); // set order status to failed by default
    db.update("orders", "status", DB::to_sql_string(get_status_string()), "id = " + std::to_string(order_id));
    // update order details - converts seller's currency of choice to xmr, the moment you create an order
    db.update("orders", "subtotal", std::to_string(Converter::to_xmr(subtotal, seller_currency)), "id = " + std::to_string(order_id));
    db.update("orders", "discount", std::to_string(Converter::to_xmr(discount, seller_currency)), "id = " + std::to_string(order_id));
    db.update("orders", "shipping_cost", std::to_string(Converter::to_xmr(shipping_cost, seller_currency)), "id = " + std::to_string(order_id));
    double total = (subtotal - discount) + shipping_cost;
    db.update("orders", "total", std::to_string(Converter::to_xmr(total, seller_currency)), "id = " + std::to_string(order_id));
    // display order details
    std::string currency = Script::get_string(DB::get_lua_state(), "neroshop.currency");
    if(currency.empty() || !Converter::is_supported_currency(currency)) currency = "usd"; // default //Converter::from_xmr(Converter::to_xmr(, seller_currency), currency);
    neroshop::print("Sit tight as we notify the seller(s) about your order.");
    std::cout << "Subtotal: " << std::fixed << std::setprecision(12) << Converter::to_xmr(subtotal, seller_currency) << " xmr" << std::fixed << std::setprecision(2) << " (" << Converter::get_currency_symbol(currency) << Converter::from_xmr(Converter::to_xmr(subtotal, seller_currency), currency) << " " << String::upper(currency) << ")" <<  std::endl; // (() ? : "")
    if(discount > 0) std::cout << "Discount: -" << std::fixed << std::setprecision(12) << Converter::to_xmr(discount, seller_currency) << " xmr" << std::fixed << std::setprecision(2) << " (" << Converter::get_currency_symbol(currency) << Converter::from_xmr(Converter::to_xmr(discount, seller_currency), currency) << " " << String::upper(currency) << ")" <<  std::endl;
    std::cout << "Shipping: " << std::fixed << std::setprecision(12) << Converter::to_xmr(shipping_cost, seller_currency) << " xmr" << std::fixed << std::setprecision(2) << " (" << Converter::get_currency_symbol(currency) << Converter::from_xmr(Converter::to_xmr(shipping_cost, seller_currency), currency) << " " << String::upper(currency) << ")" <<  std::endl;
    std::cout << "Order total: " << std::fixed << std::setprecision(12) << Converter::to_xmr(total, seller_currency) << " xmr" << std::fixed << std::setprecision(2) << " (" << Converter::get_currency_symbol(currency) << Converter::from_xmr(Converter::to_xmr(total, seller_currency), currency) << " " << String::upper(currency) << ")" <<  std::endl;
    //std::cout << "Estimated delivery date: " << delivery_date_est << std::endl;
    set_status(order_status::pending); // if everything went well then order will be set to pending
    db.update("orders", "status", DB::to_sql_string(get_status_string()), "id = " + std::to_string(order_id));
    /////////////////////////////////////////
    db.close(); // always close db when done
    // notify seller // send seller notification for every order made that contains the item they have on sale: "You have received an order on neroshop for 1 items(s) totaling: 0.2 XMR ($107.50 at current rates)". Also include buyer's name and address and contact
    // seller receives the order made by the buyer - shipping address will be encrypted then sent to the seller
    // seller generates a unique subaddress when they accept the order, the stock_qty of item_id in table Inventory (seller can choose to either accept or deny the order, but they must give a reason for denying an order)
}
////////////////////
void Order::cancel_order()
{
    // cannot cancel order if it has been at least 12 hours or more
    // sellers can request that a buyer cancels an order
    // only a buyer can cancel an order
    set_status( order_status::cancelled );
}
////////////////////
void Order::change_order()
{}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
void Order::set_id(unsigned int id) {
    this->id = id;
}
////////////////////
void Order::set_status(order_status status) { this->status = status;}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
unsigned int Order::get_id() const {
    return id;
}
////////////////////
order_status Order::get_status() const {return status;}
////////////////////
std::string Order::get_status_string() const {
    switch(status) {
        case order_status::incomplete: return "Incomplete";break; // order was interrupted while using was in the process of creating an order
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
bool Order::is_cancelled() const {return (status == order_status::cancelled);}
////////////////////
////////////////////
bool Order::in_db(unsigned int order_number) // static - can be called without an obj
{
    DB db;
    if(!db.open("neroshop.db")) {
		std::cout << Logger("Could not open sql database", 0) << std::endl;
        return false; // exit function
    }
    int order_id = db.get_column_integer("orders", "id", "id=" + std::to_string(order_number));
    if(order_id <= 0) {std::cout << "Order not found" << std::endl;return false;}
    if(order_id == order_number) {
        return true;
    }
    db.close();
    return false;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
