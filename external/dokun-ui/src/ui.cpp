#include "../include/ui.hpp"

/////////////
GUI::GUI (void) : x (0), y (0), angle(0), scale_x(1), scale_y(1), width (0), height (0), orientation(0), relative(0, 0), parent(nullptr), visible(true), active(true), disabled(false), draggable(false), droppable(false), resizeable(false), sortable(false)
{
	Factory::get_gui_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with GUI::new ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_gui_factory()->get_location(this)) + ", total_gui_count=" + std::to_string(Factory::get_gui_factory()->get_size()) + ")");
#endif		
}
/////////////
GUI::~GUI(void)
{
	Factory::get_gui_factory()->release(this); 
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " deallocated with GUI::~GUI()\n       (total_gui_count=" + String(Factory::get_gui_factory()->get_size()).str() + ")");
#endif	
}
/////////////
GUI * GUI::focused  (nullptr);
/////////////
Shader * GUI::gui_shader(new Shader());//(nullptr); // cannot create/initialize shader until we have a graphics context
/////////////
void GUI::show()
{
	set_visible(true);
}
/////////////
int GUI::show(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->show (); 
	}
	return 0;
}
/////////////  
void GUI::hide()
{
	set_visible(false);
}
/////////////  
int GUI::hide(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->hide (); 
	}
	return 0;
}
/////////////  
void GUI::show_all()
{
	for (int i = 0; i < Factory::get_gui_factory()->get_size(); i++) 
	{
		GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
		gui->show();
	}
}   
/////////////  
int GUI::show_all(lua_State *L)
{
	GUI::show_all ();
	return 0;
}
/////////////  
void GUI::hide_all()
{
	for (int i = 0; i < Factory::get_gui_factory()->get_size(); i++) 
	{
		GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
		gui->hide();
	}	
}
/////////////  
int GUI::hide_all(lua_State *L)
{
	GUI::hide_all ();
	return 0;
}
/////////////  
void GUI::focus() {
    set_focus(true); // will call Edit::set_focus if Edit has its own implementation of GUI::set_focus(), a virtual function
}
/////////////
void GUI::clear_all() {
    //GUI::focused = nullptr;// some GUI (like Edit) have their own focus member, so we have to loop all gui and set their focus
    for(auto guis : Factory::get_gui_factory()->get_storage()) {//for (int i = 0; i < Factory::get_gui_factory()->get_size(); i++) {
	    static_cast<GUI *>(guis)->set_focus(false);//GUI * gui = static_cast<GUI *>(guis);//GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
	}
	std::cout << DOKUN_UI_TAG "all focus cleared\n";
}
/////////////  
void GUI::connect(const GUI& a, int signal, const GUI& b, std::function<void(void)> slot)
{
	if(signal == 0) // hover
	{
		if(const_cast<GUI&>(a).is_hovered()) slot();
	}
    if(signal == 1) // pressed
	{
		if(const_cast<GUI&>(a).is_pressed()) slot();
	}	
    if(signal == 2) // released
	{
		if(const_cast<GUI&>(a).is_released()) slot();
	}	
}
/////////////  
// generates the default shader used by most GUI elements
void GUI::generate_shader(void)
{
    if(gui_shader->is_linked()) return; // exit if shader_program has already been generated and linked//if(!shader) throw std::runtime_error("GUI Shader is not initialized");
	// create (initialize) a shader
	//gui_shader = new Shader();
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 tex_coords;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "tex_coords = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
	    // temp --------------------------------
	    //"#define linearstep(edge0, edge1, x) clamp((x - (edge0)) / (edge1 - (edge0)), 0.0, 1.0)\n"// basically the same as smoothstep() as seen here: https://docs.gl/sl4/smoothstep // http://jeremt.github.io/pages/anti-aliased_shapes_in_glsl.html
	    "\n"
	    // temp ^ ------------------------------
        "\n"
        "const float PI = 3.14159265;\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D texture_obj;\n"
		"in vec2 tex_coords;\n"
		"uniform vec2 size;"
		"uniform float radius;"
		"\n"
		"uniform bool outline = false;\n"
		"uniform float outline_thickness = 0.2;\n"
		"uniform vec4 outline_color = vec4(255, 255, 255, 1.0);\n"
		"uniform float outline_threshold = 0.5;\n"
		"\n" // uniform float radius;
		"float round_corner(vec2 p, vec2 b, float r) {\n"
		    "return length(max(abs(p)-b+r,0.0))-r;\n" // length(max(abs(p)-b, 0.0))-r;"
		"}\n"
		"float circle(in vec2 _st, in float _radius) {\n"
        "    vec2 dist = _st - vec2(0.5);\n"
	    "    return 1. - smoothstep(_radius - (_radius * 0.01), _radius+(_radius*0.01), dot(dist, dist) * 4.0);\n"
        "}"// usage: vec3 color = vec3(circle(st,0.9));
		"\n"
		"uniform vec2 resolution;\n"
		"uniform float time;"
		"uniform vec2 mouse;"
		"\n"
		"vec2 position;" // tex_coords.x = from_left_to_right, tex_coords.y = from_up_to_down
		"struct Gradient {\n"
			"vec4 color;\n" // bottom // the color used to mix with the original color (which will be the top color)
			"float value;\n"
		    "bool enabled;\n"
		"\n"
		"};\n"
		"uniform Gradient gradient;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
            "vec2 st = tex_coords.xy / resolution.xy;\n"
            "float aspect = resolution.x / resolution.y;\n"
            "vec2 uv = (2.0 * st - 1.0) * vec2(aspect, 1.0);\n"
            "vec2 half_res = 0.5 * resolution;\n"
            "vec2 half_size = 0.5 * size;\n" // half of the box_size or center of box
			"\n"
			"\n"
			// outline requires a radius to work :/
            // temp ----------------------------------
            "vec2 uv_temp = abs((tex_coords/*uv*/ - vec2(0.5)) * vec2(aspect, 1.0));\n"
            "float half_width = aspect * 0.5;\n"
            "float radius_temp = clamp((outline && radius <= 0.00) ? 0.01 : (radius / 100.0), 0.0, 1.0) * min(half_width, 0.5);\n" // convert radius (int) to opengl floating point number = radius / 100; or radius * 0.01; // 100 is the max radius
            "vec2 center = vec2(half_width, 0.5) - vec2(radius_temp);\n"
            "// outer edge\n"
            "vec2 half_uv = uv_temp - vec2(half_width, 0.5);\n"
            "float d = mix(-max(half_uv.x, half_uv.y), radius_temp - distance(uv_temp, center),"
            "    float(uv_temp.x >= center.x && uv_temp.y >= center.y));\n"
            "// apply anti-aliasing (makes edges look smooth instead of pixelated)\n"
            "float border = smoothstep(0.0, fwidth(d), d);//linearstep(0.0, fwidth(d), d);//d;\n"
            "\n"
            // temp ^ --------------------------------
			//"if( (length(tex_coords * size - vec2(0)) < radius)  || (length(tex_coords * size - vec2(0, size.y)) < radius) || (length(tex_coords * size - vec2(size.x, 0)) < radius) || (length(tex_coords * size - size) < radius) )"
            //"{     discard;"
            //"}"
			"\n" // size_x = (size.x / 2) * (size.x / size.y)    or  size_y = (size.y / 2) * (size.y / size.x)
		    ////"float b = 1.0 - round_corner(tex_coords * size - half_res, half_res, abs(radius));  //(tex_coords - half_res, half_res, radius);\n" // position, size, radius // abs() turns a negative number into a positive number
		    ////"float round = smoothstep(0.0, 1.0, b);          \n" //"vec4 pixel = texture2D(texture_obj, tex_coords);" // if texture is present
		    //"if(!outline) {\n"
		    // fragment color (default):
		    "out_color = vec4(color.xyz, color.w);\n" //border is in percentage(%) rather than HTML pixel(px)// 1.0, 1.0, 1.0, 1.0 is default frag_color
		    // rounded corners (radius - uses anti-aliasing via smoothstep to smoothen edges):
		    "if(radius_temp > 0.00) out_color = vec4(color.xyz, color.w * border);//round);\n" // smooth or linear (anti-aliasing)
		    // setting anti-aliasing (smoothness) to gui edges without changing the radius:
		    //"out_color = vec4(color.xyz, color.w * (smoothstep(vec2(0), fwidth(tex_coords), tex_coords) * smoothstep(vec2(0), fwidth(tex_coords), vec2(1) - tex_coords)) );\n"
            //"}\n"
            //////////////////////////////////////////////////////
            // outline-border
                //"out_color = texture(texture_obj, tex_coords);\n"//makes all gui black
            "if(outline) {\n"    
                "if(out_color.a <= outline_threshold) {\n"
                    "ivec2 size = textureSize(texture_obj, 0);\n"
                    "\n"
                    "float uv_x = tex_coords.x * size.x;\n"
                    "float uv_y = tex_coords.y * size.y;\n"
                    "\n"
                    "float sum = 0.0;\n"
                    "for (int n = 0; n < 9; ++n) {\n"
                        "uv_y = (tex_coords.y * size.y) + (outline_thickness * float(n - 4.5));\n"
                        "float h_sum = 0.0;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x - (4.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x - (3.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x - (2.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x - outline_thickness, uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x, uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x + outline_thickness, uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x + (2.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x + (3.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(texture_obj, ivec2(uv_x + (4.0 * outline_thickness), uv_y), 0).a;\n"
                        "sum += h_sum / 9.0;\n"
                    "}\n"
                    "\n"
                    "if (sum / 9.0 >= 0.0001) {\n"
                        //"out_color = vec4(outline_color.xyz, (radius > 0.00) ? outline_color.w * border : outline_color.w);\n"
                        "out_color = vec4(outline_color.xyz, outline_color.w);\n"
                        "if(radius_temp > 0.00) out_color = vec4(outline_color.xyz, outline_color.w * border);\n" // outline_color.w (alpha) will be 1.0 for now
                    "}\n"
                "}\n"
            "}\n"
        //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////
		    // gradient
            "if(gradient.enabled == true)\n"
			"{"
			    // top-to-bottom gradient
			    "position  = tex_coords.st;\n" // can use either .xy or .st
			    "vec4 top = vec4(color + (1.0 - color) * gradient.value);\n" // tint  (1=white)
			    "vec4 bottom = vec4(gradient.color + (0.0 - gradient.color) * gradient.value);\n" // shade (0=black)
			    "out_color = vec4(mix(top, bottom, position.y).xyz, color.w);\n" // position.y = top-to-bottom, position.x = left-to-right
			    "if(radius_temp > 0.00) out_color = vec4(mix(top, bottom, position.y).xyz, color.w * border);\n" // rounded corner with gradient
			    // left-to-right gradient
			    // ...
			    // normal (point) gradient
			    ////"float mix_value = distance(tex_coords, vec2(0, 0));\n"// no significant difference b/t tex_coords and gl_PointCoord (for now)// (0, 0) = top-left | (0, 1) = bottom-left | (1, 0) = top-right corner
			    ////"vec3 color_final = mix(gradient.color0.xyz, gradient.color1.xyz, mix_value);\n"// or "vec4 color_final = mix(gradient.color0, gradient.color1, mix_value);\n"
			    ////"out_color = vec4(color_final, 1.0);\n" // or "out_color = vec4(color_final);\n"
			"}\n"
        "}\n"
	};
	bool is_shader = gui_shader->has_program();
    if(is_shader == false)
	{
		gui_shader->create();
		gui_shader->set_source(vertex_source  , 0);
		gui_shader->set_source(fragment_source, 1);
		gui_shader->prepare();
		/*gui_shader->compile(0);
		gui_shader->compile(1);
		gui_shader->attach(0);
		gui_shader->attach(1);
		gui_shader->link();*/
		// Shader::prepare() is equivalent to the code after the last "shader->set_source(src, shader_idx)" and does all three: compile() + attach() + link()
	    ////std::cout << DOKUN_UI_TAG "GUI shaders have been generated (" << gui_shader->get_program() << ")" << std::endl;//std::cout << "number of shaders attached to shader object program: " << gui_shader->get_shader_count() << std::endl;
	}
}
/////////////  
/////////////  
// SETTERS     
/////////////   
void GUI::set_width(int width)
{
	this->width = width;	
}
/////////////
int GUI::set_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_width(static_cast<int>(lua_tonumber(L, 2)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "width");	
	}	
	return 0;
}
/////////////   
void GUI::set_height(int height)
{
	this->height = height;
}
/////////////   
int GUI::set_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_height(static_cast<int>(lua_tonumber(L, 2)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "height");		
	}		
	return 0;
}
/////////////    
void GUI::set_size(int width, int height)
{
    set_width (width );
	set_height(height);
}
/////////////   
void GUI::set_size(const Vector2& size)
{
	set_size(size.x, size.y);
}
/////////////   
int GUI::set_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_size(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
	}		
	return 0;
}
/////////////    
void GUI::set_position(double x, double y) // will not work if GUI has a parent, use set_relative_position instead to set the position relative to the parent // once a parent is set, a gui is locked to that parent unless parent is set to nullptr
{
	this->x = x;
	this->y = y;
}
///////////// 
void GUI::set_position(const Vector2& position)
{
	set_position(position.x, position.y);
}
///////////// 
int GUI::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_position(static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "y");		
	}	
	return 0;
} 
///////////// without a parent, set_relative_position does nothing
void GUI::set_relative_position(double x, double y) // set child position relative to the parent
{
    // relative position cannot be less than 0 [2021-09-21]
    if(x <= 0) x = 0;
    if(y <= 0) y = 0;
    // relative position cannot go past parent's size
    if(parent) {
        if(x >= parent->get_width ()) x = parent->get_width () - get_width();
        if(y >= parent->get_height()) y = parent->get_height() - get_height();
    }
    // set the relative position
	relative = Vector2(x, y); // does not matter whether gui has parent or not, set the relative_position
}
///////////// 
void GUI::set_relative_position(const Vector2& position)
{
	set_relative_position(position.x, position.y);
}
/////////////
int GUI::set_relative_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);	
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_relative_position(lua_tonumber(L, 2), lua_tonumber(L, 3));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "relative_x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "relative_y");		
	}
	return 0;
}
/////////////
void GUI::set_scale(double sx, double sy)
{
	this->scale_x = sx;
	this->scale_y = sy;
}
///////////// 
int GUI::set_scale(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_scale (static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3))) ;
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "scale_x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "scale_y");		
	}
	return 0;
}
///////////// 
void GUI::set_scale(const Vector2& scale) {
    set_scale(scale.x, scale.y);
}
///////////// 
void GUI::set_angle(double angle)
{
	this->angle = angle; // in degrees
}
///////////// 
int GUI::set_angle(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_angle (static_cast<double>(lua_tonumber(L, 2)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "angle");	
	}
	return 0;
}
///////////// 
void GUI::set_orientation(int orientation)
{
	switch(orientation)
	{
		case 0:
		    set_angle(static_cast<double>(0));
		break;
		case 1:
		    set_angle(static_cast<double>(90));
		break;
	}
    this->orientation = orientation;
}
///////////// 
int GUI::set_orientation(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_orientation (static_cast<int>(lua_tonumber(L, 2))) ;
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "orientation");	
	}
	return 0;
}
///////////// 
void GUI::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
/////////////
void GUI::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    color = Vector4(red, green, blue, alpha);
}
/////////////
void GUI::set_color(const Vector3& color) {
    set_color(color.x, color.y, color.z);
}
/////////////
void GUI::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
/////////////
void GUI::set_color(const Vector4& color) {
    set_color(color.x, color.y, color.z, color.w);
}
///////////// 
void GUI::set_parent(const GUI& parent)
{
	this->parent = &const_cast<GUI&>(parent);
	// set position relative to parent (if parent set)
	set_position(parent.get_x() + relative.x, parent.get_y() + relative.y);//if(!parent->is_visible()) { this->set_visible(false); } // if parent is visible child will also be visible (the child has their own visibility, but if the parent is not visible, the child cannot be visible either)
}                 
///////////// 
int GUI::set_parent(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // parent
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    GUI * parent = *static_cast<GUI **>(lua_touserdata(L, -1));	
		    lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
		        GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		        gui->set_parent(*parent); // set parent in (C++)
			    // set parent in (Lua)
		        lua_pushvalue(L, 2);
	  	        lua_setfield(L, 1, "parent");
	        }
	    }
	}
	if(lua_isnil(L, 2))
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		    gui->parent = nullptr; // set parent to nullptr (C++)
		}
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "parent"); // set parent to nil (Lua)
	}
	return 0;	
}	 
///////////// 
void GUI:: set_visible(bool visible) // shown or hidden
{ 
	this->visible = visible;
	////////////////////////
	std::vector<GUI *> children = get_children();
	for(auto guis : children) {
		GUI * child = static_cast<GUI *>(guis);
		child->set_visible(visible); // set child visibility to the same as parent
	}
}
///////////// 
int GUI::set_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_visible((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "visible");		
	}	
	return 0;
}
///////////// 
void GUI::set_disabled(bool disabled) {
    this->disabled = disabled;
}
///////////// 
///////////// 
void GUI:: set_active(bool active) // active or disabled
{
	this->active = active;
}
///////////// 
int GUI:: set_active(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_active((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "active");	
	}
	return 0;
}
///////////// 
///////////// 
void GUI::set_focus(bool focused)
{
    //dokun::Window * window = static_cast<dokun::Window *>(Factory::get_window_factory()->get_object(0)); // get the current window
    //if(!window)	return; // cannot set gui focus without a window
	// bring window to the foreground (get window back in focus)
	// set gui focus if window is focused
    GUI::focused = (focused == true/* && window->has_focus()*/) ? this : nullptr; 
}
/////////////
int GUI::set_focus(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_focus((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "focused");	
	}	
	return 0;	
}
///////////// 
///////////// 
void GUI::set_draggable (bool draggable)
{
	this->draggable = draggable;
}
///////////// 
int GUI::set_draggable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_draggable((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "draggable");    
	}

	return 0;
}
///////////// 
void GUI::set_droppable (bool droppable)
{
	this->droppable = droppable;
}
///////////// 
int GUI:: set_droppable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_droppable((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "droppable");   
	}	
	return 0;
}
///////////// 
void GUI::set_resizeable(bool resizeable)
{
	this->resizeable = resizeable;
}
///////////// 
int GUI:: set_resizeable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_resizeable((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "resizeable");    
	}	
	return 0;
}
/////////////  
void GUI::set_sortable(bool sortable)
{
	this->sortable = sortable;
}           
/////////////  
int GUI::set_sortable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_sortable((lua_toboolean(L, 2) != 0));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "sortable");    
	}	
	return 0;	
}
/////////////  
// callback_names: hover, press, left_press, middle_press, right_press, click, left_click, middle_click, right_click
void GUI::set_callback(const std::string& callback_name, std::function<void(void)> callback_function) {
    if(String::contains(String::lower(callback_name), "hover")) {
        callback_list["on_hover"] = callback_function;
    }    
    //-------------------------------------------
    if(String::lower(callback_name) == "press") { // any button pressed
        callback_list["on_press"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_press callback set\n";    
    }
    if(String::contains(String::lower(callback_name), "left") && 
       String::contains(String::lower(callback_name), "press")) { // left button pressed
        callback_list["on_left_press"] = callback_function;
    }
    if(String::contains(String::lower(callback_name), "middle") && 
       String::contains(String::lower(callback_name), "press")) { // middle button pressed
        callback_list["on_middle_press"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_middle_press callback set\n";    
    }    
    if(String::contains(String::lower(callback_name), "right") && 
       String::contains(String::lower(callback_name), "press")) { // right button pressed
        callback_list["on_right_press"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_right_press callback set\n";
    }    
    //-------------------------------------------
    if(String::lower(callback_name) == "click") { // any button clicked
        callback_list["on_click"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_click callback set\n";
    }
    if(String::contains(String::lower(callback_name), "left") && 
       String::contains(String::lower(callback_name), "click")) { // left button clicked
        callback_list["on_left_click"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_left_click callback set\n";    
    }
    if(String::contains(String::lower(callback_name), "middle") && 
       String::contains(String::lower(callback_name), "click")) { // middle button clicked
        callback_list["on_middle_click"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_middle_click callback set\n";    
    }
    if(String::contains(String::lower(callback_name), "right") && 
       String::contains(String::lower(callback_name), "click")) { // right button clicked
        callback_list["on_right_click"] = callback_function;
        //std::cout << DOKUN_UI_TAG "on_right_click callback set\n";    
    }        
}
// https://stackoverflow.com/questions/9451559/what-is-the-difference-between-clicked-and-pressed-in-qt
/////////////  
///////////// 
// GETTERS 
///////////// 
/////////////
/*std::shared_ptr<GUI> get_shared_ptr() {//const {
    return shared_from_this(); // returns this as a shared_ptr // causes "error: ‘shared_from_this’ was not declared in this scope" for some reason :(
}*/
///////////// 
int GUI::get_width() const
{
	return width * get_scale().x; // returns width whether scaled or not
}
///////////// 
int GUI::get_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_width());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
///////////// 
int GUI::get_height() const
{
	return height * get_scale().y; // returns height whether scaled or not
}
///////////// 
int GUI:: get_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, gui->get_height());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
///////////// 
Vector2 GUI::get_size() const
{
	return Vector2(get_width(), get_height());
} 
///////////// 
int GUI::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_size().x);
		lua_pushnumber(L, gui->get_size().y);
		return 2;
	}		
	lua_pushnil(L);
	return 1;
}
///////////// 
double GUI::get_x() const
{
	return x;
} 
///////////// 
int GUI:: get_x(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_x());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
///////////// 
double GUI::get_y() const
{
	return y;
}
///////////// 
int GUI::get_y(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_y());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
///////////// 
Vector2 GUI::get_position() const
{
	return Vector2(get_x(), get_y());
}
///////////// 
int GUI::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_position().x);
		lua_pushnumber(L, gui->get_position().y);
		return 2;
	}
	lua_pushnil(L);
	return 2;
}
///////////// 
double GUI::get_relative_x() const
{
    if(!parent) { 
 #ifdef DOKUN_DEBUG1   
		dokun::Logger("warning! Calling GUI::get_relative_x without a parent.", "warning"); // this is not that serious
 #endif
	}
	return relative.x;	
} 
///////////// 
int GUI::get_relative_x(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_relative_x());
		return 1;
	}
	lua_pushnil(L);
    return 1;
}
///////////// 
double GUI::get_relative_y() const
{
    if(!parent) { 
 #ifdef DOKUN_DEBUG1   
		dokun::Logger("warning! Calling GUI::get_relative_y without a parent.", "warning"); // this is not that serious
 #endif
	}
	return relative.y;  
}
///////////// 
int GUI::get_relative_y(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_relative_y());
		return 1;
	}
	lua_pushnil(L);
    return 1;
}
///////////// 
Vector2 GUI::get_relative_position() const // returns object position relative to the parent coordinates
{
	return Vector2(get_relative_x(), get_relative_y());
}
/////////////
int GUI::get_relative_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_relative_position().x);
		lua_pushnumber(L, gui->get_relative_position().y);
		return 2;
	}
	lua_pushnil(L);
	return 2;
}
/////////////
Vector2 GUI::get_scale()const
{
	return Vector2(scale_x, scale_y);
}
/////////////
int GUI::get_scale(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_scale().x);
        lua_pushnumber(L, gui->get_scale().y);
		return 2;
	}	
	lua_pushnil(L);
    lua_pushnil(L);
	return 2;
}
/////////////
double GUI::get_angle()const
{
	return angle;
}
/////////////
int GUI::get_angle(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_angle());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
int GUI::get_orientation() const
{
	return orientation;
}   
///////////// 
int GUI::get_orientation(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_orientation());
		return 1;
	}	
	lua_pushnil(L);
	return 1;	
}
/////////////
Vector4 GUI::get_color() const {
    return color;
}
/////////////
GUI * GUI::get_parent() const
{
	return parent;
}
///////////// 
int GUI::get_parent(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "parent");
		if(lua_istable(L, -1))
		{
			return 1;
		}
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
std::vector<GUI *> GUI::get_children()
{
	std::vector<GUI *> children;
	for(auto guis : Factory::get_gui_factory()->get_storage())
	{
		GUI * gui = static_cast<GUI *>(guis);
		if(!gui) continue; // skip nullptr guis
		if(gui->get_parent() == this) {
			children.push_back(gui);
		}
	}
	return children;
}
/////////////
int GUI::get_children(lua_State *L)
{
	return 0;
}
/////////////
Vector4 GUI::get_rect()const
{
	return Vector4(get_x(), get_y(), get_width(), get_height());
}
int GUI::get_rect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_rect().x);	
		lua_pushnumber(L, gui->get_rect().y);	
		lua_pushnumber(L, gui->get_rect().z);	
		lua_pushnumber(L, gui->get_rect().w);
        return 4;
	}
    lua_pushnil(L);	
    lua_pushnil(L);	
    lua_pushnil(L);	
    lua_pushnil(L);
    return 4;	
}
/////////////
///////////// 
// BOOLEAN
///////////// 
bool GUI::is_visible() const// checks if shown or hidden
{
	return visible;
}
/////////////
int GUI::is_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_visible() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool GUI::is_disabled() const {
    return disabled;
}
/////////////
/////////////
bool GUI::is_active() const // checks if active or not
{
	return active;
} 
///////////// 
int GUI::is_active(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_active() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;
}
///////////// 
bool GUI::is_draggable()
{
	return draggable;
}
///////////// 
int GUI::is_draggable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_draggable() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
///////////// 
bool GUI::is_droppable()
{
	return droppable;
}
///////////// 
int GUI::is_droppable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_droppable() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
/////////////    
bool GUI::is_resizeable()
{
	return resizeable;
}
/////////////   
int GUI::is_resizeable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_resizeable() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
///////////// 
bool GUI::is_sortable()
{
	return sortable;
}
///////////// 
int GUI::is_sortable(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_sortable() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;	
}
///////////// 
bool GUI::is_gui()
{
    return ((this != 0) && (dokun::instanceof<GUI>(this) != 0));
}
///////////// 
int GUI::is_gui(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<GUI *>(entity)->is_gui());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool GUI::has_parent()
{
	return (parent != nullptr);
}
/////////////
int GUI::has_parent(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->has_parent());
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;		
}
/////////////
bool GUI::is_child_of(const GUI& gui) // is this your parent? UPDATED: 9-7-2018
{
	return (parent == &gui);
}
/////////////
int GUI::is_child_of(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * parent = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		    lua_pushboolean(L, gui->is_child_of(*parent));
		    return 1;
	    }
	}	
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool GUI::is_parent_of(const GUI& gui) // is this your child? UPDATED: 9-7-2018
{
	return (this == gui.get_parent());
}
/////////////
int GUI::is_parent_of(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * child = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		    lua_pushboolean(L, gui->is_parent_of(*child));
		    return 1;
	    }
	}	
	lua_pushboolean(L, false);
	return 1;		
}
/////////////
/////////////
bool GUI::has_focus() {
    return (this == focused);
}
/////////////
bool GUI::is_collided(const GUI& gui) const { 
    // 1. both guis must first be visible
    if(!visible) return false;
    if(!gui.visible) return false;
    bool x_collided = get_x() + get_width() >= gui.get_x() && gui.get_x() + gui.get_width() >= get_x();
    bool y_collided = get_y() + get_height() >= gui.get_y() && gui.get_y() + gui.get_height() >= get_y();
    return (x_collided && y_collided);
}
/////////////
bool GUI::collision(const GUI& a, const GUI& b) {
    // 1. both guis must first be visible
    if(!a.visible) return false;
    if(!b.visible) return false;
    // AABB - AABB (axis-aligned bounding box) collisions
    bool x_collided = a.get_x() + a.get_width() >= b.get_x() && b.get_x() + b.get_width() >= a.get_x();
    bool y_collided = a.get_y() + a.get_height() >= b.get_y() && b.get_y() + b.get_height() >= a.get_y();
    return (x_collided && y_collided);
}
/////////////
// if gui element is overlapped by another gui element then set bottom gui element to inactive
void GUI::on_overlapped(const GUI& gui_top) {
    /*//if(parent == &const_cast<GUI&>(gui_top)) return; // ??
    // 0. by default all gui elements are active
    ////set_active(true);
    // 1. gui elements have collided
    if(!collision(* this, gui_top)) return;
    // 2. the color the mouse is pointing to matches the top gui element's color (the overlapping gui must be at the top of all guis for this to work)
    Vector3 mouse_color = Vector3(Mouse::get_color().x, Mouse::get_color().y, Mouse::get_color().z);//Mouse::get_color();
    Vector3 gui_top_color = gui_top.get_color().xyz;//Vector3i(gui_top.get_color().x, gui_top.get_color().y, gui_top.get_color().z);
    if(mouse_color.x != gui_top_color.x && mouse_color.y != gui_top_color.y && mouse_color.z != gui_top_color.z) {
        return;
    }
    //std::cout << "(mouse_color = gui_color): " << (gui_top_color.x == mouse_color.x) << std::endl;
    //std::cout << "mouse_color: " << mouse_color << std::endl;
    //std::cout << "gui_top_color: " << gui_top_color << std::endl;
        // check if gui_top's children's color too
    set_active(true);
    if(Mouse::is_over(gui_top.get_x(), gui_top.get_y(), gui_top.get_width(), gui_top.get_height())) {
            std::cout << "Mouse is pointing at top GUI(" << String(&const_cast<GUI&>(gui_top)) << ") with rgb(" << mouse_color << ")" << std::endl;
            // 3. make overlapped gui (bottom gui) inactive, meaning it cannot receive any user input
            set_active(false);
    }
    std::cout << "this active: " << active << std::endl;*/
}
/////////////
/////////////
/////////////
// CALLBACK
/////////////
void GUI::on_create()
{}
/////////////
/////////////
void GUI::on_draw() // NOTE: position can be set regardless of whether gui is visible or not
{
    generate_shader(); // generate shaders before drawing
    on_parent(); // set child's position relative to its parent, regardless of whether it is visible or not
    //----------
    // slows everything down by a TON for some reason :/
    /*on_hover();
    on_press();
    on_click();*/
    //----------
    on_focus();  // set the GUI's focus, but only if it is visible!
    on_disable(); // set color to gray on disabling GUI
}
/////////////
void GUI::on_draw_no_focus() {
    generate_shader(); // generate shaders before drawing
    on_parent(); // set child's position relative to its parent, regardless of whether it is visible or not
    //----------
    // slows everything down by a TON for some reason :/
    /*on_hover();
    on_press();
    on_click();*/
    //----------    
    on_disable(); // set color to gray on disabling GUI
}
/////////////
void GUI::on_parent() // call this before drawing self
{
    // if self has no parent, exit function
	if(!parent) return;
	// set position of child
	// only the child's position relative to the parent can be changed, meaning the child no longer has a position of its own once the parent is set
	// so using "set_position" on a child GUI makes no sense
	set_position(parent->get_x() + get_relative_x(), parent->get_y() + get_relative_y());
	// make sure child does not go past parent's x bounds - success!
	if(get_relative_x() >= (parent->get_width() - get_width())) { 
	    set_position(parent->get_x() + (parent->get_width() - get_width()), get_y()); 
	    // fix the incorrect relative x position
	    // set the relative position to the parent_width - child_width
	    set_relative_position(parent->get_width() - get_width(), get_relative_y());	
	}
	if(get_x() <= parent->get_x()) set_position(parent->get_x(), get_y());// child_x = parent->get_x() + get_relative_x()
	// make sure child does not go past parent's y bounds - success!
	if(get_y() <= parent->get_y()) set_position(get_x(), parent->get_y());// up// child_y = parent->get_y() + get_relative_y() 
	if(get_relative_y() >= (parent->get_height() - get_height())) {
	    set_position(get_x(), parent->get_y() + (parent->get_height() - get_height()));
	    // fix the incorrect relative y position
	    // set the relative position to the parent_height - child_height
	    set_relative_position(get_relative_x(), (parent->get_height() - get_height()));
	}// down//{std::cout <<"child going outside parent y bounds!\n";}//{set_position(get_x(), parent->get_y());} // REMINDER: shrink size of button and retest this function again	    
#ifdef DOKUN_DEBUG0	    
	std::cout << "child_x: " << get_x() << std::endl;
	std::cout << "child_y: " << get_y() << std::endl;
	std::cout << "child_rel_x: " << get_relative_x() << std::endl;
	std::cout << "child_rel_y: " << get_relative_y() << std::endl; // bug: relative position goes out of bound [fixed]
#endif
}
/////////////
void GUI::on_focus() {
    // if self is not visible then exit the function
	if(!visible) return; // ONLY if self is visible (child GUI can also have their own visibility)
	if(disabled) return; // disabled GUI cannot receive focus
	if(!active) return;
	// if self is pressed, set it as the current "focused" GUI
	if(Mouse::is_over(get_rect()) && Mouse::is_pressed(1)) set_focus(true);////if(is_pressed()) set_focus  (true);
	// if mouse is pressed elsewhere (does NOT work - will only work on one GUI of a certain class)
	//else if(!Mouse::is_over(get_rect()) && Mouse::is_pressed(1)) set_focus(false);   
	#ifdef DOKUN_DEBUG0
	    if(GUI::focused) std::cout << String(String::no_digit( typeid(*this).name()) ).str() << ": " << String(GUI::focused) << " gained focus" << std::endl; // #include <typeinfo>
		if(GUI::focused == nullptr) std::cout << "focus lost" << std::endl;  
	#endif
}
/////////////
void GUI::on_disable() {
    if(!disabled) return;
    set_color(128, 128, 128, 1.0);
}
/////////////
// this is for drawing GUI children, but its best to draw the children manually
// so this function is marked for deprecation and will soon be deprecated!
//void GUI::on_draw_after() // call this after drawing self (children are drawn last, meaning they are drawn on top of parent)
//{
//	if(!is_visible()) return; // ONLY if self is visible, then draw all children (child GUI can also have their own visibility)
//	for(int i = 0; i < Factory::get_gui_factory()->get_size(); i++)
//	{
//		GUI * gui = static_cast<GUI*>(Factory::get_gui_factory()->get_object(i));
//		if(gui->parent == this && gui->visible) gui->draw(); // draw all children of self
//	}
//}
/////////////
/////////////
/////////////
/////////////
// EVENTS
/////////////
bool  GUI::is_hovered()
{	
	int x = 0;
	int y = 0;
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif	
	return Mouse::is_over(get_rect());
}
///////////// 
int GUI::is_hovered(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_hovered());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
///////////// 
bool GUI::is_pressed() // executes multiple times
{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif
	if(!visible) return false; // gui must be visible first
	if(disabled) return false; // gui must not be disabled
	if(!active) return false; // gui must not be inactive
	//////////////////////////
	// Mouse::get_color does not work unless it is called after the draw function :/
    //std::cout << "mouse_color: " << Mouse::get_color() << std::endl;
	//////////////////////////
	return (Mouse::is_over(get_rect()) && Mouse::is_pressed(1)/* && (Mouse::get_color() == get_color().xyz)*/);//return (Mouse::is_over(get_rect()) && Mouse::is_pressed(1));
}
///////////// 
int GUI::is_pressed(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_pressed());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
///////////// 
bool GUI::is_selected() // executes once
{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif	
	return false;
}
///////////// 
/*
int GUI::is_(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
*/
///////////// 
///////////// 
bool GUI::is_released() // if released (only right after button is pressed)
{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif	
	return false;
}
///////////// 
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
bool GUI::is_dragged()
{	
    if(is_draggable())
	{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif		
	}
	return false;
}
///////////// 
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
bool GUI::is_dropped() // returns true if not dragging
{
	if(is_droppable())
	{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif		
	}
	return false;
}
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
bool GUI::is_resized()
{
	if(is_resizeable())
	{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif		
	}
	return false;
}
/////////////
// might not need these two functions after all ...
bool GUI::has_shader() {
    if(!gui_shader) return false; // if shader is null, return false
#ifdef DOKUN_OPENGL
    if(gui_shader->has_program()) return true; // check if shader has a valid program // every shader object has a program at the time of their creation (edit: not anymore)
#endif
    return false;
}
/////////////
bool GUI::is_shader_generated() {
    if(!gui_shader) return false; // if shader is null, return false
#ifdef DOKUN_OPENGL
    if(gui_shader->has_program()) return true; // check if shader has a valid program
    if(gui_shader->is_linked()) return true; // check if at least 2 shaders have been linked to the shader_program
#endif
    return false;
}
/////////////
//if(!gui_shader) throw std::runtime_error("GUI Shader is not initialized");
/////////////
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
/////////////
