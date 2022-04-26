#ifndef LOGGER_HPP_DOKUN
#define LOGGER_HPP_DOKUN

#define DOKUN_FUNCTION  String(__FUNCTION__).str()
#define DOKUN_FILE      String(__FILE__).str()
#define DOKUN_LINE      String(__LINE__).str()
#define DOKUN_DATE      String(__DATE__).str()
#define DOKUN_TIME      String(__TIME__).str()
#define DOKUN_TAG       String("[dokun]: ") +
#define DOKUN_UI_TAG    "\033[1;90m[dokun-ui]:\033[0m "
#define DOKUN_LOGTAG    "dokun: " + DOKUN_FILE.substr(DOKUN_FILE.find_last_of("\\/") + 1) + " (" + DOKUN_LINE + "): "      //+ "function (" + DOKUN_FUNCTION + "): "  // added on 10-1-2018
#define DOKUN_CLASS     String(typeid(*this).name()).str()
#define DOKUN_LOGFILE   "doklog.txt"

#include "platform.hpp" // for platform-specific functions
#include "types.hpp"
#include "utility.hpp"
#include "string.hpp"
#include "system.hpp"
#include "factory.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <typeinfo>
#include <lua.hpp>

namespace dokun {
struct Logger {
	Logger();// {}
	Logger(int code, std::string type = "error"); // prints an error (string) depending on arg "code"
	Logger(const String& message, std::string type = "default");
	Logger(const String& message, /*const std::string& function,*/ const std::string& filename, const std::string& line);// // set print_out to 0 if using std::cout << operator
	~Logger();// {}
	/////////
	static Logger * logger_ptr; // static object can be called by a static class
	/////////
	std::vector<std::string> session = {};// stores all logs from current session
	std::string type;   // type: default, error, warning, info
	std::string buffer; // string
	/////////	
	static void box(const String& text, std::string caption = "dokun", unsigned int type = 0x00000000L);
	static void push(const String& message);
	static void push(const Logger& logger );
	static void pop ();
    /*static void pop(int index) { logger_ptr->session.erase(logger_ptr->session.begin() + index);}
    static void pop(const String& content) {
        for (unsigned int i = 0; i < logger_ptr->session.size(); i++)
        {
            if(String::match(logger_ptr->session[i], content.str(), true) || String::contains(logger_ptr->session[i], content.str()) ) // case sensative by default  (if strings match or string contains specific words)
            {
                logger_ptr->session.erase(logger_ptr->session.begin() + i);
            }
        }
    }*/
    /////////
	static std::string format(const std::string& fmt);// // returns date and time as a string
	/////////
	static bool save(const String& file_name);
	/////////
	static std::string print_error(int error);
	////////////////////
	static unsigned int opengl_error(const std::string& filename, const std::string& line);
	////////////////////
	static int vulkan_error(int result, const std::string& filename, const std::string& line);
	////////////////////
	static void lua_err(lua_State *L, const String& message, bool print_out = 1);
	/////////
	static int lua_error_test(lua_State * L);
	/////////	
	static void print_instance(const String& class_name);
	/////////
	/////////
	static void set_type(const std::string& type);
    /////////
	/////////
	static std::string get_type();
	/////////
    static std::string get_string(int index);// { return logger_ptr->session[index];} // get string at index
    static int         get_index (const String& content);// { for (unsigned int i = 0; i < logger_ptr->session.size(); i++) { if(String::match(logger_ptr->session[i], content.str(), true) || String::contains(logger_ptr->session[i], content.str()) ) return i;}return -1;} // get index from string	
	/////////	all of these classes listed below have access to the private members of Logger
	friend class Engine;
	friend class Entity;
	friend class Model;
	friend class Sprite;
	friend class GUI;
	friend class Texture;
	friend class Renderer;
	friend class FONT;
#ifdef DOKUN_WIN32
	friend class Window;
#endif
#ifdef __gnu_linux__
	friend class XWindow;
#endif
	friend std::ostream& operator << (std::ostream& os, const Logger & l) // String s; std::cout << s ;
	{
	    int last = logger_ptr->session.size() - 1;
		if(last >= 0) 
	        os << logger_ptr->session[last] << "\n";
		return os;
	}
private:
    /////////
	/////////
	static void open();
	/*{
	    Logger::push("=======================================================================================================");
	    Logger::push("Started dokun at " + Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
	    Logger::push("=======================================================================================================");	
        Logger::push("\n\n");	
	    Logger::push(String("Running dokun version  ") + "1.0.0";//String(ENGINE_VERSION));
	    Logger::push(String("Running Lua version    ") + String::to_string(LUA_VERSION_MAJOR) + "." + String::to_string(LUA_VERSION_MINOR) + "." + String::to_string(LUA_VERSION_RELEASE));
	    // create dir for logfiles
	    if(!File::checkdir("log")) File::makedir("log");
	    // save in append mode
	    if(!Logger::save("log/doklog.txt")) Logger("Failure to save log");
	    // pop a number of times after saving first section
	    for(int i = 0; i < 6; i++) Logger::pop();
	}*/	
	static void close();
	/////////	
	static void error_push(const String& error, String file,
	    String function, int line, 
		String date, String time);
	/*{
		dokun_session.push_back("\n");
		dokun_session.push_back("ERROR_START ===========================================================================================");
		dokun_session.push_back(" message  : " + error.str());
		dokun_session.push_back(" file     : " + file.str());
		dokun_session.push_back(" function : " + function.str());
		dokun_session.push_back(" line     : " + String::to_string(line));
		dokun_session.push_back(" date     : " + date.str());
		dokun_session.push_back(" time     : " + time.str());
	#ifdef DOKUN_WIN32
		dokun_session.push_back(" system   : Win32");
	#endif
	#ifdef __gnu_linux__
		dokun_session.push_back(" system   : GNU/Linux");
	#endif	
	    dokun_session.push_back(" user     : " + System::get_user());
		dokun_session.push_back("ERROR_END =============================================================================================");
	}	*/	
	/////////
	static void error_pop();//
	//{
		//for(int i = 0; i < 11; i++)
			//pop();
	//}
};
}
/* 
ex. Logger(String("I" " love " "you"));
    Logger("I love you");
    Logger(String("I") + String(" love ") + String("you"));
	Logger(String("I") + std::string(" love ") + std::string("you"));
		
	String str0 = "I love ";
	String str1 = str0 + "Sid";
    Logger(str0 + "Denice" + " and she is " + 20);	
	
	std::cout << Logger("My name is Sid", 0) << std::endl;
*/
#define Logger_full_push(error) Logger::error_push(error, __FILE__, __FUNCTION__, __LINE__, __DATE__, __TIME__)
#define Logger_full_pop ()      Logger::error_pop()
#define DOKUN_OPENGL_ERROR_CHECK(fn, ln)      Logger::opengl_error(fn, ln)
#define DOKUN_OPENGL_ERROR                    DOKUN_OPENGL_ERROR_CHECK
#define DOKUN_VULKAN_ERROR_CHECK(err, fn, ln) Logger::vulkan_error(err, fn, ln)
#define DOKUN_VULKAN_ERROR                    DOKUN_VULKAN_ERROR_CHECK
#endif
#endif
