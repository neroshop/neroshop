#ifndef WIN32_HEADER_HPP_DOKUN
#define WIN32_HEADER_HPP_DOKUN

#ifdef __cplusplus // if c++
extern "C" {       // make c code compatible with c++
#endif
	#include <winsock2.h> 
	#include <ws2tcpip.h>

    #include <windows.h> 
	#include <Shlobj.h>
    #include <Shellapi.h>
    #include <Commdlg.h>
	
	#include <winerror.h> // added 9-2-2018
	
	#include <Lmcons.h> // to get username
#ifdef __cplusplus // if c++
}
#endif	
#endif
