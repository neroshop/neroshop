#ifndef LINUX_HEADER_HPP_DOKUN
#define LINUX_HEADER_HPP_DOKUN

#ifdef __cplusplus // if c++
extern "C" {       // make c code compatible with c++
#endif
    #include <sys/types.h>
	#include <fcntl.h>    /* For O_RDWR */
    #include <unistd.h>   /* For open(), creat() */
	#include <pwd.h>
	#include <termios.h>
	//#include <linux/input.h>
	#include <sys/time.h> // gettimeofday, settimeofday - get / set time
	
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	
	#include <dlfcn.h>
	#include <sys/stat.h> // mkdir, process
	#include <signal.h>
	#include <dirent.h> // opendir
	#include <strings.h>
	// for all input event codes
	#include <linux/input-event-codes.h>
	// for device
	#include <sys/ioctl.h>
#ifdef __cplusplus // if c++
}
#endif	
#endif
