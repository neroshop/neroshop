#include <iostream>
#include <string>
// neroshop
#include "../include/neroshop.hpp"
using namespace neroshop;
// dokun-ui
#include <build.hpp>
#include DOKUN_HEADER
using namespace dokun;

lua_State * neroshop::lua_state = luaL_newstate(); // lua_state should be initialized by default
int main() {
    while (True) {
	std::string shellinput;
        std::cout << "neroshop>";
        std::cin >> shellinput;

	if (shellinput == "help") {std::cout "Available commands:\n\n help  Display list of available commands\n\n exit  Exit CLI";}; else
	if (shellinput == "exit") {break;}
    }
    return 0;
}
