#ifndef FACTORY_HPP_DOKUN
#define FACTORY_HPP_DOKUN

#ifdef __cplusplus // if c++
#include <iostream>
#include <vector>
#include <lua.hpp>

class Factory // Object manager - can create, store, and retrieve objects
{
    public:
	    // constructor and destructor
	    Factory();  static int new_(lua_State *L);
		~Factory();
		// normal
		template<class Base>
        static inline Base * create() // latest function added!
		{
			if(std::is_class<Base>::value) // if base is a class
	        {
                return new Base();
			}
		    return nullptr;
		} // usage:  Sprite * sprite = Factory::create<Sprite>();	
		void store(void * obj);  static void store(Factory & factory, void * obj); static int store(lua_State *L);
		void release(void * obj); static void release(Factory & factory, void *obj); static int release(lua_State *L);
	    void clean();             static int clean(lua_State *L);
		// getters
		void *get_object(int index); static void *get_object(Factory & factory, int index); static int get_object(lua_State *L);
		static int get_userdata(lua_State *L);
		int get_size(); static int get_size(Factory & factory);  static int get_size(lua_State *L);
	    int get_location(void *obj);  static int get_location(lua_State *L);
	    std::vector<void *> get_storage() const; // returns the storage object
	    // special getters 
		static Factory * get_engine_factory();
		static Factory * get_state_factory();
		
		static Factory * get_window_factory();
		static Factory * get_sprite_factory();
		static Factory * get_model_factory();
		static Factory * get_texture_factory();
		static Factory * get_gui_factory();
		
		static Factory * get_image_factory();
		// new!
		static Factory * get_font_factory();
		static Factory * get_audio_factory();
		static Factory * get_sound_factory();
		static Factory * get_music_factory();
		static Factory * get_voice_factory();
		static Factory * get_light_factory();
		static Factory * get_camera_factory();
		static Factory * get_shader_factory();
		static Factory * get_script_factory();
		// newer!
		static Factory * get_scene_factory();
		// new!
		static Factory * get_entity_factory();
		static Factory * get_component_factory();
		static Factory * get_process_factory();
		// boolean
	    bool is_empty(); static int is_empty(lua_State *L);
		bool is_stored(void *obj); static int is_stored(lua_State *L);
	private:
	    std::vector<void *> storage;
	    std::vector<std::string> table; // lua table storage;
		// factory objects
		static Factory * engine_factory;
		static Factory * window_factory; // new~!
		// new default factories
		static Factory * sprite_factory;
		static Factory * model_factory;
		static Factory * texture_factory;
		static Factory * image_factory;
		static Factory * object_factory; // object made from class Vertex
		static Factory * gui_factory;		
		// more factories
		static Factory * font_factory;
		static Factory * audio_factory;
		static Factory * sound_factory;
		static Factory * music_factory;	
        static Factory * voice_factory;
		static Factory * light_factory;
		static Factory * camera_factory;
		static Factory * shader_factory;
		static Factory * scene_factory;
		static Factory * script_factory;
		
		static Factory * entity_factory;
		static Factory * component_factory;
		
		static Factory * process_factory;
		/* C++ Usage
		    Sprite * sprite = static_cast<Sprite *>(Factory::get_sprite_factory()->get_object(0)); // get the first object in the sprite_factory
			sprite->draw();
		*/
};
#endif
#endif
