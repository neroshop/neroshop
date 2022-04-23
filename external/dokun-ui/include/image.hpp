#ifndef IMAGE_HPP_DOKUN
#define IMAGE_HPP_DOKUN

#include "platform.hpp"
#include "vector.hpp"
#include "factory.hpp" 
#include "types.hpp"
#include "logger.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#ifdef __cplusplus // if c++ 
extern "C" {      // run as c code
#endif
    #include <png.h>         // libpng (png)
    #include <jpeglib.h> // libjpeg (jpeg)
    #include <gif_lib.h> // giflib (gif)
    #include <tiffio.h>  // libtiff (tiff) // libxml (svg)
#ifdef __cplusplus // if c++ 
}
#endif

#ifdef __cplusplus // if c++
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <lua.hpp>

class Image //  loading, manipulating and saving images; image, unlike texture is created to work with some GUI elements
{
public:
    Image();   static int new_(lua_State *L);
	Image(const Image& image);
	Image(const Texture& texture);
	Image(const std::string& file_name);
	Image(void * data, int width, int height, int depth = 1, int channel = 4);
    ~Image();
    bool load(const std::string& file_name);                static int load(lua_State *L);
    bool load(void * data , int width, int height, int depth = 1, int channel = 4);  
	void draw();                                     static int draw(lua_State *L);	// draw function might be temporry as class Sprite acts as the main 2D renderer	
	void draw(double x, double y);
	void hide();static int hide(lua_State *L);
	void show();static int show(lua_State *L);
	// normal
	void copy(const Image& image);                   static int copy(lua_State *L);
	void copy(const Texture& texture);               static int copy_texture(lua_State *L);
	bool save(std::string file_name);                static int save(lua_State *L); // saves as png (default)
	void flip(int x, int y);                         static int flip(lua_State *L);
    void resize(int width, int height);              static int resize(lua_State *L); // similar to Sprite::set_size(int width, int height)
	void resize(const Vector2& size);
	void scale_to_fit(int width, int height);
	void scale_to_fit(const Vector2& size);
	void generate();                                 static int generate(lua_State *L);
	void destroy();                                  static int destroy(lua_State *L);
	void clear();                                    static int clear(lua_State * L);
	// setters
	void set_x(double x);                            static int set_x(lua_State *L);
	void set_y(double y);                            static int set_y(lua_State *L);
	void set_position(double x, double y);           static int set_position(lua_State *L);
	void set_position(const Vector2& position);
    void set_relative_position(double x, double y);  static int set_relative_position(lua_State *L);
	void set_relative_position(const Vector2& position);	
	void set_angle(double angle);                    static int set_angle(lua_State *L);
	void set_scale(double x, double y);             static int set_scale(lua_State *L);
	void set_scale(const Vector2& scale);
	void set_color(unsigned int red, unsigned int green, unsigned int blue);           static int set_color(lua_State *L);
	void set_color(unsigned int red, unsigned int green, unsigned int blue, float alpha);
	void set_color(const Vector3& color);
	void set_color(const Vector4& color);
	void set_alignment(const std::string& alignment);           static int set_alignment(lua_State *L);
	void set_visible(bool visible); static int set_visible(lua_State *L);
	void set_outline(bool outline);
	void set_outline_thickness(float outline_thickness);
	void set_outline_color(unsigned int red, unsigned int green, unsigned int blue);
	void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, float alpha);
	void set_outline_color(const Vector3& outline_color);
	void set_outline_color(const Vector4& outline_color);
	void set_outline_threshold(float outline_threshold);
	// opengl-related
	void set_filter(int min, int mag);                    static int set_filter(lua_State * L);
	void set_wrap(int wrap_s, int wrap_t);                static int set_wrap(lua_State * L);
	void set_internal_format(int internal_format);        static int set_internal_format(lua_State * L);
	void set_format(unsigned int format);                 static int set_format(lua_State * L);
	void set_param(int min_filter, int mag_filter, 
		    int wrap_s, int wrap_t, 
			int internal_format, unsigned int format);    static int set_param(lua_State *L);
	// getters
    int get_width()        const;                    static int get_width(lua_State *L);
    int get_height()       const;                    static int get_height(lua_State *L);
    int get_depth()        const;                    static int get_depth(lua_State *L);
	int get_channel()      const;                    static int get_channel(lua_State *L);
    Vector2 get_size()     const;                    static int get_size(lua_State *L);
    void * get_data()      const;                    static int get_data(lua_State *L);
    std::string get_file() const;                    static int get_file(lua_State *L);
	Vector4 get_color()    const;                    static int get_color(lua_State *L);
	double get_x()         const;                    static int get_x(lua_State *L);
	double get_y()         const;                    static int get_y(lua_State *L);
	Vector2 get_position() const;                    static int get_position(lua_State *L);
    double get_relative_x()         const;           static int get_relative_x(lua_State *L);
	double get_relative_y()         const;           static int get_relative_y(lua_State *L);
	Vector2 get_relative_position() const;           static int get_relative_position(lua_State *L);	
	double get_angle()     const;                    static int get_angle(lua_State *L);
	Vector2 get_scale()    const;                    static int get_scale(lua_State *L);
	double get_aspect_ratio_correction(int rect_width, int rect_height);     static int get_aspect_ratio_correction(lua_State * L);
	std::string get_alignment() const;               static int get_alignment(lua_State *L);
	Vector4 get_rect() const; static int get_rect(lua_State * L);
	// outline
	float get_outline_thickness() const;
	Vector4 get_outline_color() const;
	float get_outline_threshold() const;
	// OPENGL
	unsigned int get_buffer()const;                 static int get_buffer(lua_State *L); // returns a texture buffer object
	Vector2i get_filter()const;                     static int get_filter(lua_State *L);
	Vector2i get_wrap()const;                       static int get_wrap(lua_State * L);
	int get_internal_format() const;                static int get_internal_format(lua_State *L);  // pixel format	
	unsigned int get_format() const;                static int get_format(lua_State *L);  // pixel format	
	// boolean
	bool  is_png();                                  static int is_png(lua_State *L);
	bool is_jpeg();                                  static int is_jpeg(lua_State *L);
	bool  is_bmp();                                  static int is_bmp(lua_State *L);
	bool  is_tga();                                  static int is_tga(lua_State *L);
	bool  is_gif();                                  static int is_gif(lua_State *L);
	bool  is_tiff();                                 static int is_tiff(lua_State *L);
	bool  is_svg();                                  static int is_svg(lua_State *L);
	bool is_image();                                 static int is_image(lua_State *L);
	bool is_resized() const;                         static int is_resized(lua_State *L);
    bool is_generated()const;                        static int is_generated(lua_State *L);
	bool is_visible()const;                          static int is_visible(lua_State *L);
	bool has_outline() const;
	// operator overload
	Image operator = (const Texture& texture)
	{ 
		copy(texture);
		return (*this);
	}	
    // friends
	friend class Box;	
private:
    bool  load_png(std::string file_name);
    bool  load_bmp(std::string file_name);
    bool  load_jpeg(std::string file_name);
    bool  load_tga(std::string file_name);
    bool  load_gif(std::string file_name);
    bool  load_tiff(std::string file_name);
	bool  load_svg(const std::string& file_name);
	// conversion
	void bmp_to_png();
	// shader
	static void generate_shader(void);
	static Shader * image_shader;
private:
	unsigned int  width;
	unsigned int height;
	unsigned int depth;
	unsigned char * data;//std::vector<unsigned char> data; // the data to pass to opengl
	unsigned char** rdata; // the actual data
	unsigned int channel;
	std::string    file;
	Vector4 color;
	Texture * texture;
	bool resized;
	// outline
	bool outline;
	float outline_thickness;
	Vector4 outline_color;
	float outline_threshold;
	// geometric values
	double x, y;
	double angle;
	Vector2 scale;
	// for user interface
	double relative_x, relative_y;
	std::string alignment;
	bool visible;
	// OPENGL
	unsigned int buffer;	
    int mag_filter;
    int min_filter;
    int wrap_s;
 	int wrap_t;
    int internal_format;
    unsigned int format;	
};
#endif
#endif
