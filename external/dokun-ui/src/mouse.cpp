#include "../include/mouse.hpp"

/////////////
Mouse::Mouse()
{}
/////////////
Mouse::~Mouse()
{}
/////////////
// get current_mouse_pos from the system
int Mouse::x = (int)Mouse::get_position().x;
int Mouse::y = (int)Mouse::get_position().y;
/////////////
// mouse movement
bool Mouse::is_moved()
{
    return dokun::Window::mouse_moved;
}
/////////////
int Mouse::is_moved(lua_State *L)
{
	lua_pushboolean(L, Mouse::is_moved());
	return 1;
}
///////////// https://unix.stackexchange.com/questions/25601/how-do-mouse-events-work-in-linux : /dev/input/mice (this device collects events from all connected mice)
bool Mouse::is_scrolled()
{
#ifdef DOKUN_WIN32
#endif
#ifdef __gnu_linux__

#endif
	return false;
}
/////////////
int Mouse::is_scrolled(lua_State *L)
{
	lua_pushboolean(L, (int)Mouse::is_scrolled());
	return 1;
}
/////////////
bool Mouse::is_pressed(int button, int times_pressed) // a specific button is pressed but never released
{
	if(button == 1) // left
	{
		return (dokun::Window::mouse_pressed && dokun::Window::mouse_button == 1);
	}
	if(button == 2) // middle
	{
		return (dokun::Window::mouse_pressed && dokun::Window::mouse_button == 2);
	}
	if(button == 3) // right
	{
		return (dokun::Window::mouse_pressed && dokun::Window::mouse_button == 3);
	}
	return false; // invalid button
}
/////////////
int Mouse::is_pressed(lua_State *L)
{
	lua_pushboolean(L, Mouse::is_pressed(static_cast<int>(lua_tonumber(L, -1))));
	return 1;
}
/////////////
bool Mouse::is_released(int button) // a specific button is released
{
    if(button == 1)
	{
		return (dokun::Window::mouse_released && dokun::Window::mouse_button == 1);
	}
    if(button == 2)
	{
		return (dokun::Window::mouse_released && dokun::Window::mouse_button == 2);
	}
    if(button == 3)
	{
		return (dokun::Window::mouse_released && dokun::Window::mouse_button == 3);
	}
	return false; // invalid button
}
/////////////
int Mouse::is_released(lua_State *L)
{
	lua_pushboolean(L, Mouse::is_released(static_cast<int>(lua_tonumber(L, -1))));
    return 1;
}
/////////////
bool Mouse::is_clicked(int button) { // when mouse is both pressed and released
    // ...
    return false;
}
/////////////
// any mouse button is pressed
bool Mouse::is_down()
{
	return (Mouse::is_pressed(1) || Mouse::is_pressed(2) || Mouse::is_pressed(3));
}
/////////////
int Mouse::is_down(lua_State *L)
{
	lua_pushboolean(L, Mouse::is_down());
	return 1;
}
/////////////
// any mouse button is released
bool Mouse::is_up()
{
	return (Mouse::is_released(1) || Mouse::is_released(2) || Mouse::is_released(3));
}
/////////////
int Mouse::is_up(lua_State *L)
{
	lua_pushboolean(L, Mouse::is_up());
	return 1;
}
/////////////
bool Mouse::is_over(double x, double y) // hover over a position
{
	return (get_position().x == x && get_position().y == y);
}
/////////////
bool Mouse::is_over(const Vector2& position)
{
    return is_over(position.x, position.y);
}
/////////////
bool Mouse::is_over(const dokun::Window& window) // hover over a window
{
    if((get_position().x < window.get_position().x + window.get_size().x) && (window.get_position().x < get_position().x) && (get_position().y < window.get_position().y + window.get_size().y) && (window.get_position().y < get_position().y))
        return true;
	return false;
}
/////////////
/////////////
bool Mouse::is_over(double x, double y, int width, int height) // hover over a 2d object
{
    dokun::Window * window = dokun::Window::get_active();
	if(!window) return false;
	return((get_position(*window).x < x  + width) && (x < get_position(*window).x) && (get_position(*window).y < y + height) && (y < get_position(*window).y) == true);
}
/////////////
bool Mouse::is_over(const Vector2& pos, const Vector2& size)
{
	return is_over(pos.x, pos.y, size.x, size.y);
}
/////////////
bool Mouse::is_over(const Vector4& rect) // hover over a 2d object
{
	return is_over(rect.xy, rect.zw);
}
/////////////
int Mouse::is_over(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // mouse
	luaL_checkany(L, 2); // number, window, sprite, model, or ui
	if(lua_type(L, 2) == LUA_TNUMBER && lua_type(L, 3) == LUA_TNUMBER)
	{
		lua_pushboolean(L, Mouse::is_over(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5) ));
		return 1;
	}
	if(lua_istable(L, 2))
	{
		lua_getglobal(L, "is_window"); // 3
		lua_pushvalue(L, 2); // 4
		lua_call(L, 1, 1); // 5
		int is_window = static_cast<int>(lua_toboolean(L, -1));

		if(is_window)
		{
			lua_getfield(L, 2, "udata");
			if(lua_isuserdata(L, -1))
			{
		        dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
			    lua_pushboolean(L, Mouse::is_over(*window));
			    return 1;
			}
		}
	}
    lua_pushboolean(L, false);
    return 1;
}
/////////////
/////////////
void Mouse::hide()
{
#ifdef DOKUN_WIN32
	ShowCursor(false);
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    dokun::Window * window = dokun::Window::get_active();
	if(window)
	{		
	Cursor invisibleCursor;
    XColor black;
static char noData[] = { 0,0,0,0,0,0,0,0 };
black.red = black.green = black.blue = 0;

Pixmap bitmapNoData = XCreateBitmapFromData(window->get_display(), window->get_handle(), noData, 8, 8);
invisibleCursor = XCreatePixmapCursor(window->get_display(), bitmapNoData, bitmapNoData, 
                                     &black, &black, 0, 0);
XDefineCursor(window->get_display(), window->get_handle(), invisibleCursor);
//XFreeCursor(window->get_display(), invisibleCursor);
//XFreePixmap(window->get_display(), bitmapNoData);
	}
#endif
#endif
}
/////////////
int Mouse::hide(lua_State *L)
{
	Mouse::hide();
	return 0;
}
/////////////
void Mouse::show()
{
#ifdef DOKUN_WIN32
	ShowCursor(true);
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
#endif
#endif
}
/////////////
int Mouse::show(lua_State *L)
{
	Mouse::show();
	return 0;
}
/////////////
void Mouse::restore() // restore cursor
{
	dokun::Window * window = dokun::Window::get_active();
	if(window)
	{ 
    #ifdef DOKUN_WIN32
	    HCURSOR arrow = LoadCursor(nullptr, IDC_ARROW);
		//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)arrow);
    #endif	
    #ifdef __gnu_linux__
    #ifdef DOKUN_X11
        XUndefineCursor(window->get_display(), window->get_handle());
    #endif
    #endif
	    show(); // show cursor
	}
}
int Mouse::restore(lua_State *L)
{
    return 0;
}
/////////////
  // setters
void Mouse::set_position(int x, int y) // set global position(on desktop)
{
    Mouse::x = x;
	Mouse::y = y;
#ifdef DOKUN_WIN32
	SetCursorPos(Mouse::x, Mouse::y);
#endif

#ifdef __gnu_linux__
#ifdef DOKUN_X11
    Display * display = XOpenDisplay(nullptr);
    XWarpPointer(display, None, DefaultRootWindow(display),
	    0, 0, 0, 0, x, y);
	//XFlush(display);
	XCloseDisplay(display);
#endif
#endif
}
/////////////
void Mouse::set_position(const Vector2& position)
{
	Mouse::set_position((int)position.x, (int)position.y);
}
/////////////
void Mouse::set_position(int x, int y, const dokun::Window& window) // set local position(in window)
{
    Mouse::x = x;
	Mouse::y = y;
#ifdef DOKUN_WIN32
	POINT position;
	position.x = Mouse::x;
	position.y = Mouse::y;
	ClientToScreen(window.get_handle(), & position);
	SetCursorPos(position.x, position.y);
#endif

#ifdef __gnu_linux__
#ifdef DOKUN_X11
    XWarpPointer(window.get_display(), None, window.get_handle(), 0, 0, window.get_width(), window.get_height(), x, y);
#endif
#endif
}
/////////////
void Mouse::set_position(const Vector2& position, const dokun::Window& window)
{
	Mouse::set_position(position.x, position.y, window);
}
/////////////
int Mouse::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
    if(lua_istable(L, 4))
	{
		lua_getfield(L, 4, "udata");
		if(lua_isuserdata(L, -1))
		{
		    dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
	        Mouse::set_position((int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3), *window);
			return 0;
	    }
	}
	Mouse::set_position((int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
	return 0;
}
/////////////
void Mouse::set_cursor(unsigned long cursor)
{
	dokun::Window * window = dokun::Window::get_active();
	if(window != nullptr)
	{
	#ifdef DOKUN_WIN32
		HCURSOR cursor0 = LoadCursor(nullptr, reinterpret_cast<LPCSTR>(cursor));
		//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)cursor0); // DWORD = unsigned long
    #endif		
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11		
		Cursor cursor0 = XCreateFontCursor(window->get_display(), static_cast<unsigned int>(cursor)); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor0);
		XFreeCursor(window->get_display(), cursor0);
	#endif
	#endif
	}	
}
void Mouse::set_cursor(const Image& cursor) // doesnt work
{
	dokun::Window * window = dokun::Window::get_active();
	if(window)
	{
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    XColor color;
    //color.red = color.green = color.blue = 255;

    Pixmap data = XCreateBitmapFromData(window->get_display(), window->get_handle(), static_cast<const char*>(cursor.get_data()), 
	    cursor.get_width(), cursor.get_height());
    Cursor cursor0 = XCreatePixmapCursor(window->get_display(), data, data, 
        &color, &color, 0, 0);
    XDefineCursor(window->get_display(), window->get_handle(), cursor0);
    XFreeCursor(window->get_display(), cursor0);
    XFreePixmap(window->get_display(), data);
#endif
#endif	
    }
}
void Mouse::set_cursor(const std::string& cursor_file, int width, int height)
{
#ifdef DOKUN_WIN32
    if(width <= 0) 
	{
	HCURSOR cursor = LoadCursorFromFile(cursor_file.c_str());  // uses standard cursor size
    if (!cursor)
	    std::cerr <<"Could not open cursor " << cursor_file <<  std::endl;
	SetCursor(cursor); 
	}
	if(width > 0) {
	HCURSOR cursor = (HCURSOR)LoadImage(nullptr, cursor_file.c_str(), IMAGE_CURSOR, width, height, LR_LOADFROMFILE); // uses custom cursor size
	if (!cursor)
	    std::cerr <<"Could not open cursor " << cursor_file <<  std::endl;
	SetCursor(cursor);
	}
#endif	
}
int Mouse::set_cursor(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	luaL_optnumber(L, 3, 0); // width
	luaL_optnumber(L, 4, 0); // height
	if(lua_type(L, 2) == LUA_TNUMBER)
	{
		set_cursor((int)lua_tonumber(L, 2));
	}	
	if(lua_type(L, 2) == LUA_TSTRING)
	{
		set_cursor(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	}
	if(lua_type(L, 2) == LUA_TTABLE)
	{
		lua_getfield(L, -1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
			set_cursor(*image);
		}
	}	
    return 0;
}
/////////////
/////////////
// GETTERS
/////////////
Vector2 Mouse::get_position() // get global position(on desktop)
{
    int x = 0;
	int y = 0;
#ifdef DOKUN_WIN32
    POINT position;
    GetCursorPos(& position);
	x = position.x;
	y = position.y;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    Display * display = XOpenDisplay(NULL);
    assert(display);

    Window window_returned;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;
    if(XQueryPointer(display, DefaultRootWindow(display), &window_returned,
                &window_returned, &root_x, &root_y, &win_x, &win_y,
                &mask_return) != 1)
	{
		dokun::Logger("Mouse not found");
		return -1;
	}
	x = root_x;
	y = root_y;

    XCloseDisplay(display);
#endif
#endif
    return Vector2(x, y);
}
/////////////
Vector2 Mouse::get_position(const dokun::Window& window) // get local position (inside window)
{
    int x = 0;
    int y = 0;
#ifdef DOKUN_WIN32
	POINT position;
	GetCursorPos(& position);
	ScreenToClient(window.get_handle(), & position);
	x = position.x;
	y = position.y;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    Window window_returned;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;
    if(XQueryPointer(window.get_display(), window.get_handle(), &window_returned,
                &window_returned, &root_x, &root_y, &win_x, &win_y,
                &mask_return) != 1)
	{
		dokun::Logger("Mouse not found");
		return -1;
	}
    x = win_x;
    y = win_y;
#endif
#endif
    return Vector2(x, y);
}
/////////////
int Mouse::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	if(lua_istable(L, 2)) // relative to window
	{
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
		    dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
	        lua_pushnumber(L, (int)Mouse::get_position(*window).x);
	        lua_pushnumber(L, (int)Mouse::get_position(*window).y);
		    return 2;
	    }
	}
	lua_pushnumber(L, (int)Mouse::get_position().x); // relative to desktop
	lua_pushnumber(L, (int)Mouse::get_position().y);
	return 2;
}
/////////////
Vector3 Mouse::get_normalized_position(const dokun::Window& window)
{
	int width  = 0;
	int height = 0;
	double mouse_x = get_position(window).x;
	double mouse_y = get_position(window).y;
#ifdef DOKUN_WIN32
    HWND handle = ::GetActiveWindow();
	if(IsWindow(handle) && (handle == GetForegroundWindow()))
	{
		RECT size;
	    GetWindowRect(handle, &size);
	    width  = (size.right - size.left);
		height = (size.bottom - size.top);
	}
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    XWindowAttributes       gwa;
    XGetWindowAttributes(window.get_display(), window.get_handle(), &gwa); // get attributes while app is running; only gets width and height no x and y
	width  = gwa.width ;
	height = gwa.height;
#endif
#endif
    double norm_x = (2.0f * mouse_x) / width - 1.0f;
	double norm_y = 1.0f - (2.0f * mouse_y) / height;
	double norm_z = 1.0;
    return Vector3(norm_x, norm_y, norm_z);
}
/////////////
int Mouse::get_delta() // returns -120 or lower if scrolling down, 120 or higher if scrolling up
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_SDL2
#ifndef DOKUN_GLFW
	//return dokun::Window::
	//mouse_delta;
#endif
#endif
#endif
	return 0;
}
/////////////
int Mouse::get_delta(lua_State *L)
{
	lua_pushnumber(L, Mouse::get_delta());
	return 1;
}
/////////////
// This function seems to only work in certain parts of the code like after the draw() call
Vector3i Mouse::get_color() // returns the pixel color that the mouse pointer is pointing to
{
    dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0));//dokun::Window::get_active();
	//if(!window) return Vector3i(0, 0, 0);//Vector3(-1, -1, -1);
	int window_width = window->get_client_width();
	int window_height = window->get_client_height();
#ifdef DOKUN_OPENGL // requires opengl context
    struct{ GLubyte red, green, blue; } pixel;
    glReadPixels(get_position(* window).x, window_height - 1 - get_position(* window).y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);//&image);
    return Vector3i(pixel.red, pixel.green, pixel.blue);//(image[0], image[1], image[2]);//
#endif
//////////////////	
/*#ifdef DOKUN_WIN32
	    COLORREF color = GetPixel(GetDC(dokun::Window::get_active()->get_handle()), (int)Mouse::get_position(*dokun::Window::get_active()).x, (int)Mouse::get_position(*dokun::Window::get_active()).y);
		return Vector3(GetRValue(color), GetGValue(color), GetBValue(color));
#endif
#ifdef DOKUN_LINUX
    XColor color;
    Display *d = window->get_display();//XOpenDisplay((char *) NULL);
    int x = 0;  // Pixel x 
    int y = 0;  // Pixel y

    XImage * image = XGetImage (d, XRootWindow (d, XDefaultScreen (d)), x, y, 1, 1, AllPlanes, XYPixmap);
    color.pixel = XGetPixel(image, 0, 0);
    XFree(image);
    
    XQueryColor (d, XDefaultColormap(d, XDefaultScreen (d)), &color);
    return Vector3(color.red / 256, color.green / 256, color.blue / 256);
#endif*/
	return Vector3i(0, 0, 0);//Vector3(-1, -1, -1);
}
/////////////
int Mouse::get_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);

	lua_pushnumber(L, (int)Mouse::get_color(/*(int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3)*/).x);
	lua_pushnumber(L, (int)Mouse::get_color(/*(int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3)*/).y);
	lua_pushnumber(L, (int)Mouse::get_color(/*(int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3)*/).z);
	return 3;
}
/////////////
Vector2i Mouse::get_size() //const
{
#ifdef DOKUN_WIN32
    return Vector2i(SM_CXCURSOR, SM_CYCURSOR);
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
#endif
#endif
    return Vector2i(0, 0);
}
int Mouse::get_size(lua_State * L)
{
    return 2;
}
/////////////
#ifdef DOKUN_WIN32
#ifndef DOKUN_SDL2
#ifndef DOKUN_GLFW
#endif
#endif
#endif
/////////////
/////////////
double Mouse::rotate(double x, double y)
{
	double mouse_x = get_position(*dokun::Window::get_active()).x;
	double mouse_y = get_position(*dokun::Window::get_active()).y;
    double a = x - mouse_x;                                            // try mouse_x - x
    double b = y - mouse_y;                                            // try mouse_y - y
	double mouse_angle = atan2(-a, -b) * 180 / 3.14159265358979323846; // try atan2(+a, +b)
	
	double z = mouse_angle + 180;
    return z;	
}
/* 
 double mouse_rot = Mouse::rotate(sprite->get_position().x, sprite->get_position().y);
 sprite->rotate(mouse_rot);
 
 if mouse_is_over(sprite) and mouse_is_down() then rotate_with_mouse(sprite)
*/
/////////////
Vector3 Mouse::rotate(double x, double y, double z)
{
    return Vector3(0, 0, 0);
}
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
