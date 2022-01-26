#ifndef BROWSER_HPP_DOKUN
#define BROWSER_HPP_DOKUN
#include "platform.hpp"

#ifdef __cplusplus
#include <iostream>

class Browser { // deals with internet browser related stuff
public:
    Browser();
	~Browser();
	static void open(const std::string& url); // open a link in a browser
	static void display(const std::string& site); // displays a site in some sort of canvas
	
	std::string get();
};
#endif
#endif
