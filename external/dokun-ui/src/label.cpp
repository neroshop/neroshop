#include "../include/label.hpp"

dokun::Label::Label() : width(0), height(0), color(255, 255, 255, 1.0), alignment("none")/*, relative_position(0,0)*/, font(nullptr) // label size is 0 as no string has been set (empty string)
{                                 
	set_position(0, 0);
	set_relative_position(0, 0);
}
/////////////
dokun::Label::Label(const dokun::Label& label) : dokun::Label()
{
	if(label.get_font()) set_font(*label.get_font());
	set_string(label.get_string());
	set_color(label.get_color());
	set_style(label.get_style());
	set_background_color(label.get_background_color());
	set_alignment(label.get_alignment());
	set_relative_position(label.get_relative_position());
	set_position(label.get_position());
	set_width(label.get_width());
	set_height(label.get_height());	
}
/////////////
dokun::Label::Label(const dokun::Font& font) : dokun::Label()
{
	set_font(font);
}
/////////////
dokun::Label::Label(const dokun::Font& font, const std::string& text) : dokun::Label()
{
	set_font(font);
	set_string(text);
}
/////////////
dokun::Label::Label(const std::string& text) : dokun::Label()
{
	this->font = std::unique_ptr<dokun::Font>(new dokun::Font(DOKUN_DEFAULT_FONT_PATH));
	set_string(text);
}
/////////////
dokun::Label::Label(const std::string& text, int x, int y, int width, int height) : dokun::Label()
{
	this->font = std::unique_ptr<dokun::Font>(new dokun::Font(DOKUN_DEFAULT_FONT_PATH));
	set_string(text);
	set_position(x, y);
	set_width(width);
	set_height(height);
}
/////////////
dokun::Label::Label(int x, int y) : dokun::Label()
{
	set_position (x, y);
}
/////////////
dokun::Label::Label(int x, int y, int width, int height) : dokun::Label()
{
	set_position(x, y);
	set_width(width);
	set_height(height);
}

/////////////
int dokun::Label::label_new(lua_State *L)
{
	std::string string_;
	if(lua_type(L, -1) == LUA_TSTRING) string_ = static_cast<const char*>(lua_tostring(L, -1));
	lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// set mt
	lua_getglobal(L, "Label");
	lua_setmetatable(L, 1);
	// set userdata
	dokun::Label **label = static_cast<dokun::Label **>(lua_newuserdata(L, sizeof(dokun::Label *)));
	if(!string_.empty()) *label = new Label(string_);
    else *label = new Label();
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
		return 1;
	lua_pushnil(L);
	return 1;
}
/////////////
dokun::Label::~Label()
{
    // reset (delete) font
    if(font.get()) {
        font.reset();
    }
    std::cout << "label deleted\n";
}
/////////////
/////////////
void dokun::Label::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility and generates shaders for all GUIs including Labels
	if(!font.get()) return; // return if no font //if(string.empty()) return; // return if empty string (BAD: It will prevent from updating label's x and y position)
    if(!is_visible()) return; // exit function if not visible
	// STORE ALL CHARACTERS IN ARRAY BEFORE DRAWING!
	font->generate();
    // Draw text
	Renderer::draw_text(string, get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, get_color().x, get_color().y, get_color().z, get_color().w, *font.get(), label_shader);
}
/////////////
void dokun::Label::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void dokun::Label::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
///////////// 
int dokun::Label::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->draw();
	}
	return 0;
}
/////////////
void dokun::Label::copy(const dokun::Label& label)
{
	set_font(* label.get_font());
	set_string(label.get_string());
	set_color(label.get_color());
	set_style(label.get_style());
	set_background_color(label.get_background_color());
	set_alignment(label.get_alignment());
	//set_relative_position(label.get_relative_position());
	//set_position(label.get_position());
	//set_scale(label.get_scale());
	set_width(label.get_width());
	set_height(label.get_height());	
}
/////////////
int dokun::Label::copy(lua_State *L)
{
    return 0;
}
/////////////
void dokun::Label::clear() {
    this->string.clear();
}
/////////////
void dokun::Label::update(void)
{}
/////////////
/////////////
Shader * dokun::Label::label_shader (new Shader());
/////////////
void dokun::Label::generate_shader(void) {
    if(label_shader->is_linked()) return; // if label_shader is already generated and linked, exit function
	////////////////////////////
    const char * vertex_source[] =
    {
		"#version 330\n"
        "layout (location = 0) in vec4 vertex;\n"
        "\n"
		"out vec2 Texcoord;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 view;\n"
		"\n"
        "void main(void)\n"
		"{\n"
			"Texcoord    = vec2(vertex.z, 1-vertex.w);\n"
            "gl_Position = proj * view * model * vec4(vertex.xy, 0.0, 1.0);\n"
		"}\n"
	};
	const char * fragment_source[] =
    {
	    "#version 330                                                          \n"
        "\n"
        "out vec4 out_color;\n"
        "in vec2 Texcoord  ;\n"
		"\n"
        "uniform sampler2D font;\n"
        "uniform vec3 color    ;\n"
        "uniform float alpha   ;\n"
        "\n"
        "void main(void) {\n"
            "\n"
			"out_color = vec4(color, alpha) * vec4(1.0, 1.0, 1.0, texture(font, Texcoord).r);\n"
        "}\n"
	};	
	////////////////////////////
    if(!label_shader->has_program())
	{
		label_shader->create();
		label_shader->set_source(vertex_source  , 0);
		label_shader->set_source(fragment_source, 1);
		label_shader->prepare();
	    ////std::cout << DOKUN_UI_TAG "Label shaders have been generated (" << label_shader->get_program() << ")" << std::endl;//std::cout << "number of shaders attached to shader object program: " << label_shader->get_shader_count() << std::endl;
	}
}
/////////////
/////////////
// SETTERS	
/////////////
void dokun::Label::set_string(const std::string& string_)
{
	this->string = string_;
	// update label size whenever the string is changed
    if(!font.get()) return; // do not update if no font
	width = font->get_width (string_);
	height= font->get_height(string_);
#ifdef DOKUN_DEBUG0
    std::cout << "Label size updated: " << Vector2(width, height) << std::endl;
#endif
    //to-do: regex for links. Label strings containing only links can be redirected via a browser when clicked on the app 
}
/////////////
int dokun::Label::set_string(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_string(lua_tostring(L, 2));
        // set string in Lua
        lua_pushvalue(L, 2);
	    if(lua_istable(L, -1))
		{
	        lua_setfield(L, 1, "string");
	    }        
	}	
	return 0;
} 
/////////////
void dokun::Label::set_font(const dokun::Font& font)
{
    std::unique_ptr<dokun::Font> label_font(&const_cast<dokun::Font&>(font));
	this->font = std::move(label_font);
}
/////////////
int dokun::Label::set_font(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1))
		    {
			    dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
			    label->set_font (*font);
			    // set font in (Lua)
			    lua_pushvalue(L, 2);
			    lua_setfield(L, 1, "font");
		    }
	    }
	}
	if(lua_isnil(L, 2)) 
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
			label->font = nullptr; // set font to nullptr instead of deleting it so it can be reused
		}
		lua_pushvalue(L, 2); // push 2nd arg
		lua_setfield(L, 1, "font"); // set font to nil
	}	
	return 0;
}	
/////////////
void dokun::Label::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
/////////////
void dokun::Label::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
}   
/////////////
void dokun::Label::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
/////////////
void dokun::Label::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
/////////////
void dokun::Label::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int dokun::Label::set_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_color((unsigned int)lua_tonumber(L, 2), (unsigned int)lua_tonumber(L, 3), (unsigned int)lua_tonumber(L, 4));
	}
	return 0;
}
/////////////
void dokun::Label::set_style(const int style)
{
	(this)->style = style;
	if(style & DOKUN_STYLE_REGULAR) // if regular, this function ignores all other styles
	{
		std::cout << "Regular"<<std::endl;
	}
	if(style & DOKUN_STYLE_BOLD)
	{
		std::cout << "Bold"<<std::endl;
	}
	if(style & DOKUN_STYLE_ITALIC)
	{
		std::cout << "Italic"<<std::endl;
	}
	if(style & DOKUN_STYLE_UNDERLINED)
	{
		std::cout << "Underline"<<std::endl;
	}
	if(style & DOKUN_STYLE_STRIKETHROUGH)
	{
		std::cout << "Strikethrough"<<std::endl;
	}
}   
/////////////
int dokun::Label::set_style(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_style((unsigned int)lua_tonumber(L, 2));
	}	
	return 0;
}
/////////////
void dokun::Label::set_background_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
/////////////
void dokun::Label::set_background_color(const Vector3& color)
{
	set_background_color(color.x, color.y, color.z, background_color.w);
}
/////////////
void dokun::Label::set_background_color(const Vector4& color)
{
	set_background_color(color.x, color.y, color.z, color.w);
}
/////////////
int dokun::Label::set_background_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_background_color((unsigned int)lua_tonumber(L, 2), (unsigned int)lua_tonumber(L, 3), (unsigned int)lua_tonumber(L, 4));
	}		
	return 0;
}
/////////////
void dokun::Label::set_alignment(const std::string& alignment)
{
	this->alignment = alignment;
}
int dokun::Label::set_alignment(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_alignment(lua_tostring(L, 2));
	}
	return 0;
}
/////////////
/*
void dokun::Label::set_relative_position(double x, double y)
{
	relative_position = Vector2(x, y);
}
void dokun::Label::set_relative_position(const Vector2& position)
{
	set_relative_position(position.x, position.y);
}
int dokun::Label::set_relative_position(lua_State * L)
{}*/
/////////////
/////////////
// GETTERS
/////////////
dokun::Font * dokun::Label::get_font()const
{
	return font.get();
}
/////////////
int dokun::Label::get_font(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "font");
	if(lua_istable(L, -1))
	{
		return 1 ;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string dokun::Label::get_string()const
{
	return string;
}
/////////////
int dokun::Label::get_string(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_pushstring(L, label->get_string ().c_str());
		return 1 ;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 dokun::Label::get_color()const
{
	return color;
}   
/////////////
int dokun::Label::get_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_color().x);
		lua_pushnumber(L, label->get_color().y);
		lua_pushnumber(L, label->get_color().z);
		return 3;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}  
/////////////
int dokun::Label::get_style()const
{
	return 0;//style;
}   
/////////////
int dokun::Label::get_style(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_style());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 dokun::Label::get_background_color()const
{
	return background_color;
}
/////////////  
int dokun::Label::get_background_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_background_color().x);
		lua_pushnumber(L, label->get_background_color().y);
		lua_pushnumber(L, label->get_background_color().z);
		lua_pushnumber(L, label->get_background_color().w);
		return 4;
	}	
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);	
	return 4;	
}
/////////////
std::string dokun::Label::get_alignment() const
{
	return alignment;
}
int dokun::Label::get_alignment(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_pushstring(L, label->get_alignment().c_str());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
/////////////
double dokun::Label::get_aspect_ratio_correction(int rect_width, int rect_height) const
{
	double label_aspect = get_width() / get_height();
	double rect_aspect  = rect_width  / rect_height;
    double scale_factor = 0.0;
   	if(rect_aspect > label_aspect) {
		scale_factor     = rect_height / (double)get_height();
	} else scale_factor = rect_width  / (double)get_width ();
	return scale_factor;	
}
int dokun::Label::get_aspect_ratio_correction(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_aspect_ratio_correction(lua_tonumber(L, 2), lua_tonumber(L, 3)));
        return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
/////////////
std::vector<Vector2> dokun::Label::get_character_size_array()
{
	std::vector<Vector2> character_size_list;
    // Iterate through all characters in string
    for (std::string::const_iterator c = get_string().begin(); c != get_string().end(); c++) // scan through all characters in string
    {
        dokun::Font::Character ch = get_font()->character_array[*c];  // will change Character
        double width  = get_font()->character_array[*c].width;
        double height = get_font()->character_array[*c].height;
		character_size_list.push_back(Vector2(width, height));
	}		
	return character_size_list;
}
/////////////
/////////////
/////////////
// OVERRIDE	
/////////////
void dokun::Label::set_width(int width)
{
	int old_width  = (font.get() ? font->get_width (string) : get_width()); // width of entire string
	set_scale(width / static_cast<double>(old_width), get_scale().y);
	this->width = width; // save width (in value)
}
int dokun::Label::set_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_width(static_cast<int>(lua_tonumber(L, 2)));
	}		
	return 0;	
}
/////////////
void dokun::Label::set_height(int height)
{
	int old_height = (font.get() ? font->get_height(string) : get_height());
	set_scale(get_scale().x, height / static_cast<double>(old_height));
	this->height = height; // save height (in value)
}
int dokun::Label::set_height(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_height(static_cast<int>(lua_tonumber(L, 2)));
	}		
	return 0;	
}
/////////////
void dokun::Label::set_size(int width, int height)
{
	set_width(width);
	set_height(height);
}
int dokun::Label::set_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		label->set_size(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
	}		
	return 0;	
}
/////////////
// override (getters)
int dokun::Label::get_width()const
{
	/*if(font.get()) return font->get_width(this->string) * get_scale().x; // if font is set, return width of all characters combined, whether scaled or not
	return width * get_scale().x;*/ // return width whether scaled or not
    // width is not accurate, so I have to use 10 as the width for each character in a monospaced font
    return (string.length() * 10) * get_scale().x;
}
int dokun::Label::get_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_pushnumber(L, label->get_width());
        return 1;
    }
    lua_pushnumber(L, 0);
	return 1;
}
/////////////
int dokun::Label::get_height()const
{
	/*if(font.get()) return font->get_height(this->string) * get_scale().y; // if font is set, return largest glyph height, whether scaled or not
	return height * get_scale().y;*/ // return height whether scaled or not
    // height is not accurate, so I have to use 10 as the height for monospaced fonts
    return 10 * get_scale().y;
}
int dokun::Label::get_height(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_pushnumber(L, label->get_height());
        return 1;
    }
    lua_pushnumber(L, 0);
	return 1;
}
/////////////
Vector2 dokun::Label::get_size() const
{
	return Vector2(get_width(), get_height());
}
int dokun::Label::get_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_pushnumber(L, label->get_size().x);
        lua_pushnumber(L, label->get_size().y);
        return 2;
    }
    lua_pushnumber(L, 0);
    lua_pushnumber(L, 0);
	return 2;
}
/////////////
Vector4 dokun::Label::get_rect() const
{
	return Vector4(get_x(), get_y(), get_width(), get_height());
}
int dokun::Label::get_rect(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_pushnumber(L, label->get_rect().x);
        lua_pushnumber(L, label->get_rect().y);
        lua_pushnumber(L, label->get_rect().z);
        lua_pushnumber(L, label->get_rect().w);
        return 4;
    }
    lua_pushnumber(L, 0);
    lua_pushnumber(L, 0);
    lua_pushnumber(L, 0);
    lua_pushnumber(L, 0);
	return 4;
}
/////////////
/////////////
bool dokun::Label::is_empty() const {
    return this->string.empty();
}
/////////////
/////////////
bool dokun::Label::is_label() const
{
    return ((this != 0) && (dokun::instanceof<Label>(this) != 0));
}
/////////////
int dokun::Label::is_label(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<dokun::Label *>(gui)->is_label());
		return 1;
	}
    lua_pushboolean(L, false);
    return 1;	
}
/////////////
/////////////
// This function is no longer needed
// label's width and height is different from other gui elements' size which is why it needed its own implementation of GUI::on_parent()
/*void dokun::Label::on_parent() {
    // if self has no parent, exit function
	if(!get_parent()) return;
	// set position of child
	// only the child's position relative to the parent can be changed, meaning the child no longer has a position of its own once the parent is set
	// so using "set_position" on a child GUI makes no sense
	set_position(get_parent()->get_x() + get_relative_x(), get_parent()->get_y() + get_relative_y());
	// make sure child does not go past parent's x bounds - success!
	if(get_relative_x() >= (get_parent()->get_width() - get_width())) { 
	    set_position(get_parent()->get_x() + (get_parent()->get_width() - get_width()), get_y()); 
	    // fix the incorrect relative x position
	    // set the relative position to the parent_width - child_width
	    set_relative_position(get_parent()->get_width() - get_width(), get_relative_y());	
	}
	if(get_x() <= get_parent()->get_x()) set_position(get_parent()->get_x(), get_y());// child_x = parent->get_x() + get_relative_x()
	// make sure child does not go past parent's y bounds - success!
	if(get_y() <= get_parent()->get_y()) set_position(get_x(), get_parent()->get_y());// up// child_y = parent->get_y() + get_relative_y() 
	if(get_relative_y() >= (get_parent()->get_height() - get_height())) {
	    set_position(get_x(), get_parent()->get_y() + (get_parent()->get_height() - get_height()));
	    // fix the incorrect relative y position
	    // set the relative position to the parent_height - child_height
	    set_relative_position(get_relative_x(), (get_parent()->get_height() - get_height()));
	}// down//{std::cout <<"child going outside parent y bounds!\n";}//{set_position(get_x(), parent->get_y());} // REMINDER: shrink size of button and retest this function again	    
#ifdef DOKUN_DEBUG0	    
	std::cout << "label_x: " << get_x() << std::endl;
	std::cout << "label_y: " << get_y() << std::endl;
	std::cout << "label_rel_x: " << get_relative_x() << std::endl;
	std::cout << "label_rel_y: " << get_relative_y() << std::endl; // bug: relative position goes out of bound [fixed]
#endif
}*/
/////////////
/////////////
/*  
std::cout << "Label position: " << label->get_position() << std::endl;
std::cout << "Label size: " << label->get_size() << " (all glyphs' width combined, highest glyph height)" << std::endl;

*/
