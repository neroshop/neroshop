#include "cli/cli.h"
#include "cli/clilocalsession.h"
#include "cli/loopscheduler.h"

using namespace cli;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// cli plugin mechanism

class Plugin
{
public:
    explicit Plugin(string _name) : name(std::move(_name)) { cout << "Plugin " << name << " loaded" << endl; }
    virtual ~Plugin() { cout << "Plugin " << name << " unloaded" << endl; }
    const string& Name() const { return name; }
private:
    const string name;
};

using Factory = function<unique_ptr<Plugin>(Menu*)>;

class PluginRegistry
{
public:
    static PluginRegistry& Instance() { return instance; }
    void Register(const string& name, Factory factory) { plugins.emplace_back(name, factory); }
    void Print(ostream& out) const
    {
        for (auto& p: plugins)
            out << " - " << p.first << endl;
    }
    unique_ptr<Plugin> Create(const string& name, Menu* menu) const
    {
        for (auto& p: plugins)
            if (p.first == name)
                return p.second(menu);
        return {};
    }
private:
    static PluginRegistry instance;
    vector<pair<string, Factory>> plugins;
};

PluginRegistry PluginRegistry::instance;

class PluginContainer
{
public:
    static PluginContainer& Instance() { return instance; }
    void SetMenu(Menu& _menu) { menu = &_menu; }
    void Load(const string& plugin)
    {
        auto p = PluginRegistry::Instance().Create(plugin, menu);
        if (p)
            plugins.push_back(move(p));
    }
    void Unload(const string& plugin)
    {
        plugins.erase(
            remove_if(
                plugins.begin(),
                plugins.end(),
                [&](auto& p){ return p->Name() == plugin; }
            ),
            plugins.end()
        );
    }
    void PrintLoaded(ostream& out) const
    {
        for (auto& p: plugins)
            out << " - " << p->Name() << endl;
    }
private:
    static PluginContainer instance;
    vector<unique_ptr<Plugin>> plugins;
    Menu* menu;
};

PluginContainer PluginContainer::instance;

class Registration
{
public:
    Registration(const string& name, Factory factory)
    {
        PluginRegistry::Instance().Register(name, std::move(factory));
    }
};

template <typename T, const char* NAME>
class RegisteredPlugin : public Plugin
{
public:
    RegisteredPlugin() : Plugin(NAME)
    {
        const Registration& dummy = registration;
        do { (void)(dummy); } while (false); // silence unused variable warning
    }
private:
    static unique_ptr<Plugin> Create(Menu* menu) { return make_unique<T>(menu); }
    static Registration registration;
};

template <typename T, const char* NAME>
Registration RegisteredPlugin<T, NAME>::registration(NAME, &RegisteredPlugin<T, NAME>::Create);

// cli arithmetic plugin

constexpr char ArithmeticName[] = "arithmetic";

class Arithmetic : public RegisteredPlugin<Arithmetic, ArithmeticName>
{
public:
    explicit Arithmetic(Menu* menu)
    {
        auto subMenu = make_unique<Menu>(Name());
        subMenu->Insert(
                "add", {"first_term", "second_term"},
                [](std::ostream& out, int x, int y)
                {
                    out << x << " + " << y << " = " << (x+y) << "\n";
                },
                "Print the sum of the two numbers" );
        subMenu->Insert(
                "add",
                [](std::ostream& out, int x, int y, int z)
                {
                    out << x << " + " << y << " + " << z << " = " << (x+y+z) << "\n";
                },
                "Print the sum of the three numbers" );
        subMenu->Insert(
                "sub", {"subtrahend", "minuend"},
                [](std::ostream& out, int x, int y)
                {
                    out << x << " - " << y << " = " << (x-y) << "\n";
                },
                "Print the result of a subtraction" );

        menuHandler = menu->Insert(move(subMenu));
    }
    ~Arithmetic() override
    {
        menuHandler.Remove();
    }
private:
    CmdHandler menuHandler;
};


// cli strings plugin

constexpr char StringsName[] = "strings";

class Strings : public RegisteredPlugin<Strings, StringsName>
{
public:
    explicit Strings(Menu* menu)
    {
        auto subMenu = make_unique<Menu>(Name());
        subMenu->Insert(
                "reverse", {"string_to_revert"},
                [](std::ostream& out, const string& arg)
                {
                    string copy(arg);
                    std::reverse(copy.begin(), copy.end());
                    out << copy << "\n";
                },
                "Print the reverse string" );

        subMenu->Insert(
                "upper",
                [](std::ostream& out, string arg)
                {
                    std::transform(arg.begin(), arg.end(),arg.begin(), ::toupper);
                    out << arg << "\n";
                },
                "Print the string in uppercase" );
        subMenu->Insert(
            "sort", {"list of strings separated by space"},
            [](std::ostream& out, std::vector<std::string> data)
            {
                std::sort(data.begin(), data.end());
                out << "sorted list: ";
                std::copy(data.begin(), data.end(), std::ostream_iterator<std::string>(out, " "));
                out << "\n";
            },
            "Alphabetically sort a list of words" );
        menuHandler = menu->Insert(move(subMenu));
    }
    ~Strings() override
    {
        menuHandler.Remove();
    }
private:
    CmdHandler menuHandler;
};

// *****************************************************************************
int main() {
  try {
      CmdHandler colorCmd;
      CmdHandler nocolorCmd;

      // setup neroshop cli

      auto rootMenu = make_unique< Menu >( "neroshop" );
      PluginContainer::Instance().SetMenu(*rootMenu);
      rootMenu->Insert(
              "list",
              [](std::ostream& out){ PluginRegistry::Instance().Print(out); },
              "Print the plugin list" );
      rootMenu->Insert(
              "loaded",
              [](std::ostream& out)
              {
                  PluginContainer::Instance().PrintLoaded(out);
              },
              "Load the plugin specified" );
      rootMenu->Insert(
              "load", {"plugin_name"},
              [](std::ostream&, const string& plugin)
              {
                  PluginContainer::Instance().Load(plugin);
              },
              "Load the plugin specified" );
      rootMenu->Insert(
              "unload", {"plugin_name"},
              [](std::ostream&, const string& plugin)
              {
                  PluginContainer::Instance().Unload(plugin);
              },
              "Unload the plugin specified" );
      colorCmd = rootMenu -> Insert(
              "color",
              [&](std::ostream& out)
              {
                  out << "Colors ON\n";
                  SetColor();
                  colorCmd.Disable();
                  nocolorCmd.Enable();
              },
              "Enable colors in the cli" );
      nocolorCmd = rootMenu -> Insert(
              "nocolor",
              [&](std::ostream& out)
              {
                  out << "Colors OFF\n";
                  SetNoColor();
                  colorCmd.Enable();
                  nocolorCmd.Disable();
              },
              "Disable colors in the cli" );


      Cli cli( std::move(rootMenu) );
      // global exit action
      cli.ExitAction( [](auto& out){
        out << "End neroshop cli.\n"; } );

      cli::LoopScheduler scheduler;
      CliLocalTerminalSession localSession(cli, scheduler, std::cout, 200);
      localSession.ExitAction(
          [&scheduler](auto& out) // session exit action
          {
              out << "Closing neroshop-cli...\n";
              scheduler.Stop();
          }
      );

      scheduler.Run();

      // instead of scheuler.Run() you could do:
      /*
      while(!scheduler.Stopped())
          scheduler.PollOne();
      */

      return 0;
  }
  catch (const std::exception& e)
  {
      cerr << "Exception caugth in neroshop-cli main: " << e.what() << endl;
  }
  catch (...)
  {
      cerr << "Unknown exception caugth in neroshop-cli main." << endl;
  }
  return -1;
}
