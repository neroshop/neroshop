#include "../include/window.hpp"

////////////
dokun::Window::Window(void) : width (DOKUN_DEFAULT_WIDTH), height (DOKUN_DEFAULT_HEIGHT), mode (0), x (DOKUN_DEFAULT_POSITION_X), y (DOKUN_DEFAULT_POSITION_Y)
, color (0.0, 0.0, 0.0, 0.0) // screen properties
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
, handle (nullptr), instance (nullptr), style (WS_OVERLAPPEDdokun::Window | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#endif
#endif

#ifdef DOKUN_X11
#endif
{	
	Factory::get_window_factory()->store(this);
}
////////////
dokun::Window::~Window(void)
{
	Factory::get_window_factory()->release(this);
}
////////////
void dokun::Window::create()
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (instance == nullptr) // register window once
	{
	    WNDCLASS wc       = { 0 };
	    instance          = GetModuleHandle(nullptr);
        wc.style          = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc    = static_cast<WNDPROC>(dokun::Window::WndProc);
        wc.cbClsExtra     = 0; // no extra data
        wc.cbWndExtra     = 0; 
        wc.hInstance      = instance;
	    wc.hIcon          = LoadIcon(instance, IDI_APPLICATION); // default icon		
	    wc.hCursor        = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground  = nullptr;
        wc.lpszMenuName   = nullptr;
		std::string class_name = "Window" + std::to_string(Factory::get_window_factory()->get_location(this));
		wc.lpszClassName       = static_cast<const char *>(class_name.c_str());
        if(!RegisterClass(& wc))
        {
	        MessageBox(nullptr, "Could not register window", "dokun", 0);
			return;
        }	
	}
    handle = CreateWindow(get_name().c_str(), nullptr, style, x, y, width, height, nullptr, nullptr, instance, this);
	if(handle == nullptr)
	{
	    MessageBox(nullptr, "Could not create a window", "dokun", 0);
	    return;
	}
    SetWindowText(handle, title.c_str()); // set the title
	if (mode == 1)                        // set the mode
	{   
		SetWindowLong(handle, GWL_STYLE, 0); // hides title_bar
		SetWindowPos(handle, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
	}
#endif
#endif

#ifdef __gnu_linux__
#ifdef DOKUN_X11
	display = XOpenDisplay(nullptr);
    if (!display) {
        Logger("Could not create X11 Display");
    }
#ifndef DOKUN_USE_XKBLIB
    // check Xkb (x keyboard extension lib) version
    int libmajor = XkbMajorVersion, libminor = XkbMinorVersion; // set initial version //printf ("XKBlib[ 2] libmajor, libminor = %d,%d\n",libmajor,libminor); // version 1.0
    int kb_op_code, kb_event_code, kb_err_code;
    bool doit = XkbLibraryVersion(&libmajor, &libminor); // returns true if version is compatible - https://www.x.org/releases/X11R7.5/doc/man/man3/XkbLibraryVersion.3.html
    //printf ("[ 3] XkbLibraryVersion(&libmajor, &libminor) = %d\n",doit); // version 1 = equal to 1 if true (success)
    // initialize the x keyboard extension
    if (doit) doit = XkbQueryExtension(display, &kb_op_code, &kb_event_code, &kb_err_code, &libmajor, &libminor);//if(XkbQueryExtension ( display, opcode_rtrn, event_rtrn, error_rtrn, major_in_out, minor_in_out ) != True) { Logger("X Keyboard ext could not be initialized : Compatible extension is not present");}//XkbOpenDisplay ( display_name, event_rtrn, error_rtrn, major_in_out, minor_in_out, reason_rtrn);
    //printf ("[ 5] XkbQueryExtension(...) = %d\n",doit); // return true if initialized otherwise false
    if (doit) {
        #define DOKUN_USE_XKBLIB 
    }
#endif
#ifdef DOKUN_USE_XKBLIB
    //Logger::push(String("Running XKBlib version ") + String(libmajor) + "." + String(libminor)); // has nothing to do with graphics so dont add to log head
#endif
#if !defined(DOKUN_OPENGL) || !defined(DOKUN_VULKAN)
    //Logger("No API defined. Using normal window");
    int screen           = DefaultScreen(display);
	unsigned long black  = BlackPixel(display, screen);
	unsigned long white  = WhitePixel(display, screen);

   	window = XCreateSimpleWindow(display,DefaultRootWindow(display), x, y,	
		width, height, 5, white, black);
    // set window title
	XSetStandardProperties(display, window, title.c_str(), "HI!", None, nullptr, 0, nullptr);
	// create the Graphics Context 
    gc = XCreateGC(display, window, 0, 0); // XFreeGC(display, gc)        
    // set the foreground and background colors
	XSetBackground(display, gc, white);
	XSetForeground(display, gc, black);
	// selects which types of input are allowed
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask |
	    ButtonPressMask | ButtonReleaseMask | 
		EnterWindowMask | LeaveWindowMask | 
		PointerMotionMask | PointerMotionHintMask |
		Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask | ButtonMotionMask | 
		KeymapStateMask | ExposureMask | VisibilityChangeMask | StructureNotifyMask |
		/*ResizeRedirectMask |*/ SubstructureNotifyMask | SubstructureRedirectMask | 
		   FocusChangeMask | PropertyChangeMask | ColormapChangeMask | OwnerGrabButtonMask); // selects which types of input are allowed	
#endif
#ifdef DOKUN_OPENGL
if(Renderer::get_current_API() == "OpenGL") {
    int glx_major, glx_minor;
    glXQueryVersion(display, &glx_major, &glx_minor); // FBConfigs were added in GLX version 1.3
    if(!(glx_major >= 1 && glx_minor >= 3)) {
       Logger("GLX version must be 1.3 or higher");
       exit(1);
    } //Logger::push(String("Running GLX version    ") + String(glx_major) + "." + String(glx_minor));
    
    // Get the default screen's GLX extension list
    const char *extensions = glXQueryExtensionsString(display, DefaultScreen(display));
    //Logger::push("GLX extensions found: " + std::string(extensions));
    
	GLint visual_attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };//{ GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None }; //{ GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	/*{
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_dokun::Window_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      //GLX_SAMPLE_BUFFERS  , 1,
      //GLX_SAMPLES         , 4,
      None	
	};*/

    PFNGLXCHOOSEFBCONFIGPROC pfnglxChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)glXGetProcAddress((const GLubyte*)"glXChooseFBConfig"); // retrieve function from DLL since typedef(copy) of function isn't working ain't working
    PFNGLXGETVISUALFROMFBCONFIGPROC pfnglxGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC)glXGetProcAddress((const GLubyte*)"glXGetVisualFromFBConfig");

    // Getting framebuffer config
    int fbcount;// framebuffer_count
    fbc =  pfnglxChooseFBConfig(display, DefaultScreen(display), visual_attributes, &fbcount);//glXChooseFBConfig(display, visual_info->screen, visual_attributes, &fbcount);//
    if (!fbc)
    {
        Logger("Failed to retrieve a framebuffer config");
		Logger("Try a rescue solution");

		PFNGLXGETFBCONFIGSPROC pfnglxGetFBConfig = (PFNGLXGETFBCONFIGSPROC)glXGetProcAddress((const GLubyte*)"glXGetFBConfigs");
		fbc = pfnglxGetFBConfig(display, DefaultScreen(display), &fbcount);

		if (!fbc) 
		{
			Logger("Sorry it doesn't works too");
			exit (1);
		}
    }
  // Retrieving visual information	
    visual_info = pfnglxGetVisualFromFBConfig(display, fbc[0]);//glXChooseVisual(display, 0, visual_attributes); //glXGetVisualFromFBConfig(display, fbc[0]);// Chosen visual_id = 0x << visual_info->visualid << std::endl;
    if(visual_info == nullptr) 
	{
		Logger("No visual found");
		return;
	}
  // create colormap
  Colormap cmap;
  swa.colormap = cmap = XCreateColormap(display,
                                        RootWindow(display, visual_info->screen), 
                                        visual_info->visual, AllocNone );
  swa.background_pixmap = None;
  swa.border_pixel      = 0;
  swa.event_mask        = KeyPressMask | KeyReleaseMask |
	    ButtonPressMask | ButtonReleaseMask | 
		EnterWindowMask | LeaveWindowMask | 
		PointerMotionMask | PointerMotionHintMask |
		Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask | ButtonMotionMask | 
		KeymapStateMask | ExposureMask | VisibilityChangeMask | StructureNotifyMask |
		SubstructureNotifyMask | SubstructureRedirectMask | 
		FocusChangeMask | PropertyChangeMask | ColormapChangeMask | OwnerGrabButtonMask; // ResizeRedirectMask StructureNotifyMask=default
  swa.override_redirect = True;
  // create window
  // std::cout << "Creating window\n";
  window = XCreateWindow(display, RootWindow(display, visual_info->screen), 
                              x, y, width, height, 0, visual_info->depth, InputOutput, 
                              visual_info->visual, 
                              CWBorderPixel|CWColormap|CWEventMask, &swa);
  if (!window) {
    Logger("Window creation failed.");
    exit(1);
  }
  XStoreName(display, window, title.c_str()); // set window title
}
#endif	
#ifdef DOKUN_VULKAN
   // create a vulkan window
#endif
	if(mode == 1) // set mode
	{
        Atom    atom_list[2] = { XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False), None};
		XChangeProperty(display, window, XInternAtom(display, "_NET_WM_STATE", False), XA_ATOM, 32, PropModeReplace, 
		    reinterpret_cast<unsigned char *>(atom_list), 1);
	}
#endif // endif DOKUN_X11
#ifdef DOKUN_XCB
    connection = xcb_connect (nullptr, nullptr);
#endif
#ifdef DOKUN_WAYLAND
    display = wl_display_connect (nullptr);
    wl_display_roundtrip (display);
#ifdef DOKUN_OPENGL
    egl_display = eglGetDisplay (display);
	eglInitialize (egl_display, nullptr, nullptr);
	
	eglBindAPI (EGL_OPENGL_API);
	EGLint attributes[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
	EGL_NONE};
	EGLConfig config;
	EGLint num_config;
	eglChooseConfig (egl_display, attributes, &config, 1, &num_config);
	this->egl_context = eglCreateContext (egl_display, config, EGL_NO_CONTEXT, nullptr);
	
	this->surface = wl_compositor_create_surface (compositor);
	this->shell_surface = wl_shell_get_shell_surface (shell, this->surface);
	//wl_shell_surface_add_listener (this->shell_surface, &shell_surface_listener, window);
	wl_shell_surface_set_toplevel (this->shell_surface);
	this->egl_window = wl_egl_window_create (this->surface, width, height);
	this->egl_surface = eglCreateWindowSurface (egl_display, config, this->egl_window, nullptr);
#endif    
#endif // endif DOKUN_WAYLAND
#endif // endif __gnu_linux__

#ifdef DOKUN_GLFW // library already initialized in Engine::on_open()
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
    }
#endif

#ifdef DOKUN_OPENGL
	if(Renderer::get_current_API() == "OpenGL") {	// must set the current API before creating a window
		set_context();
        Logger::push(String("Running ") + Renderer::get_current_API() + " version " + Renderer::get_current_API_version()); // can ONLY check context version after creating the context //Logger::push(String("Maximum ") + Renderer::get_shader_language() + " version   " + Renderer::get_shader_language_version());//"3.30.6");
        Logger::push("\n\n");
	}
#endif	
#ifdef DOKUN_VULKAN	
    if(Renderer::get_current_API() == "Vulkan") {	// must set the current API before creating a window
		set_surface();
        Logger::push(String("Running ") + Renderer::get_current_API() + " version " + Renderer::get_current_API_version()); // can ONLY check context version after creating the context
        Logger::push("\n\n");
	}
#endif
}
////////////
void dokun::Window::create(const std::string& title)
{
   this->title = title; // using set_title(title) is a bad idea why?: It will crash unless the window is created first. It is better to change the 'title' value than change the actual window string. The same goes for set_width and set_height
   create();
}
////////////
void dokun::Window::create(int width, int height, int mode)
{
   this->width  = width;
   this->height = height;
   this->mode   = mode;
   create();
}
////////////
void dokun::Window::create(const std::string& title, int width, int height, int mode)
{
   this->title = title;
   this->width  = width;
   this->height = height;
   this->mode   = mode;
   create();	
}
////////////
void dokun::Window::create(int x, int y, int width, int height, int mode)
{
	this->x = x;
	this->y = y;
	this->width  = width;
	this->height = height;
	this->mode = mode;
	create();
}
void dokun::Window::create(const std::string& title, int x, int y, int width, int height, int mode)
{
	this->title = title;
	this->x = x;
	this->y = y;
	this->width  = width;
	this->height = height;
	this->mode = mode;
	create();
}
////////////
int dokun::Window::create(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optstring(L, 2, ""); // title
	luaL_optnumber(L, 3, DOKUN_DEFAULT_WIDTH ); // width
	luaL_optnumber(L, 4, DOKUN_DEFAULT_HEIGHT); // height
	luaL_optnumber(L, 5, 0);           // mode
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));	   
	    window->create(lua_tostring(L, 2), static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)), static_cast<int>(lua_tonumber(L, 5)));		
	}
	return 0;
}
////////////
void dokun::Window::show()
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
#endif
#endif
   
#ifdef DOKUN_X11
    XMapWindow(display, window); // opposite is XLowerWindow(display, window); which sends it to the back of the stack
	XMapRaised(display, window);
#endif

#ifdef DOKUN_GLFW
    std::cout << "GLFW: window_show function not implemented\n";
#endif
}
//////////// 
int dokun::Window::show(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->show();
	}
	return 0;
}
////////////
void dokun::Window::hide()
{	
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
    ShowWindow(handle, SW_HIDE);
#endif
#endif
   
#ifdef DOKUN_X11
    XUnmapWindow(display, window);
#endif

#ifdef DOKUN_GLFW
    
#endif
}
////////////
int dokun::Window::hide(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->hide();
	}
	return 0;	
}
////////////
void dokun::Window::update() // retrieves window messages; updates drawing (called in a loop)
{
#ifdef DOKUN_WIN32
#ifdef DOKUN_OPENGL
	if(Renderer::get_current_API() != "OpenGL") return;
	if(!is_context()) { // must be an OpenGL context
		Logger("Window is not an OpenGL context");
		return;
	}
	SwapBuffers(device);
#endif // endif DOKUN_OPENGL
#endif // endif DOKUN_WIN32

#ifdef DOKUN_X11
#ifdef DOKUN_OPENGL
	if(Renderer::get_current_API() != "OpenGL") return;
	if(!is_context()) {// must be an OpenGL context
		Logger("Window is not an OpenGL context");
		return;
	}
	glXSwapBuffers(display, window);
#endif // endif DOKUN_OPENGL
#endif // endif DOKUN_X11

#ifdef DOKUN_WAYLAND
    wl_display_dispatch_pending (display);
#ifdef DOKUN_OPENGL
    eglSwapBuffers (egl_display, this->egl_surface);
#endif
#endif //endif DOKUN_WAYLAND

#ifdef DOKUN_GLFW
    glfwSwapBuffers(window); /* Swap front and back buffers */
#endif
}
////////////
int dokun::Window::update(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->update();
	}
	return 0;
}
////////////
void dokun::Window::destroy()
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
#ifdef DOKUN_OPENGL
	wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(context);
    ReleaseDC(handle, device);
#endif
	UnregisterClass(static_cast<LPCTSTR>(get_name().c_str()), instance);
    DestroyWindow(handle);
	instance = nullptr;
	Logger("Window destroyed.");
#endif
#endif
    
#ifdef __gnu_linux__
#ifdef DOKUN_X11
#ifndef DOKUN_OPENGL
#ifndef DOKUN_VULKAN
    XFreeGC(display, gc);
#endif    
#endif
#ifdef DOKUN_OPENGL
	glXMakeCurrent(display, None, nullptr);
 	glXDestroyContext(display, context);
#endif
#ifdef DOKUN_VULKAN
#endif
	XDestroyWindow(display, window); // XDestroySubwindows (display, window);
 	XCloseDisplay(display);
	exit(1);
#endif // endif DOKUN_X11

#ifdef DOKUN_XCB
	xcb_disconnect (connection);
#endif

#ifdef DOKUN_WAYLAND
#ifdef DOKUN_OPENGL
    eglDestroySurface (egl_display, this->egl_surface);
	wl_egl_window_destroy (this->egl_window);
	wl_shell_surface_destroy (this->shell_surface);
	wl_surface_destroy (this->surface);
	eglDestroyContext (egl_display, this->egl_context);
	eglTerminate (egl_display);
	wl_display_disconnect (display);
#endif
#endif // endif DOKUN_WAYLAND
#endif // endif __gnu_linux__

#ifdef DOKUN_GLFW
    glfwDestroyWindow(window);
#endif
}
////////////
int dokun::Window::destroy(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->destroy();
	}	
	return 0;
}
////////////
////////////
void dokun::Window::clear(double red, double green, double blue, double alpha, double depth, int stencil) // sets the screen color
{
	color = Vector4(red, green, blue, alpha);
#ifdef DOKUN_OPENGL
	if(Renderer::get_current_API() == "OpenGL")
	{
		if(is_context()) {// must be an OpenGL context
	        glClearColor(static_cast<GLclampf>(color.x) / 255, static_cast<GLclampf>(color.y) / 255, static_cast<GLclampf>(color.z) / 255, static_cast<GLclampf>(color.w)); // alpha can be a decimal 
            glClearDepth(depth); 
		    glClearStencil(stencil);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}
#endif
#ifdef DOKUN_VULKAN
	if(Renderer::get_current_API() == "Vulkan")
	{
        /*
		VkClearValue clear_value;
        clear_value.color.float32[0] = static_cast<float>(red);
        clear_value.color.float32[1] = static_cast<float>(green);
        clear_value.color.float32[2] = static_cast<float>(blue);
        clear_value.color.float32[3] = static_cast<float>(alpha); // 1.0f;
		*/
	}
#endif
}
////////////
int dokun::Window::clear(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optnumber(L, 2, 0); // R
	luaL_optnumber(L, 3, 0); // G
	luaL_optnumber(L, 4, 0); // B
	luaL_optnumber(L, 5, 0); // A
	luaL_optnumber(L, 6, 1); // depth
	luaL_optnumber(L, 7, 0); // stencil
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->clear( (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3), (double)lua_tonumber(L, 4), (double)lua_tonumber(L, 5), (double)lua_tonumber(L, 6), (unsigned int)lua_tonumber(L, 7) );
	}	
	return 0;
}
////////////
void dokun::Window::iconify()
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	ShowWindow(get_handle(), SW_MINIMIZE);
#endif
#endif
	
#ifdef DOKUN_X11
    XIconifyWindow(display, window, DefaultScreen(display));
#endif
}
int dokun::Window::iconify(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->iconify();
	}	
	return 0;
}
void dokun::Window::maximize()
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	ShowWindow(get_handle(), SW_MAXIMIZE);
#endif
#endif
	
#ifdef DOKUN_X11
#endif
}
int dokun::Window::maximize(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->maximize();
	}		
	return 0;
}
void dokun::Window::restore()
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	ShowWindow(get_handle(), SW_RESTORE);
#endif
#endif
	
#ifdef DOKUN_X11
#endif
}
int dokun::Window::restore(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->restore();
	}		
	return 0;
}
////////////
////////////
////////////
void dokun::Window::poll_events() {
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	Renderer::set_display_size(get_client_width (), get_client_height ());
	MSG message;
    if(PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) { // if (GetMessage (&message, nullptr, 0, 0)) // wait for messages(input or mouse movement){
        TranslateMessage(&message);  
        DispatchMessage (&message); 	
    }
#endif
#endif // endif DOKUN_WIN32

#ifdef DOKUN_X11
    //if(XPeekEvent(display, &event)) {
    //     translate_message
    //     dispatch_message
    //}
    // https://stackoverflow.com/questions/41005367/get-x11-window-inside-the-main-loop-for-an-event
    // All events: https://tronche.com/gui/x/xlib/events/structures.html#XEvent
	//XKeyEvent event_send;//XEvent event_send;
    //event_send.type = KeyPress;
    //event_send.state = XK_Escape;
    //XSendEvent(display, window, true, KeyPressMask, (XEvent *)&event_send);
    // find a way to send a fake keyboard press event to keep the window updating constantly ...
    //while(XPending(display) > 0) { //if(XEventsQueued(display, QueuedAlready)) { //{//while(XPending(display)) { //  returns the number of events that have been received from the X server but have not been removed from the event queue
	    XNextEvent(display, &event);//XPeekEvent(display, &event);//if (XEventsQueued(display, QueuedAfterFlush/*QueuedAfterReading*/)) {
	    //XPutBackEvent(display, &event);// wtf!!
	    if(XFilterEvent(&event, window)) {
	        //continue;
	        return;
	    }
	    //////////////////////////////////////////////////////
	    if(event.type == MappingNotify) {
	        XRefreshKeyboardMapping(&event.xmapping);
	    }
	    //////////////////////////////////////////////////////
        //Atom selection = XInternAtom(display, "CLIPBOARD", 0);          
		if(event.type == SelectionRequest) {
		    // configure atom settings (for copy clipboard)
			Atom targets_atom, text_atom, UTF8;
	        targets_atom = XInternAtom(display, "TARGETS", 0);
	        text_atom = XInternAtom(display, "TEXT", 0);
	        UTF8 = XInternAtom(display, "UTF8_STRING", 1);
	        if (UTF8 == None) UTF8 = XA_STRING;//std::cout << "XA_STRING: " << XA_STRING << std::endl;//std::cout << "XA_ATOM: " << XA_ATOM << std::endl;
	        Atom selection = XInternAtom(display, "CLIPBOARD", 0);
	        // the text to be copied along with its size
	        unsigned char * text = reinterpret_cast<unsigned char *>(const_cast<char *>(clipboard_text.c_str())); // removed const from string
	        int size = strlen(clipboard_text.c_str());
			if (event.xselectionrequest.selection != selection) return;//break;
			XSelectionRequestEvent * xsr = &event.xselectionrequest;
			XSelectionEvent ev = {0};
			int R = 0;
			ev.type = SelectionNotify, ev.display = xsr->display, ev.requestor = xsr->requestor,
			ev.selection = xsr->selection, ev.time = xsr->time, ev.target = xsr->target, ev.property = xsr->property;
			if (ev.target == targets_atom) R = XChangeProperty (ev.display, ev.requestor, ev.property, XA_ATOM, 32,
					PropModeReplace, (unsigned char*)&UTF8, 1);
			else if (ev.target == XA_STRING || ev.target == text_atom) 
				R = XChangeProperty(ev.display, ev.requestor, ev.property, XA_STRING, 8, PropModeReplace, text, size);
			else if (ev.target == UTF8)
				R = XChangeProperty(ev.display, ev.requestor, ev.property, UTF8, 8, PropModeReplace, text, size);
			else ev.property = None;
			if ((R & 2) == 0) XSendEvent (display, ev.requestor, 0, 0, (XEvent *)&ev);
			//break;
			//case SelectionClear:
			//return;
		}	
		//////////////////////////////////////////////////////
		// paste event	
        if(event.type == SelectionNotify) {
            int format; unsigned long N, size; char * data;/*, * s = 0;*/
	        Atom target, CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0),
	             UTF8 = XInternAtom(display, "UTF8_STRING", True);
	        //clipboard_text.clear();// clear the clipboard_text first		
		    if(event.xselection.selection != CLIPBOARD) return;//break;
		    if(event.xselection.property) {
			    XGetWindowProperty(event.xselection.display, event.xselection.requestor,
				    event.xselection.property, 0L,(~0L), 0, AnyPropertyType, &target,
				    &format, &size, &N,(unsigned char**)&data);
			    if(target == UTF8 || target == XA_STRING) {
				    /*s*/clipboard_text = strndup(data, size);
				    XFree(data);
			    }
			    XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
		    }
	    }
	//////////////////////////////////////////////////////    		
	if(event.type == Expose) // on window show
	{}
	//////////////////////////////////////////////////////
	if(event.type == ConfigureNotify) // notify if window is resized
	{
		//On Linux the renderable area is the entire window size (updated through viewport), on Windows it is the client size.
        // Whatever you do, don't remove this, as it updates the Renderable area each time the window size changes 
        // To know if the Rendererable area is updated, sprites should be the same size on file***
        // If the image on the file is a different size from the sprite on the renderer screen, then it means you did not update the window client size
        // Also if Sprite::lock does not work it means the window size is not updated
        XConfigureEvent xce = event.xconfigure; // https://tronche.com/gui/x/xlib/events/window-state-change/configure.html#XConfigureEvent
        if (xce.width != Renderer::window_width || xce.height != Renderer::window_height) {
            Renderer::window_width = xce.width;
            Renderer::window_height = xce.height;
#ifdef DOKUN_DEBUG0
            std::cout << "Renderer size updated: " << Vector2(Renderer::window_width, Renderer::window_height) << std::endl; // display size
#endif
            // callback function
            if(callbacks["on_resize"]) callbacks["on_resize"]();
        }
	}
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	if(event.type == DestroyNotify) { // on destroying a window
	    //XDestroyWindowEvent xdwe = event.xdestroywindow; // https://tronche.com/gui/x/xlib/events/window-state-change/destroy.html#XDestroyWindowEvent
	    std::cout << "DestroyNotify: window has been destroyed\n";
	}
///////////////////////////// KEY EVENT - TEMP
/*if(event.type == KeyPress) { // XKeyEvent: KeyPress, KeyReleased
XKeyEvent *key_event;
int count;
int buffer_size = 80;
char buffer[80];
KeySym keysym;//XK_Control_L;// = XLookupKeysym(&event.xkey, 0);*/
// XComposeStatus compose; is not used, though it's in some books
   //count = XLookupString(&event.xkey/*key_event*/,buffer,buffer_size, &keysym, nullptr); // causes SEGFAULT
//if ((keysym >= XK_space) && (keysym <= XK_asciitilde)){
//      printf ("Ascii key:- ");
//      if (event.xkey.state/*key_event->state*/ & ShiftMask)
//             printf("(Shift) %s\n", buffer); // capital letter or symbol
//      else if (event.xkey.state/*key_event->state*/ & LockMask)
//             printf("(Caps Lock) %s\n", buffer); // capital letter
//      else if (event.xkey.state/*key_event->state*/ & ControlMask)
//             printf("(Control) %c\n", 'a'+ buffer[0]-1) ;
//      else printf("%s\n", buffer) ;
//   }
/*
   else if ((keysym >= XK_Shift_L) && (keysym <= XK_Hyper_R)){
      printf ("modifier key:- ");
      switch (keysym){
      case XK_Shift_L: printf("Left Shift\n"); break;
      case XK_Shift_R: printf("Right Shift\n");break;
      case XK_Control_L: printf("Left Control\n");break;
      case XK_Control_R: printf("Right Control\n");	break;
      case XK_Caps_Lock: printf("Caps Lock\n");	break;
      case XK_Shift_Lock: printf("Shift Lock\n");break;
      case XK_Meta_L: printf("Left Meta\n");	break;
      case XK_Meta_R: printf("Right Meta\n");	break;

      }
    }  
   else if ((keysym >= XK_Left) && (keysym <= XK_Down)){
      printf("Arrow Key:-");
      switch(keysym){
      case XK_Left: printf("Left\n");break;
      case XK_Up: printf("Up\n");break;
      case XK_Right: printf("Right\n");break;
      case XK_Down: printf("Down\n");break;	
      }
    }     
}*/ // keypressed
/////////////////////////////
	if(event.type == KeyPress)
    {
        //std::cout << "A key has been pressed\n"; // prints multiple times even when XPending is called
#ifdef DOKUN_DEBUG0
		if(XLookupKeysym(&event.xkey, 0) == XK_Escape)
			destroy();	
#endif
        int buffer_size = 20;//80;
        char buffer[buffer_size];//[80];
        KeySym keysym;// = XLookupKeysym(&event.xkey, 0); // represents key(s) on a keyboard
        int count = XLookupString(&event.xkey, buffer, buffer_size, &keysym, nullptr);
        // store keyboard input - change keysym to buffer ?      
        Keyboard::get_input(keysym, (event.type == KeyPress), (event.type == KeyRelease));//Keyboard::get_input(keysym, true, false);
	    // debug
	    //std::cout << buffer << " was pressed (length: " << strlen(buffer)/* << ")"*/ << ", count: " << count << ")" << std::endl;
	    //std::cout << XLookupKeysym(&event.xkey, 0) << std::endl;
	    // if one wants to find out if another client has changed the key mappings, select MappingNotify and do
	    //XRefreshKeyboardMapping(XMappingEvent* event);
	    ///////////////////////////////////////////////
	    //Keyboard::fake_event(display, window);
        //XFlush(display);
	}
	// Keyboard (release)
	if(event.type == KeyRelease)
    {
        int buffer_size = 20;//80;
        char buffer[buffer_size];//[80];
        KeySym keysym;// = XLookupKeysym(&event.xkey, 0); // represents key(s) on a keyboard
        XLookupString(&event.xkey, buffer, buffer_size, &keysym, nullptr);
        // store keyboard input - change keysym to buffer ?      
        Keyboard::get_input(keysym, false/*pressed*/, true/*release*/);//Keyboard::get_release_event(XLookupKeysym(&event.xkey, 0), (event.type == KeyRelease));
        //std::cout << buffer << " was released (length: " << strlen(buffer) << ")" << std::endl;
    }
    // Mouse (press, release)
	if(event.type == ButtonPress)  {
	    mouse_button = event.xbutton.button;
	    mouse_released = 0;
	    mouse_pressed = 1;
	    //std::cout << "mouse is pressed\n"; // prints multiple times, even when XPending is being called
	}
	if(event.type == ButtonRelease){
	    mouse_button = event.xbutton.button;
	    mouse_pressed = 0;
	    mouse_released = 1;
	}
	//}//while xpending	
#endif	
}
////////////
void dokun::Window::wait_events() {
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	Renderer::set_display_size(get_client_width (), get_client_height ());
	MSG message;
    if(GetMessage(&message, nullptr, 0, 0)) { // wait for messages(input or mouse movement) {
        TranslateMessage(&message);  
        DispatchMessage (&message); 	
    }
#endif
#endif // endif DOKUN_WIN32


}
////////////		
void dokun::Window::copy_clipboard(const char * text) {
#ifdef DOKUN_X11
	// selection owner - will override whatever text you copied from another window with a new string from our dokun window
	Atom selection = XInternAtom(display, "CLIPBOARD", 0);
	XSetSelectionOwner (display, selection, window, 0);
	if (XGetSelectionOwner (display, selection) != window) return;
	clipboard_text = text;
#endif
}
////////////
std::string dokun::Window::paste_clipboard() {
	// (for paste clipboard)
	Atom UTF8 = XInternAtom(display, "UTF8_STRING", True);
	Atom CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0),
	     XSEL_DATA = XInternAtom(display, "XSEL_DATA", 0);
	XConvertSelection(display, CLIPBOARD, UTF8, XSEL_DATA, window, CurrentTime);
	XSync(display, 0);
    return clipboard_text;
}
//////////// 
//////////// 
// SETTERS
////////////
void dokun::Window::set_title(const std::string& title)
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
    SetWindowText(get_handle(), title.c_str());
#endif
#endif
    
#ifdef DOKUN_X11
    XTextProperty window_title_property;
    char * window_title = const_cast<char *>(title.c_str());
    if(XStringListToTextProperty(&window_title, 1, &window_title_property) == 0)
	    Logger("Failure to set window title");
	XSetWMName(display, window, &window_title_property);  // causes seg fault
#endif

#ifdef DOKUN_GLFW
    glfwSetWindowTitle (window, title.c_str());
#endif
    this->title = title;
}
////////////
int dokun::Window::set_title(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_title( lua_tostring(L, 2));
	}
	return 0;
}
////////////
void dokun::Window::set_width(int width)
{    
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if(MoveWindow(get_handle(), get_x(), get_y(), width, get_height(), FALSE) == 0)
	{
		Logger("Failure to set width");
		return;
	}
#endif
#endif
	
#ifdef DOKUN_X11
	XResizeWindow(display, window, width, get_height());                   //XMoveResizeWindow(display, window, get_x(), get_y(), width, get_height());  // causes seg fault
#endif

#ifdef DOKUN_GLFW
    glfwSetWindowSize (window, width, get_height());
#endif
    this->width = width;
}
////////////
int dokun::Window::set_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_width(static_cast<int>(lua_tonumber(L, 2)));
	}	
	return 0;
}
////////////
void dokun::Window::set_height(int height)
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if(MoveWindow(get_handle(), get_x(), get_y(), get_width(), height, false) == 0)
	{
		Logger("Failure to set height");
		return;
	}	
#endif	
#endif
	
#ifdef DOKUN_X11
	XResizeWindow(display, window, get_width(), height);         // OR // XMoveResizeWindow(display, window, get_x(), get_y(), get_width(), height);
#endif

#ifdef DOKUN_GLFW
    glfwSetWindowSize (window, get_width(), height);
#endif
    this->height    = height;
}
////////////
int dokun::Window::set_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_height( (unsigned int)lua_tonumber(L, 2) );
	}	
	return 0;
}
////////////
void dokun::Window::set_size(int width, int height)
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GFLW
	if(MoveWindow(handle, get_x(), get_y(), width, height, false) == 0)
		return;
#endif
#endif

#ifdef DOKUN_X11
	XResizeWindow(display, window, width, height);         // OR // XMoveResizeWindow(display, window, get_x(), get_y(), get_width(), height);
#endif

#ifdef DOKUN_GLFW
    glfwSetWindowSize (window, width, height);
#endif
}
////////////
int dokun::Window::set_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_size( (unsigned int)lua_tonumber(L, 2), (unsigned int)lua_tonumber(L, 3));
	}	
	return 0;
}
//////////// XSetWindowBorderWidth (display, w, width)
////////////
void dokun::Window::set_mode(int mode)
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
    switch(mode) {
	    case 1: 
		{   
			SetWindowLong(handle, GWL_STYLE, 0); 
	        SetWindowPos (handle, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWdokun::Window);
	    } break;
        case 0: 
		{
		    SetWindowLong(handle, GWL_STYLE, style); 
			set_width (width);
			set_height(height);
			set_position(DOKUN_DEFAULT_POSITION_X, DOKUN_DEFAULT_POSITION_Y);	
            SetWindowPos(handle, HWND_TOP, x, y, width, height, SWP_NOMOVE | SWP_FRAMECHANGED | SWP_SHOWdokun::Window);	// show window, ignore new position, set original size
	    } break;		
	}	
#endif	
#endif
	
#ifdef DOKUN_X11
    Atom    atom_list[2] = { XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False), None};
    switch(mode) {
		case 0:
		    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_STATE_REMOVE", False), XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char *>(atom_list), 1);		
		    break;
		case 1:
		    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_STATE", False), XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char *>(atom_list), 1);
		    break;
	}
#endif
    this->mode = mode;
}
////////////
int dokun::Window::set_mode(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_mode((int)lua_tonumber(L, 2));
	}		
	return 0;
}
////////////
void dokun::Window::set_position(int x, int y)
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	MoveWindow(handle, x, y, get_width(), get_height(), false);
#endif	
#endif
	
#ifdef DOKUN_X11
	XMoveWindow(display, window, x, y);
#endif

#ifdef DOKUN_GLFW
    glfwSetWindowPos (window, x, y);
#endif
	this->x = x;
	this->y = y;
}
////////////
int dokun::Window::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_position((unsigned int)lua_tonumber(L, 2), (unsigned int)lua_tonumber(L, 3));
	}			
	return 0;
}
////////////
void dokun::Window::set_style(long style)
{
#ifdef DOKUN_X11
	switch(style)
	{
		case 0:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_DESKTOP", False);
		break;	
		case 1:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_DOCK", False);
		break;	
		case 2:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_TOOLBAR", False);
		break;	
		case 3:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_MENU", False);
		break;	
		case 4:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_UTILITY", False);
		break;	
		case 5:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_SPLASH", False);
		break;	
		case 6:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_DIALOG", False);
		break;	
		case 7:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_DROPDOWN_MENU", False);
		break;	
		case 8:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_POPUP_MENU", False);
		break;	
		case 9:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_TOOLTIP", False);
		break;	
		case 10:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_NOTIFICATION", False);
		break;	
		case 11:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_COMBO", False);
		break;	
		case 12:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_DND", False);
		break;	
		case 13:
		    style = XInternAtom(display, "_NET_WM_dokun::Window_TYPE_NORMAL", False);
		break;
        default:
            break;		
	}
    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_dokun::Window_TYPE", False),
        XA_ATOM, 32, PropModeReplace, (unsigned char *)&style, 1);
    this->style = style;
#endif   
}
////////////
int dokun::Window::set_style(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_style((int)lua_tonumber(L, 2));
	}
    return 0;	
}
////////////
void dokun::Window::set_icon(const std::string& file_name) // from .ICO
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
    HANDLE icon = LoadImage(instance, file_name.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	SendMessage(get_handle(), (UINT)WM_SETICON, ICON_BIG, (LPARAM)icon);
#endif
#endif

#ifdef DOKUN_X11
/*

// pointer to the WM hints structure. 
XWMHints* win_hints;

// load the given bitmap data and create an X pixmap containing it.
Pixmap icon_pixmap = XCreateBitmapFromData(display,
                                           win,
                                           icon_bitmap_bits,
                                           icon_bitmap_width,
                                           icon_bitmap_height);
if (!icon_pixmap) {
    fprintf(stderr, "XCreateBitmapFromData - error creating pixmap\n");
    exit(1);
}

// allocate a WM hints structure.
win_hints = XAllocWMHints();
if (!win_hints) {
    fprintf(stderr, "XAllocWMHints - out of memory\n");
    exit(1);
}

// initialize the structure appropriately. 
// first, specify which size hints we want to fill in. 
// in our case - setting the icon's pixmap. 
win_hints->flags = IconPixmapHint;
// next, specify the desired hints data.           
// in our case - supply the icon's desired pixmap. 
win_hints->icon_pixmap = icon_pixmap;

// pass the hints to the window manager. 
XSetWMHints(display, window, win_hints);

// finally, we can free the WM hints structure.
XFree(win_hints);
*/
#endif
#ifdef DOKUN_GLFW
    //glfwSetWindowIcon (GLFWwindow *window, int count, const GLFWimage *images)
#endif
}
////////////    
int dokun::Window::set_icon(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_icon(lua_tostring(L, 2));
	}		
	return 0;
} 
////////////
void dokun::Window::set_cursor(int cursor) // DWORD = unsigned long
{
	switch(cursor)
	{
		case DOKUN_ARROW: // arrow (default)
		{
			#ifdef DOKUN_WIN32
		        HCURSOR arrow = LoadCursor(nullptr, IDC_ARROW);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)arrow); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_START:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR start = LoadCursor(nullptr, IDC_APPSTARTING);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)start); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_CROSS:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR cross = LoadCursor(nullptr, IDC_CROSS);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)cross); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif				
		    break;
		}		
		case DOKUN_HAND:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR hand = LoadCursor(nullptr, IDC_HAND);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)hand); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_HELP:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR help = LoadCursor(nullptr, IDC_HELP);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)help); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_BEAM:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR lbeam = LoadCursor(nullptr, IDC_IBEAM);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)lbeam); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_ICON:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR icon = LoadCursor(nullptr, IDC_ICON);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)icon); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif
		    break;
		}
		case DOKUN_NON:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR no = LoadCursor(nullptr, IDC_NO);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)no); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif
		    break;		
		}
		case DOKUN_MOVE:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR size_all = LoadCursor(nullptr, IDC_SIZEALL);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)size_all); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif				
		    break;	
		}			
		case DOKUN_SIZENESW:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR size_nesw = LoadCursor(nullptr, IDC_SIZENESW);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)size_nesw); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_SIZENS:
		{
			#ifdef DOKUN_WIN32
			#ifndef DOKUN_SDL2
            #ifndef DOKUN_GLFW
		        HCURSOR size_ns = LoadCursor(nullptr, IDC_SIZENS);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)size_ns); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
			#endif	
            #endif
            #endif
		    break;	
		}
		case DOKUN_SIZENWSE:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR size_nwse = LoadCursor(nullptr, IDC_SIZENWSE);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)size_nwse); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif
		    break;	
		}			
		case DOKUN_SIZEWE:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR size_we = LoadCursor(nullptr, IDC_SIZEWE);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)size_we); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif				
		    break;	
		}
		case DOKUN_UPARROW:
		{
			#ifdef DOKUN_WIN32
		        HCURSOR up_arrow = LoadCursor(nullptr, IDC_UPARROW);
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)up_arrow); // HCURSOR SetCursor(HCURSOR hCursor);
				this->cursor = cursor;
            #endif	
		    break;
		}
		case DOKUN_WAIT: // hourglass
		{
			#ifdef DOKUN_WIN32
		        HCURSOR wait = LoadCursor(nullptr, IDC_WAIT); 
		        //SetClassLong(get_handle(), GCL_HCURSOR, (DWORD)wait); // HCURSOR SetCursor(HCURSOR hCursor);
                this->cursor = cursor;			
			#endif		
		    break;
		}	
	}
	#ifdef DOKUN_X11
	    this->cursor = XCreateFontCursor(display, XC_xterm); 
        XDefineCursor(display, window, cursor);
    #endif
}
////////////
void dokun::Window::set_cursor(std::string file_name, int width, int height) // from a .CUR or .ANI file
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	    if(!width && !height)
	    {
		    HCURSOR cursor = LoadCursorFromFile(file_name.c_str());  // uses standard cursor size
            if (!cursor)
	 	    {
			    std::cerr <<"Could not open cursor " << file_name <<  std::endl;
		    }
			SetCursor(cursor);
	    }
	    else
	    {
	        HCURSOR cursor = (HCURSOR)LoadImage
			    (nullptr, file_name.c_str(), IMAGE_CURSOR, width, height, LR_LOADFROMFILE); // uses custom cursor size
		    if (!cursor)
		    {
			    std::cerr <<"Could not open cursor " << file_name << std::endl;
		    }
			SetCursor(cursor);
	    }
#endif
#endif
	
#ifdef DOKUN_X11
	    //Pixmap pixmap = XCreatePixmap(display, Drawable d, unsignedintwidth, height, unsignedint depth);
	    //cursor = XCreatePixmapCursor(display, Pixmap source, Pixmap mask, XColor *foreground_color, XColor *background_color, unsignedintx, y);
		//XDefineCursor(display, window, cursor); // XUndefineCursor (display, w)   // resets the cursor
#endif
} 
////////////
int dokun::Window::set_cursor(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or number
	luaL_optnumber(L, 3, 0); // opt_width
	luaL_optnumber(L, 4, 0); // opt_height
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TNUMBER)
		{
			window->set_cursor((int)lua_tonumber(L, 2));
		}
		else if(lua_type(L, 2) == LUA_TSTRING)
		{
			window->set_cursor(lua_tostring(L, 2), (int)lua_tonumber(L, 3), (int)lua_tonumber(L, 4));
		}
	}		
	return 0;
}
////////////
void dokun::Window::set_callback(const std::string& callback_name, std::function<void(void)> callback_function) {
    if(String::contains(String::lower(callback_name), "resize")) {
        callbacks["on_resize"] = callback_function;
    }
}
////////////
// XSetWindowBorderWidth (display, w, width)
//////////// 
void dokun::Window::set_vertical_synchronization(bool v_sync)
{
#ifdef DOKUN_OPENGL
	if(!is_context())
	    return;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	wglSwapIntervalEXT(v_sync);
#endif
#endif

#ifdef DOKUN_X11
	glXSwapIntervalEXT(display, window, v_sync); // 
#endif
#endif
}
////////////
int dokun::Window::set_vertical_synchronization(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_vertical_synchronization((lua_toboolean(L, 2) != 0));
	}		
	return 0;
}
////////////
void dokun::Window::set_viewport(int width, int height) // make sure the window's title bar is not included in the viewport by using the Window's client_width and client_height (or else the title bar will overlap[hide] the object)(applies to Win32 NOT Linux)
{
#ifdef DOKUN_OPENGL
    if(Renderer::get_current_API() == "OpenGL") {
        if(height == 0) height = 1;
        glViewport(0, 0, static_cast<GLuint>(width), static_cast<GLuint>(height));
    }
#endif
#ifdef DOKUN_VULKAN
    if(Renderer::get_current_API() == "Vulkan") {
	    VkViewport viewport; // up to 10 viewports (dokun_viewport_limit = 10)
	    memset(&viewport, 0, sizeof(viewport)); // fill viewport with 0
	    viewport.x        =	0;
	    viewport.y        =	0;
	    viewport.width    = static_cast<float>(width);
	    viewport.height   = static_cast<float>(height);
	    viewport.minDepth = static_cast<float>(0.0);
	    viewport.maxDepth = static_cast<float>(1.0);
	    //vkCmdSetViewport(cmd_buf, 0, 1, &viewport);
	}
#endif
}
////////////
int dokun::Window::set_viewport(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		window->set_viewport((int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
	}		
	return 0;	
}
////////////
/*
void dokun::Window::set_event(Event * event)
{
	(this)->event = event;
}
////////////
////////////
int dokun::Window::set_event(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // event obj or number
    if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Event * event = *(Event **)lua_touserdata(L, -1);
		    lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
		        dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		        window->set_event(event);
		    }
	    }
	}
	return 0;
}
*/
////////////
void dokun::Window::set_context()
{
#ifdef DOKUN_OPENGL
	if(Renderer::get_current_API() != "OpenGL")
	    return;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if(wglGetCurrentContext()) // context already exists, delete it
    {
	    wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(context);
        ReleaseDC(handle, device);
	}
    PIXELFORMATDESCRIPTOR pfd;
	device = GetWindowDC(handle);//GetDC(handle);
	memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_dokun::Window | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
 	pfd.cColorBits   = 32; 
	pfd.cDepthBits   = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType   = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat(device, &pfd); // number of supported formats
	if (format == 0) 
	{
		Logger("ChoosePixelFormat() failed:  " "Cannot find a suitable pixel format.");
		return;
	}
	if (!SetPixelFormat(device, format, &pfd))
	{
		Logger("SetPixelFormat() failed:  " "Cannot set format specified.");
		return;
	}
	DescribePixelFormat(device, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	context = wglCreateContext(device);
    wglMakeCurrent(device, context);		
#endif
#endif

#ifdef __gnu_linux__
#ifdef DOKUN_X11    
	if(glXGetCurrentContext()) // if context already exists, delete it
	{
		glXMakeCurrent(display, None, nullptr);
 		glXDestroyContext(display, context);
	}
	// check if glXCreateContextAttribsARB function exists (typedef of function does not work so I had to get the Proc address of the function from the runtime library)      
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;//glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB =  (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
	if (glXCreateContextAttribsARB == nullptr)
    {
        std::cout << "glXCreateContextAttribsARB entry point not found. Aborting." << std::endl;
        exit(1);
    }  
    int context_attributes[] = { // minimum required context version for dokun is 3.30
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };//*/
	// Create context
	context = glXCreateContextAttribsARB(display, *fbc, nullptr, true, context_attributes); //context = glXCreateContext(display, visual_info, nullptr, GL_TRUE);
    if (!context)
    {
        Logger("GL3 context creation failed.");
        exit(1);
    }
    glXMakeCurrent(display, window, context); // set context       
#endif // DOKUN_X11
#ifdef DOKUN_WAYLAND
    eglMakeCurrent (egl_display, this->egl_surface, this->egl_surface, this->egl_context);
#endif // DOKUN_WAYLAND
#endif // __gnu_linux__

#ifdef DOKUN_GLFW
    glfwMakeContextCurrent(window);    /* Make the window's context current */
#endif
	glewExperimental = GL_TRUE;
    GLenum error  = glewInit();
    if (GLEW_OK != error)
	{
		Logger("Could not start GLEW ");
		std::cerr << glewGetErrorString(error) << std::endl;
		exit(1);
    }
#ifdef DOKUN_PRINT_ALL_LIB_VERSIONS    
    std::string version = reinterpret_cast<char *>(const_cast<unsigned char*>(glewGetString(GLEW_VERSION)));
    Logger::push(String("Running GLEW") + " version   " + version);
#endif    
    /*
	if(!Program::get_default())
	{
		Program * program = new Program();
		program->create();
		program->set_default();
	}*/
#endif
}
int dokun::Window::set_context(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
	    window->set_context ();
	}	
	return 0;
}
/////////////
void dokun::Window::set_surface() // set window as vulkan surface
{
#ifdef DOKUN_VULKAN
	if(Renderer::get_current_API() != "Vulkan")
	    return;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	VkWin32SurfaceCreateInfoKHR surface_info = {};
	surface_info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_info.pNext     = nullptr;
    surface_info.flags     = 0;
    surface_info.hinstance = instance;
    surface_info.hwnd      = handle;
			
	if(vkCreateWin32SurfaceKHR (Renderer::get_pointer()->instance, & surface_info, nullptr, & surface) != VK_SUCCESS)
		Logger("Could not create Vulkan surface");
#endif
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
	VkXlibSurfaceCreateInfoKHR surface_info = {};
	surface_info.sType     = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surface_info.pNext     = nullptr;
    surface_info.flags     = 0;
    surface_info.dpy       = display;
    surface_info.window    = window;
	if (vkCreateXlibSurfaceKHR (Renderer::get_pointer()->instance, & surface_info, nullptr, & surface) != VK_SUCCESS)
		Logger("Could not create Vulkan surface");	
#endif
#ifdef DOKUN_WAYLAND
    VkWaylandSurfaceCreateInfoKHR surface_info = {};
	surface_info.sType     = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surface_info.pNext     = nullptr;
    surface_info.flags     = 0;	
	surface_info.display   = display;
	surface_info.surface   = window;
	
	if (vkCreateWaylandSurfaceKHR(Renderer::get_default()->instance, & surface_info, nullptr, & surface) != VK_SUCCESS)
		Logger("Could not create Vulkan surface");
#endif
#ifdef DOKUN_MIR
	VkMirSurfaceCreateInfoKHR surface_info = {};
	surface_info.sType     = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
    surface_info.pNext     = nullptr;
    surface_info.flags     = 0;
	surface_info.connection = connection;
	surface_info.mirSurface = mirSurface;
		
	if (vkCreateMirSurfaceKHR(Renderer::get_default()->instance, & surface_info, nullptr, & surface) != VK_SUCCESS)
		Logger("Could not create Vulkan surface");
#endif
#endif // endif __gnu_linux__
#ifdef DOKUN_ANDROID
	VkAndroidSurfaceCreateInfoKHR surface_info = {};
	surface_info.sType     = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surface_info.pNext     = nullptr;
    surface_info.flags     = 0;	
	surface_info.window    = window; // must not be in a connected state
		
	if (vkCreateAndroidSurfaceKHR(Renderer::get_default()->instance, & surface_info, nullptr, & surface) != VK_SUCCESS)
		Logger("Could not create Vulkan surface");
#endif
#endif
}
int dokun::Window::set_surface(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
	    window->set_surface ();
	}	
	return 0;	
}
////////////
//////////// 
// GETTERS
////////////
std::string dokun::Window::get_name()
{
	return "Window" + std::to_string(get_id());
}
////////////
int dokun::Window::get_name(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushstring(L, window->get_name().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////
int dokun::Window::get_id()
{
	return Factory::get_window_factory()->get_location(this);
}
////////////                             
int dokun::Window::get_id(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushnumber(L, (int)window->get_id());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////
std::string dokun::Window::get_title()const
{
	return title;
}
////////////
int dokun::Window::get_title(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushstring(L, window->get_title().c_str()); 
		return 1;		
	}
	lua_pushnil(L);
	return 1;
}
////////////
int dokun::Window::get_width()const
{
	int width = 0;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
		return -1;
	RECT size;
	GetWindowRect(get_handle(), & size); //-- WindowRect includes titlebar and ClientRect excludes titlebar
	width = (size.right - size.left);
#endif
#endif
	
#ifdef DOKUN_X11
    XWindowAttributes gwa;
	XGetWindowAttributes(display, window, &gwa); // get attributes while app is running; only gets width and height no x and y
	width = gwa.width;
#endif

#ifdef DOKUN_GLFW
    glfwGetWindowSize(window, &width, nullptr);
#endif
	
#ifdef DOKUN_ANDROID
	return ANativeWindow_getWidth	(window);
#endif
    return width;
}
////////////
int dokun::Window::get_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, (lua_Number)window->get_width());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}	
////////////
int dokun::Window::get_height()const // returns the height of the window, includes the title bar's height
{
	int height = 0;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
	    return -1;
    RECT size;
    GetWindowRect(get_handle(), & size); //-- WindowRect includes titlebar and ClientRect excludes titlebar
    height = (size.bottom - size.top);
#endif
#endif
	
#ifdef DOKUN_X11
    XWindowAttributes gwa;
	XGetWindowAttributes(display, window, &gwa); // get attributes while app is running; only gets width and height no x and y
	height = gwa.height; 
#endif

#ifdef DOKUN_GLFW
    glfwGetWindowSize(window, nullptr, &height);
#endif

#ifdef DOKUN_ANDROID
	return ANativeWindow_getHeight	(window);	
#endif
    return height;
}
////////////
int dokun::Window::get_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_height());
		return 1;		
	}	
	lua_pushnil(L);
	return 1;
}
////////////
Vector2 dokun::Window::get_size()const // returns size of window in pixels, includes the title bar (if using win32)
{
	return Vector2(get_width(), get_height());
}
////////////
int dokun::Window::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_size().x);
		lua_pushnumber(L, window->get_size().y);
		return 2;        
	}
	lua_pushnil(L);
	return 1;
}
////////////
int dokun::Window::get_client_width()const
{
	int width = 0;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
		return -1;
	RECT size;
	GetClientRect(handle, &size);//GetWindowRect(get_handle(), & size); - WindowRect includes titlebar and ClientRect excludes titlebar
	width = (size.right - size.left);
#endif
#endif

#ifdef DOKUN_X11
    XWindowAttributes gwa;
	XGetWindowAttributes(display, window, &gwa); // get attributes while app is running; only gets width and height no x and y
	width = gwa.width;
#endif

#ifdef DOKUN_GLFW
    glfwGetFramebufferSize(window, &width, nullptr);
#endif

#ifdef DOKUN_ANDROID
	return ANativeWindow_getWidth	(window);
#endif
    return width;
}
////////////
int dokun::Window::get_client_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, (lua_Number)window->get_client_width());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}	
////////////
int dokun::Window::get_client_height()const // returns the height of the window, includes the title bar's height
{
	int height = 0;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
	    return -1;
    RECT size;
    GetClientRect(handle, &size);//GetWindowRect(get_handle(), & size); - WindowRect includes titlebar and ClientRect excludes titlebar
    height = (size.bottom - size.top);
#endif
#endif

#ifdef DOKUN_X11
    XWindowAttributes gwa;
	XGetWindowAttributes(display, window, &gwa); // get attributes while app is running; only gets width and height no x and y
	height = gwa.height; 
#endif

#ifdef DOKUN_GLFW
    glfwGetFramebufferSize(window, nullptr, &height);
#endif

#ifdef DOKUN_ANDROID
	return ANativeWindow_getHeight	(window);	
#endif
    return height;
}
////////////
int dokun::Window::get_client_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_client_height());
		return 1;		
	}	
	lua_pushnil(L);
	return 1;
}
////////////
Vector2 dokun::Window::get_client_size()const // returns size of window in pixels, excludes the title bar (if using win32)
{
	return Vector2(get_client_width(), get_client_height());
}
////////////
int dokun::Window::get_client_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_client_size().x);
		lua_pushnumber(L, window->get_client_size().y);
		return 2;        
	}
	lua_pushnil(L);
	return 1;
}

////////////
////////////
int dokun::Window::get_mode()const
{
    return mode;
}
////////////
int dokun::Window::get_mode(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_mode() );
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
////////////
int dokun::Window::get_x()const
{
	int x = 0;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
	    return -1;	
	RECT rect;
    GetWindowRect(get_handle(), & rect);
	x = rect.left;
#endif
#endif
	
#ifdef DOKUN_X11
    XWindowAttributes xwa;
	int y;
	::Window child;
	XTranslateCoordinates(display, window, DefaultRootWindow(display), 0, 0, &x, &y, &child );
    XGetWindowAttributes(display, window, &xwa);
    x = x - xwa.x;
#endif
	return x;
}
////////////
int dokun::Window::get_x(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_x());
		return 1;
	}			
	lua_pushnil(L);
	return 1;
}
////////////
int dokun::Window::get_y()const
{
	int y = 0;
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
	    return -1;
	RECT rect;
    GetWindowRect(get_handle(), & rect);
	y = rect.top;
#endif
#endif

#ifdef DOKUN_X11
    XWindowAttributes xwa;
    int x;
	::Window child;
	XTranslateCoordinates(display, window, DefaultRootWindow(display), 0, 0, &x, &y, &child );
    XGetWindowAttributes(display, window, &xwa);
	y = y - xwa.y;
#endif
	return y;
}
////////////
int dokun::Window::get_y(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_y());
		return 1;
	}		
    lua_pushnil(L);
	return 1;	
}
////////////
Vector2 dokun::Window::get_position()const
{
	int x = 0;
    int	y = 0; 
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	if (!is_open())
	    return -1;
	RECT rect;
    GetWindowRect(get_handle(), & rect);
	x = rect.left;
 	y = rect.top;
#endif
#endif
	
#ifdef DOKUN_X11
    XWindowAttributes xwa;
	::Window child;
	XTranslateCoordinates(display, window, DefaultRootWindow(display), 0, 0, &x, &y, &child );
    XGetWindowAttributes(display, window, &xwa);
    x = x - xwa.x;
	y = y - xwa.y;
#endif
	return Vector2(x, y);
}
////////////
int dokun::Window::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
        lua_pushnumber(L, window->get_position().x);
		lua_pushnumber(L, window->get_position().y);
		return 2;        
	}
	lua_pushnil(L);
	return 1;
}
////////////
// get style
long dokun::Window::get_style()const
{
#ifdef DOKUN_X11
	return style;
#endif
    return 0;
}
////////////
int dokun::Window::get_style(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window =  *static_cast<dokun::Window**>(lua_touserdata(L, -1));
		lua_pushnumber(L, window->get_style());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////
// get icon
////////////
// get cursor
////////////
dokun::Window * dokun::Window::get_active() // returns window with current focus
{
    for(auto windows : Factory::get_window_factory()->get_storage())
	{
		dokun::Window * window = static_cast<dokun::Window *>(windows);
		if(window->is_focused()) { // only one window can have focus at a time
			return window;
		}
	}
	return nullptr;
}
////////////
int dokun::Window::get_active(lua_State *L)
{
    return 0;
}
////////////
#ifdef DOKUN_X11
::Display * dokun::Window::get_display()const {
	return display;
}
////////////
int dokun::Window::get_display(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushlightuserdata (L, static_cast<void *>(window->get_display()));
		return 1;
	}
	lua_pushnil(L);
	return 1;
}	
#endif
////////////
////////////
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
HINSTANCE dokun::Window::get_instance()const {
	return instance;
}
#endif
#endif
////////////
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
HWND dokun::Window::get_handle()const {
    return handle;
}
#endif
#endif
////////////
#ifdef DOKUN_X11
::Window dokun::Window::get_handle()const {
	return window;
}
::Window dokun::Window::get_window()const {
	return window;
}
#endif
////////////
#ifdef DOKUN_GLFW
GLFWwindow* dokun::Window::get_handle() const {
    return window; // GLFWwindow* window; (window.hpp:164)
}
#endif
////////////
////////////
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
HGLRC dokun::Window::get_context()const {
    return context;
}
#endif
#endif
////////////
#ifdef DOKUN_X11
#ifdef DOKUN_OPENGL
GLXContext dokun::Window::get_context()const {
	return context;
}	
#endif
#endif
////////////
#ifdef DOKUN_X11
XEvent dokun::Window::get_xevent() {//const {
    return event;
}
#endif
////////////
////////////
// BOOLEAN
////////////
bool dokun::Window::is_open()const
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	return (IsWindow(get_handle()) != 0);
#endif
#endif
	
#ifdef DOKUN_X11
	return (display != 0);
#endif

#ifdef DOKUN_GLFW
    return !glfwWindowShouldClose(window);
#endif
    return false;
}
////////////
int dokun::Window::is_open(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_open() );
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
////////////
bool dokun::Window::is_visible()const
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	return (IsWindowVisible(get_handle()) != 0);
#endif
#endif
	
#ifdef DOKUN_X11
#endif

#ifdef DOKUN_GLFW
    
#endif
	return false;
}
////////////
int dokun::Window::is_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_visible() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;
}
////////////
bool dokun::Window::is_focused()const
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	return (get_handle() == GetFocus());
#endif
#endif
	
#ifdef DOKUN_X11
	::Window focus;
    int revert_to;
    XGetInputFocus(display, &focus, &revert_to);
    return (get_handle() == focus);	
#endif

#ifdef DOKUN_GLFW
    
#endif
	return false;
}
////////////
int dokun::Window::is_focused(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_focused() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;
}
////////////
bool dokun::Window::is_iconified()const
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	return (IsIconic(get_handle()) != 0);
#endif
#endif
	
#ifdef DOKUN_X11
#endif

#ifdef DOKUN_GLFW
    
#endif
	return false;
}
////////////
int dokun::Window::is_iconified(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_iconified() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
////////////
bool dokun::Window::is_maximized()const
{
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	return (IsZoomed(get_handle()) != 0);
#endif
#endif
	
#ifdef DOKUN_X11
#endif

#ifdef DOKUN_GLFW
    
#endif
	return false;	
}
int dokun::Window::is_maximized(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_maximized());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
////////////
bool dokun::Window::is_window()const
{
	return is_open();
}
////////////
int dokun::Window::is_window(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window * window = *static_cast<dokun::Window **>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_window());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
////////////
bool dokun::Window::is_context()const
{
#ifdef DOKUN_OPENGL

#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
	return (wglGetCurrentContext() == context);
#endif
#endif

#ifdef DOKUN_X11
	return (glXGetCurrentContext() == context);
#endif

#ifdef DOKUN_GLFW
    
#endif
#endif // endif DOKUN_OPENGL
	return false;
}
////////////
int dokun::Window::is_context(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Window* window = *static_cast<dokun::Window**>(lua_touserdata(L, -1));
		lua_pushboolean(L, window->is_context());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
////////////
////////////
////////////
#ifdef DOKUN_WIN32
#ifndef DOKUN_GLFW
HHOOK dokun::Window::hMouseHook;
LRESULT CALLBACK dokun::Window::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
    if (pMouseStruct != nullptr)
	{	
        if(wParam == WM_LBUTTONDOWN)
        {
            //printf( "clicked" ); 
        }
	}
	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}
/////////////
    LRESULT CALLBACK dokun::Window::WndProc(HWND handle, UINT mes, WPARAM wParam, LPARAM lParam) 
    {   
	    HDC device;
	    PAINTSTRUCT ps;
		
		switch(mes)
	    {
            case WM_NCCREATE:
            {
				
                //CREATESTRUCT* cs = static_cast<CREATESTRUCT*>(lParam);
			    //SetWindowLong(handle, 0, (cs->lpCreateParams)); // LPVOID
                break;
            }
			case WM_PAINT: // GDI
			{
				device = BeginPaint(handle, &ps);
				#ifndef use_directx
				#ifndef DOKUN_OPENGL
				#ifndef DOKUN_VULKAN
				#ifndef use_custom
                    FillRect(device, &ps.rcPaint, (HBRUSH) (CreateSolidBrush(RGB(0,0,0)))); // clear screen to black
				#endif
				#endif
				#endif
				#endif
				EndPaint(handle, &ps);
				break;
			}
            case WM_CLOSE: // on pressing the 'X' button
            {
				/*
				if (MessageBox(handle, "Really quit?", "My application", MB_OKCANCEL) == IDOK)
                {
                    DestroyWindow(handle);
                }
                // Else: User canceled. Do nothing.
                return 0;
				*/
		        DestroyWindow(handle);
		        break;
		    }
            case WM_DESTROY: // on window destroy	
            {		
			    PostQuitMessage(0);
			    break;
            }		
			case WM_SIZE: // on resize
			{
			#ifdef DOKUN_OPENGL
			    /*RECT size;
	            GetWindowRect(GetActiveWindow(), & size);
	            int window_width  = (size.right - size.left);
                int window_height = (size.bottom - size.top);
				*/
				//glViewport(0,0,window_width,window_height);
                break;				
            #endif
			}			
		// Mouse
			case WM_MOUSEMOVE:
			{
				mouse_moved = true;
                break;
		    }
		    case WM_LBUTTONDOWN: // left mouse
			    mouse_button   = 1;
				mouse_released = false;
			    mouse_pressed  = true;
				mouse_dragged  = true;
				SetCapture(GetActiveWindow());
		        break;
		    case WM_MBUTTONDOWN: // middle mouse
			    mouse_button   = 2;
				mouse_released = false;
			    mouse_pressed  = true;			
		        break;
		    case WM_RBUTTONDOWN: // right mouse
			    mouse_button   = 3;
				mouse_released = false;
			    mouse_pressed  = true;			
		        break;
			// released	
		    case WM_LBUTTONUP: // left mouse
			    mouse_button   = 1;
				mouse_pressed  = false;
				mouse_released = true;
				ReleaseCapture();
				mouse_dragged = false;
		        break;
		    case WM_MBUTTONUP: // middle mouse
			    mouse_button   = 2;
				mouse_pressed  = false;
				mouse_released = true;				
		        break;
		    case WM_RBUTTONUP: // right mouse
			    mouse_button   = 3;
				mouse_pressed  = false;
				mouse_released = true;			
		        break;
		    // double clicks
		    case WM_NCLBUTTONDBLCLK: //WM_LBUTTONDBLCLK: 
		        break;
		    case WM_MBUTTONDBLCLK: 
		        break;
		    case WM_RBUTTONDBLCLK: 
		        break;
		   // mouse leave
		    case WM_MOUSELEAVE:
			    mouse_leave = true;
		        break;
		  // mouse wheel
		    case WM_MOUSEWHEEL:
				//std::cout << GET_KEYSTATE_WPARAM(wParam) << std::endl; // key_state - 0 if scrolling(but not holding onto any mouse button), 1 if scrolling and holding on to a left mouse button, 2 if scrolling and holding on to a right mouse button, 3 if scrolling and holding on to both mouse buttons 
				//mouse_delta = GET_WHEEL_DELTA_WPARAM(wParam); // delta -120(down), 120(up)	       
				//std::cout << GET_X_LPARAM(lParam) << " " << GET_Y_LPARAM(lParam) << std::endl; // hort and vert position of scroll
				break;
		    case WM_MOUSEHOVER:
			    break;
		    case WM_KEYDOWN :
		        Keyboard::down = true;
		        Keyboard::up = false;
		        break;
            case WM_KEYUP:
                Keyboard::up = true;
                Keyboard::down = false;
		        break;	
            default:
			{
                break;				
			}
	    }
	    switch(wParam)
  	    {
		    case 27: // ESC (temporary)
#ifdef DOKUN_DEBUG
				DestroyWindow(handle);
				PostQuitMessage(0);
#endif
			    break;
		}
		return DefWindowProc(handle, mes, wParam, lParam);
	}
#endif
#endif
////////////
int dokun::Window::mouse_button  (0);
int dokun::Window::mouse_pressed (0);
int dokun::Window::mouse_released(0);
int dokun::Window::mouse_dragged (0);
int dokun::Window::mouse_moved   (0);
int dokun::Window::mouse_leave   (0);
//int dokun::Window::mouse_   (0);
////////////
#ifdef DOKUN_GLFW
void dokun::Window::glfw_error_callback(int error, const char* description) {
    std::cout << "\033[1;90m[GLFW]: " << description << " (" << error << ")\033[0m" << std::endl;//fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
void dokun::Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { // if ESC is pressed
        glfwSetWindowShouldClose(window, GLFW_TRUE); // close the window
    }
}
#endif
////////////
int dokun::Window::window_new(lua_State *L)
{
	lua_settop(L, 0); // clear stack
	// create table
	lua_createtable(L, 0, 0);
	// set metatable
	lua_getglobal(L, "Window");
	lua_setmetatable(L, 1);
	// set userdata
	dokun::Window ** window = static_cast<dokun::Window **>(lua_newuserdata(L, sizeof(dokun::Window*)));
	*window = new dokun::Window();
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	{
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////
