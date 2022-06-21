#include <iostream>
// neroshop
#include "../include/neroshop.hpp"
using namespace neroshop; // namespace comes after including the header files
// dokun-ui
#include <build.hpp>
#include DOKUN_HEADER
using namespace dokun;

lua_State * neroshop::lua_state = luaL_newstate(); // lua_state should be initialized by default
int main() {
    return 0;
}
