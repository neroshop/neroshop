#include "../include/image.hpp"
/////////////
Image::Image() : width(0), height(0), depth(1), channel(4), data (nullptr), x(0), y(0), angle(0), scale(1, 1), color(255, 255, 255, 1.0), 
    relative_x(0), relative_y(0), alignment("none"), visible(true),
    // outline
    outline(false), outline_thickness(0.0), outline_color(0, 0, 0, 1.0), outline_threshold(0.0)
{
#ifdef DOKUN_OPENGL	
	buffer = 0;	
    mag_filter = GL_LINEAR; // texture should use nearest while image use linear since image is intended to be used for GUI so it has to look smooth
    min_filter = GL_LINEAR;
    wrap_s = GL_CLAMP_TO_BORDER;
 	wrap_t = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format = GL_RGBA;
#endif			
	Factory::get_image_factory()->store(this);
}
/////////////
Image::Image(const Image& image) : Image()
{
	copy(image);
}
/////////////
Image::Image(const Texture& texture) : Image()
{
	copy(texture);
}
/////////////
Image::Image(const std::string& file_name) : Image()
{
	if(!load(file_name)) {
		dokun::Logger("Could not load " + file_name); // channel will be obtained once image has successfully loaded and the proper format will be set inside Image::load()
	}
}
/////////////
Image::Image(void * data, int width, int height, int depth, int channel) : Image()
{
	if(!load(data, width, height, depth, channel))
		dokun::Logger("Could not load image from data"); // channel will be obtained once image has successfully loaded and the proper format will be set inside Image::load()
}
/////////////
Image::~Image(void)
{
	destroy(); // destroy texture buffer obj and delete image pixel data
	Factory::get_image_factory()->release(this);
	//std::cout << "image deleted\n";
}
/////////////
bool Image::load(const std::string& file_name) // from file
{
	std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
	if(extension == "png") return load_png(file_name);
	if(extension == "jpeg" || extension == "jpg"  || extension == "jpe" || extension == "jif" || extension == "jfif" || extension == "jfi") 
		return load_jpeg(file_name);
	if(extension == "bmp" || extension == "dib") return load_bmp(file_name);
	if(extension == "gif") return load_gif(file_name);
	if(extension == "tif" || extension == "tiff") return load_tiff(file_name);
	dokun::Logger("Unsupported format " + extension);
	dokun::Logger("Loading as .PNG by default");
    return load_png(file_name); // default
}
/////////////
bool Image::load(void * data , int width, int height, int depth, int channel) // from data
{
	this->data    = static_cast<unsigned char *>(data);
	this->width   = width;
	this->height  = height;
	this->depth   = depth;
	this->channel = channel;
#ifdef DOKUN_OPENGL	
	format = (channel == 4) ? GL_RGBA : GL_RGB;
#endif	
	return true;
}
/////////////
int Image::load(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or userdata
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
        Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TLIGHTUSERDATA)
		{
			// get width
			luaL_checktype(L, 3, LUA_TNUMBER);
			// get height
			luaL_checktype(L, 4, LUA_TNUMBER);
			// get data
			void * data = static_cast<void *>(lua_touserdata(L, 2));
			// load data
			lua_pushboolean(L, image->load(data, (int)lua_tonumber(L, 3), (int)lua_tonumber(L, 4)));
		    return 1;
		}
		if(lua_type(L, 2) == LUA_TSTRING)
		{
		    lua_pushboolean(L, image->load(lua_tostring(L, 2)));
			return 1 ;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void Image::draw ()
{
	if(!visible) return;
	generate(); // generate texture
	generate_shader(); // generate shaders
	Renderer::draw_image(buffer, width, height, depth, x, y, angle, scale.x, scale.y, color.x, color.y, color.z, color.w, channel, image_shader, outline, outline_thickness, outline_color, outline_threshold); // pass actual width and height, instead of get_width() and get_height as it also includes the scale factor
}
/////////////
void Image::draw(double x, double y)
{
	this->x = x;
	this->y = y;
	draw();
}
/////////////
int Image::draw (lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));

        if((lua_type(L, 2) == LUA_TNUMBER) &&
	    (lua_type(L, 3) == LUA_TNUMBER))
	    {
	        image->draw (lua_tonumber(L, 2), lua_tonumber(L, 3));
	    }
		else
		{
			image->draw();
		}
	}
	return 0;
}
/////////////
bool Image::save(std::string file_name)
{
	if(file_name.empty()) // to prevent crash
	{
		file_name = "Image" + std::to_string(Factory::get_image_factory()->get_location(this)) + ".png";
	}
	FILE *fp = fopen(file_name.c_str(), "wb"); // create file in write mode
    if (!fp)
	{
		std::cout << "Could not create " << file_name << std::endl;
		return false;
	}
	return true;
}
/////////////
int Image::save(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->save(lua_tostring(L, 2));
	}
	return 0;
}
/////////////
void Image::copy(const Image& image) // copies another image's texture pixels - no need to copy the position, angle, scale, color, relative_position, nor alignment - or it will just mess up everything
{
    data = image.data;//static_cast<unsigned char *>(image.data);
	width = image.width; // must be original width
	height = image.height; // must be original height
	depth = image.depth;
	channel = image.channel;
	file = image.file; // this is not neccessary but whatever ...// copy geometric transformations or nah ?//set_scale(image.scale.x, image.scale.y);
#ifdef DOKUN_OPENGL
    // buffer must be unique like the Image itself
	min_filter = image.get_filter().x;
	mag_filter = image.get_filter().y;
	wrap_s = image.get_wrap().x;
	wrap_t = image.get_wrap().y;
	internal_format = image.get_internal_format();
	format = image.get_format();
    // check for opengl context
    Renderer::context_check();
    // delete old buffer (so we can re-create a new buffer with the copied image's data)
    destroy();
    // generate a new buffer that takes in the copied image's data (this may not be needed since the draw call can also generate a texture automatically)
    generate();
#endif
}
/////////////
void Image::copy(const Texture& texture) // same as Image:copy_texture in Lua
{
    data = static_cast<unsigned char *>(texture.get_data()); // copy the pixel data as well
    width = texture.width; // must be original width
	height = texture.height; // must be original height
	depth = texture.get_depth();
	channel = texture.get_channel();
	file = texture.get_file(); // this is not neccessary but whatever ...
#ifdef DOKUN_OPENGL
	//buffer     = texture.get_buffer(); // buffer must be unique like the Image itself (so DO NOT copy!!)
	min_filter = texture.get_filter().x;
	mag_filter = texture.get_filter().y;
	wrap_s = texture.get_wrap().x;
	wrap_t = texture.get_wrap().y;
	internal_format = texture.get_internal_format();
	format = texture.get_format();	
    // check for opengl context
    Renderer::context_check();
    // delete old buffer (so we can re-create a new buffer with the copied image's data)
    destroy();
    // generate a new buffer that takes in the copied image's data (this may not be needed since the draw call can also generate a texture automatically)
    generate();
#endif
}               
/////////////
int Image::copy(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image1 = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
			image->copy(*image1);
		}
	}
	return 0;
}
/////////////
int Image::copy_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
			image->copy(*texture);
		}
	}
	return 0;	
}
/////////////
void Image::flip(int x, int y)
{
	if(x) scale = Vector2(-x, y);
	if(y) scale = Vector2(x, -y);
}
/////////////
int Image::flip(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->flip(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
/////////////
void Image::resize(int width, int height)
{
	int old_width = this->width; // must be original width
	int old_height = this->height; // must be original height
	set_scale(width / static_cast<double>(old_width), height / static_cast<double>(old_height));
}
/////////////
void Image::resize(const Vector2& size)
{
	resize(size.x, size.y);
}
/////////////
int Image::resize(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->resize(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
	}
    return 0;	
}
/////////////
void Image::scale_to_fit(int rect_width, int rect_height)
{
	double aspect = get_aspect_ratio_correction(rect_width, rect_height);
	set_scale(aspect, aspect);
}
/////////////
void Image::scale_to_fit(const Vector2& rect_size)
{
	scale_to_fit(rect_size.x, rect_size.y);
}
/////////////
void Image::generate()
{
#ifdef DOKUN_OPENGL
    // check for opengl context first
    Renderer::context_check();
	if(!glIsTexture(buffer)) // no buffer yet (generate a single buffer and no more than 1)
	{
		glGenTextures(1, &buffer); // generate a buffer
		glBindTexture(GL_TEXTURE_2D, buffer); // bind buffer
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, static_cast<GLenum>(format), GL_UNSIGNED_BYTE, static_cast<GLvoid *>(data)); // pass texture width, height, and data to OpenGL
        glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps
		glBindTexture(GL_TEXTURE_2D, 0); // unbind buffer
	#ifdef DOKUN_DEBUG0
	    if(glIsTexture(buffer)) std::cout << "image buffer_id " << buffer << " generated\n";//dokun::Logger("Image" + String(Factory::get_image_factory()->get_location(this)).str() + " buffer " + std::to_string(buffer) + " generated");
    #endif	
	}
#endif	
}
/////////////
int Image::generate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
        image->generate();
	}
    return 0;	
}
/////////////
void Image::destroy()
{
#ifdef DOKUN_OPENGL	
    // check for opengl context first to prevent a crash
    ////Renderer::context_check(); // message keeps spamming which is annoying xD
    if(glIsTexture(buffer) && buffer != 0) {
	    unsigned int buffer_temp = buffer;
        glDeleteTextures(1, static_cast<GLuint *>(&buffer));
        buffer = 0;	// set to null so that we know the buffer is deleted for sure
	#ifdef DOKUN_DEBUG0
	    if(!glIsTexture(buffer_temp)) std::cout << "image buffer_id " << buffer_temp << " destroyed\n";// confirm that buffer is really deleted//dokun::Logger("Image::destroy(): " + String(Factory::get_image_factory()->get_location(this)).str() + " buffer destroyed");
    #endif
#endif	
	}// how to delete image pixel data hmm ...
}
/////////////
int Image::destroy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
        image->destroy();
	}
    return 0;	
}
/////////////
void Image::clear() // find a way to update textures with default pixels
{
#ifdef DOKUN_OPENGL	
    // check for opengl context first
    Renderer::context_check();
    /*unsigned char * empty_data = new unsigned char[width * height * 4];//(unsigned char *)get_data();//new unsigned char[width * height * 4]; // create an empty image
    size_t empty_data_size = sizeof(empty_data)/sizeof(empty_data[0]); // get size of empty image
    std::cout << "size of empty data: " << empty_data_size << std::endl;
    std::cout << "size of (width * height * 4): " << (width * height * 4) << std::endl;*/
    // fill empty_data with zeros
    //memset(empty_data, 0x00, width * height * 4 * sizeof(unsigned char));
    //empty_data[] = 
    //for(int i = 0; i < (width * height * 4)/*empty_data_size*/; i++)
    //    empty_data[i] = 0;
   /* for( int y = 0; y < 128; y++ )
{
    for( int x = 0; x < 128; x++ )
    {
        // Fill each pixel with red.
        const unsigned char idx = (y * (128 * 4)) + (x * 4);
        empty_data[idx + 0] = 0xff;
        empty_data[idx + 1] = 0x00;
        empty_data[idx + 2] = 0x00;
        empty_data[idx + 3] = 0xff;
    }
}*/
// generate new texture buffer    
//generate();
/*
glBindTexture(GL_TEXTURE_2D, buffer);
glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1024, 768, GL_RGBA, GL_UNSIGNED_BYTE, empty_data);
glBindTexture(GL_TEXTURE_2D, 0);
delete empty_data;
*/
/*
std::vector<GLubyte> emptyData(width * height * 4, 0);
glBindTexture(GL_TEXTURE_2D, buffer);
glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, &emptyData[0]);

glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default framebuffer
glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffer, 0); //Only need to do this once.
glDrawBuffer(GL_COLOR_ATTACHMENT0); //Only need to do this once.
GLuint clearColor[4] = {0, 0, 0, 0};
glClearBufferuiv(GL_COLOR, 0, clearColor);
*/
    // ...
        /*glClearTexSubImage(	buffer,
 	0,//level
 	0,//GLint xoffset, // left
 	0,//GLint yoffset, // top
 	0,//GLint zoffset, // front
 	width,
 	height,
 	depth,
 	format,
 	GL_UNSIGNED_BYTE,
 	empty_data);*/ // glClearTexSubImage is available only if the GL version is 4.4 or greater.
 	// clear texture first
    //if(is_generated()) glClearTexImage(buffer, 0, format, GL_UNSIGNED_BYTE, 0/*empty_data*/); // glClearTexImage is available only if the GL version is 4.4 or greater.
    // destroy old texture buffer
    destroy();
#endif
}
int Image::clear(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
        image->clear();
	}
    return 0;
}
/////////////
/////////////
Shader * Image::image_shader (new Shader());
/////////////
void Image::generate_shader(void) {
    if(image_shader->is_linked()) return; // if image_shader is already generated and linked, exit function
	////////////////////////////
    // vertex shader
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
		"uniform mat4 view ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "tex_coords = vec2(tex_coord.x, 1.0 - tex_coord.y);\n"
            "gl_Position = proj * view * model * vec4(position, 0.0, 1.0);\n" // 2:image uses camera (via "view")
        "}\n"
	};
	// fragment shader
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D base;\n"
		"in vec2 tex_coords;\n"
		"uniform bool has_texture;\n"
		"\n"
		"uniform bool outline = false;\n"
		"uniform float outline_thickness = 0.2;\n"
		"uniform vec4 outline_color = vec4(0, 0, 0, 1.0);\n"
		"uniform float outline_threshold = 0.5;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "if(has_texture == false)\n"
		    "{\n"
                "out_color = color;\n"
            "}"	
		    "if(has_texture == true)\n"
		    "{\n"
                "out_color = color * texture(base, tex_coords);\n"
            //////////////////////////////////////////////////////
                "if (outline && out_color.a <= outline_threshold) {\n"
                    "ivec2 size = textureSize(base, 0);\n"
                    "\n"
                    "float uv_x = tex_coords.x * size.x;\n"
                    "float uv_y = tex_coords.y * size.y;\n"
                    "\n"
                    "float sum = 0.0;\n"
                    "for (int n = 0; n < 9; ++n) {\n"
                        "uv_y = (tex_coords.y * size.y) + (outline_thickness * float(n - 4.5));\n"
                        "float h_sum = 0.0;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x - (4.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x - (3.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x - (2.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x - outline_thickness, uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x, uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x + outline_thickness, uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x + (2.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x + (3.0 * outline_thickness), uv_y), 0).a;\n"
                        "h_sum += texelFetch(base, ivec2(uv_x + (4.0 * outline_thickness), uv_y), 0).a;\n"
                        "sum += h_sum / 9.0;\n"
                    "}\n"
                    "\n"
                    "if (sum / 9.0 >= 0.0001) {\n"
                        "out_color = vec4(outline_color.xyz, outline_color.w);\n"
                    "}\n"
                "}\n"
        //////////////////////////////////////////////////////
            "}"
		"}\n"
	};	
	////////////////////////////
    if(!image_shader->has_program())
	{
		image_shader->create();
		image_shader->set_source(vertex_source  , 0);
		image_shader->set_source(fragment_source, 1);
		image_shader->prepare();
	    ////std::cout << DOKUN_UI_TAG "Image shaders have been generated (" << image_shader->get_program() << ")" << std::endl;//std::cout << "number of shaders attached to shader object program: " << image_shader->get_shader_count() << std::endl;
	}
}
/////////////
/////////////
// SETTERS
/////////////
void Image::set_x(double x)
{
	this->x = x;
}
/////////////
int Image::set_x(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // x
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_x(lua_tonumber(L, 2));
		// set in lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "x");
	}		
	return 0;
}
/////////////
void Image::set_y(double y)
{
	this->y = y;
}
/////////////
int Image::set_y(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_y(lua_tonumber(L, 2));
		// set in lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "y");		
	}		
	return 0;
}
/////////////
void Image::set_position(double x, double y)
{
	set_x(x);
	set_y(y);
}
/////////////
void Image::set_position(const Vector2& position)
{
	set_position(position.x, position.y);
}
/////////////
int Image::set_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // x
	luaL_checktype(L, 3, LUA_TNUMBER); // y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_position(lua_tonumber(L, 2), lua_tonumber(L, 3));
		// set in lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "y");				
	}		
	return 0;
}
/////////////
void Image::set_relative_position(double x, double y)
{
	relative_x = x;
	relative_y = y;
}
/////////////
void Image::set_relative_position(const Vector2& position)
{
	set_relative_position(position.x, position.y);
}
/////////////
int Image::set_relative_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // relative_x
	luaL_checktype(L, 3, LUA_TNUMBER); // relative_y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_relative_position(lua_tonumber(L, 2), lua_tonumber(L, 3));
		// set in lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "relative_x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "relative_y");				
	}		
	return 0;	
}
/////////////
void Image::set_angle(double angle) // in degrees
{
	this->angle = angle;
}
/////////////
int Image::set_angle(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // angle
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_angle(lua_tonumber(L, 2));
		// set in lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "angle");		
	}		
	return 0;
}
/////////////
void Image::set_scale(double sx, double sy)
{
	scale = Vector2(sx, sy);
	resized = (sx == 1.0 && sy == 1.0) ? false : true;
#ifdef  DOKUN_DEBUG0
	std::cout << "New size after scale: " << get_size() << "" << std::endl;
#endif		
}            
///////////// 
void Image::set_scale(const Vector2& scale)
{
	set_scale(scale.x, scale.y);	
}
/////////////
int Image::set_scale(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // scale_x
	luaL_checktype(L, 3, LUA_TNUMBER); // scale_y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_scale(lua_tonumber(L, 2), lua_tonumber(L, 3));
		// set in lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "scale_x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "scale_y");				
	}	
	return 0;
}
/////////////
void Image::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
/////////////
void Image::set_color(unsigned int red, unsigned int green, unsigned int blue, float alpha)
{
	color = Vector4(red, green, blue, alpha);
}
/////////////
void Image::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}           
/////////////
void Image::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int Image::set_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // red
	luaL_checktype(L, 3, LUA_TNUMBER); // green
	luaL_checktype(L, 4, LUA_TNUMBER); // blue
	luaL_optnumber(L, 5, 1.0);         // alpha
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "red");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "green");
		lua_pushvalue(L, 4);
		lua_setfield(L, 1, "blue");
		lua_pushvalue(L, 5);
		lua_setfield(L, 1, "alpha");								
	}		
	return 0;
}
/////////////
void Image::set_alignment(const std::string& alignment)
{
	this->alignment = alignment;
}
/////////////
int Image::set_alignment(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_alignment(lua_tostring(L, 2));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "alignment");		
	}
    return 0;	
}
/////////////
void Image::set_visible(bool visible)
{
	this->visible = visible;
}
int Image::set_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_visible(lua_toboolean(L, 2));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "visible");		
	}
    return 0;		
}
/////////////
void Image::set_outline(bool outline) {
    this->outline = outline;
}
/////////////
void Image::set_outline_thickness(float outline_thickness) {
    this->outline_thickness = outline_thickness;
}
/////////////
void Image::set_outline_color(unsigned int red, unsigned int green, unsigned int blue) {
	outline_color = Vector4(red, green, blue, outline_color.w);
}
/////////////
void Image::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, float alpha) {
	outline_color = Vector4(red, green, blue, alpha);
}
/////////////
void Image::set_outline_color(const Vector3& outline_color) {
    set_outline_color(outline_color.x, outline_color.y, outline_color.z);
}
/////////////
void Image::set_outline_color(const Vector4& outline_color) {
    set_outline_color(outline_color.x, outline_color.y, outline_color.z, outline_color.w);
}
/////////////
void Image::set_outline_threshold(float outline_threshold) {
    this->outline_threshold = outline_threshold;
}
/////////////
/////////////
/////////////
void Image::set_filter(int min_filter, int mag_filter)
{
	this->min_filter = min_filter;
	this->mag_filter = mag_filter;
}
int Image::set_filter(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_wrap(int wrap_s, int wrap_t)
{
	this->wrap_s = wrap_s;
	this->wrap_t = wrap_t;
}
int Image::set_wrap(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_internal_format(int internal_format)
{
	this->internal_format = internal_format;
}
int Image::set_internal_format(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_format(unsigned int format)
{
	this->format = format;
}
int Image::set_format(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_param(int min_filter, int mag_filter, 
	int wrap_s, int wrap_t, 
	int internal_format, unsigned int format)
{
	set_filter(min_filter, mag_filter);
	set_wrap(wrap_s, wrap_t);
	set_internal_format(internal_format);
	set_format(format);
}
int Image::set_param(lua_State *L)
{
    return 0;
}
/////////////
/////////////
// GETTERS
/////////////
// if the scale is zero, then the size will also be zero, so scale must always be 1
int Image::get_width () const
{
    return width * scale.x; // in case of set_scale, will return scaled width else original width
}
/////////////
int Image::get_width(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_width());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
int Image::get_height () const
{
    return height * scale.y; // in case of set_scale, will return scaled height else original height
}
/////////////
int Image::get_height(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_height());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
int Image::get_depth() const
{
	return depth;
}
int Image::get_depth(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_depth());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
Vector2 Image::get_size() const
{
    return Vector2(get_width(), get_height());
}
/////////////
int Image::get_size(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_size().x);
		lua_pushnumber(L, image->get_size().y);
		return 2;
	}
    lua_pushnil(L);
	lua_pushnil(L);
    return 2;
}
/////////////
/////////////
void * Image::get_data() const
{
    return data;
}
/////////////
int Image::get_data(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, image->get_data());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
std::string Image::get_file() const
{
	return file;
}
/////////////
int Image::get_file(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushstring(L, image->get_file().c_str());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
int Image::get_channel()        const
{
	return channel;
}
/////////////
int Image::get_channel(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_channel());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Image::get_buffer()const
{
	return buffer;
}
int Image::get_buffer(lua_State *L)
{
    return 1;
}
/////////////
Vector2i Image::get_filter()const
{
	return Vector2i(min_filter, mag_filter);
}
int Image::get_filter(lua_State *L)
{
    return 1;
}
/////////////
Vector2i Image::get_wrap()const
{
	return Vector2i(wrap_s, wrap_t);
}
int Image::get_wrap(lua_State * L)
{
    return 1;
}
/////////////
int Image::get_internal_format() const
{
	return internal_format;
}
int Image::get_internal_format(lua_State *L)
{
    return 1;
}
/////////////
unsigned int Image::get_format()         const
{
	return format;
}
/////////////
int Image::get_format (lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_format());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 Image::get_color()      const
{
	return color;
}
/////////////
int Image::get_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_color().x);
		lua_pushnumber(L, image->get_color().y);
		lua_pushnumber(L, image->get_color().z);
		lua_pushnumber(L, image->get_color().w);
		return 4;
	}			
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;		
}
/////////////
double Image::get_x()const
{
	return x;
}
/////////////
int Image::get_x(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_x());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
double Image::get_y()const
{
	return y;
}
/////////////
int Image::get_y(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_y());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
Vector2 Image::get_position()const
{
	return Vector2(get_x(), get_y());
}
/////////////
int Image::get_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_position().x);
		lua_pushnumber(L, image->get_position().y);
		return 2;
	}	
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
double Image::get_relative_x()         const
{
	return relative_x;
}
/////////////
int Image::get_relative_x(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_relative_x());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
double Image::get_relative_y()         const
{
	return relative_y;
}
/////////////
int Image::get_relative_y(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_relative_y());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
Vector2 Image::get_relative_position() const
{
	return Vector2(get_relative_x(), get_relative_y());
}
/////////////
int Image::get_relative_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_relative_position().x);
		lua_pushnumber(L, image->get_relative_position().y);
		return 2;
	}	
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
double Image::get_angle()const
{
	return angle;
}
/////////////
int Image::get_angle(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_angle());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
Vector2 Image::get_scale()const
{
	return scale;
}
/////////////
int Image::get_scale(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_scale().x);
		lua_pushnumber(L, image->get_scale().y);
		return 2;
	}			
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
double Image::get_aspect_ratio_correction(int rect_width, int rect_height) //const // scale to fit inside a rect
{
	double image_aspect = width / height; // use the raw width and height. Do not use (size * scale) to get aspect ratio or it will screw everything up!!
	double rect_aspect = rect_width / rect_height;
    double scale_factor = 1.0; // default
    // calculate scale factor from aspect ratio
   	if(rect_aspect >= image_aspect) {
		scale_factor = rect_height / static_cast<double>(height);
	} 
	else if(image_aspect <= rect_aspect) {// what if both aspect ratios are equal (this just mean image's width and height are the same value; same goes for the rect)
	    scale_factor = rect_width / static_cast<double>(width);
	}
#ifdef DOKUN_DEBUG0
	std::cout << "image original size: " << Vector2(width, height) << std::endl;
	std::cout << "image ascpect: " << image_aspect << std::endl;
	std::cout << "box ascpect: " << rect_aspect << std::endl;
	std::cout << "aspect_ratio: " << scale_factor << std::endl;
#endif	
	return scale_factor;
}
/////////////
int Image::get_aspect_ratio_correction(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_aspect_ratio_correction(lua_tonumber(L, 2), lua_tonumber(L, 3)));
        return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
/////////////
std::string Image::get_alignment() const
{
	return alignment;
}
int Image::get_alignment(lua_State *L)
{
    return 1;
}
/////////////
Vector4 Image::get_rect() const
{
	return Vector4(x, y, get_width(), get_height());
}
/////////////
int Image::get_rect(lua_State * L)
{
    return 4;
}
/////////////
float Image::get_outline_thickness() const {
    return outline_thickness;
}
/////////////
Vector4 Image::get_outline_color() const {
    return outline_color;
}
/////////////
float Image::get_outline_threshold() const {
    return outline_threshold;
}
/////////////
/////////////
/////////////
/////////////
/////////////
// BOOLEAN
/////////////
bool Image::is_png()
{
    png_byte header[8];
	std::FILE * fp;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&fp, get_file().c_str(), "rb");
	#else
        fp = fopen(get_file().c_str(), "rb");
    #endif
    if (!fp) // file not found !
    {
		return false;
    }
    fread(header, 1, 8, fp);
    bool png = !png_sig_cmp(header, 0, 8);
    if (!png)
    {
	    fclose(fp);
		return false;
    }
	fclose(fp);
    return true;
}
/////////////
int Image::is_png(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, image->is_png());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool Image::is_image() // if image is loaded, it returns true
{
	return (is_png()); // only format we can load for now
}
/////////////
int Image::is_image(lua_State *L)
{
	/*
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, image->is_image());
        return 1;
	}
    lua_pushboolean(L, false);
    return 1;
	*/
	luaL_checktype(L, 1, LUA_TTABLE); // 1
	lua_getmetatable(L, 1); // 2
	lua_getglobal(L, "Image"); // 3
#ifdef DOKUN_LUA51
	lua_pushboolean(L, lua_equal (L, 2, 3));
#endif
#ifndef DOKUN_LUA51
	lua_pushboolean(L, lua_compare(L, 2, 3, LUA_OPEQ));
#endif
	return 1;
}
/////////////
bool Image::is_resized() const
{
	return (resized == true);
}
int Image::is_resized(lua_State *L)
{
    return 1;
}
/////////////
bool Image::is_generated()const
{
#ifdef DOKUN_OPENGL
    // check for opengl context first
    Renderer::context_check();
    return (glIsTexture(buffer) == true);
#endif	
    return false;	
}
int Image::is_generated(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, image->is_generated());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;			
}
/////////////
bool Image::is_visible()const
{
	return visible;
}
/////////////
int Image::is_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushboolean(L, image->is_visible());
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool Image::has_outline() const {
    return (outline && (outline_thickness > 0.0));
}
/////////////
/////////////
bool Image::load_png(std::string file_name)
{
// header for testing if it is a png
    int number=8;

    png_byte header[8];
    // open the file
	std::FILE * fp;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&fp, file_name.c_str(), "rb");
	#else
        fp = fopen(file_name.c_str(), "rb");
    #endif
    if (!fp)
    {
        return false;
    }
	else
	{
		file =(file_name);
	}
    // read the header
    fread(header, 1, number, fp);
    int is_png = !png_sig_cmp(header, 0, number);
    if (!is_png)
	{
        fclose(fp);
        return false;
    }
   // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
        nullptr, nullptr);
    if (!png_ptr)
	{
        fclose(fp);
        return false;
    }
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
     fclose(fp);
     return false;
   }
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
     fclose(fp);
     return false;
   }
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return false;
   }
   //init png reading
   png_init_io(png_ptr, fp);
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, number);
   // change zlib compression buffer size when reading compressed data
   //int buffer_size = 8192;
   //png_set_compression_buffer_size(png_ptr, buffer_size);
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
   //variables to pass to get info
   int bit_depth, color_type, interlace_type, compression_type, filter_method;

   //update width and height based on png info
   width =  png_get_image_width(png_ptr, info_ptr);
   height = png_get_image_height(png_ptr, info_ptr);
   // get bit_depth and color_type based on png info
   bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
   color_type = png_get_color_type(png_ptr, info_ptr);
   interlace_type=png_get_interlace_type(png_ptr,
                         info_ptr);
   compression_type=png_get_compression_type(png_ptr,
                         info_ptr);
   filter_method    = png_get_filter_type(png_ptr,
                         info_ptr);

    channel = png_get_channels(png_ptr, info_ptr);
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}

// .. must be called before using png_read_update_info, as well as other setters
   int number_of_passes;
   number_of_passes = png_set_interlace_handling(png_ptr);
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);


   // Allocate the image_data as a big block, to be given to opengl
   data = new png_byte[rowbytes * height];
   if (!data)
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return false;
   }

   //row_pointers is for pointing to data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers)
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     data = nullptr;
     fclose(fp);
     return false;
   }
   // set the individual row_pointers to point at the correct offsets of data
   for (unsigned int i = 0; i < height; ++i)
   {
     row_pointers[height - 1 - i] = (png_byte*)data + i * rowbytes;
   }
    // save row pointers
   rdata = row_pointers;//reinterpret_cast<unsigned char *>(row_pointers);
   // the actual reading of the png file
   //read the png into data through row_pointers
   png_read_image(png_ptr, row_pointers);

   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(fp);
   return true;
}
/////////////
bool Image::load_jpeg(std::string file_name)
{
	FILE * file = fopen(file_name.c_str(), "rb");
	if(!file) { return false;}

	struct jpeg_decompress_struct info;  //for our jpeg info
	struct jpeg_error_mgr        error;  //the error handler
	//jmp_buf jmp_buffer;
	unsigned char * rowptr;    // pointer to an array   // JSAMPARRAY buffer;
    int row_stride;		// physical row width in output buffer

    info.err = jpeg_std_error(&error);

    jpeg_create_decompress(&info); // create decompression object
	jpeg_stdio_src  (&info, file); // specify data source (e.g a file)
	jpeg_read_header(&info, TRUE); // read header
	jpeg_start_decompress (&info); // decompress the file

	width  = info.output_width ;
	height = info.output_height;

	row_stride = width * info.output_components;


	channel = info.num_components; //  3 =>RGB   4 =>RGBA
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	int size = width * height * channel;
	data = new unsigned char [size];

	while (info.output_scanline < info.output_height)
	{
		rowptr = static_cast<unsigned char *>(data) + info.output_scanline * info.output_width * channel;
		jpeg_read_scanlines(&info, &rowptr, 1);
	}

	jpeg_finish_decompress (&info);   //finish decompressing
	jpeg_destroy_decompress(&info);
    fclose(file);                    //close the file
	return true;
}
/////////////
bool Image::load_bmp(std::string file_name)
{
    FILE * file = fopen(file_name.c_str(), "rb");
	if(file == nullptr) {
		return false;
	}
	unsigned char header[54]; // 54 byte header (contains information about a bitmap)
	fread(header, sizeof(unsigned char), 54, file); // read header

	width  = *(unsigned int *)&header[18]; // 18 = width (offset)
	height = *(unsigned int *)&header[22]; // 22 = height (offset)
	channel = 4;
	unsigned int size = channel * width * height;
	unsigned char * image = new unsigned char[size];

	fread(image, sizeof(unsigned char), size, file); // read the rest of the data at once

	for(int i = 0; i < size; i += 3)
	{
		unsigned char t = image[i];
		image[i]    = image[i + 2];
		image[i + 2] =           t;
	}
	data = image;
	format = DOKUN_RGB;
	fclose(file);
	return true;
}
/////////////
bool Image::load_tga(std::string file_name)
{
	return false;
}
/////////////
bool Image::load_gif(std::string file_name)
{
	int error         = D_GIF_SUCCEEDED;
	GifFileType * gif = DGifOpenFileName(file_name.c_str(), &error); // open gif
	if(gif == nullptr)          {
		std::cerr << GifErrorString(D_GIF_ERR_OPEN_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
		return false;
	}
	if(DGifSlurp(gif) != GIF_OK) {                             // read gif
		std::cerr << GifErrorString(D_GIF_ERR_READ_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
		return false;
	}
	if(gif->ImageCount <= 0)     {
		dokun::Logger("Gif does not contain any image.");
		return false;
	}
    //-------------------
    SavedImage* first_image = &gif->SavedImages[0];
    ColorMapObject* color_map = first_image->ImageDesc.ColorMap ?
       first_image->ImageDesc.ColorMap : gif->SColorMap;
    width   = first_image->ImageDesc.Width;
    height  = first_image->ImageDesc.Height;
    channel = 3;
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	data = new unsigned char[width * height * channel];

	for (int i = 0; i < height; ++i)
	{
        unsigned char * p_dst = static_cast<unsigned char *>(data) + i * width * channel; // row pointer
        for (int j = 0; j < width; ++j)
		{
            GifByteType color_index = first_image->RasterBits[i * width + j];
            const GifColorType& gif_color = color_map->Colors[color_index];
            p_dst[j * channel + 0] = gif_color.Red;
            p_dst[j * channel + 1] = gif_color.Green;
            p_dst[j * channel + 2] = gif_color.Blue;
        }
    }
    //-------------------
    if(DGifCloseFile (gif, &error) != GIF_OK) {
		std::cerr << GifErrorString(D_GIF_ERR_CLOSE_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
	}
	return true;
}
/////////////
bool Image::load_tiff(std::string file_name)
{
    TIFF* tiff = TIFFOpen(file_name.c_str(), "r");
    if(tiff == nullptr) {
		return false;
	}
	// --------------------
	TIFFRGBAImage img;
	uint16 bit_per_sample; // 8 for grayscale
	uint16 BitsPerSample;
	tsize_t scanline;
	uint32 row;
	uint32 imagelength;
	char emsg[1024];
	uint32 npixels;
	uint16 SamplesPerPixel;
	uint32 * raster;
	//unsigned char * buf;
	uint32 length;
	// --------------------

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH ,     &width  );
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH,     &height ); // length
	TIFFGetField(tiff, TIFFTAG_IMAGEDEPTH ,     &depth  );
	TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &channel);
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE,   &bit_per_sample);
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	data = new unsigned char[width * height * channel];

	npixels = width * height;
    raster=(uint32 *) _TIFFmalloc(npixels *sizeof(uint32));
	if(raster == nullptr){
		dokun::Logger("Could not allocate memory");
		return false;
	}

    if(TIFFReadRGBAImage(tiff, width, height, raster, 0) == 0)
	{
	    dokun::Logger("Could not read image");
		_TIFFfree(raster);
		return false;
	}
    unsigned char * image = new unsigned char[width * height * channel];
	// process raster data
	for (int i = 0; i < height; ++i) {
	//for (int i = height - 1; i != -1; i--) {
        for (int j = 0; j < width; j++)
        {
            int red   = TIFFGetR(raster[i * width + j]);
            int green = TIFFGetG(raster[i * width + j]);
            int blue  = TIFFGetB(raster[i * width + j]);

			image[j * channel + 0] = TIFFGetR(raster[i * width + j]);
			image[j * channel + 1] = TIFFGetG(raster[i * width + j]);
			image[j * channel + 2] = TIFFGetB(raster[i * width + j]);
        }
    }
	//data = new unsigned char[width * height * channel];
    // --------------------
	//
	//data = image;
	_TIFFfree(raster);
    TIFFClose(tiff);
	return true;
}
/////////////
/////////////
/////////////
int Image::new_(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
	// clear stack
    lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// set metatable
	lua_getglobal(L, "Image");
	lua_setmetatable(L, 1);
	// set userdata
	Image **image = static_cast<Image **>(lua_newuserdata(L, sizeof(Image *)));
	if(!file_name.empty())
	{
	    *image = new Image(file_name);
    }
	else
	{
	    *image = new Image();
	}
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	{
	    return 1;
	}
	lua_pushnil(L);
	return 1;
}
