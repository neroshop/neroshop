#ifndef MESSAGE_NEROSHOP
#define MESSAGE_NEROSHOP

#include "box.hpp"
#include "button.hpp"
#include "edit.hpp"
#include "timer.hpp"

namespace neroshop {
class Message {
public:
    Message();
    Message(const std::string& text);
    Message(const std::string& text, int red, int green, int blue, double alpha = 1.0);
    Message(const std::string& text, std::string color);
    ~Message();
    static void init();
    static void show();
    static void hide();
    static void draw();
    static void center(unsigned int window_width, unsigned int window_height);
    static void restore(); // restores original text color, clears text, etc.
    static void add_button(const std::string& text);
    static void add_edit();
    // setters
    static void set_text(const std::string& text);
    static void set_text(const std::string& text, int red, int green, int blue, double alpha = 1.0);
    static void set_text(const std::string& text, std::string color);
    static void set_title(const std::string& title);
    // getters
    static Box * get_box();
    static Button * get_button(const std::string& text);
    static Edit * get_edit();
    // boolean
    static bool is_visible();
private:
    static Box * box;
    static Button * ok_button;
    static Button * cancel_button;
    static Edit * edit;
    int button_press_count; // for ok button
    static void on_draw();
};
}
#endif
