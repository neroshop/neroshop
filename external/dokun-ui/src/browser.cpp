#include "../include/browser.hpp"

#ifdef __cplusplus
Browser::Browser()
{}
////////////
Browser::~Browser()
{}
////////////
void Browser::open(const std::string& url) // Browser::open("https://stackoverflow.com");
{
#ifdef DOKUN_WIN32
	ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
#ifdef __gnu_linux__
	std::system(("xdg-open " + url).c_str());
#endif
}
////////////
#endif
