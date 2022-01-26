#ifndef UTILITY_HPP_DOKUN
#define UTILITY_HPP_DOKUN

#include "platform.hpp"
#include "types.hpp"
#include "factory.hpp"
#include "string.hpp"

#ifdef __cplusplus
#include <iostream> // std::cout
#include <cstdlib>
#include <cassert>
#include <cstring> // std::strdup, std::strtok
#include <string> // std::string, std::stoi
#include <ctime>
#include <chrono> // std::chrono
#include <thread> // std::thread
#include <iomanip> // std::put_time
#include <stdexcept> // std::runtime_error
#include <cctype> // std::tolower, std::toupper
#include <algorithm>
#include <vector> // std::vector
#include <fstream> // std::fstream
#include <sstream> // std::stringstream
#include <random>
#include <iterator>
#include <numeric>
#include <bitset>
#include <type_traits>
#include <cassert>
#include <map>
#include <lua.hpp> // scripting (lua)

#define VARSTR(name) dokun::varstr(#name)
#define DOKUN_ASSERT(fun, mess) assert(fun != 0); std::cerr << "dokun: " << mess << std::endl;
/////////
namespace dokun // utility functions
{
	inline void sleep(unsigned int seconds)
	{
        #ifdef DOKUN_WIN32
		    Sleep(seconds * 1000);
		#endif
		#ifdef __gnu_linux__
		    sleep(seconds);
		#endif
	}
	/////////
	inline double memory() // returns memory usage
	{
		#ifdef DOKUN_WIN32
		#endif
		return (0);
	}
	/////////
	static int memory(lua_State *L)
	{
        lua_pushnumber(L, static_cast<double>(lua_gc (L, LUA_GCCOUNT, 0)) * (0.0009765625)); // returns the current amount of memory (in Megabytes) in use by Lua
		return (1);		
	}
	/////////
	inline int stack(lua_State *L)
	{
		return (lua_gettop(L));		 // returns stack size
	}
	/////////
	inline void runtime_error(std::string what) // throws a runtime error
	{
		throw (std::runtime_error (what.c_str()));
	}
	/////////
	inline void error(std::string what) // generates an error (basic printing to console)
	{
		#ifdef DOKUN_WIN32
		    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		#endif
		#ifdef __gnu_linux__
		    std::cerr << "\033[1;31m"<< what << "\033[0m\n";
		#endif
		std::cerr << "(ERROR): " << what << std::endl;
		#ifdef DOKUN_WIN32
		    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		#endif
	}
	/////////
	static void error(lua_State *L, std::string what) // generates lua error (in C++)
	{
		luaL_error(L, what.c_str());		
	}
	/////////
	static void print(const std::string& what)
	{
		std::clog << "dokun: " << what << std::endl;
	}
	/////////
	static std::string varstr(const std::string& variable)
	{
		return variable;
	}
	/////////
	inline bool chance(double percentage)
	{
		/*
	    std::mt19937 gen;
		gen.seed(std::random_device()());
		
        std::uniform_int_distribution<> dis(1, 100);       		                             
 	    */
		srand (time(nullptr));
		int random = rand() % 100 + 1; // from 1 to 100

		if(percentage >= random/*dis(gen)*/)
		{
			return (true);
		}
		return (false);        		
	}
    /////////
	inline void browse(std::string site_name) // opens browser and goes to specified link
	{
		#ifdef DOKUN_WIN32
		    ShellExecute(nullptr, "open", site_name.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
		#endif
	}
	/////////
	inline void envset(const std::string& name, const std::string& value) // sets environmental path
	{
		#ifdef DOKUN_WIN32
		    SetEnvironmentVariable(name.c_str(), value.c_str());
		#endif
	}
	/////////
	inline std::string platform()
	{
		#ifdef DOKUN_WIN32 
		    return ("Windows");
	    #endif
	    #ifdef __gnu_linux__
		    return ("Linux");	
	    #endif
	    #ifdef __macosx__
		    return ("Mac OS X");	
	    #endif
	    #ifdef __android__
		    return ("Android");	
	    #endif
		return ("No supported platform");
	}
}
namespace dokun // string functions
{  
    // contains
	inline bool contains(const std::string& s, const std::string& what) 
	{
		return (s.find(what) != std::string::npos);
	}
    /////////
	inline int occurances(const std::string& s, const std::string& sub) // number of times a string appears inside of a string
	{
        int occ = 0;
    
		std::size_t pos = s.find(sub, 0);
        while(pos != std::string::npos)
        {
	        occ++;
            pos = s.find(sub, pos + 1);
        }
        return (occ);		
	}
	/////////		
	inline void remove(std::string& str, char key) // removes all occurances of a char
	{
        str.erase(std::remove(str.begin(), str.end(), key), str.end());
	}
	/////////
    inline void swap(std::string& s, const std::string& from, const std::string& to) // find a word(from) inside(str) and replace it with (to) 
	{
		size_t start_pos = s.find(from);
	
		if(start_pos == std::string::npos) // error
		{
			return;
		}
	 	s.replace(start_pos, from.length(), to);				
	}
	/////////
    inline std::string lower(const std::string& s)
	{
		std::string my_string = s;
		for(unsigned int i = 0; i < my_string.length(); ++i) 
        {
            my_string[i] = std::tolower(my_string[i]); // convert to lowercase
        }	
		return my_string;
	}
	/////////	
	inline std::string upper(const std::string& s)
	{
		std::string my_string = s;
		for(unsigned int i = 0; i < my_string.length(); ++i) 
        {
            my_string[i] = std::toupper(my_string[i]); // convert to lowercase
        }	
		return my_string;			
	}	
	/////////
	inline bool match(const std::string& a, const std::string& b, bool case_sensitive = false)
	{
		if(!case_sensitive)
		{
            return (lower(a) == lower(b));		
		}
		return (a == b);
	}
	/////////
    inline std::vector<std::string> split(const std::string& str, char delimiter) 
	{
        std::vector<std::string> output;
        std::stringstream stream(str); // Turn the string into a stream.
        std::string token;
		
        while(std::getline(stream, token, delimiter)) 
		{
            output.push_back(token);
        }
        return (output);
    }	
    /////////
    inline std::vector<std::string> split(const String& str, char delimiter) 
	{
        std::vector<std::string> output;
        std::stringstream stream(str.get_value()); // Turn the string into a stream.
        std::string token;
		
        while(std::getline(stream, token, delimiter)) 
		{
            output.push_back(token);
        }
        return (output);
    }	
	/////////
	inline std::vector<std::string> split(std::string str, std::string delimiter) // most likely will not work
	{
	    size_t pos = 0;
        std::vector<std::string> token;
        while ((pos = str.find(delimiter)) != std::string::npos) 
		{
            token.push_back(str.substr(0, pos));
            str.erase(0, pos + delimiter.length());
        }
		return (token);
    }
    /////////
    inline void split(const std::string& str, char delimiter, std::vector<std::string>& output) 
	{
        auto i = 0;
        auto pos = str.find(delimiter);
        while (pos != std::string::npos) 
		{
            output.push_back(str.substr(i, pos-i));
            i = ++pos;
            pos = str.find(delimiter, pos);
            if (pos == std::string::npos)
			{
                output.push_back(str.substr(i, str.length()));
			}
        }
    }
	/////////
    inline void split(const std::string& str, std::string delimiter, std::vector<std::string>& output) // more C-like than C++
	{
        char * dup = strdup(str.c_str());
        char * token = strtok(dup, delimiter.c_str());
        while(token != nullptr)
		{
            output.push_back(std::string(token));
            token = strtok(nullptr, delimiter.c_str());
        }
        free(dup);
    }		
	/////////
	inline double extract(const std::string& s)
	{	
		return std::stod(s);
	}
	/////////
	inline std::string reverse(const std::string& s) // cat -> tac
	{
		std::string word = s;
		for(int i = 0; i < word.size()/2; i++)
		{
		    std::swap(word[i], word[word.size() - i - 1]);
		}
		return word;
	}	
	/////////
}
/////////
namespace dokun // conversion functions
{ 
    static int char_to_decimal(char c)
	{
		return static_cast<int>(c);
	}
	/////////
	static char decimal_to_char(int decimal)
	{
		return static_cast<char>(decimal);
	}
	/////////
	static std::string char_to_binary(char c)
	{
		return std::bitset<8>(char_to_decimal(c)).to_string();
	}
	/////////
	static char binary_to_char(const std::string& bin)
	{
		std::stringstream stream(bin);
		std::bitset<8> bits;
        stream >> bits;
        return static_cast<char>(bits.to_ulong());
	}
	/////////
	static std::string string_to_binary(const std::string& str)
	{
		std::string binary;
		for(int i = 0; i < str.size(); i++)
		{
			binary += (dokun::char_to_binary(str.at(i)) + " ");
		}
		return binary;
	}
	/////////
	static std::string binary_to_string(const std::string& bin)
	{
		std::string str;
		for(int i = 0; i < dokun::split(bin, " ").size(); i++)
		{
	        str += binary_to_char(dokun::split(bin, " ")[i]);
		}
		return str;
	}
	/////////
	static float pixel_to_ndc(float n) // pixel coordinates to normalized device coordinates
	{
		return 0;
		
	}
	static Vector2 pixel_to_ndc(double x, double y, double width, double height) // requires window width and height; pixel coordinates to normalized device coordinates
	{
		double width_div_2  = width  * 0.5;  // same as  (width / 2 )
        double height_div_2 = height * 0.5;  // same as  (height/ 2 )	
		double aspect = width / height    ;
		
		double dx = (x / width_div_2 - 1.0f) / aspect;
		double dy = 1.0f - y / height_div_2;
		
		return Vector2(dx, dy);
	}	
	static float ndc_to_pixel(float X_NDC, float Y_NDC)
	{
		//s_x * X_NDC + d_x = X_pixel
        //s_y * Y_NDC + d_y = Y_pixel
        return 0;
	}
	/////////
}
/////////
namespace dokun // instance functions
{ 
    template<typename Base, typename T> // works
    inline bool instanceof(const T*) 
    {
        return (std::is_base_of<Base, T>::value);
    }
	/////////
    template<typename Base, typename T> // for polymorphic objects
    inline bool instanceof_p(const T *ptr) 
	{
        return dynamic_cast<const Base*>(ptr) != nullptr;
    }	
    /////////
    inline bool is_nullptr(void * ptr) // works   no more multiple definitions of 'is_null' with 'inline'
    {
	    if(ptr)
	    {
		    return (false);
	    }
	    return (true);
    }
	/////////
	template<class Base>
	inline Base * allocate() // works
	{
		return new Base();
	}
	/////////
	template<class Base>
	inline void deallocate(void * ptr) // works
	{
		if(dokun::is_nullptr(ptr))
		{
			return;
		}
		delete static_cast<Base *>(ptr);
	}	
	/////////
	template<class Base>
	inline bool is_class()
	{
		if(!std::is_class<Base>::value)
		{
			return (false);
		}
		return (true);
	} 
	/////////	
	static void * global_object(const std::string& name, int index) // returns a global object
	{
		if(dokun::lower(name) == "entity")
		{
			return Factory::get_entity_factory()->get_object(index);
		}		
		if(dokun::lower(name) == "sprite")
		{
			return Factory::get_sprite_factory()->get_object(index);
		}
		if(dokun::lower(name) == "model")
		{
			return Factory::get_model_factory()->get_object(index);
		}
		if(dokun::lower(name) == "ui" || dokun::lower(name) == "gui")
		{
			return Factory::get_gui_factory()->get_object(index);
		}
        return nullptr;	
	}
}
/////////
namespace dokun // lua table-object functions
{ 
    template<typename Base>
	inline Base * table_to_object(lua_State *L, const std::string& table) // table with a userdata attached
	{
		// get table
        if(!dokun::contains(table, ".")) // no dots
	    {
	        lua_getglobal(L, table.c_str());
		    if(lua_type(L, -1) != LUA_TTABLE) // not a table?   YOU can only get tables with this function
		    {
				dokun::error(table + "not table");
			    lua_pop(L, 1); // pop it from stack
			    return nullptr;
		    }
	    }
	    // get subtable (contains dots)
        std::vector<std::string> list = dokun::split(table, '.');
	    if(!list.empty())
	    {
		    lua_getglobal(L, list[0].c_str()); // get first table
		    for(unsigned int i = 1; i <= (list.size() - 1); i++)
		    {
			    lua_getfield(L, -1, list[i].c_str()); // get sub_table(s)		
		    }
		    if(lua_type(L, -1) != LUA_TTABLE) // not a table?   YOU can only get tables with this function
		    {
				dokun::error(table + "not table");
			    lua_pop(L, 1); // pop it from stack
			    return nullptr;
		    } 		
	    }		
		
		// get userdata
		lua_getfield(L, -1, "udata");
		if(!lua_isuserdata(L, -1))
		{
			lua_pop(L, 2); // pop table and nil
			return nullptr;
		}			
		Base * object_ptr = static_cast<Base *>(*static_cast<Base **>(lua_touserdata(L, -1)));
		lua_pop(L, 2); // pop table and userdata
		return object_ptr;
	}
}
#endif
#endif
/*
USAGES:
    // convert a lua table to a c++ object
	Sprite * sprite = dokun::table_to_object<Sprite>(L, "sprite");
	if(sprite)
	{
		sprite->set_color(12, 28, 96);
		std::cout << sprite->get_color() << std::endl;
	}	
    // returns a global object
    Sprite * sprite = static_cast<Sprite *>(dokun::object("sprite", 0));
	std::cout << sprite->get_component("name") << std::endl;
	
	dokun::instanceof<Sprite>(ptr);
	dokun::is_null(ptr)
    Sprite * sprite = dokun::allocate<Sprite>();
	

	
	
	std::vector<std::string> token; 
	dokun::split(str, "..", token);
	for(int i = 0; i < token.size(); i++)
		std::cout << token[i] << "\n";
	return 0;	
	
	std::cout << static_cast<int>('A') << "  dec" << std::endl;
	std::cout << std::bitset<8>(static_cast<int>('A')).to_string() << "  bin" << std::endl;	
*/
