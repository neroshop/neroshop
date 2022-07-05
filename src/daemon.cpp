#include <sys/stat.h>
#include <syslog.h>
#include <vector>
#include <iostream>
#include <thread>
#include <boost/program_options.hpp>
#include <xapian.h>
#include <zmq.hpp>

#include "debug.hpp"
#include "project_config.hpp"

using namespace neroshop;

#define NEROMON_TAG std::string("\033[1;95m[neromon]: \033[0m")

// *****************************************************************************
void interpret_query( std::string&& query ) {
  if (query[0] == 'd' && query[1] == 'b') {
     query.erase( 0, 3 );
     std::cout << "db '" << query << "'\n";
  }
}

// *****************************************************************************
void run_server( zmq::socket_t&& socket ) {

  using namespace std::chrono_literals;

  while(true) {
    zmq::message_t request;
    socket.recv( request, zmq::recv_flags::none );
    std::cout << NEROMON_TAG << "\033[1;97mReceived \033[0m"
            << request.to_string() << "\033[0m" << std::endl;
    socket.send( zmq::buffer("accept"), zmq::send_flags::none );
    interpret_query( request.to_string() );
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
     neroshop::cli_executable() + " to interact with it." );

  std::string usage( "Usage: " + neroshop::daemon_executable() + " [OPTIONS]" );

  // Defaults
  int server_port = 1234;
  std::string db_name( "neroshop_test" );

  // Supported command line arguments
  po::options_description desc( "Options" );
  std::string port_help( "Listen on custom port, default: " );
  port_help += std::to_string( server_port );
  std::string db_help( "Use database, default: " + db_name );
  desc.add_options()
    ("help", "Show help message")
    ("version", "Show version information")
    ("license", "Show license information")
    ("detach", "Run as a daemon in the background")
    ("port", po::value<int>(), port_help.c_str())
    ("db", po::value<std::string>(), db_help.c_str())
  ;

  po::variables_map vm;
  po::store( po::parse_command_line(argc, argv, desc), vm );
  po::notify( vm );

  if (vm.count( "help" )) {

    std::cout << version << "\n\n" << welcome << "\n\n" << usage << "\n\n"
              << desc << '\n';
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

  }

  if (db_name.empty()) {
    std::cerr << "Need a database name\n";
    return -1;
  }

  std::cout << version << '\n';

  if (vm.count( "detach" )) {

    std::cout << "Running in daemon mode.\n";

    // Fork the current process. The parent process continues with a process ID
    // greater than 0.  A process ID lower than 0 indicates a failure in either
    // process.
    pid_t pid = fork();
    if (pid > 0) {
      std::cout << NEROMON_TAG << "\033[1;97mForked PID: \033[0;32m" << pid
                << "\033[0m" << std::endl;
      return EXIT_SUCCESS;
    } else if (pid < 0) {
      return EXIT_FAILURE;
    }

    // TODO: I think we should log to ~/.config/neroshop/ instead of syslog

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

  std::cout << "Using database: " << db_name << '\n';

  // initialize the zmq context with a single IO thread
  zmq::context_t context{ 1 };
  // construct a REP (reply) socket and bind to interface
  zmq::socket_t socket{ context, zmq::socket_type::rep };
  socket.bind( "tcp://*:" + std::to_string(server_port) );

  std::cout << NEROMON_TAG << "\033[1;97mServer bound to port \033[0;36m"
            << std::to_string(server_port) << "\033[0m" << std::endl;

  // start some threads
  std::vector< std::thread > daemon_threads;
  daemon_threads.emplace_back( run_server, std::move(socket) );
  daemon_threads.emplace_back( run_database, db_name );

  // wait for all threads to finish
  for (auto& t : daemon_threads) t.join();

  // Close system logs for the child process
  syslog(LOG_NOTICE, "Stopping neroshop-daemon");
  closelog();

  // Terminate the child process when the daemon completes
  return EXIT_SUCCESS;
}
