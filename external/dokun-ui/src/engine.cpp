#include "../include/engine.hpp"
////////////
dokun::Engine::Engine(void) 
{}
////////////
dokun::Engine::~Engine(void)
{
	close();
}
////////////
dokun::Engine * dokun::Engine::engine_ptr (new dokun::Engine());
////////////
bool dokun::Engine::status (false);
////////////
bool dokun::Engine::open()
{
	return on_open();
}
////////////
int dokun::Engine::open(lua_State *L)
{
    lua_pushboolean(L, open());
	return 1;
}
////////////
void dokun::Engine::close()
{
	on_close();
}
////////////
int dokun::Engine::close (lua_State *L) 
{
	on_close();
	lua_close(L);
	exit(0);
	return 0;
}
////////////
////////////
////////////
dokun::Engine * dokun::Engine::get_engine()
{
	return engine_ptr;
}
//////////// 
bool dokun::Engine::get_status()
{
	return status;
}
//////////// 
bool dokun::Engine::on_open()
{
    if(status == 1) return true; // engine is on (already) - ensures that the engine is not initialized more than once
#ifdef __gnu_linux__ // engine should be closed manually
    std::atexit(dokun::Engine::close);
    std::at_quick_exit(dokun::Engine::close);
#endif 
    // set "user-defined" global locale (utf-8, etc.) - std::setlocale(LC_ALL, ""); // for C and C++ where synced with stdio // (#include <clocale>) //std::locale::global(std::locale("")); // for C++ (#include <locale>)  // wstring + wcout works, string + cout does not  (use L"str")//std::wcout << "User-preferred locale setting is " << std::locale("").name().c_str() << '\n';
    //std::locale::global(std::locale(""));
    // start session
    ////Logger::open();
    /////////////////////////////////////////
    // must be called when using threads on Linux before it calls XOpenDisplay (for thread-safety and so the app does not crash)
#ifdef __gnu_linux__
	if(!XInitThreads()) std::cout << DOKUN_UI_TAG "XInitThreads: multi-threads not supported" << std::endl;
#endif    
	/////////////////////////////////////////
#ifdef DOKUN_GLFW	   
    // Setup glfw (initialize glfw)
    glfwSetErrorCallback(dokun::Window::glfw_error_callback);
    if (!glfwInit())
        return false;
    // Decide GL+GLSL versions
#if defined(DOKUN_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // dokun uses GL 3.3 + GLSL 330 // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330";//"#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
#endif 
	/////////////////////////////////////////
#ifdef DOKUN_VULKAN
#ifdef DOKUN_WIN32
	HMODULE vulkan = LoadLibrary("vulkan-1.dll");
#endif
#ifdef __gnu_linux__
	void * vulkan = dlopen("libvulkan.so", RTLD_NOW);
#endif
	if(vulkan == nullptr)
	{
		Logger("Vulkan not detected");
		status = 0;
		return false;
	}
#ifdef DOKUN_WIN32
	const char * vulkan_ext[] = { "VK_KHR_surface", "VK_KHR_win32_surface" }; // enable extensions
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    const char * vulkan_ext[] = { "VK_KHR_surface", "VK_KHR_xlib_surface"  }; // enable extensions
#endif
#endif	 
	get_engine()->app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    get_engine()->app_info.pNext = nullptr;
    get_engine()->app_info.pApplicationName   = nullptr;
    get_engine()->app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    get_engine()->app_info.pEngineName   = "Dokun";
    get_engine()->app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);	
    get_engine()->app_info.apiVersion    = VK_MAKE_VERSION(1, 0, 2);			
	Renderer::get_pointer()->instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	Renderer::get_pointer()->instance_info.pNext = nullptr;
	Renderer::get_pointer()->instance_info.flags = 0;
	Renderer::get_pointer()->instance_info.pApplicationInfo = &get_engine()->app_info;
	Renderer::get_pointer()->instance_info.enabledLayerCount = 0;
	Renderer::get_pointer()->instance_info.ppEnabledLayerNames = nullptr;
	Renderer::get_pointer()->instance_info.enabledExtensionCount = 2;
#ifndef DOKUN_GLFW
	Renderer::get_pointer()->instance_info.ppEnabledExtensionNames = vulkan_ext;
#endif	
    VkResult result = vkCreateInstance(& Renderer::get_pointer()->instance_info, nullptr, & Renderer::get_pointer()->instance);
	if(result != VK_SUCCESS) 
	{
		Logger("Could not start Vulkan");
		DOKUN_VULKAN_ERROR(result, DOKUN_FILE, DOKUN_LINE);
		status = 0;
		return false;
	}
#endif // end of ifdef DOKUN_VULKAN
#ifdef DOKUN_WIN32
	if (WSAStartup(MAKEWORD(2, 2), & get_engine()->wsa_data) != 0)
    {
		Logger("Could not start Winsocket : " + std::to_string(WSAGetLastError()));
		status = 0;
		return false;
    }
#endif
    if(!dokun::Font::open())
	{
		Logger("Could not start FreeType");
		status = 0;
		return false;
	}
    /*if(!Audio::open())
	{
		Logger("Could not start OpenAL");
		status = 0;
		return false;
	}*/
    /////////////////////////////////////////
    //Renderer::start(); // initializes Renderer by setting default values
	// set status to 1 (proof that engine has been initialized)
	status = 1;
    //Logger("engine initialized.");
	return true;
}
////////////
void dokun::Engine::on_close()
{
    if(status == 0) return; // engine is off (already)
#ifdef DOKUN_VULKAN
    if(Renderer::get_instance() != VK_NULL_HANDLE ) { vkDestroyInstance (Renderer::get_instance(), nullptr);}
#endif
#ifdef DOKUN_WIN32
#ifdef DOKUN_VULKAN
    //FreeLibrary(vulkan);
#endif
	WSACleanup ();  // close winsocket
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_VULKAN
    //dlclose(vulkan);
#endif        
#endif
	dokun::Font::close (); // close freetype
    //Audio::close(); // close openal
	/////////////////////////////////////////
#ifdef DOKUN_GLFW
    Logger("GLFW terminated");
    glfwTerminate();
#endif	
    /////////////////////////////////////////
    // save cache (scripts)
    //Script::save_cache();
    // save session
    ////Logger::close();
	// reset status to its default (turns engine off)
	status = 0;
    Logger("Exiting engine with code 0 ..."); // exit message
}
////////////
////////////
/*
void pause_thread(int n) 
{
  std::this_thread::sleep_for (std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}
////////////
static int num_clients = 0;
static std::mutex m;
////////////
void client()
{
m.lock();
	// client
    Client * client = new Client();
	client->connect(1234, "localhost");
	std::cout << client->read() << std::flush << std::endl; // read from server once
	while (1)  // write to server (while program is running)
	{
		std::string text;
		std::cin >> text;
		client->write("client: " + text);
	}
m.unlock();	
}
void server()
{
    // server
	Server * server = new Server();
	server->bind(1234);
	server->listen(); // listens for any incoming connection
	if(server->accept() != -1) // accepts any incoming connection
	{
	    num_clients = num_clients + 1;
	    std::cout << "Server: client " << num_clients << " added to server" << std::endl;
	    std::thread new_client(client); // create a new client thread each time it accepts
	    new_client.join();
	}
	server->write("Welcome to Server0"); // write to client once
	while(1) // read from client (while program is running)
	{
		std::cout << server->read() << std::endl; 
	}
}*/
//////////// 
////////////
int dokun::Engine::test(lua_State *L)
{
	return 0;
} // ./dokun -e"dokun:test()"
////////////	
#ifdef __cplusplus // if c++
extern "C" {
#endif	
int dokun::Engine::reg(lua_State *L)
{	
    static const luaL_Reg module[] = 
    {
		{"start" , dokun::Engine::open  },
	    {"close" , dokun::Engine::close },
		{"test"  , dokun::Engine::test  },
        {nullptr , nullptr       },
    };	
	
    // -----------------------------------------------------------
    Script::global(L, "_ENGINE", "dokun");
	Script::global(L, "DOKUN_VERSION", std::string(ENGINE_VERSION));
	Script::global(L, "DOKUN_VERSION_MAJOR", std::string(ENGINE_VERSION_MAJOR));
	Script::global(L, "DOKUN_VERSION_MINOR", std::string(ENGINE_VERSION_MINOR));
	Script::global(L, "DOKUN_VERSION_PATCH", std::string(ENGINE_VERSION_PATCH));
	Script::global(L, "DOKUN_STATUS", (dokun::Engine::get_status() != 0));
	Script::global(L, "DOKUN_PLATFORM",         dokun::platform());
	// window -----------------------------------------------------------
	Script::table   (L, "Window");
	Script::function(L, "Window", "new"   ,  dokun::Window::window_new );
	Script::function(L, "Window", "create",  dokun::Window::create );
	Script::function(L, "Window", "show"  ,  dokun::Window::show   );
	Script::function(L, "Window", "update",  dokun::Window::update );	
	Script::function(L, "Window", "clear" ,  dokun::Window::clear  );
	Script::function(L, "Window", "destroy", dokun::Window::destroy);
	Script::function(L, "Window", "iconify", dokun::Window::iconify);		
	Script::function(L, "Window", "maximize", dokun::Window::maximize);		
	Script::function(L, "Window", "restore", dokun::Window::restore);				
	//Script::function(L, "Window", "poll_events", dokun::Window::poll_events); 
	//Script::function(L, "Window", "wait_events", dokun::Window::wait_events); 
	Script::function(L, "Window", "set_title", dokun::Window::set_title);
	Script::function(L, "Window", "set_width", dokun::Window::set_width);
	Script::function(L, "Window", "set_height", dokun::Window::set_height);
	Script::function(L, "Window", "set_size", dokun::Window::set_size);	
	Script::function(L, "Window", "set_mode", dokun::Window::set_mode);
	Script::function(L, "Window", "set_position", dokun::Window::set_position);
	Script::function(L, "Window", "set_style"  , dokun::Window::set_style); // sets the window's style
	Script::function(L, "Window", "set_icon"   , dokun::Window::set_icon);  // sets the window's icon
	Script::function(L, "Window", "set_cursor" , dokun::Window::set_cursor); // sets the window cursor			
	//Script::function (L, "Window", "set_event"  , dokun::Window::set_event);	// sets the how messages are retrieved		
	Script::function(L, "Window", "set_context", dokun::Window::set_context); // set the current context
	Script::function(L, "Window", "set_vsync", dokun::Window::set_vertical_synchronization); // set vertical synchronization	
	Script::function(L, "Window", "set_viewport", dokun::Window::set_viewport);		
	Script::function(L, "Window", "get_title", dokun::Window::get_title);
	Script::function(L, "Window", "get_width", dokun::Window::get_width);
	Script::function(L, "Window", "get_height", dokun::Window::get_height);
	Script::function(L, "Window", "get_size", dokun::Window::get_size);
	Script::function(L, "Window", "get_client_width", dokun::Window::get_client_width);
	Script::function(L, "Window", "get_client_height", dokun::Window::get_client_height);
	Script::function(L, "Window", "get_client_size", dokun::Window::get_client_size);
	Script::function(L, "Window", "get_mode", dokun::Window::get_mode);
	Script::function(L, "Window", "get_x", dokun::Window::get_x);
	Script::function(L, "Window", "get_y", dokun::Window::get_y);
	Script::function(L, "Window", "get_position", dokun::Window::get_position);
	Script::function(L, "Window", "is_open", dokun::Window::is_open);  
	Script::function(L, "Window", "is_visible", dokun::Window::is_visible);	
	Script::function(L, "Window", "is_focused", dokun::Window::is_focused);
	Script::function(L, "Window", "is_iconified", dokun::Window::is_iconified);
	Script::function(L, "Window", "is_context", dokun::Window::is_context);		
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
	    Script::function (L, "Window", "get_display", dokun::Window::get_display);
	#endif    
	#endif
	// texture -----------------------------------------------------------
	Script::table   (L, "Texture"                                    );
	Script::function(L, "Texture", "new"       , Texture::texture_new);	
	Script::function(L, "Texture", "load"      , Texture::load       );
	Script::function(L, "Texture", "copy"      , Texture::copy       );
	Script::function(L, "Texture", "generate"        , Texture::generate);
    Script::function(L, "Texture", "bind"        , Texture::bind);
	Script::function(L, "Texture", "unbind"        , Texture::unbind);	
	Script::function(L, "Texture", "destroy"        , Texture::destroy);
	Script::function(L, "Texture", "set_rect"        , Texture::set_rect);
	Script::function(L, "Texture", "set_filter"        , Texture::set_filter);
	Script::function(L, "Texture", "set_wrap"        , Texture::set_wrap);
	Script::function(L, "Texture", "set_internal_format"        , Texture::set_internal_format);
	Script::function(L, "Texture", "set_format"        , Texture::set_format);
	Script::function(L, "Texture", "set_param" , Texture::set_param  );
	//Script::function(L, "Texture", "set_"        , Texture::set_);
	Script::function(L, "Texture", "set_type"        , Texture::set_type);
	Script::function(L, "Texture", "get_width" , Texture::get_width  );
	Script::function(L, "Texture", "get_height", Texture::get_height );
	Script::function(L, "Texture", "get_size"  , Texture::get_size   );
	Script::function(L, "Texture", "get_data"  , Texture::get_data   );
	Script::function(L, "Texture", "get_file"  , Texture::get_file   );	
	Script::function(L, "Texture", "get_buffer", Texture::get_buffer );
    Script::function(L, "Texture", "get_filter"         , Texture::get_filter);	
    Script::function(L, "Texture", "get_wrap"           , Texture::get_wrap);	
    Script::function(L, "Texture", "get_internal_format", Texture::get_internal_format);	
    Script::function(L, "Texture", "get_format"         , Texture::get_format);	
	Script::function(L, "Texture", "get_info"  , Texture::get_info   );	
	Script::function(L, "Texture", "is_texture", Texture::is_texture ); // returns true if texture data is valid(texture has been loaded without error)	
    Script::function(L, "Texture", "is_generated", Texture::is_generated);			
	Script::function(L, "Texture", "", 0);
    //Script::function(L, "Texture", ""        , ::);	
	// image -----------------------------------------------------------
	Script::table   (L, "Image"                         ); // can load multiple image formats unlike texture
	Script::function(L, "Image", "new", Image::new_     );
	Script::function(L, "Image", "load", Image::load    );
	Script::function(L, "Image", "draw", Image::draw    );
	Script::function(L, "Image", "copy", Image::copy    );			
	Script::function(L, "Image", "copy_texture", Image::copy_texture    );			
	Script::function(L, "Image", "save", Image::save    );
	Script::function(L, "Image", "flip", Image::flip    );		
	Script::function(L, "Image", "resize", Image::resize);
    //Script::function(L, "Image", "scale_to_fit", Image::scale_to_fit);
    //Script::function(L, "Image", "generate", Image::generate);
	Script::function(L, "Image", "destroy", Image::destroy);
	Script::function(L, "Image", "clear"  , Image::clear  );
	Script::function(L, "Image", "translate", 0);
	Script::function(L, "Image", "rotate", 0);
	Script::function(L, "Image", "scale", 0);
	Script::function(L, "Image", "shear", 0);		
    Script::function(L, "Image", "set_x", Image::set_x);
    Script::function(L, "Image", "set_y", Image::set_y);
	Script::function(L, "Image", "set_position", Image::set_position);
    Script::function(L, "Image", "set_relative_position", Image::set_relative_position);
	Script::function(L, "Image", "set_angle", Image::set_angle);		
	Script::function(L, "Image", "set_scale", Image::set_scale);
    Script::function(L, "Image", "set_color", Image::set_color);
    Script::function(L, "Image", "set_alignment", Image::set_alignment);// gui-related
    Script::function(L, "Image", "set_visible", Image::set_visible);
    Script::function(L, "Image", "set_filter", Image::set_filter); // opengl-related
    Script::function(L, "Image", "set_wrap", Image::set_wrap);
    Script::function(L, "Image", "set_internal_format", Image::set_internal_format);
    Script::function(L, "Image", "set_format", Image::set_format);
    Script::function(L, "Image", "set_param", Image::set_param);
    //Script::function(L, "Image", "set_", Image::set_);
	Script::function(L, "Image", "get_width", Image::get_width);
	Script::function(L, "Image", "get_height", Image::get_height);
	Script::function(L, "Image", "get_depth", Image::get_depth);			
	Script::function(L, "Image", "get_size", Image::get_size);
	Script::function(L, "Image", "get_data", Image::get_data); // pixel data			
	Script::function(L, "Image", "get_file", Image::get_file);
	Script::function(L, "Image", "get_channel", Image::get_channel);
	Script::function(L, "Image", "get_format", 0);		
    Script::function(L, "Image", "get_color", Image::get_color);	   
    Script::function(L, "Image", "get_x", Image::get_x);	   
    Script::function(L, "Image", "get_y", Image::get_y);	   
    Script::function(L, "Image", "get_position", Image::get_position);	  
    Script::function(L, "Image", "get_relative_x", Image::get_relative_x);
    Script::function(L, "Image", "get_relative_y", Image::get_relative_y);
    Script::function(L, "Image", "get_relative_position", Image::get_relative_position); 
    Script::function(L, "Image", "get_angle", Image::get_angle);	   
    Script::function(L, "Image", "get_scale", Image::get_scale);	   
    Script::function(L, "Image", "get_aspect_ratio_correction", Image::get_aspect_ratio_correction);
    Script::function(L, "Image", "get_alignment", Image::get_alignment);
    Script::function(L, "Image", "get_rect", Image::get_rect);
    Script::function(L, "Image", "get_buffer", Image::get_buffer); // opengl-related
    Script::function(L, "Image", "get_filter", Image::get_filter);
    Script::function(L, "Image", "get_wrap", Image::get_wrap);
    Script::function(L, "Image", "get_internal_format", Image::get_internal_format);
    Script::function(L, "Image", "get_format", Image::get_format);	
    //Script::function(L, "Image", "get_", Image::get_);   
	Script::function(L, "Image", "is_png", Image::is_png);
	Script::function(L, "Image", "is_jpeg", 0);		
	Script::function(L, "Image", "is_bmp", 0);
	Script::function(L, "Image", "is_tga", 0);		
	Script::function(L, "Image", "is_gif", 0);    // Image:: 
	Script::function(L, "Image", "is_tiff", 0);			
	Script::function(L, "Image", "is_image", Image::is_image);
	//Script::function(L, "", ""        , ::);
    // mouse -----------------------------------------------------------
	Script::table   (L, "Mouse"                                     );
    Script::function(L, "Mouse", "show"        , Mouse::show        ); // make mouse visible
	Script::function(L, "Mouse", "hide"        , Mouse::hide        ); // make mouse invisible
	Script::function(L, "Mouse", "set_position", Mouse::set_position); // set mouse position
	Script::function(L, "Mouse", "set_cursor", Mouse::set_cursor); // set mouse position
	Script::function(L, "Mouse", "get_position", Mouse::get_position); // get mouse position
	Script::function(L, "Mouse",  "get_delta"  , Mouse::get_delta   ); // get mouse wheel delta		
	Script::function(L, "Mouse", "get_color"   , Mouse::get_color   ); // get pixel at a specific coordinate	
	Script::function(L, "Mouse", "is_moved"    , Mouse::is_moved    ); // checks if mouse has moved
	Script::function(L, "Mouse", "is_scrolled" , Mouse::is_scrolled );	// checks if mouse is being scrolled
	Script::function(L, "Mouse", "is_pressed"  , Mouse::is_pressed  ); // specific mouse button pressed
	Script::function(L, "Mouse", "is_released" , Mouse::is_released ); 
	Script::function(L, "Mouse", "is_down"     , Mouse::is_down     ); // any mouse button pressed
	Script::function(L, "Mouse", "is_up"       , Mouse::is_up       ); 	
	Script::function(L, "Mouse", "is_over"     , Mouse::is_over     );
    //Script::function(L, "", ""        , ::);	
	// keyboard -----------------------------------------------------------
	Script::table   (L, "Keyboard"                                      );
	Script::function(L, "Keyboard", "is_pressed" , Keyboard::is_pressed );// specific key is pressed
	Script::function(L, "Keyboard", "is_released", Keyboard::is_released);// specific key is released
	Script::function(L, "Keyboard", "is_down"    , Keyboard::is_down    );// any key is pressed 
	Script::function(L, "Keyboard", "is_up"      , Keyboard::is_up      ); // any key is released
	//Script::function(L, "", ""        , ::);
	// -----------------------------------------------------------
	//Script::table(L, "Touch"   ); // Touch pad stuff
	//Script::function(L, "", ""        , ::);
	// audio -----------------------------------------------------------
	/*Script::table   (L, "Audible");
	Script::function(L, "Audible", "play"        , Audible::play        );
	Script::function(L, "Audible", "pause"       , Audible::pause       );
	Script::function(L, "Audible", "stop"        , Audible::stop        );
	Script::function(L, "Audible", "set_volume"  , Audible::set_volume  );
	Script::function(L, "Audible", "set_loop"    , Audible::set_loop    );		
	Script::function(L, "Audible", "set_pitch"   , Audible::set_pitch   );
	Script::function(L, "Audible", "set_position", Audible::set_position);		
	Script::function(L, "Audible", "get_volume"  , Audible::get_volume  );
	Script::function(L, "Audible", "get_pitch"   , Audible::get_pitch   );		
	Script::function(L, "Audible", "get_duration", Audible::get_duration);
	Script::function(L, "Audible", "get_position", Audible::get_position);
	Script::function(L, "Audible", "get_data"    , Audible::get_data    );
	Script::function(L, "Audible", "get_file"    , Audible::get_file    );		
	Script::function(L, "Audible", "is_playing"  , Audible::is_playing  );
	Script::function(L, "Audible", "is_paused"   , Audible::is_paused   );		
	Script::function(L, "Audible", "is_stopped"  , Audible::is_stopped  );
	Script::function(L, "Audible", "is_looped"   , Audible::is_looped   );
	//Script::function(L, "", ""        , ::);
	// sound -----------------------------------------------------------
	Script::table   (L, "Sound");
	Script::function(L, "Sound", "new" , Sound::sound_new);
	Script::function(L, "Sound", "load", Sound::load     );
	//Script::function(L, "", ""        , ::);
	// music -----------------------------------------------------------
	Script::table   (L, "Music");
	Script::function(L, "Music", "new",  Music::music_new);
	Script::function(L, "Music", "load", Music::load     );		
	Script::function(L, "Music", "play", Music::play     );	
    // sound, music inherit audio metatable (and functions) 		
    Script::inherit (L, "Audible", "Sound");
    Script::inherit (L, "Audible", "Music");	
	//Script::function(L, "", ""        , ::);
	// voice -----------------------------------------------------------
	Script::table   (L, "Voice");
	Script::function(L, "Voice", "new", Voice::new_);	
	Script::function(L, "Voice", "start", Voice::start);
	Script::function(L, "Voice", "stop", Voice::stop);		
	Script::function(L, "Voice", "play", Voice::play);
	Script::function(L, "Voice", "pause", Voice::pause);		
	Script::function(L, "Voice", "save", Voice::save);
	Script::function(L, "Voice","set_playback_volume", 0);
	Script::function(L, "Voice", "set_microphone_volume", 0);		
	Script::function(L, "Voice","get_data", Voice::get_data);
	Script::function(L, "Voice","is_recording", Voice::is_recording);	
	Script::function(L, "Voice","is_playing", Voice::is_playing);
	Script::function(L, "Voice","is_paused", Voice::is_paused);
	Script::function(L, "Voice","is_stopped", Voice::is_stopped);
	//Script::function(L, "", ""        , ::);*/
	// ui -----------------------------------------------------------
	Script::table   (L, "GUI");
	Script::function(L, "GUI","destroy", GUI::destroy);			
	Script::function(L, "GUI","show", GUI::show);
	Script::function(L, "GUI","hide", GUI::hide);
	Script::function(L, "GUI","show_all", GUI::show_all);
	Script::function(L, "GUI", "hide_all", GUI::hide_all);	
	Script::function(L, "GUI","set_width", GUI::set_width);		
	Script::function(L, "GUI","set_height", GUI::set_height);		
	Script::function(L, "GUI","set_size", GUI::set_size);
	Script::function(L, "GUI","set_position", GUI::set_position);
	Script::function(L, "GUI", "set_relative_position", GUI::set_relative_position);
    Script::function(L, "GUI", "set_scale"        , GUI::set_scale);
    Script::function(L, "GUI", "set_angle"        , GUI::set_angle);		
	Script::function(L, "GUI","set_orientation", GUI::set_orientation);
	Script::function(L, "GUI","set_parent", GUI::set_parent);		
	Script::function(L, "GUI","set_visible", GUI::set_visible);
	Script::function(L, "GUI","set_active", GUI::set_active);
	Script::function(L, "GUI","set_draggable", GUI::set_draggable);
	Script::function(L, "GUI","set_droppable", GUI::set_droppable);
	Script::function(L, "GUI","set_resizeable", GUI::set_resizeable);
	Script::function(L, "GUI","set_sortable", GUI::set_sortable);		
	Script::function(L, "GUI","get_width", GUI::get_width);
	Script::function(L, "GUI","get_height", GUI::get_height);
	Script::function(L, "GUI","get_size", GUI::get_size);
	Script::function(L, "GUI", "get_rect", GUI::get_rect);
	Script::function(L, "GUI","get_x", GUI::get_x);
	Script::function(L, "GUI","get_y", GUI::get_y);
	Script::function(L, "GUI", "get_position", GUI::get_position);
	Script::function(L, "GUI","get_relative_x", GUI::get_relative_x);
	Script::function(L, "GUI","get_relative_y", GUI::get_relative_y);	
    Script::function(L, "GUI", "get_relative_position", GUI::get_relative_position);
    Script::function(L, "GUI", "get_scale"        , GUI::get_scale);
    Script::function(L, "GUI", "get_angle"        , GUI::get_angle);
	Script::function(L, "GUI","get_orientation", GUI::get_orientation);
	Script::function(L, "GUI","get_parent", GUI::get_parent);
	Script::function(L, "GUI","is_visible", GUI::is_visible);
	Script::function(L, "GUI","is_active", GUI::is_active); 
	Script::function(L, "GUI", "is_draggable", GUI::is_draggable);
	Script::function(L, "GUI","is_droppable", GUI::is_droppable);
	Script::function(L, "GUI","is_resizeable", GUI::is_resizeable);
	Script::function(L, "GUI", "is_sortable", GUI::is_sortable);
	Script::function(L, "GUI", "is_hovered" , GUI::is_hovered);
	Script::function(L, "GUI", "is_pressed" , GUI::is_pressed);
	Script::function(L, "GUI", "is_selected", 0);
	Script::function(L, "GUI", "is_released", 0);		
	Script::function(L, "GUI", "is_focused" , 0);
	Script::function(L, "GUI", "is_dragged" , 0);		
	Script::function(L, "GUI", "is_dropped" , 0);
	Script::function(L, "GUI", "is_sorted"  , 0);
	Script::function(L, "GUI", "is_gui"     , GUI::is_gui);	
	Script::function(L, "GUI", "has_parent" , GUI::has_parent);
	Script::function(L, "GUI", "is_child_of", GUI::is_child_of);
	Script::function(L, "GUI", "is_parent_of", GUI::is_parent_of);
	//Script::function(L, "GUI", ""        , GUI::);
	// widget -----------------------------------------------------------
    Script::table   (L, "Box"          ); 
	Script::attach  (L, "GUI"   , "Box"); // attach Box to GUI : GUI.Box 
	Script::inherit (L, "GUI"   , "Box"); 
    Script::function(L, "Box", "add", Box::add);
	Script::function(L, "Box", "new" , Box::widget_new);
	Script::function(L, "Box", "draw", Box::draw      );
	Script::function(L, "Box", "set_color", Box::set_color);
	//Script::function(L, "Box", "set_alpha", Box::set_alpha);
	//Script::function(L, "Box", "set_fill", Box::set_fill);
	Script::function(L, "Box", "set_outline", Box::set_outline);
	Script::function(L, "Box", "set_outline_width", Box::set_outline_width);
	Script::function(L, "Box", "set_outline_color", Box::set_outline_color);
	//Script::function(L, "Box", "set_outline_style", Box::set_outline_style);
	//Script::function(L, "Box", "set_outline_antialiased", Box::set_outline_antialiased);
	//Script::function(L, "Box", "set_border", Box::set_border);
	//Script::function(L, "Box", "set_border_size", Box::set_border_size);
	//Script::function(L, "Box", "set_border_width", Box::set_border_width);
	//Script::function(L, "Box", "set_border_color", Box::set_border_color);
	//Script::function(L, "Box", "set_border_style", Box::set_border_style);
	//Script::function(L, "Box", "set_border_radius", Box::set_border_radius);
	//Script::function(L, "Box", "set_radius", Box::set_radius);
	//Script::function(L, "Box", "set_trim", Box::set_trim);
	Script::function(L, "Box", "set_gradient", Box::set_gradient);
	//Script::function(L, "Box", "set_gradient_color", Box::set_gradient_color);
	Script::function(L, "Box", "set_title_bar", Box::set_title_bar);
	Script::function(L, "Box", "set_title_bar_size", Box::set_title_bar_size);
	Script::function(L, "Box", "set_title_bar_text", Box::set_title_bar_text);
	Script::function(L, "Box", "set_title_bar_text_color", Box::set_title_bar_text_color);
    Script::function(L, "Box", "set_title_bar_label", Box::set_title_bar_label);
    Script::function(L, "Box", "set_title_bar_image", Box::set_title_bar_image);
	Script::function(L, "Box", "set_title_bar_icon", Box::set_title_bar_image);
	Script::function(L, "Box", "set_title_bar_color", Box::set_title_bar_color);
	Script::function(L, "Box", "set_title_bar_button_iconify", Box::set_title_bar_button_iconify);
	Script::function(L, "Box", "set_title_bar_button_maximize", Box::set_title_bar_button_maximize);
	Script::function(L, "Box", "set_title_bar_button_close", Box::set_title_bar_button_close);
	//Script::function(L, "Box", "set_as_icon", Box::set_as_icon);
	Script::function(L, "Box", "set_as_tooltip", Box::set_as_tooltip);
	//Script::function(L, "Box", "set_as_", Box::set_as_);
	Script::function(L, "Box", "set_image", Box::set_image);
	Script::function(L, "Box", "set_label", Box::set_label);
	Script::function(L, "Box", "set_text", Box::set_text);
    Script::function(L, "Box", "set_image_list", Box::set_image_list);
    //Script::function(L, "Box", "set_gui_list", Box::set_gui_list); // or child_list
	//Script::function(L, "Box", "set_", Box::set_);
	//Script::function(L, "Box", "", Box::);
	Script::function(L, "Box", "get_color", Box::get_color);
	Script::function(L, "Box", "get_image", Box::get_image);
	Script::function(L, "Box", "get_label", Box::get_label);
    Script::function(L, "Box", "get_image_list", Box::get_image_list);
    Script::function(L, "Box", "get_label_list", Box::get_label_list);
	//Script::function(L, "Box", "get_text", Box::get_text);
	//Script::function(L, "Box", "", Box::);
	//Script::function(L, "Box", "", Box::);
	//Script::function(L, "Box", "", Box::);
	//Script::function(L, "Box", "", Box::);
    Script::execute(L, "Widget = Box"); // assign Widget to Box as another name for Box (You can also use Widget in place of Box)
	// button -----------------------------------------------------------
	Script::table   (L, "Button"); // Button = {}  Button_mt = {__index = Button }
	Script::attach  (L, "GUI", "Button"); // GUI.Button = Button
	Script::inherit (L, "GUI", "Button"); // setmetatable(Button, GUI_mt)
	Script::function(L, "Button", "new", Button::new_ ); // default, radio, check_box
	Script::function(L, "Button", "draw", Button::draw);
	Script::function(L, "Button", "", 0);
	Script::function(L, "Button", "", 0);
	Script::function(L, "Button", "set_color", Button::set_color);		
	Script::function(L, "Button", "set_text", Button::set_text  ); // generates Label		
	Script::function(L, "Button", "set_label", Button::set_label);
	Script::function(L, "Button", "set_image", Button::set_image);
	Script::function(L, "Button", "get_color", Button::get_color);
	Script::function(L, "Button", "get_text", Button::get_text  ); // shortcut to button:get_label():get_string()
	Script::function(L, "Button", "get_label", Button::get_label); 
	Script::function(L, "Button", "get_image", Button::get_image);
	//Script::function(L, "", ""        , ::);
	// label -----------------------------------------------------------
	Script::table   (L, "Label"                                 ); // create a table Label with a Label_mt metatable
	Script::attach  (L, "GUI"  , "Label"                        ); // attach Label to GUI : GUI.Label
	Script::inherit (L, "GUI"  , "Label"                        ); // inherit from GUI (Label inherits GUI functions)
	Script::function(L, "Label", "new"       , Label::label_new );
	Script::function(L, "Label", "draw"      , Label::draw      );
	Script::function(L, "Label", "set_font"  , Label::set_font  );	
	Script::function(L, "Label", "set_string", Label::set_string);
	Script::function(L, "Label", "set_color" , Label::set_color );
    Script::function(L, "Label", "set_style", Label::set_style);
    Script::function(L, "Label", "set_background_color", Label::set_background_color);
    Script::function(L, "Label", "set_alignment", Label::set_alignment); // test
    //Script::function(L, "Label", "set_", Label::set_);
    // getters	
	Script::function(L, "Label", "get_font"  , Label::get_font  );
	Script::function(L, "Label", "get_string", Label::get_string);
    Script::function(L, "Label", "get_color", Label::get_color  );
    Script::function(L, "Label", "get_style", Label::get_style  );
    Script::function(L, "Label", "get_background_color", Label::get_background_color);
    Script::function(L, "Label", "get_alignment", Label::get_alignment); // test
    //Script::function(L, "Label", "get_", Label::get_);
    Script::function(L, "Label", "get_width", Label::get_width  ); // function crashes in Lua
    Script::function(L, "Label", "get_height", Label::get_height);
    Script::function(L, "Label", "get_rect", Label::get_rect);
    //Script::function(L, "Label", "get_", Label::get_);
	// overridden functions
	Script::function(L, "Label", "set_size"        , Label::set_size);
	// boolean functions
	Script::function(L, "Label", "is_label"        , Label::is_label);
    //Script::function(L, "Label", ""        , Label::);	
	// edit -----------------------------------------------------------
	Script::table   (L, "Edit"                    );
	Script::function(L, "Edit", "new" , Edit::new_);
    Script::function(L, "Edit", "draw", Edit::draw);
    //Script::function(L, "", ""        , ::);	
	// progressbar -----------------------------------------------------------
	Script::table   (L, "Progressbar");
	Script::inherit (L, "GUI", "Progressbar");
	Script::function(L, "Progressbar", "new", Progressbar::new_);
	Script::function(L, "Progressbar", "draw", Progressbar::draw);
	Script::function(L, "Progressbar", "set_text", Progressbar::set_text);
	Script::function(L, "Progressbar", "set_label", Progressbar::set_label);
	Script::function(L, "Progressbar", "set_foreground_color", Progressbar::set_foreground_color);
	Script::function(L, "Progressbar", "set_background_color", Progressbar::set_background_color);
	Script::function(L, "Progressbar", "set_range", Progressbar::set_range);
	Script::function(L, "Progressbar", "set_minimum_value", Progressbar::set_minimum_value);
	Script::function(L, "Progressbar", "set_maximum_value", Progressbar::set_maximum_value);
	Script::function(L, "Progressbar", "set_value", Progressbar::set_value);
	Script::function(L, "Progressbar", "reset", Progressbar::reset);
	Script::function(L, "Progressbar", "set_outline", Progressbar::set_outline);
	Script::function(L, "Progressbar", "set_outline_width", Progressbar::set_outline_width);
	Script::function(L, "Progressbar", "set_outline_color", Progressbar::set_outline_color);
	Script::function(L, "Progressbar", "set_outline_antialiased", Progressbar::set_outline_antialiased);
	//Script::function(L, "Progressbar", "set_", Progressbar::set_);
	//Script::function(L, "Progressbar", "", Progressbar::);	
	Script::function(L, "Progressbar", "get_text"         , Progressbar::get_text                  );
	Script::function(L, "Progressbar", "get_label"        , Progressbar::get_label                 );
	Script::function(L, "Progressbar", "get_color"        , Progressbar::get_color                 );
    Script::function(L, "Progressbar", "get_foreground_color", Progressbar::get_foreground_color   );
    Script::function(L, "Progressbar", "get_background_color", Progressbar::get_background_color   );
	Script::function(L, "Progressbar", "get_range"        , Progressbar::get_range                 );
	Script::function(L, "Progressbar", "get_minimum_value", Progressbar::get_minimum_value         );
	Script::function(L, "Progressbar", "get_maximum_value", Progressbar::get_maximum_value         );
	Script::function(L, "Progressbar", "get_value"        , Progressbar::get_value                 );
	//Script::function(L, "Progressbar", "get_", Progressbar::get_);
	//Script::function(L, "Progressbar", "", Progressbar::);
	///Script::function(L, "Progressbar", "", Progressbar::);
	//Script::function(L, "Progressbar", "", Progressbar::);
	//Script::function(L, "Progressbar", "", Progressbar::);
	//Script::function(L, "Progressbar", "", Progressbar::);
	// slider -----------------------------------------------------------
	Script::table   (L, "Slider");
	Script::inherit (L, "GUI", "Slider");
	//Script::function(L, "Slider", ""        , Slider::);
	// toggle -----------------------------------------------------------
	Script::table   (L, "Toggle");
	//Script::inherit (L, "GUI", "Toggle");
	//Script::function(L, "Toggle", ""        , ::);
	// scrollbar -----------------------------------------------------------
	Script::table   (L, "Scrollbar");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// list -----------------------------------------------------------
	Script::table   (L, "List");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// spinner -----------------------------------------------------------
	Script::table   (L, "Spinner");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// menubar -----------------------------------------------------------
	Script::table   (L, "Menubar");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// tooltip ----------------------------------------------------------- // deprecated
	//Script::table   (L, "Tooltip");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// grid -----------------------------------------------------------
	Script::table   (L, "Grid");
	Script::inherit (L, "GUI", "Grid");
	Script::function(L, "Grid", "new"       , Grid::grid_new);
	Script::function(L, "Grid", "draw"      , Grid::draw);
	Script::function(L, "Grid", "update"    , Grid::update);
	Script::function(L, "Grid", "set_row"   , Grid::set_rows);
	Script::function(L, "Grid", "set_column", Grid::set_columns);
	//Script::function(L, "Grid", ""        , Grid::);
	// font -----------------------------------------------------------
	Script::table   (L, "Font"                          );
	Script::function(L, "Font", "new"     , dokun::Font::font_new);
	Script::function(L, "Font", "load"    , dokun::Font::load   );
    Script::function(L, "Font", "generate", dokun::Font::generate);
	Script::function(L, "Font", "destroy" , dokun::Font::destroy);
    Script::function(L, "Font", "set_width", dokun::Font::set_width);
    Script::function(L, "Font", "set_height", dokun::Font::set_height);
    Script::function(L, "Font", "set_size", dokun::Font::set_size);
	Script::function(L, "Font", "get_size", dokun::Font::get_size);
	Script::function(L, "Font", "get_rect", dokun::Font::get_rect);
	Script::function(L, "Font", "get_data", dokun::Font::get_data);
    Script::function(L, "Font", "is_generated", dokun::Font::is_generated);
	//Script::function(L, "Font", ""        , dokun::Font::);
	// network -----------------------------------------------------------
	//Script::global (L, "socket", Socket::new_); // returns a socket
	//Script::table (L, "Network"); // Network_mt
	//Script::function(L, "Network", "connect", Socket::connect);
	//Script::function(L, "Network", "read", Socket::read); // receives data
	//Script::function(L, "Network", "write", Socket::write); // sends data
	//Script::function(L, "Network", "bind", 0);
	//Script::function(L, "", ""        , ::);
    // entity -----------------------------------------------------------
    Script::table   (L, "Entity"                                           );
	Script::function(L, "Entity", "new"             , Entity::create       );
	Script::function(L, "Entity", "destroy"         , Entity::destroy      );
	//Script::function(L, "Entity", "draw"            , Entity::draw         ); // cannot use this in lua as it will override Sprite.draw, Model.draw(), etc.
    Script::function(L, "Entity", "add_component"   , Entity::add_component);
    Script::function(L, "Entity", "remove_component", 0);//Entity::remove_component);		
    Script::function(L, "Entity", "set_component"   , Entity::set_component);
    Script::function(L, "Entity", "set_shader"      , Entity::set_shader   );
    Script::function(L, "Entity", "set_script"      , Entity::set_script   );		
    Script::function(L, "Entity", "get_component"   , Entity::get_component);
    Script::function(L, "Entity", "get_shader"      , Entity::get_shader   );
    Script::function(L, "Entity", "get_script"      , Entity::get_script   );	
    Script::function(L, "Entity", "set_visible"     , Entity::set_visible  );			
	Script::function(L, "Entity", "is_entity"       , Entity::is_entity    );	
	Script::function(L, "Entity", "is_visible"      , Entity::is_visible   );			
	Script::function(L, "Entity", "has_component"   , Entity::has_component);
    //Script::function(L, "", ""        , ::);	
    // component -----------------------------------------------------------	
	Script::table   (L, "Component"                               );
	Script::function(L, "Component", "new"    , Component::create );
    Script::function(L, "Component", "destroy", Component::destroy);
    //Script::function(L, "", ""        , ::);	
	// sub-entities -----------------------------------------------------------
    Script::inherit(L, "Entity", "GUI"   );
	// timer -----------------------------------------------------------
	Script::table   (L, "Timer"                                     ); // stop watch
	Script::function(L, "Timer", "new"         , Timer::new_        );
	Script::function(L, "Timer", "start"       , Timer::start       );
	Script::function(L, "Timer", "stop"        , Timer::stop        );
	Script::function(L, "Timer", "reset"       , Timer::reset       );	
    Script::function(L, "Timer", "get_milliseconds", Timer::get_milliseconds);	
	Script::function(L, "Timer", "get_seconds"     , Timer::get_seconds     );
	Script::function(L, "Timer", "get_minutes"     , Timer::get_minutes     );			
	Script::function(L, "Timer", "get_hours"       , Timer::get_hours       );
	Script::function(L, "Timer", "get_status"      , Timer::get_status      );	
	// elapsed	
	Script::function(L, "Timer", "milliseconds" , Timer::milliseconds);	// elapsed		
	Script::function(L, "Timer", "seconds"      , Timer::seconds     );
	Script::function(L, "Timer", "minutes"      , Timer::minutes     );			
	Script::function(L, "Timer", "hours"        , Timer::hours       );
	Script::function(L, "Timer", "days"         , Timer::days        );
	Script::function(L, "Timer", "weeks"        , Timer::weeks       );
	Script::function(L, "Timer", "months"       , Timer::months      );
	Script::function(L, "Timer", "years"        , Timer::years       );
	Script::function(L, "Timer", "reset_elapsed", Timer::reset_e     );
	// delta
	Script::function(L, "Timer", "delta"       , Timer::delta       );
	// framerate
	Script::function(L, "Timer", "framerate"   , Timer::framerate   );
	//Script::function(L, "Timer", ""        , Timer::);
	// shader -----------------------------------------------------------	
	Script::table   (L, "Shader"                                  );
    //Script::function(L, "Shader", "new"       , Shader::new_      );
	//Script::function(L, "Shader", "create"    , Shader::create    );		
	//Script::function(L, "Shader", "destroy"   , Shader::destroy   );			
	//Script::function(L, "Shader", "load"      , Shader::load      );
	//Script::function(L, "Shader", "compile"   , Shader::compile   );
	//Script::function(L, "Shader", "set_source", Shader::set_source);
	//Script::function(L, "Shader", "get_id"    , Shader::get_id    );
	//Script::function(L, "Shader", "get_type"  , Shader::get_type  );		
	//Script::function(L, "Shader", "get_source", Shader::get_source);
	//Script::function(L, "Shader", "is_shader" , Shader::is_shader );
	//Script::function(L, "", ""        , ::);
	// resources -----------------------------------------------------------
	/*Script::table   (L, "Resource"                        ); 
    Script::function(L, "Resource", "load", Resource::load); // loads all resources
    //Script::function(L, "Resource", "", Resource::);*/
	// data serialization -----------------------------------------------------------
	// deprecated
	/*Script::table   (L, "Data"                          );
	Script::function(L, "Data", "new"    , Data::new_   ); 
	Script::function(L, "Data", "open"   , Data::open   ); // creates an sql database
	Script::function(L, "Data", "close"  , Data::close  );		
	Script::function(L, "Data", "execute", Data::execute); // executes an sql statement		
	Script::function(L, "Data", "exec"   , Data::execute); // executes an sql statement
	Script::function(L, "Data", "table"  , Data::table  );
	Script::function(L, "Data", "insert" , Data::insert );
	Script::function(L, "Data", "select" , Data::select );	
	Script::function(L, "Data", "update" , Data::update );
	Script::function(L, "Data", "column" , Data::column );	
	Script::function(L, "Data", "drop"   , Data::drop   ); // deletes a SQL table
	Script::function(L, "Data", "", 0);*/
    //Script::function(L, "", ""        , ::);	
	// factory -----------------------------------------------------------
	Script::table   (L, "Factory"                                    ); // new !
	Script::function(L, "Factory","new"       , Factory::new_        );
	Script::function(L, "Factory","store"     , Factory::store       );
	Script::function(L, "Factory","release"   , Factory::release     );
	Script::function(L, "Factory","get_object", Factory::get_object  );
	Script::function(L, "Factory","get_size"  , Factory::get_size    );
	Script::function(L, "Factory","get_index" , Factory::get_location); // can be used as an id
	Script::function(L, "Factory","is_empty"  , Factory::is_empty    );
	Script::function(L, "Factory","is_stored" , Factory::is_stored   );
	//Script::function(L, "", ""        , ::);
	// script -----------------------------------------------------------
	Script::table   (L, "Script"                                          );
	Script::function(L, "Script", "new"           , Script::new_          );	
	Script::function(L, "Script", "load"          , Script::load_script   );	
	Script::function(L, "Script", "load_directory", Script::load_directory);		
	Script::function(L, "Script", "generate"      , Script::generate      );
	Script::function(L, "Script", "write"         , Script::write         );	
	Script::function(L, "Script", "copy"          , Script::copy          );			
	Script::function(L, "Script", "save"          , Script::save          );			
	Script::function(L, "Script", "count"         , Script::get_count     );
	Script::function(L, "Script", "get_file"      , Script::get_file      );		
	Script::function(L, "Script", "is_loaded"     , Script::is_script     );
    //Script::function(L, "", ""        , ::);	
	// console -----------------------------------------------------------
	Script::table   (L, "Console"                          ); 
	Script::function(L, "Console", "free", Console::destroy);   // destroys the console window
	Script::function(L, "Console", "show", Console::create );	 // recreates the console window (and brings it forward)
	Script::function(L, "Console", "draw", Console::draw   );
	Script::function(L, "Console", "write" ,Console::write ); // writes to the console output window
	Script::function(L, "Console", "code" ,Console::code );  // executes lua code
	Script::function(L, "Console", "show_edit", Console::show_edit);
	Script::function(L, "Console", "hide_edit", Console::hide_edit);
	Script::function(L, "Console", "set_size", Console::set_size);
	Script::function(L, "Console", "set_position", Console::set_position);
	Script::function(L, "Console", "set_mode", Console::set_mode);
	//Script::function(L, "Console", ""        , Console::);
    // renderer ----------------------------------------------------------
    Script::table   (L, "Renderer"                                    ); // static class (pass to function as 'self' without any instances)
    Script::global(L, "set_current_API", Renderer::set_current_API);
    //Script::function(L, "Renderer", "", Renderer::);
    // vector3 -----------------------------------------------------------
	Script::table   (L, "Vector"                                 ); // a vector class (3d)
	Script::function(L, "Vector", "new"      , Vector3::new_     );
	Script::function(L, "Vector", "length"   , Vector3::length   );
	Script::function(L, "Vector", "normalize", Vector3::normalize);
	Script::function(L, "Vector", "dot"      , Vector3::dot      );
	Script::function(L, "Vector", "cross"    , Vector3::cross    );		
	Script::function(L, "Vector", "add"      , Vector3::add      );
	Script::function(L, "Vector", "sub"      , Vector3::sub      );
	Script::function(L, "Vector", "mul"      , Vector3::mul      );
	Script::function(L, "Vector", "div"      , Vector3::div      );	
	Script::function(L, "Vector", "set"      , Vector3::set      );				 
	Script::function(L, "Vector", "get"      , Vector3::get      );	
    // Math --------------------------------------------------------------	
	Script::table(L, "Math");
	Script::function(L, "Math", "distance", Math::distance2d);
	//Script::function(L, "Math", "", Math::);
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	//Script::function(L, "", ""        , ::);
	// globals -----------------------------------------------------------
	// utilities
	// entity
	//Script::global(L, "set_shader", Entity::set_shader);
	// global variables -----
	// vector functions
	Script::global (L, "Vector2", Vector2::Vector_new);
    Script::global (L, "Vector3", Vector3::Vector_new);
    Script::global (L, "Vector4", Vector4::Vector_new);	
	// buttons
	Script::global(L, "MOUSE_LEFT",     DOKUN_MOUSE_LEFT);
	Script::global(L, "MOUSE_MIDDLE", DOKUN_MOUSE_MIDDLE);
	Script::global(L, "MOUSE_RIGHT",   DOKUN_MOUSE_RIGHT);	
	// keys
	Script::global(L,"KEY_UP",       DOKUN_KEY_UP);
	Script::global(L,"KEY_DOWN",   DOKUN_KEY_DOWN);
	Script::global(L,"KEY_LEFT",   DOKUN_KEY_LEFT);
	Script::global(L,"KEY_RIGHT", DOKUN_KEY_RIGHT);
    // special
    Script::global(L,"KEY_SPACE", DOKUN_KEY_SPACE);	
    Script::global(L,"KEY_RETURN", DOKUN_KEY_RETURN);
    Script::global(L,"KEY_ESCAPE", DOKUN_KEY_ESCAPE);	
    Script::global(L,"KEY_BACKSPACE", DOKUN_KEY_BACKSPACE);	
    Script::global(L,"KEY_TAB", DOKUN_KEY_TAB);
    Script::global(L,"KEY_CAPS_LOCK", DOKUN_KEY_CAPS_LOCK);
    Script::global(L,"KEY_SHIFT_L", DOKUN_KEY_SHIFT_L);
    Script::global(L,"KEY_SHIFT_R", DOKUN_KEY_SHIFT_R);
    Script::global(L,"KEY_CONTROL_L", DOKUN_KEY_CONTROL_L);
    Script::global(L,"KEY_CONTROL_R", DOKUN_KEY_CONTROL_R);
    Script::global(L,"KEY_ALT_L", DOKUN_KEY_ALT_L);
    Script::global(L,"KEY_ALT_R", DOKUN_KEY_ALT_R);
    Script::global(L,"KEY_MENU", DOKUN_KEY_MENU);
    Script::global(L,"KEY_PRINT_SCREEN", DOKUN_KEY_PRINT_SCREEN);
    Script::global(L,"KEY_SCROLL_LOCK", DOKUN_KEY_SCROLL_LOCK);
    Script::global(L,"KEY_PAUSE", DOKUN_KEY_PAUSE);
    Script::global(L,"KEY_INSERT", DOKUN_KEY_INSERT);
    Script::global(L,"KEY_DELETE", DOKUN_KEY_DELETE);
    Script::global(L,"KEY_HOME", DOKUN_KEY_HOME);
    Script::global(L,"KEY_END", DOKUN_KEY_END);
    Script::global(L,"KEY_PRIOR", DOKUN_KEY_PRIOR);
    Script::global(L,"KEY_NEXT", DOKUN_KEY_NEXT);
    Script::global(L,"KEY_NUM_LOCK", DOKUN_KEY_NUM_LOCK);
    // Numpad
    Script::global(L,"KEY_KP_DIVIDE", DOKUN_KEY_KP_DIVIDE);
    Script::global(L,"KEY_KP_MULTIPLY", DOKUN_KEY_KP_MULTIPLY);
    Script::global(L,"KEY_KP_SUBTRACT", DOKUN_KEY_KP_SUBTRACT);
    Script::global(L,"KEY_KP_ADD", DOKUN_KEY_KP_ADD);
    Script::global(L,"KEY_KP_ENTER", DOKUN_KEY_KP_ENTER);
    Script::global(L,"KEY_KP_DECIMAL", DOKUN_KEY_KP_DECIMAL);
    Script::global(L,"KEY_KP_DELETE", DOKUN_KEY_KP_DELETE);
    Script::global(L,"KEY_KP_INSERT", DOKUN_KEY_KP_INSERT);
    Script::global(L,"KEY_KP_DELETE", DOKUN_KEY_KP_DELETE);
    Script::global(L,"KEY_KP_INSERT", DOKUN_KEY_KP_INSERT);
    Script::global(L,"KEY_KP_END", DOKUN_KEY_KP_END);
    Script::global(L,"KEY_KP_DOWN", DOKUN_KEY_KP_DOWN);
    Script::global(L,"KEY_KP_NEXT", DOKUN_KEY_KP_NEXT);
    Script::global(L,"KEY_KP_LEFT", DOKUN_KEY_KP_LEFT);
    Script::global(L,"KEY_KP_BEGIN", DOKUN_KEY_KP_BEGIN);
    Script::global(L,"KEY_KP_RIGHT", DOKUN_KEY_KP_RIGHT);
    Script::global(L,"KEY_KP_HOME", DOKUN_KEY_KP_HOME);
    Script::global(L,"KEY_KP_UP", DOKUN_KEY_KP_UP);
    Script::global(L,"KEY_KP_PRIOR", DOKUN_KEY_KP_PRIOR);
    Script::global(L,"KEY_KP_0", DOKUN_KEY_KP_0);
    Script::global(L,"KEY_KP_1", DOKUN_KEY_KP_1);
    Script::global(L,"KEY_KP_2", DOKUN_KEY_KP_2);
    Script::global(L,"KEY_KP_3", DOKUN_KEY_KP_3);
    Script::global(L,"KEY_KP_4", DOKUN_KEY_KP_4);
    Script::global(L,"KEY_KP_5", DOKUN_KEY_KP_5);
    Script::global(L,"KEY_KP_6", DOKUN_KEY_KP_6);
    Script::global(L,"KEY_KP_7", DOKUN_KEY_KP_7);
    Script::global(L,"KEY_KP_8", DOKUN_KEY_KP_8);
    Script::global(L,"KEY_KP_9", DOKUN_KEY_KP_9);
    // Symbols
    Script::global(L,"KEY_GRAVE", DOKUN_KEY_GRAVE);	
    Script::global(L,"KEY_ASCIITILDE", DOKUN_KEY_ASCIITILDE);	
    Script::global(L,"KEY_EXCLAMATION", DOKUN_KEY_EXCLAMATION);	
    Script::global(L,"KEY_AT", DOKUN_KEY_AT);	
    Script::global(L,"KEY_NUMBERSIGN", DOKUN_KEY_NUMBERSIGN);	
    Script::global(L,"KEY_DOLLAR", DOKUN_KEY_DOLLAR);	
    Script::global(L,"KEY_PERCENT", DOKUN_KEY_PERCENT);	
    Script::global(L,"KEY_ASCIICIRCUM", DOKUN_KEY_ASCIICIRCUM);	
    Script::global(L,"KEY_AMPERSAND", DOKUN_KEY_AMPERSAND);	
    Script::global(L,"KEY_ASTERISK", DOKUN_KEY_ASTERISK);	
    Script::global(L,"KEY_PARENLEFT", DOKUN_KEY_PARENLEFT);	
    Script::global(L,"KEY_PARENRIGHT", DOKUN_KEY_PARENRIGHT);	
    Script::global(L,"KEY_MINUS", DOKUN_KEY_MINUS);	
    Script::global(L,"KEY_UNDERSCORE", DOKUN_KEY_UNDERSCORE);	
    Script::global(L,"KEY_EQUAL", DOKUN_KEY_EQUAL);	
    Script::global(L,"KEY_PLUS", DOKUN_KEY_PLUS);	
    Script::global(L,"KEY_BRACKETLEFT", DOKUN_KEY_BRACKETLEFT);	
    Script::global(L,"KEY_BRACKETRIGHT", DOKUN_KEY_BRACKETRIGHT);	
    Script::global(L,"KEY_BRACELEFT", DOKUN_KEY_BRACELEFT);	
    Script::global(L,"KEY_BRACERIGHT", DOKUN_KEY_BRACERIGHT);	
    Script::global(L,"KEY_BACKSLASH", DOKUN_KEY_BACKSLASH);
    Script::global(L,"KEY_BAR", DOKUN_KEY_BAR);
    Script::global(L,"KEY_SEMICOLON", DOKUN_KEY_SEMICOLON);
    Script::global(L,"KEY_COLON", DOKUN_KEY_COLON);	
    Script::global(L,"KEY_APOSTROPHE", DOKUN_KEY_APOSTROPHE);
    Script::global(L,"KEY_QUOTE", DOKUN_KEY_QUOTEDBL);
    Script::global(L,"KEY_COMMA", DOKUN_KEY_COMMA);
    Script::global(L,"KEY_PERIOD", DOKUN_KEY_PERIOD);
    Script::global(L,"KEY_LESS", DOKUN_KEY_LESS);
    Script::global(L,"KEY_GREATER", DOKUN_KEY_GREATER);
    Script::global(L,"KEY_SLASH", DOKUN_KEY_SLASH);
    Script::global(L,"KEY_QUESTION", DOKUN_KEY_QUESTION);
    //Script::global(L,"KEY_", DOKUN_KEY_);
    // F1-F12
	Script::global(L,"KEY_F1", DOKUN_KEY_F1);	
	Script::global(L,"KEY_F2", DOKUN_KEY_F2);	
	Script::global(L,"KEY_F3", DOKUN_KEY_F3);	
	Script::global(L,"KEY_F4", DOKUN_KEY_F4);	
	Script::global(L,"KEY_F5", DOKUN_KEY_F5);	
	Script::global(L,"KEY_F6", DOKUN_KEY_F6);	
	Script::global(L,"KEY_F7", DOKUN_KEY_F7);	
	Script::global(L,"KEY_F8", DOKUN_KEY_F8);	
	Script::global(L,"KEY_F9", DOKUN_KEY_F9);	
	Script::global(L,"KEY_F10", DOKUN_KEY_F10);	
	Script::global(L,"KEY_F11", DOKUN_KEY_F11);	
	Script::global(L,"KEY_F12", DOKUN_KEY_F12);	
	// 0-9
	Script::global(L,"KEY_0", DOKUN_KEY_0);
	Script::global(L,"KEY_1", DOKUN_KEY_1);
	Script::global(L,"KEY_2", DOKUN_KEY_2);
	Script::global(L,"KEY_3", DOKUN_KEY_3);
	Script::global(L,"KEY_4", DOKUN_KEY_4);
	Script::global(L,"KEY_5", DOKUN_KEY_5);
	Script::global(L,"KEY_6", DOKUN_KEY_6);
	Script::global(L,"KEY_7", DOKUN_KEY_7);
	Script::global(L,"KEY_8", DOKUN_KEY_8);
	Script::global(L,"KEY_9", DOKUN_KEY_9);	
    //	A-Z
	Script::global(L,"KEY_A", DOKUN_KEY_A);
    Script::global(L,"KEY_B", DOKUN_KEY_B);		
    Script::global(L,"KEY_C", DOKUN_KEY_C);
    Script::global(L,"KEY_D", DOKUN_KEY_D);	
    Script::global(L,"KEY_E", DOKUN_KEY_E);		
    Script::global(L,"KEY_F", DOKUN_KEY_F);		
    Script::global(L,"KEY_G", DOKUN_KEY_G);		
    Script::global(L,"KEY_H", DOKUN_KEY_H);		
    Script::global(L,"KEY_I", DOKUN_KEY_I);		
    Script::global(L,"KEY_J", DOKUN_KEY_J);		
    Script::global(L,"KEY_K", DOKUN_KEY_K);		
    Script::global(L,"KEY_L", DOKUN_KEY_L);		
    Script::global(L,"KEY_M", DOKUN_KEY_M);		
    Script::global(L,"KEY_N", DOKUN_KEY_N);		
    Script::global(L,"KEY_O", DOKUN_KEY_O);		
    Script::global(L,"KEY_P", DOKUN_KEY_P);		
    Script::global(L,"KEY_Q", DOKUN_KEY_Q);		
    Script::global(L,"KEY_R", DOKUN_KEY_R);	
    Script::global(L,"KEY_S", DOKUN_KEY_S);	
    Script::global(L,"KEY_T", DOKUN_KEY_T);		
    Script::global(L,"KEY_U", DOKUN_KEY_U);		
    Script::global(L,"KEY_V", DOKUN_KEY_V);
    Script::global(L,"KEY_W", DOKUN_KEY_W);	
    Script::global(L,"KEY_X", DOKUN_KEY_X);		
    Script::global(L,"KEY_Y", DOKUN_KEY_Y);		
    Script::global(L,"KEY_Z", DOKUN_KEY_Z);	
    //Script::global(L,"KEY_", DOKUN_KEY_);	
   	
	//Script::global (L, "", 0);		
	// -----------------------------------------------------------
#ifndef DOKUN_LUA51
    luaL_newlibtable(L, module);
    luaL_setfuncs   (L, module, 0);	// luaL_newlib(L, module) is equivalent to (luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))
#endif
#ifdef DOKUN_LUA51
	lua_newtable (L);
	luaL_register(L, "dokun", module);
#endif	
	return 1;	// return table library		
}
/////////////////////////////
#ifdef DOKUN_BUILD_LIBRARY
    LUA_API int luaopen_dokun (lua_State *L)
    {
		return dokun::Engine::reg(L);
    }
#endif
/////////////////////////////
#ifdef DOKUN_BUILD_MODULE
    DOKUN_MODULE int luaopen_dokun (lua_State *L)
    {
        return dokun::Engine::reg(L);
    }
#endif
/////////////////////////////
#ifdef DOKUN_BUILD_CONSOLE
    LUA_API int luaopen_dokun (lua_State *L)
    {
	    dokun::Engine::open      (L); // initialize engine by default in lua
		return dokun::Engine::reg(L);
    }
#endif
#ifdef __cplusplus // if c++
}
#endif
