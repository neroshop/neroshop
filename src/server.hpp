#pragma once

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// neroshop
#include "debug.hpp"

#define NEROMON_TAG std::string("\033[1;95m[neromon]: \033[0m") +

namespace neroshop {

class Server {
  public:
    Server();
    ~Server();

    bool bind( unsigned int port );
    bool listen();
    bool accept();
    void write( const std::string& text );
    std::string read();
    void close();
    void shutdown();

    #ifdef __linux__
    int get_socket() const { return socket; }
    int get_client_socket(int index = 0) const { return newsocket; }
    #endif

  private:
    #ifdef _WIN32
    SOCKET socket, newsocket;
    char buffer[512];
    struct sockaddr_in server_addr, client_addr;
    socklen_t clilen;
    #endif
    #ifdef __linux__
    int socket, newsocket;
    char buffer[512];
    std::string buffer_new;
    struct sockaddr_in server_addr, client_addr;
    socklen_t clilen;
    #endif
    unsigned int port;
};

} // neroshop::
