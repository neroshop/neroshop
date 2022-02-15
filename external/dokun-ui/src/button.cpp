#include "../include/button.hpp"

Button::Button() : color(0, 51, 102, 1.0), tint_factor(0.05), shade_factor(0.25), depth(1), label(nullptr), image(nullptr), fill(true), shadow(false), border(false), type("polygon"),
    old_color(color),//(0, 51, 102, 1.0), 
	// outline (is the same as a border)
    outline(false),
	outline_width(2.0),
	outline_color(255, 255, 255, 1.0),
	outline_antialiased(false),
// border properties - remove this asap
    border_color(255, 255, 255),
    border_size(1, 1),
    border_thickness(0),
    border_style(0),
    border_radius(0),
// shadow properties
// gradient properties
    gradient(false),//(true),
	gradient_color(color)
{
	set_position(0, 0);
	set_width(100);
	set_height(50);	
	set_orientation(0);
    label = new dokun::Label();
}
/////////////
Button::Button(const std::string& text) : color(0, 51, 102, 1.0), tint_factor(0.05), shade_factor(0.25), depth(1), label(nullptr), image(nullptr), fill(true), shadow(false), border(false), type("polygon")
{
	set_position(0, 0);
	set_width(100);
	set_height(50);	
	set_orientation(0);
	label = new dokun::Label();
}
/////////////
Button::Button(int x, int y) : color(0, 51, 102, 1.0), tint_factor(0.05), shade_factor(0.25), depth(1), label(nullptr), image(nullptr), fill(true), shadow(false), border(false), type("polygon")
{
    set_position(x, y);
	set_width(100);
	set_height(50);
	set_orientation(0);
	label = new dokun::Label();
}
/////////////
Button::Button(int x, int y, int width, int height) : color(0, 51, 102, 1.0), tint_factor(0.05), shade_factor(0.25), depth(1), label(nullptr), image(nullptr), fill(true), shadow(false), border(false), type("polygon")
{
	set_position(x, y);
	set_width(width);
	set_height(height);
	set_orientation(0);
	label = new dokun::Label();
}  
/////////////
Button::Button(const std::string& text, int x, int y) : color(0, 51, 102, 1.0), tint_factor(0.05), shade_factor(0.25), depth(1), label(nullptr), image(nullptr), fill(true), shadow(false), border(false), type("polygon")
{
	set_position(x, y);
	set_width(100);
	set_height(50);
	set_orientation(0);
	label = new dokun::Label();
}
/////////////
Button::Button(const std::string& text, int x, int y, int width, int height) : color(0, 51, 102, 1.0), tint_factor(0.05), shade_factor(0.25), depth(1), label(nullptr), image(nullptr), fill(true), shadow(false), border(false), type("polygon")
{
	set_position(x, y);
	set_width(width);
	set_height(height);	
	set_orientation(0);
	label = new dokun::Label();
}
/////////////
Button::~Button(void)
{
    // delete image
    if(image) {
        delete image;
        image = nullptr; // its good practice to set all deleted objects to null regardless of what anybody says
    }
    // delete label
    if(label) {
        delete label;
        label = nullptr;
    }
    std::cout << "button deleted\n";
}
/////////////
void Button::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(is_visible()) 
	{
		//on_hover();
		//on_press();
		on_mouse_interact();
	    std::string text = get_text();
        // Draw button
	    Renderer::draw_button(get_x(), get_y(), get_width(), get_height(), 
		    get_angle(), get_scale().x, get_scale().y, 
			get_color().x, get_color().y, get_color().z, get_color().w,
            // outline
            outline, outline_width, outline_color, outline_antialiased,
            // gradient
            gradient,
		    gradient_color
		);
		// Draw image (over button)
		if(image != nullptr)
		{
            int image_width  = (!image->is_resized()) ? image->get_width () : image->get_width_scaled ();
			int image_height = (!image->is_resized()) ? image->get_height() : image->get_height_scaled();
			if(image->get_alignment() == "left"  ) { image->set_relative_position(0, 0);	}
			if(image->get_alignment() == "center") { image->set_relative_position((get_width() - image_width) / 2, (get_height() - image_height) / 2);}
			if(image->get_alignment() == "right" ) { image->set_relative_position(get_width() - image_width, 0);	}
			image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
			// if image is bigger than button, resize to match the exact size of the button
			//if(image_width  > get_width ()) image->resize(get_width(), image_height);
		    //if(image_height > get_height()) image->resize(image_width, get_height());
			image->draw(); // draw
		}
		// Draw text  (over button)
		if(label != nullptr)
		{
		    if(label->get_alignment() == "left"  ) { label->set_relative_position(0                                     , (get_height() - 10/*label->get_height()*/) / 2); } // keep label_y centered always
			if(label->get_alignment() == "center") { label->set_relative_position((get_width() - label->get_string().length() * 10/*label->get_width()*/) / 2, (get_height() - 10/*label->get_height()*/) / 2); } // keep label_y centered always					
			if(label->get_alignment() == "right" ) { label->set_relative_position(get_width() - label->get_string().length() * 10/*label->get_width()*/, (get_height() - 10/*label->get_height()*/) / 2); } // keep label_y centered always	
            if(label->get_alignment() == "none"  ) {} // with this you are free to set the label's relative position to whatever you want  // default - relative_position will always be (0, 0) unless you change the alignment
            label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // set actual position
		    // draw label manually since there is only one
		    label->draw();
		}
	}
}
/////////////
void Button::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Button::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int Button::draw(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_gettop(L) > 3)
	{
	    if(lua_isnumber(L, 2) && lua_isnumber(L, 3))
		{
		    Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
			button->draw((int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3));
		}
	}
	else
	{
	    Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->draw();
	}
	return 0;
}
/////////////
/////////////
// SETTERS
/////////////
void Button::set_text(const std::string& text)
{
    if(!label) throw std::runtime_error("label is not initialized");
	label->set_string(text);
}
/////////////
int Button::set_text(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_text (lua_tostring(L, 2));
	}
	return 0;
}
/////////////
void Button::set_image(const Image& image) 
{
	this->image = &const_cast<Image&>(image); // image->set_position ( button.x + pixels_to_units(button.width / 2), button.y + pixels_to_units(button.height / 2)) // I think this centers the image on the button, idk;    1 unit of movement = 16(for sprites) or 100(default) pixels
}
/////////////
int Button::set_image(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
			    Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
			    button-> set_image (*image);
			    // set image in (Lua)
			    lua_pushvalue(L, 2);
			    lua_setfield(L, 1, "image");
	        }
	    }
	}
	if(lua_isnil(L, 2)) 
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
			button->image = (nullptr); // set image to nullptr
		}
		lua_pushvalue(L, 2); // push 2nd arg
		if(lua_isnil(L, -1))
		{
			lua_setfield(L, 1, "image"); // set image to nil
		}
	}	
	return 0;
}
/////////////
void Button::set_label(const dokun::Label& label)
{
	this->label = &const_cast<dokun::Label&>(label);
	this->label->set_parent(* this);
}
/////////////
int Button::set_label(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1))
		    {
			    Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
			    button->set_label (*label); // set label in C++
			    // set label in (lua)
			    lua_pushvalue(L, 2);
			    lua_setfield(L, 1, "label");
		    }
	    }		
	}
	if(lua_isnil(L, 2)) 
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
			button->label = nullptr; // set label to nullptr
		}
		lua_pushvalue(L, 2); // push 2nd arg
		lua_setfield(L, 1, "label"); // set label to nil
	}		
	return 0;
}
/////////////
void Button::set_color(unsigned int red, unsigned int green, unsigned int blue) 
{
    color = Vector4(red, green, blue, color.w);
    old_color = color; // save user-defined color before changing the color    
}
/////////////
void Button::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
    old_color = color; // save user-defined color before changing the color	
}
/////////////
void Button::set_color(const Vector3& color) {
	set_color(color.x, color.y, color.z);
}
/////////////
void Button::set_color(const Vector4& color) {
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int Button::set_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;
}
/////////////
void Button::set_alpha(double alpha)
{
    old_color.w = color.w; // save old alpha before setting new one
	color.w = alpha;
}
/////////////
int Button::set_alpha(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_alpha(lua_tonumber(L, 2));
	}
	return 0;
}

/////////////
void Button::set_outline(bool outline)
{
    this->outline = outline;
}
void Button::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    outline_color = Vector4(red, green, blue, 1.0/*alpha*/);
}
void Button::set_outline_color(const Vector3& color)
{
    set_outline_color(color.x, color.y, color.z, outline_color.w);
}
void Button::set_outline_color(const Vector4& color)
{
    set_outline_color(color.x, color.y, color.z, color.w);
}
/////////////
void Button::set_fill(bool fill)
{
	this->fill = fill;
}
/////////////
int Button::set_fill(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_fill((lua_toboolean(L, 2) != 0));
	}
	return 0;
}
/////////////
void Button::set_shadow(bool shadow)
{
	this->shadow = shadow;
} 
/////////////
int Button::set_shadow(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_shadow((lua_toboolean(L, 2) != 0));
	}
	return 0;	
}
/////////////
void Button::set_gradient(bool gradient)
{
    this->gradient = gradient;
}
/////////////
int Button::set_gradient(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_gradient(lua_toboolean(L, 2));
	}
    return 0;
}
/////////////
void Button::set_gradient_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    gradient_color = Vector4(red, green, blue, alpha);
}
/////////////
void Button::set_gradient_color(const Vector3& gradient_color)
{
    set_gradient_color(gradient_color.x, gradient_color.y, gradient_color.z, this->gradient_color.w);
}
/////////////
void Button::set_gradient_color(const Vector4& gradient_color)
{
    set_gradient_color(gradient_color.x, gradient_color.y, gradient_color.z, gradient_color.w);
}
/////////////
int Button::set_gradient_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		button->set_gradient_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;
}
/////////////
/////////////
/////////////
//  GETTERS
/////////////
dokun::Label * Button::get_label()const
{
	return label;
}
/////////////
int Button::get_label(lua_State *L)
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
Image * Button::get_image()const
{
	return image;
} 
/////////////
int Button::get_image(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "image");
	if(lua_istable(L, -1))
	{
		return 1;
	}
	lua_pushnil(L);
	return 1;	
}
/////////////
std::string Button::get_text()const
{
	return get_label()->get_string();
}
/////////////
int Button::get_text(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
		lua_pushstring(L, button->get_text().c_str());
		return 1;
	} 
	lua_pushnil(L); 
	return 1;	
}
/////////////
Vector4 Button::get_color()const
{
	return color;
}
/////////////
int Button::get_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Button * button = *static_cast<Button **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, button->get_color().x);
		lua_pushnumber(L, button->get_color().y);
		lua_pushnumber(L, button->get_color().z);
		return 3 ;
	}		
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}
/////////////
Vector4 Button::get_hover_color()const
{
	return hover_color;
}
int Button::get_hover_color(lua_State *L)
{
	return 4;
}
Vector4 Button::get_press_color()const
{
	return press_color;
}
int Button::get_press_color(lua_State *L)
{
	return 4;
}
/////////////
/////////////
/////////////
/////////////
// BOOLEAN
/////////////
/////////////
/////////////
bool Button::is_image()const
{
	return (type == "image");
}
/////////////
/////////////
void Button::on_hover()
{}
void Button::on_press()
{}
/////////////
void Button::on_mouse_interact()
{
	if(Mouse::is_over(get_position(), get_size()))
	{
		if(Mouse::is_pressed(1)) {
			// add a shade to color (on press)
		    press_color.x = color.x * (1 - shade_factor); // 1 = black
		    press_color.y = color.y * (1 - shade_factor);
		    press_color.z = color.z * (1 - shade_factor);	
		    press_color.w = color.w;
            //std::cout << "press_color: " << press_color << std::endl;
		    color = press_color;
		}
		else color = old_color;//set_color(old_color); // set to hover_color
		
		// add a tint to color (on hover)
		hover_color.x = color.x + (255 - color.x) * tint_factor; //  255 = white
		hover_color.y = color.y + (255 - color.y) * tint_factor;
		hover_color.z = color.z + (255 - color.z) * tint_factor;
        hover_color.w = color.w;
        //std::cout << "hover_color: " << hover_color << std::endl;
		color = hover_color;
	} else color = old_color; // revert back to original color
}
/////////////
/////////////
int Button::new_(lua_State *L)
{
	int x = 0;
	int y = 0;
	int width;
	int height ;
	width = 100;
	height = 50;
    if(lua_gettop(L) == 2) // x, y
	{
		x = (int)lua_tonumber(L, 1);
		y = (int)lua_tonumber(L, 2);	
	}
    if(lua_gettop(L) == 3) // table, x, y
	{
		x = (int)lua_tonumber(L, 2);
		y = (int)lua_tonumber(L, 3);	
	}
	if(lua_gettop(L) == 5)
	{
		x = (int)lua_tonumber(L, 2);
		y = (int)lua_tonumber(L, 3);
		width = (unsigned int)lua_tonumber(L, 4);
		height = (unsigned int)lua_tonumber(L, 5);
	}
	if(lua_gettop(L) == 4)
	{
		x = (int)lua_tonumber(L, 1);
		y = (int)lua_tonumber(L, 2);
		width = (unsigned int)lua_tonumber(L, 3);
		height = (unsigned int)lua_tonumber(L, 4);
	}
    lua_settop(L, 0); // clear stack
	// create table (object)
	lua_createtable(L, 0, 0); 
	// set metatable
	lua_getglobal(L, "Button"); 
	lua_setmetatable(L, 1); 
    // set userdata
	Button **button = static_cast<Button **>(lua_newuserdata(L, sizeof(Button *)));
	if((x != 0 || y != 0) && (width == 100 && height == 50)) // width and height have not been touched
	    *button = new Button(x, y); // make sure to allocate memory in C++ or it'll crash
	else if((x != 0 || y != 0) && (width != 100 || height != 50)) // one or two of each have been touched
	    *button = new Button(x, y, width, height);
	else
		*button = new Button();
	
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	    return 1;
	lua_pushnil(L);
	    return 1;
}
