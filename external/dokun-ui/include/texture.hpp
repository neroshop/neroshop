#ifndef TEXTURE_HPP_DOKUN
#define TEXTURE_HPP_DOKUN

#include "platform.hpp"
#include "utility.hpp"
#include "vector.hpp"
#include "logger.hpp"
#ifdef __cplusplus       // if c++ // if c++
extern "C" {             // run as c code
#endif
    #include <png.h>     // libpng (png)
    #include <jpeglib.h> // libjpeg (jpeg)
    #include <gif_lib.h> // giflib (gif)
    #include <tiffio.h>  // libtiff (tiff) // libxml (svg) (will be supported in future releases)
#ifdef __cplusplus       // if c++
}
#endif

#ifdef __cplusplus // if c++
#include <iostream>
#include <vector>
#include <lua.hpp>

class Texture { // Texture cannot be drawn but applied to Sprites and Models
public: // C++                                                                                                  // Lua
	Texture();                                      static int texture_new(lua_State *L);       /* texture = Texture:new() */
	Texture(const Texture &texture);
	Texture(const std::string& file_name); 
	Texture(const std::string& file_name, const std::string& type);
    Texture(const std::string& file_name, int x, int y, int width, int height); 		
	Texture(const void * data, int width, int height, int depth = 1, int channel = 4);        
	~Texture();                
	// normal functions
    bool load(const std::string& file_name);        static int load(lua_State *L);
	void load(const std::string& file_name, int * error);
	bool load(const void * data, int width, int height, int depth = 1, int channel = 4);
	void copy(const Texture& texture);                 static int copy(lua_State *L);
	void generate();                                   static int generate(lua_State *L);
	void generate_array(int layer_count);              static int generate_array(lua_State *L);
	void bind();                                       static int bind(lua_State * L);
	void unbind();                                     static int unbind(lua_State * L);
	void destroy();                                    static int destroy(lua_State *L);
	void clear();                                      static int clear(lua_State * L);
	// setters -  new! 
	void set_rect(int x, int y, int width, int height);   static int set_rect(lua_State *L);// crop a part of the texture 11/7/2015 
	// opengl-related
	void set_buffer(unsigned int buffer);                 static int set_buffer(lua_State * L);
	void set_filter(int min, int mag);                 static int set_filter(lua_State * L);
	void set_wrap(int wrap_s, int wrap_t);                                    static int set_wrap(lua_State * L);
	void set_internal_format(int internal_format);                 static int set_internal_format(lua_State * L);
	void set_format(unsigned int format);                 static int set_format(lua_State * L);
	void set_param(int min_filter, int mag_filter, 
		    int wrap_s, int wrap_t, 
			int internal_format, unsigned int format);        static int set_param(lua_State *L);
	void set_target(unsigned int target);           static int set_target(lua_State * L); // static int set_(lua_State * L);
	void set_level (int level);                     static int set_level(lua_State * L); // static int set_(lua_State * L);
	// ....
	void set_invert(bool invert);                   static int set_invert(lua_State *L);
    // types: diffuse (base_color), normal, specular
    void set_type(const std::string& type);		    static int set_type(lua_State *L);
    void set_name(const std::string& name);               static int set_name(lua_State * L); // bonus function (gives texture a custom name)
	// getters
	int get_width() const;                          static int get_width(lua_State *L);  /* texture:get_width()  */
	int get_height() const;                         static int get_height(lua_State *L); /* texture:get_height() */
	int get_depth() const;                          static int get_depth(lua_State * L);
	int get_channel() const;                        static int get_channel(lua_State *L);
	Vector2 get_size() const;                       static int get_size(lua_State *L);   /* texture:get_size()   */
	void * get_data() const;                        static int get_data(lua_State *L);   /* texture:get_data()   */
	std::string get_file() const;                   static int get_file(lua_State *L);   /* texture:get_file()   */
	Vector4 get_rect() const;                       static int get_rect(lua_State*L);
	std::string get_type()const;                    static int get_type(lua_State *L);
    std::string get_name()const;                    static int get_name(lua_State * L);
	// OPENGL
	unsigned int get_buffer()const;                      static int get_buffer(lua_State *L); // returns a texture buffer object
	Vector2i get_filter()const;                     static int get_filter(lua_State *L);
	Vector2i get_wrap()const;                       static int get_wrap(lua_State * L);
	unsigned int get_format() const;                         static int get_format(lua_State *L);  // pixel format
	int get_internal_format() const;                static int get_internal_format(lua_State *L);  // pixel format
	//....
	unsigned int get_target() const;        static int get_target(lua_State * L);
	int get_level() const;                  static int get_level(lua_State * L); // static int get_(lua_State * L);
	//static int get_(lua_State * L);
	//static int get_(lua_State * L);
	// ...
	void get_info(std::string name, int * data);    static int get_info(lua_State *L); // returns additional texture information via glGetTexLevelParameteriv
	// boolean
	bool is_texture()const;                         static int is_texture(lua_State *L); // returns true if texture is not empty (has data)
	bool is_inverted()const;                        static int is_inverted(lua_State *L);
	bool is_generated()const;                       static int is_generated(lua_State*L);
	// boolean : format check
	bool is_png();                                  static int is_png(lua_State *L);
	bool is_jpeg();                                 static int is_jpeg(lua_State *L);
	bool is_gif();                                  static int is_gif(lua_State *L);
	bool is_bmp();                                  static int is_bmp(lua_State *L);
	bool is_tiff();                                 static int is_tiff(lua_State *L);
	// friend
	friend class Sprite;
	friend class Model;
	friend class Renderer;
	friend class Image;
    friend class FONT;
    friend class Video;
private:	
	// loader (importer)
	bool load_png(const std::string& file_name);               // complete!
	void load_png(const std::string& file_name, int * error);
	bool load_jpeg(const std::string& file_name);              // complete!
	void load_jpeg(const std::string& file_name, int * error);		
	bool load_bmp(const std::string& file_name);
	void load_bmp(const std::string& file_name, int * error);
	bool load_gif(const std::string& file_name);
	void load_gif(const std::string& file_name, int * error);
	bool load_tiff(const std::string& file_name);
	void load_tiff(const std::string& file_name, int * error);		
	bool load_svg(const std::string& file_name); // xml format
	// exporter
public:	
    void export_png ();
    void export_jpeg();	
	void export_gif();
	void export_tiff();
private:		
    // properties
	unsigned int width;    
    unsigned int height;
	unsigned int depth;
	unsigned int channel; // AKA sample_per_pixel  3 => RGB, 4 =>RGBA		
    void * data; 
	std::string file;
	std::string type;
	bool invert;
	Vector4 rect;
    std::string name; // custom name for texture. Added 2019-07-29
	// OPENGL
	unsigned int buffer;	
    int mag_filter;
    int min_filter;
    int wrap_s;
 	int wrap_t;
    int internal_format;
    unsigned int format;	
    // ...
    unsigned int target;
    int level;	
};
#endif
#endif
