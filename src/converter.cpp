#include "../include/converter.hpp"

////////////////////
////////////////////
////////////////////
////////////////////
std::string Converter::json_string ("");
////////////////////
////////////////////
double Converter::from_xmr(double amount, const std::string& currency_code) {
    if(!is_supported_currency(currency_code)) {std::cout << "(Converter::from_xmr): This currency is not supported: " << currency_code << std::endl; return 0.00;};
    std::string currency = String::lower(currency_code);
    if(!request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=" + currency)) {
        std::cout << "Request failed" << std::endl;
    }
    nlohmann::json monj;
    monj = nlohmann::json::parse(json_string); // store json string // https://stackoverflow.com/questions/50011977/how-do-you-get-a-json-object-from-a-string-in-nlohmann-json
    if(monj.is_null()) { std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl; return 0.0;}
    json_string.clear(); // clear json string when done
    double rate =  monj["monero"][currency].get<double>();
    return amount * rate;
}
////////////////////
////////////////////
////////////////////
double Converter::to_xmr(double amount, const std::string& currency_code) 
{  
    if(String::lower(currency_code) == "btc") {return btc_to_xmr(amount);}
    if(String::lower(currency_code) == "eth") {return eth_to_xmr(amount);}
    if(String::lower(currency_code) == "dai") {return dai_to_xmr(amount);} // :(
    if(String::lower(currency_code) == "usd") {return usd_to_xmr(amount);}
    if(String::lower(currency_code) == "eur") {return eur_to_xmr(amount);}
    if(String::lower(currency_code) == "jpy") {return jpy_to_xmr(amount);}
    if(String::lower(currency_code) == "gbp") {return gbp_to_xmr(amount);}
    if(String::lower(currency_code) == "cad") {return cad_to_xmr(amount);}
    if(String::lower(currency_code) == "chf") {return chf_to_xmr(amount);}
    if(String::lower(currency_code) == "aud") {return aud_to_xmr(amount);}
    if(String::lower(currency_code) == "cny") {return cny_to_xmr(amount);}
    if(String::lower(currency_code) == "sek") {return sek_to_xmr(amount);}
    if(String::lower(currency_code) == "nzd") {return nzd_to_xmr(amount);}
    if(String::lower(currency_code) == "mxn") {return mxn_to_xmr(amount);}
    //if(String::lower(currency_code) == "") {return _to_xmr(amount);}
    std::cout << "(Converter::to_xmr): This currency is not supported: " << currency_code << std::endl;
    return 0.0;
}
////////////////////
////////////////////
double Converter::btc_to_xmr(double btc) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=btc");//("https://coincodex.com/api/coincodex/get_coin/xmr");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string ); // store json string // https://stackoverflow.com/questions/50011977/how-do-you-get-a-json-object-from-a-string-in-nlohmann-json
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate =  monj["monero"]["btc"].get<double>();
    xmr_amount = btc / rate;
    return xmr_amount;
}
////////////////////
// coincodex: price_high_24_usd, last_price_usd, price_btc
////////////////////
double Converter::eth_to_xmr(double eth) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=eth");//("https://coincodex.com/api/coincodex/get_coin/xmr");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string ); // store json string // https://stackoverflow.com/questions/50011977/how-do-you-get-a-json-object-from-a-string-in-nlohmann-json
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["eth"].get<double>(); // 1 xmr = ? eth
    xmr_amount = eth / rate;
    return xmr_amount;
}
////////////////////
double Converter::dai_to_xmr(double dai) {
    double xmr_amount = 0.0;
    // first, convert dai to usd
    request("https://api.coingecko.com/api/v3/simple/price?ids=dai&vs_currencies=usd");
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string ); // store json string // https://stackoverflow.com/questions/50011977/how-do-you-get-a-json-object-from-a-string-in-nlohmann-json
    if(monj.is_null()) { std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl; return 0.0;}
    json_string.clear(); // clear json string when done
    double dai_to_usd = monj["dai"]["usd"].get<double>();
    // now convert the dai-converted usd to xmr
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=usd");
    monj = nlohmann::json::parse( json_string ); // store json string // https://stackoverflow.com/questions/50011977/how-do-you-get-a-json-object-from-a-string-in-nlohmann-json
    if(monj.is_null()) { std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl; return 0.0;}
    json_string.clear(); // clear json string when done
    double xmr_to_usd = monj["monero"]["usd"].get<double>();
    xmr_amount = (dai_to_usd * dai) / (xmr_to_usd / dai_to_usd);
    return xmr_amount;
}
////////////////////
double Converter::usd_to_xmr(double usd) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=usd");//("https://coincodex.com/api/coincodex/get_coin/xmr");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string ); // store json string // https://stackoverflow.com/questions/50011977/how-do-you-get-a-json-object-from-a-string-in-nlohmann-json
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["usd"].get<double>(); // 1 xmr = ? usd
    xmr_amount = usd / rate;
    return xmr_amount;
}
////////////////////
double Converter::eur_to_xmr(double eur) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=eur");//("https://coincodex.com/api/coincodex/get_coin/xmr");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["eur"].get<double>(); // 1 xmr = ? eur
    xmr_amount = eur / rate;
    return xmr_amount;
}
////////////////////
double Converter::jpy_to_xmr(double jpy) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=jpy");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["jpy"].get<double>(); // 1 xmr = ? eur
    xmr_amount = jpy / rate;
    return xmr_amount;
}
////////////////////
double Converter::gbp_to_xmr(double gbp) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=gbp");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["gbp"].get<double>(); // 1 xmr = ? eur
    xmr_amount = gbp / rate;
    return xmr_amount;
}
////////////////////
double Converter::cad_to_xmr(double cad) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=cad");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["cad"].get<double>(); // 1 xmr = ? eur
    xmr_amount = cad / rate;
    return xmr_amount;
}
////////////////////
double Converter::chf_to_xmr(double chf) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=chf");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["chf"].get<double>(); // 1 xmr = ? eur
    xmr_amount = chf / rate;
    return xmr_amount;
}
////////////////////
double Converter::aud_to_xmr(double aud) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=aud");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["aud"].get<double>(); // 1 xmr = ? eur
    xmr_amount = aud / rate;
    return xmr_amount;
}
////////////////////
double Converter::cny_to_xmr(double cny) {
    double xmr_amount = 0.0;
    // get json from url
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=cny");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    // create a json object
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) {
        std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl;
        return 0.0;
    }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["cny"].get<double>(); // 1 xmr = ? eur
    xmr_amount = cny / rate;
    return xmr_amount;
}
////////////////////
double Converter::sek_to_xmr(double sek) {
    double xmr_amount = 0.0;
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=sek");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) { std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl; return 0.0; }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["sek"].get<double>(); // 1 xmr = ? eur
    xmr_amount = sek / rate;
    return xmr_amount;
}
////////////////////
double Converter::nzd_to_xmr(double nzd) {
    double xmr_amount = 0.0;
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=nzd");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) { std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl; return 0.0; }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["nzd"].get<double>(); // 1 xmr = ? eur
    xmr_amount = nzd / rate;
    return xmr_amount;
}
////////////////////
double Converter::mxn_to_xmr(double mxn)
{
    double xmr_amount = 0.0;
    request("https://api.coingecko.com/api/v3/simple/price?ids=monero&vs_currencies=mxn");
#ifdef NEROSHOP_DEBUG0
    std::cout << json_string << std::endl;
#endif
    nlohmann::json monj;
    monj = nlohmann::json::parse( json_string );
    if(monj.is_null()) { std::cout << "\033[0;91m" << "json is null !" << "\033[0m" << std::endl; return 0.0; }
    json_string.clear(); // clear json string when done
    double rate = monj["monero"]["mxn"].get<double>(); // 1 xmr = ? eur
    xmr_amount = mxn / rate;
    return xmr_amount;
}
////////////////////
////////////////////
////////////////////
double Converter::to_kg(double amount, const std::string& unit_name) const {
    if(String::lower(unit_name) == "lb" || String::lower(unit_name) == "lbs" || String::lower(unit_name) == "pound") {return lb_to_kg(amount);}
    return 0.0;
}
////////////////////
double Converter::lb_to_kg(double lb) {
    double lb2kg = 0.45359237; // 1 lb = 0.45359237 kg
    return lb * lb2kg;
}
////////////////////
////////////////////
////////////////////
////////////////////
std::string Converter::get_currency_symbol(const std::string& currency_code) {
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
bool Converter::is_supported_currency(const std::string& currency_code) {
    if(String::lower(currency_code) == "usd") return true;
    if(String::lower(currency_code) == "eur") return true;
    if(String::lower(currency_code) == "jpy") return true;
    if(String::lower(currency_code) == "gbp") return true;
    if(String::lower(currency_code) == "cad") return true;
    if(String::lower(currency_code) == "chf") return true;
    if(String::lower(currency_code) == "aud") return true;
    if(String::lower(currency_code) == "cny") return true;
    if(String::lower(currency_code) == "sek") return true;
    if(String::lower(currency_code) == "nzd") return true;
    if(String::lower(currency_code) == "mxn") return true;
    return false;
}
////////////////////
////////////////////
////////////////////
////////////////////
bool Converter::request(const std::string& url)
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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Converter::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &Converter::json_string);//&buffer);
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
std::string Converter::get_json() // const; // returns whole json as a string
{
    return json_string;
}
////////////////////
////////////////////
std::size_t Converter::write_callback(char* in, std::size_t size, std::size_t num, std::string* out)
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
