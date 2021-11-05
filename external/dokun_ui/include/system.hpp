#ifndef _SYSTEM
#define _SYSTEM

#include "platform.hpp"
#ifdef __cplusplus
class System
{
public:
    // directory
	static std::string get_current_dir()
	{
		std::string current;
    #ifdef __windows__
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
    // user
	static std::string get_user()
	{
	#ifdef __windows__ // ??
	    char username[UNLEN+1];
        DWORD username_len = UNLEN+1;
        if(GetUserName(username, &username_len) == 0)
			std::cerr << "Failure to get username" << std::endl;
		return std::string(username);
	#endif
	#ifdef __gnu_linux__ // works!
		/*register */struct passwd *pw;
        /*register */uid_t uid;
        uid = geteuid ();
        pw = getpwuid (uid);
        if (pw == 0)
			std::cerr << "Failure to get username" << std::endl;
        return std::string(pw->pw_name);
	#endif
	    return std::string("");
	}
	// visual
	static int get_display_count()
	{
	#ifdef __windows__
		return GetSystemMetrics(SM_CMONITORS);
	#endif
	#ifdef __gnu_linux__
	#endif
        return 0;
	}
	static int get_monitor_width()
	{
	#ifdef __windows__
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
	#ifdef __windows__
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
	#ifdef __windows__
	    return GetSystemMetrics(SM_MOUSEPRESENT);
	#endif
	#ifdef __gnu_linux__
	#endif	
        return false;
	}
	static int get_mouse_button_count()
	{
#ifdef __windows__
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
	#ifdef __windows__
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
