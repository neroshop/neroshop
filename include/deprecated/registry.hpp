#ifndef REGISTRY_HPP_NEROSHOP
#define REGISTRY_HPP_NEROSHOP

#include <iostream>
#include <vector>

class Registry {
public:
	Registry();
	~Registry();
	void add(void * obj);  
	static void add(Registry & registry, void * obj);  //static int add(lua_State *L);
	void remove(void * obj); 
	static void remove(Registry & registry, void *obj); //static int release(lua_State *L);
	void clear();                                        //static int clean(lua_State *L);
	// getters
	void *get_object(unsigned int index); 
	static void *get_object(Registry & registry, int index);
	unsigned int get_size(); 
	static unsigned int get_size(Registry & registry);
	int get_location(void *obj);
	// special getters
	static Registry * get_seller_registry();
	static Registry * get_item_registry();
	// boolean
	bool is_empty(); //static int is_empty(lua_State *L);
	bool is_stored(void *obj); //static int is_stored(lua_State *L);	
	//////////////////////////
	static void register_user(); // check database to see if username is not taken, then generate hash from pw+salt and store it in the database along with the new user's username
	//static void delete_user();
private:
	std::vector<void *> records;
    static Registry * seller_registry;
    static Registry * item_registry;
}; // registry - for sellers; entry - for items

#endif
