#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <zmq.hpp>

#include "logging.hpp"
#include "project_config.hpp"

// *****************************************************************************
void connect( zmq::socket_t& socket, const std::string& host, int port )
{
  NLOG(DEBUG) << "connect";
  NLOG(INFO) << "Connecting to neroshop daemon " << host << ':' << port
             << " ... ";
  socket.connect( "tcp://" + host + ':' + std::to_string(port) );
  socket.send( zmq::message_t("connect"), zmq::send_flags::none );
  // wait for reply from server
  zmq::message_t reply{};
  auto res = socket.recv( reply, zmq::recv_flags::none );
  NLOG(DEBUG) << reply.to_string();
  if (reply.to_string() == "accept") NLOG(INFO) << "Connected.";
}

// *****************************************************************************
void disconnect( bool& connected, zmq::socket_t& socket,
                 const std::string& host, int port )
{
  NLOG(DEBUG) << "disconnect";
  if (connected) {
    socket.disconnect( "tcp://" + host + ':' + std::to_string(port) );
    NLOG(INFO) << "Disconnected";
  } else {
    NLOG(INFO) << "Not connected";
  }
  connected = false;
}

// *****************************************************************************
void status( bool connected )
{
  NLOG(DEBUG) << "status";
  if (connected)
    NLOG(INFO) << "Connected";
  else
    NLOG(INFO) << "Not connected";
}

// *****************************************************************************
void db_query( bool connected, zmq::socket_t& socket, const std::string& query )
{
  NLOG(DEBUG) << "db";
  if (not connected) {
    NLOG(INFO) << "Not connected. Use 'connect' to connect to a "
                 "neroshop daemon.";
    return;
  }

  // send message
  const std::string q( "db " + query );
  socket.send( zmq::message_t(q), zmq::send_flags::none );
  // wait for reply from server
  zmq::message_t reply{};
  auto res = socket.recv( reply, zmq::recv_flags::none );
  NLOG(INFO) << reply.to_string();
}

// *****************************************************************************
void crash_handler( int sig ) {
  if (sig == SIGINT) {
    NLOG(INFO) << "Ctrl-C pressed, " << neroshop::cli_executable()
               << " exiting";
  } else {
    NLOG(ERROR) << "Crashed!";
    el::Helpers::logCrashReason( sig, true );
  }
  // FOLLOWING LINE IS ABSOLUTELY NEEDED TO ABORT APPLICATION
  el::Helpers::crashAbort( sig );
}

// *****************************************************************************
int main( int argc, char **argv ) {
  try {
    // Setup logging
    std::string logfile( neroshop::cli_executable() + ".log" );
    setup_logging( neroshop::cli_executable(), crash_handler, logfile,
                   /* file_logging = */ true, /* console_logging = */ true );

    std::string version( "Neroshop: " + neroshop::cli_executable() + " v"
                         + neroshop::project_version() + "-"
                         + neroshop::build_type() );

    std::string welcome( "Welcome to Neroshop, a peer-to-peer marketplace for "
      "monero users. On Neroshop\nanyone can buy and sell products using "
      "the private digital cash, monero. For more\ninformation on monero, see "
      "https://getmonero.org. This is the command line\nclient of Neroshop. "
      "It needs to connect to a Neroshop daemon to work correctly.\n"
      "Type 'help' to list the available commands." );

    // Display initial info
    NLOG(INFO) << version;
    std::cout << welcome << std::endl;
    NLOG(INFO) << "Logging to " << logfile;

    std::string host = "localhost";
    int port = 55090;
    bool connected = false;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 1 };

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{ context, zmq::socket_type::req };

    auto rootMenu = std::make_unique< cli::Menu >( "neroshop" );

    auto connectCmd = rootMenu -> Insert(
      "connect", { "hostname", "port" },
      [&](std::ostream& out, const std::string& h, int p ) {
        host = h;
        port = p;
        connected = true;
        connect( socket, host, port );
      },
      "Connect to a neroshop daemon. The first argument specifies a\n"
      "\thostname or an IPv4 address in standard dot notation. See also \n"
      "\t'man gethostbyname'. The second argument is an integer specifying\n"
      "\t a port." );

    auto disconnectCmd = rootMenu -> Insert(
      "disconnect",
      [&](std::ostream& out ) { disconnect( connected, socket, host, port ); },
      "Disconnect from a neroshop daemon." );

    auto dbCmd = rootMenu -> Insert(
      "db", { "command" },
      [&](std::ostream& out, const std::string& query) {
        db_query( connected, socket, query );
      },
      "Send database command to server.\n"
      "\tYou must have connected to a neroshop daemon first." );

    auto statusCmd = rootMenu -> Insert(
      "status",
      [&](std::ostream& out) { status( connected ); },
      "Query " + neroshop::cli_executable() + " status" );

    auto welcomeCmd = rootMenu -> Insert(
      "welcome",
      [&](std::ostream& out) {
        NLOG(DEBUG) << "welcome";
        std::cout << welcome << std::endl;
      },
      "Display welcome message" );

    auto versionCmd = rootMenu -> Insert(
      "version",
      [&](std::ostream& out) {
        NLOG(DEBUG) << "version";
        NLOG(INFO) << version;
        NLOG(INFO) << neroshop::copyright();
      },
      "Display neroshop-cli version" );

    auto licenseCmd = rootMenu -> Insert(
      "license",
      [&](std::ostream& out) {
        NLOG(DEBUG) << "license";
        NLOG(INFO) << neroshop::license();
      },
      "Display neroshop-cli license" );

    cli::SetColor();
    cli::Cli cli( std::move(rootMenu) );
    // global exit action
    cli.ExitAction( [&](auto& out){
      disconnect( connected, socket, host, port );
      NLOG(DEBUG) << "End " << neroshop::cli_executable() + '\n'; } );

    cli::LoopScheduler scheduler;
    cli::CliLocalTerminalSession localSession(cli, scheduler, std::cout, 200);
    localSession.ExitAction( [&scheduler](auto& out) { scheduler.Stop(); } );
    scheduler.Run();

    return EXIT_SUCCESS;
  }
  catch (const std::exception& e) {
    std::cerr << "Exception caugth in neroshop-cli main: " << e.what()
              << std::endl;
  }
  catch (...) {
    std::cerr << "Unknown exception caugth in neroshop-cli main." << std::endl;
  }

  return -1;
}
