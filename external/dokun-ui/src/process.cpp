#include "../include/process.hpp"

Process::Process()
{
    Factory::get_process_factory()->store(this);
}
////////////////////
Process::Process(const std::string& program, const std::string& arg) : Process()
{
    if(!create(program, arg))
	{
		Logger("Process creation failed");
	}
}
////////////////////
Process::~Process()
{
	terminate();
}
////////////////////	
void * Process::open()
{
#ifdef __windows__
	this->handle = static_cast<void *>(OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId()));
	return this->handle;
#endif	
#ifdef __gnu_linux__
#endif
    return nullptr;
}
////////////////////
bool Process::create(const std::string& program, const std::string& argument)
{
#ifdef __windows__
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory (&si, sizeof(si));
    si.cb = sizeof (si);
    ZeroMemory (&pi, sizeof(pi));
       // static_cast<LPSTR>(const_cast<char *>(program.c_str()))
	std::string cmd = program + argument;   
    // Start the child process. 
    if(CreateProcess( 
	    NULL,//static_cast<LPCTSTR>("glue.exe"),   // No module name (use command line)
        static_cast<LPSTR>(const_cast<char *>(cmd.c_str())),//static_cast<LPCTSTR>("glue.exe srlua.exe main.lua main.exe"),        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    == 0) 
    {
        return false;
    }
    // Wait until child process exits.
    WaitForSingleObject (pi.hProcess, INFINITE);
    // Close process and thread handles. 
    CloseHandle (pi.hProcess);
    CloseHandle (pi.hThread );
#endif	
#ifdef __gnu_linux__
    pid_t child_pid; 
    child_pid = fork (); 
    if (child_pid != 0) {
		this->handle = static_cast<pid_t>(child_pid);
	}		
    else {
	  std::vector<std::string> arg_split = String::split(argument, " ");
	  char * arg_list[arg_split.size()];
	  for(int i = 0; i < arg_split.size(); i++)
	  {
		  arg_list[i] = static_cast<char *>(const_cast<char *>(arg_split[i].c_str()));
		  //std::cout << arg_list[i] << std::endl;
	  }
	  arg_list[arg_split.size()] = nullptr; // arg_list must end with a nullptr
    /* Now execute PROGRAM, searching for it in the path.  */ 
    execvp (static_cast<char *>(const_cast<char *>(program.c_str())), arg_list); 
    /* The execvp  function returns only if an error occurs.  */ 
    fprintf (stderr,  "an error occurred in execvp\n"); 
    abort (); 
  } 	
#endif
	return true;
}
////////////////////
bool Process::terminate()
{
#ifdef __windows__
	return (TerminateProcess(static_cast<HANDLE>(this->handle), 0) != 0);
#endif	
#ifdef __gnu_linux__
    kill(static_cast<pid_t>(handle), SIGTERM); // #include <signal.h>
#endif	
    return false;
}
////////////////////
bool Process::terminate(Process * process)
{
#ifdef __windows__
	return (TerminateProcess(static_cast<HANDLE>(process->get_handle()), 0) != 0);
#endif
#ifdef __gnu_linux__
    kill(static_cast<pid_t>(process->get_handle()), SIGTERM); // #include <signal.h>
#endif	
    return false;
}
////////////////////
void Process::exit(int code)
{
#ifdef __windows__
	ExitProcess(code);
#endif	
#ifdef __gnu_linux__
#endif	
}
////////////////////
#ifdef __windows__
void * Process::get_handle() const
{
	return this->handle;
}
#endif
////////////////////
#ifdef __gnu_linux__
int Process::get_handle() const
{
	return this->handle;
}
////////////////////
int Process::get_process_by_name(const std::string& process_name) {
    int pid = -1;
    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                std::getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    if (process_name == cmdLine)
                        pid = id;
                }
            }
        }
    }
    closedir(dp);
    return pid;
}
#endif
////////////////////
void * Process::get_active()
{
#ifdef __windows__
	return static_cast<void *>(GetCurrentProcess());
#endif
#ifdef __gnu_linux__
#endif	
    return nullptr;
}
