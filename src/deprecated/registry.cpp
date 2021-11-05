#include "../include/registry.hpp"

////////////////////
Registry::Registry()
{}
////////////////////
Registry::~Registry()
{
	clear();
}
////////////////////
void Registry::add(void *obj) // & model, & sprite, *(void **)lua_touserdata(L, -1); 
{ 
    // make sure obj is not added to the same registry twice
    //if(std::find(records.begin(), records.end(), obj) != records.end()) {
    //    std::cerr << "obj has already stored in registry" << std::endl;
    //    return;
    //}
    records.push_back (obj);
}
////////////////////
void Registry::add(Registry &registry, void *obj)
{
	registry.add (obj);
}
////////////////////
void Registry::remove(void * obj)
{
	for (unsigned int i = 0; i < records.size(); i++)
	{
		if(records[i] == obj) 
		{	
			// remove from records
			records.erase(records.begin() + i);
		}
	}
	//std::remove_if (records.begin(), records.end(), (std::find(records.begin(), records.end(), obj) != records.end()));//std::remove(records.begin(), records.end(), obj);
}
////////////////////
void Registry::clear(void)
{
	records.clear();
}
////////////////////
////////////////////
void *Registry::get_object(unsigned int index) //Sprite *sprite = (Sprite *)factory->get_object(0); sprite->draw()
{
    if(index > (records.size()-1)) throw std::runtime_error("Registry::get_object(unsigned int): attempt to access invalid index");
	return records[index];
}
////////////////////
unsigned int Registry::get_size()
{
	return records.size();
}
////////////////////
unsigned int Registry::get_size(Registry &registry)
{
	return registry.get_size();
}
////////////////////
int Registry::get_location(void * obj) // works!
{
	for(unsigned int i = 0; i < records.size(); i++) // scan through records 
	{
		if(records[i] == obj) {
			return i; // location
		}
	}
	return -1; // invalid location
}
////////////////////
Registry * Registry::get_seller_registry()
{
	if(!seller_registry) {
		seller_registry = new Registry(); 
	}
	return seller_registry;
}
////////////////////
Registry * Registry::get_item_registry()
{
	if(!item_registry) {
		item_registry = new Registry();
	}
	return item_registry;
}
////////////////////
/*Registry * Registry::get__registry()
{
	if(!_registry)
	{
		_registry = new Registry(); 
	}
	return _registry;
}*/
/////////////
bool Registry::is_empty()
{
	return (records.size() == 0);
}
////////////////////
Registry * Registry::seller_registry (nullptr);
Registry * Registry::item_registry (nullptr);
////////////////////
