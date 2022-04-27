// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef CONVERTER_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define CONVERTER_HPP_NEROSHOP

#include <iostream>
// curl
#include <curl/curl.h>
// nlohmann-json
#include <nlohmann/json.hpp>
// dokun-ui
#include <string.hpp>

//namespace neroshop {
//enum class tracker {coinmarketcap, coincodex, coingecko};
// https://www.coingecko.com/api/documentations/v3
namespace neroshop {
class Converter {
public:
    // to other currencies//static double xmr_to_btc(double xmr);//static double xmr_to_usd(double xmr);
    static double from_xmr(double amount, const std::string& currency_code);
    // to_xmr - https://api.coingecko.com/api/v3/simple/supported_vs_currencies (list of supported vs_currencies)
    static double to_xmr(double amount, const std::string& currency_code);
    static double btc_to_xmr(double btc); // the king
    static double eth_to_xmr(double eth); // #2 coin
    static double dai_to_xmr(double dai); // the one and only truly decentralized stablecoin!
    static double usd_to_xmr(double usd);
    static double eur_to_xmr(double eur);
    static double jpy_to_xmr(double jpy); // japanese yen
    static double gbp_to_xmr(double gbp); // great british pound
    static double aud_to_xmr(double aud); // australian dollar    
    static double cad_to_xmr(double cad); // canadian dollar
    static double chf_to_xmr(double chf); // swiss franc
    static double cny_to_xmr(double cny); // chinese yuan
    static double sek_to_xmr(double sek); // swedish krona
    static double nzd_to_xmr(double nzd); // new zealand dollar
    static double mxn_to_xmr(double mxn); // mexican peso
    //static double _to_xmr(double ); // I chose to convert from the most traded currencies // https://www.countries-ofthe-world.com/most-traded-currencies.html // free country flags: https://github.com/hampusborgos/country-flags | https://flagpedia.net/download/images  
    // weight (mass)(kilogram[kg] is commonly used worldwide while pounds[lbs] is mostly used in both US and UK)
    double to_kg(double amount, const std::string& unit_name) const;
    static double lb_to_kg(double lb); //static double pound_to_kilogram(double pound); // The correct way of abbreviation in expressing singular or plural pounds is “lb.” though “lbs.”, which stands for libra, is the common abbreviation used in expressing pounds
    // getters
    static std::string get_currency_symbol(const std::string& currency_code);
    // boolean
    static bool is_supported_currency(const std::string& currency_code);
private:
    static bool request(const std::string& url);
    static std::string get_json();// const; // returns whole json as a string //static std::string get_json(const std::string& key);// const
    static std::size_t write_callback(char* in, std::size_t size, std::size_t num, std::string* out);
    static std::string json_string;//std::string url;
};
}
#endif
/*Usage:
	std::cout << "1 USD = " << "\033[0;96m" << std::fixed << std::setprecision(12) << Converter::usd_to_xmr(1.00                  ) << "\033[0m" << " xmr" << std::endl;
	std::cout << "1 EUR = " << "\033[0;96m" << std::fixed << std::setprecision(12) << Converter::eur_to_xmr(1.00                  ) << "\033[0m" << " xmr" << std::endl;
	std::cout << "1 BTC = " << "\033[0;96m" << std::fixed << std::setprecision(12) << Converter::btc_to_xmr(1.00000000            ) << "\033[0m" << " xmr" << std::endl;
	std::cout << "1 DAI = " << "\033[0;96m" << std::fixed << std::setprecision(12) << Converter::dai_to_xmr(1.000000000000000000  ) << "\033[0m" << " xmr" << std::endl;
	std::cout << "1 ETH = " << "\033[0;96m" << std::fixed << std::setprecision(12) << Converter::eth_to_xmr(1.000000000000000000  ) << "\033[0m" << " xmr" << std::fixed << std::setprecision(2) << std::endl;

	double pounds = 1;
	std::cout << pounds << " lb = " << Converter::lb_to_kg(pounds) << " kg" << std::endl;
	pounds = 50;
	std::cout << pounds << " lb = " << Converter::lb_to_kg(pounds) << " kg" << std::endl;

	std::cout << "1 XMR = " << "\033[0;96m" << std::fixed << std::setprecision(2) << Converter::xmr_to_usd(1.000000000000) << "\033[0m" << " USD" << std::endl;
	std::cout << "1 XMR = " << "\033[0;96m" << std::fixed << std::setprecision(2) << Converter::xmr_to_eur(1.000000000000) << "\033[0m" << " EUR" << std::endl;
	std::cout << "1 XMR = " << "\033[0;96m" << std::fixed << std::setprecision(8) << Converter::xmr_to_btc(1.000000000000) << "\033[0m" << " BTC" << std::endl;
	std::cout << "1 XMR = " << "\033[0;96m" << std::fixed << std::setprecision(18) << Converter::xmr_to_eth(1.000000000000) << "\033[0m" << " ETH" << std::fixed << std::setprecision(2) << std::endl;	
*/    
