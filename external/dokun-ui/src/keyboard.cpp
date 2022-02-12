#include "../include/keyboard.hpp"

/////////////
dokun::Keyboard::Keyboard()
{}
/////////////
dokun::Keyboard::~Keyboard()
{}
/////////////
bool dokun::Keyboard::is_pressed(int key, int times_pressed)
{
#ifdef DOKUN_WIN32
	if(GetAsyncKeyState(key) & 0x8000 != 0) // & 0x8000 != 0(single speed)  != 0(multi speed)    & (1 << 16) - multiple presses
	{
	    return true;
	}
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11 // XkbGetState() is equivalent to GetAsyncKeyState() in Win32
    /*if((dokun::Keyboard::key == key) & 0x8000 != 0) {
		if(dokun::Keyboard::down) return true;
	}*/
	return (dokun::Keyboard::key == key && dokun::Keyboard::down);
#endif	
#endif

#ifdef DOKUN_SDL2
#endif
#ifdef DOKUN_GLFW	
#endif
	return false;
}
/////////////
//bool dokun::Keyboard::is_pressed(const std::string& key, int times_pressed) {
//if(key == ":") key = "colon"; // keysyms are used and are defined in <X11/keysymdef.h>
//return (dokun::Keyboard::key == XStringToKeysym(key) && dokun::Keyboard::down);
// XStringToKeysym() will work for letters and numbers, but that's about it, because the KeySym name happens to map directly for those ASCII characters. For other ASCII characters such as punctuation or space it won't work. // https://stackoverflow.com/a/25771958 
//}
/////////////
int dokun::Keyboard::is_pressed(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);     // Keyboard (table)
	luaL_checkany (L, 2);                 // key (number or string)
	luaL_optnumber(L, 3, 1);              // times_pressed
	
	if(lua_type(L, 2) == LUA_TSTRING) 
	{
		std::string key(lua_tostring(L, 2));
		if(key == "up")
		{
		    lua_pushboolean(L, dokun::Keyboard::is_pressed(DOKUN_KEY_UP, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
		if(key == "down")		
		{
		    lua_pushboolean(L, dokun::Keyboard::is_pressed(DOKUN_KEY_DOWN, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
		if(key == "left")		
		{
		    lua_pushboolean(L, dokun::Keyboard::is_pressed(DOKUN_KEY_LEFT, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
		if(key == "right")		
		{
		    lua_pushboolean(L, dokun::Keyboard::is_pressed(DOKUN_KEY_RIGHT, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
	}
	if(lua_type(L, 2) == LUA_TNUMBER) 
	{
	    int key = static_cast<int>(lua_tonumber(L, 2));
		lua_pushboolean(L, dokun::Keyboard::is_pressed(key, static_cast<int>(lua_tonumber(L, 3))));
	    return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool dokun::Keyboard::is_released(int key)
{
#ifdef __gnu_linux__ 
    return (dokun::Keyboard::key == key && dokun::Keyboard::up);
#endif
	return false;	
}
/////////////
int dokun::Keyboard::is_released(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkstring(L, 2);
	if(lua_type(L, -1) == LUA_TNUMBER) 
	{
	    lua_pushboolean(L, dokun::Keyboard::is_released((int)lua_tonumber(L, -1)));
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool dokun::Keyboard::is_down()
{
#ifdef __window__
    return dokun::Keyboard::down;
#endif
#ifdef __gnu_linux__
    return dokun::Keyboard::down;
#endif
	return false;
}
/////////////
int dokun::Keyboard::is_down(lua_State *L)
{
	lua_pushboolean(L, dokun::Keyboard::is_down());
	if(lua_isboolean(L, -1)) 
	{
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool dokun::Keyboard::is_up()
{
#ifdef __window__
    return dokun::Keyboard::up;
#endif	
#ifdef __gnu_linux__
    return dokun::Keyboard::up;
#endif
	return false;
}
/////////////
int dokun::Keyboard::is_up(lua_State *L)
{
	lua_pushboolean(L, dokun::Keyboard::is_up());
	if(lua_isboolean(L, -1)) 
	{
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/////////////
#ifdef DOKUN_X11
//KeySym dokun::Keyboard::KeycodeToKeysym(Display *d, KeyCode k, unsigned i, unsigned l)
//{
//#ifdef DOKUN_USE_XKBLIB
//    KeySym ks = XkbKeycodeToKeysym(d, k, i, l);
//    printf ("[ 7] KeySym = %d\n",(int)ks);
//    return ks;
#endif
//    return 0;
//} // to disable Xkblib use : XkbIgnoreExtension (true);
/////////////
/////////////
#ifdef DOKUN_X11 // if using X Window System on Linux
void dokun::Keyboard::fake_event(Display* display, const ::Window& window)
{
    XKeyEvent event_send;
    event_send.type = KeyPress;
    XSendEvent(display, window, true, KeyPressMask, (XEvent *)&event_send);
}
#endif
/////////////
/////////////
void dokun::Keyboard::get_input(int key, bool pressed, bool released)
{
	dokun::Keyboard::down = pressed;
	dokun::Keyboard::up   = released;
    dokun::Keyboard::key  = key;
    return; // not sure if this will work? (I want it to execute one-time)
}	
/////////////
bool dokun::Keyboard::up   (0);
bool dokun::Keyboard::down (0);
int dokun::Keyboard::key   (0);












