#ifndef STRING_HPP_DOKUN
#define STRING_HPP_DOKUN

#include "platform.hpp"
#include "vector.hpp"
#ifdef __cplusplus
#include <iostream>
#include <string>
#include <cctype> // tolower + toupper
#include <vector>
#ifdef __cplusplus
extern "C" {
#endif	
#include <string.h>
#ifdef __cplusplus
}
#endif
#include <sstream>
#include <fstream>
#include <algorithm>
#include <locale>
//#include <utf8.h>
#include <lua.hpp>
// https://stackoverflow.com/a/24063783
// https://github.com/JuliaStrings/utf8proc
// 
struct String // String manipulation
{
	// constructor
    String(void)                   {}
	String(const String& str)      { value = str.value;         }
	String(const std::string& str) { value = str;               }
	String(const char * c_str)     { value = c_str;             }
	String(char c_str)             { value = to_string(c_str);  }
	String(int number)             { value = to_string(number); }
	String(double number)          { value = to_string(number); }
	String(float number)           { value = to_string(number); }
	String(void * pointer)         { value = to_string(pointer);}
    String(const Vector2& vector)  { value = to_string(vector); }
	String(const Vector3& vector)  { value = to_string(vector); }
	String(const Vector4& vector)  { value = to_string(vector); }		
	// destructor
	~String(void)                  { value.clear();             }
	// conversion
	static std::string to_unicode(const std::string& str) {return "";}
	static std::wstring widen( const std::string& str )
{
    std::wostringstream wstm ;
    const std::ctype<wchar_t>& ctfacet = 
                        std::use_facet< std::ctype<wchar_t> >( wstm.getloc() ) ;
    for( size_t i=0 ; i<str.size() ; ++i ) 
              wstm << ctfacet.widen( str[i] ) ;
    return wstm.str() ;
}

static std::string narrow( const std::wstring& str )
{
    std::ostringstream stm ;
    const std::ctype<char>& ctfacet = 
                         std::use_facet< std::ctype<char> >( stm.getloc() ) ;
    for( size_t i=0 ; i<str.size() ; ++i ) 
                  stm << ctfacet.narrow( str[i], 0 ) ;
    return stm.str() ;
}
	static const std::string to_string(const String& str)     { return str.get_value(); }
	static const std::string to_string(const char * c_str)    { return static_cast<std::string>(c_str); }
	static const std::string to_string(char c_str)            { return std::string(1 , c_str); }
	static const std::string to_string(int number)            { return std::to_string(number); }
	static const std::string to_string(double number)         { return std::to_string(number); }
	static const std::string to_string(float number)          { return std::to_string(number); }
	static const std::string to_string(const Vector2& vector) { return std::to_string(vector.x) + " " + std::to_string(vector.y); }
	static const std::string to_string(const Vector3& vector) { return std::to_string(vector.x) + " " + std::to_string(vector.y) + " " + std::to_string(vector.z); }	
	static const std::string to_string(const Vector4& vector) { return std::to_string(vector.x) + " " + std::to_string(vector.y) + " " + std::to_string(vector.z) + " " + std::to_string(vector.w); }	
	template <typename T>
    static std::string to_string(const T& object) 
	{
        std::ostringstream ss;
        ss << object;
        return ss.str      ();
    }
    template <typename T>
    static std::string to_string_with_precision(const T a_value, const int n)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return out.str();
    } 	
	// wrapper
	std::string str()    const{return this->value;}
	const char * c_str() const{return this->value.c_str();}
	int         length() const{return this->value.length();}
	int         size()   const{return this->value.size();}
	int         empty()  const{return this->value.empty();}
	std::string::iterator end()         noexcept {return this->value.end();}         // do not use get_value() for this function
	std::string::const_iterator end()   const noexcept {return this->value.end();}   // do not use get_value() for this function
	std::string::iterator begin()       noexcept {return this->value.begin();}       // do not use get_value() for this function
	std::string::const_iterator begin() const noexcept {return this->value.begin();} // do not use get_value() for this function
	//std::string& append(const std::string& s){return value.append(s);}
	// setters + getters
	void set_value(const std::string& str) {this->value=str;}
	void set_value(const char * str)       {this->value=str;}
	void set_value(const String& str)      {this->value=str.get_value();}
	std::string get_value() const          {return this->value;}
	// manipulation
	static bool match(const std::string& str_1, const std::string& str_2, bool case_sen=true)
	{
		if(case_sen == false) // not case_sensative
			return (lower(str_1) == lower(str_2));
		return (str_1 == str_2);
	}
	static std::string lower(const std::string& str) // consider: std:: transform(upTxt.begin(), upTxt.end(), upTxt.begin(), ::tolower);
	{
		std::string string0 (str);
		/*for (int i = 0; i < string0.length(); ++i)
			string0[i] = std::tolower(string0[i]);*/
		std::transform(string0.begin(), string0.end(), string0.begin(), [](unsigned char c){ return std::tolower(c); });	
		return string0;
	}
	static std::string upper(const std::string& str)
	{
		std::string string0 (str);
		/*for (int i = 0; i < string0.length(); ++i)
			string0[i] = std::toupper(string0[i]);*/
		std::transform(string0.begin(), string0.end(), string0.begin(), [](unsigned char c){ return std::toupper(c); });	
		return string0;		
	}
	static std::string reverse(const std::string& str) // cat -> tac
	{
		std::string string0 (str);
		for(int i = 0; i < string0.size() / 2; i++)
			std::swap(string0[i], string0[string0.size() - i - 1]);
		return string0;
	}
	static std::vector<std::string> split(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> output;
        char * dup = strdup(str.c_str());
        char * token = strtok(dup, delimiter.c_str());
        while(token != nullptr)
		{
            output.push_back(std::string(token));
            token = strtok(nullptr, delimiter.c_str());
        }
        free(dup);	
        return output;		
	}
	// removing, adding and changing
	static std::string backspace(const std::string& str, int amount) // remove last character in String a number of times
	{
		std::string string0 (str);
		for(int i =1; i <= amount; i++)
			string0.pop_back();
		return string0;
	}
	static std::string append(const std::string& str_1, const std::string& str_2)
	{
		std::string string0 (str_1);
		std::string string1 (str_2);
		return string0.append(string1);
	}
	static std::string swap_first_of(const std::string& str, const std::string& from, const std::string& to) // replaces first occurance of a word from a String with another
	{
		std::string string0 (str);
		size_t start = string0.find(from); // find location of the first 'from'
		if(start == std::string::npos)
			return "error"; // error
		return string0.replace(start, from.length(), to); // from location of 'from' to the length of 'from', replace with 'to'
	}
	static std::string swap_last_of(const std::string& str, const std::string& from, const std::string& to) // replace last occurance of a word from a String with another
	{
		std::string string0 (str);
		size_t start = string0.rfind(from); // find location of the last 'from'
		if(start == std::string::npos)
			return "error"; // error
		return string0.replace(start, from.length(), to); // from location of 'from' to the length of 'from', replace with 'to'
	}	
	static std::string swap_all(const std::string& str, const std::string& from, const std::string& to)
	{
		std::string string0 (str);
		while (string0.find(from) != std::string::npos) // while String contains 'from'
			string0.replace(string0.find(from), from.length(), to); // replace all occurances of 'from' (first-to-last)
		return string0;
	}
	static std::string remove_first_of(const std::string& str, const std::string& key)
	{
        std::string string0 (str);
		size_t start = string0.find(key); // find location of the first 'key'
		if(start == std::string::npos)
			return "error"; // error
		return string0.erase(start, key.length());
	}
	static std::string remove_last_of(const std::string& str, const std::string& key) // remove last occurence of word from String
	{
        std::string string0 (str);
		size_t start = string0.rfind(key); // find location of the last 'key'
		if(start == std::string::npos)
			return "error"; // error
		return string0.erase(start, key.length());		
	}
	static std::string remove_all(const std::string& str, const std::string& key) // remove all occurences of word from String
	{
		std::string string0 (str);
		while(string0.find(key) != std::string::npos)
			string0.erase(string0.find(key), key.length());
		return string0;		
	}
	static bool contains (const std::string& str, const std::string& what)
	{
		return (str.find(what) != std::string::npos);
	}
	static int occurrences(const std::string& str, const std::string& what) // returns number or times a word or letter appears in String
    {
		int o = 0;
		std::size_t start = str.find(what, 0); // find first of
		while(start != std::string::npos)
		{
			o++;
			start = str.find(what, start + 1);
		}
		return o;
	}
	static bool starts_with(const std::string& str, char what) // check if String starts in a letter
	{
		// last char in String
		char first_char = str.at(str.find_first_of(str));
		if(what == first_char)
			return true;
		return false;
	}
	static bool starts_with(const std::string& str, const std::string& what) // check if String starts in a word
	{
        // find first word in String
	    std::size_t first_space_found = str.find_first_of(" "); // first space in String
	    std::string first_word = str.substr(0, first_space_found); // from the start of String to where the space is
        if(what == first_word)
		    return true;
		return false;
	}
	static bool ends_with(const std::string& str, char what) // check if String ends in a letter
	{
		// first char in String
		char last_char = str.at(str.find_last_of(str));
		if(what == last_char)
			return true;
		return false;
	}
	static bool ends_with(const std::string& str, const std::string& what) // check if String ends in a word
	{
		// find last word in String
		std::size_t last_space_found = str.find_last_of(" "); // last space in String	
		std::string last_word = str.substr(last_space_found + 1); // from the start of last space + 1	
		if(what == last_word)
			return true;
		return false;
	}
	static char get_first_character(const std::string& str)
	{
		return str.at(str.find_first_of(str));
	}
	static char get_last_character(const std::string& str)
	{
		return str.at(str.find_last_of(str));
	}
	static std::string get_first_word(const std::string& str)
	{
        std::size_t first_space_found = str.find_first_of(" "); // first space in String
	    std::string first_word = str.substr(0, first_space_found); 
        return first_word;		
	}
	static std::string get_last_word(const std::string& str)
	{
        std::size_t last_space_found = str.find_last_of(" "); // last space in String	
		std::string last_word = str.substr(last_space_found + 1); // from the start of last space + 1			
	    return last_word;
	}
	static std::string get_first_n_characters(const std::string& str, int n) {
	    std::string first_n = str.substr(0, std::min<size_t>(n, str.length()));
	    return first_n;
	}
	static std::string get_last_n_characters(const std::string& str, int n) {
	    std::string last_n = str.substr(str.length() - n);
	    return last_n;
	}	
	static std::vector<unsigned int> get_index(const std::string& str, char what) // finds a character at multiple locations in a string - added 2019-07-31
	{
	    std::vector<unsigned int> what_index;
	    for(int i = 0; i < str.size(); i++) {
            if(str[i] == what) 
            {
                what_index.push_back(i); 
            }
        }
        return what_index;
	}
	static int get_index_first(const std::string& str, char what) // returns index of the first "what" found in string
	{
	    std::string string0 (str);
	    size_t first = string0.find(what); // find location of the first 'what'
	    if(first == std::string::npos)
			return -1; // error
	    return static_cast<int>(first);	
	}
	static int get_index_last (const std::string& str, char what) // returns index of the last "what" found in string
	{
	    std::string string0 (str);
	    size_t last = string0.rfind(what); // find location of the last 'what'
	    if(last == std::string::npos)
			return -1; // error
	    return static_cast<int>(last);
	}
	static double extract(const std::string& str)
	{
		if(!str.empty())
		return std::stod(str); // return atof(str.c_str());
	    return 0;
	}
	static std::string no_alphabet(const std::string& str)
	{
		std::string string0 (str);
		string0.erase(std::remove_if(string0.begin(), 
            string0.end(),
            [](char x){ return std::isalpha(x); }),
        string0.end());
        return string0;		
	}
	static std::string no_digit(const std::string& str)
	{
		std::string string0 (str);
		string0.erase(std::remove_if(string0.begin(), 
            string0.end(),
            [](char x){ return std::isdigit(x); }),
        string0.end());
        return string0;
	}
	static std::string no_spacing(const std::string& str)
	{
		std::string string0 (str);
		string0.erase(std::remove_if(string0.begin(), 
            string0.end(),
            [](char x){return std::isspace(x);}),
        string0.end());
		return string0;
	}
	static std::string no_punctuation(const std::string& str)
	{
		std::string string0 (str);
		string0.erase(std::remove_if(string0.begin(), 
            string0.end(),
            [](char x){return std::ispunct(x);}),
        string0.end());
		return string0;
	}
	static bool is_whitespace_only(const std::string& str) // String contains ONLY white space
	{
		// Empty strings do not contain anything nor whitespace.
		if(str.empty())
			return false;
		// If String is not empty
		if(!str.empty()) {
		    for(int i=0; i < str.length(); i++) { 
			    // String does not ONLY contain whitespace but other characters as well.
			    if(!std::isspace(str[i]) )
			        return false;
		    }
		}
		return true;
	}
	// file-related
	static void write_to_file(const std::string& file_name,const std::string& str, int line_number) // does not work
	{
		std::ifstream rfile(file_name.c_str());
		if (!rfile.is_open()) {
			std::cerr << "failed to open " << file_name << std::endl;
			return;
		}
	    std::stringstream stream;
	    stream << rfile.rdbuf(); // dump file contents	
	    std::vector<std::string> content = split(stream.str(), "\n"); // split each line
	
		for(int i = 0; i < content.size(); i++)
	    { 
	        if(is_whitespace_only(content[i])) // is a whitespace string
			{
				content.erase(content.begin()+i); //std::cout << content[i] << "\n";
			} 
	    }

		rfile.close();
		//-------------------------
		std::ofstream wfile(file_name.c_str(), std::ios::trunc);
		if (!wfile.is_open()) {
			std::cerr << "failed to open " << file_name << std::endl;
			return;
		}
        // make new vector
		std::vector<std::string> content2;
		if(!content.empty())
		{		
			if(content.size() >= line_number)
			{
				// from beginning to end, replace __ with __
				std::replace (content.begin(), content.end(), content[(line_number-1)], str);
				// write String to file @line_number
		        for(int i = 0; i < content.size(); i++){
			        wfile << content[i];  // store String in file
			        //std::cout << "[" << (i + 1) << "] " << content[i] << std::endl; // display
		        }
			}		
			if(content.size() < line_number)
			{
				for(int i =0; i < content.size(); i++)
				{  // copy all from content to content2
				    content2.push_back(content[i]);
				    if(i == (line_number-1))
					{
					    content2.push_back(str);
						break;
					}
				}
				// write String to file @line_number
		        for(int i = 0; i < content2.size(); i++){
			        wfile << content2[i];  // store String in file
			        //std::cout << "[" << (i + 1) << "] " << content[i] << std::endl; // display
		        }
			}	
		}		
		// file has no content
        if(content.empty()){
			for(int i = 0; i < line_number; i++)
			{
				if(i != (line_number - 1)) content.push_back("\n");
				if(i == (line_number - 1)) content.push_back(str );
			}
			// write String to file @line_number
		    for(int i = 0; i < content.size(); i++){
			    wfile << content[i];  // store String in file
			    //std::cout << "[" << (i + 1) << "] " << content[i] << std::endl; // display
		    }
		}
        wfile.close();		
	}
	// word + grammar
	// operation overloads that change the string
	// operators
	String operator = (const std::string& str)
	{ 
		value = str; 
		return (*this);
	}
	String operator = (const char * str)
	{ 
		value = str; 
		return (*this);
	}	
	String operator = (char c_str)
	{ 
		value = String::to_string(c_str);
        return (*this);		
	}		
	String operator = (int s)
	{ 
		value = std::to_string(s);
        return (*this);		
	}		
	String operator = (double s)
	{ 
		value = String::to_string(s);
        return (*this);		
	}	
	String operator = (float s)
	{ 
		value = String::to_string(s); 
		return (*this);
	}		
	String operator = (void * s)
	{ 
		value = String::to_string(s);
		return (*this);
	}
    String operator = (const Vector2& s)
	{ 
		value = String::to_string(s); 
		return (*this);
	}	
	String operator = (const Vector3& s)
	{ 
		value = String::to_string(s);
        return (*this);		
	}	
	String operator = (const Vector4& s)
	{ 
		value = String::to_string(s); 
		return (*this);
	}
	String operator ++ (int)
	{
		value = value + " ";
		return (*this);
	}	
	String operator += (const std::string & str)
	{
		value += str; // value = value + str;
		return (*this);
	}		
	String operator += (const String & str)
	{
		value += str.value; // value = value + str;
		return (*this);
	}
	String operator += (const char * c_str)
	{
		value += c_str; // value = value + c_str;
		return (*this);
	}		
	String operator += (char s)
	{
		value += s; // value = value + s;
		return (*this);
	}	
    String operator += (int s)
	{
		value += String::to_string(s);
		return (*this);
	}			
    String operator += (double s)
	{
		value += String::to_string(s);
		return (*this);
	}	
    String operator += (float s)
	{
		value += String::to_string(s);
		return (*this);
	}
    String operator += (const Vector2& vector)
	{
		value += String::to_string(vector);
		return (*this);
	}
    String operator += (const Vector3& vector)
	{
		value += String::to_string(vector);
		return (*this);
	}
    String operator += (const Vector4& vector)
	{
		value += String::to_string(vector);
		return (*this);
	}	
    String operator += (void * pointer)
	{
		value += String::to_string(pointer);
		return (*this);
	}
	String operator -- (int number)
	{
		value = value.substr(0, value.length() - number);
		return (*this);
	}	
// operation overloads do NOT change the string	
	String operator + (const String & str) const
	{
		return String(value + str.value);//(value + str.value);
	}
	//////////////
	String operator + (const std::string& str) const // returns dokun_string when added with an std::string
	{
		return String(value + str);//(value + str);
	}
	String operator + (const char * c_str) const
	{
		return String(value + c_str);
	}	
	String operator + (char s) const
	{
		return String(value + s);
	}		
    String operator + (int s) const
	{
		return String(value + String::to_string(s));
	}			
    String operator + (double s) const
	{
		return String(value + String::to_string(s));
	}	
    String operator + (float s) const
	{
		return String(value + String::to_string(s));
	}
    String operator + (const Vector2& vector) const
	{
		return String(value + String::to_string(vector));
	}
    String operator + (const Vector3& vector) const
	{
		return String(value + String::to_string(vector));
	}
    String operator + (const Vector4& vector) const
	{
		return String(value + String::to_string(vector));
	}	
    String operator + (void * pointer) const
	{
		return String(value + String::to_string(pointer));
	}
	String operator - (const String& str) const
	{
		return String(value.substr(0, value.length() - str.length()));
	}			
	String operator - (int number) const
	{
		return String(value.substr(0, value.length() - number));
	}
    bool operator < (const String &str) const
	{
		return (value.length() < str.value.length()) ? true : false;
	}	
	//////////////////
    bool operator > (const String &str) const
	{
		return (value.length() > str.value.length()) ? true : false;
	}		
	//////////////////
    bool operator <= (const String &str) const
	{
		return (value.length() <= str.value.length()) ? true : false;
	}	
	//////////////////
    bool operator >= (const String &str) const
	{
		return (value.length() >= str.value.length()) ? true : false;
	}			
	//////////////////
    bool operator == (const String &str) const
	{
		return (value == str.value) ? true : false;
	}		
	//////////////////
    bool operator != (const String &str) const
	{
		return (value != str.value) ? true : false;
	}	
	//////////////////	
	friend std::ostream& operator << (std::ostream& os, const String & str) // String s; std::cout << s ;
	{   
		os << str.get_value();
		return os;
	}
	friend std::istream& operator >> (std::istream& is, const String& str)
	{
	    is >> str;
		return is;
	}
	char operator [] (unsigned int index) const
	{
		return get_value().at(index);
	}
	// properties
	std::string value;
private:
};
#endif
#endif
