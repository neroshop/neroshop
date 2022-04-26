#ifndef ENTITY_HPP_DOKUN
#define ENTITY_HPP_DOKUN

#include "utility.hpp"
#include "types.hpp"
#include "string.hpp"
#include "vector.hpp" // for vector operations
//#include "matrix.hpp" // for matrix operations
//#include "quaternion.hpp"
#include "factory.hpp" // for storing objects
#include "shader.hpp" // for drawing // modern opengl :C :\ :|
#include "component.hpp"
#include "script.hpp"
#include "renderer.hpp" // for rendering

#ifdef __cplusplus // if c++
#include <iostream>
#include <memory> // std::shared_ptr, std::unique_ptr
#include <type_traits>
#include <string> // std::to_string
#include <lua.hpp>

class Entity // or GameObject; base abtract class that acts as parent for Sprites, Models, and GUIs
{
    public:
	    Entity();
		Entity(const std::string& name);
		virtual ~Entity(); // calls derived object's destructors as well
		// normal
	    static Entity * create();                      static int create(lua_State * L);   
		template<class Base>
		static inline Base * create_object()
		{
			// Base must be a class
			if(std::is_class<Base>::value) 
	        {
				// create a dummy_ptr
			    Base dummy_ptr;
				// dummy_ptr must be entity or derived entity object e.g Sprite*, Model*
			    if(!dokun::instanceof<Entity>(&dummy_ptr)) 
			    {
				    std::cerr << "Invalid entity" << std::endl;
				    return nullptr;
			    }
			}
		    return new Base();
		} 
		void destroy();                                static int destroy(lua_State *L);
		static void destroy(Entity * entity);
		virtual void draw();                           static int draw(lua_State *L);// does absolutely nothing; BUT I might add custom rendering feature so users can render however they want 
		virtual void show();                           static int show(lua_State *L);
		virtual void hide();                           static int hide(lua_State *L);
		// virtual functions can be overriden, but pure virtual MUST be overriden by the derived class by force!
		void add_component(const Component& component);     static int add_component(lua_State *L);
		void remove_component(const Component& component);
		void remove_component(const std::string& name);
		// setters
		void set_component(const std::string& name, int value, bool boolean_ = false);        static int set_component(lua_State *L); // update an attached component
		void set_component(const std::string& name, double value);
		void set_component(const std::string& name, const std::string& value);
		void set_component(const std::string& name, const String& value);
		void set_component(const std::string& name, const Vector2& value);
		void set_component(const std::string& name, const Vector3& value);
		void set_component(const std::string& name, const Vector4& value);
		void set_component(const std::string& name, const void * value);
		void set_component(const std::string& name, lua_CFunction value);
		void set_component(const std::string& name, std::function<void (void)> value);
		void set_shader(const Shader& shader);                                static int set_shader(lua_State *L);
		void set_script(lua_State *L, const std::string& file_name);          static int set_script(lua_State *L);
		void set_script(const Script& script); 
        virtual void set_visible(bool visible);		   static int set_visible(lua_State *L);
		void set_polygon_mode(int mode); static int set_polygon_mode(lua_State *L);// wireframe, fill, or points
		// virtual getters
		// getters
		Component * get_component(int index)const;          static int get_component(lua_State *L);// get a component at a index
		Component * get_component(const std::string& name)const;   // get a component by name 
		std::vector<std::shared_ptr<Component>> get_component_container() const;
		Shader * get_shader()const;                         static int get_shader(lua_State *L); // once set, shaders will be attached to the main program; pure entity clasess(excluding Sprites and Models) must have their own shaders. There are no default entity shaders
		Script * get_script() const;                        static int get_script(lua_State *L);   		
		int get_polygon_mode()const;                        static int get_polygon_mode(lua_State *L);
		int get_count(const std::string& what)const;        static int get_count(lua_State *L);
		// getting entities by their "name" component
		static Entity * get_entity_by_name(const std::string& name);
		// other getters
		virtual std::string get_name();
		virtual int get_id();
		// boolean_
		bool is_entity()const;                              static int is_entity(lua_State *L);
		virtual bool is_visible()const;                             static int is_visible(lua_State *L);
		bool has_component(const std::string& name)const;          static int has_component(lua_State *L);
		bool has_component(const Component& component)const;
	protected:
	    //double x, y;		
		// component list
		std::vector<std::shared_ptr<Component>> component_list; // component - additional data members that can be attached to entities
		// shader
		std::unique_ptr<Shader> shader; // shaders - for custom rendering of entities
		// script (each entity needs no more than 1 script)
		std::unique_ptr<Script> script;
		bool visible;
		int mode;
};
/*
// Entities can have components, custom shaders, and scripts 
// Sprite * sprite_ptr = Entity::create_t<Sprite>();

    Model * m = new Model();
	m->destroy();

EXAMPLE USAGE:
    Entity * player = new Entity();
    player->add_component(new Component("name", String("sidpoison")));
	player->add_component(new Component("level", 1));
	player->add_component(new Component("health", 100));
	player->add_component(new Component("mana", 100));
	player->add_component(new Component("power", 1));
	player->add_component(new Component("defense", 1));
	player->add_component(new Component("speed", 1));
	player->add_component(new Component("magic", 0));
	
	
	player->set_script(L,"lib/test.lua");	
	player->get_script(0)->write("player = Sprite:new()");
	
	std::cout << player->get_script(0)->get_file() << "\n";
	std::cout << player->get_script(0)->get_directory() << "\n";
	std::cout << player->get_script(0)->get_content() << "\n";

	
	// dynamic cast
	Entity * entity = new Entity; // pure entity
	dynamic_cast<Sprite*> (entity) << std::endl; // will return nullptr or 0; works only for polymorphic objects
	
	Entity * sprite = new Sprite; // sprite entity
	dynamic_cast<Sprite*> (sprite) << std::endl; // will return the address of the object
	
	
	// Entity to Sprite
	Entity * entity = new Sprite();
	dynamic_cast<Sprite*>(entity)->load("misc/res/bakegami.png");
	
    Window * window = new Window();
	window->create("Sid's Game", 800, 600, 0);
	window->show();

    while (1)
	{
		dynamic_cast<Sprite*>(entity)->draw();
		window->update();
	}		
*/
#endif
#endif
