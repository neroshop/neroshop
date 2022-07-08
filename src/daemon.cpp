#include <vector>
#include <iostream>
#include <thread>
#include <boost/program_options.hpp>
#include <xapian.h>
#include <zmq.hpp>

#include "logging.hpp"
#include "project_config.hpp"

// *****************************************************************************
std::string query_db( const std::string& db_name, std::string&& query_string) {
  if (query_string[0] == 'd' && query_string[1] == 'b') {
    try {
      query_string.erase( 0, 3 );
      NLOG(DEBUG) << "Search db user query '" << query_string << "'";
      // Open the database for searching
      Xapian::Database db( db_name );
      // Start an enquire session
      Xapian::Enquire enquire( db );
      // Parse the query string to produce a Xapian::Query object
      Xapian::QueryParser qp;
      Xapian::Stem stemmer("english");
      qp.set_stemmer( stemmer );
      qp.set_database( db );
      qp.set_stemming_strategy( Xapian::QueryParser::STEM_SOME );
      Xapian::Query query = qp.parse_query( query_string );
      NLOG(DEBUG) << "Parsed query: '" << query.get_description() << "'";
      // Find the top 10 results for the query
      enquire.set_query( query );
      NLOG(DEBUG) << "Set query: '" << query.get_description() << "'";
      Xapian::MSet matches = enquire.get_mset( 0, 10 );
      NLOG(DEBUG) << "Got matches";
      // Construct the results
      auto nr = matches.get_matches_estimated();
      NLOG(DEBUG) << "Got estimated matches: " << nr;
      std::stringstream result;
      result << nr << " results found.";
      if (nr) {
        result << "\nMatches 1-" << matches.size() << ":\n\n";
        for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
          NLOG(DEBUG) << "Getting match: " << i.get_rank();
          result << i.get_rank() + 1 << ": " << i.get_weight() << " docid=" << *i
                   << " [" << i.get_document().get_data() << "]\n";
        }
      }
      NLOG(DEBUG) << "Results: " + result.str();
      return result.str();
    } catch ( const Xapian::Error &e ) {
      NLOG(ERROR) << e.get_description();
    }
  }
  return {};
}

// *****************************************************************************
void index_db( const std::string& db_name, const std::string& input_filename ) {
  if (input_filename.empty()) return;
  NLOG(DEBUG) << "Indexing " << input_filename;
  Xapian::WritableDatabase db( db_name, Xapian::DB_CREATE_OR_OPEN );
  Xapian::TermGenerator indexer;
  Xapian::Stem stemmer( "english" );
  indexer.set_stemmer( stemmer );
  indexer.set_stemming_strategy( indexer.STEM_SOME_FULL_POS );
  std::ifstream cin( input_filename );
  try {
    std::string para;
    std::size_t lcnt = 0, pcnt = 0;
    while (true) {
      std::string line;
      if (cin.eof()) {
        if (para.empty()) break;
      } else {
        getline( cin, line );
        cin.ignore();
        ++lcnt;
      }
      if (line.empty()) {
        if (!para.empty()) {
          // We've reached the end of a paragraph, so index it
          Xapian::Document doc;
          doc.set_data(para);
          indexer.set_document( doc );
          indexer.index_text( para );
          // Add the document to the database
          db.add_document( doc );
          para.resize( 0 );
          ++pcnt;
        }
      } else {
        if (!para.empty()) para += '\n';
        para += line;
      }
    }
    NLOG(INFO) << "Finished indexing " + std::to_string(lcnt) << " lines and "
               << std::to_string(pcnt) << " paragraphs, commit to db";
    // Explicitly commit so that we get to see any errors. WritableDatabase's
    // destructor will commit implicitly (unless we're in a transaction) but
    // will swallow any exceptions produced.
    db.commit();
  } catch ( const Xapian::Error &e ) {
    NLOG(ERROR) << e.get_description();
  }
}

// *****************************************************************************
void run_database( const std::string& db_name,
                   const std::string& input_filename )
{
  el::Helpers::setThreadName( "db" );
  NLOG(INFO) << el::Helpers::getThreadName() << " thread initialized";
  NLOG(INFO) << "Using database: " << db_name;
  if (input_filename.empty()) {
    try {
      Xapian::Database db( db_name );
      NLOG(INFO) << "Number of documents: " << db.get_doccount();
    } catch ( const Xapian::Error &e ) {
      NLOG(ERROR) << e.get_description();
    }
  } else {
    std::ifstream f( input_filename );
    if (not f.good()) {
      NLOG(ERROR) << "Database input file does not exist: " << input_filename;
    } else {
      NLOG(INFO) << "Populating database using: " << input_filename;
      index_db( db_name, input_filename );
   }
  }
}

// *****************************************************************************
void run_server( zmq::socket_t&& socket, const std::string& db_name ) {
  el::Helpers::setThreadName( "server" );
  NLOG(INFO) << el::Helpers::getThreadName() << " thread initialized";
  while(true) {
    zmq::message_t request;
    auto res = socket.recv( request, zmq::recv_flags::none );
    NLOG(DEBUG) << "Received command: '" << request.to_string() << "'";
    auto result = query_db( db_name, request.to_string() );
    socket.send( zmq::buffer(result), zmq::send_flags::none );
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
  int server_port = 1234;
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
  po::store( po::parse_command_line(argc, argv, desc), vm );
  po::notify( vm );

  if (vm.count( "help" )) {

    NLOG(DEBUG) << "help";
    NLOG(INFO) << usage;
    std::stringstream ss;
    ss << desc;
    NLOG(INFO) << ss.str();
    return 1;

  } else if (vm.count( "version" )) {

    NLOG(DEBUG) << "version";
    NLOG(INFO) << neroshop::copyright();
    return 1;

  } else if (vm.count( "license" )) {

    NLOG(DEBUG) << "license";
    NLOG(INFO) << neroshop::license();
    return 1;

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

  } else {

    NLOG(INFO) << "Running in standalone mode";

  }

  // initialize the zmq context with a single IO thread
  zmq::context_t context{ 1 };
  // construct a REP (reply) socket and bind to interface
  zmq::socket_t socket{ context, zmq::socket_type::rep };
  socket.bind( "tcp://*:" + std::to_string(server_port) );

  NLOG(INFO) << "Server bound to port " << std::to_string(server_port);

  // start some threads
  std::vector< std::thread > daemon_threads;
  daemon_threads.emplace_back( run_server, std::move(socket), db_name );
  daemon_threads.emplace_back( run_database, db_name, input_filename );

  // wait for all threads to finish
  for (auto& t : daemon_threads) t.join();

  // Terminate the child process when the daemon completes
  return EXIT_SUCCESS;
}
