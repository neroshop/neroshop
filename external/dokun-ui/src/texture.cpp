#include "../include/texture.hpp"

/////////////
Texture::Texture() : width(32), height(32), depth(1), data (nullptr),
    channel(4), rect(0, 0, 0, 0), type("diffuse")
{
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
    format          = GL_RGBA;
	
	target = GL_TEXTURE_2D; 
	level  = 0;
#endif
    Factory::get_texture_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with Texture::new ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif    
}
/////////////
Texture::Texture(const Texture& texture)
{
    copy(texture);
	Factory::get_texture_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with Texture::new ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif  	
}
/////////////
Texture::Texture(const std::string& file_name) : rect(0, 0, 0, 0), type("diffuse")
{
    if(!load(file_name)) 
	{
		dokun::Logger("Could not open texture from file " + file_name, DOKUN_FILE, DOKUN_LINE);
		return;
	}
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format = (channel == 4) ? DOKUN_RGBA : DOKUN_RGB;
	
	target = GL_TEXTURE_2D; 
	level  = 0;	
#endif		
    Factory::get_texture_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with Texture::new ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif       
}
/////////////
Texture::Texture(const std::string& file_name, const std::string& type) : rect(0, 0, 0, 0)
{
    if(!load(file_name)) 
	{
		dokun::Logger("Could not open texture from file " + file_name, DOKUN_FILE, DOKUN_LINE);
		return;
	}
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format = (channel == 4) ? DOKUN_RGBA : DOKUN_RGB;
	
	target = GL_TEXTURE_2D; 
	level  = 0;	
#endif		
	set_type(type);
	Factory::get_texture_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with Texture::new ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif  	
}
/////////////
Texture::Texture(const std::string& file_name, int x, int y, int width, int height) : type("diffuse")
{
	if(!load(file_name)) 
	{
        dokun::Logger("Could not open texture from file " + file_name, DOKUN_FILE, DOKUN_LINE);
        return;
    }
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format = (channel == 4) ? DOKUN_RGBA : DOKUN_RGB;
	
	target = GL_TEXTURE_2D; 
	level  = 0;	
#endif		
    set_rect(x, y, width, height);
	Factory::get_texture_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with Texture::new     ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif 	
}
/////////////
Texture::Texture(const void * data, int width, int height, int depth, int channel) : rect(0, 0, 0, 0), type("diffuse")
{
    if(!load(data, width, height, depth, channel)) 
	{
		dokun::Logger("Could not open texture from data", DOKUN_FILE, DOKUN_LINE);
		return;
	}
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format = (channel == 4) ? DOKUN_RGBA : DOKUN_RGB;
	
	target = GL_TEXTURE_2D; 
	level  = 0;	
#endif
    Factory::get_texture_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been allocated with Texture::new ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif     		
}
/////////////
/////////////
Texture::~Texture(void)
{
    destroy(); // destroy texture buffer obj and texture pixel data
	Factory::get_texture_factory()->release(this);
#ifdef DOKUN_DEBUG 	
	dokun::Logger::push("dokun: " + String(this).str() + " has been deallocated with Texture::~Texture ()");
	dokun::Logger::push("       (index=" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ", total_texture_count=" + std::to_string(Factory::get_texture_factory()->get_size()) + ")");
#endif	
}
/////////////
bool Texture::load(const std::string& file_name)
{
	std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
	if(extension == "png")
	{
		return load_png(file_name);
	}
	if(extension == "jpeg" || extension == "jpg"  || extension == "jpe" || extension == "jif" || extension == "jfif" || extension == "jfi")
	{
		return load_jpeg(file_name);
	}
	if(extension == "bmp" || extension == "dib")
	{
		return load_bmp(file_name);
	}
	if(extension == "gif")
	{
		return load_gif(file_name);
	}
	if(extension == "tif" || extension == "tiff")
	{
		return load_tiff(file_name);
	}
	if(extension == "svg" || extension == "svgz")
	{
		return load_svg(file_name);
	}
    return load_png(file_name); // default
}
/////////////
void Texture::load(const std::string& file_name, int * error)
{
    load_png(file_name, error);
}
/////////////
bool Texture::load(const void * data, int width, int height, int depth, int channel)
{
    this->data    = const_cast<void *>(data);
	this->width   = width;
	this->height  = height;
	this->depth   = depth;
	this->channel = channel;
	return true;
}
/////////////
int Texture::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TLIGHTUSERDATA) // works!
		{
			luaL_checktype(L, 3, LUA_TNUMBER); // get width		
			luaL_checktype(L, 4, LUA_TNUMBER); // get height
			void * data = static_cast<void *>(lua_touserdata(L, 2)); // get data
			lua_pushboolean(L, static_cast<int>(texture->load(data, static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)))));
			return 1;
		}
	    if(lua_type(L, 2) == LUA_TSTRING)
	    {
		    lua_pushboolean(L, texture->load(lua_tostring(L, 2)));
	 	    return 1;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void Texture::copy(const Texture& texture)
{
    data    = texture.get_data  ();
	width   = texture.get_width ();
	height  = texture.get_height();
	channel = texture.get_channel();
	file    = texture.get_file  ();
	set_param(texture.get_filter().x, texture.get_filter().y, texture.get_wrap().x, texture.get_wrap().y, 
	    texture.get_internal_format(), texture.get_format());
	set_type(texture.get_type());
	// check for opengl context
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif
#endif
    // generate texture buffer if it not a valid OpenGL texture and it did not previously exist
    if(buffer == 0) glGenTextures(1, &buffer);
	// update texture buffer without having to generate a new one
    glBindTexture(GL_TEXTURE_2D, buffer); // bind buffer
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(get_width()), static_cast<GLsizei>(get_height()), 0, static_cast<GLenum>(get_format()), GL_UNSIGNED_BYTE, static_cast<GLvoid *>(get_data())); // pass texture width, height, and data to OpenGL
        glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps
	glBindTexture(GL_TEXTURE_2D, 0);
#ifdef DOKUN_DEBUG	
	std::cout << "Texture::copy(const Texture&): buffer name: " << buffer << " (updated)" << std::endl;
#endif	
#endif
}
/////////////
int Texture::copy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // texture0
	luaL_checktype(L, 2, LUA_TTABLE); // texture1
    lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture0 = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Texture * texture1 = *static_cast<Texture **>(lua_touserdata(L, -1));
			texture1->copy(*texture0);
		}
	}
	return 0;
}
/////////////
/////////////
void Texture::generate()
{
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif
#endif
	if(!glIsTexture(static_cast<GLuint>(buffer))) // no buffer yet (generate a single buffer and no more than 1)
	{
		glGenTextures(1, static_cast<GLuint *>(&buffer)); // generate a buffer
		glBindTexture(target, static_cast<GLuint>(buffer)); // bind buffer
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
        glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
        glTexImage2D(target, level, static_cast<GLint>(internal_format), static_cast<GLsizei>(get_width()), static_cast<GLsizei>(get_height()), 0, static_cast<GLenum>(get_format()), GL_UNSIGNED_BYTE, static_cast<GLvoid *>(get_data())); // pass texture width, height, and data to OpenGL
        glGenerateMipmap(target); // generate mipmaps              glGenerateTextureMipmap(static_cast<GLuint>(buffer)); // <- this is only in OpenGL 4.5
		glBindTexture(target, 0); // unbind buffer
	#ifdef DOKUN_DEBUG
	    dokun::Logger::push("dokun: Texture_" + std::to_string(Factory::get_texture_factory()->get_location(this)) + " generated (value=" + String((int)buffer).str() + ")");
    #endif	
	}
#endif	
}
/////////////
int Texture::generate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->generate();
	}
    return 0;	
}
/////////////
void Texture::generate_array(int layer_count)
{
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif		
#endif
	if(!glIsTexture(static_cast<GLuint>(buffer))) // no buffer yet (generate a single buffer and no more than 1)
	{	
        glGenTextures(1, &buffer);
        glBindTexture(GL_TEXTURE_2D_ARRAY, buffer);
        //Allocate the storage.
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1/*level_count*/, GL_RGBA8, width, height, layer_count);      
		//Upload pixel data.      
		//The first 0 refers to the mipmap level (level 0, since there's only 1)
        //The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
        //The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
        //Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, 0/*index*/, width, height, layer_count, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D_ARRAY); // generate mipmaps              glGenerateTextureMipmap(static_cast<GLuint>(buffer)); // <- this is only in OpenGL 4.5
        //Always set reasonable texture parameters
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap_t);  		
		//glBindTexture(GL_TEXTURE_2D_ARRAY, 0); // unbind buffer    
	#ifdef DOKUN_DEBUG
	    dokun::Logger::push(DOKUN_LOGTAG + "Texture array generated");
    #endif		
	}
#endif	
}
/////////////
void Texture::bind()
{
#ifdef DOKUN_OPENGL	
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif
#endif
    if(glIsTexture(buffer))
	{
        glBindTexture(GL_TEXTURE_2D, buffer);
	}
#endif
}
int Texture::bind(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // texture
    lua_getfield(L, -1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->bind();
	}
	return 0;	
}
/////////////
void Texture::unbind()
{
#ifdef DOKUN_OPENGL	
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
#endif	
}
int Texture::unbind(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // texture
    lua_getfield(L, -1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->unbind();
	}
	return 0;	
}
/////////////
void Texture::destroy()
{
#ifdef DOKUN_OPENGL	
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	    return;
#endif	    
#endif
    if(glIsTexture(static_cast<GLuint>(buffer)) && buffer != 0) // if texture buffer obj is valid, then delete it (zero means it's not valid)
    {
        GLuint buffer_temp = buffer; // copy texture buffer obj name just for debug
        glDeleteTextures(1, static_cast<GLuint *>(&buffer)); // delete texture buffer obj
        buffer = 0;     // set texture buffer obj to nullptr
	#ifdef DOKUN_DEBUG
        if(!glIsTexture(static_cast<GLuint>(buffer))) {std::cout << "Texture::destroy(): buffer " << buffer_temp << " deleted" << std::endl;} // check if GL_texture is actually deleted
    #endif
    }
    // delete texture pixel data as well ..
	if(data != nullptr)
	{
        if(is_png()) delete [] static_cast<png_byte *>(data); // array allocated with "new", so I guess I have to delete it this way???
        data = nullptr; // set texture pixel data to nullptr
#ifdef DOKUN_DEBUG
        if(!data) std::cout << "Texture::destroy(): Texture_" << String(Factory::get_texture_factory()->get_location(this)) << ": data deleted." << std::endl;
#endif
	}
#endif
}
int Texture::destroy(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // texture
    lua_getfield(L, -1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->destroy();
	}
	return 0;
}
/////////////
void Texture::clear()
{
#ifdef DOKUN_OPENGL	
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
        return;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif
#endif
    //glClearTexImage(buffer, 0, format, GL_UNSIGNED_BYTE, 0/*empty_data*/); // glClearTexImage is available only if the GL version is 4.4 or greater.
    destroy();
#endif
}
int Texture::clear(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // texture
    lua_getfield(L, -1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->clear();
	}
    return 0;
}
///////////// to_do: create a Texture::update function so pixels can be updated from an array of pixels
/////////////
void Texture::set_rect(int x, int y, int width, int height) // cuts a portion of a sprite sheet
{	
	rect = Vector4(x, y, width, height);
}
/////////////
int Texture::set_rect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_rect(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)), static_cast<int>(lua_tonumber(L, 5)));
	}
	return 0;
}
/////////////
void Texture::set_name(const std::string& name)
{
    this->name = name;
}
/////////////
int Texture::set_name(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);    
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_name(lua_tostring(L, 2));
    }
    return 0;
}
/////////////
void Texture::set_buffer(unsigned int buffer)
{
#ifdef DOKUN_OPENGL
    this->buffer = buffer;
#ifdef DOKUN_DEBUG // if buffer (GL_texture) does not exist then send warning
#ifdef DOKUN_WIN32
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return;
#endif
#endif
    if(!glIsTexture(buffer)) dokun::Logger(String("warning! Buffer \"" + std::to_string(buffer) + "\" has not been generated."));// if the buffer is not a valid GL_texture
#endif    
#endif
}
/////////////
int Texture::set_buffer(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_buffer(lua_tointeger(L, 2));	
	}
	return 0;
}
/////////////
void Texture::set_filter(int min_filter, int mag_filter)
{
#ifdef DOKUN_OPENGL	
	this->min_filter = min_filter;
	this->mag_filter = mag_filter;
#endif	
}
/////////////
int Texture::set_filter(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_filter(lua_tonumber(L, 2), lua_tonumber(L, 3));	
	}
	return 0;
}
/////////////
void Texture::set_wrap(int wrap_s, int wrap_t)
{
#ifdef DOKUN_OPENGL	
    this->wrap_s = wrap_s;
	this->wrap_t = wrap_t;
#endif	
}
/////////////
int Texture::set_wrap(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_wrap(lua_tonumber(L, 2), lua_tonumber(L, 3));	
	}
	return 0;	
}
/////////////
void Texture::set_internal_format(int internal_format)
{
#ifdef DOKUN_OPENGL	
    this->internal_format = internal_format;
#endif		
}
/////////////
int Texture::set_internal_format(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_internal_format(lua_tonumber(L, 2));	
	}
	return 0;	
}
/////////////
void Texture::set_format(unsigned int format)
{
#ifdef DOKUN_OPENGL	
    this->format = format;
#endif		
}
/////////////
int Texture::set_format(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_format(lua_tonumber(L, 2));	
	}
	return 0;	
}
/////////////
void Texture::set_param(int min_filter, int mag_filter,
    int wrap_s, int wrap_t,
	int internal_format, unsigned int format)
{
#ifdef DOKUN_OPENGL	
	this->min_filter      = min_filter;
	this->mag_filter      = mag_filter;
	this->wrap_s          = wrap_s;
	this->wrap_t          = wrap_t;
	this->internal_format = internal_format;
	this->format          = format;
#endif	
}
/////////////
int Texture::set_param(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);	
	luaL_checktype(L, 6, LUA_TNUMBER);	
	luaL_checktype(L, 7, LUA_TNUMBER);	
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_param(lua_tonumber(L, 2),lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6), lua_tonumber(L, 7)); 
	}		
	return 0;
}
/////////////
void Texture::set_target(unsigned int target)
{
	this->target = target;
}
int Texture::set_target(lua_State * L)
{
    return 0;
}
/////////////
void Texture::set_level (int level)
{
	this->level = level;
}
int Texture::set_level(lua_State * L)
{
    return 0;
}
/////////////
void Texture::set_invert(bool invert)
{
	this->invert = invert;
}
/////////////
int Texture::set_invert(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_invert(lua_toboolean(L, 2));
	}
	return 0;		
}
/////////////
void Texture::set_type(const std::string& type)
{
	this->type = type;
}
/////////////
int Texture::set_type(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		texture->set_type(lua_tostring(L, 2));
	}
	return 0;	
}
/////////////
/////////////
int Texture::get_width() const
{
	return width;
}
/////////////
int Texture::get_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_width());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
int Texture::get_height() const
{
	return height;
}
/////////////
int Texture::get_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_height());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
int Texture::get_depth() const
{
	return depth;
}
int Texture::get_depth(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_depth());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
int Texture::get_channel() const
{
	return channel;
}
int Texture::get_channel(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_channel());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector2 Texture::get_size() const
{
	return Vector2(width, height);
}
int Texture::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_width());
		lua_pushnumber(L, texture->get_height());
		return 2;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
void * Texture::get_data() const
{
	return data;
}
/////////////
int Texture::get_data(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, texture->get_data());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string Texture::get_file() const
{
	return file;
}
/////////////
int Texture::get_file(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushstring(L, texture->get_file().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Texture::get_buffer()const // returns 0 if generation failed
{
#ifdef DOKUN_OPENGL	
	return buffer;
#endif	
    return 0;
}
/////////////
int Texture::get_buffer(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushinteger(L, texture->get_buffer());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
Vector2i Texture::get_filter()const
{
#ifdef DOKUN_OPENGL
    return Vector2i(min_filter, mag_filter);
#endif
    return Vector2i(0, 0);
}
/////////////
int Texture::get_filter(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_filter().x);
		lua_pushnumber(L, texture->get_filter().y);
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
Vector2i Texture::get_wrap()const
{
#ifdef DOKUN_OPENGL
    return Vector2i(wrap_s, wrap_t);
#endif	
    return Vector2i(0, 0);
}
/////////////
int Texture::get_wrap(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_wrap().x);
		lua_pushnumber(L, texture->get_wrap().y);
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
int Texture::get_internal_format() const
{
#ifdef DOKUN_OPENGL
    return internal_format;
#endif	
    return 0;
}
/////////////
int Texture::get_internal_format(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_internal_format());
		return 1;
	}
	lua_pushnil(L);
	return 1;	
}
/////////////
unsigned int Texture::get_format() const// RGB or RGBA
{
#ifdef DOKUN_OPENGL		
	return format;
#endif	
    return 0;
}
/////////////
int Texture::get_format(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushnumber(L, texture->get_format());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Texture::get_target() const
{
	return target;
}
int Texture::get_target(lua_State * L)
{
    return 1;
}
/////////////
int Texture::get_level() const
{
	return level;
}
int Texture::get_level(lua_State * L)
{
    return 1;
}
/////////////
void Texture::get_info(std::string name, int * data) //const// USAGE: int width; tex_ptr->get_info("width", &width); std::cout << width << std::endl;
{
#ifdef DOKUN_OPENGL
	if(name == "width")
	{
		GLuint buffer = static_cast<GLuint>(get_buffer()); // get buffer
		glBindTexture(GL_TEXTURE_2D, buffer); // bind buffer (to use texture)
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  data); // store data
		glBindTexture(GL_TEXTURE_2D, 0); // unbind buffer (to remove texture)
	}
	if(name == "height")
	{
		GLuint buffer = static_cast<GLuint>(get_buffer());
		glBindTexture(GL_TEXTURE_2D, buffer); // bind buffer
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  data); // store data
		glBindTexture(GL_TEXTURE_2D, 0); // unbind buffer
	}
	if(name == "depth")
	{
		GLuint buffer = static_cast<GLuint>(get_buffer());
		glBindTexture(GL_TEXTURE_2D, buffer); // bind buffer
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH,  data); // store data
		glBindTexture(GL_TEXTURE_2D, 0); // unbind buffer
	}
#endif
}
/////////////
int Texture::get_info(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
        int param; // for storing texture information
		texture->get_info(lua_tostring(L, 2), &param); // store data in 'param'
		lua_pushnumber(L, param);
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
std::string Texture::get_type()const
{
	return type;
}
/////////////
int Texture::get_type(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushstring(L, texture->get_type().c_str());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
Vector4 Texture::get_rect() const
{
	return rect;
}
int Texture::get_rect(lua_State*L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, texture->get_rect().x);
	    lua_pushnumber(L, texture->get_rect().y);
	    lua_pushnumber(L, texture->get_rect().z);
	    lua_pushnumber(L, texture->get_rect().w);
		return 4;
	}		
    lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
    return 4;	
}
/////////////
std::string Texture::get_name()const
{
    return name;
}
int Texture::get_name(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
        lua_pushstring(L, texture->get_name().c_str());
        return 1;
    }
    lua_pushstring(L, "");
    return 1;
}
/////////////
/////////////
// BOOLEAN
/////////////
bool Texture::is_texture()const // returns true if texture is loaded
{
	return (data != nullptr);	
}
/////////////
int Texture::is_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushboolean(L, texture->is_texture());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool Texture::is_inverted()const
{
	return (invert != 0);
}
/////////////
int Texture::is_inverted(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushboolean(L, texture->is_inverted());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool Texture::is_generated()const // returns true if texture buffer is generated
{
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return false;
#endif		
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
		return false;
#endif
#endif
    return (glIsTexture(static_cast<GLuint>(buffer)) == true);
#endif	
    return false;
}
int Texture::is_generated(lua_State*L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
		lua_pushboolean(L, texture->is_generated());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;		
}
/////////////
/////////////
bool Texture::is_png()
{
	if(get_file().empty()) {
		dokun::Logger("Unable to get image format unless it has been loaded from a file");
		return false;
	}
    png_byte header[8];
    std::FILE * file = fopen(get_file().c_str(), "rb");
    if (!file)  {
		return false;
    }
    fread(header, 1, 8, file);
    bool png = !png_sig_cmp(header, 0, 8);
    if (!png) {
	    fclose  (file);
		return false;
    }
	fclose (file);
    return true;
}
int Texture::is_png(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, texture->is_png());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

bool Texture::is_jpeg()
{
    return false;
}
int Texture::is_jpeg(lua_State *L)
{
	return 1;
}

bool Texture::is_gif()
{
    return false;
}
int Texture::is_gif(lua_State *L)
{
	return 1;
}

bool Texture::is_bmp()
{
    return false;
}
int Texture::is_bmp(lua_State *L)
{
	return 1;
}

bool Texture::is_tiff()
{
    return false;
}
int Texture::is_tiff(lua_State *L)
{
	return 1;
}
/////////////
/////////////
// LOADERS
/////////////
bool Texture::load_png(const std::string& file_name)
{
    png_byte header[8];
    std::FILE *   file;
	file = fopen(file_name.c_str(), "rb");
	if (!file)
        { return false;}
	else
	{
		(this)->file =(file_name);
	}
    fread(header, 1, 8, file); // read header
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(file);
        return false;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr); // create png struct
    if (!png_ptr) {
        fclose(file);
        return false;
    }
   png_infop info_ptr = png_create_info_struct(png_ptr); //create png info struct
   if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
        fclose(file);
        return false;
   }
   png_infop end_info = png_create_info_struct(png_ptr);  //create png end info struct
   if (!end_info) {
       png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
       fclose(file);
       return false;
   }
   if (setjmp(png_jmpbuf(png_ptr))) { //png error stuff
       png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
       fclose(file);
       return false;
   }
   //init png reading
   png_init_io(png_ptr, file);
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);
   // change zlib compression buffer size when reading compressed data
   int buffer_size = 8192;
   png_set_compression_buffer_size(png_ptr, buffer_size);
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
   //variables to pass to get info
   //update width and height based on png info
    width =  png_get_image_width(png_ptr,  info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
	channel = png_get_channels(png_ptr, info_ptr);
   	if(channel == 3) {format = DOKUN_RGB; }
	if(channel == 4) {format = DOKUN_RGBA;}
   // get bit_depth and color_type based on png info
    int bit_depth      = png_get_bit_depth(png_ptr,  info_ptr);
    int color_type     = png_get_color_type(png_ptr, info_ptr);
    int interlace_type = png_get_interlace_type(png_ptr,     info_ptr);
    int compression_type = png_get_compression_type(png_ptr, info_ptr);
    int filter_method    = png_get_filter_type(png_ptr, info_ptr);
// .. must be called before using png_read_update_info, as well as other setters
   int number_of_passes = png_set_interlace_handling(png_ptr);
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
   // Allocate the image_data as a big block, to be given to opengl
   data = new png_byte[rowbytes * height];
   if (!data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(file);
     return false;
   }

   //row_pointers is for pointing to data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers)
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     data = nullptr;
     fclose(file);
     return false;
   }
   // set the individual row_pointers to point at the correct offsets of data
   for (unsigned int i = 0; i < height; ++i)
   {
     row_pointers[height - 1 - i] = (png_byte*)data + i * rowbytes;
   }

   // the actual reading of the png file
   //read the png into data through row_pointers
   png_read_image(png_ptr, row_pointers);

   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(file);
   return true;
}
/////////////
void Texture::load_png(const std::string& file_name, int * error)
{
  // header for testing if it is a png
	unsigned int number = 8;
    png_byte header[8];
    // open the file
    std::FILE *file ;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&file, file_name.c_str(), "rb");
	#else
	    file = fopen(file_name.c_str(), "rb");
	#endif
	if (!file)
    {
		*error = DOKUN_ERROR_FILE_NOT_FOUND;
		return; // exit after retrieving error code
    }
	else
	{
		this->file =(file_name);
	}
    // read the header
    fread(header, 1, number, file);
    int is_png = !png_sig_cmp(header, 0, number);
    if (!is_png)
	{
		*error = DOKUN_ERROR_FILE_INVALID;
        fclose(file);
		return;
    }
   // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
        nullptr, nullptr);
    if (!png_ptr)
	{
		*error = DOKUN_ERROR_FILE_BROKEN;
        fclose(file);
		return;
    }
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
     fclose(file);
	 return;
   }
   //create png end info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
     fclose(file);
	 return;
   }
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(file);
	 return;
   }
   //init png reading
   png_init_io(png_ptr, file);
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, number);
   // change zlib compression buffer size when reading compressed data
   int buffer_size = 8192;
   png_set_compression_buffer_size(png_ptr, buffer_size);
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
   //variables to pass to get info
   int bit_depth, color_type, interlace_type, compression_type, filter_method;

   //update width and height based on png info
    width =  png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    channel = png_get_channels(png_ptr, info_ptr);
	if(channel == 2) {format = DOKUN_RG;  }
	if(channel == 3) {format = DOKUN_RGB; }
	if(channel == 4) {format = DOKUN_RGBA;}
   // get bit_depth and color_type based on png info
   bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
   color_type = png_get_color_type(png_ptr, info_ptr);
   interlace_type=png_get_interlace_type(png_ptr,
                         info_ptr);
   compression_type=png_get_compression_type(png_ptr,
                         info_ptr);
   filter_method    = png_get_filter_type(png_ptr,
                         info_ptr);
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
	 *error = DOKUN_ERROR_FILE_BROKEN;
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(file);
	 return;
   }

   //row_pointers is for pointing to data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers)
   {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     data = nullptr;
     fclose(file);
	 return;
   }
   // set the individual row_pointers to point at the correct offsets of data
   for (unsigned int i = 0; i < height; ++i)
   {
     row_pointers[height - 1 - i] = (png_byte*)data + i * rowbytes;
   }

   // the actual reading of the png file
   //read the png into data through row_pointers
   png_read_image(png_ptr, row_pointers);

   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(file);
   (*error) = DOKUN_NO;
}
/////////////
bool Texture::load_bmp(const std::string& file_name) // supports 24-bit bmp                            void Texture::load_bmp(const std::string& file_name, int * error){}
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
bool Texture::load_jpeg(const std::string& file_name)
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
	if(channel == 2) { format = DOKUN_RG;  }
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	int size = width * height * channel;
	data = new unsigned char [size];

	while (info.output_scanline < info.output_height)
	{
		rowptr = static_cast<unsigned char *>(data) + (info.output_height-1-info.output_scanline) * info.output_width * channel; // //rowptr = (JSAMPROW) &pdata[(cinfo.image_height-1-cinfo.next_scanline)*row_stride];
		jpeg_read_scanlines(&info, &rowptr, 1);
	}

	jpeg_finish_decompress (&info);   //finish decompressing
	jpeg_destroy_decompress(&info);
    fclose(file);                    //close the file
	return true;
}
//void Texture::load_jpeg(const std::string& file_name, int * error){}
/////////////
bool Texture::load_gif(const std::string& file_name) // void Texture::load_gif(const std::string& file_name, int * error){}
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
	if(channel == 2) { format = DOKUN_RG;  }
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
bool Texture::load_tiff(const std::string& file_name) // void Texture::load_tifff(const std::string& file_name, int * error){}
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
	if(channel == 2) { format = DOKUN_RG;  }
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
	data = image;
	delete image;
	_TIFFfree(raster);
    TIFFClose(tiff);
	return true;
}
/////////////
bool Texture::load_svg(const std::string& file_name) //char * chr = strdup(file_name.c_str());
{
	return false;
}
/////////////
/////////////
void Texture::export_png() // hmm.. gif
{
	// READ JPG

	// WRITE PNG
	// generate filename
	std::string file_name = "out.png";//"texture" + std::to_string(Factory::get_texture_factory()->get_location(this)) + ".png";

	FILE *file = fopen(file_name.c_str(), "wb"); // create file in write mode
    if (!file) {
		std::cout << "Could not create " << file_name << std::endl;
	}
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
	    nullptr, nullptr); // write_struct
    if (!png_ptr)
	{
		dokun::Logger("Out of memory");
		//return false; // out of memory
	}
    png_infop info_ptr = png_create_info_struct(png_ptr); // info_struct
    if (!info_ptr)
	{
        png_destroy_write_struct(&png_ptr, nullptr);
        //return false;
	}
    png_infop end_ptr = png_create_info_struct(png_ptr);
	// end_struct
    if (!end_ptr)
	{
        png_destroy_write_struct(&png_ptr, nullptr);
		//return false;
	}
	// png error stuff
    if (setjmp(png_jmpbuf(png_ptr)))
	{
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }


	// init png writing
	png_init_io(png_ptr, file);
    // write header - store color, depth, etc.
	int bit_depth  = 8;
	int color_type = 6;
    png_set_IHDR(png_ptr, info_ptr, get_width(), get_height(),
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	// write all image info
	png_write_info(png_ptr, info_ptr);
	//--------------------------
	// rowbytes
	// Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	// row pointer - create output data
	png_bytep * data_p = new png_bytep[get_height()];
	if (!data_p)
	{
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(file);
	    return;
   }
	for(int i=0; i < get_height(); ++i)
	{
		data_p[height - 1 - i] = (png_byte*)data + i * rowbytes;
	}
	//---------------------------
    // write byte data
	png_write_image(png_ptr, (png_bytepp)data_p);


	png_write_end(png_ptr, nullptr); // stop writing
    if (png_ptr && info_ptr)
	{
        png_destroy_write_struct(&png_ptr, &info_ptr);	// destroy write struct
	}
	fclose(file); // close file
	//return true;
}
/////////////
void Texture::export_jpeg() // works with gif and 24-bit bmp
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  std::string filename = "out.jpg";
   if ((outfile = fopen(filename.c_str(), "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename.c_str());
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width =  get_width (); 	/* image width and height, in pixels */
  cinfo.image_height = get_height();
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  int quality = 95;
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);


  jpeg_start_compress(&cinfo, TRUE);


  row_stride = get_width () * 3;	/* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = & static_cast</*JSAMPLE **/unsigned char *>(data)[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
}
////////////////
void Texture::export_gif()
{
	// OPEN GIF FOR READING
	int error         = E_GIF_SUCCEEDED;
	GifFileType * gifout = EGifOpenFileName("out.gif", true, &error);
	if(!gifout){
		std::cerr << GifErrorString(E_GIF_ERR_OPEN_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
	}

	GifFileType * gifin = DGifOpenFileName(get_file().c_str(), 0);
	// -------------
	gifout->SWidth  = get_width ();
    gifout->SHeight = get_height();
    gifout->SColorResolution = gifin->SColorResolution;
    gifout->SBackGroundColor = gifin->SBackGroundColor;
    gifout->SColorMap = GifMakeMapObject(
				 gifin->SColorMap->ColorCount,
				 gifin->SColorMap->Colors);

    for (int i = 0; i < gifin->ImageCount; i++)
	    (void) GifMakeSavedImage(gifout, &gifin->SavedImages[i]);
	//---------------
	// WRITE GIF
	if(EGifSpew(gifout) != GIF_OK) {
		std::cerr << GifErrorString(E_GIF_ERR_WRITE_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
	}
	// -------------
	if(EGifCloseFile (gifout, &error) != GIF_OK) {
		std::cerr << GifErrorString(E_GIF_ERR_CLOSE_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
	}
}
////////////////
void Texture::export_tiff()
{
	TIFF * out = TIFFOpen("out.tiff", "w");
	unsigned char *data = new unsigned char[width * height * channel];
	TIFFSetField (out, TIFFTAG_IMAGEWIDTH, width);  // set the width of the image
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);    // set the height of the image
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, channel);   // set number of channels per pixel
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);    // set the size of the channels
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
    //   Some other essential fields to set that you do not have to understand for now.
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	tsize_t linebytes = channel * width;     // length in memory of one row of pixel in the image.
    unsigned char * buf = nullptr;        // buffer used to store the row of pixel information for writing to file
    //    Allocating memory to store the pixels of current row
    if (TIFFScanlineSize(out))//linebytes)
        buf =(unsigned char *)_TIFFmalloc(linebytes);
    else
        buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));
    // We set the strip size of the file to be size of one row of pixels
    TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, width * channel));
    //Now writing image to the file one strip at a time
	for (uint32 row = 0; row < height; row++)
    {
        memcpy(buf, &static_cast<unsigned char *>(data)[(height-row-1)*linebytes], linebytes);    // check the index here, and figure out why not using h*linebytes
        if (TIFFWriteScanline(out, buf, row, 0) < 0)
            break;
    }
    // Finally we close the output file, and destroy the buffer
    delete data;
    TIFFClose(out);
    if (buf)
    _TIFFfree(buf);
}
/////////////
int Texture::texture_new(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
        file_name = (const char*)lua_tostring(L, -1);
    }
    // clear stack
	lua_settop(L, 0);
	// create a new table
	lua_createtable(L, 0, 0);
	// set the metatable
	lua_getglobal(L, "Texture");
	lua_setmetatable(L, 1);
	// create a new userdata
	Texture ** texture = static_cast<Texture **>(lua_newuserdata(L, sizeof(Texture*)));
	// file_name is pushed in parameter
	if(!file_name.empty())
	{
	    * texture = new Texture(file_name);
	}
	else
	{
	    * texture = new Texture();
	}
	// set userdata
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1))
	{
	    return 1;
	}
	// push nil if no table
	lua_pushnil(L);
	return 1;
}
/////////////
