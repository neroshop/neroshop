// neroshop
#include "../include/neroshop.hpp"
using namespace neroshop;
// dokun-ui
#include <build.hpp>
#include DOKUN_HEADER
using namespace dokun;

lua_State * neroshop::lua_state = luaL_newstate(); // lua_state should be initialized by default
int main() {
    bool running = True;
    while (running) {
	std::string shellinput;
        std::cout << "neroshop>";
        std::cin >> shellinput;

	switch2(input, { // "switch replacement" where STRINGS are possible
	{"help", defer(say, "help text here")},
	{"exit", defer(quit, "exited", &running)}};
	
    }
    return 0;
}
