#include <vector>
#include <iostream>
#include <thread>
#include <boost/program_options.hpp>
#include <zmq.hpp>

#include "logging.hpp"
#include "project_config.hpp"
#include "db.hpp"

// *****************************************************************************
void run_database( const std::string& db_name,
                   const std::string& input_filename )
{
  el::Helpers::setThreadName( "db" );
  NLOG(INFO) << el::Helpers::getThreadName() << " thread initialized";
  NLOG(INFO) << "Using database: " << db_name;
  if (input_filename.empty()) {
    NLOG(INFO) << "Number of documents: " << neroshop::get_doccount( db_name );
  } else {
    std::ifstream f( input_filename );
    if (not f.good()) {
      NLOG(ERROR) << "Database input file does not exist: " << input_filename;
    } else {
      NLOG(INFO) << "Populating database using: " << input_filename;
      neroshop::index_db( db_name, input_filename );
   }
  }
}

// *****************************************************************************
void run_server( const std::string& db_name, int server_port ) {
  // query switch
  auto interpret_query = [&]( zmq::socket_t& socket, std::string&& q ){
    NLOG(DEBUG) << "Interpreting message: '" << q << "'";
    if (q[0]=='c' && q[1]=='o') {
      const std::string q( "accept" );
      NLOG(DEBUG) << "Sending message: '" << q << "'";
      socket.send( zmq::buffer(q), zmq::send_flags::none );
    } else if (q[0]=='d' && q[1]=='b') {
      auto result = neroshop::query_db( db_name, std::move(q) );
      socket.send( zmq::buffer(result), zmq::send_flags::none );
   } else {
      NLOG(ERROR) << "unknown command";
      socket.send( zmq::buffer("unknown command"), zmq::send_flags::none );
   }
  };
  el::Helpers::setThreadName( "server" );
  NLOG(INFO) << el::Helpers::getThreadName() << " thread initialized";
  // initialize zmq context with a single IO thread
  zmq::context_t context{ 1 };
  // construct a REP (reply) socket and bind to interface
  zmq::socket_t socket{ context, zmq::socket_type::rep };
  socket.bind( "tcp://*:" + std::to_string(server_port) );
  NLOG(INFO) << "Server bound to port " << server_port;
  // listen for messages
  while(true) {
    zmq::message_t request;
    auto res = socket.recv( request, zmq::recv_flags::none );
    NLOG(DEBUG) << "Received message: '" << request.to_string() << "'";
    interpret_query( socket, request.to_string() );
  }
}

void crash_handler( int sig ) {
  if (sig == SIGINT) {
    NLOG(DEBUG) << "Ctrl-C pressed, " << neroshop::daemon_executable()
                << " exiting";
  } else {
    NLOG(ERROR) << "Crashed!";
    el::Helpers::logCrashReason( sig, true );
  }
  // FOLLOWING LINE IS ABSOLUTELY NEEDED TO ABORT APPLICATION
  el::Helpers::crashAbort( sig );
}

// *****************************************************************************
// neroshop daemon (neromon) main
int main( int argc, char **argv ) {

  // Setup logging
  std::string logfile( neroshop::daemon_executable() + ".log" );
  setup_logging( neroshop::daemon_executable(), crash_handler, logfile,
                 /* file_logging = */ true, /* console_logging = */ true );

  std::string version( "Neroshop: " + neroshop::daemon_executable() + " v"
                       + neroshop::project_version() + "-"
                       + neroshop::build_type() );

  std::string welcome( "This is the daemon of neroshop. It can run standalone "
    "or as a daemon in the\nbackground using --detach. You can use " +
     neroshop::cli_executable() + " to interact with it." );

  std::string usage( "Usage: " + neroshop::daemon_executable() + " [OPTIONS]" );

  // Defaults
  int server_port = 55090;
  std::string db_name( "neroshop.db" );
  std::string input_filename;

  // Display initial info
  NLOG(INFO) << version;
  std::cout << welcome << std::endl;

  // Supported command line arguments
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  std::string port_help( "Listen on custom port, default: " );
  port_help += std::to_string( server_port );
  std::string db_help( "Use database, default: " + db_name );
  std::string input_help( "Database input filename" );
  desc.add_options()
    ("help", "Show help message")
    ("version", "Show version information")
    ("license", "Show license information")
    ("detach", "Run as a daemon in the background")
    ("port", po::value<int>(), port_help.c_str())
    ("db", po::value<std::string>(), db_help.c_str())
    ("input", po::value<std::string>(), input_help.c_str())
  ;

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional({}).run(),
              vm);
  } catch (po::too_many_positional_options_error &e) {
    NLOG(ERROR) << "Command line only accepts options with '--': " << e.what();
    return EXIT_FAILURE;
  } catch (po::error_with_option_name &e) {
    NLOG(ERROR) << "Command line: " << e.what();
    return EXIT_FAILURE;
  }
  po::notify( vm );

  if (vm.count( "help" )) {

    NLOG(DEBUG) << "help";
    NLOG(INFO) << usage;
    std::stringstream ss;
    ss << desc;
    NLOG(INFO) << ss.str();
    return EXIT_SUCCESS;

  } else if (vm.count( "version" )) {

    NLOG(DEBUG) << "version";
    NLOG(INFO) << neroshop::copyright();
    return EXIT_SUCCESS;

  } else if (vm.count( "license" )) {

    NLOG(DEBUG) << "license";
    NLOG(INFO) << neroshop::license();
    return EXIT_SUCCESS;

  } else if (vm.count( "port" )) {

    NLOG(DEBUG) << "port";
    server_port = vm[ "port" ].as< int >();

  } else if (vm.count( "db" )) {

    NLOG(DEBUG) << "db";
    db_name = vm[ "db" ].as< std::string >();

  } else if (vm.count( "input" )) {

    NLOG(DEBUG) << "input";
    input_filename = vm[ "input" ].as< std::string >();

  }

  NLOG(INFO) << "Logging to " << logfile;

  if (vm.count( "detach" )) {

    NLOG(INFO) << "Running in daemon mode";

    // Fork the current process. The parent process continues with a process ID
    // greater than 0.  A process ID lower than 0 indicates a failure in either
    // process.
    pid_t pid = fork();
    if (pid > 0) {
      NLOG(INFO) << "Forked PID: " << pid;
      return EXIT_SUCCESS;
    } else if (pid < 0) {
      return EXIT_FAILURE;
    }

    // Generate a session ID for the child process and ensure it is valid.
    if(setsid() < 0) {
      // Log failure and exit
      std::cerr << "Could not generate session ID for child process\n";
      // If a new session ID could not be generated, we must terminate the child
      // process or it will be orphaned.
      return EXIT_FAILURE;
    }

    // A daemon cannot use the terminal, so close standard file descriptors for
    // security reasons and also because ctest hangs in daemon mode waiting for
    // stdout and stderr to be closed.
    close( STDIN_FILENO );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );

  } else {

    NLOG(INFO) << "Running in standalone mode";

  }

  // start some threads
  std::vector< std::thread > daemon_threads;
  daemon_threads.emplace_back( run_server, db_name, server_port );
  daemon_threads.emplace_back( run_database, db_name, input_filename );

  // wait for all threads to finish
  for (auto& t : daemon_threads) t.join();

  // Terminate the child process when the daemon completes
  return EXIT_SUCCESS;
}
