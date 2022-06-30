#include "server.hpp"

using namespace neroshop;

// *****************************************************************************
Server::Server()
{
  #ifdef __windows__
  socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket == INVALID_SOCKET) {
    std::cerr << "Could not create socket " << WSAGetLastError() << std::endl;
    WSACleanup();
  }
  #endif
  #ifdef __linux__
  socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket < 0) {
    std::cerr << "Could not create socket" << std::endl;
    return;
  }
  // set socket options : SO_REUSEADDR (TCP:restart a closed/killed process on
  // the same address so you can reuse address over and over again)
  int one = 1; // enable
  if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &one, sizeof(one)) < 0)
  {
      std::cerr << "Could not set socket options" << std::endl;
  }
  #endif
}

// *****************************************************************************
Server::~Server()
{
  close();
}

// *****************************************************************************
bool
Server::bind(unsigned int port)
{
  #ifdef __windows__
  memset(&server_addr, 0, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons( port );        // Port to listen
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (::bind(this->socket, (struct sockaddr*) &server_addr,
             sizeof(server_addr)) < 0)
  {
    std::cerr << "Cannot bind socket" << std::endl; 
    close();
    shutdown();
    return false;
  }
  #endif
  
  #ifdef __linux__
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_port = htons(port);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (::bind(socket, (struct sockaddr *) &server_addr,
             sizeof(server_addr)) < 0)
  {
    std::cerr << "Cannot bind socket" << std::endl;
    close();
    return false;
  }
  #endif

  this->port = port;
  return true;
}

// *****************************************************************************
bool Server::listen()
{
  #ifdef __windows__
  // Listen for a connection,number of connections allowed on the incoming queue
  if (::listen(socket, 2) < 0) {
    std::cerr << "Cannot listen for a connection" << std::endl;
    shutdown();
    close();
    return false;
  }
  #endif
  #ifdef __linux__
  // SOMAXCONN=4096 // number of requests to listen to at a time
  if (::listen(socket, SOMAXCONN) < 0) {
    std::cerr << "Cannot listen for a connection" << std::endl;
    shutdown();
    close();			
    return false;
  }
  #endif

  return true;
}

// *****************************************************************************
bool Server::accept()
{
  #ifdef __windows__
  clilen = sizeof(client_addr);
  newsocket = ::accept(this->socket, (struct sockaddr *) &client_addr, &clilen);
  if (newsocket < 0) {		 
    shutdown();
    close();
    return false;
  }

  if (newsocket > 0) {
    std::cout << newsocket << "\n";
  }

  std::cout << "server: got connection from " << inet_ntoa(client_addr.sin_addr)
            << " port " << ntohs(client_addr.sin_port) << std::endl;
  #endif

  #ifdef __linux__
  clilen = sizeof(client_addr);
  newsocket = ::accept(socket, (struct sockaddr *) &client_addr, &clilen);
  if (newsocket < 0) {
    std::cerr << ("ERROR on accept\n");
    shutdown();
    close();
    return false;
  }

  std::cout << NEROSHOP_TAG "\033[0;37mReceived connection from "
            << inet_ntoa(client_addr.sin_addr) << ":\033[0;36m"
            << ntohs(client_addr.sin_port) << "\033[0m" << std::endl;
  #endif

  return true;
}

// *****************************************************************************
void Server::write(const std::string& text)
{
  #ifdef __windows__
  send(newsocket, text.c_str(), text.length(), 0);	
  #endif

  #ifdef __linux__
  //bzero(buffer,256);
  //fgets(buffer,255,stdin);
  ssize_t write_result = ::write( newsocket, text.c_str(), text.length() );
  if (write_result < 0) {
    std::cout << "Server unable to write to client" << std::endl;
  }
  #endif
}

// *****************************************************************************
std::string Server::read()
{
  #ifdef __windows__
  if (recv(newsocket, buffer, 512, 0) < 0) {	
    std::cerr << "Client disconnected" << std::endl;
    shutdown();
    close();
  }
  #endif

  #ifdef __linux__
  memset(buffer, 0, 256);
  ssize_t read_result = ::read(newsocket, buffer, 255);
  if (read_result < 0) {
    perror("socket read error: ");//std::cerr << "Client disconnected" << std::endl;
    shutdown();
    close();
  } else if(read_result > 0 && read_result < 511) {
    //std::cout << "Received less bytes than expected" << std::endl;
  }
  //::write (STDOUT_FILENO, buffer, read_result);
  #endif

  return static_cast< std::string >( buffer );
}

// *****************************************************************************
void Server::close()
{
  #ifdef __windows__
  closesocket(socket);
  closesocket(newsocket);
  #endif

  #ifdef __linux__
  ::close(socket);
  ::close(newsocket);
  #endif
}

// *****************************************************************************
void Server::shutdown()
{
  #ifdef __windows__
  ::shutdown(socket, SD_BOTH); // SD_RECEIVE, SD_SEND, SD_BOTH
  ::shutdown(newsocket, SD_BOTH);
  #endif

  #ifdef __linux__
  ::shutdown(socket, SHUT_RDWR); // SHUT_RD, SHUT_WR, SHUT_RDWR
  ::shutdown(newsocket, SHUT_RDWR);
  #endif
}
