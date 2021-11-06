#ifndef _CLIENT
#define _CLIENT

#include "platform.hpp"

#ifdef __cplusplus
#include <iostream>
#include <cstring>
#include <thread>
#include <mutex>
#include <lua.hpp>

class Client {
public:
    Client();
	Client(std::string address, unsigned int port);
	~Client();
	
	bool connect(unsigned int port, std::string address = "localhost"); static int connect(lua_State *L);
	void write(const std::string& text);                                static int write(lua_State *L);// send
	std::string read();                                                 static int read(lua_State *L);//  receive
	void close();                                                       static int close(lua_State *L);
	void shutdown();                                                    static int shutdown(lua_State *L);
private:
    #ifdef _WIN32
	    SOCKET socket;
		char buffer[512];	
		struct sockaddr_in socket_addr;	
        struct hostent * host;
	#endif
	#ifdef __gnu_linux__
	    int socket;
		char buffer[256];	
		struct sockaddr_in socket_addr;	
        struct hostent * host;
	#endif
};
#endif
#endif
/*
void server()
{
    // server
	Server * server = new Server();
	server->bind(1234);
	server->listen();
	server->accept();
	server->write("Welcome to Server0"); // write to client once
	while(1) // read from client (while program is running)
		std::cout << server->read() << std::endl; 
}
void client()
{
	// client
    Client * client = new Client();
	client->connect(1234, "localhost");
	std::cout << client->read() << std::endl; // read from server once
	while (1)  // write to server (while program is running)
	{
		std::string text;
		std::cin >> text; 
		client->write(text);
	}
}
*/
