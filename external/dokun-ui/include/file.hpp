#ifndef FILE_HPP_DOKUN
#define FILE_HPP_DOKUN

#include "string.hpp"
#include "types.hpp"
#include "logger.hpp"
//#include "process.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class File { // file system
	public:
	    File();
	    File(const std::string&);
	    ~File();
		void cout(); // shows content of file in console window
	    void write(const std::string& text, int flag = DOKUN_IO_APPEND, bool binary = 0); // write to file
		void read(int flag = 0, bool binary = 0); // reads file line by line and then stores each line in cache
	    bool destroy();
		static bool destroy(const std::string& file_name); // delete a file from a directory
		void getline(std::string line); // will be equivalent to std::getline
	// setters
	void set_filter
	#ifdef DOKUN_WIN32
		(LPTSTR filter);
	#else
		(std::string filter);
	#endif	
	// getters
	std::string get_file(); // returns file_name
	std::string get_line(int index); // returns a line in the file
	std::string get_content(); // returns entire file as a string
	int get_count(); // number of lines in file
	int get_size(); // returns file size
	// file + folder name manipulation
	// boolean
	bool exists();
	static bool exists(const std::string& file_name);
	static bool is_directory(const std::string& path);
	bool is_open();
	// dialog + ui
	std::string dialog(int mode/*, const std::string& filter = ""*/); // open =0, save=1, print=2      // opens file dialog and returns the file_name that is opened or file_name that is saved
	// filename string manipulation
	static std::string extension          (const std::string& file_name); // ex. player.png => .png 
    static std::string no_extension       (const std::string& file_name); // ex. player.png => player
	static std::string directory_from_file(const std::string& path);      // ex. res/player.png => res    
	static std::string file_from_directory(const std::string& path);      // ex. res/player.png => player.png
	// directory
	static bool                     check_dir(const std::string& path);                      // returns true if a directory exists, else false
	static std::vector<std::string> get_dir  (const std::string& path, std::string filter = "*."); // returns a list of all filenames within a directory
	static bool make_dir                     (const std::string& path);                      // creates a new directory and returns true if it was a success
	static std::string get_current_dir        ();                                             // returns the current directory
	// properties
	private:
	    // file content
		std::string file;
		std::vector<std::string> line;
	// platform-specific
	#ifdef DOKUN_WIN32
		LPTSTR filter ;//= (LPTSTR)"All Files \0*.*\0\0";
	#else
		std::string filter;
	#endif	
};
#endif
#endif
/*
	std::vector<std::string> files = File::getdir("misc/res", "*.ogg");
	for(int i = 0; i < files.size(); i++)
	{
		std::cout << files[i] << "\n";
	}	
	
	std::string path = "res/sprite/player.png";
	std::cout << "full path with file: " << path << "\n";
	std::cout << "file_name: " <<    File::file_from_directory(path) << "\n";
	std::cout << "no extension: " << File::no_extension(path) << "\n";
	std::cout << "extension: " <<    File::extension(path) << "\n";
	std::cout << "directory: " <<    File::directory_from_file(path) << "\n";
*/
