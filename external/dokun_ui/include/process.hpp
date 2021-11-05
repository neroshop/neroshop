#ifndef _PROCESS
#define _PROCESS
#include "platform.hpp"
#include "utility.hpp"
#include "logger.hpp"
#include "factory.hpp"

#ifdef __cplusplus
#include <iostream>

class Process
{
public:
    Process();
	Process(const std::string& program, const std::string& argument);
	~Process();
	
    void * open();
	bool create(const std::string& program, const std::string& argument);
	bool terminate();
	static bool terminate(Process * process);//void kill(); // linux
	void exit(int code = 0);
	// getters
#ifdef _WIN32
	void * get_handle() const;
#endif
#ifdef __gnu_linux__
    int get_handle() const;
    static int get_process_by_name(const std::string& process_name);
#endif
	static void * get_active();
private:
#ifdef _WIN32
    void * handle;
#endif  
#ifdef __gnu_linux__
    int handle;
#endif  
}; // Process process("glue.exe", " srlua.exe test.lua test.exe");
#endif
#endif
