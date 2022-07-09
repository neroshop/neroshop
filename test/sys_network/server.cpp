#include <iostream>
#include <signal.h>

#include "server.hpp"

using namespace neroshop;

Server * server = new Server();

void close_server() {
    server->shutdown();
    delete server;
    server = nullptr;
}

void heartbeat() {
  if (server->accept()) {
     server->write( "server accepted client connection" );
  }
  std::cout << server->read() << std::flush << std::endl;
}

int main( int argc, char** argv )
{
  auto f = fopen( "network_test.pid", "r" );
  if (f) {
    int pid;
    fscanf( f, "%d", &pid );
    fclose( f );
    std::cout << "Killing PID: " << pid << std::endl;
    auto ret = kill( pid, SIGKILL );
    if (ret == -1) {
      perror("kill");
      return EXIT_FAILURE;
    } else {
      remove( "network_test.pid" );
    }
    return EXIT_SUCCESS;
  }

  // Fork the current process. The parent process continues with a process ID
  // greater than 0.  A process ID lower than 0 indicates a failure in either
  // process.
  pid_t pid = fork();
  if (pid > 0) {
    std::cout << "Forked pid: " << pid << std::endl;
    auto f = fopen( "network_test.pid", "w" );
    fprintf( f, "%d", pid );
    fclose( f );
    return EXIT_SUCCESS;
  } else if (pid < 0) {
    return EXIT_FAILURE;
  }

  // Generate a session ID for the child process and ensure it is valid.
  if(setsid() < 0) {
    // Log failure and exit
    std::cerr << "Could not generate session ID for child process" << std::endl;
    // If a new session ID could not be generated, we must terminate the child
    // process or it will be orphaned.
    return EXIT_FAILURE;
  }

  // A daemon cannot use the terminal, so close standard file descriptors for
  // security reasons
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // Start server
  std::atexit( close_server );
  int server_port = 55080;
  std::cout << "Starting server on port " << server_port << std::endl;
  if (server->bind(server_port)) {
    std::cout << "Server bound to port " << std::to_string(server_port)
              << std::endl;
  }
  server->listen();

  // Enter daemon loop
  while(true) {
    // Execute daemon heartbeat
    heartbeat();
    // Sleep for a period of time
    sleep(1);
  }

  return EXIT_SUCCESS;
}
