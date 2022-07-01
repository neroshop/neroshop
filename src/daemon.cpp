#include <dirent.h>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>
#include <iostream>

#include "server.hpp"
#include "debug.hpp"

#define NEROMON_TAG std::string("\033[1;95m[neromon]: \033[0m ") +

using namespace neroshop;

Server * server = new Server();

void close_server() {
    server->shutdown();
    delete server;
    server = nullptr;
    std::cout << NEROMON_TAG "\033[1;91mdisconnected\033[0m" << std::endl;
}

void interpret_query( std::string&& query ) {
  if (query.find("lmdb") != std::string::npos) {
    std::cout << "lmdb query" << '\n';
  }
}

void heartbeat() {
  if (server->accept()) server->write("accept");
  interpret_query( server->read() );
}

// neroshop daemon (neromon) main
int main(void) {

  // Fork the current process. The parent process continues with a process ID
  // greater than 0.  A process ID lower than 0 indicates a failure in either
  // process.
  pid_t pid = fork();
  if (pid > 0) {
    std::cout << "Forked pid: " << pid << std::endl;
    return EXIT_SUCCESS;
  } else if (pid < 0) {
    return EXIT_FAILURE;
  }

  // TODO: I think we should log to ~/.config/neroshop/

  // The parent process has now terminated, and the forked child process will
  // continue (the pid of the child process was 0).  Since the child process is
  // a daemon, the umask needs to be set so files and logs can be written.
  umask(0);

  // Open system logs for the child process
  openlog("neromon", LOG_NOWAIT | LOG_PID, LOG_USER);
  syslog(LOG_NOTICE, "Successfully started neromon");

  // Generate a session ID for the child process and ensure it is valid.
  if(setsid() < 0) {
    // Log failure and exit
    std::cout << "Could not generate session ID for child process" << std::endl;
    syslog(LOG_ERR, "Could not generate session ID for child process");
    // If a new session ID could not be generated, we must terminate the child
    // process or it will be orphaned.
    return EXIT_FAILURE;
  }

  // Change the current working directory to a directory guaranteed to exist
  if (chdir("/") < 0) {
    // Log failure and exit
    syslog(LOG_ERR, "Could not change working directory to /");
    // If our guaranteed directory does not exist, terminate the child process
    // to ensure the daemon has not been hijacked.
    return EXIT_FAILURE;
  }

  // A daemon cannot use the terminal, so close standard file descriptors for
  // security reasons
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // Start server
  std::atexit(close_server);
  int server_port = 1234;
  if (server->bind(server_port)) {
    std::cout << std::endl << NEROMON_TAG "\033[1;97mboud to port "
              << std::to_string(server_port) << "\033[0m" << std::endl;
  }
  server->listen(); // listens for any incoming connection

  // Daemon-specific intialization should go here
  const int SLEEP_INTERVAL = 1;

  // Enter daemon loop
  while(true) {
    // Execute daemon heartbeat
    heartbeat();
    // Sleep for a period of time
    sleep(SLEEP_INTERVAL);
  }

  // Close system logs for the child process
  syslog(LOG_NOTICE, "Stopping neroshop-daemon");
  closelog();

  // Terminate the child process when the daemon completes
  return EXIT_SUCCESS;
}
