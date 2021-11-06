#include "../include/keyboard.hpp"

/////////////
Keyboard::Keyboard()
{}
/////////////
Keyboard::~Keyboard()
{}
/////////////
bool Keyboard::is_pressed(int key, int times_pressed)
{
#ifdef __windows__
	if(GetAsyncKeyState(key) & 0x8000 != 0) // & 0x8000 != 0(single speed)  != 0(multi speed)    & (1 << 16) - multiple presses
	{
	    return true;
	}
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11 // XkbGetState() is equivalent to GetAsyncKeyState() in Win32
    if((Keyboard::key == key) & 0x8000 != 0)
	{
		if(Keyboard::down) return true;
	}
#endif	
#endif

#ifdef DOKUN_SDL2
#endif
#ifdef DOKUN_GLFW	
#endif
	return false;
}
/////////////
int Keyboard::is_pressed(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);     // Keyboard (table)
	luaL_checkany (L, 2);                 // key (number or string)
	luaL_optnumber(L, 3, 1);              // times_pressed
	
	if(lua_type(L, 2) == LUA_TSTRING) 
	{
		std::string key(lua_tostring(L, 2));
		if(key == "up")
		{
		    lua_pushboolean(L, Keyboard::is_pressed(DOKUN_KEY_UP, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
		if(key == "down")		
		{
		    lua_pushboolean(L, Keyboard::is_pressed(DOKUN_KEY_DOWN, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
		if(key == "left")		
		{
		    lua_pushboolean(L, Keyboard::is_pressed(DOKUN_KEY_LEFT, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
		if(key == "right")		
		{
		    lua_pushboolean(L, Keyboard::is_pressed(DOKUN_KEY_RIGHT, static_cast<int>(lua_tonumber(L, 3)))); 
		    return 1;
		}
	}
	if(lua_type(L, 2) == LUA_TNUMBER) 
	{
	    int key = static_cast<int>(lua_tonumber(L, 2));
		lua_pushboolean(L, Keyboard::is_pressed(key, static_cast<int>(lua_tonumber(L, 3))));
	    return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool Keyboard::is_released(int key)
{
#ifdef __gnu_linux__ 
#endif
	return false;	
}
/////////////
int Keyboard::is_released(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkstring(L, 2);
	if(lua_type(L, -1) == LUA_TNUMBER) 
	{
	    lua_pushboolean(L, Keyboard::is_released((int)lua_tonumber(L, -1)));
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool Keyboard::is_down()
{
#ifdef __window__
    return Keyboard::down;
#endif
#ifdef __gnu_linux__
    return Keyboard::down;
#endif
	return false;
}
/////////////
int Keyboard::is_down(lua_State *L)
{
	lua_pushboolean(L, Keyboard::is_down());
	if(lua_isboolean(L, -1)) 
	{
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool Keyboard::is_up()
{
#ifdef __window__
    return Keyboard::up;
#endif	
#ifdef __gnu_linux__
    return Keyboard::up;
#endif
	return false;
}
/////////////
int Keyboard::is_up(lua_State *L)
{
	lua_pushboolean(L, Keyboard::is_up());
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
KeySym Keyboard::KeycodeToKeysym(Display *d, KeyCode k, unsigned i, unsigned l)
{
#ifdef DOKUN_USE_XKBLIB
    KeySym ks = XkbKeycodeToKeysym(d, k, i, l);
    printf ("[ 7] KeySym = %d\n",(int)ks);
    return ks;
#endif
    return 0;
} // to disable Xkblib use : XkbIgnoreExtension (true);
/////////////
/////////////
XKeyEvent Keyboard::createKeyEvent(Display *display, Window &win,
                           Window &winRoot, bool press,
                           int keycode, int modifiers)
{
  XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycode);
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;	
}					   
#endif
/////////////
/////////////
void Keyboard::get_input(int key, bool pressed, bool released)
{
	Keyboard::down = pressed;
	Keyboard::up   = released;
    Keyboard::key  = key; 	
}	
/////////////
bool Keyboard::up   (0);
bool Keyboard::down (0);
int Keyboard::key   (0);












