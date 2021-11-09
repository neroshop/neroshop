#include "../include/item.hpp"

////////////////////
Item::Item() : id(0)//, quantity(0), price(0.00), weight(1.0), size(std::make_tuple<double, double, double>(0.0, 0.0, 0.0)), discount(0.0), category("unspecified")/* or none */, condition("new") {}// name and desc and empty strings by default
{}
////////////////////
Item::Item(unsigned int id) {//: Item() { // for registered items that already have an id
    set_id(id);
}
////////////////////
Item::Item(const std::string& name, const std::string& desc, double price, double weight, double length, double width, double height, const std::string& condition, const std::string& product_code) : Item() // quantity is set by cart
{
    register_item(name, desc, price, weight, length, width, height, condition, product_code);
}
////////////////////
Item::Item(const std::string& name, const std::string& desc, double price, double weight, const std::tuple<double, double, double>& size, const std::string& condition, const std::string& product_code) : Item() // quantity is set by cart
{
    register_item(name, desc, price, weight, std::get<0>(size), std::get<1>(size), std::get<2>(size), condition, product_code);  
}        
////////////////////
////////////////////
////////////////////
Item::~Item() {}
////////////////////
void Item::register_item(const std::string& name, const std::string& description, double price, double weight, double length, double width, double height, const std::string& condition, const std::string& product_code) {
    DB db("neroshop.db");
    //db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked) // https://www.sqlite.org/pragma.html#pragma_journal_mode
    ///////////
    // if item is already registered, then exit function
    if(db.table_exists("item")) {
        int registered_item = db.get_column_integer("item", "id", "product_code = " + DB::to_sql_string(product_code));
        if(registered_item != 0) {
            neroshop::print("An item with the same product code has already been registered (id will be set regardless)", 2);
            set_id(registered_item);
            return; // exit function
        }
    }
    // table item
    if(!db.table_exists("item")) {
	    db.table("item"); // item_id is primary key which will be auto generated
	    db.column("item", "ADD", "name", "TEXT");
	    db.column("item", "ADD", "description", "TEXT"); //db.column("item", "ADD", "quantity", "INTEGER"); // item quantity is managed by cart
	    db.column("item", "ADD", "price", "REAL");
        db.column("item", "ADD", "weight", "REAL");
	    db.column("item", "ADD", "size", "TEXT");//"REAL");//db.column("item", "ADD", "discount", "REAL"); // seller determines the discount//db.column("item", "ADD", "condition", "TEXT"); // seller is able to change the item's condition
	    db.column("item", "ADD", "product_code", "TEXT");
	    db.column("item", "ADD", "category_id", "INTEGER");
	    db.index("idx_product_codes", "item", "product_code"); // item product codes must be unique
	}
	std::string item_size = std::to_string(length) + "x" + std::to_string(width) + "x" + std::to_string(height);//std::to_string(std::get<0>(item.size)) +"x"+ std::to_string(std::get<1>(item.size)) +"x"+ std::to_string(std::get<2>(item.size));
	db.insert("item", "name, description, price, weight, size, product_code", 
	    DB::to_sql_string(name) + ", " + DB::to_sql_string(description) + ", " + std::to_string(price) + ", " + std::to_string(weight) + ", " + DB::to_sql_string(item_size) + ", " + DB::to_sql_string(product_code) //+ ", " + // + ", " + // + ", " + 
	);
	// save the item id
	unsigned int item_id = db.get_column_integer("item ORDER BY id DESC LIMIT 1", "*");
	set_id(item_id);
    NEROSHOP_TAG std::cout << "\033[1;36m" << name << " (id: " << get_id() << ") has been registered" << "\033[0m" << std::endl;
    ///////////
    db.close();        
}
////////////////////
void Item::register_item(const Item& item) { 
    // if item is already registered, then exit function
    /*if(item.is_registered()) {std::cout << "\033[0;93m" << "Item " << item.name << " has already been registered" << "\033[0m" << std::endl;return;}
    DB db("neroshop.db");
    //db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked) // https://www.sqlite.org/pragma.html#pragma_journal_mode
    ///////////
    int reg_item = db.get_column_integer("item", "id", "product_code = " + DB::to_sql_string(item.product_code));
    if(reg_item != 0) {
        neroshop::print("Item with the same product code is registered", 1);
        const_cast<Item&>(item).set_id(reg_item);
        NEROSHOP_TAG std::cout << "item id set to: " << reg_item << std::endl;
        return; // exit function
    }*/
    ///////////
    /*if(!db.table_exists("categories")) {
	    db.table("categories");
	    db.column("categories", "ADD", "category_name", "TEXT");
	    // subcategories
	    if(!db.table_exists("subcategories")) {
	        db.table("subcategories");
	        db.column("subcategories", "ADD", "subcategory_name", "TEXT");
	        int category_id = 0;
	        // these categories and subcategories are only created once
	        db.insert("categories", "category_name", DB::to_sql_string("Appliances")); // Furniture and Appliances
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "category_id, subcategory_name", std::to_string(category_id) + ", " + DB::to_sql_string(""));

	        db.insert("categories", "category_name", DB::to_sql_string("Arts, Crafts, & Sewing")); // paints, ceramics, etc
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "category_id, subcategory_name", std::to_string(category_id) + ", " + DB::to_sql_string(""));	        	        	        	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Automotive Parts & Accessories")); // Automotive & Powersports, Bikes
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "category_id, subcategory_name", std::to_string(category_id) + ", " + DB::to_sql_string(""));

	        db.insert("categories", "category_name", DB::to_sql_string("Baby")); // excluding apparel
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "category_id, subcategory_name", std::to_string(category_id) + ", " + DB::to_sql_string(""));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Beauty")); // Beauty & Personal Care
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "category_id, subcategory_name", std::to_string(category_id) + ", " + DB::to_sql_string(""));

	        db.insert("categories", "category_name", DB::to_sql_string("Books"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Book Series") + ", " + std::to_string(category_id));	        
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Children’s Books") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Comics") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Cookbooks") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Journals") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Magazines") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Newspapers") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Poetry") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Textbooks") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Novels") + ", " + std::to_string(category_id));	        
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("CDs & Vinyl"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Camera & Photo"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Cell Phones & Accessories"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	
	               	        	        	        
	        db.insert("categories", "category_name", DB::to_sql_string("Clothing, Shoes and Jewelry")); // Apparel // https://sewguide.com/types-of-clothes/
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Aprons") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Bras") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Coats, Jackets, and Cardigans") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Dresses") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Footwear") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Shorts, Pants and Jeans") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Skirts") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Socks") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Sweaters, Jackets") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("T-shirts, Tees, Tops") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Traditional clothing") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Underwear") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Collectible Coins")); // Collectibles & Fine Art, Art - Fine, Art - Home Decor// Currency, Coins, Cash Equivalents, and Gift Cards
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id)); 
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Electronics")); // Consumer Electronics
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Consumer Electronics") + ", " + std::to_string(category_id));	        
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Computers·Laptops·PC") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	 
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Smart watches") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	 
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("TVs") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	 
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	 	        	        
	 
	        db.insert("categories", "category_name", DB::to_sql_string("Entertainment Collectibles"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Food & Beverage"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Cereal + Grains") + ", " + std::to_string(category_id)); // Bread, (Whole Grains), Rice
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Bakery") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Dairy") + ", " + std::to_string(category_id)); // Milk, Yougurt, Cheese 
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Fruits") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Herbs·Spices") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Honey") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Meat·Fish·Poutry·Eggs") + ", " + std::to_string(category_id));	// ·Protein	        
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Nuts·Seeds-Legumes") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Fats·Oils·Sugar·Salt") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Poutry·Eggs") + ", " + std::to_string(category_id)); // ·Protein	        
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Snacks") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Sweets") + ", " + std::to_string(category_id));
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("Vegetables") + ", " + std::to_string(category_id));		                
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Fine Art"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Grocery & Gourmet Foods"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Handmade"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Health & Personal Care")); // Health, Household & Baby Care
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        	        	        	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Home & Kitchen")); // furniture, coffee makers, etc.
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Home & Garden")); // Garden & Outdoor
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Independent Design"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	 
	        db.insert("categories", "category_name", DB::to_sql_string("Industrial & Scientific"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Luggage & Travel Gear"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Major Appliances"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Movies & TV"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Music and DVD"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Musical Instruments"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        	        	        	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Office Products"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Outdoors"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	 
	        db.insert("categories", "category_name", DB::to_sql_string("Personal Computers"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Pet Supplies"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Software")); // Apps & Games
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Sports")); // Sports & Outdoors
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        	        	        	        
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Sports Collectibles"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Tools & Home Improvement"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	 
	        db.insert("categories", "category_name", DB::to_sql_string("Toys & Games")); // Toys, Hobbies, DIY
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Video, DVD & Blu-ray"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));

	        db.insert("categories", "category_name", DB::to_sql_string("Video Games"));
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	        
	        db.insert("categories", "category_name", DB::to_sql_string("Watches")); // Jewelry and Watches
	        category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));	        	        
    	        
	        // next category + subcategories ...
	        //db.insert("categories", "category_name", DB::to_sql_string(""));
	        //category_id = db.get_column_integer("categories ORDER BY id DESC LIMIT 1", "*"); // last inserted category
	        //db.insert("subcategories", "subcategory_name, category_id", DB::to_sql_string("") + ", " + std::to_string(category_id));
	    }
	}*/
    ///////////
    /*if(!db.table_exists("item")) {
	    db.table("item"); // item_id is primary key which will be auto generated
	    db.column("item", "ADD", "name", "TEXT");
	    db.column("item", "ADD", "description", "TEXT"); //db.column("item", "ADD", "quantity", "INTEGER"); // item quantity is managed by cart // seller determines how much is in stock
	    db.column("item", "ADD", "price", "REAL");
        db.column("item", "ADD", "weight", "REAL");
	    db.column("item", "ADD", "size", "TEXT");//"REAL");
	    db.column("item", "ADD", "discount", "REAL");
	    db.column("item", "ADD", "condition", "TEXT"); // seller determines the item's condition
	    db.column("item", "ADD", "product_code", "TEXT");
	    db.column("item", "ADD", "category_id", "INTEGER");
	    db.index("idx_product_codes", "item", "product_code"); // item product codes must be unique
	}
	std::string item_size = std::to_string(std::get<0>(item.size)) +"x"+ std::to_string(std::get<1>(item.size)) +"x"+ std::to_string(std::get<2>(item.size));
	db.insert("item", "name, description, price, weight, size, discount, condition, product_code", 
	    DB::to_sql_string(item.name)         + ", " + 
	    DB::to_sql_string(item.description)  + ", " + //std::to_string(item.get_quantity()) + ", " + 
	    std::to_string(item.price)           + ", " + // original / list price
	    std::to_string(item.weight)          + ", " + 
	    DB::to_sql_string(item_size)         + ", " + 
	    std::to_string(item.discount)        + ", " + 
	    DB::to_sql_string(item.condition)    + ", " + 
	    DB::to_sql_string(item.product_code) //+ ", " + 
	    // + ", " + 
	    // + ", " + 
	);
	// save the item id
	unsigned int item_id = db.get_column_integer("item ORDER BY id DESC LIMIT 1", "*");
	const_cast<Item&>(item).set_id(item_id);
    ///////////
    //const_cast<Item&>(item).show_info();
    NEROSHOP_TAG std::cout << "\033[1;36m" << item.name << " (id: " << item.id << ") has been registered" << "\033[0m" << std::endl;
    ///////////
    db.close();*/
}
////////////////////
////////////////////
////////////////////
////////////////////
void Item::set_id(unsigned int id) {
    this->id = id;
}
////////////////////
void Item::set_quantity(unsigned int quantity) {
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db(cart_file);
    if(!db.table_exists("Cart")) {db.close(); return;}
    db.update("Cart", "item_qty", std::to_string(quantity), "item_id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_quantity(unsigned int item_id, unsigned int quantity) {
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db(cart_file);
    if(!db.table_exists("Cart")) {db.close(); return;}
    db.update("Cart", "item_qty", std::to_string(quantity), "item_id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_name(const std::string& name) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "name", DB::to_sql_string(name), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_name(unsigned int item_id, const std::string& name) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "name", DB::to_sql_string(name), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_description(const std::string& description) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "description", DB::to_sql_string(description), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_description(unsigned int item_id, const std::string& description) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "description", DB::to_sql_string(description), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_price(double price) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "price", std::to_string(price), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_price(unsigned int item_id, double price) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "price", std::to_string(price), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
////////////////////
void Item::set_weight(double weight) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "weight", std::to_string(weight), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_weight(unsigned int item_id, double weight) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "weight", std::to_string(weight), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_size(double l, double w, double h) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    std::string size = std::to_string(l) + "x" + std::to_string(w) + "x" + std::to_string(h);
    //std::cout << "item_size set: " << size << std::endl;
    db.update("item", "size", DB::to_sql_string(size) , "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_size(unsigned int item_id, double l, double w, double h) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    std::string size = std::to_string(l) + "x" + std::to_string(w) + "x" + std::to_string(h);
    //std::cout << "item_size set: " << size << std::endl;
    db.update("item", "size", DB::to_sql_string(size) , "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
////////////////////
void Item::set_size(const std::tuple<double, double, double>& size) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    std::string item_size = std::to_string(std::get<0>(size)) + "x" + std::to_string(std::get<1>(size)) + "x" + std::to_string(std::get<2>(size));
    //std::cout << "item_size set: " << item_size << std::endl;
    db.update("item", "size", DB::to_sql_string(item_size) , "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_size(unsigned int item_id, const std::tuple<double, double, double>& size) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    std::string item_size = std::to_string(std::get<0>(size)) + "x" + std::to_string(std::get<1>(size)) + "x" + std::to_string(std::get<2>(size));
    //std::cout << "item_size set: " << item_size << std::endl;
    db.update("item", "size", DB::to_sql_string(item_size) , "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_discount(double discount) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "discount", std::to_string(discount), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_discount(unsigned int item_id, double discount) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "discount", std::to_string(discount), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_discount_by_percentage(double percent) { // converts a percentage to a price then sets the price as the discount
    // convert discount (a percentage) into a decimal
    double num2dec = percent / 100; //std::cout << percent << "% is " << num2dec << std::endl;
    double original_price = get_price();
    std::cout << "original_price: $" << original_price << std::endl;
    double discount = original_price * num2dec; 
    std::cout << "discount: $" << discount/* << " = " << percent << "% of " << original_price*/ << std::endl;
    //double sale_price = original_price - discount;
    //std::cout << "*new* sales_price: $" << sale_price << std::endl;
    /////////////////////
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "discount", std::to_string(discount), "id = " + std::to_string(this->id));
    db.close();    
}
////////////////////
void Item::set_discount_by_percentage(unsigned int item_id, double percent) {
    // convert discount (a percentage) into a decimal
    double num2dec = percent / 100; //std::cout << percent << "% is " << num2dec << std::endl;
    double original_price = get_price(item_id);
    std::cout << "original_price: $" << original_price << std::endl;
    double discount = original_price * num2dec; 
    std::cout << "discount: $" << discount/* << " = " << percent << "% of " << original_price*/ << std::endl;
    //double sale_price = original_price - discount;
    //std::cout << "*new* sales_price: $" << sale_price << std::endl;
    /////////////////////
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "discount", std::to_string(discount), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_condition(const std::string& condition) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "condition", DB::to_sql_string(condition), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_condition(unsigned int item_id, const std::string& condition) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "condition", DB::to_sql_string(condition), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
void Item::set_product_code(const std::string& product_code) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "product_code", DB::to_sql_string(product_code), "id = " + std::to_string(this->id));
    db.close();
}
////////////////////
void Item::set_product_code(unsigned int item_id, const std::string& product_code) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return;}
    db.update("item", "product_code", DB::to_sql_string(product_code), "id = " + std::to_string(item_id));
    db.close();
}
////////////////////
////////////////////
// getters
////////////////////
////////////////////
unsigned int Item::get_id() const {
    return id; // should be 0 by default for unregistered items
}
////////////////////
std::string Item::get_name() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_name = db.get_column_text("item", "name", "id = " + std::to_string(this->id));
    db.close();
    return item_name;
}
////////////////////
std::string Item::get_name(unsigned int item_id) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_name = db.get_column_text("item", "name", "id = " + std::to_string(item_id));
    db.close();
    return item_name;
}
////////////////////
std::string Item::get_description() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_description = db.get_column_text("item", "description", "id = " + std::to_string(this->id));
    db.close();
    return item_description;
}
////////////////////
std::string Item::get_description(unsigned int item_id) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_description = db.get_column_text("item", "description", "id = " + std::to_string(item_id));
    db.close();
    return item_description;
}
////////////////////
double Item::get_price() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return 0.0;}
    double item_price = db.get_column_real("item", "price", "id = " + std::to_string(this->id));
    db.close();
    return item_price;
}
////////////////////
double Item::get_price(unsigned int item_id) { // original price (list price)
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return 0.0;}
    double item_price = db.get_column_real("item", "price", "id = " + std::to_string(item_id));
    db.close();
    return item_price;
}
////////////////////
unsigned int Item::get_quantity() const {
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db(cart_file);
    if(!db.table_exists("Cart")) {db.close(); return 0;}
    unsigned int item_qty = db.get_column_integer("Cart", "item_qty", "item_id = " + std::to_string(this->id));//std::cout << "item_id: " << item_id << " has a quantity of " << item_qty << std::endl;
    db.close();
    return item_qty;
}
////////////////////
unsigned int Item::get_quantity(unsigned int item_id) {
    std::string cart_file = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cart.db";
    DB db(cart_file);
    if(!db.table_exists("Cart")) {db.close(); return 0;}
    unsigned int item_qty = db.get_column_integer("Cart", "item_qty", "item_id = " + std::to_string(item_id));//std::cout << "item_id: " << item_id << " has a quantity of " << item_qty << std::endl;
    db.close();
    return item_qty;
}
////////////////////
////////////////////
////////////////////
double Item::get_weight() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return 0.0;}
    double item_weight = db.get_column_real("item", "weight", "id = " + std::to_string(this->id));
    db.close();
    return item_weight;
}
////////////////////
double Item::get_weight(unsigned int item_id) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return 0.0;}
    double item_weight = db.get_column_real("item", "weight", "id = " + std::to_string(item_id));
    db.close();
    return item_weight;
}
////////////////////
std::tuple<double, double, double> Item::get_size() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return std::make_tuple(0, 0, 0);}
    std::vector<std::string> item_size = String::split(db.get_column_text("item", "size", "id = " + std::to_string(this->id)), "x");
    db.close();
    return std::make_tuple(std::stod(item_size[0]), std::stod(item_size[1]), std::stod(item_size[2]));
}
////////////////////
std::tuple<double, double, double> Item::get_size(unsigned int item_id) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return std::make_tuple(0, 0, 0);}
    std::vector<std::string> item_size = String::split(db.get_column_text("item", "size", "id = " + std::to_string(item_id)), "x");
    db.close();
    return std::make_tuple(std::stod(item_size[0]), std::stod(item_size[1]), std::stod(item_size[2]));
}
////////////////////
double Item::get_discount(unsigned int seller_id) const {
    return get_seller_discount(seller_id); // item discount is determined by seller
}
////////////////////
double Item::get_discount(unsigned int item_id, unsigned int seller_id) {
    return get_seller_discount(item_id, seller_id); // item discount is determined by seller
}
////////////////////
std::string Item::get_condition() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_condition = db.get_column_text("item", "condition", "id = " + std::to_string(this->id));
    db.close();
    return item_condition;
}
////////////////////
std::string Item::get_condition(unsigned int item_id) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_condition = db.get_column_text("item", "condition", "id = " + std::to_string(item_id));
    db.close();
    return item_condition;
}
////////////////////
//std::string get_subcategory() const {/*return subcategory;*/}
////////////////////
std::string Item::get_product_code() const {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_product_code = db.get_column_text("item", "product_code", "id = " + std::to_string(this->id));
    db.close();
    return item_product_code;
}
////////////////////
std::string Item::get_product_code(unsigned int item_id) {
    DB db("neroshop.db");
    if(!db.table_exists("item")) {db.close(); return "";}
    std::string item_product_code = db.get_column_text("item", "product_code", "id = " + std::to_string(item_id));
    db.close();
    return item_product_code;
}
////////////////////
////////////////////
////////////////////
////////////////////
// seller
////////////////////
////////////////////
double Item::get_seller_price(unsigned int seller_id) const {
    DB db("neroshop.db");
    if(!db.table_exists("inventory")) {db.close(); return 0.00;} // make sure table exists first
    double seller_price = 0.00;
    if(seller_id > 0) seller_price = db.get_column_real("inventory", "seller_price", "item_id = " + std::to_string(this->id) + " AND seller_id = " + std::to_string(seller_id)); // get a specific seller's price for this item
    if(seller_id <= 0) seller_price = db.get_column_real("inventory", "seller_price", "item_id = " + std::to_string(this->id)); // get a random seller's price for this item
    db.close();
    return seller_price;
}
////////////////////
double Item::get_seller_price(unsigned int item_id, unsigned int seller_id) {// seller price (sales price)
    DB db("neroshop.db");
    if(!db.table_exists("inventory")) {db.close(); return 0.00;} // make sure table exists first
    double seller_price = 0.00;
    if(seller_id > 0) seller_price = db.get_column_real("inventory", "seller_price", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
    if(seller_id <= 0) seller_price = db.get_column_real("inventory", "seller_price", "item_id = " + std::to_string(item_id));
    db.close();
    return seller_price;
}
////////////////////
double Item::get_seller_discount(unsigned int seller_id) const {
    DB db("neroshop.db");
    if(!db.table_exists("inventory")) {db.close(); return 0.00;} // make sure table exists first
    double seller_discount = 0.00;
    if(seller_id > 0) seller_discount = db.get_column_real("inventory", "seller_discount", "item_id = " + std::to_string(this->id) + " AND seller_id = " + std::to_string(seller_id));
    if(seller_id <= 0) seller_discount = db.get_column_real("inventory", "seller_discount", "item_id = " + std::to_string(this->id));
    db.close();
    return seller_discount;
}
////////////////////
double Item::get_seller_discount(unsigned int item_id, unsigned int seller_id) {
    DB db("neroshop.db");
    if(!db.table_exists("inventory")) {db.close(); return 0.00;} // make sure table exists first
    double seller_discount = 0.00;
    if(seller_id > 0) seller_discount = db.get_column_real("inventory", "seller_discount", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
    if(seller_id <= 0) seller_discount = db.get_column_real("inventory", "seller_discount", "item_id = " + std::to_string(item_id));
    db.close();
    return seller_discount;
}
////////////////////
std::string Item::get_seller_condition(unsigned int seller_id) const {
    DB db("neroshop.db");
    if(!db.table_exists("inventory")) {db.close(); return "";} // make sure table exists first
    std::string seller_condition = db.get_column_text("inventory", "seller_condition", "item_id = " + std::to_string(this->id) + " AND seller_id = " + std::to_string(seller_id));
    db.close();
    return seller_condition;
}
////////////////////
std::string Item::get_seller_condition(unsigned int item_id, unsigned int seller_id) {
    DB db("neroshop.db");
    if(!db.table_exists("inventory")) {db.close(); return "";} // make sure table exists first
    std::string seller_condition = db.get_column_text("inventory", "seller_condition", "item_id = " + std::to_string(item_id) + " AND seller_id = " + std::to_string(seller_id));
    db.close();
    return seller_condition;
}
////////////////////
unsigned int Item::get_stock_quantity() const {
    DB db;if(!db.open("neroshop.db")) {std::cout << "Could not open sql database" << std::endl; return 0;}
    if(!db.table_exists("inventory")) {db.close(); return 0;} // make sure table exists first
    int stock_qty = db.get_column_integer("inventory", "stock_qty", "item_id=" + std::to_string(this->id)); // check stock_quantity
    db.close();
    return stock_qty;
}
//////////////////// 
unsigned int Item::get_stock_quantity(unsigned int item_id) {
    DB db;if(!db.open("neroshop.db")) {std::cout << "Could not open sql database" << std::endl; return 0;}
    if(!db.table_exists("inventory")) {db.close(); return 0;} // make sure table exists first
    int stock_qty = db.get_column_integer("inventory", "stock_qty", "item_id=" + std::to_string(item_id)); // check stock_quantity
    db.close();
    return stock_qty;
}
////////////////////
////////////////////
bool Item::is_registered() const {
    DB db;if(!db.open("neroshop.db")) {std::cout << "Could not open sql database" << std::endl; return false;}
    if(!db.table_exists("item")) {db.close(); return false;} // make sure table exists first
    int id = db.get_column_integer("item", "id", "id=" + std::to_string(this->id));
    db.close();
    return (id > 0);
}
////////////////////
bool Item::is_registered(unsigned int item_id) {
    DB db;if(!db.open("neroshop.db")) {std::cout << "Could not open sql database" << std::endl; return false;}
    if(!db.table_exists("item")) {db.close(); return false;} // make sure table exists first
    int id = db.get_column_integer("item", "id", "id=" + std::to_string(item_id));
    db.close();
    return (id > 0);
}
////////////////////
////////////////////
bool Item::in_stock() const {
    DB db;if(!db.open("neroshop.db")) {std::cout << "Could not open sql database" << std::endl; return false;}
    if(!db.table_exists("inventory")) {db.close(); return false;} // make sure table exists first
    int stock_qty = db.get_column_integer("inventory", "stock_qty", "item_id=" + std::to_string(this->id)); // check stock_quantity
    if(stock_qty < 1) return false;
    db.close();
    return true;
}
////////////////////
bool Item::in_stock(unsigned int item_id) {
    DB db;if(!db.open("neroshop.db")) {std::cout << "Could not open sql database" << std::endl; return false;}
    if(!db.table_exists("inventory")) {db.close(); return false;} // make sure table exists first
    int stock_qty = db.get_column_integer("inventory", "stock_qty", "item_id=" + std::to_string(item_id)); // check stock_quantity
    if(stock_qty < 1) return false;
    db.close();
    return true;
}
////////////////////
////////////////////
bool Item::in_cart(/*unsigned int cart_id*/) const {
    std::string user = System::get_user();
    std::string cart_file = std::string("/home/" + user + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {std::cout << "Could not open sql database" << std::endl; return false;}
    if(!db.table_exists("Cart")) {db.close(); return false;} // make sure table exists first
    int id = db.get_column_integer("Cart", "item_id", "item_id = " + std::to_string(this->id));
    //    "cart_id = " + std::to_string(cart_id) + " AND item_id = " + std::to_string(this->id));
    if(id <= 0) return false;
    if(id == this->id) return true; // if this->id matches the item_id in table Cart, return true
    db.close();
    return false;
}
////////////////////
bool Item::in_cart(unsigned int item_id/*, unsigned int cart_id*/) {
    std::string user = System::get_user();
    std::string cart_file = std::string("/home/" + user + "/.config/neroshop/") + "cart.db";
    DB db; if(!db.open(cart_file)) {std::cout << "Could not open sql database" << std::endl; return false;}
    if(!db.table_exists("Cart")) {db.close(); return false;} // make sure table exists first
    int id = db.get_column_integer("Cart", "item_id", "item_id = " + std::to_string(item_id));
    //    "cart_id = " + std::to_string(cart_id) + " AND item_id = " + std::to_string(item_id));
    if(id <= 0) return false;
    if(id == item_id) return true; // if item_id matches the item_id in table Cart, return true
    db.close();
    return false;
}
////////////////////
////////////////////
////////////////////
////////////////////
void Item::show_info() {
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
