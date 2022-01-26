#include "../include/timer.hpp"

Timer::Timer() : millisecond(0), second(0), minute(0), hour(0), status(false)//,
    //start_tick(std::chrono::high_resolution_clock::now())//, stop_tick(std::chrono::high_resolution_clock::now())
{}
/////////////
Timer::~Timer(void)
{}
/////////////
double Timer::frames (0.0);
/////////////
double Timer::start_time (clock() / CLOCKS_PER_SEC);
/////////////
double Timer::frame_rate(0);
/////////////
double Timer::counter (0);
/////////////
/////////////
double Timer::target_fps (MAX_TARGET_FPS);
/////////////
double Timer::report_interval (0.1);
/////////////
/////////////
double Timer::delta_time(0);
/////////////
std::chrono::time_point<std::chrono::high_resolution_clock> Timer::old_time (std::chrono::high_resolution_clock::now());
std::chrono::time_point<std::chrono::high_resolution_clock> Timer::new_time (std::chrono::high_resolution_clock::now());
/////////////
std::chrono::time_point<std::chrono::high_resolution_clock> Timer::beg_(std::chrono::high_resolution_clock::now());
/////////////
void Timer::start()
{
    if(status) {/*std::cout << "Timer is already on." << std::endl;*/ return;}

	status = true; // turn the timer on  
    start_ = std::chrono::high_resolution_clock::now();
}
/////////////
int Timer::start(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->start();
	}		
	return 0;
}
/////////////
void Timer::stop()
{
    if(!status) {/*std::cout << "Timer is already off." << std::endl;*/ return;}
    
	end_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end_ - start_;
	// save the elapsed time in msecs, secs, mins, and hours
	millisecond = elapsed.count() * 1000;  // good
	second = elapsed.count(); // good
	minute = (second < 60  ) ? static_cast<int>(second / 60  ) : second / 60; // eh .. can't get decimal ONLY int
	hour   = (second < 3600) ? static_cast<int>(second / 3600) : second / 3600; // eh .. can't get decimal ONLY int
#ifdef DOKUN_DEBUG0	    
	std::cout << "It took " << millisecond << " milliseconds to execute.\n";
	std::cout << "It took " << second      << " seconds      to execute.\n"; // same as std::cout << "It took " << std::chrono::duration_cast<second_>(end_ - start_).count() << " seconds to load the model\n"; //same as std::cout << "It took " << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(end_ - start_).count() << " seconds to load the model\n";
	std::cout << "It took " << minute      << " minutes      to execute.\n";
	std::cout << "It took " << hour        << " hours        to execute.\n";
#endif
	status = false; // turn the timer off
}
/////////////
int Timer::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->stop();
	}		
	return 0;
}
/////////////
double Timer::increment() // realtime increment between start and stop (in seconds)
{
     // if timer is off, return the time it stopped (end)
    if(!status) { std::chrono::duration<double> elapsed = end_ - start_; return elapsed.count();}
    // as long as timer is on, get the increments between start and stop
    return std::chrono::duration_cast<second_>(std::chrono::high_resolution_clock::now() - start_).count();
}
/////////////
int Timer::increment(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		lua_pushnumber(L, timer->increment());
	}		
	lua_pushnumber(L, -1);
	return 1;
}
/////////////
void Timer::reset() // reset time
{
    millisecond = 0;
	second = 0;
	minute = 0;
	hour   = 0;
	status = false;
}
/////////////
int Timer::reset(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->reset();
		return 0;
	}
	return 0;
}
/////////////
void Timer::reset_s() // reset time
{}
/////////////
int reset_s(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->reset_s();
		return 0;
	}
	return 0;
}
//////////////
void Timer::reset_e() // resets ALL elapsed time (HOLY SHIT. THIS WORKS!!)
{
    beg_ = std::chrono::high_resolution_clock::now();
}
/////////////
int Timer::reset_e(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->reset_e();
		return 0;
	}
	return 0;
}
/////////////
void Timer::delay(double nseconds)
{
    std::this_thread::sleep_for (std::chrono::seconds((int)nseconds)); // std::this_thread::sleep_until(timePoint);
}
/////////////
int Timer::delay(lua_State *L)
{
    Timer::delay(lua_tonumber(L, -1));
    return 0;
}
/////////////
double Timer::get_milliseconds()
{
    return millisecond;
}
/////////////
int Timer::get_milliseconds(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		lua_pushnumber(L, timer->get_milliseconds());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Timer::get_seconds()
{
    return second;
}
/////////////
int Timer::get_seconds(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		lua_pushnumber(L, timer->get_seconds());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Timer::get_minutes()
{
	return minute;
}
/////////////
int Timer::get_minutes(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		lua_pushnumber(L, timer->get_minutes());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Timer::get_hours()
{
	return hour;
}
/////////////
int Timer::get_hours(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		lua_pushnumber(L, timer->get_hours());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
/////////////
bool Timer::get_status()
{
	return status;
}
/////////////
int Timer::get_status(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		lua_pushboolean(L, timer->get_status());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Timer::seconds() // confirmed! 2019-08-03 - get seconds since program started (reset with Timer::reset_e)
{
    return std::chrono::duration_cast<second_> (std::chrono::high_resolution_clock::now() - beg_).count();    
}
int Timer::seconds(lua_State *L)
{
	lua_pushnumber(L, Timer::seconds());
	return 1;
}
/////////////
double Timer::milliseconds() // confirmed! 2019-08-03
{
	return std::chrono::duration_cast<millisecond_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}
int Timer::milliseconds(lua_State *L)
{
	lua_pushnumber(L, Timer::milliseconds());
	return 1;
}
/////////////
double Timer::minutes() // confirmed! 2019-08-03 
{
	return std::chrono::duration_cast<minute_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}
int Timer::minutes(lua_State *L)
{
	lua_pushnumber(L, Timer::minutes());
	return 0;
}
/////////////
double Timer::hours() // confirmed! 2019-08-03
{
	return std::chrono::duration_cast<hour_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}        
/////////////
int Timer::hours(lua_State *L)
{
	lua_pushnumber(L, Timer::hours());
	return 0;
}
/////////////
double Timer::days() // Not yet tested :o but 99% sure it works :D
{
	return std::chrono::duration_cast<day_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}
int Timer::days(lua_State *L)
{
	lua_pushnumber(L, Timer::days());
	return 0;
}
/////////////
double Timer::weeks() // Not yet tested :o but 99% sure it works :D
{
    if(seconds() < 604800) return 0;
	return std::chrono::duration_cast<week_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}
int Timer::weeks(lua_State *L)
{
	lua_pushnumber(L, Timer::weeks());
	return 0;
}
/////////////
double Timer::months() // Not yet tested :o but 99% sure it works :D
{
    if(seconds() < 2629746) return 0;
	return std::chrono::duration_cast<month_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}
int Timer::months(lua_State *L)
{
	lua_pushnumber(L, Timer::months());
	return 0;
}
/////////////
double Timer::years() // Not yet tested :o but 99% sure it works :D
{
    if(seconds() < 31556952) return 0;
	return std::chrono::duration_cast<year_>  (std::chrono::high_resolution_clock::now() - beg_).count();
}
int Timer::years(lua_State *L)
{
	lua_pushnumber(L, Timer::years());
	return 0;
}
/////////////
double Timer::delta() // get delta time - not sure how to implement it
{
	old_time = new_time; 
	new_time = std::chrono::high_resolution_clock::now();
    delta_time = ((std::chrono::duration_cast<millisecond_>(new_time - old_time).count()) / 1000 /* * 1000 */); // / PERFORMANCE_FREQUENCY(); // delta = new_time - old_time
#ifdef DOKUN_DEBUG    
    std::cout << "delta_time: " << delta_time << std::endl;
#endif    
    return 1.0 / delta_time; 
}
///////////// use case: position += speed * delta_time.
int Timer::delta(lua_State *L)
{
	lua_pushnumber(L, Timer::delta());
	return 1;
}
/////////////
/////////////
std::string Timer::get_format(std::string format) 
{
	auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), format.c_str());
	return ss.str();
}
/////////////
/////////////
void Timer::start_performance_counter() // source: https://gist.github.com/ikhramts/5798661 https://stackoverflow.com/questions/1739259/how-to-use-queryperformancecounter
{
#ifdef DOKUN_WIN32
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li)) {std::cout << "QueryPerformanceFrequency failed!" << std::endl; return;} // get ticks per second

    PCFreq = double(li.QuadPart)/1000.0; // in msecs. If you want secs then change to: PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
#endif
#ifdef __gnu_linux__
#endif
}
/////////////
double Timer::get_performance_counter() 
{
#ifdef DOKUN_WIN32 // source: https://gist.github.com/ikhramts/5798661 https://stackoverflow.com/questions/1739259/how-to-use-queryperformancecounter
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
#endif
#ifdef __gnu_linux__
#endif
    return 0;
}
/////////////
double Timer::get_performance_frequency() // long long is more portable than int64_t. They are pretty much the same too.
{
#ifdef DOKUN_WIN32
    return PCFreq; // This is all you need to get the performance frequency // QueryPerformanceCounter((LARGE_INTEGER *)&frequency)
#endif
#ifdef __gnu_linux__
#endif
    return 0;
}
/////////////
double Timer::get_tick_count(void) 
{
#ifdef DOKUN_WIN32
    return timeGetTime(); // or GetTickCount(); - The return value is the number of milliseconds that have elapsed since the system was started // or GetTickCount64 // or timeGetTime(); - Returns the system time, in milliseconds. //  timeGetTime is a better choice if you want to calculate fps for your game
#endif
#ifdef __gnu_linux__
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC_RAW, &ts) != 0) // CLOCK_MONOTONIC_RAW is not subject to NTP adjustments and is newer, CLOCK_MONOTONIC, CLOCK_REALTIME // MONOTONIC is the best option If you want to compute the elapsed time between two events observed on the one machine without an intervening reboot 
        return 0;
    return ts.tv_sec * 1000.0 + (ts.tv_nsec / 1000000.0);
#endif
}
/////////////
double Timer::framerate() 
{
	frames = frames + 1; // increase counter by 1
	
	if((Timer::seconds() - start_time) > 1.0)
	{
		frame_rate = frames / (Timer::seconds() - start_time);
		start_time = Timer::seconds(); // reset
		frames = 0;                    // reset
	}
	return (frame_rate);
}
/////////////
int Timer::framerate(lua_State *L)
{
	lua_pushnumber(L, Timer::framerate());
	return 1;
}
/////////////
void Timer::set_target_framerate(double limit)
{
	target_fps = limit;
	if(limit > MAX_TARGET_FPS) target_fps = MAX_TARGET_FPS;
	if(limit < MIN_TARGET_FPS) target_fps = MIN_TARGET_FPS;
}

void Timer::set_interval(double interval)
{
	report_interval = interval;
}
/////////////
void Timer::increment_by_second(double * value_ptr, int seconds)
{
    //std::intmax_t copyarg2 (seconds);
    //std::chrono::duration< double, std::ratio<copyarg2, 1> > nsec; // every nsecs
    //*value_ptr = std::chrono::duration_cast<nsec>(std::chrono::high_resolution_clock::now() - nsec).count();
}
/////////////
void Timer::increment_by_minute(double * value_ptr, int minutes)
{}
/////////////
void Timer::increment_by_hour(double * value_ptr, int hours)
{}
/////////////
/////////////
/////////////
/////////////
/////////////
int Timer::new_(lua_State *L)
{
    lua_settop(L, 0); // clear unnecessary arguments
	// create table
	lua_createtable(L, 0, 0);
	// set mt
	lua_getglobal(L, "Timer");
	lua_setmetatable(L, 1);
	// set userdata
	Timer **timer = static_cast<Timer**>(lua_newuserdata(L, sizeof(Timer*)));
	*timer = new Timer();
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1)) // top arg is table 
	    return 1;
	lua_pushnil(L);
	return 1;
}
/////////////
/////////////
/*


wait function:
    double ms = 0; 
    luaL_checknumber(L, 1); 
    ms = lua_tonumber(L, 1);
  
    clock_t start_time_time = clock();
    clock_t end_time = ms * 1000 + start_time_time;
    while(clock() != end_time) {}
	
    return 0;
*/	
