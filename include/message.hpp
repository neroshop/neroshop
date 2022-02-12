#ifndef MESSAGE_NEROSHOP_
#define MESSAGE_NEROSHOP_

#include "box.hpp"
#include "button.hpp"
#include "edit.hpp"
#include "timer.hpp"
#include <memory> // std::make_unique

namespace neroshop {
class Message {
public:
    Message();
    Message(const std::string& text);
    Message(const std::string& text, int red, int green, int blue, double alpha = 1.0);
    Message(const std::string& text, std::string color);
    ~Message();
    void initialize();
    void show();
    void hide();
    void draw();
    void center(unsigned int window_width, unsigned int window_height);
    void restore(); // restores original text color, clears text, etc.
    void add_button(const std::string& text, int relative_x = 0, int relative_y = 0, int width = 100, int height = 50);//void add_button(int relative_x = 0, int relative_y = 0, std::string text = "");
    void add_edit(int relative_x = 0, int relative_y = 0, int width = 300, int height = 30);
    // setters
    void set_text(const std::string& text);
    void set_text(const std::string& text, int red, int green, int blue, double alpha = 1.0);
    void set_text(const std::string& text, std::string color);
    void set_title(const std::string& title);
    // getters
    static Message * get_first(); // returns the first and original message box
    //Box * box() const;
    Box * get_box() const;
    std::shared_ptr<Button>/*Button **/ get_button(int index) const;//Button * get_button_by_text(const std::string& text);
    std::shared_ptr<Edit>/*Edit **/ get_edit(int index) const;
    int get_button_count() const;
    int get_edit_count() const;
    // boolean
    bool is_visible();
private: // I still don't understand smart pointers :( // https://codereview.stackexchange.com/questions/160053/c-erasing-an-object-from-vector-of-pointers/160058#160058
    static Message * first;
    Box * box; // unique_ptr cannot be copied so I switched to smart pointers
    std::vector<std::shared_ptr<Button>> button_list;//std::vector<Button*> button_list;   
    std::vector<std::shared_ptr<Edit>> edit_list;//std::vector<Edit*> edit_list;
    void on_draw();
    void destroy_children(); // No longer a need for this now that I'm using shared pointers
    void draw_children();
    void hide_children();
    void show_children();
};
}
#endif
