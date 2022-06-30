#include "cli/cli.h"
#include "cli/clilocalsession.h"
#include "cli/loopscheduler.h"

using namespace cli;
using namespace std;


// *****************************************************************************
int main() {
  try {
      // setup neroshop cli

      CmdHandler colorCmd;
      CmdHandler nocolorCmd;

      auto rootMenu = make_unique< Menu >( "neroshop" );
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

      Cli cli( std::move(rootMenu) );
      // global exit action
      cli.ExitAction( [](auto& out){ out << "End neroshop-cli\n"; } );

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
    cerr << "Exception caugth in neroshop-cli main: " << e.what() << endl;
  }
  catch (...) {
    cerr << "Unknown exception caugth in neroshop-cli main." << endl;
  }

  return -1;
}
