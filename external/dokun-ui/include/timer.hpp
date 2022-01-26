#ifndef TIMER_HPP_DOKUN
#define TIMER_HPP_DOKUN

#include "platform.hpp"
#include "logger.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <ctime>
#include <cmath>
#include <chrono> // since C++11
#include <thread>
#include  <cstdint> // for int64_t, etc.
#include <lua.hpp> 

#ifndef MAX_TARGET_FPS
#define MAX_TARGET_FPS 60
#endif
#ifndef MIN_TARGET_FPS
#define MIN_TARGET_FPS 20
#endif

class Timer 
{
    public:
	    Timer(); static int new_(lua_State *L);
		~Timer();
		// normal functions
		void start();                 static int start(lua_State *L);// increment by second
		void stop();                  static int stop(lua_State *L);// stop incrementing
		void reset();                 static int reset(lua_State *L); // resets timer to default values
		double increment();           static int increment(lua_State *L); // returns increment between start and stop
		static void reset_s();        static int reset_s(lua_State *L);
		static void reset_e();        static int reset_e(lua_State *L);
		// delay
		static void delay(double nseconds); static int delay(lua_State *L);
		// increment a number by ..
		static void increment_by_second(double * value_ptr, int seconds = 1);
		static void increment_by_minute(double * value_ptr, int minutes = 1);
		static void increment_by_hour  (double * value_ptr, int hours   = 1);
		// setters
		// getters
		// for stopwatch objects ONLY - stop watch can only return msecs, secs, minutes, and hours
		double get_milliseconds();    static int get_milliseconds(lua_State *L);
		double get_seconds();         static int get_seconds(lua_State *L); // seconds since start()
		double get_minutes();         static int get_minutes(lua_State *L);
		double get_hours();           static int get_hours(lua_State *L);
		bool get_status();            static int get_status(lua_State *L);
		// get elapsed time in..
		static double milliseconds(); static int milliseconds(lua_State *L);// return current miliseconds since program started
		static double seconds();      static int seconds(lua_State *L);// return current seconds since program started
		static double minutes();      static int minutes(lua_State *L);
		static double hours();        static int hours(lua_State *L);
		static double days();         static int days(lua_State *L);
		static double weeks();        static int weeks(lua_State *L);
		static double months();       static int months(lua_State *L);
		static double years();        static int years(lua_State *L);
		// get delta time
		static double delta();        static int delta(lua_State *L); // return delta time
		// get date and time in a specific format
		static std::string get_format(std::string format="[%Y-%m-%d  %l:%M:%S %p]");
		// get frames per second
		static double framerate();    static int framerate(lua_State *L); // return frames per second
		static void set_target_framerate(double limit);
		static void set_interval(double interval); // report interval (how often the FPS is reported)
		// platform-specific functions
		static void   start_performance_counter();
		static double get_performance_counter  ();
	    static double get_performance_frequency();//(int64_t *frequency);
		static double get_tick_count           (void);	
		// boolean
	private:
	#ifdef DOKUN_WIN32
    double PCFreq = 0.0;
    __int64 CounterStart = 0;
	#endif
	#ifdef __gnu_linux__
	    int64_t start_tick, stop_tick;
		static const unsigned usec_per_sec = 1000000;
		static const unsigned usec_per_msec = 1000;
	#endif
	long long frequency; // or long long // or int64_t
	public:
	    // stopwatch time
		double millisecond;
	    double second;
	    double minute;
		double hour;
        static double counter;
		// delta
        static double delta_time;
		// status
		bool status; // status is 1 if time is being incremented, otherwise returns 0
		// frame rate
		static double frames;
	    static double start_time;
	    static double frame_rate;
		static double target_fps;
		static double report_interval;
///////////////////////////////// std::this_thread::sleep_for(std::chrono::seconds(1));
private:
    typedef std::chrono::high_resolution_clock clock_; // the type of clock we will be using // NOTE: std::chrono::high_resolution_clock is the de-factor timer class for games.
    typedef std::chrono::duration<double, std::ratio<1, 1000> > millisecond_; // represents duration in milliseconds // 1sec = 1000msec // same as std::chrono::duration<double, std::milli> millisecond_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;     // represents duration in seconds // 1sec // .count() returns double
    typedef std::chrono::duration<double, std::ratio<60, 1> > minute_; // represents duration in minutes // 60sec = 1min // A ratio of 60:1 with seconds // same as std::chrono::minutes
    typedef std::chrono::duration<double, std::ratio<3600, 1> > hour_; // represents duration in hours
    typedef std::chrono::duration<double, std::ratio<86400, 1> > day_;
    typedef std::chrono::duration<double, std::ratio<604800, 1> > week_;
    typedef std::chrono::duration<double, std::ratio<2629746, 1> > month_;
    typedef std::chrono::duration<double, std::ratio<31556952, 1> > year_;
    static std::chrono::time_point<clock_> beg_; // the time point from start to finish	- duration elapsed = clock::now() - start;
    // stopwatch
    std::chrono::time_point<std::chrono::high_resolution_clock> start_, end_;
    // delta 
    static std::chrono::time_point<std::chrono::high_resolution_clock> old_time, new_time;
};
/*
NOTE: std::chrono::high_resolution_clock is the de-factor timer class for games.


yocto	std::ratio<1, 1000000000000000000000000>, if std::intmax_t can represent the denominator
zepto	std::ratio<1, 1000000000000000000000>, if std::intmax_t can represent the denominator
atto	std::ratio<1, 1000000000000000000>
femto	std::ratio<1, 1000000000000000>
pico	std::ratio<1, 1000000000000>
nano	std::ratio<1, 1000000000>
micro	std::ratio<1, 1000000>
milli	std::ratio<1, 1000>
centi	std::ratio<1, 100>
deci	std::ratio<1, 10>
deca	std::ratio<10, 1>
hecto	std::ratio<100, 1>
kilo	std::ratio<1000, 1>
mega	std::ratio<1000000, 1>
giga	std::ratio<1000000000, 1>
tera	std::ratio<1000000000000, 1>
peta	std::ratio<1000000000000000, 1>
exa	std::ratio<1000000000000000000, 1>
zetta	std::ratio<1000000000000000000000, 1>, if std::intmax_t can represent the numerator
yotta	std::ratio<1000000000000000000000000, 1>



std::chrono::nanoseconds	duration<//signed integer type of at least 64 bits, std::nano>
std::chrono::microseconds	duration<//signed integer type of at least 55 bits, std::micro>
std::chrono::milliseconds	duration<//signed integer type of at least 45 bits, std::milli>
std::chrono::seconds	duration<//signed integer type of at least 35 bits>
std::chrono::minutes	duration<//signed integer type of at least 29 bits, std::ratio<60>>
std::chrono::hours	duration<//signed integer type of at least 23 bits, std::ratio<3600>>
std::chrono::days (since C++20)	duration<//signed integer type of at least 25 bits, std::ratio<86400>>
std::chrono::weeks (since C++20)	duration<//signed integer type of at least 22 bits, std::ratio<604800>>
std::chrono::months (since C++20)	duration<//signed integer type of at least 20 bits, std::ratio<2629746>>
std::chrono::years (since C++20)	duration<//signed integer type of at least 17 bits, std::ratio<31556952>>


Example usage:
    Timer * stopwatch = new Timer();
    stopwatch->start(); // start the timer
    model->load("res/monkey.obj");
    stopwatch->stop(); // stop the timer
    std::cout << "It took " << stopwatch->get_seconds() << " seconds to load the model." << std::endl;
    
Example 2 usage:    
        // outside loop
        Timer * frame_timer = new Timer();
        frame_timer->start();
        
        // run this code every 5 seconds
        // inside loop
        std::cout << "Frame timer: " << frame_timer->increment() << "s" << std::endl;
        if(frame_timer->increment() >= 5.01) 
        {
            std::cout << "Five seconds has passed" << std::endl;
            // reset timer if x seconds has passed
            frame_timer->reset();
            // and start the cycle all over again ...
            frame_timer->start();
        }
=====================================================================================================
Getting elapse time since the program started:    
    std::cout << "Milliseconds passed since dokun launched : " << Timer::milliseconds() << std::endl;
    std::cout << "Seconds passed since dokun launched      : " << Timer::seconds     () << std::endl;
    std::cout << "Minutes passed since dokun launched      : " << Timer::minutes     () << std::endl;
    std::cout << "Hours   passed since dokun launched      : " << Timer::hours       () << std::endl;
    Timer::reset_e(); // This resets ALL the elapsed time since the program opened
Getting frame rate:
    std::cout << "Framerate: " << Timer::framerate() << std::endl; // does not work for now

	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "I love you\n" << std::flush;;
	Timer::delay(5);
	std::cout << "No, I love you more\n";
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds to say love stuff\n";

performance_counter example usage:	
    Timer::start_performance_counter();
    Sleep(1000);
    cout << Timer::get_performance_counter() <<"\n";	
	
*/
#endif
#endif
