#ifndef MATH_HPP_DOKUN
#define MATH_HPP_DOKUN // an extension of the lua math library
#define dokun_pi 3.14159265359
#include "vector.hpp"

#ifdef __cplusplus // if c++
#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <lua.hpp>

struct Math  { // helps with your math homework :)
	/////////////
	Math(void) {}
	/////////////
	~Math(void) {}
	/////////////
        // basic arithmetics here
	static double add(double x, double y){return (x + y);}
	static double sub(double x, double y){return (x - y);}	
	static double mul(double x, double y){return (x * y);}		
	static double div(double x, double y){return (x / y);}
    /////////////
    // rounding
    static int round10(double number) {
        float dec,numcpy;  int n;
        dec = number - floor(number);        
        n = floor(number);
        if(n % 10 < 5) {
            n = (n / 10) * 10;
        }
        else if(n % 10 == 5)
        {
            if(dec > 0) n = (((n + 10) / 10) * 10);
            else n = (n / 10) * 10;
        }
        else {
            n = (((n + 10) / 10) * 10);
        }
        //std::cout << "The rounded off number is: " << n << std::endl;
        return n;
    }
    /////////////	
	// algebra, linear algebra
	
    /////////////
	// geometry
	double area_of_square(double side)                       {return (side * side);}
	double area_of_rectangle(double length, double width)    {return (length * width);}
	double area_of_parallelogram(double base, double height) {return (base * height);}
	double area_of_triangle(double base, double height)      {return (1/2) * (base * height);}
	double area_of_circle(double r)
	{
		return (dokun_pi * r) * (dokun_pi * r); // radius
	}
	double area_of_trapezoid(double b1, double b2, double h)
	{
		double A = (1/2) * (b1 + b2) * h;
		return A;
	}
	double surface_area_of_sphere(double r)
	{
		double S = 4 * ((dokun_pi * r) * (dokun_pi * r));
		return S;
	}
	double surface_area_of_cube(double s)
	{
		double SA = 6 * (s * s); // s = any side
		return SA;
	}
	double surface_area_of_cylinder(double r, double h)
	{
		double SA = 2 * (dokun_pi * r * h); // r, h = radius, height
		return SA;
	}
	double perimeter_of_square(double side){return (4 * side);}
	double perimeter_of_rectangle(double side)
	{
		return (4 * side);
	}
	/*
	double perimeter_of()
	{}
	double perimeter_of()
	{}
	double perimeter_of()
	{}
	double perimeter_of()
	{}
	double perimeter_of()
	{}*/
	// type of angle
	static bool is_acute(double degrees)
	{
		return (degrees < 90);
	}
	static bool is_right(double degrees)
	{
		return (degrees == 90);
	}
	static bool is_obtuse(double degrees)
	{
		return (degrees > 90 && degrees < 180);
	}
	static bool is_straight_line(double degrees)
	{
		return (degrees == 180);
	}
	static bool is_reflex(double degrees)
	{
		return (degrees > 180);
	}	
	///////
	static bool is_pythagorean_triple(double a, double b, double c)
	{
		return (pow(a, 2) + pow(b, 2) == pow(c, 2));
	}
    /////////////
	// trigonometry
	
    /////////////
	// Calculus
    /////////////
	// misc
	static bool chance(double percent) // returns ?% chance to be true; range from 1 to 100
	{
		return (percent >= (rand() % 100) + 1); // 1 to 100
	}
    static bool chance50() // returns 50% chance to be true; range from 0 to 1
	{
		return ((rand() % 2) == 1); // 0 to 1
	}	
	static double random_generator(double minimum_range, double maximum_range) {
	    std::random_device rd; // Generating random numbers with C++11's random requires an engine and a distribution.
        std::mt19937 mt(rd()); // This is an engine based on the Mersenne Twister 19937 (64 bits):
        std::uniform_real_distribution<double> dist(minimum_range, maximum_range);
	    return dist(mt);
	    // or just use: std::default_random_engine generator(<some seed number>);
	}
	/////////////
	// pathfinding - A star (de facto), Dijkstra, Depth and Breadth-First 
	void a_star(){}
    /////////////
	// calculations and algorithms here
	static double distance(const Vector2& a, const Vector2& b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	} 
	static double distance(const Vector3& a, const Vector3& b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	}		
	/////////////	
	// Linear interpolation
    float lerp_imprecise(float v0, float v1, float t) {
        return v0 + t * (v1 - v0);
    }// Precise method, which does notguarantees v = v1 when t = 1.
    float lerp_percise(float v0, float v1, float t) { 
        return (1 - t) * v0 + t * v1;
    }	// Precise method, which guarantees v = v1 when t = 1.
	float lerp(float start, float end, float percent)
	{	
	    return ((end - start) * percent) + start;
	}
	/////////////	
        // conversions here
		double degrees_to_radian(double degrees) // converts degrees to radians
		{
			// limit degrees to 360 
		    //degrees = ((int)degrees + 360) % 360;
			return degrees * (dokun_pi/180);
		}
		double radian_to_degrees(double radians) // converts radians to degrees
		{
			return (radians * 57.2958);
		}
		// physics-related
		double pounds_to_kg(double pounds)
		{
			return (pounds / 2.2046);
		}
		static double percentage(double current, double maximum) // returns percentage to maximum
		{
            double n = current * 100;
            double d = maximum;
 
            double percent = n/d;			
			return percent;
		} // e.x  double player_current_exp_as_percentage = percentage(0, 14);
		
		double mass_to_weight(double mass) // returns weight in Newtons
		{
			return (mass * 9.8); // weight(N) = mass(kg) x gravity(9.8 m/s^2)
		}
		double weight_to_mass(double weight)
		{
			double mass = weight / 9.8; // mass(kg) = weight(N) / gravity(9.8 m/s^2)
			return mass;
		}
		double size_to_gravity(double mass)
		{
			double gravity = mass_to_weight(mass) / mass;
			return gravity;
		}
		double force(double mass, double acceleration) // mass should be in kg; acceleration in seconds returns force in Newtons
		{
			double force = mass * acceleration;
			return force;
		}
		// conversions time
		double milisecond_per_second(double second)
		{
			return second * 1000;
		}
		double second_per_minute(double minute)
		{
			return minute * 60;
		}
		double minute_per_hour(double hour)
		{
			return hour * 60;
		}
		double hour_per_day(double day)
		{
			return day * 24;
		}
		double day_per_week(double week)
		{
			return week * 7;
		}
		double week_per_month(double month)
		{
			return month * 4.34524;
		}
		double month_per_year(double year)
		{
			return year * 12;
		}
		/////////////
		
		// Lua functions here
		static int add(lua_State *L)
		{
			luaL_checktype(L, 1, LUA_TNUMBER);
			luaL_checktype(L, 2, LUA_TNUMBER);
			double x = lua_tonumber(L, 1);
			double y = lua_tonumber(L, 2);
			lua_pushnumber(L, x + y);
			return 1;
		}
		static int sub(lua_State *L)
		{
			luaL_checktype(L, 1, LUA_TNUMBER);
			luaL_checktype(L, 2, LUA_TNUMBER);
			double x = lua_tonumber(L, 1);
			double y = lua_tonumber(L, 2);
			lua_pushnumber(L, x - y);
			return 1;
		}
		static int mul(lua_State *L)
		{
			luaL_checktype(L, 1, LUA_TNUMBER);
			luaL_checktype(L, 2, LUA_TNUMBER);
			double x = lua_tonumber(L, 1);
			double y = lua_tonumber(L, 2);
			lua_pushnumber(L, x * y);
			return 1;
		}
		static int div(lua_State *L)
		{
			luaL_checktype(L, 1, LUA_TNUMBER);
			luaL_checktype(L, 2, LUA_TNUMBER);
			double x = lua_tonumber(L, 1);
			double y = lua_tonumber(L, 2);
			lua_pushnumber(L, x / y);
			return 1;
		}
		
		static int distance2d(lua_State * L)
		{
			luaL_checktype(L, 1, LUA_TNUMBER);
			luaL_checktype(L, 2, LUA_TNUMBER);
			luaL_checktype(L, 3, LUA_TNUMBER);
			luaL_checktype(L, 4, LUA_TNUMBER);
            double x0 = lua_tonumber(L, 1);
            double y0 = lua_tonumber(L, 2);	
            double x1 = lua_tonumber(L, 3);
            double y1 = lua_tonumber(L, 4);
			lua_pushnumber(L, Math::distance(Vector2(x0, y0), Vector2(x1, y1)));
            return 1;			
		}
		static int distance3d(lua_State * L)
		{
			luaL_checktype(L, 1, LUA_TNUMBER);
			luaL_checktype(L, 2, LUA_TNUMBER);
			luaL_checktype(L, 3, LUA_TNUMBER);
			luaL_checktype(L, 4, LUA_TNUMBER);
			luaL_checktype(L, 5, LUA_TNUMBER);
			luaL_checktype(L, 6, LUA_TNUMBER);			
            double x0 = lua_tonumber(L, 1);
            double y0 = lua_tonumber(L, 2);	
            double z0 = lua_tonumber(L, 3);	
            double x1 = lua_tonumber(L, 4);
            double y1 = lua_tonumber(L, 5);
            double z1 = lua_tonumber(L, 6);
			lua_pushnumber(L, Math::distance(Vector3(x0, y0, z0), Vector3(x1, y1, z1)));
            return 1;			
		}		
		// properties
		
};
#endif
#endif
