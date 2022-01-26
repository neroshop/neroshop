#ifndef TYPES_HPP_DOKUN
#define TYPES_HPP_DOKUN

#include "platform.hpp"
#include "vector.hpp"
#ifdef __cplusplus
#include <functional> // std::function
#include <utility>
#include <tuple>
#include <vector>
#endif

// data types
typedef unsigned char dokun_byte;
typedef unsigned int  dokun_uint;
// functions
#ifdef __cplusplus
typedef std::function<void (void)> dokun_function;
#endif
// globals
// defines
#ifndef __CLASS__
#define __CLASS__(class_name) typeid(class_name).name()
#define __CLASST__ typeid(*this).name()
#endif
// structs
typedef struct dokun_Character 
{
    unsigned int texture;  // ID handle of the glyph texture
    Vector2 size;       // Size of glyph
    Vector2 bearing;    // Offset from baseline to left/top of glyph
    long int advance;    // Offset to advance to next glyph
} dokun_Character;
typedef struct dokun_GPU
{
	#ifdef __cplusplus
	std::string gpu_name;
	std::string gpu_vendor;
	std::string API_name;
	std::tuple<int, int, int> API_version;
	std::string API_version_str;
	#endif
	unsigned int API_version_major;
	unsigned int API_version_minor;
	unsigned int API_version_patch;
	#ifdef __cplusplus
	std::string shader_version;
	std::string shader_lang;
	#endif
	unsigned int ext_count;
} dokun_GPU;
// enumerators
typedef enum dokun_Key {
    DOKUN_KEY_UP    = 0x26,
    DOKUN_KEY_DOWN  = 0x28,
	DOKUN_KEY_LEFT  = 0x25,
	DOKUN_KEY_RIGHT = 0x27,
	
    DOKUN_KEY_A = 65,
    DOKUN_KEY_B = 66,
    DOKUN_KEY_C = 67,
    DOKUN_KEY_D = 68,
    DOKUN_KEY_E = 69,
    DOKUN_KEY_F = 70,
    DOKUN_KEY_G = 71,
    DOKUN_KEY_H = 72,
    DOKUN_KEY_I = 73,
    DOKUN_KEY_J = 74,
    DOKUN_KEY_K = 75,
    DOKUN_KEY_L = 76,
    DOKUN_KEY_M = 77,
    DOKUN_KEY_N = 78,
    DOKUN_KEY_O = 79,
    DOKUN_KEY_P = 80,
    DOKUN_KEY_Q = 81,
    DOKUN_KEY_R = 82,
    DOKUN_KEY_S = 83,
    DOKUN_KEY_T = 84,
    DOKUN_KEY_U = 85,
    DOKUN_KEY_V = 86,
    DOKUN_KEY_W = 87,
    DOKUN_KEY_X = 88,
    DOKUN_KEY_Y = 89,
    DOKUN_KEY_Z = 90,
	
    DOKUN_KEY_1 = 0x31,
    DOKUN_KEY_2 = 0x32,
    DOKUN_KEY_3 = 0x33,
    DOKUN_KEY_4 = 0x34,
    DOKUN_KEY_5 = 0x35,
    DOKUN_KEY_6 = 0x36,
    DOKUN_KEY_7 = 0x37,
    DOKUN_KEY_8 = 0x38,
    DOKUN_KEY_9 = 0x39,
    DOKUN_KEY_0 = 0x30,
    // symbols here
	/*
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,	
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,
    DOKUN_KEY_ = 0,		
	*/	
	// OTHER
	DOKUN_KEY_ESCAPE = 27,
	DOKUN_KEY_BACKSPACE = 0,
	DOKUN_KEY_SPACE = 0x20,
	DOKUN_KEY_RETURN = 0,
    DOKUN_KEY_ENTER = DOKUN_KEY_RETURN,
    DOKUN_KEY_LSHIFT = 0,
    DOKUN_KEY_RSHIFT = 0,
    DOKUN_KEY_LCTRL = 0,
    DOKUN_KEY_RCTRL = 0,
    DOKUN_KEY_LALT = 0,
    DOKUN_KEY_RALT = 0,
    
	DOKUN_KEY_TAB= 0,
    DOKUN_KEY_CAPS_LOCK = 0,
    DOKUN_KEY_CAPITAL = 0,
    //DOKUN_KEY_ = 0,	
    DOKUN_KEY_INSERT = 0x2D,
	DOKUN_KEY_DELETE = 0,
    DOKUN_KEY_HOME = 0,
    DOKUN_KEY_END = 0,
    DOKUN_KEY_PGUP = 0,
    DOKUN_KEY_PGDOWN = 0,
   
    DOKUN_KEY_NUM_LOCK = 0,
    DOKUN_KEY_PRINTSCREEN = 0,	
    DOKUN_KEY_SCROLL = 0, // scroll lock
	DOKUN_KEY_HELP = 0x2F,
		
    DOKUN_KEY_NUMPAD1 = 0,
    DOKUN_KEY_NUMPAD2 = 0,
    DOKUN_KEY_NUMPAD3 = 0,
    DOKUN_KEY_NUMPAD4 = 0,
    DOKUN_KEY_NUMPAD5 = 0,
    DOKUN_KEY_NUMPAD6 = 0,
    DOKUN_KEY_NUMPAD7 = 0,
    DOKUN_KEY_NUMPAD8 = 0,
    DOKUN_KEY_NUMPAD9 = 0,
    DOKUN_KEY_NUMPAD0 = 0,
    //DOKUN_KEY_NUMPAD = 0,
	
	DOKUN_KEY_ADD      = 0,
    DOKUN_KEY_MULTIPLY = 0,
    DOKUN_KEY_SUBTRACT = 0,
    DOKUN_KEY_DIVIDE   = 0,

    DOKUN_KEY_F1  = 0,
    DOKUN_KEY_F2  = 0,
    DOKUN_KEY_F3  = 0,
    DOKUN_KEY_F4  = 0,
    DOKUN_KEY_F5  = 0,
    DOKUN_KEY_F6  = 0,
    DOKUN_KEY_F7  = 0,
    DOKUN_KEY_F8  = 0,
    DOKUN_KEY_F9  = 0,
    DOKUN_KEY_F10 = 0,
    DOKUN_KEY_F11 = 0,
    DOKUN_KEY_F12 = 0,	

	// natural keys
    DOKUN_KEY_LWIN = 0,
    DOKUN_KEY_RWIN = 0,	
    DOKUN_KEY_MODIFIER = 0, // FN
    DOKUN_KEY_FN = 0,		
	DOKUN_KEY_UNDEFINED = 0x1a,
} dokun_Key;

typedef enum dokun_Mouse { // good
	DOKUN_MOUSE_LEFT   = 1,
	DOKUN_MOUSE_MIDDLE = 2,
	DOKUN_MOUSE_RIGHT  = 3,
} dokun_Mouse;

typedef enum dokun_Shader {
	DOKUN_SHADER_VERTEX          = 0,
	DOKUN_SHADER_FRAGMENT        = 1,
	DOKUN_SHADER_GEOMETRY        = 2,
	DOKUN_SHADER_TESS_CONTROL    = 3,
	DOKUN_SHADER_TESS_EVALUATION = 4,
	DOKUN_SHADER_COMPUTE         = 5,
	//DOKUN_SHADER_ = ,
} dokun_Shader;

typedef enum dokun_Object {
	DOKUN_OBJECT_NULL   = -1,
	DOKUN_OBJECT_SPRITE =  0,
	DOKUN_OBJECT_MODEL  =  1,
	DOKUN_OBJECT_GUI    =  2,
} dokun_Object;

typedef enum dokun_Component // good
{
	DOKUN_COMPONENT_NONE     = -1,
	DOKUN_COMPONENT_INTEGER  = 0,
	DOKUN_COMPONENT_DECIMAL  = 1,
	DOKUN_COMPONENT_STRING   = 2,
	DOKUN_COMPONENT_VECTOR   = 3,
	DOKUN_COMPONENT_POINTER  = 4,
	DOKUN_COMPONENT_FUNCTION = 5,
	DOKUN_COMPONENT_BOOLEAN  = 10,
	DOKUN_COMPONENT_THREAD   = 20,
} dokun_Component;

typedef enum dokun_Map
{
	DOKUN_NORMAL_MAP,
	DOKUN_SHADOW_MAP,
} dokun_Map;

typedef enum dokun_TextureParamDef
{
#ifdef DOKUN_OPENGL
    // MINMAGFILTER
    DOKUN_NEAREST = GL_NEAREST,
	DOKUN_LINEAR  = GL_LINEAR ,
	DOKUN_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	DOKUN_LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST ,
	DOKUN_NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR ,
	DOKUN_LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR  ,
	// WRAP
	DOKUN_REPEAT          = GL_REPEAT,
	DOKUN_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	DOKUN_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
	DOKUN_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	DOKUN_MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
#endif
#ifndef DOKUN_OPENGL
    // MINMAGFILTER
    DOKUN_NEAREST = 0 ,
	DOKUN_LINEAR  = 1 ,
	DOKUN_NEAREST_MIPMAP_NEAREST = 2 ,
	DOKUN_LINEAR_MIPMAP_NEAREST  = 3 ,
	DOKUN_NEAREST_MIPMAP_LINEAR  = 4 ,
	DOKUN_LINEAR_MIPMAP_LINEAR   = 5 ,
	// WRAP
	DOKUN_REPEAT          = 0,
	DOKUN_MIRRORED_REPEAT = 1,
	DOKUN_CLAMP_TO_EDGE   = 2,
	DOKUN_CLAMP_TO_BORDER = 3,
	DOKUN_MIRROR_CLAMP_TO_EDGE = 4,
#endif	
} dokun_TextureParamDef;

typedef enum dokun_VideoRecord
{
	DOKUN_SCREEN = 0,
	DOKUN_WEBCAM = 1,
	DOKUN_RECORD_SCREEN = 0,
	DOKUN_RECORD_WEBCAM = 1,
}dokun_VideoRecord;

typedef enum dokun_Format
{
	// audio
	DOKUN_FORMAT_MONO8,
	DOKUN_FORMAT_STEREO8,
	DOKUN_FORMAT_MONO16,
	DOKUN_FORMAT_STEREO16,	
	// video + image
	DOKUN_VIDEO_FORMAT_YV12 = 0,//THEORAPLAY_VIDFMT_YV12,  /* NTSC colorspace, planar YCrCb 4:2:0 */
    DOKUN_VIDEO_FORMAT_IYUV = 1,//THEORAPLAY_VIDFMT_IYUV,  /* NTSC colorspace, planar YCbCr 4:2:0 */
	DOKUN_VIDEO_FORMAT_RGB  = 2,//THEORAPLAY_VIDFMT_RGB,
	DOKUN_VIDEO_FORMAT_RGBA = 3,//THEORAPLAY_VIDFMT_RGBA,
#ifdef DOKUN_OPENGL
    DOKUN_RG          = GL_RG  ,
    DOKUN_RGB         = GL_RGB ,
	DOKUN_RGBA        = GL_RGBA,
    DOKUN_FORMAT_RGB  = GL_RGB ,   /* 24 bits packed pixel RGB */
    DOKUN_FORMAT_RGBA = GL_RGBA,   /* 32 bits packed pixel RGBA (full alpha). */
#endif
#ifndef DOKUN_OPENGL
    DOKUN_RG         ,
    DOKUN_RGB        ,
	DOKUN_RGBA       ,
    DOKUN_FORMAT_RGB ,   /* 24 bits packed pixel RGB */
    DOKUN_FORMAT_RGBA,   /* 32 bits packed pixel RGBA (full alpha). */
#endif
} dokun_Format;

typedef enum dokun_Dialog {
	DOKUN_DIALOG_OPEN  = 0,
	DOKUN_DIALOG_SAVE  = 1,
	DOKUN_DIALOG_PRINT = 2,
} dokun_Dialog;

typedef enum dokun_IO {
	DOKUN_IO_READ       = 0,
	DOKUN_IO_WRITE      = 2,
	DOKUN_IO_READ_WRITE = 4,
	DOKUN_IO_BOTH       = 4,
	DOKUN_IO_BINARY     = 8,
	DOKUN_IO_AT_END     = 16,
	DOKUN_IO_APPEND     = 32,
	DOKUN_IO_TRUNCATE   = 64,
} dokun_IO;

typedef enum dokun_Side {
	DOKUN_SIDE_ALL          = 0,
	DOKUN_SIDE_TOP_LEFT     = 1,
	DOKUN_SIDE_TOP_RIGHT    = 2,
	DOKUN_SIDE_BOTTOM_LEFT  = 3,
	DOKUN_SIDE_BOTTOM_RIGHT = 4,
} dokun_Side;

typedef enum dokun_Orientation {
	DOKUN_ORIENTATION_HORIZONTAL = 0,
	DOKUN_ORIENTATION_VERTICAL   = 1,
} dokun_Orientation;

typedef enum dokun_Border {
	DOKUN_BORDER_SOLID  = 0,
	DOKUN_BORDER_DASHED = 1,
	DOKUN_BORDER_DOTTED = 2,
	DOKUN_BORDER_DOUBLE = 3,
	DOKUN_BORDER_GROOVE = 4,
	DOKUN_BORDER_RIDGE  = 5,
	DOKUN_BORDER_INSET  = 6,
	DOKUN_BORDER_OUTSET = 7,
} dokun_Border;

typedef enum dokun_Action {
	DOKUN_PRESSED  = 0,
	DOKUN_HOVERED  = 1,
	DOKUN_FOCUSED  = 2,
	DOKUN_SELECTED = 3,
} dokun_Action;

typedef enum dokun_State { 
	DOKUN_STATE_NORMAL   = 0,
	DOKUN_STATE_ACTIVE   = 1,
	DOKUN_STATE_DISABLED = 2,
	DOKUN_STATE_HOVERED  = 3,
	DOKUN_STATE_PRESSED  = 4,
	DOKUN_STATE_FOCUSED  = 5,
} dokun_State; 

typedef enum dokun_Style {
	// Label
	DOKUN_STYLE_REGULAR       = 0,
	DOKUN_STYLE_BOLD          = 2,
	DOKUN_STYLE_ITALIC        = 4,
	DOKUN_STYLE_UNDERLINED    = 8,
	DOKUN_STYLE_STRIKETHROUGH = 16,
	// Window
#ifdef DOKUN_WIN32	
	DOKUN_BORDER  = WS_BORDER,
	DOKUN_CAPTION = WS_CAPTION,
	DOKUN_CHILD   = WS_CHILD, // cannot be used with pop_up
	DOKUN_CHILDWINDOW  = WS_CHILDWINDOW,
	DOKUN_CLIPCHILDREN = WS_CLIPCHILDREN,
	DOKUN_CLIPSIBLINGS = WS_CLIPSIBLINGS,
	DOKUN_DISABLED = WS_DISABLED,
	DOKUN_DLGFRAME = WS_DLGFRAME, // double border, no title
	DOKUN_GROUP    = WS_GROUP,
	DOKUN_HSCROLL  = WS_HSCROLL, // has a horizontal scroll ba
	DOKUN_ICONIC   = WS_ICONIC,
	DOKUN_MAXIMIZE = WS_MAXIMIZE,
	DOKUN_MAXIMIZEBOX = WS_MAXIMIZEBOX,
	DOKUN_MINIMIZE    = WS_MINIMIZE,
	DOKUN_MINIMIZEBOX      = WS_MINIMIZEBOX,
	DOKUN_OVERLAPPED       = WS_OVERLAPPED, // title bar and a border
	DOKUN_OVERLAPPEDWINDOW = WS_OVERLAPPEDWINDOW,  
	DOKUN_POPUP       = WS_POPUP, // cannot be used with child
	DOKUN_POPUPWINDOW = WS_POPUPWINDOW,
	DOKUN_SIZEBOX = WS_SIZEBOX,
	DOKUN_SYSMENU = WS_SYSMENU,
	DOKUN_TABSTOP = WS_TABSTOP,
	DOKUN_THICKFRAME  = WS_THICKFRAME,
	DOKUN_TILED       = WS_TILED, // same as overlapped
	DOKUN_TILEDWINDOW = WS_TILEDWINDOW,
	DOKUN_VISIBLE = WS_VISIBLE, // is initially visible
	DOKUN_VSCROLL = WS_VSCROLL, // has a vertical scroll bar
	//------------------------
	DOKUN_DOCK    = DOKUN_POPUP,
#endif	
#ifdef __gnu_linux__
	DOKUN_DESKTOP =0,
	DOKUN_DOCK =1,
	DOKUN_TOOLBAR =2,
	DOKUN_MENU =3,
	DOKUN_UTILITY =4,
	DOKUN_SPLASH =5,
	DOKUN_DIALOG =6,
	DOKUN_DROPDOWN_MENU =7,
	DOKUN_POPUP_MENU =8,
	DOKUN_TOOLTIP =9,
	DOKUN_NOTIFICATION =10,
	DOKUN_COMBO =11,
	DOKUN_DND =12,
	DOKUN_NORMAL =13,
#endif
} dokun_Style;

typedef enum dokun_Light {
	DOKUN_LIGHT_DIRECTIONAL = 0,
	DOKUN_LIGHT_POSITIONAL  = 1,
	DOKUN_LIGHT_SPOT        = 2,
	DOKUN_SUN       = DOKUN_LIGHT_DIRECTIONAL,
	DOKUN_LAMP      = DOKUN_LIGHT_POSITIONAL,
    DOKUN_SPOTLIGHT	= DOKUN_LIGHT_SPOT,
} dokun_Light;

typedef enum dokun_Event {
	DOKUN_EVENT_PEEK = 0,
	DOKUN_EVENT_WAIT = 1,
} dokun_Event;

typedef enum dokun_Mode {
	DOKUN_MODE_WINDOW     = 0,
	DOKUN_MODE_FULLSCREEN = 1,
} dokun_Mode;

typedef enum dokun_Scene {
	DOKUN_SCENE_ANY = 0,
	DOKUN_SCENE_2D  = 1,
	DOKUN_SCENE_3D  = 2,
} dokun_Scene;

typedef enum dokun_Origin {
	DOKUN_ORIGIN_TOP_LEFT    = 0,
	DOKUN_ORIGIN_BOTTOM_LEFT = 1,
} dokun_Origin;

typedef enum dokun_PolygonMode {
	DOKUN_POLYGONMODE_FILL      = 0,
	DOKUN_POLYGONMODE_WIREFRAME = 1,
	DOKUN_POLYGONMODE_POINT     = 2,	
} dokun_PolygonMode;

typedef enum dokun_Cursor {
#ifdef DOKUN_WIN32	
	DOKUN_ARROW = 0,
	DOKUN_MOVE  = 1,
	DOKUN_CROSS = 2,
	DOKUN_HAND  = 3,
	DOKUN_START = 4,
	DOKUN_HELP  = 5,
	DOKUN_BEAM  = 6,
	DOKUN_ICON  = 7,
	DOKUN_NON    = 8,
	DOKUN_SIZENS   = 9, 
	DOKUN_SIZEWE   = 10,
    DOKUN_SIZENWSE = 11,
	DOKUN_SIZENESW = 12,
	DOKUN_WAIT     = 13,
	DOKUN_UPARROW  = 14,
#endif	
#ifndef DOKUN_WIN32
	DOKUN_ARROW = 0,
	DOKUN_MOVE  = 1,
	DOKUN_CROSS = 2,
	DOKUN_HAND  = 3,
	DOKUN_START = 4,
	DOKUN_HELP  = 5,
	DOKUN_BEAM  = 6,
	DOKUN_ICON  = 7,
	DOKUN_NON    = 8,
	DOKUN_SIZENS   = 9, 
	DOKUN_SIZEWE   = 10,
    DOKUN_SIZENWSE = 11,
	DOKUN_SIZENESW = 12,
	DOKUN_WAIT     = 13,
	DOKUN_UPARROW  = 14,
#endif
} dokun_Cursor;

typedef enum dokun_MessageBox
{
	DOKUN_BOX_DEFAULT = 0x00000000L,
} dokun_MessageBox;

typedef enum dokun_Default {
	DOKUN_DEFAULT_WIDTH  = 	800,
	DOKUN_DEFAULT_HEIGHT =  600,
#ifdef DOKUN_WIN32
    DOKUN_DEFAULT_POSITION_X  = CW_USEDEFAULT,
	DOKUN_DEFAULT_POSITION_Y  = CW_USEDEFAULT,
#endif	
#ifndef DOKUN_WIN32
    DOKUN_DEFAULT_POSITION_X  = 200,
	DOKUN_DEFAULT_POSITION_Y  = 200,
#endif
} dokun_Default;

typedef enum dokun_Result { // all boolean and dokun_Result functions return a dokun_Result (0 or 1)
	DOKUN_FAILURE = 0,
	DOKUN_SUCCESS = 1,
	DOKUN_GOOD    = 1,
	DOKUN_BAD     = 0,
	DOKUN_OK      = 1,
	DOKUN_NO      = 0,
	DOKUN_YES     = 1,
	DOKUN_ERROR   = 0,
} dokun_Result;

typedef enum dokun_Error { // detects a specific error in function
	// standard
	DOKUN_ERROR_NONE                  = 0   ,
	DOKUN_ERROR_UNKNOWN               = 1001,
	DOKUN_ERROR_INITIALIZATION_FAILED = 1002,
	// object
	DOKUN_ERROR_NULLPTR               = 2001,
	DOKUN_ERROR_OUT_OF_MEMORY         = 2002,
    // file
	DOKUN_ERROR_FILE_NOT_FOUND        = 3001, // if file does not exist in the directory
	DOKUN_ERROR_FILE_MISSING          = 3001,
	DOKUN_ERROR_FILE_INVALID          = 3002, // if file is not supported or file contents are unusual + foreign
	DOKUN_ERROR_FILE_NOT_SUPPORTED    = DOKUN_ERROR_FILE_INVALID,
	DOKUN_ERROR_FILE_BROKEN           = 3003, // if file is broken, corrupted, or an error is found in the file 
} dokun_Error;
#endif
/*
USAGE:
	dokun_Error error;
	Texture * texture = new Texture();
	texture->load("image.png", &error);
	if(error == DOKUN_ERROR_NO_ERROR)
	{
		std::cerr << "File has been successfully loaded\n";
	}
	if(error == DOKUN_ERROR_FILE_NOT_FOUND)
	{
		std::cerr << "File is missing\n";
	}
	if(error == DOKUN_ERROR_FILE_NOT_SUPPORTED)
	{
		std::cerr << "File is not supported\n";
	}
	if(error == DOKUN_ERROR_FILE_BROKEN)
	{
		std::cout << "File is broken\n";
	}	

USAGE:
    void call(dokun_function f) 
	{  
	    f(); 
	}
*/
