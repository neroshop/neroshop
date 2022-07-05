#pragma once

#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <memory>

#include "debug.hpp"

namespace neroshop {

class Client {
  public:
    Client();
    Client( std::string address, unsigned int port );
    ~Client();
    void create(); // creates a new socket
    bool connect( unsigned int port, std::string address );
    bool write( const std::string& text );
    std::string read();
    void close();
    void shutdown();
    void disconnect();
    bool reconnect( unsigned int port, std::string address );

    #ifdef _WIN32
    SOCKET get_socket() const
    #endif
    #ifdef __linux__
    int get_socket() const
    #endif
    { return socket; }

    static Client * get_main_client();
    bool connected() const;

  private:
    #ifdef _WIN32
    SOCKET socket;
    char buffer[512];	
    struct sockaddr_in socket_addr;	
    struct hostent * host;
    #endif

    #ifdef __linux__
    int socket;
    char buffer[256];
    struct sockaddr_in socket_addr;
    struct hostent * host;
    #endif

    static std::unique_ptr< Client > client_obj; // global singleton
};
}
