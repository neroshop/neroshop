#ifndef KEYBOARD_HPP_DOKUN
#define KEYBOARD_HPP_DOKUN
#include "platform.hpp"

#ifdef DOKUN_WIN32 // must be Windows.
/*
// confirmed !
#define SPACEBAR 0x20
#define EXCLAMATION 0x21 // !
#define QUOTE 0x22 // ""
#define HASHTAG 0x23 // #
#define DOLLAR 0x24 // $
#define PERCENT 0x25 // %
#define AND 0x26 // &
#define APOSTROPHE 0x27 // '
#define LEFT_RBRAKET 0x28 // ( LEFT PARENTHESIS
#define RIGHT_RBRACKET 0X29 // ) RIGHT PARENTHESIS
#define ASTERISK 0x2A // *
#define PLUS 0x2B // +
#define COMMA 0x2C // ,
#define MINUS 0x2D // -
#define PERIOD 0x2E // .
#define SLASH 0x2F// / aka  solidus
*/
    #define DOKUN_KEY_ESCAPE   0xff1b // ESC
    // f1-f12
    #define DOKUN_KEY_F1  0x70
    #define DOKUN_KEY_F2  0x71
    #define DOKUN_KEY_F3  0x72
    #define DOKUN_KEY_F4  0x73
    #define DOKUN_KEY_F5  0x74
    #define DOKUN_KEY_F6  0x75
    #define DOKUN_KEY_F7  0x76
    #define DOKUN_KEY_F8  0x77
    #define DOKUN_KEY_F9  0x78
    #define DOKUN_KEY_F10 0X79
    #define DOKUN_KEY_F11 0X7A
    #define DOKUN_KEY_F12 0X7B
//  -----------------------------
    #define DOKUN_KEY_GRAVE      0x0060 // `
    #define DOKUN_KEY_ASCIITILDE 0x007e // ~ 
    // 0-9
    #define DOKUN_KEY_0 0x30
    #define DOKUN_KEY_1 0x31
    #define DOKUN_KEY_2 0x32
    #define DOKUN_KEY_3 0x33
    #define DOKUN_KEY_4 0x34
    #define DOKUN_KEY_5 0x35
    #define DOKUN_KEY_6 0x36
    #define DOKUN_KEY_7 0x37
    #define DOKUN_KEY_8 0x38
    #define DOKUN_KEY_9 0x39
    // Symbols
    #define DOKUN_KEY_EXCLAMATION 0x21 // !
    #define DOKUN_KEY_AT          0x40 // @
    #define DOKUN_KEY_HASH        0x23 // #
    #define DOKUN_KEY_NUMBERSIGN  0x23 // #
    #define DOKUN_KEY_DOLLAR      0x24 // $
    #define DOKUN_KEY_DOLLARSIGN  0x24 // $
    #define DOKUN_KEY_PERCENT     0x25 // %
    #define DOKUN_KEY_ASCIICIRCUM 0x5e // ^
    #define DOKUN_KEY_AMPERSAND   0x26 // &
    #define DOKUN_KEY_ASTERISK    0x2a // *
    #define DOKUN_KEY_PARENLEFT   0x28 // (
    #define DOKUN_KEY_PARENRIGHT  0x29 // )
    #define DOKUN_KEY_MINUS       0x2d // -
    #define DOKUN_KEY_UNDERSCORE  0x5f // _
    #define DOKUN_KEY_EQUAL       0x3d // =
    #define DOKUN_KEY_PLUS        0x2b // +
    // Special keys
    #define DOKUN_KEY_BACKSPACE   0xff08 // Backspace  
    #define DOKUN_KEY_TAB         0xff09 // Tab <- ->                 
    #define DOKUN_KEY_CAPS_LOCK   0xffe5 // Caps Lock
    #define DOKUN_KEY_SHIFT_L     0xffe1 // Shift_L  
    #define DOKUN_KEY_LSHIFT      DOKUN_KEY_SHIFT_L // Shift_L                                           
    #define DOKUN_KEY_LEFT_SHIFT  DOKUN_KEY_SHIFT_L // Shift_L   
    #define DOKUN_KEY_CTRL_L      0xffe3 // CTRL_L                     
    #define DOKUN_KEY_LEFT_CTRL   DOKUN_KEY_CTRL_L // CTRL_L         
    #define DOKUN_KEY_LCTRL       DOKUN_KEY_CTRL_L // CTRL_L 
    #define DOKUN_KEY_CONTROL_L   DOKUN_KEY_CTRL_L // CTRL_L 
    //#define DOKUN_KEY_HOME        0x0000 // Windows Button (not correct)         
    #define DOKUN_KEY_ALT_L       0xffe9 // ALT_L
    #define DOKUN_KEY_LEFT_ALT    DOKUN_KEY_ALT_L // ALT_L         
    #define DOKUN_KEY_LALT        DOKUN_KEY_ALT_L // ALT_L  
    // Spacebar
    #define DOKUN_KEY_SPACE       0x0020
    // A-Z
    #define DOKUN_KEY_A 65
    #define DOKUN_KEY_B 66
    #define DOKUN_KEY_C 67
    #define DOKUN_KEY_D 68
    #define DOKUN_KEY_E 69
    #define DOKUN_KEY_F 70
    #define DOKUN_KEY_G 71
    #define DOKUN_KEY_H 72
    #define DOKUN_KEY_I 73
    #define DOKUN_KEY_J 74
    #define DOKUN_KEY_K 75
    #define DOKUN_KEY_L 76
    #define DOKUN_KEY_M 77
    #define DOKUN_KEY_N 78
    #define DOKUN_KEY_O 79
    #define DOKUN_KEY_P 80
    #define DOKUN_KEY_Q 81
    #define DOKUN_KEY_R 82
    #define DOKUN_KEY_S 83
    #define DOKUN_KEY_T 84
    #define DOKUN_KEY_U 85
    #define DOKUN_KEY_V 86
    #define DOKUN_KEY_W 87
    #define DOKUN_KEY_X 88
    #define DOKUN_KEY_Y 89
    #define DOKUN_KEY_Z 90
    // Symbols
    #define DOKUN_KEY_BRACKETLEFT  0x5b // [
    #define DOKUN_KEY_BRACKETRIGHT 0x5d // ]
    #define DOKUN_KEY_BRACELEFT    0x7b // {
    #define DOKUN_KEY_BRACERIGHT   0x7d // }
    #define DOKUN_KEY_BACKSLASH    0x5c /* \ */
    #define DOKUN_KEY_BAR          0x7c // |
    #define DOKUN_KEY_SEMICOLON    0x3b // ;
    #define DOKUN_KEY_COLON        0x3a // :
    #define DOKUN_KEY_APOSTROPHE   0x27 // '
    #define DOKUN_KEY_QUOTEDBL     0x22 // "
    #define DOKUN_KEY_QUOTE        DOKUN_KEY_QUOTEDBL
    #define DOKUN_KEY_COMMA        0x2c // ,
    #define DOKUN_KEY_PERIOD       0x2e // .
    #define DOKUN_KEY_LESS         0x3c // <
    #define DOKUN_KEY_GREATER      0x3e // >
    #define DOKUN_KEY_LESSTHAN     DOKUN_KEY_LESS
    #define DOKUN_KEY_GREATERTHAN  DOKUN_KEY_GREATER
    #define DOKUN_KEY_SLASH        0x2f // /
    #define DOKUN_KEY_QUESTION     0x3f // ?
    // Special keys 
    #define DOKUN_KEY_ALT_R       0xffea // ALT_R
    #define DOKUN_KEY_RIGHT_ALT   DOKUN_KEY_ALT_R // ALT_R        
    #define DOKUN_KEY_RALT        DOKUN_KEY_ALT_R // ALT_R
    #define DOKUN_KEY_MENU        0xff67 // [_--] 
    #define DOKUN_KEY_CTRL_R      0xffe4 // CTRL_R                    
    #define DOKUN_KEY_RIGHT_CTRL  DOKUN_KEY_CTRL_R // CTRL_R         
    #define DOKUN_KEY_RCTRL       DOKUN_KEY_CTRL_R // CTRL_R 
    #define DOKUN_KEY_CONTROL_R   DOKUN_KEY_CTRL_R // CTRL_R 
    #define DOKUN_KEY_SHIFT_R     0xffe2 // Shift_R
    #define DOKUN_KEY_RSHIFT      DOKUN_KEY_SHIFT_R // Shift_R                                           
    #define DOKUN_KEY_RIGHT_SHIFT DOKUN_KEY_SHIFT_R // Shift_R    
    #define DOKUN_KEY_RETURN      0xff0d    
    #define DOKUN_KEY_ENTER       DOKUN_KEY_RETURN
// ------------------------------
    #define DOKUN_KEY_PRINT_SCREEN 0X0000 // Print Screen
    #define DOKUN_KEY_SCROLL_LOCK  0xff14 // Scroll Lock
    #define DOKUN_KEY_PAUSE        0xff13 // Pause-Break
// ------------------------------
    #define DOKUN_KEY_INSERT 0xff63 // Insert
    #define DOKUN_KEY_DELETE 0xffff // Delete
    #define DOKUN_KEY_HOME   0xff50 // Home <<
    #define DOKUN_KEY_END    0xff57 // End >>
    #define DOKUN_KEY_PRIOR  0xff55 // Page Up ^
    #define DOKUN_KEY_NEXT   0xff56 // Page Down
// ------------------------------
    // Arrow keys
    #define DOKUN_KEY_LEFT                          0x25 
    #define DOKUN_KEY_UP                            0x26
    #define DOKUN_KEY_RIGHT                         0x27
    #define DOKUN_KEY_DOWN                          0x28
// -------------------------------
    // Numpad
    #define DOKUN_KEY_NUM_LOCK    0xff7f
    //
    #define DOKUN_KEY_KP_DIVIDE   0xffaf
    #define DOKUN_KEY_KP_MULTIPLY 0xffaa
    #define DOKUN_KEY_KP_SUBTRACT 0xffad 
    #define DOKUN_KEY_KP_ADD      0xffab
    #define DOKUN_KEY_KP_ENTER    0xff8d
    #define DOKUN_KEY_KP_DECIMAL  0xffae
    #define DOKUN_KEY_KP_DELETE   0xff9f 
    //
    #define DOKUN_KEY_KP_INSERT   0xff9e
    #define DOKUN_KEY_KP_END      0xff9c
    #define DOKUN_KEY_KP_DOWN     0xff99 
    #define DOKUN_KEY_KP_NEXT     0xff9b 
    #define DOKUN_KEY_KP_LEFT     0xff96 
    #define DOKUN_KEY_KP_BEGIN    0xff9d 
    #define DOKUN_KEY_KP_RIGHT    0xff98 
    #define DOKUN_KEY_KP_HOME     0xff95 
    #define DOKUN_KEY_KP_UP       0xff97 
    #define DOKUN_KEY_KP_PRIOR    0xff9a
    // 0-9
    #define DOKUN_KEY_KP_0 0xffb0
    #define DOKUN_KEY_KP_1 0xffb1 
    #define DOKUN_KEY_KP_2 0xffb2
    #define DOKUN_KEY_KP_3 0xffb3 
    #define DOKUN_KEY_KP_4 0xffb4 
    #define DOKUN_KEY_KP_5 0xffb5 
    #define DOKUN_KEY_KP_6 0xffb6 
    #define DOKUN_KEY_KP_7 0xffb7 
    #define DOKUN_KEY_KP_8 0xffb8 
    #define DOKUN_KEY_KP_9 0xffb9 
#endif
//---------------------------------
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
//---------------------------------
#ifdef __gnu_linux__//DOKUN_X11
    #define DOKUN_KEY_ESCAPE   0xff1b // ESC
    // f1-f12
    #define DOKUN_KEY_F1  0xffbe
    #define DOKUN_KEY_F2  0xffbf
    #define DOKUN_KEY_F3  0xffc0
    #define DOKUN_KEY_F4  0xffc1
    #define DOKUN_KEY_F5  0xffc2
    #define DOKUN_KEY_F6  0xffc3
    #define DOKUN_KEY_F7  0xffc4
    #define DOKUN_KEY_F8  0xffc5
    #define DOKUN_KEY_F9  0xffc6
    #define DOKUN_KEY_F10 0xffc7
    #define DOKUN_KEY_F11 0xffc8
    #define DOKUN_KEY_F12 0xffc9
//  -----------------------------
    #define DOKUN_KEY_GRAVE      0x0060 // `
    #define DOKUN_KEY_ASCIITILDE 0x007e // ~ 
    // 0-9
    #define DOKUN_KEY_0 0x0030
    #define DOKUN_KEY_1 0x0031
    #define DOKUN_KEY_2 0x0032
    #define DOKUN_KEY_3 0x0033
    #define DOKUN_KEY_4 0x0034
    #define DOKUN_KEY_5 0x0035
    #define DOKUN_KEY_6 0x0036
    #define DOKUN_KEY_7 0x0037
    #define DOKUN_KEY_8 0x0038
    #define DOKUN_KEY_9 0x0039
    // Symbols
    #define DOKUN_KEY_EXCLAMATION 0x21 // !
    #define DOKUN_KEY_AT          0x40 // @
    #define DOKUN_KEY_HASH        0x23 // #
    #define DOKUN_KEY_NUMBERSIGN  0x23 // #
    #define DOKUN_KEY_DOLLAR      0x24 // $
    #define DOKUN_KEY_DOLLARSIGN  0x24 // $
    #define DOKUN_KEY_PERCENT     0x25 // %
    #define DOKUN_KEY_ASCIICIRCUM 0x5e // ^
    #define DOKUN_KEY_AMPERSAND   0x26 // &
    #define DOKUN_KEY_ASTERISK    0x2a // *
    #define DOKUN_KEY_PARENLEFT   0x28 // (
    #define DOKUN_KEY_PARENRIGHT  0x29 // )
    #define DOKUN_KEY_MINUS       0x2d // -
    #define DOKUN_KEY_UNDERSCORE  0x5f // _
    #define DOKUN_KEY_EQUAL       0x3d // =
    #define DOKUN_KEY_PLUS        0x2b // +
    // Special keys
    #define DOKUN_KEY_BACKSPACE   0xff08 // Backspace  
    #define DOKUN_KEY_TAB         0xff09 // Tab <- ->                 
    #define DOKUN_KEY_CAPS_LOCK   0xffe5 // Caps Lock
    #define DOKUN_KEY_SHIFT_L     0xffe1 // Shift_L  
    #define DOKUN_KEY_LSHIFT      DOKUN_KEY_SHIFT_L // Shift_L                                           
    #define DOKUN_KEY_LEFT_SHIFT  DOKUN_KEY_SHIFT_L // Shift_L   
    #define DOKUN_KEY_CTRL_L      0xffe3 // CTRL_L                     
    #define DOKUN_KEY_LEFT_CTRL   DOKUN_KEY_CTRL_L // CTRL_L         
    #define DOKUN_KEY_LCTRL       DOKUN_KEY_CTRL_L // CTRL_L 
    #define DOKUN_KEY_CONTROL_L   DOKUN_KEY_CTRL_L // CTRL_L 
    //#define DOKUN_KEY_HOME        0x0000 // Windows Button (not correct)         
    #define DOKUN_KEY_ALT_L       0xffe9 // ALT_L
    #define DOKUN_KEY_LEFT_ALT    DOKUN_KEY_ALT_L // ALT_L         
    #define DOKUN_KEY_LALT        DOKUN_KEY_ALT_L // ALT_L  
    // Spacebar
    #define DOKUN_KEY_SPACE       0x0020
    // A-Z
    #define DOKUN_KEY_A 0x0061
    #define DOKUN_KEY_B 0x0062
    #define DOKUN_KEY_C 0x0063
    #define DOKUN_KEY_D 0x0064
    #define DOKUN_KEY_E 0x0065
    #define DOKUN_KEY_F 0x0066
    #define DOKUN_KEY_G 0x0067
    #define DOKUN_KEY_H 0x0068
    #define DOKUN_KEY_I 0x0069
    #define DOKUN_KEY_J 0x006a
    #define DOKUN_KEY_K 0x006b
    #define DOKUN_KEY_L 0x006c
    #define DOKUN_KEY_M 0x006d
    #define DOKUN_KEY_N 0x006e
    #define DOKUN_KEY_O 0x006f
    #define DOKUN_KEY_P 0x0070
    #define DOKUN_KEY_Q 0x0071
    #define DOKUN_KEY_R 0x0072
    #define DOKUN_KEY_S 0x0073
    #define DOKUN_KEY_T 0x0074
    #define DOKUN_KEY_U 0x0075
    #define DOKUN_KEY_V 0x0076
    #define DOKUN_KEY_W 0x0077
    #define DOKUN_KEY_X 0x0078
    #define DOKUN_KEY_Y 0x0079
    #define DOKUN_KEY_Z 0x007a
    // Symbols
    #define DOKUN_KEY_BRACKETLEFT  0x5b // [
    #define DOKUN_KEY_BRACKETRIGHT 0x5d // ]
    #define DOKUN_KEY_BRACELEFT    0x7b // {
    #define DOKUN_KEY_BRACERIGHT   0x7d // }
    #define DOKUN_KEY_BACKSLASH    0x5c /* \ */
    #define DOKUN_KEY_BAR          0x7c // |
    #define DOKUN_KEY_SEMICOLON    0x3b // ;
    #define DOKUN_KEY_COLON        0x3a // :
    #define DOKUN_KEY_APOSTROPHE   0x27 // '
    #define DOKUN_KEY_QUOTEDBL     0x22 // "
    #define DOKUN_KEY_QUOTE        DOKUN_KEY_QUOTEDBL
    #define DOKUN_KEY_COMMA        0x2c // ,
    #define DOKUN_KEY_PERIOD       0x2e // .
    #define DOKUN_KEY_LESS         0x3c // <
    #define DOKUN_KEY_GREATER      0x3e // >
    #define DOKUN_KEY_LESSTHAN     DOKUN_KEY_LESS
    #define DOKUN_KEY_GREATERTHAN  DOKUN_KEY_GREATER
    #define DOKUN_KEY_SLASH        0x2f // /
    #define DOKUN_KEY_QUESTION     0x3f // ?
    // Special keys 
    #define DOKUN_KEY_ALT_R       0xffea // ALT_R
    #define DOKUN_KEY_RIGHT_ALT   DOKUN_KEY_ALT_R // ALT_R        
    #define DOKUN_KEY_RALT        DOKUN_KEY_ALT_R // ALT_R
    #define DOKUN_KEY_MENU        0xff67 // [_--] 
    #define DOKUN_KEY_CTRL_R      0xffe4 // CTRL_R                    
    #define DOKUN_KEY_RIGHT_CTRL  DOKUN_KEY_CTRL_R // CTRL_R         
    #define DOKUN_KEY_RCTRL       DOKUN_KEY_CTRL_R // CTRL_R 
    #define DOKUN_KEY_CONTROL_R   DOKUN_KEY_CTRL_R // CTRL_R 
    #define DOKUN_KEY_SHIFT_R     0xffe2 // Shift_R
    #define DOKUN_KEY_RSHIFT      DOKUN_KEY_SHIFT_R // Shift_R                                           
    #define DOKUN_KEY_RIGHT_SHIFT DOKUN_KEY_SHIFT_R // Shift_R    
    #define DOKUN_KEY_RETURN      0xff0d    
    #define DOKUN_KEY_ENTER       DOKUN_KEY_RETURN
// ------------------------------
    #define DOKUN_KEY_PRINT_SCREEN 0X0000 // Print Screen
    #define DOKUN_KEY_SCROLL_LOCK  0xff14 // Scroll Lock
    #define DOKUN_KEY_PAUSE        0xff13 // Pause-Break
// ------------------------------
    #define DOKUN_KEY_INSERT 0xff63 // Insert
    #define DOKUN_KEY_DELETE 0xffff // Delete
    #define DOKUN_KEY_HOME   0xff50 // Home <<
    #define DOKUN_KEY_END    0xff57 // End >>
    #define DOKUN_KEY_PRIOR  0xff55 // Page Up ^
    #define DOKUN_KEY_NEXT   0xff56 // Page Down
// ------------------------------
    // Arrow keys
    #define DOKUN_KEY_LEFT                          0xff51  // Move left, left arrow 
    #define DOKUN_KEY_UP                            0xff52  // Move up, up arrow 
    #define DOKUN_KEY_RIGHT                         0xff53  // Move right, right arrow 
    #define DOKUN_KEY_DOWN                          0xff54  // Move down, down arrow
// -------------------------------
    // Numpad
    #define DOKUN_KEY_NUM_LOCK    0xff7f
    //
    #define DOKUN_KEY_KP_DIVIDE   0xffaf
    #define DOKUN_KEY_KP_MULTIPLY 0xffaa
    #define DOKUN_KEY_KP_SUBTRACT 0xffad 
    #define DOKUN_KEY_KP_ADD      0xffab
    #define DOKUN_KEY_KP_ENTER    0xff8d
    #define DOKUN_KEY_KP_DECIMAL  0xffae
    #define DOKUN_KEY_KP_DELETE   0xff9f 
    //
    #define DOKUN_KEY_KP_INSERT   0xff9e
    #define DOKUN_KEY_KP_END      0xff9c
    #define DOKUN_KEY_KP_DOWN     0xff99 
    #define DOKUN_KEY_KP_NEXT     0xff9b 
    #define DOKUN_KEY_KP_LEFT     0xff96 
    #define DOKUN_KEY_KP_BEGIN    0xff9d 
    #define DOKUN_KEY_KP_RIGHT    0xff98 
    #define DOKUN_KEY_KP_HOME     0xff95 
    #define DOKUN_KEY_KP_UP       0xff97 
    #define DOKUN_KEY_KP_PRIOR    0xff9a
    // 0-9
    #define DOKUN_KEY_KP_0 0xffb0
    #define DOKUN_KEY_KP_1 0xffb1 
    #define DOKUN_KEY_KP_2 0xffb2
    #define DOKUN_KEY_KP_3 0xffb3 
    #define DOKUN_KEY_KP_4 0xffb4 
    #define DOKUN_KEY_KP_5 0xffb5 
    #define DOKUN_KEY_KP_6 0xffb6 
    #define DOKUN_KEY_KP_7 0xffb7 
    #define DOKUN_KEY_KP_8 0xffb8 
    #define DOKUN_KEY_KP_9 0xffb9
#endif


#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

namespace dokun {
class Keyboard {
public:
    Keyboard();
    ~Keyboard();
    static bool is_pressed(int key, int times_pressed = 1); static int is_pressed(lua_State *L);
    static bool is_released(int key);                       static int is_released(lua_State *L);
    static bool is_down();                                  static int is_down(lua_State *L);
    static bool is_up();                                    static int is_up(lua_State *L);
#ifdef DOKUN_X11    
    static void fake_event(Display* display, const ::Window& window);
#endif
	friend class Window; // Window can now access Keyboard's private members //not XWindow but dokun::Window
	static bool up;
	static bool down;
	static int key;
private:
    static void get_input(int key, bool pressed, bool released);
};
}
#endif
#endif
