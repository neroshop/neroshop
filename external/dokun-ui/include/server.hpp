#ifndef _SERVER
#define _SERVER

#include "platform.hpp"

#ifdef __cplusplus // if c++ // if c++
#include <iostream>
#include <thread>
#include <mutex>
#include <lua.hpp>

class Server { // listens to a port and accepts a socket
public:
    Server();
	~Server();
	
	bool bind(unsigned int port);        static int bind(lua_State *L);
	bool listen();                       static int listen(lua_State *L); // listen for connection
	bool accept();                       static int accept(lua_State *L); // accept a client (socket)
	bool accept_all();
	void write(const std::string& text); static int write(lua_State *L); // send data
	std::string read();                  static int read(lua_State *L); // receive data
	void close();                        static int close(lua_State *L); // closes a socket	
	void shutdown();                     static int shutdown(lua_State *L); // shuts down entire connection, ending receiving and sending
	
	void * get_client(int index);
private:
    #ifdef _WIN32
	    SOCKET socket, newsocket;
		char buffer[512];
		struct sockaddr_in server_addr, client_addr;
	    socklen_t clilen;
	#endif
	#ifdef __gnu_linux__
	    int socket, newsocket;
		char buffer[256];
		struct sockaddr_in server_addr, client_addr;
	    socklen_t clilen;
	#endif	
	unsigned int port;
};
#endif
#endif
