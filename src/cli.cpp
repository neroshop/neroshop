#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <zmq.hpp>

#include "project_config.hpp"

using namespace neroshop;

// *****************************************************************************
void connect( zmq::socket_t& socket, const std::string& host, int port )
{
  std::cout << "Connecting to neroshop daemon " << host << ':' << port << '\n';
  socket.connect( "tcp://" + host + ':' + std::to_string(port) );
}

// *****************************************************************************
void disconnect( bool& connected, zmq::socket_t& socket,
                 const std::string& host, int port )
{
  if (connected) {
    socket.disconnect( "tcp://" + host + ':' + std::to_string(port) );
    std::cout << "Disconnected." << std::endl;
  } else {
    std::cout << "Not connected." << std::endl;
  }
  connected = false;
}

// *****************************************************************************
void status( bool connected )
{
  if (connected)
    std::cout << "Connected." << std::endl;
  else
    std::cout << "Not connected." << std::endl;
}

// *****************************************************************************
void db_query( bool connected, zmq::socket_t& socket, const std::string& query )
{
  if (not connected) {
    std::cout << "Not connected. Use 'connect' to connect to a "
                 "neroshop daemon." << std::endl;
    return;
  }

  // send message
  const std::string q( "db " + query );
  socket.send( zmq::message_t(q), zmq::send_flags::none );

  // wait for reply from server
  zmq::message_t reply{};
  socket.recv( reply, zmq::recv_flags::none );
  std::cout << reply.to_string() << std::endl;
}


using namespace cli;

// *****************************************************************************
int main() {
  try {
    // Display initial info
    std::cout << "Neroshop: " << cli_executable() << " v"
              << project_version() << '-'
              << build_type() << '\n'
              << "This is the command line client of neroshop.\nIt needs to "
                 "connect to a neroshop daemon to work correctly.\n"
              << "Logging to ...\n"
                 "Type 'help' to list the available commands.\n";

    // Setup neroshop cli commands
    CmdHandler colorCmd;
    CmdHandler nocolorCmd;

    std::string host = "localhost";
    int port = 1234;
    bool connected = false;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 1 };

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{ context, zmq::socket_type::req };

    auto rootMenu = std::make_unique< Menu >( "neroshop" );

    colorCmd = rootMenu -> Insert(
      "color",
      [&](std::ostream& out) {
        out << "Colors ON\n";
        SetColor();
        colorCmd.Disable();
        nocolorCmd.Enable();
      },
      "Enable colors" );

    nocolorCmd = rootMenu -> Insert(
      "nocolor",
      [&](std::ostream& out) {
        out << "Colors OFF\n";
        SetNoColor();
        colorCmd.Enable();
        nocolorCmd.Disable();
      },
      "Disable colors" );

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
      "Query " + cli_executable() + " status" );

    auto welcomeCmd = rootMenu -> Insert(
      "welcome",
      [&](std::ostream& out) {
        out << "Welcome to Neroshop, a peer-to-peer marketplace for monero "
               "users. On Neroshop\nanyone can buy and sell products using "
               "the digital money, monero. For more\ninformation on monero, "
               "see https://getmonero.org.\n"; },
        "Display welcome message" );

    auto versionCmd = rootMenu -> Insert(
      "version",
      [&](std::ostream& out) {
        out << cli_executable() << " v" << project_version() << '-'
            << build_type() << '\n'
            << copyright() << '\n';
      },
      "Display neroshop-cli version" );

    auto licenseCmd = rootMenu -> Insert(
      "license",
      [&](std::ostream& out) {
        out << license() << '\n';
      },
      "Display neroshop-cli license" );

    // Turn on colors by default
    SetColor();
    colorCmd.Disable();
    nocolorCmd.Enable();

    Cli cli( std::move(rootMenu) );
    // global exit action
    cli.ExitAction( [&](auto& out){
      disconnect( connected, socket, host, port );
      out << "End " << cli_executable() + '\n'; } );

    cli::LoopScheduler scheduler;
    CliLocalTerminalSession localSession(cli, scheduler, std::cout, 200);
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
