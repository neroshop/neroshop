#pragma once

#include <iostream>

#define NEROSHOP_TAG_OUT neroshop::io_write("");
#define NEROSHOP_TAG_IN std::string("\033[1;35;49m[neroshop]: \033[0m") +
#define NEROSHOP_TAG NEROSHOP_TAG_IN

// path
#ifdef _WIN32
#define NEROSHOP_PATH ""
#endif
#ifdef __linux__
#define NEROSHOP_PATH "/home/" + System::get_user() + "/.config/neroshop"
#endif

namespace neroshop {

inline void print( const std::string& text, int code = 0 ) {
  if (code == 0)
    std::cout << "\033[1;35;49m" << "[neroshop]: " << "\033[1;37;49m"
              << text << "\033[0m" << std::endl;
  else if (code == 1)
    std::cout << "\033[1;35;49m" << "[neroshop]: " << "\033[1;91;49m"
              << text << "\033[0m" << std::endl;
  else if (code == 2)
    std::cout << "\033[1;35;49m" << "[neroshop]: " << "\033[1;33;49m"
              << text << "\033[0m" << std::endl;
  else if (code == 3)
    std::cout << "\033[1;35;49m" << "[neroshop]: " << "\033[1;32;49m"
              << text << "\033[0m" << std::endl;
  else if (code == 4)
    std::cout << "\033[1;35;49m" << "[neroshop]: " << "\033[1;34;49m"
              << text << "\033[0m" << std::endl;
}

inline void io_write( const std::string& text ) {
  std::cout << "\033[1;35;49m" << "[neroshop]: " << "\033[1;37;49m"
            << text << "\033[0m";
}

} // ::neroshop
