#include "../include/progressbar.hpp"

Progressbar::Progressbar() : value(0), range(0, 100), direction(0), 
// color
   foreground_color(0, 51, 102, 255), 
   background_color(160, 160, 160, 255),
// text   
   label(nullptr),
   text_direction(0),
// outline
    outline(false), 
	outline_width(1.0), 
	outline_antialiased(false),
	outline_color(0, 0, 0, 255),
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
Progressbar::Progressbar(int x, int y) : value(0), range(0, 100), direction(0), 
// color
   foreground_color(0, 51, 102, 255), 
   background_color(160, 160, 160, 255),
// text   
   label(nullptr),
   text_direction(0),
// outline
    outline(false), 
	outline_width(1.0), 
	outline_antialiased(false),
	outline_color(0, 0, 0, 255),
// border
    border(false)
{
	set_position(x, y);
	set_size(200, 10);
	set_orientation(0);	
}
/////////////
Progressbar::Progressbar(int x, int y, int width, int height) : value(0), range(0, 100), direction(0), 
// color
   foreground_color(0, 51, 102, 255), 
   background_color(160, 160, 160, 255),
// text   
   label(nullptr),
   text_direction(0),
// outline
    outline(false), 
	outline_width(1.0), 
	outline_antialiased(false),
	outline_color(0, 0, 0, 255),
// border
    border(false)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
}
/////////////
Progressbar::~Progressbar()
{}
/////////////
void Progressbar::draw()
{
	if(is_visible())  // is it visible? can it be seen? (will not be seen if not visible)
	{
		if(is_active()) {}// is it active or disabled? (would be darker in color if not active)    
	    double min_val = get_range().x;
	    double max_val = get_range().y;
		double value = get_value();
	    double x = get_position().x;
		double y = get_position().y;
		double angle = get_angle();
		double scale_x = get_scale().x;
		double scale_y = get_scale().y;
		int width  = get_width();
		int height = get_height();
        int red    = get_color().x;
        int green  = get_color().y;
        int blue   = get_color().z;		
		int alpha  = get_color().w;
	
		Renderer::draw_progressbar(x, y, width, height, angle, scale_x, scale_y,
		    red, green, blue, alpha,
			min_val, max_val, value, background_color,
			outline, outline_width, outline_color, outline_antialiased
			);
		if(label) // as long as label is not nullptr. Does not matter if text is empty, still need to set proper x and y positions
		{
            // set label_position relative to progressbar_position
			if(label->get_alignment() == "left"  ) {label->set_relative_position(0                                     , (get_height() - 10/*label->get_height()*/) / 2);} // keep label_y positioned at center of progressbar at all times
			if(label->get_alignment() == "center") {label->set_relative_position((get_width() - label->get_width()) / 2, (get_height() - 10/*label->get_height()*/) / 2);} // keep label_y positioned at center of progressbar at all times						
			if(label->get_alignment() == "right" ) {label->set_relative_position( get_width() - label->get_width()     , (get_height() - 10/*label->get_height()*/) / 2);} // keep label_y positioned at center of progressbar at all times	
            if(label->get_alignment() == "none"  ) {}
			label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y());
            // NO need to draw label since child GUI are automatically drawn //label->draw();//Renderer::draw_label(get_text(), x, y, angle, 0.5, 0.5, get_label()->get_font()->get_data(), get_label()->get_color().x, get_label()->get_color().y, get_label()->get_color().z, get_label()->get_color().w);
		}
    }
    on_draw(); // callback for all gui	
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
void Progressbar::set_label(const Label& label)
{
	(this)->label = &const_cast<Label&>(label);
	this->label->set_parent(* this); // also set as child
}   
/////////////
int Progressbar::set_label(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{	
        Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
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
void Progressbar::set_foreground_color(int red, int green, int blue, int alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}
//////////////
void Progressbar::set_foreground_color(const Vector3& color)
{
	set_foreground_color(color.x, color.y, color.z);
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
	luaL_optnumber(L, 5, 255);
	int d = lua_tonumber(L, 5);
	if(d == 0) d = 255;
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Progressbar * progressbar = *static_cast<Progressbar **>(lua_touserdata(L, -1));
		progressbar->set_foreground_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), d);
	}		
	return 0;	
}
//////////////
void Progressbar::set_background_color(int red, int green, int blue, int alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
//////////////
void Progressbar::set_background_color(const Vector3& color)
{
	set_background_color(color.x, color.y, color.z);
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
	luaL_optnumber(L, 5, 255);
	int d = lua_tonumber(L, 5);
	if(d == 0) d = 255;
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
	(this)->direction = direction;
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
void Progressbar::set_outline_color(int red, int green, int blue, int alpha)
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
	luaL_optnumber(L, 5, 255);
	int d = lua_tonumber(L, 5);
	if(d == 0) {Logger("color.w is 0 ?? | progressbar.cpp (143)\n");d=255;};
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
Label * Progressbar::get_label()const
{
	return label;
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
