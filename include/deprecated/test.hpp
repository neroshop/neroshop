

namespace neroshop {
    typedef enum payment_status = {
        STATUS_PAYMENT_NOT_RECEIVED, // red
        STATUS_PAYMENT_CONFIRMED, // yellow
        STATUS_PAYMENT_RECEIVED, // green
    } payment_status;
    typedef enum item_category {
        ITEM_CATEGORY_APPAREL,
        //ITEM_CATEGORY_,
    } item_category;    
    extern std::vector<std::string> item_categories;/* = {
        "apparel",
        "electronics",
    };*/
    extern std::vector<std::string> carriers;/* = {"USPS", "FedEX", "UPS", "DHL", //"",};*/
}
// https://stackoverflow.com/questions/7837190/c-c-global-vs-static-global#comment9555515_7837254
// https://stackoverflow.com/a/8317056
