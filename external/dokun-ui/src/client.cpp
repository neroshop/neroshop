#include "../include/client.hpp"

Client::Client()
{
#ifdef __windows__
	socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_UNSPEC, AF_BTH (Bluetooth), AF_INET6 (IPV6), IPPROTO_UDP (UDP)
	if (socket == SOCKET_ERROR)
    {
        std::cerr << "Could not create socket " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
#endif	
#ifdef __gnu_linux__
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(socket < 0)
	{
		// error
		std::cerr << "Could not create socket " << std::endl;
	}
#endif
}
//////////
Client::~Client()
{
	close();
}
//////////
bool Client::connect(unsigned int port, std::string address)
{
#ifdef __windows__
	host = gethostbyname(address.c_str());
	socket_addr.sin_port = htons(port);
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
		
    return (::connect(socket, (struct sockaddr *)(&socket_addr), sizeof(socket_addr)) == 0);
#endif
#ifdef __gnu_linux__
	host = gethostbyname(address.c_str());
	if(host == nullptr)
	{
		// error: no such host
		std::cerr << "No host to connect to" << std::endl;
	}
	bzero((char *) &socket_addr, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    bcopy((char *)host->h_addr,
    (char *)&socket_addr.sin_addr.s_addr, host->h_length);
    socket_addr.sin_port = htons(port);
		
	if (::connect(socket, (struct sockaddr *)(&socket_addr), sizeof(socket_addr)) < 0)
	{
		// error connecting to server
		std::cerr << "Could not connect to server" << std::endl;
		return false;
	}
	return true;
#endif
    return false;
}
//////////
void Client::write(const std::string& text)
{
#ifdef __windows__
    send(socket, text.c_str(), text.length(), 0);	
#endif	
#ifdef __gnu_linux__
	bzero(buffer, 256);
	if(::write(socket, text.c_str(), text.length()) < 0)
	{
		// error
		std::cerr << "Could not write to server" << std::endl;
	}
#endif
}
std::string Client::read()
{
	// Display message from server
#ifdef __windows__
	memset(buffer, 0, 511);
	if(recv(socket, buffer, 512, 0) < 0)
	{
		// error
		std::cerr << "Could not read from server" << std::endl;
	}
	return static_cast<std::string>(buffer);
#endif	
#ifdef __gnu_linux__
	bzero(buffer, 256);
	if(::read(socket, buffer, 255) < 0)
	{
		// error
		std::cerr << "Could not read from server" << std::endl;	
	}
	return static_cast<std::string>(buffer);
#endif
}
//////////	
void Client::close()
{
#ifdef __windows__
    closesocket(socket);	
#endif	
#ifdef __gnu_linux__
	::close(socket);
#endif
}
//////////
void Client::shutdown()
{
#ifdef __windows__
	::shutdown(socket, SD_BOTH); // SD_RECEIVE, SD_SEND, SD_BOTH
#endif	
#ifdef __gnu_linux__
	::shutdown(socket, SHUT_RDWR); // SHUT_RD, SHUT_WR, SHUT_RDWR
#endif
}
//////////
