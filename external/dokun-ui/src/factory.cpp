#include "../include/factory.hpp"

Factory::Factory()
{}
/////////////
Factory::~Factory()
{
	clean();
}
/////////////
void Factory::store(void *obj) // & model, & sprite, *(void **)lua_touserdata(L, -1); 
{ 
    storage.push_back (obj);
}
/////////////
void Factory::store(Factory &factory, void *obj)
{
	factory.store (obj);
}
/////////////
int Factory::store(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE); // object
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    void * object = *static_cast<void **>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
	    {
	        Factory * factory = *static_cast<Factory **>(lua_touserdata(L, -1));
	        factory->store(object);
	        
			
			lua_getfield(L, 1, "storage");
	        if(lua_istable(L, -1))
			{
			    #ifdef DOKUN_LUA51
			    int size = lua_objlen(L, -1);
			    #endif
			    #ifndef DOKUN_LUA51
			    int size = luaL_len(L, -1);
			    #endif
		        lua_pushvalue(L, 2);
		        lua_rawseti (L, -2, size + 1);
			}
		}
	}
	return 0;
}
/////////////
void Factory::release(void * obj)
{
	for (unsigned int i = 0; i < storage.size(); i++)
	{
		if(storage[i] == obj) 
		{	
			// remove from storage
			storage.erase(storage.begin() + i);
		}
	}
}
/////////////
int Factory::release(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);

    lua_getfield(L, 1, "storage");
    if(lua_istable(L, -1))
    {
		#ifdef DOKUN_LUA51
		int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
		#endif
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i);
			#ifdef DOKUN_LUA51
			if(lua_equal (L, -1, 2))
			#endif
			#ifndef DOKUN_LUA51
			if(lua_compare (L, -1, 2, LUA_OPEQ))
			#endif	
			{
				int index = i ; // get location
				lua_pushnil(L);
				lua_rawseti(L, 3, index); 
				//std::cout << "Found object at index " << index << "\n";
			}
			lua_pop(L, 1);
		}
    }
	return 0;
}
/////////////
void Factory::clean(void)
{
	storage.clear();
}
/////////////
int Factory::clean(lua_State *L)
{
	return (0);
}
/////////////
/////////////
void *Factory::get_object(int index) //Sprite *sprite = (Sprite *)factory->get_object(0); sprite->draw()
{
	return storage[index];
}
/////////////
int Factory::get_object(lua_State *L) 
{
	luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER); // 2
	
    lua_getfield(L, 1, "storage");
	if(lua_istable(L, -1))
	{
        lua_rawgeti(L, -1, lua_tonumber(L, 2)); // returns table found in storage
	    return 1;
	}
	lua_pushnil(L);
	return 1;
}
int Factory::get_userdata(lua_State *L)
{
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{            
        Factory * factory = *static_cast<Factory **>(lua_touserdata(L, -1));
		void ** object = static_cast<void **>(lua_newuserdata(L, sizeof(factory->get_object(lua_tonumber(L, 2)))     ));//factory->get_object(lua_tonumber(L, 2);
	    * object = factory->get_object(lua_tonumber(L, 2)); // returns copy of userdata (with different address) found in storage
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
int Factory::get_size()
{
	return storage.size();
}
/////////////
int Factory::get_size(Factory &factory)
{
	return factory.get_size();
}
/////////////
int Factory::get_size(lua_State *L) // works!
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "storage");
	if(lua_istable(L, -1))
	{
		#ifdef DOKUN_LUA51
		int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
		#endif
		lua_pushnumber(L, size);
		return 1;
	}
	return 0;
}
/////////////
/////////////
int Factory::get_location(void * obj) // works!
{
	for(unsigned int i = 0; i < storage.size(); i++) // scan through storage 
	{
		if(storage[i] == obj)
		{
			return i; // location
		}
	}
	return -1; // invalid location
}
/////////////
int Factory::get_location(lua_State *L) // works!
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 1, "storage");
	if(lua_istable(L, -1))
	{
		#ifdef DOKUN_LUA51
		int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
		#endif // storage size
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i);
			#ifdef DOKUN_LUA51
			if(lua_equal (L, -1, 2))
			#endif
			#ifndef DOKUN_LUA51
			if(lua_compare (L, -1, 2, LUA_OPEQ))
			#endif	 // check if object matches the object pushed into the stack
			{
				lua_pushnumber(L, i);
				return 1;
			}
			lua_pop(L, 1);
		}
	}
	lua_pushnumber(L, -1);
	return 1;
}
/////////////
std::vector<void *> Factory::get_storage() const {
    return storage; // container for objects of any type
}
/////////////
/////////////
Factory * Factory::engine_factory (nullptr);
// container factories
Factory * Factory::window_factory (nullptr);
// graphical factories
Factory * Factory::sprite_factory (nullptr);
Factory * Factory::model_factory (nullptr);
Factory * Factory::texture_factory (nullptr);
Factory * Factory::image_factory (nullptr);
Factory * Factory::object_factory (nullptr);
Factory * Factory::gui_factory (nullptr);
Factory * Factory::font_factory (nullptr);
// audio factories
Factory * Factory::audio_factory (nullptr); 
Factory * Factory::sound_factory (nullptr); 
Factory * Factory::music_factory (nullptr); 
Factory * Factory::voice_factory (nullptr); 
// utility factories
Factory * Factory::scene_factory (nullptr);
Factory * Factory::light_factory (nullptr);
Factory * Factory::camera_factory (nullptr);
Factory * Factory::shader_factory (nullptr); 
Factory * Factory::script_factory (nullptr);
// entity factories - sprite, model, gui
Factory * Factory::entity_factory (nullptr); 
Factory * Factory::component_factory (nullptr); 
/////////////
Factory * Factory::process_factory(nullptr);
/////////////
Factory * Factory::get_engine_factory()
{
	if(!engine_factory)
	{
		engine_factory = new Factory();
	}
	return engine_factory;
}
/////////////
Factory * Factory::get_window_factory()
{
	if(!window_factory)
	{
		window_factory = new Factory();
	}
	return window_factory ;
}
/////////////
Factory * Factory::get_sprite_factory()
{
	if(!sprite_factory) 
	{
	    sprite_factory = new Factory(); 
	}
	return sprite_factory ;
}
/////////////
Factory * Factory::get_model_factory()
{
	if(!model_factory)
	{
		model_factory = new Factory();
	}
	return model_factory;
}
/////////////
Factory * Factory::get_texture_factory()
{
	if(!texture_factory)
	{
		texture_factory = new Factory();
	}
	return texture_factory;
}
/////////////
Factory * Factory::get_image_factory()
{
	if(!image_factory)
	{
		image_factory = new Factory();
	}
	return image_factory;
}
/////////////
Factory * Factory::get_gui_factory()
{
	if(!gui_factory)
	{
		gui_factory = new Factory();
	}
	return gui_factory;
}
/////////////
Factory * Factory::get_font_factory()
{
	if(!font_factory)
	{
		font_factory = new Factory();
	}
	return font_factory;
}
/////////////
Factory * Factory::get_audio_factory()
{
	if(!audio_factory)
	{
		audio_factory = new Factory();
	}
	return audio_factory;	
}
/////////////
Factory * Factory::get_sound_factory()
{
	if(!sound_factory)
	{
		sound_factory = new Factory();
	}
	return sound_factory;	
}
/////////////
Factory * Factory::get_music_factory()
{
	if(!music_factory)
	{
		music_factory = new Factory();
	}
	return music_factory;	
}
/////////////
Factory * Factory::get_voice_factory()
{
	if(!voice_factory)
	{
		voice_factory = new Factory();
	}
	return voice_factory;
}
/////////////
Factory * Factory::get_camera_factory()
{
	if(!camera_factory)
	{
		camera_factory = new Factory();
	}
	return camera_factory;	
}
/////////////
Factory * Factory::get_light_factory()
{
	if(!light_factory)
	{
		light_factory = new Factory();
	}
	return light_factory;	
}
/////////////
Factory * Factory::get_shader_factory()
{
	if(!shader_factory)
	{
		shader_factory = new Factory();
	}
	return shader_factory;	
}
/////////////
Factory * Factory::get_script_factory()
{
	if(!script_factory)
	{
		script_factory = new Factory();
	}
	return script_factory;		
}
/////////////
/////////////
Factory * Factory::get_scene_factory()
{
	if(!scene_factory)
	{
		scene_factory = new Factory();
	}
	return scene_factory;
}
/////////////
Factory * Factory::get_entity_factory()
{
	if(!entity_factory)
	{
		entity_factory = new Factory();
	}
	return entity_factory;
}
/////////////
Factory * Factory::get_component_factory()
{
	if(!component_factory)
	{
		component_factory = new Factory(); 
	}
	return component_factory;
}
/////////////
Factory * Factory::get_process_factory() {
    if(!process_factory)
    {
        process_factory = new Factory();
    }
    return process_factory;
}
/////////////
/////////////
bool Factory::is_empty()
{
	return (storage.size() == 0);
}
/////////////
int Factory::is_empty(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "storage");
	if(lua_istable(L, -1))
	{
        #ifdef DOKUN_LUA51
		int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
		#endif
		lua_pushboolean(L, (size == 0)) ; // lua index starts at 1
		return 1 ;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool Factory::is_stored(void *obj)
{
	for(unsigned int i = 0; i < storage.size(); i++)
	{
		if(storage[i] == obj)
			return true;
	}
	return false;
}
/////////////
int Factory::is_stored(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TTABLE); // 2
	lua_getfield(L, 1, "storage");
	if(lua_istable(L, -1))
	{
		#ifdef DOKUN_LUA51
		int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
		#endif // storage size
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i);
			#ifdef DOKUN_LUA51
			if(lua_equal (L, -1, 2))
			#endif
			#ifndef DOKUN_LUA51
			if(lua_compare (L, -1, 2, LUA_OPEQ))
			#endif // check if object matches the object pushed into the stack
			{
					lua_pushboolean(L, true);
					return 1;
			}
            lua_pop(L, 1); // pop each object until we find the right object
		}
	}
	lua_pushboolean(L, false) ; 
	return 1 ;
}
/////////////
/////////////
int Factory::new_(lua_State *L)
{
	lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// set mt
	lua_getglobal(L, "Factory");
	lua_setmetatable(L, 1);
	// set udata
	Factory **factory = (Factory **)lua_newuserdata(L, sizeof(Factory*));
	*factory = new Factory() ;
	lua_setfield(L, 1, "udata");
	// set storage table
	lua_newtable(L);
	lua_setfield(L, 1, "storage");
	// return table
	if(lua_istable(L, -1))
		return 1;
	lua_pushnil(L);
	return 1;
}
/*
			Sprite * sprite = (Sprite *)get_sprite_factory()->get_object(0); 
			sprite->draw();

*/
