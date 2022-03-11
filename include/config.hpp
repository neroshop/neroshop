#ifndef CONFIG_HPP_NEROSHOP//DATABASE_LUA_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define CONFIG_HPP_NEROSHOP//DATABASE_LUA_HPP_NEROSHOP
#define LUA_TAG "\033[1;34m[lua]:\033[0m "

#include "debug.hpp"
#include <script.hpp>

//////////////////////////////
//extern lua_State * lua_state;// (luaL_newstate());//static lua_State * lua_state;// (luaL_newstate());//(nullptr);//should I use extern ?//because its in a namespace, there may not be a need to use extern // extern is for declaration in .h and then defined in a .cpp file
//////////////////////////////
namespace neroshop {
//namespace DB {
//class Lua {
//public:
    //////////////////////////////
    extern lua_State * lua_state;// (luaL_newstate());//static lua_State * lua_state;// (luaL_newstate());//(nullptr);//should I use extern ?//because its in a namespace, there may not be a need to use extern // extern is for declaration in .h and then defined in a .cpp file
    //////////////////////////////
	static bool load_config() {
        std::string user = System::get_user();
        // "/home/<user>/.config/neroshop"
        std::string neroshop_config_path = "/home/" + user + "/.config/neroshop";
        // "/home/<user>/.config/neroshop/config.lua"
        std::string neroshop_config_name = neroshop_config_path + "/config.lua";
        Script script;
        if(!script.load(lua_state, neroshop_config_name)) {return false;}
        NEROSHOP_TAG_OUT std::cout << "\033[1;94;49m" << "loaded script \"" << script.get_file() << "\"" << "\033[0m" << std::endl;
        return true; // default return-value
	}
	//////////////////////////////
	static bool create_config() {
        std::string user = System::get_user();
        std::string text = "-- config.lua\n"
        "localhost = \"127.0.0.1\"\n"
        "neroshop = {\n"
        //"    --general = {\n"
        "    currency = \"usd\",\n" // currency that will be displayed alongside xmr: 0.045000000000 XMR ($10 USD)
        //"    theme = \"dark\",\n" // dark, light, default
        //"    --}\n"
        "    window = {\n"
        "        width = 1280,\n"
        "        height = 720,\n"
        //"        x = 200,\n"
        //"        y = 200,\n"
        "        mode = 0, -- 0 = window_mode or 1 = fullscreen\n"
        "    },\n"
        "    daemon = {\n"
        "        data_dir = \"/home/user/.bitmonero\", -- /path/to/lmdb\n" // change this
        "        network_type = \"stagenet\",\n"
        "        ip = localhost, --\"0.0.0.0\" means that other wallets can connect to your node\n"
        "        port = \"38081\",\n" // 127.0.0.1:38081
        "        confirm_external_bind = false, -- set to true if binding to 0.0.0.0 -- confirms you want to allow connections from wallets that are not on this system\n"
        "        restricted_rpc = true, -- restricts what information can be requested from the daemon by any client\n"//"        daemon_address = \"\",--\"node.moneroworld.com:18089\", -- connect to a remote node (in case you are unable to sync the entire blockchain)\n"
        "        remote = false, -- set this true to connect to a remote node\n"
        "    },\n"
        "    wallet = {\n"
        "        file = \"\", -- include '.keys' ext\n"
        "        restore_height = 0, -- block height or date (YYYY-MM-DD)\n"    
        "    },\n"
        "}\n";
        //"    \n"
        // swap data_dir with user
    #ifdef __gnu_linux__ // works!    
        text = String::swap_first_of(text, "/home/user/.bitmonero", ("/home/" + user + "/.bitmonero"));
    #endif    
        // "/home/<user>/.config/neroshop"
        std::string neroshop_config_path = "/home/" + user + "/.config/neroshop"; // System::get_user()
        // "/home/<user>/.config/neroshop/config.lua"
        std::string neroshop_config_name = neroshop_config_path + "/config.lua";
        // if file already exists, no need to create it again
        if(File::exists(neroshop_config_name)) return false; // false because it will not be created
        // check if script works before saving
        if(luaL_dostring(lua_state, text.c_str()) != 0) {
		    NEROSHOP_TAG_OUT std::cout << LUA_TAG << "\033[0;31m" << "invalid Lua code" << "\033[0m" << std::endl;
		    lua_error(lua_state);
		    return false; // exit function so it does not save text
	    }
        // if path does not exist
        if(!File::is_directory(neroshop_config_path)) 
        {   // create the path
            NEROSHOP_TAG_OUT std::cout << "\033[0;33m" << "path \"" << neroshop_config_path << "\" does not exist, but we'll create it for you :)" << "\033[0m" << std::endl;
            if(!File::make_dir(neroshop_config_path)) {NEROSHOP_TAG_OUT std::cout << "\033[0;31m" << "failed to make the path :(" << "\033[0m" << std::endl; return false;}
            NEROSHOP_TAG_OUT std::cout << "\033[1;97;49m" << "created path \"" << neroshop_config_path << "\"" << "\033[0m" << std::endl;
        }
        // if path exists, but the file is missing or deleted
        if(!File::exists(neroshop_config_name)) {
            // create config file (one time)
            std::ofstream cfg;
            cfg.open (neroshop_config_name, std::ios::out | std::ios::trunc);
            cfg << text << "\n"; // write to file
            cfg.close();
            NEROSHOP_TAG_OUT std::cout << "\033[1;97;49m" << "created file \"" << neroshop_config_name << "\"" << "\033[0m" << std::endl;  
        }
        return true;		
	}
	//////////////////////////////
	//static void edit_config(const std::string& old_str, const std::string& new_str); // not possible to edit lua files with my current knowledge
	//////////////////////////////
	static lua_State * get_lua_state() {
	    return lua_state;
	}
//};
//}
}

#endif
