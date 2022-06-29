#include <iostream>

#include "client.hpp"

using namespace neroshop;

// *****************************************************************************
int main()
{
  // Connect client to server
  Client * client = Client::get_main_client();
  int client_port = 1234;
  std::string client_ip = "localhost";
  std::cout << "Client connecting to server ..." << std::endl;
  client->connect( client_port, client_ip );
  std::cout << "Client attempting to read from server" << std::endl;
  std::cout << "Message client read: " << client->read() << std::endl;
  return EXIT_SUCCESS;
}
