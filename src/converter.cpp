#include "../include/converter.hpp"

////////////////////
////////////////////
////////////////////
////////////////////
std::string neroshop::Converter::json_string ("");
////////////////////
////////////////////
double neroshop::Converter::to_kg(double amount, const std::string& unit_name) const {
    if(String::lower(unit_name) == "lb" || String::lower(unit_name) == "lbs" || String::lower(unit_name) == "pound") {return lb_to_kg(amount);}
    return 0.0;
}
////////////////////
double neroshop::Converter::lb_to_kg(double lb) {
    double lb2kg = 0.45359237; // 1 lb = 0.45359237 kg
    return lb * lb2kg;
}
////////////////////
////////////////////
////////////////////
////////////////////
std::string neroshop::Converter::get_currency_symbol(const std::string& currency_code) {
    if(String::lower(currency_code) == "usd") return "$";// or US$
    if(String::lower(currency_code) == "eur") return "€";
    if(String::lower(currency_code) == "jpy") return "¥";
    if(String::lower(currency_code) == "gbp") return "£";
    if(String::lower(currency_code) == "cad") return "C$"; // or "$", "C$", "CA$", "CAD$";
    if(String::lower(currency_code) == "chf") return "CHF";// or "francs";//francs comes after the number
    if(String::lower(currency_code) == "aud") return "A$"; // or "$", "A$", "AUD$";
    if(String::lower(currency_code) == "cny") return "CN¥";//or "元"//if(String::lower(currency_code) == "ghs") return "¢";// Ghanaian cedis//if(String::lower(currency_code) == "ngn") return "₦";// Nigerian Naira
    if(String::lower(currency_code) == "sek") return "SEK";// e.g  20 kr, 50 kr, 100 kr, etc.
    if(String::lower(currency_code) == "nzd") return "NZ$";// or $
    if(String::lower(currency_code) == "mxn") return "MX$";// or $
    //if(String::lower(currency_code) == "") return "";
    return "";
} // https://www.xe.com/symbols.php
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::Converter::is_supported_currency(const std::string& currency_code) {
    std::vector<std::string> supported_currency {"usd", "eur", "jpy", "gbp", "cad", "btc", "eth",
                                                "chf", "aud", "cny", "sek", "nzd", "mxn"};

    auto it = std::find(supported_currency.begin(), supported_currency.end(), currency_code);

    if (it != supported_currency.end()) {
        return true;
    } 
    else {
        return false;
    }
}
////////////////////
////////////////////
////////////////////
////////////////////
double neroshop::Converter::convert_xmr(double quantity, const std::string currency, bool to) { //to: if we want currency->xmr (true) or rather xmr->currency (false)
    /*
        Usage : 
        std::cout << "100€ = " << convert_xmr(100, "eur", true) << "XMR" << endl;
        std::cout << "10XMR = " << convert_xmr(10, "eur", false) << "€" << endl; 
    */
    if (is_supported_currency(currency) && quantity >= 0) {
        std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=" + currency;

        if (!request(url)) {
            return -1;
        }
        std::string response = get_json();

        auto json_response = nlohmann::json::parse(response);
        double price = json_response["monero"][currency];

        if (to) {
            return quantity/price;
        } else {
            return price*quantity;
        }
    }
    return -1;
}
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::Converter::request(const std::string& url)
{
    // parse raw json str
    //std::string buffer;
    CURL * curl = curl_easy_init();
    long http_code = 0;
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        //curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // opt - Don't bother trying IPv6, which would increase DNS resolution time.
        //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10); // opt - don't wait forever, time out after 10 secs
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // opt follow https redirects if necessary
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, neroshop::Converter::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &neroshop::Converter::json_string);//&buffer);
        res = curl_easy_perform(curl);
        //curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code); // opt
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res) << std::endl;
            return false;
        }
        curl_easy_cleanup(curl);
        //if (http_code == 200) std::cout << "\nGot successful response from " << url << std::endl; // opt
    } 
    else { std::cout << "Could not initialize curl" << std::endl; return false; }
    return true;
}
////////////////////
std::string neroshop::Converter::get_json() // const; // returns whole json as a string
{
    return json_string;
}
////////////////////
////////////////////
std::size_t neroshop::Converter::write_callback(char* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t total_bytes = size * num;
    if(total_bytes) {
        out->append(in, total_bytes);
        return total_bytes;
    }
    return 0;
}
////////////////////
////////////////////
////////////////////
////////////////////
