#ifndef DATABASE_JSON_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define DATABASE_JSON_HPP_NEROSHOP
#define JSON_TAG "\033[1;37;2m[json]:\033[0m "

namespace neroshop {
namespace DB {
class Json {
	void json_init();
	void json_parse(const char * json);
	bool json_load(const std::string& file_name);
	bool json_write(const std::string& output_file);
	std::string json_stringify(); // prints out the json file content
	
	void json_start_object();
	void json_end_object();
	void json_array_start(); // can add a value in between attay_start and array_end
	void json_array_end(); // arr_start()   number(1) number(2) number(3) arr_end()  array with 3 numbers		
	void json_key(const std::string& key_name); // outputs a key
	void json_int(int value); // followed by a value
	void json_uint(unsigned int value);
	void json_double(double value);
	void json_bool(bool value);
	void json_string(const std::string& value);
	void json_string(const String& value);
	void json_null();
	void json_set(const std::string& key, const std::string& value);
	void json_set(const std::string& key, int value, bool is_bool = false); // or bool
	void json_set(const std::string& key, double value);
	void json_set(const std::string& key, const String& value);
	void json_set(const std::string& key, void * value);
	
	void json_get(const std::string& key);
	void json_get(const std::string& key, int * value);
	void json_get(const std::string& key, double * value);
	void json_get(const std::string& key, String& value);
	void json_get(const std::string& key, void * value);
	
	bool json_is_object(); // check if document is object
	bool json_has_member(const std::string& key);
	bool json_is_null(const std::string& key);
	bool json_is_boolean(const std::string& key);
	bool json_is_object(const std::string& key);
	bool json_is_array(const std::string& key);
	bool json_is_string(const std::string& key);
	bool json_is_number(const std::string& key);
	bool json_is_integer(const std::string& key);
	bool json_is_double(const std::string& key);
};
}
}

#endif
