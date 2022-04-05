#include "../include/font.hpp"

dokun::Font::Font() : width(0), height(16), face(nullptr), data(nullptr)
{
	Factory::get_font_factory()->store(this);
}
/////////////
dokun::Font::Font(const dokun::Font& font) : dokun::Font() // copies data from another font
{
    copy(font);
}
/////////////
dokun::Font::Font(const std::string& file_name) : dokun::Font()
{
    if(!load(file_name)) {
	    std::cerr << dokun::Logger("Could not open font from " + file_name);
	    exit(0);
	}
}
/////////////
int dokun::Font::font_new(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING) file_name = lua_tostring(L, -1);
    lua_settop(L, 0); // clear any value pushed on top
	// create table (object)
	lua_createtable(L, 0, 0); // #1
    // set mt
	lua_getglobal(L, "Font");
	lua_setmetatable(L, 1);
	// set userdata
	dokun::Font **font = static_cast<dokun::Font **>(lua_newuserdata(L, sizeof(dokun::Font *)));
	if(!file_name.empty()) *font = new dokun::Font(file_name);
	else *font = new dokun::Font();
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1)) // return font, if table
	    return 1; 
	lua_pushnil(L); // if not table, return nil
	return 1;
}
/////////////
dokun::Font::~Font(void)
{
	destroy(); // destroy texture_buffer
	Factory::get_font_factory()->release(this);
    std::cout << "font deleted\n";
}
/////////////
FT_Library dokun::Font::library (nullptr);
/////////////
bool dokun::Font::open()
{
	if(library != nullptr) return true; // if freetype2 was previously initialized, return true

    FT_Error error = FT_Init_FreeType( &library );
    if ( error ) { dokun::Logger("FT_Init_FreeType failed."); return false; }
#ifdef DOKUN_DEBUG0
    dokun::Logger("freetype initialized.");
#endif
	return true;
}
/////////////
void dokun::Font::close()
{
    if(library == nullptr) return; // if freetype2 is already closed, then return
	FT_Error error = FT_Done_FreeType( library );
	if ( error ) { dokun::Logger("FT_Done_FreeType failed."); return; }
	library = nullptr; // set library to nullptr once FT_Done_FreeType succeeds.
#ifdef DOKUN_DEBUG
        dokun::Logger("freetype closed.");
#endif
}
/////////////
bool dokun::Font::load(const std::string& file_name) // load a font as a whole
{
    FT_Error error = FT_New_Face( library, file_name.c_str(), 0, &face );
	if(error) { dokun::Logger("Could not load font from " + file_name); return false; }
	this->file = file_name; // save filename once FT_New_Face is succeeds.
	error = FT_Set_Pixel_Sizes(face, width, height); // ppem (pixel per em)  // at 8, "Sid" would be 13 x 5 | at 16, "Sid" would be 24 x 11 |at 32, "Sid" would be 47 x 22  | at 48, "Sid" would be 74 x 32 //FT_Error error = FT_Set_Char_Size( face, height * 64, 0, 72, 0 );   // DPI: 72 or 96 
    if(error) { dokun::Logger("Could not set pixel size " + file_name); return false; }
    return true;
}
/////////////
bool dokun::Font::load(const void * buffer, long size)
{
    FT_Error error = FT_New_Memory_Face( library, static_cast<FT_Byte *>(const_cast<void *>(buffer)), static_cast<FT_Long>(size), 0, &face );
    if(error) { dokun::Logger("Could not load font from memory"); return false; }
	return true;
}
/////////////
int dokun::Font::load(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushboolean(L, font->load(lua_tostring(L, 2)));
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void dokun::Font::generate() // load each character and store it in array for later use
{
	if(is_generated()) return; // return if previously generated
	for (unsigned char c = 0; c < face->num_glyphs/*128*/; c++) {
        if (FT_Load_Char(static_cast<FT_Face>(face), c, FT_LOAD_RENDER)) //Load and render glyph
	    {
	        dokun::Logger("Could not load glyph from font");
			continue;
	    }
		// generate texture for each glyph (character)
        unsigned int texture_id = 0;		
    #ifdef DOKUN_OPENGL
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
		glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
		    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		glBindTexture(GL_TEXTURE_2D, 0);
	#endif
        // get metrics information --------------------------------
        metrics[ c ] = face->glyph->metrics;
        //Calculate max bearing
        if( metrics[ c ].horiBearingY / 64 > max_bearing )
        {
            max_bearing = metrics[ c ].horiBearingY / 64;
        }
        //Calculate max width
        if( metrics[ c ].width / 64 > cellW )
        {
            cellW = metrics[ c ].width / 64;
        }
        //Calculate gylph hang
        int glyph_hang = ( metrics[ c ].horiBearingY - metrics[ c ].height ) / 64;
        if( glyph_hang < min_hang )
        {
            min_hang = glyph_hang;
        }
        //Create bitmap font
            cellH = max_bearing - min_hang;
            //createPixels8( cellW * 16, cellH * 16 );

            //Begin creating bitmap font
            unsigned int currentChar = 0;
            //LFRect nextClip = { 0.f, 0.f, cellW, cellH };

            //Blitting coordinates
            int bX = 0;
            int bY = 0;
        //std::cout << "Cell dimensions: " << Vector2(cellW, cellH) << " max_bearing: " << max_bearing << " min_hang" << min_hang << " glyph: " << c << std::endl;
        // get character (glyph) information ---------------------
		Character character = {
			texture_id,                // id
			face->glyph->bitmap.width, // width // if char is a space, change width
			face->glyph->bitmap.rows , // height
			face->glyph->bitmap_left , // bearing_x 
			face->glyph->bitmap_top  , // bearing_y
			face->glyph->advance.x   , // advance_x
			face->glyph->advance.y   , // advance_y
            face->glyph->bitmap.buffer,// buffer (data)
            //line_height = static_cast((face->size->metrics.ascender - face->size->metrics.descender) >> 6);
		};
	#ifdef DOKUN_DEBUG0 // show all glyphs and their sizes
	    if(glIsTexture(texture_id)) std::cout << "GL_texture: " << texture_id << " generated for glyph: " << c << std::endl;//std::cout << String::to_string(c) << " "<< character.width << ", " << character.height << " (buffer: " << String::to_string(texture_id) << ")" << std::endl;//dokun::Logger(String::to_string(c) +" "+ std::to_string(character.width) +", "+ std::to_string(character.height) + "\n");
	#endif
        // save bitmaps to Texture -------------------------------
        Texture * texture = new Texture(face->glyph->bitmap.buffer, face->glyph->bitmap.width, 
            face->glyph->bitmap.rows, 1, 6403); // depth=1, channel=GL_RED=6403
        texture->set_filter(GL_LINEAR, GL_LINEAR);
        texture->set_wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        texture->set_internal_format(GL_RED);
        texture->set_format(GL_RED);
        texture->set_buffer(texture_id);// copy GL_texture as well
        texture->set_name(String::to_string(c)); // copy character
        bitmaps.push_back(texture); // store bitmap in vector
        //std::cout << "Texture name is: " << bitmaps[c]->get_name() << "; bitmap_array_size: " << bitmaps.size() << "; character: " << c << "; width: " << bitmaps[c]->get_width() << "; height: " << bitmaps[c]->get_height() << " (buffer: " << String::to_string(bitmaps[c]->get_buffer()) << ")"  << std::endl;
		//--------------------------------------------------------
        // store each character (for later use)
		character_array.insert(std::pair<char, Character>(c, character));
		// you can store and replace texture_id like this: character_array['a'].id = 10; // WORKS!!  //character_array.find('a')->second.id = 10;
	}
	#ifdef DOKUN_DEBUG0
	    //dokun::Logger("Font_" + String::to_string((int)Factory::get_font_factory()->get_location(this)) + " generated with " + String::to_string((int)character_array.size()) + " characters.");
	#endif
}
/////////////
int dokun::Font::generate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        font->generate();
    }
    return 0;
}
/////////////
void dokun::Font::destroy()
{
#ifdef DOKUN_OPENGL
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
    int character_count = character_array.size();	
    for(int i = 0; i < character_count; i++)
	{
		GLuint texture = character_array[i].id;
		if(glIsTexture(texture))
		{
	        glDeleteTextures(1, &texture); // delete each character's GL_texture
            character_array[i].id = 0;         // set to nullptr
	    #ifdef DOKUN_DEBUG0
            if(!glIsTexture(texture)) {std::cout << "GL_texture: " << texture << " deleted" << std::endl;} // check if GL_texture is actually deleted
        #endif	            
        } 
	}
    // clear character_array so you can push_back new glyphs
    character_array.clear(); 
    // destroy face once done getting all font information
    FT_Error error = FT_Done_Face(face);
    if(error) {std::cout << "FT_Done_Face failed." << std::endl; return;}
    face = nullptr; // set face to nullptr once deleted with "FT_Done_Face"
#ifdef DOKUN_DEBUG0
    if(!face) std::cout << "Font_" << String(Factory::get_font_factory()->get_location(this)) << ": face deleted." << std::endl;
#endif    
#endif	
}
/////////////
int dokun::Font::destroy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        font->destroy();		
	}
	return 0;
}
/////////////
/////////////
void dokun::Font::copy(const dokun::Font& font)
{
    // destroy old texture so you can generate a new one
    if(is_generated()) destroy();
	
    width  = font.get_width (); // in pixels
	height = font.get_height(); // in pixels
	face   = font.get_face  ();
	file   = font.get_file  ();
}
/////////////
int dokun::Font::copy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    dokun::Font * font2 = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
 	    {
	        dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
            font->copy(*font2);		
	    }
    }
    return 0;
}
/////////////
/////////////
/////////////
/////////////
/////////////
//setters
void dokun::Font::set_width(unsigned int width) // ONLY works if set before loading font
{
    this->width = width;
}
/////////////
int dokun::Font::set_width(lua_State *L) // ONLY works if set before loading font
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        font->set_width(lua_tonumber(L, 2));
    }
    return 0;
}
/////////////
void dokun::Font::set_height(unsigned int height) // ONLY works if set before loading font
{
    this->height = height;
}
/////////////
int dokun::Font::set_height(lua_State *L) // ONLY works if set before loading font
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        font->set_height(lua_tonumber(L, 2));
    }
    return 0;
}
/////////////
void dokun::Font::set_size(unsigned int width, unsigned int height) // ONLY works if set before loading font
{
    set_width(width);
    set_height(height);
}
/////////////
int dokun::Font::set_size(lua_State *L) // ONLY works if set before loading font
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        font->set_size(lua_tonumber(L, 2), lua_tonumber(L, 3));
    }
    return 0;
}
/////////////
void dokun::Font::set_pixel_size(unsigned int width, unsigned int height) {   
    this->width = width;
    this->height = height;
    // this always fails for some reason. I guess you have to set the pixel size before loading the font
    /*FT_Error error = FT_Set_Pixel_Sizes(face, width, height);
    if(error) std::cout << DOKUN_UI_TAG "Could not set font pixel size" << std::endl;*/
}    
/////////////
/////////////
/////////////
/////////////
int dokun::Font::get_width() const  // returns FT_PIXEL_SIZES width
{
    return width;
}
/////////////
int dokun::Font::get_width(char glyph) const // returns width of single character (glyph)
{
    const_cast<dokun::Font *>(this)->generate(); // generate font texture to get the glyph size// NOTE: IT is impossible to get the size of a character unless character_array has already been generated beforehand
    if(!is_generated()) return 0;
    if(glyph == ' ') return face->glyph->advance.x >> 6; // white space " " width comes up as 0, need to fix that, set to 10 for now // the advance is for spacing between characters in a string of text
    return character_array.find(glyph)->second.width;//face->glyph->bitmap.width;//character_array['A'].width;
}
/////////////
int dokun::Font::get_width(const std::string& text) const // returns biggest font_width (actual font_width in pixels) multiplied by the number_of_characters in arg "text"
{
    double font_size = get_height(); 
    Vector2 resolution = Vector2(72, 72);//Resolution means DPI here (dots per inch)    //Standard values are 72 or 96 dpi for display devices like the screen. The resolution is used to compute the character pixel size from the character point size.
    double pixel_size = font_size * resolution.x / 72; // 16
    int font_width = round((face->bbox.xMax - face->bbox.xMin) * pixel_size / face->units_per_EM); //Font width in pixels (returns the biggest width of a glyph in the font (rounded))
    // -------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------
    int num_newline = String::occurrences(text, "\n");
#ifdef DOKUN_DEBUG0    
    std::cout << "newlines found: " << num_newline << std::endl;
    for(int i = 0; i < text.size(); i++) {
        if(text[i] == '\n') {std::cout << "newline at index: text[" << i << "]" << std::endl;} // find location of newline in the text //std::cout << "first_string " << text.substr(0, text.find("\n")) << std::endl; std::cout << "last_string "  << text.substr(text.find_last_of("\n") + 1)<< std::endl<< std::endl;// newline_pos + 1   find_char_at_index
    }
#endif    
    std::vector<std::string> words = String::split(text, "\n"); // split text after newline to find the word with the most characters
    std::string word_with_most_characters = "";
    for(int i = 0; i < words.size();i++)
    {
    #ifdef DOKUN_DEBUG0
        std::cout << "words found in text: " << words[i] << std::endl;
    #endif    
        if( words[i].length() > word_with_most_characters.length() ) { word_with_most_characters = words[i];}
    }
#ifdef DOKUN_DEBUG0    
    std::cout << "word with most glyphs: " << word_with_most_characters << ", "<< word_with_most_characters.size() << std::endl;
#endif
    if(num_newline > 0) return font_width * word_with_most_characters.size(); // if text contains a newline, return the font_width multiplied by the word with the most glyphs (or characters)
    //--------------------------------------------------------------------------------------------
    return font_width * text.size(); // if text has no newline, return the font_size multiplied by the text's normal length
}
/////////////
int dokun::Font::get_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_width());
		return 1;		
	}
	lua_pushnil(L);
	return 1;		
}
/////////////
int dokun::Font::get_height() const // returns FT_PIXEL_SIZES height
{
    return height;
}
/////////////
int dokun::Font::get_height(char glyph) const // returns height of single character (glyph)
{
    const_cast<dokun::Font *>(this)->generate();
    if(!is_generated()) return 0;
    return character_array.find(glyph)->second.height;//return face->glyph->bitmap.rows;	 // NOTE: IT is impossible to get the size of a character unless character_array has already been generated beforehand
}
/////////////
int dokun::Font::get_height(const std::string& text) const // returns the largest height of a single character (glyph) found in the text // This also returns the same "height" (returns biggest (actual font_height in pixels)  height found in a glyph in the font)
{
    // But what if the text contains a newline? Then the height would have to change!
    int font_size = get_height(); // 16
    Vector2 resolution = Vector2(72, 72);//Resolution means DPI here (dots per inch)    //Standard values are 72 or 96 dpi for display devices like the screen. The resolution is used to compute the character pixel size from the character point size.
    double pixel_size = font_size * resolution.y / 72; // 16
    int font_height = round((face->bbox.yMax - face->bbox.yMin) * pixel_size / face->units_per_EM); //std::cout << "FONT height = " << font_height << std::endl;
    // -------------------------------------------------------------------------------------------
    int num_newline = String::occurrences(text, "\n"); 
#ifdef DOKUN_DEBUG0  
    std::cout << "newlines found: " << num_newline << std::endl;  /*if(num_newline != 0) std::cout << "text " << text << " contains a newline. Please change height!\n";*/
#endif
    std::vector<std::string> words = String::split(text, "\n");
#ifdef DOKUN_DEBUG0     
    std::cout << "words seperated by newline: " << words.size() << std::endl;
#endif // newlines still count as part of text
    if(num_newline == 1 && (words.size() == 2)) return font_height * 2; // newline but with a 2nd word on the newline
    if(num_newline >  0) return font_height * num_newline;//* words.size(); // number of words seperated by newline (font_height = 19 * num_newline)
    // -------------------------------------------------------------------------------------------
    return font_height;
}
/////////////
int dokun::Font::get_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_height());
		return 1;		
	}
	lua_pushnil(L);
	return 1;	
}
/////////////
Vector2 dokun::Font::get_size() const
{
	return Vector2(get_width(), get_height());
}
/////////////
Vector2 dokun::Font::get_size(char glyph) const
{
	return Vector2(get_width(glyph), get_height(glyph));
}
/////////////
int dokun::Font::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_size().x);
		lua_pushnumber(L, font->get_size().y);
		return 2;		
	}
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
// this gives totally incorrect results
Vector4 dokun::Font::get_rect()const
{
	int x = face->bbox.xMax;      // right 
	int y = face->bbox.yMin;      // bottom
	int width  = face->bbox.xMin; // left
	int height = face->bbox.yMax; // top
	return Vector4(x, y, width, height);
#ifdef DOKUN_DEBUG
#endif
}
/////////////
int dokun::Font::get_rect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_rect().x);
		lua_pushnumber(L, font->get_rect().y);
		lua_pushnumber(L, font->get_rect().z);
		lua_pushnumber(L, font->get_rect().w);
		return 4;		
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;		
}
/////////////
FT_Byte * dokun::Font::get_data() const
{
	return data;
}
/////////////
int dokun::Font::get_data(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, font->get_data());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
FT_Face dokun::Font::get_face() const
{
	return face;
}
/////////////
int dokun::Font::get_face(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, font->get_face());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
/////////////
std::string dokun::Font::get_file()const
{
	return file;
}
/////////////
int dokun::Font::get_file(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushstring(L, font->get_file().c_str());
		return 1;
	}
	lua_pushstring(L, ""); // return empty string (default)
    return 1;
}
/////////////
unsigned int dokun::Font::get_buffer(char glyph) const
{
    if(!is_generated()) { dokun::Logger("Font::get_buffer: Cannot get buffer without a context."); return 0; }
    return character_array.find(glyph)->second.id;
}
/////////////
unsigned int dokun::Font::get_buffer(const std::string& glyph) const
{
    if(!is_generated()) { dokun::Logger("Font::get_buffer: Cannot get buffer without a context."); return 0; }
    return character_array.find(glyph[0])->second.id;
}
/////////////
int dokun::Font::get_buffer(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        lua_pushinteger(L, font->get_buffer(lua_tostring(L, 2)));
		return 1;
	}
    lua_pushinteger(L, 0);
    return 1;
}
/////////////
Texture * dokun::Font::get_bitmap(int index) const // returns bitmap by index
{
    return bitmaps[index];
}
/////////////
Texture * dokun::Font::get_bitmap(const std::string& name) const // returns bitmap by name
{
    for(int b = 0; b < bitmaps.size(); b++)
    {
        Texture * bitmap = bitmaps[b];
        if(bitmap->get_name() == name) return bitmap;
    }
    return nullptr;
}
int dokun::Font::get_bitmap(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, font->get_bitmap(lua_tonumber(L, 2)));
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int dokun::Font::get_character_count() const
{
    return face->num_glyphs;
}
int dokun::Font::get_character_count(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        lua_pushnumber(L, font->get_character_count());
        return 1;
    }
    lua_pushnumber(L, 0);
    return 1;
}
/////////////
std::string dokun::Font::get_family_name()const
{
    return face->family_name;
}
int dokun::Font::get_family_name(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushstring(L, font->get_family_name().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string dokun::Font::get_style_name ()const
{
    return face->style_name;
}
int dokun::Font::get_style_name (lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushstring(L, font->get_style_name().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
/////////////
/////////////
Vector2 dokun::Font::get_bearing(char glyph)const // char holds only -128 to 127. unsigned char holds 0 to 255
{
    const_cast<dokun::Font *>(this)->generate();
    if(!is_generated()) return 0;
    return Vector2(character_array.find(glyph)->second.bearing_x, character_array.find(glyph)->second.bearing_y);
}
int get_bearing(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_bearing(lua_tonumber(L, 2)).x);
		lua_pushnumber(L, font->get_bearing(lua_tonumber(L, 2)).y);
		return 2;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;
}
/////////////
Vector2 dokun::Font::get_advance(char glyph)const
{
    const_cast<dokun::Font *>(this)->generate();
    if(!is_generated()) return 0;
    return Vector2(character_array.find(glyph)->second.advance_x, character_array.find(glyph)->second.advance_y);    
}
int get_advance(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_advance(lua_tonumber(L, 2)).x);
		lua_pushnumber(L, font->get_advance(lua_tonumber(L, 2)).y);
		return 2;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;
}
/////////////
/////////////
/////////////
/////////////
/*
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
		lua_push(L, font->get_());
		return 1;
	}
	lua_pushnil(L);
	return 1;
*/
/////////////
/////////////
/////////////
/////////////
bool dokun::Font::is_generated() const // checks if a texture has been generated for each glyph
{
	return !character_array.empty();
}
int dokun::Font::is_generated(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Font * font = *static_cast<dokun::Font **>(lua_touserdata(L, -1));
        lua_pushboolean(L, font->is_generated());
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}
/////////////
/* // This is all the "face" data members
typedef struct  FT_FaceRec_
  {
    FT_Long           num_faces;
    FT_Long           face_index;

    FT_Long           face_flags;
    FT_Long           style_flags;

    FT_Long           num_glyphs;

    FT_String*        family_name;
    FT_String*        style_name;

    FT_Int            num_fixed_sizes;
    FT_Bitmap_Size*   available_sizes;

    FT_Int            num_charmaps;
    FT_CharMap*       charmaps;

    FT_Generic        generic;

    //# The following member variables (down to `underline_thickness`)
    //# are only relevant to scalable outlines; cf. @FT_Bitmap_Size   
    //# for bitmap fonts.                                             
    FT_BBox           bbox;

    FT_UShort         units_per_EM;
    FT_Short          ascender;
    FT_Short          descender;
    FT_Short          height;

    FT_Short          max_advance_width;
    FT_Short          max_advance_height;

    FT_Short          underline_position;
    FT_Short          underline_thickness;

    FT_GlyphSlot      glyph;
    FT_Size           size;
    FT_CharMap        charmap;

    //@private begin

    FT_Driver         driver;
    FT_Memory         memory;
    FT_Stream         stream;

    FT_ListRec        sizes_list;

    FT_Generic        autohint;   // face-specific auto-hinter data
    void*             extensions; // unused                        

    FT_Face_Internal  internal;

    //@private end

  } FT_FaceRec;
  
// Glyph properties  
  typedef struct  FT_GlyphSlotRec_
  {
    FT_Library        library;
    FT_Face           face;
    FT_GlyphSlot      next;
    FT_UInt           reserved;       // retained for binary compatibility
    FT_Generic        generic;

    FT_Glyph_Metrics  metrics;
    FT_Fixed          linearHoriAdvance;
    FT_Fixed          linearVertAdvance;
    FT_Vector         advance;

    FT_Glyph_Format   format;

    FT_Bitmap         bitmap;
    FT_Int            bitmap_left;
    FT_Int            bitmap_top;

    FT_Outline        outline;

    FT_UInt           num_subglyphs;
    FT_SubGlyph       subglyphs;

    void*             control_data;
    long              control_len;

    FT_Pos            lsb_delta;
    FT_Pos            rsb_delta;

    void*             other;

    FT_Slot_Internal  internal;

  } FT_GlyphSlotRec;  
  
// Bitmap properties
 typedef struct  FT_Bitmap_
  {
    int             rows;
    int             width;
    int             pitch;
    unsigned char*  buffer;
    short           num_grays;
    char            pixel_mode;
    char            palette_mode;
    void*           palette;

  } FT_Bitmap;
  
  
*/
/*
Number of glyphs in font /usr/share/fonts/X11/Type1/c0583bt_.pfb: 229       // std::cout << "Number of glyphs in font " << file_name << ": " << face->num_glyphs << std::endl;
Font family: Courier 10 Pitch                                               // std::cout << "Font family: " << face->family_name << std::endl;
Font style: Bold                                                            // std::cout << "Font style: " << face->style_name << std::endl;
Font units_per_EM: 1000                                                     // std::cout << "Font units_per_EM: " << face->units_per_EM << std::endl;
Font bbox: -44 -309 664 875                                                 // std::cout << "Font bbox: " << Vector4(face->bbox.xMin, face->bbox.yMin, face->bbox.xMax, face->bbox.yMax) << std::endl;
Number of charmaps in font: 2                                               // std::cout << "Number of charmaps in font: " << face->num_charmaps << std::endl;
                                                                            //std::cout << ": " << face-> << std::endl; 
*/
/////////////
/////////////
/////////////
