#include "../include/progressbar.hpp"

Progressbar::Progressbar() : value(0), range(0, 100), direction(0), 
// color
   foreground_color(0, 51, 102, 1.0), 
   background_color(160, 160, 160, 1.0),
// text   
   label(nullptr),
   text_direction(0),
// outline
    outline(false), 
	outline_width(1.0), 
	outline_antialiased(false),
	outline_color(0, 0, 0, 1.0),
// radius
    radius(0.0), 
// gradient
    gradient(false),
	gradient_color(background_color), // gradient.color1 = backgound_color, gradient.color0 = foreground_color
// border
    border(false)
{
	set_position(0, 0);
	set_size(200, 10);
	set_orientation(0);
}
/////////////
Progressbar::Progressbar(int x, int y) : Progressbar()
{
	set_position(x, y);
}
/////////////
Progressbar::Progressbar(int x, int y, int width, int height) : Progressbar()
{
	set_position(x, y);
	set_size(width, height);
}
/////////////
Progressbar::~Progressbar()
{
    if(label.get()) {
        label.reset();//if(!label.get()) std::cout << "progress_bar label deleted\n";
    }
    std::cout << "progress_bar deleted\n";
}
/////////////
void Progressbar::draw()
{
	if(is_visible())  // is it visible? can it be seen? (will not be seen if not visible)
	{
	    on_draw(); // callback for all gui	(for setting child position)
		if(is_active()) {}// is it active or disabled? (would be darker in color if not active)    
	    double min_val = get_range().x;
	    double max_val = get_range().y;
	
		Renderer::draw_progressbar(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y,
		    foreground_color.x, foreground_color.y, foreground_color.z, foreground_color.w,
		    (!shader.get()) ? GUI::gui_shader : shader.get(),
			min_val, max_val, value, background_color,
			outline, outline_width, outline_color, outline_antialiased,
			radius
			);
		if(!label.get()) return;// as long as label is not nullptr. Does not matter if text is empty, still need to set proper x and y positions
        // set label_position relative to progressbar_position
		if(label->get_alignment() == "left"  ) {label->set_relative_position(0                                     , (get_height() - label->get_height()) / 2);} // keep label_y positioned at center of progressbar at all times
		if(label->get_alignment() == "center") {label->set_relative_position((get_width() - label->get_width()) / 2, (get_height() - label->get_height()) / 2);} // keep label_y positioned at center of progressbar at all times						
		if(label->get_alignment() == "right" ) {label->set_relative_position( get_width() - label->get_width()     , (get_height() - label->get_height()) / 2);} // keep label_y positioned at center of progressbar at all times	
        if(label->get_alignment() == "none"  ) {}
        // draw label manually since there is only one
        label->draw();//Renderer::draw_label(get_text(), x, y, angle, 0.5, 0.5, get_label()->get_font()->get_data(), get_label()->get_color().x, get_label()->get_color().y, get_label()->get_color().z, get_label()->get_color().w);
    }
}
/////////////
void Progressbar::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Progressbar::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int Progressbar::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->draw();
	}
	return 0;
}
/////////////
/////////////
// SETTERS
/////////////
void Progressbar::set_text(const std::string& text)
{
	label->set_string(text);
}
/////////////
int Progressbar::set_text(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_text(lua_tostring(L, 2));
	}		
	return 0;
}
/////////////
void Progressbar::set_label(const dokun::Label& label)
{
    // unique_ptr - faster than shared_ptr and better for use with private class members that do not need to be shared
    std::unique_ptr<dokun::Label> progress_label(&const_cast<dokun::Label&>(label)); // raw pointers cannot be moved so just copy from the normal raw pointer
    this->label = std::move(progress_label); // unique pointers cannot be copied, but can only be moved // "std::unique_ptr::release()" is a similar function but "std::move()" is better of the two
    this->label->set_parent(*this);
}   
/////////////
int Progressbar::set_label(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{	
        dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_getfield(L, 1, "udata");
        if(lua_isuserdata(L, -1))
	    {
		    Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		    progressbar->set_label(*label);
			// set in lua
			lua_pushvalue(L, 2);
			lua_setfield(L, 1, "label");
	    }
	}	
	return 0;
}
/////////////
//////////////
//////////////
void Progressbar::set_foreground_color(unsigned int red, unsigned int green, unsigned int blue) {
    foreground_color = Vector4(red, green, blue, foreground_color.w);
}
void Progressbar::set_foreground_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}
//////////////
void Progressbar::set_foreground_color(const Vector3& color)
{
	set_foreground_color(color.x, color.y, color.z);
}
//////////////
void Progressbar::set_foreground_color(const Vector3& color, double alpha) {
    set_foreground_color(color.x, color.y, color.z, alpha);
}
//////////////
void Progressbar::set_foreground_color(const Vector4& color)
{
	set_foreground_color(color.x, color.y, color.z, color.w);
}
//////////////
int Progressbar::set_foreground_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	int d = lua_tonumber(L, 5);
	if(d == 0) d = 1.0;
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_foreground_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), d);
	}		
	return 0;	
}
//////////////
void Progressbar::set_background_color(unsigned int red, unsigned int green, unsigned int blue) {
    background_color = Vector4(red, green, blue, background_color.w);
}
//////////////
void Progressbar::set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
//////////////
void Progressbar::set_background_color(const Vector3& color)
{
	set_background_color(color.x, color.y, color.z);
}
//////////////
void Progressbar::set_background_color(const Vector3& color, double alpha) {
    set_background_color(color.x, color.y, color.z, alpha);
}
//////////////
void Progressbar::set_background_color(const Vector4& color)
{
	set_background_color(color.x, color.y, color.z, color.w);
}
//////////////
int Progressbar::set_background_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	int d = lua_tonumber(L, 5);
	if(d == 0) d = 1.0;
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_background_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), d);
	}		
	return 0;	
}
//////////////
//////////////
void Progressbar::set_range(double minimum_value, double maximum_value)
{
	range = Vector2(minimum_value, maximum_value);
	// to make sure value does not go below min_val or surpass max_val
	if(value < minimum_value) value = minimum_value;
	if(value > maximum_value) value = maximum_value;
}  
/////////////
void Progressbar::set_range(const Vector2& range)
{
	set_range(range.x, range.y);
}
/////////////
int Progressbar::set_range(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // min_value
	luaL_checktype(L, 3, LUA_TNUMBER); // max_value
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_range(lua_tonumber(L, 2), lua_tonumber(L, 3)); // ERROR FIXED: 10-22-2018 (WAS 1,2 BUT CHANGED TO 2, 3)
	}		
	return 0;
} 
/////////////
void Progressbar::set_minimum_value(double minimum_value)
{
	range.x = minimum_value;
}  
/////////////
int Progressbar::set_minimum_value(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_minimum_value(lua_tonumber(L, 2));
	}		
	return 0;
} 
/////////////
void Progressbar::set_maximum_value(double maximum_value)
{
	range.y = maximum_value;
}  
/////////////
int Progressbar::set_maximum_value(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_maximum_value(lua_tonumber(L, 2));
	}		
	return 0;
} 
/////////////
void Progressbar::set_value(double value)
{	
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value
	this->value = value;
}  
/////////////
int Progressbar::set_value(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_value(lua_tonumber(L, 2));
	}		
	return 0;
} 
/////////////
void Progressbar::set_direction(int direction) // bar fill goes ?  0 = right(default), 1 = left
{
	this->direction = direction;
	// this works, but it is not the appropriate way to set the bar direction, instead do it in the renderer
	//if(direction == 0) set_angle(static_cast<float>(0.0)); // left to right
	//if(direction == 1) set_angle(static_cast<float>(180.0)); // right to left
}  
/////////////
int Progressbar::set_direction(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_direction(lua_tonumber(L, 2));
	}		
	return 0;
}
/////////////
void Progressbar::set_text_direction(int direction) // 0 = center(default), 1 = left, 2 = right
{
	text_direction = direction;
}
/////////////
int Progressbar::set_text_direction(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_text_direction(lua_tonumber(L, 2));
	}		
	return 0;
}
/////////////
void Progressbar::reset()
{
	value = get_minimum_value();//0;
} 
/////////////
int Progressbar::reset(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->reset();
	}		
	return 0;
} 
/////////////
/////////////
/////////////
void Progressbar::set_outline(bool outline)
{
	this->outline = outline;
}
/////////////
int Progressbar::set_outline(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_outline(lua_toboolean(L, 2));
	}
    return 0;	
}
/////////////
void Progressbar::set_outline_width(double width)
{
	outline_width = width;
}
/////////////
int Progressbar::set_outline_width(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_outline_width(lua_tonumber(L, 2));	
	}
	return 0;
}
/////////////
void Progressbar::set_outline_color(unsigned int red, unsigned int green, unsigned int blue) {
    outline_color = Vector4(red, green, blue, outline_color.w);
}
/////////////
void Progressbar::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	outline_color = Vector4(red, green, blue, alpha);
}
/////////////
void Progressbar::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z);
}
/////////////
void Progressbar::set_outline_color(const Vector4& color)
{
	set_outline_color(color.x, color.y, color.z, color.w);
}
/////////////
int Progressbar::set_outline_color(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	int d = lua_tonumber(L, 5);
	if(d == 0) {dokun::Logger("color.w is 0 ?? | progressbar.cpp (143)\n");d=1.0;};
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_outline_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), d);
	}		
	return 0;		
}
/////////////
void Progressbar::set_outline_antialiased(bool antialised)
{
	outline_antialiased = antialised;
}
/////////////
int Progressbar::set_outline_antialiased(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_outline_antialiased(lua_toboolean(L, 2));	
	}
	return 0;
}
/////////////
void Progressbar::set_radius(double radius) {
    this->radius = radius;
}
/////////////
/////////////
/////////////
/////////////
/////////////
// GETTERS
/////////////
std::string Progressbar::get_text()const
{
	if(!get_label())
	{
		return ("");
	}
	return get_label()->get_string();
}     
/////////////
int Progressbar::get_text(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushstring(L, progressbar->get_text().c_str());
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
} 
/////////////
dokun::Label * Progressbar::get_label()const
{
	return label.get();
}
/////////////
int Progressbar::get_label(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "label");
	if(lua_istable(L, -1))
	{
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
}
/////////////
Vector4 Progressbar::get_color()const
{
	return foreground_color;
}       
/////////////
int Progressbar::get_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_color().x);
		lua_pushnumber(L, progressbar->get_color().y);
		lua_pushnumber(L, progressbar->get_color().z);
		lua_pushnumber(L, progressbar->get_color().w);
		return 4;
	}		
    lua_pushnil(L);	
	return 4;
} 
/////////////
Vector4 Progressbar::get_foreground_color()const
{
    return foreground_color;
}
/////////////
int Progressbar::get_foreground_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_foreground_color().x);
		lua_pushnumber(L, progressbar->get_foreground_color().y);
		lua_pushnumber(L, progressbar->get_foreground_color().z);
		lua_pushnumber(L, progressbar->get_foreground_color().w);
		return 4;
	}		
    lua_pushnil(L);	
	return 4;
}
/////////////
Vector4 Progressbar::get_background_color()const
{
    return background_color;
}
/////////////
int Progressbar::get_background_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_background_color().x);
		lua_pushnumber(L, progressbar->get_background_color().y);
		lua_pushnumber(L, progressbar->get_background_color().z);
		lua_pushnumber(L, progressbar->get_background_color().w);
		return 4;
	}		
    lua_pushnil(L);	
	return 4;
}
/////////////
Vector2 Progressbar::get_range()const
{
	return range;
}        
/////////////
int Progressbar::get_range(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_range().x);
		lua_pushnumber(L, progressbar->get_range().y);
		return 2;
	}		
    lua_pushnil(L);	
	return 2;
} 
/////////////
double Progressbar::get_minimum_value()const
{
	return range.x;
}        
/////////////
int Progressbar::get_minimum_value(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_minimum_value());
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
} 
/////////////
double Progressbar::get_maximum_value()const
{
	return range.y;
}       
///////////// 
int Progressbar::get_maximum_value(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_maximum_value());
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
} 
/////////////
double Progressbar::get_value()const
{
	return value;
}      
/////////////  
int Progressbar::get_value(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		lua_pushnumber(L, progressbar->get_value());
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
} 
/////////////
int Progressbar::get_direction()const
{
	return direction;
}    
/////////////    
int Progressbar::get_direction(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		//progressbar->get_();
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
} 
/////////////
int Progressbar::get_text_direction()const
{
	return text_direction;
}     
/////////////
int Progressbar::get_text_direction(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		//progressbar->get_();
		return 1;
	}		
    lua_pushnil(L);	
	return 1;
}
/////////////
/////////////
double Progressbar::get_radius() const {
    return radius;
}
/////////////
/////////////
// BOOLEAN
/////////////
/////////////
int Progressbar::new_(lua_State *L)
{
    lua_settop(L, 0); // clear stack
	// create table (object)
	lua_createtable(L, 0, 0); 
	// set metatable
	lua_getglobal(L, "Progressbar"); 
	lua_setmetatable(L, 1); 
    // set userdata
	Progressbar ** progressbar = static_cast<Progressbar **>(lua_newuserdata(L, sizeof(Progressbar *)));	
	*progressbar = new Progressbar();
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1))
		return 1;
	lua_pushnil(L);
	return 1;
}
/////////////
