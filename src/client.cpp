#include "client.hpp"

using namespace neroshop;

std::unique_ptr< Client > Client::client_obj(nullptr);

// *****************************************************************************
Client * Client::get_main_client() {
  client_obj = std::make_unique< Client >();
  return client_obj.get();
}

// *****************************************************************************
Client::Client() : socket(0) {}

// *****************************************************************************
Client::~Client() {
  close();
}

// *****************************************************************************
void Client::create() {
  #ifdef __windows__
  socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket == SOCKET_ERROR) {
     std::cerr << "Could not create socket " << WSAGetLastError() << std::endl;
     WSACleanup();
   }
  #endif

  #ifdef __linux__
  if (socket) return;
  socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket < 0) std::cerr << "Could not create socket " << std::endl;
  #endif
}

// *****************************************************************************
bool Client::connect(unsigned int port, std::string address) {
  create();

  #ifdef __windows__
  host = gethostbyname(address.c_str());
  socket_addr.sin_port = htons(port);
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
  // connect to a server
  return ::connect(socket, (struct sockaddr *)(&socket_addr), sizeof(socket_addr)) == 0;
  #endif

  #ifdef __linux__
  host = gethostbyname( address.c_str() );
  if (host == nullptr) {
    std::cerr << "No host to connect to" << std::endl;
    return false;
  }
  bzero((char *) &socket_addr, sizeof(socket_addr));
  socket_addr.sin_family = AF_INET;
  bcopy((char *)host->h_addr, (char *)&socket_addr.sin_addr.s_addr, host->h_length);
  socket_addr.sin_port = htons(port);
  // connect to server	
  if (::connect(socket, (struct sockaddr *)(&socket_addr), sizeof(socket_addr)) < 0) {
    close();
    return false;
  }
  return true;
  #endif

  return false;
}

// *****************************************************************************
bool Client::write(const std::string& text)
{
  #ifdef __windows__
  send(socket, text.c_str(), text.length(), 0);	
  #endif	
  #ifdef __linux__
  ssize_t write_result = ::write(socket, text.c_str(), text.length());
  if (write_result < 0) return false; else return true;
  #endif
}

// *****************************************************************************
std::string Client::read() {
  // Display message from server
  #ifdef __windows__
  memset(buffer, 0, 511);
  if (recv(socket, buffer, 512, 0) < 0)
    std::cerr << "Could not read from server" << std::endl;
  return static_cast<std::string>(buffer);
  #endif

  #ifdef __linux__
  memset(buffer, 0, 256);
  ssize_t read_result = ::read(socket, buffer, 255);
  if (read_result < 0)
    std::cerr << "Could not read from server" << std::endl;	

  return static_cast<std::string>(buffer);
  #endif
}

// *****************************************************************************
void Client::close()
{
  #ifdef __windows__
  closesocket(socket);	
  #endif

  #ifdef __linux__
  if(socket == 0) return;
  ::close(socket);
  socket = 0;
  #endif
}

// *****************************************************************************
void Client::shutdown()
{
  #ifdef __windows__
  ::shutdown(socket, SD_BOTH); // SD_RECEIVE, SD_SEND, SD_BOTH
  #endif

  #ifdef __linux__
  ::shutdown(socket, SHUT_RDWR); // SHUT_RD, SHUT_WR, SHUT_RDWR
  #endif
}

// *****************************************************************************
void Client::disconnect() {
  write("disconnect");
  shutdown();
  close();
}

// *****************************************************************************
bool Client::reconnect(unsigned int port, std::string address) {
  // kill socket first before attempting to re-connect
  close();
  return connect( port, address );
}

// *****************************************************************************
bool Client::connected() const {
  if (not socket) return false;
  #ifdef __linux__
  const std::string& text = "";
  ssize_t send_result = ::send(socket, text.c_str(), text.length(), MSG_NOSIGNAL);
  if (send_result < 0) return false; else return true;
  #endif
}
