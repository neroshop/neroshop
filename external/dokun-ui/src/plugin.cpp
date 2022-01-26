#include "../include/plugin.hpp"

Plugin::Plugin()
{}
/////////////
Plugin::Plugin(const std::string& file_name)
{
	if(!load(file_name))
	{
		dokun::Logger("Could not load module " + file_name);
	}
}
/////////////
Plugin::~Plugin()
{
	free();
}
/////////////
bool Plugin::load(const std::string& file_name)
{
	#ifdef DOKUN_WIN32
		module = LoadLibrary(static_cast<LPCTSTR>(file_name.c_str()));
	#endif
	#ifdef __gnu_linux__
	    module = dlopen(file_name.c_str(), RTLD_LAZY);
	#endif
	return (module != nullptr);	
} 
/////////////
bool Plugin::load_directory(std::string folder, std::string filter)
{
	std::vector<std::string> module_list = File::get_dir(folder, filter);
	for(int i = 0; i < module_list.size(); i++)
	{
		#ifdef DOKUN_DEBUG	
	        std::cout << module_list[i] << std::endl;
		#endif		
		if(load(folder + "/" + module_list[i]))
		{
			return true; // all modules were able to load
		}
	}	
	return false; // at least one or more modules did not load
} 
/////////////
void Plugin::free()
{
	#ifdef DOKUN_WIN32
        FreeLibrary(module);
    #endif	
	#ifdef __gnu_linux__
	    dlclose(module);
	#endif
}
/////////////
/////////////
void * Plugin::get(const std::string& data)
{
	#ifdef DOKUN_WIN32
		return reinterpret_cast<void *>(GetProcAddress(module, static_cast<LPCTSTR>(data.c_str())));	
	#endif
	#ifdef __gnu_linux__
	    return dlsym(module, data.c_str());
	#endif
	return nullptr;
}
/////////////
/* Usage:
    Plugin * plugin = new Plugin();
	
	for(unsigned int i = 0; i < plugin->get_all_file_names_within_folder("bin").size(); i++)
	{
	    std::cout << plugin->get_all_file_names_within_folder("bin")[i] << "\n";
	}
	// plugin->load("dokun_plugin.dll"); // load a plugin
	// plugin->load_directory("Plugins", "*.dll*"); // load all '.dll' files from folder 'Plugins'
*/
/////////////
/////////////
