#ifndef CLIENT_HPP_NEROSHOP
#define CLIENT_HPP_NEROSHOP

#include <iostream>
#include <cstring>
#include <thread>
#include <mutex>

#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "debug.hpp"

namespace neroshop {

class Client {
public:
    Client();
	Client(std::string address, unsigned int port);
	~Client();
	void create(); // creates a new socket
	bool connect(unsigned int port, std::string address = "0.0.0.0");
	void write(const std::string& text);
	std::string read();
	void close(); // kills socket
	void shutdown(); // shuts down connection (disconnects from server)
    // special functions
    void disconnect(); // breaks connection to server then closes the client socket // combination of shutdown() and close()
    bool reconnect(unsigned int port, std::string address = "0.0.0.0"); // closes socket then connects again
#ifdef _WIN32
	SOCKET get_socket() const;
#endif
#ifdef __gnu_linux__
	int get_socket() const;
#endif
    static Client * get_main_client();
    bool is_connected();// const; // doesn't work :/
private:
#ifdef _WIN32
	SOCKET socket;
	char buffer[512];	
	struct sockaddr_in socket_addr;	
    struct hostent * host;
#endif
#ifdef __gnu_linux__
	int socket;
	char buffer[256]; //std::string buffer_new;
	// struct sockaddr // universal endpoint type
	struct sockaddr_in socket_addr;	// IPv4 
	// struct sockaddr_in6 // IPv6
	// sockaddr_un // ??
	// 4 types of sockets in POSIX API: TCP, UDP, UNIX, and (optionally) RAW
    struct hostent * host;
#endif
    static Client * client_obj; // global/singleton client object that can be access by any neroshop class or function
    bool test_socket();// const; // temporary
};
}
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
