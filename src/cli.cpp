#include "cli/cli.h"
#include "cli/clilocalsession.h"
#include "cli/loopscheduler.h"

#include "project_config.hpp"
#include "client.hpp"

neroshop::Client * client = neroshop::Client::get_main_client();

// *****************************************************************************
void connect( const std::string& host, int port )
{
  using namespace neroshop;

  if (client->connected()) {
    std::cout << "Alredy connected." << std::endl;
    return;
  }

  std::cout << "Client connecting to neroshop daemon " << host << ':' << port
            << " ... ";

  if (not client->connect( port, host ))
    std::cout << "failed." << std::endl;
  else
    if (client->read() == "accept") std::cout << "connected." << std::endl;
}

// *****************************************************************************
void status()
{
  using namespace neroshop;

  if (client->connected())
    std::cout << "Connected." << std::endl;
  else
    std::cout << "Not connected." << std::endl;
}

// *****************************************************************************
void disconnect()
{
  using namespace neroshop;

  if (client->connected()) {
    client->disconnect();
    std::cout << "Disconnected." << std::endl;
  } else {
    std::cout << "Not connected." << std::endl;
  }
}

using namespace cli;

// *****************************************************************************
int main() {
  try {
      // Display initial info
      std::cout << "Neroshop: " << neroshop::cli_executable() << " v"
                << neroshop::project_version() << '-'
                << neroshop::build_type() << '\n'
                << "This is the command line client of neroshop.\nIt needs to "
                   "connect to a neroshop daemon to work correctly.\n"
                << "Logging to ...\n"
                   "Type 'help' to list the available commands.\n";

      // Setup neroshop cli commands
      CmdHandler colorCmd;
      CmdHandler nocolorCmd;

      std::string default_hostname = "localhost";
      int default_port = 1234;

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
        "connect",
        [&](std::ostream& out, const std::string& host, int port ) {
          connect( host, port );
        },
        "Connect to a neroshop daemon. The first argument <string> specifies\n"
        "\ta hostname or an IPv4 address in standard dot notation. The default\n"
        "\thostname is 'localhost'. See also 'man gethostbyname'. The second\n"
        "\targument <int> is an integer specifying a port. The default port is\n"
        "\t1234." );

      auto disconnectCmd = rootMenu -> Insert(
        "disconnect",
        [&](std::ostream& out ) { disconnect(); },
        "Disconnect from a neroshop daemon." );

      auto dbCmd = rootMenu -> Insert(
        "db",
        [&](std::ostream& out) {
          out << "Querying neroshop database ...\n";
        },
        "You must have connected to a neroshop daemon first.",
        { "query command" } );
      auto statusCmd = rootMenu -> Insert(
        "status",
        [&](std::ostream& out) { status(); },
        "Query " + neroshop::cli_executable() + " status" );

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
          out << neroshop::cli_executable() << " v"
              << neroshop::project_version() << '-'
              << neroshop::build_type() << '\n'
              << neroshop::copyright() << '\n';
        },
        "Display neroshop-cli version" );

      auto licenseCmd = rootMenu -> Insert(
        "license",
        [&](std::ostream& out) {
          out << neroshop::license() << '\n';
        },
        "Display neroshop-cli license" );

      Cli cli( std::move(rootMenu) );
      // global exit action
      cli.ExitAction( [](auto& out){
        if (client->connected()) disconnect();
        out << "End " << neroshop::cli_executable() + '\n'; } );

      cli::LoopScheduler scheduler;
      CliLocalTerminalSession localSession(cli, scheduler, std::cout, 200);
      localSession.ExitAction(
        [&scheduler](auto& out) // session exit action
        {
          scheduler.Stop();
        }
      );

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
