#include "../include/entity.hpp"

Entity::Entity() : visible(true), mode(0), component_list({}), shader(nullptr), script(nullptr)
{
	Factory::get_entity_factory()->store(this);
}
///////////
Entity::Entity(const std::string& name) : Entity()
{
    std::shared_ptr<Component> component = std::make_shared<Component>("name", String(name));//std::shared_ptr<Component>();//std::make_shared is the recommendation for allocating new shared_ptrs
	std::cout << DOKUN_UI_TAG "component 'name' set (" << static_cast<String>(component->get_value()) << ")" << std::endl;
	add_component(*component.get());//(*new Component("name", String(name)));
}
///////////
Entity::~Entity()
{
    // delete (reset) all components
	for(auto components : component_list) {
		if(components.get()) components.reset(); // to-do: erasing each component from the component_list would be better if we want to avoid a double free error
	}
	// delete (reset) shader and script
	if(shader.get()) shader.reset(); // shader object containing multiple shaders and a single program
    if(script.get()) script.reset();
	Factory::get_entity_factory()->release(this);
#ifdef DOKUN_DEBUG
    std::cout << "entity deleted\n";
#endif		
}
///////////
void Entity::draw() // does not do anything
{}
///////////
int Entity::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		entity->draw();
	}
	return 0;
}
///////////
Entity * Entity::create()
{
	return new Entity();
}
///////////
int Entity::create(lua_State * L)
{
	std::string name;
	if(lua_type(L, -1) == LUA_TSTRING) 
	{
	    name = lua_tostring(L, -1);
	}
	lua_settop(L, 0);
	// table
	lua_createtable(L, 0, 0);
	// metatable
	lua_getglobal(L, "Entity");
	lua_setmetatable(L, 1);
	// userdata
	Entity ** entity = static_cast<Entity **>(lua_newuserdata(L, sizeof(Entity*)));
	if(!name.empty())
	{
		*entity = new Entity(name);
		lua_pushstring(L, name.c_str());
		lua_setfield(L, 1, "name");
	}
	else
	{
	    *entity = new Entity();
	}
	lua_setfield(L, 1, "udata");
	// return
	return 1;
}
///////////
void Entity::destroy()
{
	delete this; // if entity is a sprite, will call sprite destructor
}
///////////
void Entity::destroy(Entity * entity)
{
	entity->destroy();
}
///////////
int Entity::destroy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
	    entity->destroy();
	}	
	return 0;
}
///////////
void Entity::show()
{
	set_visible(true);
}
///////////
int Entity::show(lua_State *L)
{
	return 0;
}
///////////
void Entity::hide()
{
	set_visible(false);
}
///////////
int Entity::hide(lua_State *L)
{
	return 0;
}
///////////
void Entity::add_component(const Component& component)
{
    std::shared_ptr<Component> entity_component(&const_cast<Component&>(component));//(&const_cast<Component&>(component));
	component_list.push_back(entity_component);
}
///////////
int Entity::add_component(lua_State *L)
{/*
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checkany(L, 3);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		if(entity->has_component(lua_tostring(L, 2))) {dokun::Logger("Component " + std::string(lua_tostring(L, 2)) + " previously attached to entity"); return 0;}
		if(lua_type(L, 3) == LUA_TNUMBER) entity->add_component(new Component(lua_tostring(L, 2), lua_tonumber(L, 3)));
		if(lua_type(L, 3) == LUA_TSTRING) entity->add_component(new Component(lua_tostring(L, 2), String(lua_tostring(L, 3))));
		if(lua_type(L, 3) == LUA_TBOOLEAN) entity->add_component(new Component(lua_tostring(L, 2), lua_toboolean(L, 3), true));
		if(lua_istable(L, 3)) { lua_getfield(L, 3, "udata"); if(lua_isuserdata(L, -1)) entity->add_component(new Component(lua_tostring(L, 2), *static_cast<void **>(lua_touserdata(L, -1)))); }
		if(lua_isuserdata(L, 3))  entity->add_component(new Component(lua_tostring(L, 2), *static_cast<void **>(lua_touserdata(L, 3))));
		if(lua_isfunction(L, 3)) entity->add_component(new Component(lua_tostring(L, 2), lua_tocfunction(L, 3)));
		if(lua_isthread(L, 3)) entity->add_component(new Component(lua_tostring(L, 2), const_cast<void *>(lua_topointer(L, 3)))); // lua_State * L
		if(lua_isnil(L, 3))       entity->add_component(new Component(lua_tostring(L, 2), static_cast<void *>(nullptr)));
	}
	lua_pushvalue(L, 3);
	lua_setfield(L, 1, lua_tostring(L, 2));	*/
    return 0;	
}
///////////
void Entity::remove_component(const Component& component)
{
	for (int i = 0; i < component_list.size(); i++)
	{
		if(component_list[i].get() == &const_cast<Component&>(component)) {
			component_list.erase(component_list.begin() + i);
		}
	}
}
///////////
void Entity::remove_component(const std::string& name)
{
	for (int i = 0; i < component_list.size(); i++)
	{
		if(component_list[i]->get_name() == name) {	
			component_list.erase(component_list.begin() + i);
		}
	}	
}
///////////
///////////
void Entity::set_visible(bool visible)
{
	this->visible = visible;
}
///////////
int Entity::set_visible(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
        entity->set_visible(lua_toboolean(L, 2));				
	}
	return 0;
}
///////////
void Entity::set_component(const std::string& name, int value, bool boolean_)
{
    get_component(name)->set_value(value, boolean_);
}
///////////
void Entity::set_component(const std::string& name, double value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, const std::string& value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, const String& value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, const Vector2& value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, const Vector3& value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, const Vector4& value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, const void * value)
{
    get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, lua_CFunction value)
{
	get_component(name)->set_value(value);
}
///////////
void Entity::set_component(const std::string& name, std::function<void (void)> value)
{
    get_component(name)->set_value(value);	
}
///////////
int Entity::set_component(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checkany(L, 3);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
	    if(!entity->has_component(lua_tostring(L, 2))) {dokun::Logger("Component " + std::string(lua_tostring(L, 2)) + " not attached to entity"); return 0;}
		if(lua_type(L, 3) == LUA_TNUMBER) entity->set_component(lua_tostring(L, 2), lua_tonumber(L, 3));
		if(lua_type(L, 3) == LUA_TSTRING) entity->set_component(lua_tostring(L, 2), String(lua_tostring(L, 3)));
		if(lua_type(L, 3) == LUA_TBOOLEAN) entity->set_component(lua_tostring(L, 2), lua_toboolean(L, 3), true);
		if(lua_istable(L, 3)) { lua_getfield(L, 3, "udata"); if(lua_isuserdata(L, -1)) entity->set_component(lua_tostring(L, 2), *static_cast<void **>(lua_touserdata(L, -1))); }
		if(lua_isuserdata(L, 3))  entity->set_component(lua_tostring(L, 2), *static_cast<void **>(lua_touserdata(L, 3)));
		if(lua_isfunction(L, 3)) entity->set_component(lua_tostring(L, 2), lua_tocfunction (L, 3));
		if(lua_isthread(L, 3)) entity->set_component(lua_tostring(L, 2), const_cast<void *>(lua_topointer(L, 3))); // lua_State * L
		if(lua_isnil(L, 3))       entity->set_component(lua_tostring(L, 2), static_cast<void *>(nullptr));
	}
	lua_pushvalue(L, 3);
	lua_setfield(L, 1, lua_tostring(L, 2));
	return 0;
}
///////////
void Entity::set_shader(const Shader& shader)
{	
    std::unique_ptr<Shader> entity_shader(&const_cast<Shader&>(shader));
	this->shader = std::move(entity_shader);
}
///////////
int Entity::set_shader(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
    if(lua_isuserdata(L, -1)) 
	{	
		Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1)) 
	    {
		    Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
            entity->set_shader(*shader);
	    }
	}
	return 0;
}
///////////
// maybe remove this function ?
void Entity::set_script(lua_State *L, const std::string& file_name)
{
	Script * script = new Script(L, file_name);
	set_script(*static_cast<Script *>(script));
}
///////////
void Entity::set_script(const Script& script)
{
    std::unique_ptr<Script> entity_script(&const_cast<Script&>(script));
	this->script = std::move(entity_script);
}
///////////
int Entity::set_script(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
    if(lua_istable(L, 2)) 
	{	
        lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1)) 
	    {
		    Script * script = *static_cast<Script **>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1)) 
	        {
		        Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
                entity->set_script(*static_cast<Script *>(script));
                // set script in Lua
			    lua_pushvalue(L, 2);
		        lua_setfield(L, 1, "script");	        				
	        }	
		}			
	}
    if(lua_type(L, 2) == LUA_TSTRING)
	{
		lua_setfield(L, 1, "script");
	}		
	return 0;
}
///////////
////////////
void Entity::set_polygon_mode(int polygon_mode)
{
	mode = polygon_mode;
}
////////////
int Entity::set_polygon_mode(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{	
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
        entity->set_polygon_mode(static_cast<int>(lua_tonumber(L, 2)));			
	}
	return 0;	
}
///////////
///////////
Component * Entity::get_component(int index) const
{
    if(index > (component_list.size() - 1)) throw std::out_of_range("Entity::get_component: invalid or out of range container index");
    return component_list[index].get();
}
///////////
Component * Entity::get_component(const std::string& name) const
{
	for(auto components : component_list) {
		if(components->get_name() == name) 
			return components.get();
	}
    return nullptr;	
}
///////////
int Entity::get_component(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, lua_tostring(L, 2));
	return 1;
}
///////////
std::vector<std::shared_ptr<Component>> Entity::get_component_container() const
{
	return component_list;
}
///////////
Shader * Entity::get_shader()const
{
	return shader.get();
}
///////////
int Entity::get_shader(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "shader");
	if(lua_istable(L, -1)) 
	{
		lua_rawgeti (L, -1, lua_tonumber(L, 2));
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
///////////
Script * Entity::get_script() const
{
    return script.get();
}
///////////
int Entity::get_script(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "script");
	return 1;
}
///////////
int Entity::get_polygon_mode()const
{
	return mode;
}
///////////
int Entity::get_polygon_mode(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushinteger(L, entity->get_polygon_mode());
		return 1;
	}			
	lua_pushinteger(L, 0);
	return 1;
}
///////////
int Entity::get_count(const std::string& what)const
{
	if(String::lower(what).find("component") != std::string::npos)
		return component_list.size();
	if(String::lower(what).find("shader") != std::string::npos)
		return (shader.get() != nullptr); // should return 1 ... I think :O
	if(String::lower(what).find("script") != std::string::npos)
		return (script.get() != nullptr);
	return 0;
}
///////////
int Entity::get_count(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushinteger(L, entity->get_count(lua_tostring(L, 2)));
		return 1;
	}		
	lua_pushinteger(L, 0);
	return 1;
}
///////////
Entity * Entity::get_entity_by_name(const std::string& name) {
    for(auto entities : Factory::get_entity_factory()->get_storage()) {
        Entity * entity = static_cast<Entity *>(entities);
        if(!entity->has_component("name")) continue; // skip entities that do not have a "name" component
        if(entity->get_component("name")->to_string() == name) {//std::cout << DOKUN_UI_TAG "found entity with name: " << entity->get_component("name")->to_string() << std::endl;
            return entity;
        }
    }
    return nullptr;
}
///////////
///////////
///////////
///////////
///////////
std::string Entity::get_name()
{
	if(has_component("name")) {
		return get_component("name")->to_string();
	}
    return "Entity" + std::to_string(Factory::get_entity_factory()->get_location(this));
}
///////////
int Entity::get_id()
{
    return Factory::get_entity_factory()->get_location(this);
}
///////////
///////////
bool Entity::is_entity()const
{
	return ((this != nullptr) && (dokun::instanceof<Entity>(this) != 0));
}
///////////
int Entity::is_entity(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushboolean(L, entity->is_entity());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
///////////
bool Entity::is_visible()const
{
	return (visible == true);
}
///////////
int Entity::is_visible(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushboolean(L, entity->is_visible());
		return 1;
	}	
	lua_pushboolean(L, false);
    return 1;
}
///////////
bool Entity::has_component(const std::string& name)const
{
	for(auto components : component_list) {
		if(components->get_name() == name) 
			return true;
	}
    return false;	
}
///////////
bool Entity::has_component(const Component& component)const
{
	for(auto components : component_list) {
		if(components.get() == &const_cast<Component&>(component)) 
			return true;
	}
    return false;		
}
///////////
int Entity::has_component(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, lua_tostring(L, 2));
	lua_pushboolean(L, (lua_isnil(L, -1) == 0));
	return 1;
}
///////////
/*
static void entity_to_table(lua_State *L, Entity * object, bool reset) // , bool reset = false)
    {
		std::string fn;
		fn = std::string("entity_" + std::to_string(Factory::get_entity_factory()->get_location(object)) + ".lua").c_str();
		std::ios_base::openmode mode;
		if(reset == false)
		{
			mode = std::ios::app;
		}
		if(reset == true)
		{
			mode = std::ios::trunc;
		}
		std::ofstream file(fn.c_str(), mode | std::ios::binary);
		if (!file.is_open()) 
	    {
			dokun::Logger("Could not generate lua");
			return;
		}
		file << "-- ";
		file.write(fn.c_str(), fn.length());
		
		file << "\n" << fn.substr(0, fn.find(".")) << " = \n{\n";
		//------------
		
		std::vector<std::pair<std::string, std::string>> data = {};
		
		if(dynamic_cast<Sprite *>(object)->is_sprite())
	    {
		    data.push_back(std::make_pair("position", String(static_cast<Sprite *>(object)->get_position()).str()));
			data.push_back(std::make_pair("angle", String(static_cast<Sprite *>(object)->get_angle()).str()));
			data.push_back(std::make_pair("scale", String(static_cast<Sprite *>(object)->get_scale()).str()));
			data.push_back(std::make_pair("texture", String(static_cast<Sprite *>(object)->get_texture()).str() ));
			data.push_back(std::make_pair("size", String(static_cast<Sprite *>(object)->get_size()).str()));
			data.push_back(std::make_pair("color", String(static_cast<Sprite *>(object)->get_color()).str()));
			data.push_back(std::make_pair("parent", "\"Sprite\""));
		}	
		if(dynamic_cast<Model *>(object)->is_model())
	    {
			data.push_back(std::make_pair("position", String(static_cast<Model *>(object)->get_position()).str()));
			data.push_back(std::make_pair("rotation", String(static_cast<Model *>(object)->get_rotation()).str()));
			data.push_back(std::make_pair("scale", String(static_cast<Model *>(object)->get_scale()).str()));
			data.push_back(std::make_pair("texture", "\"??\""));
			data.push_back(std::make_pair("size", String(static_cast<Model *>(object)->get_size()).str()));
		    data.push_back(std::make_pair("color", String(static_cast<Model *>(object)->get_color()).str()));
			data.push_back(std::make_pair("parent", "\"Model\""));
		}
		if(dynamic_cast<GUI *>(object)->is_gui())
	    {	
	        data.push_back(std::make_pair("size", String(static_cast<GUI *>(object)->get_size()).str()));
			data.push_back(std::make_pair("position", String(static_cast<GUI *>(object)->get_position()).str()));
			data.push_back(std::make_pair("orientation", String(static_cast<GUI *>(object)->get_orientation()).str()));
			data.push_back(std::make_pair("parent", String(static_cast<GUI *>(object)->get_parent()).str()));
		    data.push_back(std::make_pair("visible", String::to_string(static_cast<GUI *>(object)->is_visible())));
			data.push_back(std::make_pair("active", String::to_string(static_cast<GUI *>(object)->is_active()))); 
			data.push_back(std::make_pair("draggable", String(static_cast<GUI *>(object)->is_draggable()).str()));
			data.push_back(std::make_pair("droppable", String(static_cast<GUI *>(object)->is_droppable()).str()));
			data.push_back(std::make_pair("sizeable", String(static_cast<GUI *>(object)->is_resizeable()).str()));
			data.push_back(std::make_pair("sortable", String(static_cast<GUI *>(object)->is_sortable()).str()));
            data.push_back(std::make_pair("parent", "\"GUI\""));			
		}
		//------------
		for(int i = 0; i < data.size(); i++)
		{
			std::string key   = data[i].first;
			std::string value = data[i].second; 
			
			if(value.find(" ") == std::string::npos) // single value
			{
			    file << "    " + key + " = " + value << ",\n";
			}
            if(value.find(" ") != std::string::npos) // multiple value
			{
				std::vector<std::string> value_II = String::split(value, ' ');
				file << "    "; // four space
				file << key + " = "; // key assign
				file << "{ "; // first sub_table bracket
				for(int j =0; j < value_II.size(); j++)
				{
					file << value_II[j] << ", ";
				}			
			    file << "},\n"; // last sub_table bracket
			}			
		}	
        // components
		for(int j = 0; j < object->get_count("component"); j++)
		{
			std::string value;
			if(object->get_component(j)->get_type() == 0) value = String::to_string(object->get_component(j)->to_integer());				
			if(object->get_component(j)->get_type() == 1) value = String::to_string(object->get_component(j)->to_double());
			if(object->get_component(j)->get_type() == 2) value = "\"" + object->get_component(j)->to_string() + "\"";				
			if(object->get_component(j)->get_type() == 3) value = String::to_string(object->get_component(j)->to_vector().x) + (", ") + String::to_string(object->get_component(j)->to_vector().y) + (", ") + String::to_string(object->get_component(j)->to_vector().z) + (", ") + String::to_string(object->get_component(j)->to_vector().w);
			if(object->get_component(j)->get_type() == 4)
			{
				if(object->get_component(j)->to_pointer() != nullptr) value = "userdata: " + String::to_string(object->get_component(j)->to_pointer());
				if(object->get_component(j)->to_pointer() == nullptr) value = "null"; // nullptr
			}
			if(object->get_component(j)->get_type() == 5) value = "function: " + String::to_string(&object->get_component(j)->function); // function
            if(object->get_component(j)->get_type() == 10) // boolean_
			{
				if(object->get_component(j)->to_boolean_() != 0) value = "true";
				if(object->get_component(j)->to_boolean_() == 0) value = "false";
			}					
			file << "    " << object->get_component(j)->get_name() << " = " << value << ",\n";
		}
		//------------
		//------------
		//------------
		file << "}";
		file.close();
    }
*/	
