#include <sys/stat.h>
#include <syslog.h>
#include <vector>
#include <iostream>
#include <thread>
#include <boost/program_options.hpp>
#include <xapian.h>

#include "server.hpp"
#include "debug.hpp"
#include "project_config.hpp"

using namespace neroshop;

Server * server = new Server();

// *****************************************************************************
void close_server() {
    server->shutdown();
    delete server;
    server = nullptr;
    std::cout << NEROMON_TAG "\033[1;91mdisconnected\033[0m" << std::endl;
}

// *****************************************************************************
void interpret_query( std::string&& query ) {
  if (query.find("lmdb") != std::string::npos) {
    std::cout << "lmdb query" << '\n';
  }
}

// *****************************************************************************
void run_server() {
  while(true) {
    if (server->accept()) server->write("accept");
    interpret_query( server->read() );
    sleep(1);
  }
}

// *****************************************************************************
void run_database( const std::string& db_name ) {
  Xapian::WritableDatabase db( db_name, Xapian::DB_CREATE_OR_OPEN );
  Xapian::TermGenerator indexer;
  Xapian::Stem stemmer( "english" );
  indexer.set_stemmer( stemmer );
  indexer.set_stemming_strategy( indexer.STEM_SOME_FULL_POS );
}

namespace po = boost::program_options;

// *****************************************************************************
// neroshop daemon (neromon) main
int main( int argc, char **argv ) {

  // Display initial info
  std::string version( "Neroshop: " + neroshop::daemon_executable() + " v"
                       + neroshop::project_version() + "-"
                       + neroshop::build_type() );

  std::string welcome( "This is the daemon of neroshop. It can run standalone "
    "or as a daemon in the\nbackground using --detach. You can use " +
     neroshop::cli_executable() + " to interact with it.\n" );

  // Supported command line arguments
  po::options_description desc("Arguments");
  desc.add_options()
    ("help", "Show help message")
    ("version", "Show version information")
    ("license", "Show license information")
    ("detach", "Run as a daemon in the background")
    ("port", po::value<int>(), "Listen on custom port given")
    ("db", po::value<std::string>(), "Use database given")
  ;

  po::variables_map vm;
  po::store( po::parse_command_line(argc, argv, desc), vm );
  po::notify( vm );

  int server_port = 1234;
  std::string db_name;

  if (vm.count( "help" )) {

    std::cout << version << '\n' << welcome << '\n' << desc << '\n';
    return 1;

  } else if (vm.count( "version" )) {

    std::cout << version << '\n' << neroshop::copyright() << '\n';
    return 1;

  } else if (vm.count( "license" )) {

    std::cout << version << '\n' << neroshop::license() << '\n';
    return 1;

  } if (vm.count( "port" )) {

    server_port = vm[ "port" ].as< int >();

  } if (vm.count( "db" )) {

    db_name = vm[ "db" ].as< std::string >();

  } else if (vm.count( "detach" )) {

    std::cout << "Running in daemon mode.\n";

    // Fork the current process. The parent process continues with a process ID
    // greater than 0.  A process ID lower than 0 indicates a failure in either
    // process.
    pid_t pid = fork();
    if (pid > 0) {
      std::cout << NEROMON_TAG "\033[1;97mForked PID: \033[0;32m" << pid
                << "\033[0m" << std::endl;
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
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);

  } else {

    std::cout << "Running in standalone mode." << std::endl;

  }

  if (db_name.empty()) {
    std::cerr << "Need a database name\n";
    return -1;
  }

  // Start server
  std::atexit(close_server);
  if (server->bind(server_port)) {
    std::cout << NEROMON_TAG "\033[1;97mServer bound to port \033[0;36m"
              << std::to_string(server_port) << "\033[0m" << std::endl;
  }
  server->listen(); // listens for any incoming connection

  // start some threads
  std::vector< std::thread > daemon_threads;
  daemon_threads.emplace_back( run_server );
  daemon_threads.emplace_back( run_database, db_name );

  // wait for all threads to finish
  for (auto& t : daemon_threads) t.join();

  // Close system logs for the child process
  syslog(LOG_NOTICE, "Stopping neroshop-daemon");
  closelog();

  // Terminate the child process when the daemon completes
  return EXIT_SUCCESS;
}
