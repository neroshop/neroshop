#include "../include/logger.hpp"

dokun::Logger::Logger() : type("default") {} // types: ERROR, WARNING, INFO
////////////
dokun::Logger::Logger(int code, std::string type) // Usage: Logger(1, "error");
{
    this->type   = type;
    this->buffer = print_error(code);
#ifdef DOKUN_LINUX // change console color    
    if(String::lower(type) == "default") std::cerr << "\033[1;37m";
    if(String::lower(type) == "error"  ) std::cerr << "\033[0;31m";
    if(String::lower(type) == "warning") std::cerr << "\033[0;33m";
    if(String::lower(type) == "info"   ) std::cerr << "\033[0;32m";
#endif
    std::cout << print_error(code) << std::endl; // print error //Console::write(print_error(code));
#ifdef DOKUN_LINUX
	std::cout << "\033[0m"; // restore console color
#endif    
}
////////////
dokun::Logger::Logger(const String& message, std::string type) // Usage: Logger("Hello from Logger", "info");
{
    this->type   = type;
    this->buffer = message.str();
#ifdef DOKUN_LINUX // change console color
    if(String::lower(type) == "default") std::cout << "\033[1;37m"; // 37=light gray
    if(String::lower(type) == "error"  ) std::cout << "\033[0;31m"; // 31=red
    if(String::lower(type) == "warning") std::cout << "\033[0;33m"; // 33=yellow
    if(String::lower(type) == "info"   ) std::cout << "\033[0;32m"; // 32=green
    if(String::lower(type) == "special") std::cout << "\033[0;34m"; // 34=blue
    if(String::lower(type) == "dark"   ) std::cout << "\033[0;30m"; // 30=black
#endif
	std::cout << DOKUN_UI_TAG << message << std::endl; // print message on console // std::clog, std::cerr - for errors and info, std::cout - for actual program output //Console::write(message.str());
#ifdef DOKUN_LINUX
	std::cout << "\033[0m"; // restore console color
#endif
}
////////////
dokun::Logger::Logger(const String& message, /*const std::string& function,*/ const std::string& filename, const std::string& line) // set print_out to 0 if using std::cout << operator
{
    logger_ptr->buffer = message.str();
#ifdef DOKUN_LINUX // change console color
    if(String::lower(logger_ptr->type) == "default") std::cout << "\033[1;37m"; // 37=light gray
    if(String::lower(logger_ptr->type) == "error"  ) std::cout << "\033[0;31m"; // 31=red
    if(String::lower(logger_ptr->type) == "warning") std::cout << "\033[0;33m"; // 33=yellow
    if(String::lower(logger_ptr->type) == "info"   ) std::cout << "\033[0;32m"; // 32=green
    if(String::lower(logger_ptr->type) == "special") std::cout << "\033[0;34m"; // 34=blue
    if(String::lower(logger_ptr->type) == "dark"   ) std::cout << "\033[0;30m"; // 30=black
#endif
	std::cout << DOKUN_UI_TAG << message << std::endl; // print message on console // std::clog, std::cerr - for errors and info, std::cout - for actual program output //Console::write(message.str());
#ifdef DOKUN_LINUX
	std::cout << "\033[0m"; // restore console color
#endif
}
//////////// error=red, warning=yellow, info=green or white https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal : Daniel Langr
dokun::Logger::~Logger() {}
////////////
dokun::Logger * dokun::Logger::logger_ptr (new dokun::Logger());
////////////
void dokun::Logger::open() // private: can only be called by engine
{
    dokun::Logger::push("=======================================================================================================");
	dokun::Logger::push("Started dokun " + dokun::Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
	dokun::Logger::push("=======================================================================================================");	
    dokun::Logger::push("\n\n");	
	dokun::Logger::push(String("Running dokun version  ") + String("1.0.0"));
	dokun::Logger::push(String("Running Lua version    ") + String::to_string(LUA_VERSION_MAJOR) + "." + String::to_string(LUA_VERSION_MINOR) + "." + String::to_string(LUA_VERSION_RELEASE));//dokun::Logger::push("\n\n");

#ifdef DOKUN_PRINT_ALL_LIB_VERSIONS // native - these libearies are a core part of dokun
    dokun::Logger::push(String("Using png version      ") + String("1.6"));
    dokun::Logger::push(String("Using ogg version      ") + String("1.3.3"));
    dokun::Logger::push(String("Using vorbis version   ") + String("1.3.6"));
    dokun::Logger::push(String("Using theora version   ") + String("1.1.1"));
    dokun::Logger::push(String("Using freetype version ") + String(FREETYPE_MAJOR).str() + "." + String(FREETYPE_MINOR).str() +  "." + String(FREETYPE_PATCH).str());
    // non-native - these libraries are just extras and can be dropped anytime
    dokun::Logger::push(String("Using jpeg version     ") + String::get_first_character(std::to_string(JPEG_LIB_VERSION));// + String(".x.x"));
    dokun::Logger::push(String("Using gif version      ") + String(GIFLIB_MAJOR).str  () + "." + String(GIFLIB_MINOR).str  () +  "." + String(GIFLIB_RELEASE).str()); // JPEG_LIB_VERSION, TIFFLIB_VERSION
    dokun::Logger::push(String("Using tiff version     ") + String("4.0"));// # 14//dokun::Logger::push(String("   ") + String(""));
    dokun::Logger::push(String("Using flac version     ") + String("1.3.3"));
#endif
    // create dir for logfiles//if(!File::checkdir("log")) File::makedir("log");
	if(!dokun::Logger::save("dokun_log.txt")) Logger("dokun::Logger::save : Could not save log to: dokun_log.txt");// save in append mode
	// pop a number of times after saving first section
#ifndef DOKUN_PRINT_ALL_LIB_VERSIONS
	for(int i = 0; i < 6; i++) dokun::Logger::pop();	
#endif
#ifdef DOKUN_PRINT_ALL_LIB_VERSIONS
	for(int i = 0; i < 15; i++) dokun::Logger::pop();	
#endif
}
////////////	
void dokun::Logger::close() // private: can only be called by engine
{
    dokun::Logger::push("\n=======================================================================================================");
    dokun::Logger::push("Closed dokun " + dokun::Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
    dokun::Logger::push("=======================================================================================================\n");
	if(!dokun::Logger::save("dokun_log.txt")) Logger("dokun::Logger::save : Could not save log to: dokun_log.txt");
}
////////////
void dokun::Logger::push(const String& message) 
{
    //if(logger_ptr->session.size() > 1) 
    //{
    //    std::string prev_str = logger_ptr->session[logger_ptr->session.size() - 1]; 
    //}    
    // push message    
    logger_ptr->session.push_back(message.str());
}
////////////
void dokun::Logger::push(const Logger& logger ) // Usage: dokun::Logger::push(Logger("This is a warning", "warning"));
{
    push(const_cast<Logger&>(logger).buffer);
}
////////////
void dokun::Logger::pop () { logger_ptr->session.pop_back();}
////////////
bool dokun::Logger::save(const String& file_name)
{
	std::ofstream file(file_name.c_str(), std::ios::app);
	if(!file.is_open())
		return false;
	for(int i=0; i < logger_ptr->session.size(); i++) {	
		file << logger_ptr->session[i] << std::endl;
	}
	file.close();
	return true;
}
////////////
std::string dokun::Logger::format(const std::string& fmt)// // returns date and time as a string
{
#if defined(__cplusplus) && (__cplusplus >= 201103L) // C++11 style - Unfortunately a few compilers dont support std::put_time :c
	auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), fmt.c_str());
	return ss.str();
#endif
#if defined(__cplusplus) && (__cplusplus < 201103L)
    std::time_t timer;
    char buffer[26];
    struct std::tm* tm_info;
    std::time(&timer);
    tm_info = std::localtime(&timer);	
	std::strftime(buffer, 26, fmt.c_str(), tm_info);
	return buffer;		
#endif	
}
////////////
void dokun::Logger::box(const String& text, std::string caption, unsigned int type)
	{
#ifdef DOKUN_WIN32
    int id = MessageBox(nullptr, static_cast<LPCTSTR>(text.c_str()), static_cast<LPCTSTR>(caption.c_str()), static_cast<UINT>(type)); 
    switch(id)
	{
		case IDABORT:
		    std::abort();
		    break;
		case IDCANCEL:
		    break;
		case IDCONTINUE:
		    break;
		case IDIGNORE:
		    break;
		case IDNO:
		    break;
		case IDYES:
		    break;			
		case IDOK:
		    break;
		case IDRETRY:
		    system("dokun");
			std::exit(0);
		    break;
		case IDTRYAGAIN:
		    system("dokun");
			std::exit(0);
		    break;
	}		
#endif	
#ifdef __gnu_linux__
#endif	
	}
////////////
// errors
////////////
std::string dokun::Logger::print_error(int error_code)
{
	std::string message;
	if(error_code != 0) // 0 = no error
	{
	    switch(error_code)
		{
			case 1: message = "An unknown error"; break;
			case 2: message = "Initialization failed"; break;
			case 3: message = "Object is null"; break;
			case 4: message = "Out of memory"; break;
			case 5: message = "File not found"; break;
			case 6: message = "File is invalid"; break;
			case 7: message = "File not suported"; break;	
			case 8: message = "File is broken"; break;
			case 9: message = ""; break;
			case 10: message = ""; break;
			case 11: message = ""; break;
			case 12: message = ""; break;
			case 13: message = ""; break;				
			case 14: message = ""; break;
			case 15: message = ""; break;
			case 16: message = ""; break;
			case 17: message = ""; break;
			case 18: message = ""; break;
			case 19: message = ""; break;
			case 20: message = ""; break;			
		}
		logger_ptr->buffer = message;
	}
	return message;
}
////////////
unsigned int dokun::Logger::opengl_error(const std::string& filename, const std::string& line)
{
		unsigned int error_code = 0;
    #ifdef DOKUN_OPENGL	
	    while((error_code = glGetError()) != GL_NO_ERROR)
	    {
			std::string error;
            switch(error_code)
		    {
			    case GL_INVALID_ENUM:
			        error = "OpenGL Error: INVALID ENUM";
			        break;
			    case GL_INVALID_VALUE:
			        error = "OpenGL Error: INVALID VALUE";
			        break;
			    case GL_INVALID_OPERATION:
			        error = "OpenGL Error: INVALID OPERATION";
			        break;
			    case GL_STACK_OVERFLOW:
			        error = "OpenGL Error: STACK OVERFLOW";
			        break;
			    case GL_STACK_UNDERFLOW:
			        error = "OpenGL Error: STACK UNDERFLOW";
			        break;
			    case GL_OUT_OF_MEMORY:
			        error = "OpenGL Error: OUT OF MEMORY";
			        break;
			    case GL_INVALID_FRAMEBUFFER_OPERATION:
			        error = "OpenGL Error: INVALID FRAMEBUFFER OPERATION";
			        break;				
		    }
		    logger_ptr->buffer = error;//Logger(error, filename, line);
	    }
	#endif  	
		return error_code; 
}
////////////
int dokun::Logger::vulkan_error(int result, const std::string& filename, const std::string& line)
{
#ifdef DOKUN_VULKAN
        switch(result)
        {
            case VK_SUCCESS                          : Logger("VK_SUCCESS", filename, line); break;
            case VK_NOT_READY                        : Logger("VK_NOT_READY", filename, line); break;
            case VK_TIMEOUT                          : Logger("VK_TIMEOUT", filename, line); break;
            case VK_EVENT_SET                        : Logger("VK_EVENT_SET", filename, line); break;
            case VK_EVENT_RESET                      : Logger("VK_EVENT_RESET", filename, line); break;
            case VK_INCOMPLETE                       : Logger("VK_INCOMPLETE", filename, line); break;
            case VK_ERROR_OUT_OF_HOST_MEMORY         : Logger("VK_ERROR_OUT_OF_HOST_MEMORY", filename, line); break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY       : Logger("VK_ERROR_OUT_OF_DEVICE_MEMORY", filename, line); break;
            case VK_ERROR_INITIALIZATION_FAILED      : Logger("VK_ERROR_INITIALIZATION_FAILED", filename, line); break;
            case VK_ERROR_DEVICE_LOST                : Logger("VK_ERROR_DEVICE_LOST", filename, line); break;
            case VK_ERROR_MEMORY_MAP_FAILED          : Logger("VK_ERROR_MEMORY_MAP_FAILED", filename, line); break;
            case VK_ERROR_LAYER_NOT_PRESENT          : Logger("VK_ERROR_LAYER_NOT_PRESENT", filename, line); break;
            case VK_ERROR_EXTENSION_NOT_PRESENT      : Logger("VK_ERROR_EXTENSION_NOT_PRESENT", filename, line); break;
            case VK_ERROR_FEATURE_NOT_PRESENT        : Logger("VK_ERROR_FEATURE_NOT_PRESENT", filename, line); break;
            case VK_ERROR_INCOMPATIBLE_DRIVER        : Logger("VK_ERROR_INCOMPATIBLE_DRIVER", filename, line); break;
            case VK_ERROR_TOO_MANY_OBJECTS           : Logger("VK_ERROR_TOO_MANY_OBJECTS", filename, line); break;
            case VK_ERROR_FORMAT_NOT_SUPPORTED       : Logger("VK_ERROR_FORMAT_NOT_SUPPORTED", filename, line); break;
            //case VK_ERROR_FRAGMENTED_POOL            : Logger("VK_ERROR_FRAGMENTED_POOL", filename, line); break;
            //case VK_ERROR_OUT_OF_POOL_MEMORY         : Logger("VK_ERROR_OUT_OF_POOL_MEMORY", filename, line); break;
            //case VK_ERROR_INVALID_EXTERNAL_HANDLE    : Logger("VK_ERROR_INVALID_EXTERNAL_HANDLE", filename, line); break;
            case VK_ERROR_SURFACE_LOST_KHR           : Logger("VK_ERROR_SURFACE_LOST", filename, line); break;
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR   : Logger("VK_ERROR_NATIVE_WINDOW_IN_USE", filename, line); break;
            case VK_SUBOPTIMAL_KHR                   : Logger("VK_SUBOPTIMAL", filename, line); break;
            case VK_ERROR_OUT_OF_DATE_KHR            : Logger("VK_ERROR_OUT_OF_DATE", filename, line); break;
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR   : Logger("VK_ERROR_INCOMPATIBLE_DISPLAY", filename, line); break;
            case VK_ERROR_VALIDATION_FAILED_EXT      : Logger("VK_ERROR_VALIDATION_FAILED_EXT", filename, line); break;
            //case VK_ERROR_INVALID_SHADER_NV          : Logger("VK_ERROR_INVALID_SHADER_NV", filename, line); break;
            //case VK_ERROR_FRAGMENTATION_EXT          : Logger("VK_ERROR_FRAGMENTATION_EXT", filename, line); break;
            //case VK_ERROR_NOT_PERMITTED_EXT          : Logger("VK_ERROR_NOT_PERMITTED_EXT", filename, line); break;
            //case VK_ERROR_OUT_OF_POOL_MEMORY_KHR     : Logger("VK_ERROR_OUT_OF_POOL_MEMORY", filename, line); break;
            //case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: Logger("VK_ERROR_INVALID_EXTERNAL_HANDLE", filename, line); break;
            //case : Logger("", DOKUN_FILE, DOKUN_LINE); break;
}
#endif	
    return result;
}
////////////
/*
const char * GetOpenALErrorString(int errID)
{	
	if (errID == AL_NO_ERROR) return "";
	if (errID == AL_INVALID_NAME) return "Invalid name";
    if (errID == AL_INVALID_ENUM) return " Invalid enum ";
    if (errID == AL_INVALID_VALUE) return " Invalid value ";
    if (errID == AL_INVALID_OPERATION) return " Invalid operation ";
    if (errID == AL_OUT_OF_MEMORY) return " Out of memory like! ";
    
    return " Don't know ";	
}
*/
////////////
void dokun::Logger::lua_err(lua_State *L, const String& message, bool print_out) // posts a Lua error + saves to LOG
{
	    std::ofstream file("log/dokun_log (lua).txt", std::ios::out | std::ios::app);
		lua_Debug info;
        for(int level = 0; level < 5; level++) 
		{
		    lua_getstack(L, level, &info); // get stack
		    lua_getinfo(L, "nSltufL", &info); // store data in debug struct
		    if(level == 0) // level 0 = main function
		    {
		        file << "stdin:1: " << message.str() << std::endl;
		        file << "stack traceback:" << std::endl;
		        file << "\t[C]: in function '" << info.name << "'" << std::endl;
				//file << lua_file_name << std::endl; // file name
		    }
		    if(level > 0) // within other functions
		    {
			    if(info.name) // function_name
			        file << "\t" << info.short_src << ":" << info.currentline << ":" << " in function '" << info.name << "'" << std::endl;
            }
		}
		file << "\tstdin:1: in main chunk" << std::endl;
		file << "\t[C]: in ?" << std::endl << std::endl;			
		// show error
		if(print_out != 0)
		{
		    std::string mes_str = "stdin:1: " + message.str();
		    lua_pushstring(L, mes_str.c_str());
	        lua_error(L);
		}
		// close file
	    file.close();			
}
////////////
int dokun::Logger::lua_error_test(lua_State * L) // // same as function "error" except this saves it to a log
{
	luaL_checktype(L, 1, LUA_TSTRING);
	bool print_out = 1; // print to console
    luaL_optinteger(L, 2, print_out);
	lua_err(L, lua_tostring(L, 1), lua_tointeger(L, 2));
	return 0;
}
////////////
// instances
////////////
void dokun::Logger::print_instance(const String& class_name)
{
	if(String::lower(class_name.str()) == "sprite") std::cout << "Sprite count: " << Factory::get_sprite_factory()->get_size() << std::endl;
	if(String::lower(class_name.str()) == "model" || String::lower(class_name.str()) == "mesh") std::cout << "Model  count: " << Factory::get_model_factory ()->get_size() << std::endl;
	if(String::lower(class_name.str()) == "gui"   || String::lower(class_name.str()) == "ui"  ) std::cout << "GUI    count: " << Factory::get_gui_factory   ()->get_size() << std::endl;
	if(String::lower(class_name.str()) == "image") std::cout   << "Image count " << Factory::get_image_factory()->get_size() << std::endl;
	if(String::lower(class_name.str()) == "texture") std::cout << "Texture count " << Factory::get_texture_factory()->get_size() << std::endl;
	if(String::lower(class_name.str()) == "font") std::cout << "Font count " << Factory::get_font_factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	   
	//push("instance count \n\t")
	//push(Factory::get_._factory()->get_size());
}
////////////
////////////
// dokun::Logger::print(bool enable)
////////////
////////////
////////////
// setters
////////////
void dokun::Logger::set_type(const std::string& type)
{
    logger_ptr->type = type;
}
////////////
////////////
//getters
////////////
std::string dokun::Logger::get_type()
{
    return logger_ptr->type;
}
////////////
std::string dokun::Logger::get_string(int index) { return logger_ptr->session[index];}
////////////
int dokun::Logger::get_index(const String& content) { for (unsigned int i = 0; i < logger_ptr->session.size(); i++) { if(String::match(logger_ptr->session[i], content.str(), true) || String::contains(logger_ptr->session[i], content.str()) ) return i;}return -1;}
////////////
////////////
////////////
////////////
////////////

