#ifndef FONT_HPP_DOKUN
#define FONT_HPP_DOKUN

#include "vector.hpp"
#include "logger.hpp"
#include "texture.hpp"
#ifdef __cplusplus // if c++ 
extern "C" {       // run as c code
#endif
    #include <ft2build.h>
    #include FT_FREETYPE_H
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus // if c++
#include <iostream>
#include <string>
#include <map>
#include <lua.hpp>
// To avoid naming conflictions with X11, dokun namespace will be used
#ifdef DOKUN_WINDOWS
#define DOKUN_DEFAULT_FONT_PATH "C:/Windows/Fonts/ariblk.ttf"
#endif
#ifdef __gnu_linux__
// Debian/Ubuntu
#define DOKUN_DEFAULT_FONT_PATH "/usr/share/fonts/X11/Type1/c0583bt_.pfb"//"/usr/share/fonts/X11/Type1/c0419bt_.pfb"//"/usr/share/fonts/X11/Type1/c0582bt_.pfb"
// Arch
////#define DOKUN_DEFAULT_FONT_PATH "/usr/share/fonts/liberation/LiberationMono-Regular.ttf"
#endif
#ifdef __macosx__
#define DOKUN_DEFAULT_FONT_PATH "System/Library/Fonts/HelveticaNeue.dfont"
#endif

namespace dokun {
class Font {
public:
	Font();                                                 static int font_new(lua_State *L);
	Font(const Font& font);
	Font(const std::string& file_name);
	~Font(); 
    // methods
	bool load(const std::string& file_name);                static int load(lua_State *L);
	bool load(const void * data, long size);
	void generate();                                        static int generate(lua_State *L); /* stores all characters and their sizes in array*/
	void destroy();                                         static int destroy(lua_State *L);
    void copy(const Font& font);                            static int copy(lua_State *L);
    // setters
    void set_width(unsigned int width);                     static int set_width(lua_State *L); 
    void set_height(unsigned int height);                   static int set_height(lua_State *L);
    void set_size(unsigned int width, unsigned int height); static int set_size(lua_State *L);
	void set_pixel_size(unsigned int width, unsigned int height);
	// getters
	int get_width()const;                                   static int get_width(lua_State *L);
	int get_width(char glyph)const;                         static int get_width_of_glyph(lua_State *L);
	int get_width(const std::string& text) const;		
	int get_height()const;                                  static int get_height(lua_State *L);
	int get_height(char glyph)const;                        static int get_height_of_glyph(lua_State *L);	
	int get_height(const std::string& text)const;
	Vector2 get_size()const;                                static int get_size(lua_State *L);
	Vector2 get_size(char glyph)const;                      //static int get_size(lua_State *L);
	Vector4 get_rect()const;                                static int get_rect(lua_State *L);
	FT_Byte * get_data()const;                              static int get_data(lua_State *L);
	FT_Face get_face()const;                                static int get_face(lua_State *L);
	std::string get_file()const;                            static int get_file(lua_State *L);
	FT_Library get_library()const;
	// font info  
	unsigned int get_buffer(char glyph) const;              static int get_buffer(lua_State *L);
	unsigned int get_buffer(const std::string& glyph) const;
    //////////////////////////////////////////////////////////////////////////////////////////////////
    Texture * get_bitmap(int index) const;                  static int get_bitmap(lua_State *L);
    Texture * get_bitmap(const std::string& name) const;
    unsigned int get_character_count() const;               static int get_character_count(lua_State *L);
    std::string get_family_name()const;                     static int get_family_name(lua_State *L);
    std::string get_style_name ()const;                     static int get_style_name (lua_State *L);
    //void get_()const; //static int get_(lua_State *L);
    Vector2 get_bearing(char glyph)const;                   static int get_bearing(lua_State *L);
    Vector2 get_advance(char glyph)const;                   static int get_advance(lua_State *L);
    //void get_()const; //static int get_(lua_State *L);
    //void get_()const; //static int get_(lua_State *L);
    //////////////////////////////////////////////////////////////////////////////////////////////////
	// boolean
	bool is_generated() const;                              static int is_generated(lua_State * L);
	// friends
	friend class Engine;
	friend class Logger;
	friend class Label;
	static FT_Library library;
private:
	static bool open ();
	static void close();
	////////////////////
	unsigned int  width;
	unsigned int height;
	FT_Face face;   // face = collection of glyphs (single characters)
	FT_Byte * data; // font data loaded from memory
	std::string file;
	////////////////////
    std::vector<Texture *> bitmaps; // to save textures
    FT_Glyph_Metrics metrics[256];
    //Get cell dimensions
    unsigned int cellW = 0;
    unsigned int cellH = 0;
    int max_bearing = 0;
    int min_hang = 0;
public:	
	typedef struct Character {
		unsigned int id;         // opengl texture / ID handle of the glyph texture
        unsigned int width;      // width  of glyph
		unsigned int height;     // height of glyph
        int bearing_x;           // offset from baseline to left of glyph
        int bearing_y;			 // offset from baseline to top of glyph
        long int advance_x;      // offset to advance to next glyph (to the right)
        long int advance_y;      // offset to advance to next glyph (to the bottom)
		unsigned char * buffer;  // bitmap data
    } Character;	
	std::map<char, Character> character_array; // map is like a std::vector that stores two types
};
}
#endif
#endif
