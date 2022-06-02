#ifndef SYSTEM_HPP_DOKUN
#define SYSTEM_HPP_DOKUN

#include "platform.hpp"
#include "string.hpp"
#ifdef __cplusplus

class System
{
public:
    // current_distro - returns the name of the current distro
    static inline std::string get_current_distro() {
    #ifdef __gnu_linux__
        // get output from command
        FILE* pipe = popen("ls /etc/*release; ls /etc/*version", "r"); // &&
        if (!pipe) return "ERROR";
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        // save release filenames
        std::vector<std::string> os_filenames = String::split(result, "\n");
        if(os_filenames.empty()) return "linux"; // defaults to linux
        // check for lsb-release and os-release first
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/lsb-release") != os_filenames.end()) {}
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/os-release") != os_filenames.end()) { // https://www.linux.org/docs/man5/os-release.html
            std::ifstream os_file("/etc/os-release");
            if(!os_file.good()) return "linux"; // defaults to linux
            std::stringstream os_stream;
            os_stream << os_file.rdbuf(); // dump file content into stringstream//std::cout << os_stream.str() << std::endl;
            std::vector<std::string> os_content = String::split(os_stream.str(), "\n"); // split each newline
            // ID= will give us the current OS name
            for(int i = 0; i < os_content.size(); i++) {
                if(os_content[i].find("ID=") != std::string::npos) {
                    os_content[i] = String::swap_first_of(os_content[i], "ID=", ""); // remove ID=
                    std::string os_name = os_content[i];//String::remove_all(os_content[i], "\"");// remove quotes
                    return os_name;
                }            
            }
        }
    #endif
        return "";
    }
    // base_distro - returns the name of the major distro in which the current distro is based off
    static inline std::string get_base_distro() {
    #ifdef __gnu_linux__
        // the ls /etc/*release command should list all the release files on the system
        // ls /etc/*version for some other distros 
        // get output from command
        FILE* pipe = popen("ls /etc/*release; ls /etc/*version", "r"); // &&
        if (!pipe) return "ERROR";
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        // save release filenames
        std::vector<std::string> os_filenames = String::split(result, "\n");
        if(os_filenames.empty()) return "linux"; // defaults to linux
    #ifdef DOKUN_DEBUG
        std::cout << "os_filenames " << "(" << os_filenames.size() << "): " << std::endl;
        for(int i = 0; i < os_filenames.size(); i++) std::cout << os_filenames[i] << std::endl;
    #endif
        // check if vector contains a specific release filename - https://serverfault.com/questions/422880/etc-release-files-examples
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/debian_version") != os_filenames.end()) return "debian";
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/arch-release") != os_filenames.end()) return "arch"; // or Arch Linux
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/fedora-release") != os_filenames.end()) return "fedora";
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/SuSE-release") != os_filenames.end()) return "suse"; // or SUSE Linux // deprecated: https://en.opensuse.org/Etc_SuSE-release
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/redhat-release") != os_filenames.end()) return "rhel";//return "fedora"; // or Red Hat Enterprise Linux (RHEL)
        //if(std::find(os_filenames.begin(), os_filenames.end(), "") != os_filenames.end()) return "";
        // fallback to lsb-release and os-release, if all else fails
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/lsb-release") != os_filenames.end()) {}
        if(std::find(os_filenames.begin(), os_filenames.end(), "/etc/os-release") != os_filenames.end()) { // https://www.linux.org/docs/man5/os-release.html
            std::ifstream os_file("/etc/os-release");
            if(!os_file.good()) return "linux"; // defaults to linux
            std::stringstream os_stream;
            os_stream << os_file.rdbuf(); // dump file content into stringstream//std::cout << os_stream.str() << std::endl;
            std::vector<std::string> os_content = String::split(os_stream.str(), "\n"); // split each newline
            // ID_LIKE= will give us the OS that the current OS is based on and ID= will give us the current OS
            for(int i = 0; i < os_content.size(); i++) {
                // Debian and Debian-based distros (e.g Ubuntu)
                if(os_content[i].find("ID=debian") != std::string::npos) return "debian"; // ID=debian
                if(os_content[i].find("ID_LIKE=debian") != std::string::npos) return "debian"; // ID=ubuntu
                // Ubuntu-based distros (e.g Linux Mint)
                if(os_content[i].find("ID_LIKE=ubuntu") != std::string::npos) return "debian"; //return "ubuntu";// ID=linuxmint
                // Arch and Arch-based distros
                if(os_content[i].find("ID=arch") != std::string::npos) return "arch";
                if(os_content[i].find("ID_LIKE=arch") != std::string::npos) return "arch"; // ID=manjaro
                // Fedora and Fedora-based distros (e.g RHEL)
                if(os_content[i].find("ID=fedora") != std::string::npos) return "fedora";
                if(os_content[i].find("ID_LIKE=fedora") != std::string::npos) return "fedora";
                if(os_content[i].find("ID_LIKE=\"fedora\"") != std::string::npos) return "fedora"; // ID="rhel"
                // Red Hat Enterprise Linux (RHEL)-based distros
                if(os_content[i].find("ID_LIKE=\"rhel fedora\"") != std::string::npos) return "fedora";//return "rhel"; // ID="centos"
                // SUSE and SUSE-based distros (e.g OpenSUSE)
                if(os_content[i].find("ID=suse") != std::string::npos) return "suse";
                if(os_content[i].find("ID_LIKE=\"suse\"") != std::string::npos) return "suse"; // ID=opensuse
                // OpenSUSE-based distros
                if(os_content[i].find("ID_LIKE=\"suse opensuse\"") != std::string::npos) return "suse";//return "opensuse"; // ID="opensuse-leap", ID="opensuse-tumbleweed", ID="opensuse-tumbleweed-kubic"
                /*// _ and _-based distros
                if(os_content[i].find("ID=") != std::string::npos) return "";
                if(os_content[i].find("ID_LIKE=") != std::string::npos) return "";*/
            }
        }        
    #endif
        return "";
    }
    // distro
    static inline std::string get_distro() {
        return get_base_distro();
    }
    // directory
	static std::string get_current_dir()
	{
		std::string current;
    #ifdef DOKUN_WIN32
	    char buffer[1024];
	    GetModuleFileName(nullptr, buffer, 1024);
	    current =  buffer;
	    current =  current.substr(0, current.find_last_of("\\/"));
    #endif
    #ifdef __gnu_linux__
	    char buffer[1024];
        if (getcwd(buffer, sizeof(buffer)) != nullptr)
		    current = buffer;
    #endif
	    return current;
	}
	// home dir
	static std::string get_home_dir() {
	#ifdef __gnu_linux__
	    std::string home_dir = getenv("HOME"); // /home/bob // in #include <stdlib.h>
	    if(home_dir.empty()) {
	        // if getenv fails, try getpwuid
	        std::cout << "::getenv failed. Trying ::getpwuid" << std::endl;
	        std::string user = get_user();
	        if(user.empty()) return ""; // return empty string if ::getpwuid fails too
	        home_dir = "/home/" + user;
	    }
	    return home_dir;
	#endif    
	    return ""; // return empty string by default
	}
    // user
	static std::string get_user()
	{
	#ifdef DOKUN_WIN32 // ??
	    char username[UNLEN+1];
        DWORD username_len = UNLEN+1;
        if(GetUserName(username, &username_len) == 0) {
			std::cerr << "::GetUserName failed" << std::endl;
		    return ""; // return empty string if ::GetUserName fails
		}
		return std::string(username);
	#endif
	#ifdef __gnu_linux__ // works!
	    uid_t uid = geteuid();
		struct passwd * pw = getpwuid(uid);
        if(!pw) {
            std::cout << "::getpwuid failed. Trying ::getlogin" << std::endl;
			// try a different function if getpwuid() fails
			std::string username = getlogin();
			if(username.empty()) { 
			    std::cerr << "::getlogin failed" << std::endl; 
			    return ""; // return empty string if ::getlogin fails
			}
			return username;
			// or
			//char username_r[256];
            //getlogin_r(username_r, sizeof(username_r) - 1); // defined in unistd.h
	        //return username_r;
	        // refer to https://manpages.ubuntu.com/manpages/trusty/man3/getlogin.3posix.html
	    }
        return std::string(pw->pw_name);
	#endif
	    return ""; // return empty string by default
	}
	// visual
	static int get_display_count()
	{
	#ifdef DOKUN_WIN32
		return GetSystemMetrics(SM_CMONITORS);
	#endif
	#ifdef __gnu_linux__
	#endif
        return 0;
	}
	static int get_monitor_width()
	{
	#ifdef DOKUN_WIN32
		return GetSystemMetrics(SM_CXSCREEN); // 
	#endif
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
		Display* disp = XOpenDisplay(nullptr);
        Screen*  scrn = DefaultScreenOfDisplay(disp);
		XCloseDisplay(disp);
        return scrn->width;
	#endif	
	#endif
        return 0;
	}
	static int get_monitor_height()
	{
	#ifdef DOKUN_WIN32
	    return GetSystemMetrics(SM_CYSCREEN);
	#endif
	#ifdef __gnu_linux__
	#ifdef DOKUN_X11
	    Display* disp = XOpenDisplay(nullptr);
        Screen*  scrn = DefaultScreenOfDisplay(disp);
		XCloseDisplay(disp);
        return scrn->height;
	#endif	
	#endif		
        return 0;
	}
    static Vector2i get_monitor_size()
	{
		int width  = get_monitor_width ();
		int height = get_monitor_height();
		return Vector2i(width, height);
	}	
	// audio
	// mouse
	static bool is_mouse_installed()
	{
	#ifdef DOKUN_WIN32
	    return GetSystemMetrics(SM_MOUSEPRESENT);
	#endif
	#ifdef __gnu_linux__
	#endif	
        return false;
	}
	static int get_mouse_button_count()
	{
#ifdef DOKUN_WIN32
	    return GetSystemMetrics(SM_CMOUSEBUTTONS); // returns 0 if no mouse is installed
#endif
#ifdef __gnu_linux__
#endif
        return 0;
	}
	// keyboard
	// time
	static void get_time()
	{
	#ifdef DOKUN_WIN32
		/*
	SYSTEMTIME st;
GetSystemTime(&st);

WORD wYear;
WORD wMonth;
WORD wDayOfWeek;
WORD wDay;
WORD wHour;
WORD wMinute;
WORD wSecond;
WORD wMilliseconds;
	
	st.wSecond;
		*/
	#endif
	}
};
#endif
#endif
