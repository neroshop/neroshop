#ifndef ICON_HPP_NEROSHOP
#define ICON_HPP_NEROSHOP

#include "image.hpp"

//typedef std::tuple<unsigned int, unsigned int, unsigned int> dimensions; // width height channel

class Icon {
public:
    Icon();
    ~Icon();
    static bool load_all(); // loads all images
private:
    static bool load_monero();
    static bool load_settings(); // cog//settings
    static bool load_cart(); // cart
    static bool load_search(); // search
    static bool load_user(); // user
    static bool load_heart(); // heart (favorite)
    static bool load_star();
    static bool load_circle(); // circle, circle_outline
    static bool load_speaker(); // volume_up, volume_down
    static bool load_bookmark();
    static bool load_trash();
    static bool load_order();
    static bool load_flag(); // flag, flag_outline (report)
    static bool load_numbers();
    static bool load_upload();
    static bool load_login();
    static bool load_console();
    static bool load_wallet();
    //static bool load_();
public:    
    static std::map<std::string, Image *> get; // a map is basically a std::vector of std::pairs
    //static std::vector< std::pair<std::string, Image *> > get; //Icon * icon_ptr; // singleton obj
}; 
#endif
// name, data, width, height, channel, depth
//    Image * image_ = new Image(const_cast<char *>(), 64, 64, 1, 4);
//    Icon::get.insert(std::make_pair("", image_));


// Icon::get["cog"]->get_data();
// Icon::get_icon()[]
// std::cout <<  << icon_list["cog"] << std::endl;
