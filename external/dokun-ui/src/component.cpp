#include "../include/component.hpp"

Component::Component() : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	// set default name
	set_name("Component" + std::to_string(Factory::get_component_factory()->get_location(this)));
	set_type(DOKUN_COMPONENT_NONE);
}
////////////
Component::Component(const std::string& name) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);	
	set_name(name);
	set_type(DOKUN_COMPONENT_NONE);
}
////////////
Component::Component(const std::string& name, int value, bool boolean_) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value, boolean_);
}
////////////
Component::Component(const std::string& name, double value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);
}
////////////
Component::Component(const std::string& name, const std::string& value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);
}
////////////
Component::Component(const std::string& name, const String& value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);
}
////////////
Component::Component(const std::string& name, const Vector2& value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);	
}
////////////
Component::Component(const std::string& name, const Vector3& value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);	
}
////////////
Component::Component(const std::string& name, const Vector4& value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);
}
////////////
Component::Component(const std::string& name, const void * value) : number(-1), boolean_(-1), pointer(nullptr), vector(-1, -1, -1, -1)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);
}
////////////
Component::Component(const std::string& name, lua_CFunction value)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);	
}
////////////
Component::Component(const std::string& name, std::function<void (void)> value)
{
	Factory::get_component_factory()->store(this);
	set_name(name);
	set_value(value);
}
////////////
Component::~Component()
{
	Factory::get_component_factory()->release(this);
}
////////////
Component * Component::create()
{
	return new Component();
}
////////////
Component * Component::create(const std::string& name)
{
	return new Component(name);
}
////////////
Component * Component::create(const std::string& name, int value, bool boolean_)
{
	return new Component(name, value, boolean_);
}	
////////////
Component * Component::create(const std::string& name, double value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, const std::string& value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, const String& value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, const Vector2& value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, const Vector3& value)
{
    return new Component(name, value);	
}
////////////
Component * Component::create(const std::string& name, const Vector4& value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, const void * value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, lua_CFunction value)
{
	return new Component(name, value);
}
////////////
Component * Component::create(const std::string& name, std::function<void (void)> value)
{
	return new Component(name, value);
}
////////////
int Component::create(lua_State *L)
{
	return 0;
}
////////////
void Component::destroy()
{
	delete this;
}
////////////
int Component::destroy(lua_State *L)
{
	return 0;
}
////////////
void Component::call()
{
	if (function)
	{
	    function();
	}
}
////////////
int Component::call(lua_State *L)
{
	return 0;
}
////////////
////////////
// SETTERS
////////////
void Component::set_name(const std::string& name)
{
	this->name = name;
}
////////////
int Component::set_name(lua_State *L)
{
	return 0;
} 
////////////
void Component::set_value(int value, bool boolean_) // integer or bool
{
	if(!boolean_) // default
	{
		this->number = static_cast<double>(value);
	    set_type(DOKUN_COMPONENT_INTEGER);
	    return;
	}
	if(boolean_)
	{
	    if(value == 0) this->boolean_ = false;
	    if(value != 0) this->boolean_ = true;		
		set_type(DOKUN_COMPONENT_BOOLEAN);
	}
}
////////////
void Component::set_value(double value)
{
	number = value;
	set_type(DOKUN_COMPONENT_DECIMAL);
}
////////////
void Component::set_value(const std::string& value) // string - this function is treated as Component::set_value(void *) unless you use a static cast
{
	this->string = value;
	set_type(DOKUN_COMPONENT_STRING);
}
////////////
void Component::set_value(const String& value) // string - this function is treated as Component::set_value(void *) unless you use a static cast
{
	this->string = value;
	set_type(DOKUN_COMPONENT_STRING);
}
////////////
void Component::set_value(const Vector2& value)
{
	vector = Vector4(value);
	set_type(DOKUN_COMPONENT_VECTOR);
}
////////////
void Component::set_value(const Vector3& value)
{
	vector = Vector4(value);
	set_type(DOKUN_COMPONENT_VECTOR);
}
////////////
void Component::set_value(const Vector4& value)
{
	vector = Vector4(value);
	set_type(DOKUN_COMPONENT_VECTOR);
}
////////////
void Component::set_value(const void * value) // pointer
{
	pointer = const_cast<void *>(value);
	set_type(DOKUN_COMPONENT_POINTER);
}
////////////
void Component::set_value(lua_CFunction value)
{
	lua_function = value;
	set_type(DOKUN_COMPONENT_FUNCTION);
}
////////////
void Component::set_value(std::function<void (void)> value)
{
	this->function = value;
	set_type(DOKUN_COMPONENT_FUNCTION);
}
////////////
void Component::set_type(int type)
{
	this->type = type;
}
////////////
int Component::set_type(lua_State *L)
{
	return 0;
}
////////////
////////////
// GETTERS
////////////
std::string Component::get_name()
{
	return name;
}
////////////
int Component::get_name(lua_State *L)
{
	return 0;
}
////////////
int Component::get_id()
{
	return Factory::get_component_factory()->get_location(this);
}
////////////
int Component::get_id(lua_State *L)
{
	return 0;
}
////////////
int Component::get_type() const
{
	return (type);
}
////////////
int Component::get_type(lua_State *L)
{
	return 0;
}
////////////
void * Component::get_value()
{
	switch(get_type())
	{
		case -1:
		    return to_pointer();
		    break;
		case 0:
		    return &number;
		    break;
		case 1:
		    return &number;
		    break;
		case 2:
		    return &string;
		    break;
		case 3:
		    return &vector;
		    break;
		case 4:
		    return to_pointer();
		    break;
		case 5:
		    return &function;
		    break;			
	}
	return nullptr;
}
int Component::get_value(lua_State *L)
{
	return 0;
}
////////////
////////////
// CONVERSION
////////////
int Component::to_integer() const
{
	return static_cast<int>(number);
}
////////////
double Component::to_double() const
{
	return static_cast<double>(number);
}
////////////
float Component::to_float() const
{
	return static_cast<float>(number);
}
////////////
std::string Component::to_string() const
{
	return string.get_value();
}
////////////
bool Component::to_boolean() const
{
	return boolean_;
}	
////////////
Vector4 Component::to_vector() const
{
	return vector;
}
////////////
void * Component::to_pointer() const
{
	return pointer;
}
////////////
std::function<void (void)> Component::to_function() const
{
	return function;
}
////////////
////////////
////////////
////////////
