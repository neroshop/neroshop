#include "../include/file.hpp"

File::File() // The file system is only good for opening dialogs :p
{}
/////////////
File::File(const std::string& file_name)
{}
/////////////
File::~File()
{}
/////////////
void File::cout() // shows content of file
{
	if(!is_open()) {
		dokun::Logger("File is not opened");
		return;
	}
	std::ifstream file(get_file().c_str());
	
	if(!file.is_open())
	{
		dokun::Logger("Could not display content in " + get_file());
		return;
	}
	
	std::stringstream stream ;
	stream << file.rdbuf   (); // dump file contents		
    std::cout << stream.str();
	
	file.close();
}
/////////////
void File::write(const std::string& text, int flag, bool binary) // write to file
{
	if(!is_open()) {
		dokun::Logger("File is not opened");
		return;
	}	
	std::ios_base::openmode mode;
	switch(flag)
	{
		case DOKUN_IO_APPEND:
		    mode = std::ios::app  ;
		    break;
		case DOKUN_IO_TRUNCATE:
		    mode = std::ios::trunc;
		    break;
	}
	switch(static_cast<int>(binary))
	{
		case 1:
		    mode = mode | std::ios::binary;
		    break;
	}
    std::ofstream file(get_file().c_str(), mode);
	if(!file.is_open())
	{
		dokun::Logger("Could not write to " + get_file());
		return;
	}
	file << text << std::endl;	
	file.close();
}
/////////////
void File::read(int flag, bool binary)
{
	if(!is_open()) {
		dokun::Logger("File is not opened");
		return;
	}	
	std::ios_base::openmode  mode  ;
	switch(flag)
	{
		case DOKUN_IO_AT_END:
		    mode = std::ios::ate;
		    break;
	}
	switch(static_cast<int>(binary))
	{
		case 1:
		    mode = mode | std::ios::binary;
		    break;
	}
	std::ifstream file(get_file().c_str(), mode);
	if(!file.is_open())
	{
		dokun::Logger("Could not read from " + get_file());
		return;
	}
	std::string line;
	while(std::getline(file, line))
	{
		(this)->line.push_back(line); // store each line
	}
	file.close();	
}
/////////////
bool File::destroy()
{
	return destroy(get_file());
}
/////////////
bool File::destroy(const std::string& file_name)
{
	if(std::remove(file_name.c_str()) != 0)
        return false;

	return true; // does not exist	
}	
/////////////
std::string File::dialog(int mode)
{
	// "All Files \0*.lua*\0\0"
	/*
	std::string format_name = "All Files";
	std::string token_1 = " \\0";
	std::string format = "*.txt*";
	std::string token_2 = "\\0\\0";
	
	const char * p = (format_name + token_1 + format + token_2).c_str();
	
	static_cast<std::string>(filter) = (format_name + token_1 + format + token_2);
	std::cout << filter << "\n";
	*/
#ifdef DOKUN_WIN32	
	std::string file_name;
	filter = "All Files \0*.*\0\0";
	if(mode == 0) // open mode
	{
        OPENFILENAME ofn;
        char szFileName[256] = "";
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = 256;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;
        ofn.lpstrDefExt = "txt";
        // no file was opened (cancelled or exited without selecting file)
	    if(!GetOpenFileName(&ofn))
			return ("");
		return file_name.append(static_cast<std::string>(ofn.lpstrFile));
	}
    if(mode == 1) // save mode
	{
	    OPENFILENAME ofn;
        char szFileName[256] = "";
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = 256;
        ofn.Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;
		ofn.lpstrDefExt = "txt"; 
	    if(! GetSaveFileName(&ofn))
			return ("");
		return file_name.append(static_cast<std::string>(ofn.lpstrFile));	
	}		
	if(mode == 2) // print
	{}
    if(mode == 3)
	{

		TCHAR path[MAX_PATH];
    //const char * path_param = saved_path.c_str();
    BROWSEINFO bi = { 0 };
    bi.lpszTitle  = ("Browse for folder...");
    bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn       = NULL;
    //bi.lParam     = (LPARAM) path_param;
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        //get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, path );
        //free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
        return path;
	}
	
	}
#endif
#ifdef __gnu_linux__
    char file[1024];
    FILE *f = popen("zenity --file-selection", "r");
    fgets(file, 1024, f);
	return file;
#endif	
	return "";
}
/////////////
/////////////
/////////////
/////////////
// GETTERS
/////////////
std::string File::get_file()
{
	return file;
}
/////////////
std::string File::get_line(int index)
{
	if(line.empty())
	{
		dokun::Logger("File is empty");
		return "";
	}
	return line[index];
}
/////////////
std::string File::get_content()
{
	if(line.empty())
	{
		dokun::Logger("File is empty");
		return ("");
	}
	std::string content;
	for(unsigned int i = 0; i < line.size(); i++)
	{
		content = content + line[i];
	}
	return content;
}
/////////////
int File::get_count()
{
	if(get_file().empty())
	{
		std::cerr << "File is not opened" << std::endl;
		return (-1);
	}
    std::ifstream file (get_file().c_str());   
    std::size_t lines = 0;
    std::string line;
	if(file.is_open())
	{
        while (std::getline(file , line))
            ++lines;	
	
	    file.close();
    }
	return lines;
}
/////////////
int File::get_size() // not complete
{
	if(get_file().empty())
	{
		std::cerr << "File is not opened" << std::endl;
		return (-1);
	}	
	std::ifstream file (get_file().c_str(), std::ios::binary | std::ios::ate); 
	if(!file.is_open())
	{
		std::cerr << "Could not get file size" << std::endl;
		return (-1);
	}
	std::streamoff size = file.tellg();
	file.close();
	return size;
} 
/////////////
/////////////
// BOOLEAN
/////////////
bool File::exists()
{
	return File::exists(get_file());
}
/////////////
bool File::exists(const std::string& file_name)
{
	std::ifstream file(file_name.c_str());
	if(!file.good())
	{
		file.close();
		return false;
	}
	return true;	
}
/////////////
bool File::is_open()
{
	return (!get_file().empty()); // non-empty file (proof that the file is opened)
}
/////////////
/////////////
bool File::is_directory(const std::string& path)
{
    return check_dir(path);
}
/////////////
/////////////
/////////////	
// FILENAME FUNCTIONS
/////////////	
std::string File::directory_from_file(const std::string& path)      // ex. res/player.png => res           gets path from path+filename    if no directory (no dashes, returns plain file)
{
	std::string folder = path.substr(0, path.find_last_of("\\/"));	// works for both foward and back slashes
	return folder;
}	
/////////////
std::string File::file_from_directory(const std::string& path) // ex. res/player.png => player.png    gets filename from path
{
	std::string file =  path.substr(path.find_last_of("\\/") + 1); // works for both foward and back slashes
	return file;
}
/////////////
std::string File::no_extension(const std::string& file_name) // ex. player.png => player    get filename without ext
{
	std::string no_extension = file_name.substr(0, file_name.find_last_of("."));
	return no_extension;
}
/////////////
std::string File::extension(const std::string& file_name) // ex. player.png => .png         get ext from filename
{
	std::string extension = file_name.substr(file_name.find_last_of(".") + 1);
	return extension;
}
/////////////
/////////////	
// DIRECTORY FUNCTIONS
/////////////
bool File::check_dir(const std::string& path)
{
#ifdef DOKUN_WIN32
	DWORD dwAttrib = GetFileAttributes(path.c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
	    (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#endif
#ifdef __gnu_linux__
    DIR * dir = opendir(path.c_str());
    if (dir)
    {
        closedir(dir);
	    return true;
    }		
#endif
	return false;		
}
/////////////	
std::vector<std::string> File::get_dir  (const std::string& path, std::string filter)
{
	std::vector<std::string> file;
#ifdef DOKUN_WIN32
	filter = "%s/" + filter;
	char search_path[200];
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    sprintf_s(search_path, 200, filter.c_str(), path.c_str());
#else
    sprintf(search_path, filter.c_str(), path.c_str());
#endif
	WIN32_FIND_DATA fd; 
    HANDLE hFind = FindFirstFile(search_path, &fd); 
    if(hFind != INVALID_HANDLE_VALUE) 
	{
        do 
		{ 
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
			{
			    // store module names
                file.push_back(fd.cFileName);
            } 
        } while(FindNextFile(hFind, &fd)); 
        FindClose(hFind); 
    } 
#endif
#ifdef __gnu_linux__
	DIR *dir = opendir (path.c_str());
    struct dirent * ent;
    if (dir != nullptr) 
	{
        while ((ent = readdir (dir)) != nullptr)   /* print all the files and directories within directory */
        {
		#ifdef DOKUN_DEBUG0
            std::cout << ent->d_name << std::endl;
		#endif
		    if(!String::contains(ent->d_name, "." + String::remove_all(File::extension(filter), "*"))) // if filename does not contain the extenstion from filter
				continue;
			file.push_back(ent->d_name);
        }
        closedir (dir);
    }
	#endif
    return file;		
}
/////////////	
std::string File::get_current_dir        ()
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
/////////////	
bool File::make_dir                     (const std::string& path)
{
#ifdef DOKUN_WIN32
	return (CreateDirectory  (path.c_str(), nullptr) != 0);
#endif
#ifdef __gnu_linux__
	return (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
#endif	
    return false;	
} 	
/////////////	
/////////////	
/////////////	
/////////////		
/////////////		
/////////////		
