#ifndef VECTOR_HPP_DOKUN
#define VECTOR_HPP_DOKUN

#ifdef __cplusplus // if c++
#include <iostream>
#include <cmath>
#include <lua.hpp>

struct Vector2 {
	//////////////////
	// Constructor(s)
    Vector2() : x(0.0), y(0.0) {}
	//////////////////
	Vector2(double number)
	{
		x = number;
		y = number;
	}
	//////////////////
    Vector2(double x, double y) 
    {
	    this->x = x;
	    this->y = y;
    }
	//////////////////
	Vector2(const Vector2& vector)
	{
		x = vector.x;
		y = vector.y;
	}
	//////////////////
	// Destructor
	~Vector2(void){}
	//////////////////
	void set(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	//////////////////
	void set(const Vector2& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
	}
	//////////////////
	Vector2 get()
	{
		return (*this);
	}
	//////////////////
	bool empty() const
	{
		return (x == 0 && y == 0);
	}
	//////////////////
	double length() const
	{
		return sqrt(x * x + y * y);
	}
	//////////////////
	static double length(Vector2& vector)
	{
		return vector.length();
	}	
	//////////////////
	Vector2 radians() const
	{
		return Vector2(x * (3.14/180), y * (3.14/180));
	}
	//////////////////
    Vector2 normalize() 
	{
		double len = length();
		if(len != 0) 
		{
			x = x / len;
			y = y / len;
		}
		return (*this);
	}
    //////////////////
    Vector2 normal()
	{
		Vector2 vector;
		if(length() != 0)
		{
		    vector.x = x / length();
		    vector.y = y / length();
		}
		return vector;
	}	
	//////////////////
	double dot(const Vector2 &vector) const
	{
		return (x * vector.x) + (y * vector.y);
	}
	//////////////////
	double dot(double x, double y) 
	{
		return (this->x * x) + (this->y * y);
	}
	//////////////////
	double cross(const Vector2 &vector) 
	{
		return (x * vector.y) - (y * vector.x);  
	}
	//////////////////
	// Transformations
	void translate(double x, double y) // tested!
	{
		this->x = this->x + (x);
		this->y = this->y + (y);  
	}
	static Vector2 translate(Vector2 &vector, double x, double y){vector.translate(x, y);return vector;}
	//////////////////
	void rotate(double degrees) // tested!
	{
		// limit degrees to 360 
		degrees = ((int)degrees + 360) % 360; 
		// convert degrees to radians
		double angle = (double)degrees * (3.14 / 180); 	
		// calculate rotation
		double x = this->x * cos(angle) - this->y * sin(angle); 
		double y = this->x * sin(angle) + this->y * cos(angle);
		// apply rotation
		this->x = (x);
		this->y = (y); 
	}
	static Vector2 rotate(Vector2 &vector, double degrees){vector.rotate(degrees);return vector;}
	//////////////////
	void scale(double sx, double sy) // tested!
	{
		this->x = this->x * (sx);
		this->y = this->y * (sy);
	} 
	static Vector2 scale(Vector2 &vector, double sx, double sy) {vector.scale(sx, sy);return vector;}
	//////////////////
	Vector2 shear(double shx, double shy)  // double tested!
	{
			// calculate shear
			if(shx > 0)
			double x = this->x + (shx * this->y);
            if(shy > 0)
			double y = this->x * (shy + this->y); // just the opposite of shearing x			
			// apply shear
			this->x = (x);
			this->y = (y);		
			// return 
			return (*this);
	} 
	static Vector2 shear(Vector2 &vector, double shx, double shy) {vector.shear(shx, shy);return vector;}
	//////////////////
	Vector2 reflect(int x, int y)
	{
		if(x)
		{
		    scale(-1, 1);
	    }
		if(y)
		{
		    scale(1, -1);
		}
		return (*this);
	}
	//////////////////
	// Lua functions
	static int Vector_new(lua_State *L) // Vector2(0, 0, 0)
	{
	    luaL_checktype(L, 1, LUA_TNUMBER);
		luaL_checktype(L, 2, LUA_TNUMBER);
		return 2;
	} 
	//////////////////
    static int new_(lua_State *L) // vector = Vector:new(0, 0, 0)
	{
		double x = 0; 
        double y = 0;
		bool number_set = false ;
		if
		(
		    lua_type(L, -2) == LUA_TNUMBER && 
		    lua_type(L, -1) == LUA_TNUMBER
		)
		{
			x = lua_tonumber(L, -2);
			y = lua_tonumber(L, -1); 
			number_set = true ;
		}
		lua_settop(L, 0); // clear stack
		// create table
		lua_createtable(L, 0, 0);
		// set mt
		lua_getglobal(L, "Vector_mt");
		lua_setmetatable(L, 1);
		// set udata
		Vector2 ** vector = (Vector2 **)lua_newuserdata(L, sizeof(Vector2));
		if ( !number_set )
			*vector = new Vector2();
		else
			*vector = new Vector2(x, y);
		lua_setfield(L, 1, "udata");
		// set values
		lua_pushnumber(L, x);
		lua_setfield(L, 1, "x");
		lua_pushnumber(L, y);
		lua_setfield(L, 1, "y");
		// return table
		if(lua_istable(L, -1))
			return 1;
		lua_pushnil(L);
		    return 1;
	}
	//////////////////
	static int length(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector2 * vector = *(Vector2 **)lua_touserdata(L, -1);
		    lua_pushnumber(L, vector->length());
            return 1;				
		}
		lua_pushnil(L);
		return 1;
	}
    //////////////////	
	static int normalize(lua_State *L) // returns the normalized values of a vector
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Vector2 * vector = *(Vector2 **)lua_touserdata(L, -1);
			
		    lua_pushnumber(L, vector -> normalize ().x);
		    lua_pushnumber(L, vector -> normalize ().y);
		    return 2 ;
		}
		lua_pushnil(L);
		return 1;
	}
	//////////////////
	static int dot(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
        lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector2 * vector_2 = *(Vector2 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector_1 = *(Vector2 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->dot(*vector_2));
                return 1;
            }				
		}
		lua_pushnil(L);
		return 1;
	}		
	//////////////////
	static int cross(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
        lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector2 * vector_2 = *(Vector2 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector_1 = *(Vector2 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->cross(*vector_2));
                return 1;
            }				
		}	
		lua_pushnil(L);
		return 1;
	}	
    //////////////////	
	static int add(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1)) 
		{
			Vector2 * vector_2 = *(Vector2 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector_1 = *(Vector2 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->x + vector_2->x);
				lua_pushnumber(L, vector_1->y + vector_2->y);
                return 2;
            }			
		}
		lua_pushnil(L);
		return 1;
	}
	//////////////////
	static int sub(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1)) 
		{
			Vector2 * vector_2 = *(Vector2 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector_1 = *(Vector2 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->x - vector_2->x);
				lua_pushnumber(L, vector_1->y - vector_2->y);
                return 2;
            }			
		}	
		lua_pushnil(L);
		return 1;
	}
	//////////////////
	static int mul(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1)) 
		{
			Vector2 * vector_2 = *(Vector2 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector_1 = *(Vector2 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->x * vector_2->x);
				lua_pushnumber(L, vector_1->y * vector_2->y);
                return 2;
            }			
		}
		lua_pushnil(L);
		return 1;
	    
	}
	//////////////////
	static int div(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1)) 
		{
			Vector2 * vector_2 = *(Vector2 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector_1 = *(Vector2 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->x / vector_2->x);
				lua_pushnumber(L, vector_1->y / vector_2->y);
                return 2;
            }			
		}		
		lua_pushnil(L);
		return 1;
	}
	//////////////////
	static int set(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TNUMBER);
		luaL_checktype(L, 3, LUA_TNUMBER);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector2 * vector = *(Vector2 **)lua_touserdata(L, -1);
			*vector = Vector2(lua_tonumber(L, 2), lua_tonumber(L, 3));
			// set to (LUA)
			lua_pushnumber(L, vector->x);
			lua_setfield(L, 1, "x");
			lua_pushnumber(L, vector->y);
			lua_setfield(L, 1, "y");
		}
		return 0;
	}
	//////////////////
	static int get(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		// get from (LUA)
		lua_getfield(L, 1, "x");
		lua_getfield(L, 1, "y");
	
		if(lua_type(L, 2) != LUA_TNUMBER && lua_type(L, 3) != LUA_TNUMBER) // If x and y are nil
		{
			// return from (C++)
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector2 * vector = *(Vector2 **)lua_touserdata(L, -1);
				lua_pushnumber(L, vector->x);
				lua_pushnumber(L, vector->y);
				return 2 ;
			}
		}
		// return from (LUA)
		lua_pushnumber(L, lua_tonumber(L, 2));
		lua_pushnumber(L, lua_tonumber(L, 3));
		return 2;
	}
	//////////////////
	// Opertator overloading
	//////////////////
	Vector2 operator + (const Vector2 &vector) const
	{
		return Vector2(x + vector.x, y + vector.y);
	}
	//////////////////
	Vector2 operator + (double number) const 
	{
		return Vector2(x + number, y + number);
	}	
	//////////////////
	Vector2 operator - (const Vector2 &vector) const
	{
		return Vector2(x - vector.x, y - vector.y);
	}
	//////////////////
	Vector2 operator - (double number) const 
	{
		return Vector2(x - number, y - number);
	}	
	//////////////////
	Vector2 operator * (const Vector2 &vector) const
	{
		return Vector2(x * vector.x, y * vector.y);
	}
	//////////////////
	Vector2 operator * (double number) const 
	{
		return Vector2(x * number, y * number);
	}	
	//////////////////
	Vector2 operator / (const Vector2 &vector) const
	{
		return Vector2(x / vector.x, y / vector.y);
	}
	//////////////////
	Vector2 operator / (double number) const 
	{
		return Vector2(x / number, y / number);
	}	
	//////////////////
	Vector2 operator -(void) const // negate
    {
        return Vector2(-x, -y);
    }	
	//////////////////
	Vector2 operator = (const Vector2 &vector)
	{
        x = vector.x;
		y = vector.y;
		return (*this);
	}	
	//////////////////
	Vector2 operator = (double number)
	{
		x = number;
		y = number;
		return (*this);
	}	
	//////////////////
	Vector2 operator += (const Vector2 &vector)
	{
		x += vector.x;
		y += vector.y;
		return (*this);
	}	
	//////////////////
	Vector2 operator += (double number)
	{
		x += number; 
		y += number;
		return (*this);
	}
	//////////////////
	Vector2 operator -= (const Vector2 &vector)
	{
		x -= vector.x;
		y -= vector.y;
		return (*this);
	}	
	//////////////////
	Vector2 operator -= (double number)
	{
		x -= number; 
		y -= number;
		return (*this);
	}
	//////////////////
	Vector2 operator *= (const Vector2 &vector)
	{
		x *= vector.x;
		y *= vector.y;
		return (*this);
	}	
	//////////////////
	Vector2 operator *= (double number)
	{
		x *= number; 
		y *= number;
		return (*this);
	}
	//////////////////
	Vector2 operator /= (const Vector2 &vector)
	{
		x /= vector.x;
		y /= vector.y;
		return (*this);
	}	
	//////////////////
	Vector2 operator /= (double number)
	{
		x /= number; 
		y /= number;
		return (*this);
	}	
	//////////////////
    bool operator < (const Vector2 &vector) const
	{
		return ((x < vector.x) || (y < vector.y)) ? true : false;
	}	
	//////////////////
    bool operator > (const Vector2 &vector) const
	{
		return ((x > vector.x) || (y > vector.y)) ? true : false;
	}		
	//////////////////
    bool operator <= (const Vector2 &vector) const
	{
		return ((x <= vector.x) || (y <= vector.y)) ? true : false;
	}	
	//////////////////
    bool operator >= (const Vector2 &vector) const
	{
		return ((x >= vector.x) || (y >= vector.y)) ? true : false;
	}			
	//////////////////
    bool operator == (const Vector2 &vector) const
	{
		return ((x == vector.x) && (y == vector.y)) ? true : false;
	}		
	//////////////////
    bool operator != (const Vector2 &vector) const
	{
		return ((x != vector.x) || (y != vector.y)) ? true : false;
	}	
	//////////////////
	double& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
		}
		return x;
	}
	//////////////////
	const double& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
		}	
		return x;	
	}		
	//////////////////
	friend std::ostream& operator << (std::ostream& os, const Vector2 & vector)
	{
		os << vector.x << " " << vector.y;
		return os;		
	}	
    // properties
	double x;
    double y;
	// no need for xy (the whole Vector2 can be used)
};
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
//////////////////
struct Vector3 
{
	//////////////////
	// Constructor(s)
    Vector3() : x(0.0), y(0.0), z(0.0){}
	//////////////////
    Vector3(double number) // from number
	{
		x = number;
		y = number;
		z = number;
		
		xy = Vector2(x, y);
	}
	//////////////////
	Vector3(double x, double y, double z) // from multiple numbers 
    {
		this->x = x;
		this->y = y;
	    this->z = z;
		
		xy = Vector2(x, y);
    }
	//////////////////
	Vector3(const Vector2 & vector) // from a Vector2 (might be removed or not)
	{
		x = vector.x;
		y = vector.y;
		z = 0.0;
		
		xy = Vector2(x, y);
	}
	//////////////////
	Vector3(const Vector2 & vector, double z) // from a Vector2
	{
		x = vector.x;
		y = vector.y;
		this->z = z;
		
		xy = Vector2(x, y);
	}	
	//////////////////
	Vector3(double x, const Vector2 & vector) // from a Vector2
	{
		this->x = x;
		y = vector.x;
		z = vector.y;
		
		xy = Vector2(x, y);
	}		
	//////////////////	
	Vector3(const Vector3 & vector) // from a Vector3
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		
		xy = Vector2(x, y);
	}
	//////////////////
	~Vector3(void){}
	//////////////////
	void set(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void set(const Vector2& vector) // set to another vector's values
	{
		x = vector.x;
		y = vector.y;
		z = this->z;
	}	
	void set(const Vector3& vector) // set to another vector's values
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}
	static void set(Vector3& vector, double x, double y, double z) // no need to make this const since vector will be altered
	{
		vector.set(x, y, z);
	}
	static void set(Vector3& a, Vector3& b) 
	{
        a.set(b);
	}
	//////////////////
	Vector3 get() const
	{
		return (*this);
	}
	//////////////////
	bool empty() const
	{
		return (x == 0 && y == 0 && z == 0);
	}	
	//////////////////
	double length() const// getter
	{
		return sqrt(x * x + y * y + z * z);
	}
	static double length(Vector3& vector)
	{
		return vector.length();
	}
	//////////////////
	Vector3 radians() const// getter
	{
		return Vector3(x * (3.14/180), y * (3.14/180), z * (3.14/180));
	}
	static Vector3 radians(Vector3& vector)
	{
		return vector.radians();
	}
	//////////////////
	void normalize(void) // setter
	{
		double l = length();
		if (l == 0) l = 1.0;
		x = x / l;
		y = y / l;
		z = z / l;
	}
	static void normalize(Vector3& vector)
	{
        vector.normalize();
	}
    //////////////////	
	Vector3 normal() const
	{
		Vector3 vector;
		double l = length();
		if(l == 0) l = 1.0;
		vector.x = x / l;
		vector.y = y / l;
		vector.z = z / l;
		return vector;
	}
	static Vector3 normal(const Vector3& vector)
	{
		return vector.normal();
	}
	//////////////////
	double dot(double x, double y, double z) const // getter
	{
		return ((this->x * x) + (this->y * y) + (this->z * z));
	}	
	double dot(const Vector3& vector) const 
	{		
		return ((x * vector.x) + (y * vector.y) + (z * vector.z));
	}
	static double dot(Vector3 a, Vector3 b) // no need to make parameter const since it already passes const in prev. functions
	{
		return a.dot(b);
	}
	//////////////////
	Vector3 cross(const Vector3& vector) const// getter
	{
		return Vector3((y * vector.z) - (z * vector.y), (z * vector.x) - (x * vector.z), (x * vector.y) - (y * vector.x));
	}
	static Vector3 cross(Vector3 a, Vector3 b)  // no need to make parameter const since it already passes const in prev. functions
	{
       return a.cross(b);
	}
	//////////////////
	Vector3 scalar(double number) const// getter
	{
		return Vector3(x * number, y * number, z * number);
	}
	static Vector3 scalar(Vector3& vector, double number) 
	{
		return vector.scalar(number);
	}
	//////////////////
	//////////////////
	// Basic Math Operations - getters
	Vector3 add(double number) const
	{
		return Vector3(x + number, y + number, z + number);
	}
	Vector3 add(const Vector3& vector) const// Vector3(0, 0, 0), Vector vector
	{
		return Vector3(x + vector.x, y + vector.y, z + vector.z);		
	}
	static Vector3 add(Vector3 a, Vector3 b) // no need to make parameter const since it already passes const in prev. functions
	{
		return a.add(b);
	}
	//////////////////
	Vector3 sub(double number) const
	{
		return Vector3(x - number, y - number, z - number);		
	}
	Vector3 sub(const Vector3& vector) const
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);				
	}
	static Vector3 sub(Vector3 a, Vector3 b)  // no need to make parameter const since it already passes const in prev. functions
	{
		return a.sub(b);
	}	
	//////////////////
	Vector3 mul(double number) const
	{
		return Vector3(x * number, y * number, z * number);			
	}
	Vector3 mul(const Vector3& vector) const
	{
		return Vector3(x * vector.x, y * vector.y, z * vector.z);			
	}	
	static Vector3 mul(Vector3 a, Vector3 b)    // no need to make parameter const since it already passes const in prev. functions
	{
		return a.mul(b);
	}	
	//////////////////
	Vector3 div(double number) const
	{
		return Vector3(x / number, y / number, z / number);			
	}
	Vector3 div(const Vector3& vector) const
	{
		return Vector3(x / vector.x, y / vector.y, z / vector.z);			
	}
	static Vector3 div(Vector3 a, Vector3 b)     // no need to make parameter const since it already passes const in prev. functions
	{
		return a.div(b);
	}
	//////////////////
	// Transforms - setters and getters
	Vector3 translate(double x, double y, double z)
	{
		// calculate translation
		this->x = this->x + x;
		this->y = this->y + y;
		this->z = this->z + z;
		xy = Vector2(this->x, this->y);
		// return translated vector
		return (*this);
	}
	//////////////////
	Vector3 translate(Vector3& vector)
	{
		return translate(vector.x, vector.y, vector.z);
	}
	//////////////////
	static Vector3 translate(Vector3& vector, double x, double y, double z)
	{
		return vector.translate(x, y, z);
	}
	//////////////////
	Vector3 rotate(double degrees, int x, int y, int z) // 0 = no rotation in specified axis(s) 1 = rotation in specified axis
	{
		// set limit from 0-360 degrees 
		degrees = ((int)degrees + 360) % 360; 
		// convert degrees to radians
		double angle = (double)degrees * (3.14 / 180); 	
        // calculate rotations	
		if(x == 1)
		{
			// calculate x-axis rotation
			double x = this->x;
			double y = this->y * cos(angle) - this->z * sin(angle);
			double z = this->x * sin(angle) + this->z * cos(angle);
			// apply rotation
			this->x = (x);
			this->y = (y);
			this->z = (z);
		}
	    if(y == 1)
		{
			// calculate y-axis rotation
			double x = this->x * cos(angle) + this->z * sin(angle);
			double y = this->y;
			double z = this->x * sin(angle) - this->z * cos(angle);
			// apply rotation
			this->x = (x);
			this->y = (y);
			this->z = (z);
		}
	    if(z == 1)
		{
			// calculate z-axis rotation
			double x = this->x * cos(angle) - this->y * sin(angle);
			double y = this->x * sin(angle) + this->y * cos(angle);
			double z = this->z;
			// apply rotation
			this->x = (x);
			this->y = (y);
			this->z = (z);
		}
		xy = Vector2(this->x, this->y);
		// return rotated vector
		return (*this);
	}
	//////////////////
	Vector3 rotate(Vector3& vector, double degrees) // vector1->rotate(Vector3(1, 0, 0), 90)
	{
		return rotate(degrees, (int)vector.x, (int)vector.y, (int)vector.z);
	}
	//////////////////
	static Vector3 rotate(Vector3& vector, double degrees, int x, int y, int z)
	{
		return vector.rotate(degrees, x, y, z);
	}
	//////////////////
	void rotate_no_translate()
	{
		// rotate without changing the position
	    translate(-1, -1, -1);
        rotate(90, 1, 1 , 1);
	    translate(1, 1, 1);
	}
	//////////////////
	Vector3 scale(double sx, double sy, double sz) // 1 = no shear
	{
		// calculate scale
	    this->x = this->x * (sx);
		this->y = this->y * (sy);
		this->z = this->z * (sz);
		xy = Vector2(this->x, this->y);
		// return scaled vector
		return (*this);
	}
	//////////////////
	Vector3 scale(Vector3& vector)
	{
		return scale(vector.x, vector.y, vector.z);
	}
	static Vector3 scale(Vector3& vector, double sx, double sy, double sz)
	{
		return vector.scale(sx, sy, sz);
	}
	//////////////////
	Vector3 shear(double shx, double shy, double shz)
	{
		if(shx > 0) // if not 0, but can be 0.1+
		{
			// calculate scale x
			double x = this->x + (shx * this->y);
            double y = this->y;
			double z = this->z;	
			// apply scale
			this->x = (x);
			this->y = (y);
			this->z = (z); 		
		}
		if(shy > 0)
		{
			// calculate scale y
			double x = this->x;
			double y = this->y + (shy * this->x);
			double z = this->z;
			// apply scale
			this->x = (x);
			this->y = (y);
			this->z = (z);   	
		}
		if(shz > 0)
		{
			// calculate scale z
			double x = this->x + (shx * this->z);
            double y = this->y + (shy * this->z);	
            double z = this->z;	
			// apply scale
			this->x = (x);
			this->y = (y);
			this->z = (z);    			
		}	
		xy = Vector2(x, y);
		// return sheared vector
		return (*this);
	}
	//////////////////
	Vector3 shear(Vector3& vector)
	{
		return shear(vector.x, vector.y, vector.z);
	}
	static Vector3 shear(Vector3& vector, double sx, double sy, double sz)
	{
		return vector.shear(sx, sy, sz);
	}
	//////////////////
	Vector3 reflect(int x, int y, int z)
	{
		if(x)
		{
			scale(-1, 1, 1);
		}
	    if(y)
		{
			scale(1, -1, 1);
		}
	    if(z)
		{
			scale(1, 1, -1);
		}
		xy = Vector2(x, y);
		return (*this);
	}
	//////////////////
	static Vector3 reflect(Vector3& vector, int x, int y, int z)
	{
		return vector.reflect(x, y, z);
	}
	//////////////////
	static Vector3 rgb_to_yuv_test(double red, double green, double blue)
	{/*
        double Y = (0.257 * red) + (0.504 * green) + (0.098 * blue) + 16;
        double V = (0.439 * red) - (0.368 * green) - (0.071 * blue) + 128;
        double U = -(0.148 * red) - (0.291 * green) + (0.439 * blue) + 128;
        */
        double Y =  0.257 * red + 0.504 * green + 0.098 * blue +  16;
        double U = -0.148 * red - 0.291 * green + 0.439 * blue + 128;
        double V =  0.439 * red - 0.368 * green - 0.071 * blue + 128;
        return Vector3(Y, U, V);
	}
	//////////////////
	static Vector3 yuv_to_rgb_test(double y, double u, double v) 
	{/*
        double B = 1.164 * (y - 16) + 2.018 * (u - 128);
        double G = 1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128);
        double R = 1.164 * (y - 16) + 1.596 * (v - 128);
        (*/
        y -= 16;
        u -= 128;
        v -= 128;
        double R = 1.164 * y             + 1.596 * v;
        double G = 1.164 * y - 0.392 * u - 0.813 * v;
        double B = 1.164 * y + 2.017 * u;
        return Vector3(R, G, B);
	}	
	//////////////////
	//////////////////
	static Vector3 rgb_to_yuv(double red, double green, double blue) 
	{
	    double Y = (0.299 * red) + (0.587 * green) + (0.114 * blue);
        double U = (blue - Y) * 0.565;
        double V = (red  - Y) * 0.713;
        return Vector3(Y, U, V);
	}
	static Vector3 rgb_to_yuv(const Vector3& vector) 
	{
	    double Y = (0.299 * vector.x) + (0.587 * vector.y) + (0.114 * vector.z);
        double U = (vector.z - Y) * 0.565;
        double V = (vector.x - Y) * 0.713;
        return Vector3(Y, U, V);	
	}
	static Vector3 yuv_to_rgb(double y, double u, double v) 
	{
	    double R = y + (1.403 * v);
        double G = y - (0.344 * u) - (0.714 * v);
        double B = y + (1.770 * u);
        return Vector3(R, G, B);
	}
	static Vector3 yuv_to_rgb(const Vector3& vector) 
	{
	    double R = vector.x + (1.403 * vector.z);
        double G = vector.x - (0.344 * vector.y) - (0.714 * vector.z);
        double B = vector.x + (1.770 * vector.y);
        return Vector3(R, G, B);
	}
	//////////////////
	// Lua functions
	static int Vector_new(lua_State *L) // equivalent = Vector3(0, 0, 0) -> 0, 0, 0
	{
	    luaL_checktype(L, 1, LUA_TNUMBER);
		luaL_checktype(L, 2, LUA_TNUMBER);
		luaL_checktype(L, 3, LUA_TNUMBER);
		return 3;
	}
	//////////////////
	static int new_(lua_State *L)
	{
		double x = 0; 
        double y = 0;
		double z = 0;
		bool number_set = false;
		if
		(
		    lua_isnumber(L, -3) &&
		    lua_isnumber(L, -2) && 
		    lua_isnumber(L, -1) 
		)
		{
			x = lua_tonumber(L, -3);
			y = lua_tonumber(L, -2); 
			z = lua_tonumber(L, -1);
			number_set = true ;
		}
		lua_settop(L, 0); // clear stack
		// create table
		lua_createtable(L, 0, 0);
		// set mt
		lua_getglobal(L, "Vector_mt");
		lua_setmetatable(L, 1);
		// set udata
		Vector3 ** vector = (Vector3 **)lua_newuserdata(L, sizeof(Vector3));
		if ( !number_set )
			*vector = new Vector3();
		else
			*vector = new Vector3(x, y, z);
		lua_setfield(L, 1, "udata");
		// set values
		lua_pushnumber(L, x);
		lua_setfield(L, 1, "x");
		lua_pushnumber(L, y);
		lua_setfield(L, 1, "y");
		lua_pushnumber(L, z);
		lua_setfield(L, 1, "z");		
		// return table
		if(lua_istable(L, -1))
			return 1;		
		lua_pushnil(L);
		return 1;
	}
	//////////////////
	static int set(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TNUMBER);
		luaL_checktype(L, 3, LUA_TNUMBER);
		luaL_checktype(L, 4, LUA_TNUMBER);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
			vector->set(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
			// set to (LUA)
			lua_pushnumber(L, vector->x);
			lua_setfield(L, 1, "x");
			lua_pushnumber(L, vector->y);
			lua_setfield(L, 1, "y");
			lua_pushnumber(L, vector->z);
			lua_setfield(L, 1, "z");			
		}
		return 0;		
	}
	//////////////////
	static int get(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		// get from (LUA)
		lua_getfield(L, 1, "x");
		lua_getfield(L, 1, "y");
		lua_getfield(L, 1, "z");
		if(!lua_isnumber(L, 2) && !lua_isnumber(L, 3) && !lua_isnumber(L, 4)) // If x and y are nil
		{
			// return from (C++)
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
				lua_pushnumber(L, vector->x);
				lua_pushnumber(L, vector->y);
				lua_pushnumber(L, vector->z);
				return 3;
			}
		}
		// return from (LUA)
		lua_pushnumber(L, lua_tonumber(L, 2));
		lua_pushnumber(L, lua_tonumber(L, 3));
        lua_pushnumber(L, lua_tonumber(L, 4));
		return 3;
	}
	//////////////////
	static int length(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
		    lua_pushnumber(L, vector->length());
            return 1;				
		}		
		lua_pushnil(L);
		return 1;
	}	
	//////////////////
	static int normal(lua_State *L) // returns normal of a vector
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
		    lua_pushnumber(L, vector->normal().x);
			lua_pushnumber(L, vector->normal().y);
			lua_pushnumber(L, vector->normal().z);
            return 3;				
		}				
		lua_pushnil(L);
		return 3;
	}	
	//////////////////
	static int normalize(lua_State *L) // normalizes a vector
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
		    vector->normalize();				
		}				
		return 0;
	}		
	//////////////////
	static int dot(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector_2 = *(Vector3 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1))
		    {
			    Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
		        lua_pushnumber(L, vector->dot(*vector_2));
                return 1;				
		    }
		}		
		lua_pushnil(L);
		return 1;
	}	
	//////////////////
	static int cross(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector_2 = *(Vector3 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1))
		    {
			    Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
		        lua_pushnumber(L, vector->cross(*vector_2).x);
				lua_pushnumber(L, vector->cross(*vector_2).y);
				lua_pushnumber(L, vector->cross(*vector_2).z);
				return 3;
			}
		}			
		lua_pushnil(L);
		lua_pushnil(L);
		lua_pushnil(L);
		return 3;
	}	
	//////////////////
	// Basic math
	static int add(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector_2 = *(Vector3 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
		    {
				Vector3 * vector_1 = *(Vector3 **)lua_touserdata(L, -1);
				lua_pushnumber(L, vector_1->add(*vector_2).x );
				lua_pushnumber(L, vector_1->add(*vector_2).y );
				lua_pushnumber(L, vector_1->add(*vector_2).z );
                return 3;				
			}
		}
		lua_pushnil(L);
		return 1;
	}	

	//////////////////
	static int sub(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector_2 = *(Vector3 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
		    {
				Vector3 * vector_1 = *(Vector3 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->sub(*vector_2).x );
				lua_pushnumber(L, vector_1->sub(*vector_2).y );
				lua_pushnumber(L, vector_1->sub(*vector_2).z );
                return 3;				
			}
		}
		lua_pushnil(L);
		return 1;		
	}	
	//////////////////
	static int mul(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector_2 = *(Vector3 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
		    {
				Vector3 * vector_1 = *(Vector3 **)lua_touserdata(L, -1);
				lua_pushnumber(L, vector_1->mul(*vector_2).x );
				lua_pushnumber(L, vector_1->mul(*vector_2).y );
				lua_pushnumber(L, vector_1->mul(*vector_2).z );
                return 3;				
			}
		}
		lua_pushnil(L);
		return 1;		
	}		
	//////////////////
	static int div(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector_2 = *(Vector3 **)lua_touserdata(L, -1);
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
		    {
				Vector3 * vector_1 = *(Vector3 **)lua_touserdata(L, -1);
                lua_pushnumber(L, vector_1->div(*vector_2).x );
				lua_pushnumber(L, vector_1->div(*vector_2).y );
				lua_pushnumber(L, vector_1->div(*vector_2).z );
                return 3;				
			}
		}
		lua_pushnil(L);
		return 1;		
	}	
	//////////////////
	// Transformations
	static int translate(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
        lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
            //vector->translate( lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4) );
            return 1;				
		}
		lua_pushnil(L);
        return 1;		
	}
	//////////////////
	static int rotate(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
        lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
            //vector->();
            return 1;				
		}
		lua_pushnil(L);
        return 1;			
	}	
	//////////////////
	static int scale(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
        lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
            //vector->();
            return 1;				
		}
		lua_pushnil(L);
        return 1;			
	}	
	//////////////////
	static int shear(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
        lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
            //vector->();
            return 1;				
		}
		lua_pushnil(L);
        return 1;			
	}	
	//////////////////
 	static int reflect(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
        lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Vector3 * vector = *(Vector3 **)lua_touserdata(L, -1);
            //vector->();
            return 1;				
		}
		lua_pushnil(L);
        return 1;			
	}	
	//////////////////	
	// Opertator overloading
	//////////////////
	Vector3 operator + (const Vector3 &vector) const
	{
	    return Vector3(x + vector.x, y + vector.y, z + vector.z);
	}
	//////////////////
	Vector3 operator + (const Vector2 &vector) const
	{
	    return Vector3(x + vector.x, y + vector.y, z);
	}	
	//////////////////
	Vector3 operator + (double number) const
	{
		return Vector3(x + number, y + number, z + number);
	}	
	//////////////////	
	Vector3 operator - (const Vector3 &vector) const
	{		
	    return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}
	//////////////////
	Vector3 operator - (const Vector2 &vector) const
	{		
	    return Vector3(x - vector.x, y - vector.y, this->z);
	}	
	//////////////////
	Vector3 operator - (double number) const
	{
		return Vector3(x - number, y - number, z - number);
	}	
	//////////////////	
	Vector3 operator * (const Vector3 &vector) const
	{
	    return Vector3(x * vector.x, y * vector.y, z * vector.z);
	}
	//////////////////
	Vector3 operator * (const Vector2 &vector) const
	{
	    return Vector3(x * vector.x, y * vector.y, this->z);
	}	
	//////////////////
	Vector3 operator * (double number) const
	{
		return Vector3(x * number, y * number, z * number);
	}		
	//////////////////	
	Vector3 operator / (const Vector3 &vector) const
	{
	    return Vector3(x / vector.x, y / vector.y, z / vector.z);
	}
	//////////////////
	Vector3 operator / (const Vector2 &vector) const
	{
	    return Vector3(x / vector.x, y / vector.y, this->z);
	}	
	//////////////////
	Vector3 operator / (double number) const
	{
		return Vector3(x / number, y / number, z / number);
	}		
	//////////////////
	Vector3 operator -(void) const // negate
    {
        return Vector3(-x, -y, -z);
    }
	//////////////////
	Vector3 operator = (const Vector3 &vector)
	{
        x = vector.x;
		y = vector.y;
		z = vector.z;
		return (*this);
	}
	//////////////////
	Vector3 operator = (const Vector2 &vector)
	{
        x = vector.x;
		y = vector.y;
		z = this->z;
		return (*this);
	}
	//////////////////
	Vector3 operator = (double number)
	{
		x = number;
		y = number;
		z = number;
		return (*this);
	}	
    //////////////////	
	Vector3 operator += (const Vector3 &vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return (*this);
	}	
	//////////////////
	Vector3 operator += (const Vector2 &vector)
	{
		x += vector.x;
		y += vector.y;
		z = this->z;
		return (*this);
	}	
	//////////////////
	Vector3 operator += (double number)
	{
		x += number;
		y += number;
		z += number;
		return (*this);
	}		
	//////////////////
	Vector3 operator -= (const Vector3 &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return (*this);		
	}	
	//////////////////
	Vector3 operator -= (const Vector2 &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z = this->z;
		return (*this);		
	}		
	//////////////////
	Vector3 operator -= (double number)
	{
		x -= number;
		y -= number;
		z -= number;
        return (*this);
	}	
	//////////////////	
	Vector3 operator *= (const Vector3 &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		return (*this);		
	}	
	//////////////////
	Vector3 operator *= (const Vector2 &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= this->z;
		return (*this);		
	}	
	//////////////////
	Vector3 operator *= (double number)
	{
		x *= number;
		y *= number;
		z *= number;
		return (*this);	
	}	
	//////////////////
	Vector3 operator /= (const Vector3 &vector)
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		return (*this);		
	}	
	//////////////////
	Vector3 operator /= (const Vector2 &vector)
	{
		x /= vector.x;
		y /= vector.y;
		z /= this->z;
		return (*this);		
	}	
	//////////////////
	Vector3 operator /= (double number)
	{
		x /= number;
		y /= number;
		z /= number;
		return (*this);	
	}		
	//////////////////
    bool operator < (const Vector3 &vector) const
	{
		return ((x < vector.x) || (y < vector.y) || (z < vector.z)) ? true : false;
	}	
	//////////////////
    bool operator > (const Vector3 &vector) const
	{
		return ((x > vector.x) || (y > vector.y) || (z > vector.z)) ? true : false;
	}		
	//////////////////
    bool operator <= (const Vector3 &vector)
	{
		return ((x <= vector.x) || (y <= vector.y) || (z <= vector.z)) ? true : false;
	}	
	//////////////////
    bool operator >= (const Vector3 &vector)
	{
		return ((x >= vector.x) || (y >= vector.y) || (z >= vector.z)) ? true : false;
	}			
	//////////////////
    bool operator == (const Vector3 &vector)
	{
		return ((x == vector.x) && (y == vector.y) && (z == vector.z)) ? true : false;
	}		
	//////////////////
    bool operator != (const Vector3 &vector)
	{
		return ((x != vector.x) || (y != vector.y) || (z != vector.z)) ? true : false;
	}	
	//////////////////
	double& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
		}
		return x;
	}
	//////////////////
	const double& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
		}		
		return x;
	}
	//////////////////
	friend std::ostream& operator << (std::ostream& os, const Vector3 & vector)
	{
		os << vector.x << " " << vector.y << " " << vector.z;
		return os;		
	}	
	//////////////////
    double x;
    double y;
    double z;
	Vector2 xy;
	// no need for xyz (the whole Vector3 can be used)
}; 
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
struct Vector4 {
	//////////////////
    Vector4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    //////////////////
	Vector4(double number)
	{
		x = number;
		y = number;
		z = number;
		w = number;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}
	//////////////////
	Vector4(double x, double y, double z, double w) 
    {
		this->x = x;
		this->y = y;
	    this->z = z;
		this->w = w;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
    }
	//////////////////
	Vector4(const Vector2 &vector)
	{
		x = vector.x;
		y = vector.y;
		z = 0.0;
		w = 1.0;//0.0;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}	
	//////////////////
	Vector4(const Vector2 &a, const Vector2 &b)
	{
		x = a.x;
		y = a.y;
		z = b.x;
		w = b.y;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}
	//////////////////
	Vector4(const Vector2& vector, double z, double w) 
	{
		x = vector.x;
		y = vector.y;
		this->z = z;
		this->w = w;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}		
	//////////////////
	Vector4(double x, double y, const Vector2& vector) 
	{
		this->x = x;
		this->y = y;
		z = vector.x;
		w = vector.y;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}
	//////////////////
	Vector4(double x, const Vector2 &vector, double w) 
	{
		this->x = x;
		y = vector.x;
		z = vector.y;
		this->w = w;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}		
	//////////////////
	Vector4(const Vector3 &vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = 1.0;//0.0;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}	
	//////////////////
	Vector4(const Vector3 &vector, double w)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		this->w = w;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);
	}	
	//////////////////
	Vector4(double x, const Vector3 &vector)
	{
		this->x = x;
		y = vector.x;
		z = vector.y;
		w = vector.z;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);		
	}	
	//////////////////
	Vector4(const Vector4 &vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
		
		xy  = Vector2(x, y);
		zw  = Vector2(z, w);
		xyz = Vector3(x, y, z);		
	}
	//////////////////
	~Vector4(void){}
	//////////////////
	bool empty() const
	{
		return (x == 0 && y == 0 && z == 0 && w == 0);
	}	
	//////////////////
	double length()
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
	//////////////////
	Vector4 radians() // getter
	{
		return Vector4
		(
		    (x * (3.14/180)),
			(y * (3.14/180)),
			(z * (3.14/180)),
			(w * (3.14/180))
		);
	}
	//////////////////
	static Vector4 radians(Vector4 vector)
	{
		return vector.radians();
	}	
	//////////////////
	Vector4 normalize()
	{
		Vector4 vector;
		double len = length();
		if(len != 0) 
		{
			vector.x = x / len;
			vector.y = y / len;
			vector.z = z / len;
			vector.w = w / len;
		}
		return (vector);
	}
    //////////////////
	static Vector4 normalize(Vector4 vector)
	{
        return vector.normalize();
	}
    //////////////////	
	Vector4 normal()
	{
		Vector4 vector;
		double l = length();
		if(l == 0) l = 1.0;
		vector.x = x / l;
		vector.y = y / l;
		vector.z = z / l;
		vector.w = w / l;
		return vector;
	}
	//////////////////	
	double dot(double x, double y, double z, double w) // getter
	{
		return ((this->x * x) + (this->y * y) + (this->z * z) + (this->w * w));
	}	
	//////////////////
	double dot(Vector4 vector)  
	{		
		return ((x * vector.x) + (y * vector.y) + (z * vector.z) + (w * vector.w));
	}
	//////////////////
	static double dot(Vector4 a, Vector4 b)
	{
		return a.dot(b);
	}
	//////////////////
	Vector4 cross(Vector4 vector) // getter
	{
		return Vector4
		(
		    (y * vector.z) - (z * vector.y),
			(z * vector.x) - (x * vector.z),
			(x * vector.y) - (y * vector.x),
			(1.0)
		);
	}
	//////////////////
	static Vector4 cross(Vector4 a, Vector4 b)
	{
       return a.cross(b);
	}
	//////////////////
	Vector4 scalar(double number) // getter
	{
		return Vector4
		(
		    (x * number),
			(y * number),
			(z * number),
			(w * number)
		);
	}
	//////////////////
	static Vector4 scalar(Vector4 vector, double number) 
	{
		return vector.scalar(number);
	}
	//////////////////
	static Vector4 inverse(Vector4 &vector)
	{
	   vector = vector * Vector4(-1, -1, -1, -1);
	   return vector;
	}
	//////////////////
	// Transformations
	void translate(double x, double y, double z)
	{
		// copy values from self
		Vector3 vec3(this->x, this->y, this->z);
		// shear vector3
		vec3.translate(x, y, z);
		// save translated vector
		(*this) = Vector4 (vec3.x, vec3.y, vec3.z, 1.0);		
	}
	void rotate(double degrees, int x, int y, int z)
	{
		// copy values from self
		Vector3 vec3(this->x, this->y, this->z);
		// shear vector3
		vec3.rotate(degrees, x, y, z);
		// save rotated vector
		(*this) = Vector4 (vec3.x, vec3.y, vec3.z, 1.0);		
	}
	void scale(double sx, double sy, double sz)
	{
		// copy values from self
		Vector3 vec3(this->x, this->y, this->z);
		// shear vector3
		vec3.scale(sx, sy, sz);
		// save scaled vector
		(*this) = Vector4 (vec3.x, vec3.y, vec3.z, 1.0);		
	}
	void shear(double shx, double shy, double shz)
	{
		// copy values from self
		Vector3 vec3(this->x, this->y, this->z);
		// shear vector3
		vec3.shear(shx, shy, shz);
		// save sheared vector
		(*this) = Vector4 (vec3.x, vec3.y, vec3.z, 1.0);
	}
	//////////////////
	// Lua functions
	static int Vector_new(lua_State *L)
	{
	    luaL_checktype(L, 1, LUA_TNUMBER);
		luaL_checktype(L, 2, LUA_TNUMBER);
		luaL_checktype(L, 3, LUA_TNUMBER);
		luaL_checktype(L, 4, LUA_TNUMBER);
		return 4;
	}	
	//////////////////
	// Operator overloading
	//////////////////
	Vector4 operator + (const Vector4 &vector) const 
	{
	    return Vector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
	}
	//////////////////
	Vector4 operator + (const Vector3 &vector) const 
	{
	    return Vector4(x + vector.x, y + vector.y, z + vector.z, w);
	}
	//////////////////
	Vector4 operator + (const Vector2 &vector) const 
	{
	    return Vector4(x + vector.x, y + vector.y, z, w);
	}	
	//////////////////
	Vector4 operator + (double number) const 
	{
		return Vector4(x + number, y + number, z + number, w + number);
	}
	//////////////////
	Vector4 operator - (const Vector4 &vector) const
	{		
	    return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}
	Vector4 operator - (const Vector3 &vector) const
	{		
	    return Vector4(x - vector.x, y - vector.y, z - vector.z, w);
	}
	Vector4 operator - (const Vector2 &vector) const
	{		
	    return Vector4(x - vector.x, y - vector.y, z, w);
	}	
	Vector4 operator - (double number) const 
	{
		return Vector4(x - number, y - number, z - number, w - number);
	}	
	//////////////////
	Vector4 operator * (const Vector4 &vector) const
	{
	    return Vector4(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
	}
	//////////////////
	Vector4 operator * (const Vector3 &vector) const
	{
	    return Vector4(x * vector.x, y * vector.y, z * vector.z, w);
	}
	//////////////////
	Vector4 operator * (const Vector2 &vector) const
	{
	    return Vector4(x * vector.x, y * vector.y, z, w);
	}	
	//////////////////
	Vector4 operator * (double number) const 
	{
		return Vector4(x * number, y * number, z * number, w * number);
	}	
	//////////////////
	Vector4 operator / (const Vector4 &vector) const 
	{
	    return Vector4(x / vector.x, y / vector.y, z / vector.z, w / vector.w);
	}
	//////////////////
	Vector4 operator / (const Vector3 &vector) const 
	{
	    return Vector4(x / vector.x, y / vector.y, z / vector.z, w);
	}
	//////////////////
	Vector4 operator / (const Vector2 &vector) const 
	{
	    return Vector4(x / vector.x, y / vector.y, z, w);
	}	
	//////////////////
	Vector4 operator / (double number) const 
	{
		return Vector4(x / number, y / number, z / number, w / number);
	}	
	//////////////////
	Vector4 operator -(void) const // negate
    {
        return Vector4(-x, -y, -z, -w);
    }	
	//////////////////
	Vector4 operator = (const Vector3 &vector)
	{
        x = vector.x;
		y = vector.y;
		z = vector.z;
		w = this->w;
		return (*this);
	}
	//////////////////
	Vector4 operator = (const Vector2 &vector)
	{
        x = vector.x;
		y = vector.y;
		z = this->z;
		w = this->w;
		return (*this);
	}
	//////////////////
	Vector4 operator = (double number)
	{
		x = number;
		y = number;
		z = number;
		w = number;
		return (*this);
	}	
	//////////////////
    bool operator < (const Vector4 &vector) const
	{
		return ((x < vector.x) || (y < vector.y) || (z < vector.z) || (w < vector.w)) ? true : false;
	}	
	//////////////////
    bool operator > (const Vector4 &vector) const
	{
		return ((x > vector.x) || (y > vector.y) || (z > vector.z) || (w > vector.w)) ? true : false;
	}		
	//////////////////
    bool operator <= (const Vector4 &vector) const
	{
		return ((x <= vector.x) || (y <= vector.y) || (z <= vector.z) || (w <= vector.z)) ? true : false;
	}	
	//////////////////
    bool operator >= (const Vector4 &vector) const
	{
		return ((x >= vector.x) || (y >= vector.y) || (z >= vector.z) || (w >= vector.w)) ? true : false;
	}			
	//////////////////
    bool operator == (const Vector4 &vector) const
	{
		return ((x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w)) ? true : false;
	}
    //////////////////	
    bool operator != (const Vector4 &vector) const
	{
		return ((x != vector.x) || (y != vector.y) || (z != vector.z) || (w != vector.w)) ? true : false;
	}	
	//////////////////
	double& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
			case 3: return w; break;
		}
		return x;
	}
	//////////////////
	const double& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
			case 3: return w; break;
		}		
		return x;
	}		
    //////////////////	
	friend std::ostream& operator << (std::ostream& os, const Vector4 & vector)
	{
		os << vector.x << " " << vector.y << " " << vector.z << " " << vector.w;
		return os;		
	}
    double x;
    double y;
    double z;
	double w;
	Vector2 xy;
	Vector2 zw;
	Vector3 xyz;
	// no need for xyzw (the whole Vector4 can be used)
};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector2f
{
	// Constructor(s)
    Vector2f() : x(0.0), y(0.0) 
	{}
	Vector2f(float number)
	{
		x = number;
		y = number;
	}
    Vector2f(float x, float y) 
    {
	    this->x = x;
	    this->y = y;
    }
	Vector2f(const Vector2f& vector)
	{
		x = vector.x;
		y = vector.y;
	}
	//////////////////
	// Destructor
	~Vector2f(void){}
	//////////////////
	
	
	
	//////////////////
	float& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
		}
		return x;
	}
	//////////////////
	const float& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
		}		
		return x;
	}		
	//////////////////
	
	float x, y;
};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector3f
{
	// Constructor(s)
    Vector3f() : x(0.0), y(0.0), z(0.0)
	{}
    Vector3f(float number)
	{
		x = number;
		y = number;
		z = number;
		
		xy = Vector2f(x, y);
	}
	Vector3f(float x, float y, float z)
    {
		this->x = x;
		this->y = y;
	    this->z = z;
		
		xy = Vector2f(x, y);
    }
	Vector3f(const Vector2f & vector) // might be removed or not
	{
		x = vector.x;
		y = vector.y;
		z = 0.0;
		
		xy = Vector2f(x, y);
	}
	Vector3f(const Vector2f & vector, float z)
	{
		x = vector.x;
		y = vector.y;
		this->z = z;
		
		xy = Vector2f(x, y);
	}	
	Vector3f(float x, const Vector2f & vector)
	{
		this->x = x;
		y = vector.x;
		z = vector.y;
		
		xy = Vector2f(x, y);
	}		
	Vector3f(const Vector3f & vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		
		xy = Vector2f(x, y);
	}
	//////////////////
	~Vector3f(void){}
	//////////////////
	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	static float length(const Vector3f& vector)
	{
		return vector.length();
	}
	//////////////////
	Vector3f radians() const
	{
		return Vector3f(x * (3.14/180), y * (3.14/180), z * (3.14/180));
	}
	static Vector3f radians(const Vector3f& vector)
	{
		return vector.radians();
	}
	//////////////////
	void normalize(void) // normalize the vector
	{
		float l = length();
		if (l == 0) l = 1.0;
		x = x / l;
		y = y / l;
		z = z / l;
	}	
	static void normalize(Vector3f& vector) // cannot be const since it will be normalized
	{
        vector.normalize();
	}
	//////////////////
	Vector3f normal(void) const// return the normal of a vector
	{
		Vector3f vector = (*this);
		float l = length();
		if (l == 0) l = 1.0;
		vector.x = x / l;
		vector.y = y / l;
		vector.z = z / l;
		return vector;
	}
	static Vector3f normal(const Vector3f & vector) // static functions cannot have a const at the end
	{
        return vector.normal();
	}
	//////////////////
	float dot(float x, float y, float z) const
	{
		return ((this->x * x) + (this->y * y) + (this->z * z));
	}
	float dot(const Vector3f& vector) const
	{		
		return ((x * vector.x) + (y * vector.y) + (z * vector.z));
	}
	static float dot(const Vector3f& a, const Vector3f& b)
	{
		return a.dot(b);
	}
	//////////////////
	Vector3f cross(float x, float y, float z) const
	{
		return Vector3f((this->y * z) - (this->z * y), (this->z * x) - (this->x * z), (this->x * y) - (this->y * x));
	}
	Vector3f cross(const Vector3f& vector) const
	{
		return Vector3f((y * vector.z) - (z * vector.y), (z * vector.x) - (x * vector.z), (x * vector.y) - (y * vector.x));
	}
	static Vector3f cross(const Vector3f& a, const Vector3f& b)
	{
       return a.cross(b);
	}
	//////////////////
	Vector3f scalar(float number) const
	{
		return Vector3f(x * number, y * number, z * number);
	}
	static Vector3f scalar(const Vector3f & vector, float number) 
	{
		return vector.scalar(number);
	}
	//////////////////	
	
	
	
	
	
	
	
	
	
	
	// Opertator overloading
	//////////////////
	Vector3f operator + (const Vector3f &vector) const
	{
	    return Vector3f(x + vector.x, y + vector.y, z + vector.z);
	}
	//////////////////
	Vector3f operator + (const Vector2f &vector) const
	{
	    return Vector3f(x + vector.x, y + vector.y, z);
	}	
	//////////////////
	Vector3f operator + (float number) const
	{
		return Vector3f(x + number, y + number, z + number);
	}	
	//////////////////	
	Vector3f operator - (const Vector3f &vector) const
	{		
	    return Vector3f(x - vector.x, y - vector.y, z - vector.z);
	}
	//////////////////
	Vector3f operator - (const Vector2f &vector) const
	{		
	    return Vector3f(x - vector.x, y - vector.y, this->z);
	}	
	//////////////////
	Vector3f operator - (float number) const
	{
		return Vector3f(x - number, y - number, z - number);
	}	
	//////////////////	
	Vector3f operator * (const Vector3f &vector) const
	{
	    return Vector3f(x * vector.x, y * vector.y, z * vector.z);
	}
	//////////////////
	Vector3f operator * (const Vector2f &vector) const
	{
	    return Vector3f(x * vector.x, y * vector.y, this->z);
	}	
	//////////////////
	Vector3f operator * (float number) const
	{
		return Vector3f(x * number, y * number, z * number);
	}		
	//////////////////	
	Vector3f operator / (const Vector3f &vector) const
	{
	    return Vector3f(x / vector.x, y / vector.y, z / vector.z);
	}
	//////////////////
	Vector3f operator / (const Vector2f &vector) const
	{
	    return Vector3f(x / vector.x, y / vector.y, this->z);
	}	
	//////////////////
	Vector3f operator / (float number) const
	{
		return Vector3f(x / number, y / number, z / number);
	}		
	//////////////////
	Vector3f operator -(void) const // negate
    {
        return Vector3f(-x, -y, -z);
    }
	//////////////////
	Vector3f operator = (const Vector3f &vector)
	{
        x = vector.x;
		y = vector.y;
		z = vector.z;
		return (*this);
	}
	//////////////////
	Vector3f operator = (const Vector2f &vector)
	{
        x = vector.x;
		y = vector.y;
		z = this->z;
		return (*this);
	}
	//////////////////
	Vector3f operator = (float number)
	{
		x = number;
		y = number;
		z = number;
		return (*this);
	}	
    //////////////////	
	Vector3f operator += (const Vector3f &vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return (*this);
	}	
	//////////////////
	Vector3f operator += (const Vector2f &vector)
	{
		x += vector.x;
		y += vector.y;
		z = this->z;
		return (*this);
	}	
	//////////////////
	Vector3f operator += (float number)
	{
		x += number;
		y += number;
		z += number;
		return (*this);
	}		
	//////////////////
	Vector3f operator -= (const Vector3f &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return (*this);		
	}	
	//////////////////
	Vector3f operator -= (const Vector2f &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z = this->z;
		return (*this);		
	}		
	//////////////////
	Vector3f operator -= (float number)
	{
		x -= number;
		y -= number;
		z -= number;
        return (*this);
	}	
	//////////////////	
	Vector3f operator *= (const Vector3f &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		return (*this);		
	}	
	//////////////////
	Vector3f operator *= (const Vector2f &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= this->z;
		return (*this);		
	}	
	//////////////////
	Vector3f operator *= (float number)
	{
		x *= number;
		y *= number;
		z *= number;
		return (*this);	
	}	
	//////////////////
	Vector3f operator /= (const Vector3f &vector)
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		return (*this);		
	}	
	//////////////////
	Vector3f operator /= (const Vector2f &vector)
	{
		x /= vector.x;
		y /= vector.y;
		z /= this->z;
		return (*this);		
	}	
	//////////////////
	Vector3f operator /= (float number)
	{
		x /= number;
		y /= number;
		z /= number;
		return (*this);	
	}		
	//////////////////
    bool operator < (const Vector3f &vector) const
	{
		return ((x < vector.x) || (y < vector.y) || (z < vector.z)) ? true : false;
	}	
	//////////////////
    bool operator > (const Vector3f &vector) const
	{
		return ((x > vector.x) || (y > vector.y) || (z > vector.z)) ? true : false;
	}		
	//////////////////
    bool operator <= (const Vector3f &vector)
	{
		return ((x <= vector.x) || (y <= vector.y) || (z <= vector.z)) ? true : false;
	}	
	//////////////////
    bool operator >= (const Vector3f &vector)
	{
		return ((x >= vector.x) || (y >= vector.y) || (z >= vector.z)) ? true : false;
	}			
	//////////////////
    bool operator == (const Vector3f &vector)
	{
		return ((x == vector.x) && (y == vector.y) && (z == vector.z)) ? true : false;
	}		
	//////////////////
    bool operator != (const Vector3f &vector)
	{
		return ((x != vector.x) || (y != vector.y) || (z != vector.z)) ? true : false;
	}	
	//////////////////
	friend std::ostream& operator << (std::ostream& os, const Vector3f & vector)
	{
		os << vector.x << " " << vector.y << " " << vector.z;
		return os;		
	}	
	//////////////////
	float& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
		}
		return x;
	}
	//////////////////
	const float& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
		}		
		return x;
	}		
	//////////////////	
	// Properties
    float x, y, z;
	Vector2f xy;
};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector4f
{
    Vector4f() : x(0.0), y(0.0), z(0.0), w(0.0) 
	{}
    //////////////////
	Vector4f(float number)
	{
		x = number;
		y = number;
		z = number;
		w = number;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}
	//////////////////
	Vector4f(float x, float y, float z, float w) 
    {
		this->x = x;
		this->y = y;
	    this->z = z;
		this->w = w;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
    }
	//////////////////
	Vector4f(const Vector2f &vector) // might remove or not
	{
		x = vector.x;
		y = vector.y;
		z = 0.0;
		w = 1.0;//0.0;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}	
	//////////////////
	Vector4f(const Vector2f &a, const Vector2f &b)
	{
		x = a.x;
		y = a.y;
		z = b.x;
		w = b.y;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}
	//////////////////
	Vector4f(const Vector2f& vector, float z, float w) 
	{
		x = vector.x;
		y = vector.y;
		this->z = z;
		this->w = w;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}		
	//////////////////
	Vector4f(float x, float y, const Vector2f& vector) 
	{
		this->x = x;
		this->y = y;
		z = vector.x;
		w = vector.y;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}
	//////////////////
	Vector4f(float x, const Vector2f &vector, float w) 
	{
		this->x = x;
		y = vector.x;
		z = vector.y;
		this->w = w;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}		
	//////////////////
	Vector4f(const Vector3f &vector) // might remove or not
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = 1.0;//0.0;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}	
	//////////////////
	Vector4f(const Vector3f &vector, float w)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		this->w = w;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);
	}	
	//////////////////
	Vector4f(float x, const Vector3f &vector)
	{
		this->x = x;
		y = vector.x;
		z = vector.y;
		w = vector.z;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);		
	}	
	//////////////////
	Vector4f(const Vector4f &vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
		
		xy  = Vector2f(x, y);
		xyz = Vector3f(x, y, z);		
	}
	//////////////////
	~Vector4f(void){}
	//////////////////
	
	//////////////////
	float& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
			case 3: return w; break;
		}
		return x;
	}
	//////////////////
	const float& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
			case 3: return w; break;
		}		
		return x;
	}		
	//////////////////
	
	float x, y, z, w;
	Vector2f xy;
	Vector3f xyz;
};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector2i
{
	//////////////////
	// Constructor(s)
    Vector2i() : x(0), y(0) {}
	//////////////////
	Vector2i(int number)
	{
		x = number;
		y = number;
	}
	//////////////////
    Vector2i(int x, int y) 
    {
	    this->x = x;
	    this->y = y;
    }
	//////////////////
	Vector2i(const Vector2i& vector)
	{
		x = vector.x;
		y = vector.y;
	}
	//////////////////
	// Destructor
	~Vector2i(void){}
	//////////////////
	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	//////////////////
	void set(const Vector2i& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
	}
	//////////////////
	Vector2i get()
	{
		return (*this);
	}
	//////////////////
	double length() const
	{
		return sqrt(x * x + y * y);
	}
	//////////////////
	static double length(Vector2i& vector)
	{
		return vector.length();
	}	
	//////////////////
	Vector2i radians() const
	{
		return Vector2i(x * (3.14/180), y * (3.14/180));
	}
	//////////////////
    Vector2i normalize() 
	{
		double len = length();
		if(len != 0) 
		{
			x = x / len;
			y = y / len;
		}
		return (*this);
	}
    //////////////////
    Vector2i normal() const
	{
		Vector2i vector;
		if(length() != 0)
		{
		    vector.x = x / length();
		    vector.y = y / length();
		}
		return vector;
	}	
	//////////////////
	double dot(const Vector2i &vector) const
	{
		return (x * vector.x) + (y * vector.y);
	}
	//////////////////
	double dot(int x, int y) const
	{
		return (this->x * x) + (this->y * y);
	}
	//////////////////
	double cross(const Vector2i &vector) const
	{
		return (x * vector.y) - (y * vector.x);  
	}
	//////////////////	
	//////////////////
	// Opertator overloading
	//////////////////
	Vector2i operator + (const Vector2i &vector) const
	{
		return Vector2i(x + vector.x, y + vector.y);
	}
	//////////////////
	Vector2i operator + (int number) const 
	{
		return Vector2i(x + number, y + number);
	}	
	//////////////////
	Vector2i operator - (const Vector2i &vector) const
	{
		return Vector2i(x - vector.x, y - vector.y);
	}
	//////////////////
	Vector2i operator - (int number) const 
	{
		return Vector2i(x - number, y - number);
	}	
	//////////////////
	Vector2i operator * (const Vector2i &vector) const
	{
		return Vector2i(x * vector.x, y * vector.y);
	}
	//////////////////
	Vector2i operator * (int number) const 
	{
		return Vector2i(x * number, y * number);
	}	
	//////////////////
	Vector2i operator / (const Vector2i &vector) const
	{
		return Vector2i(x / vector.x, y / vector.y);
	}
	//////////////////
	Vector2i operator / (int number) const 
	{
		return Vector2i(x / number, y / number);
	}	
	//////////////////
	Vector2i operator -(void) const // negate
    {
        return Vector2i(-x, -y);
    }	
	//////////////////
	Vector2i operator = (const Vector2i &vector)
	{
        x = vector.x;
		y = vector.y;
		return (*this);
	}	
	//////////////////
	Vector2i operator = (int number)
	{
		x = number;
		y = number;
		return (*this);
	}	
	//////////////////
	Vector2i operator += (const Vector2i &vector)
	{
		x += vector.x;
		y += vector.y;
		return (*this);
	}	
	//////////////////
	Vector2i operator += (int number)
	{
		x += number; 
		y += number;
		return (*this);
	}
	//////////////////
	Vector2i operator -= (const Vector2i &vector)
	{
		x -= vector.x;
		y -= vector.y;
		return (*this);
	}	
	//////////////////
	Vector2i operator -= (int number)
	{
		x -= number; 
		y -= number;
		return (*this);
	}
	//////////////////
	Vector2i operator *= (const Vector2i &vector)
	{
		x *= vector.x;
		y *= vector.y;
		return (*this);
	}	
	//////////////////
	Vector2i operator *= (int number)
	{
		x *= number; 
		y *= number;
		return (*this);
	}
	//////////////////
	Vector2i operator /= (const Vector2i &vector)
	{
		x /= vector.x;
		y /= vector.y;
		return (*this);
	}	
	//////////////////
	Vector2i operator /= (int number)
	{
		x /= number; 
		y /= number;
		return (*this);
	}	
	//////////////////
    bool operator < (const Vector2i &vector) const
	{
		return ((x < vector.x) || (y < vector.y)) ? true : false;
	}	
	//////////////////
    bool operator > (const Vector2i &vector) const
	{
		return ((x > vector.x) || (y > vector.y)) ? true : false;
	}		
	//////////////////
    bool operator <= (const Vector2i &vector) const
	{
		return ((x <= vector.x) || (y <= vector.y)) ? true : false;
	}	
	//////////////////
    bool operator >= (const Vector2i &vector) const
	{
		return ((x >= vector.x) || (y >= vector.y)) ? true : false;
	}			
	//////////////////
    bool operator == (const Vector2i &vector) const
	{
		return ((x == vector.x) && (y == vector.y)) ? true : false;
	}		
	//////////////////
    bool operator != (const Vector2i &vector) const
	{
		return ((x != vector.x) || (y != vector.y)) ? true : false;
	}	
	//////////////////
	int& operator [] (unsigned int number) // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
		}
		return x;
	}
	//////////////////
	const int& operator [] (unsigned int number) const // added 9-18-2018
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
		}		
		return x;
	}	
	//////////////////
	friend std::ostream& operator << (std::ostream& os, const Vector2i & vector)
	{
		os << vector.x << " " << vector.y;
		return os;		
	}	
    // properties
	int x;
    int y;
	// no need for xy (the whole Vector2 can be used)	
};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector3i
{
	//////////////////
	// Constructor(s)
    Vector3i() : x(0), y(0), z(0), xy(0, 0) {}
	//////////////////
	Vector3i(int number)
	{
		x = number;
		y = number;
		z = number;
		
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);
	}
	//////////////////
    Vector3i(int x, int y, int z) 
    {
	    this->x = x;
	    this->y = y;
	    this->z = z;
	    
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);	    
    }    
	//////////////////
	Vector3i(const Vector2i& vector)
	{
		x = vector.x;
		y = vector.y;
		// z stays the same
		
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);		
	}
	//////////////////
	Vector3i(const Vector3i& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);		
	}	
	//////////////////
	// Destructor
	~Vector3i(void) {}
	//////////////////
	void set(int number) 
	{
		x = number;
		y = number;
		z = number;
		
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);
	}
	//////////////////	
	void set(int x, int y, int z)
	{
		this->x = x;
		this->y = y;
		this->z = z;

		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);		
	}
	//////////////////
	void set(const Vector2i& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		// z stays the same
		
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);		
	}
	//////////////////
	void set(const Vector3i& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		
		xy = Vector2i(x, y);
		//rgb = Vector3i(x, y, z);		
	}	
	//////////////////
	Vector3i get()
	{
		return (*this);
	}
	//////////////////
	/*double length() const
	{
		return sqrt(x * x + y * y);
	}
	//////////////////
	static double length(Vector2i& vector)
	{
		return vector.length();
	}	
	//////////////////
	Vector2i radians() const
	{
		return Vector2i(x * (3.14/180), y * (3.14/180));
	}
	//////////////////
    Vector2i normalize() 
	{
		double len = length();
		if(len != 0) 
		{
			x = x / len;
			y = y / len;
		}
		return (*this);
	}
    //////////////////
    Vector2i normal() const
	{
		Vector2i vector;
		if(length() != 0)
		{
		    vector.x = x / length();
		    vector.y = y / length();
		}
		return vector;
	}	
	//////////////////
	double dot(const Vector3i &vector) const
	{
		return (x * vector.x) + (y * vector.y);
	}
	//////////////////
	double dot(int x, int y) const
	{
		return (this->x * x) + (this->y * y);
	}
	//////////////////
	double cross(const Vector3i &vector) const
	{
		return (x * vector.y) - (y * vector.x);  
	}*/
	//////////////////	
	//////////////////
	// Opertator overloading
	//////////////////
	/*Vector3i operator + (const Vector2i &vector) const
	{
		return Vector2i(x + vector.x, y + vector.y);
	}
	//////////////////
	Vector3i operator + (int number) const 
	{
		return Vector2i(x + number, y + number);
	}	
	//////////////////
	Vector3i operator - (const Vector2i &vector) const
	{
		return Vector2i(x - vector.x, y - vector.y);
	}
	//////////////////
	Vector3i operator - (int number) const 
	{
		return Vector2i(x - number, y - number);
	}	
	//////////////////
	Vector3i operator * (const Vector2i &vector) const
	{
		return Vector2i(x * vector.x, y * vector.y);
	}
	//////////////////
	Vector3i operator * (int number) const 
	{
		return Vector2i(x * number, y * number);
	}	
	//////////////////
	Vector3i operator / (const Vector2i &vector) const
	{
		return Vector2i(x / vector.x, y / vector.y);
	}
	//////////////////
	Vector3i operator / (int number) const 
	{
		return Vector2i(x / number, y / number);
	}	
	//////////////////
	Vector3i operator -(void) const // negate
    {
        return Vector2i(-x, -y);
    }*/	
	//////////////////
	Vector3i operator = (const Vector3i &vector)
	{
        x = vector.x;
		y = vector.y;
		z = vector.z;
		return (*this);
	}	
	//////////////////
	Vector3i operator = (int number)
	{
		x = number;
		y = number;
		z = number;
		return (*this);
	}	
	//////////////////
	Vector3i operator += (const Vector3i &vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return (*this);
	}	
	//////////////////
	Vector3i operator += (int number)
	{
		x += number; 
		y += number;
		z += number;
		return (*this);
	}
	//////////////////
	Vector3i operator -= (const Vector3i &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return (*this);
	}	
	//////////////////
	Vector3i operator -= (int number)
	{
		x -= number; 
		y -= number;
		z -= number;
		return (*this);
	}
	//////////////////
	Vector3i operator *= (const Vector3i &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		return (*this);
	}	
	//////////////////
	Vector3i operator *= (int number)
	{
		x *= number; 
		y *= number;
		z *= number;
		return (*this);
	}
	//////////////////
	Vector3i operator /= (const Vector3i &vector)
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		return (*this);
	}	
	//////////////////
	Vector3i operator /= (int number)
	{
		x /= number; 
		y /= number;
		z /= number;
		return (*this);
	}	
	//////////////////
    bool operator < (const Vector3i &vector) const
	{
		return ((x < vector.x) || (y < vector.y) || (z < vector.z)) ? true : false;
	}	
	//////////////////
    bool operator > (const Vector3i &vector) const
	{
		return ((x > vector.x) || (y > vector.y) || (z > vector.z)) ? true : false;
	}		
	//////////////////
    bool operator <= (const Vector3i &vector) const
	{
		return ((x <= vector.x) || (y <= vector.y) || (z <= vector.z)) ? true : false;
	}	
	//////////////////
    bool operator >= (const Vector3i &vector) const
	{
		return ((x >= vector.x) || (y >= vector.y) || (z >= vector.z)) ? true : false;
	}
	//////////////////
    bool operator == (const Vector3i &vector) const
	{
		return ((x == vector.x) && (y == vector.y) && (z == vector.z));// ? true : false;
	}		
	//////////////////
    bool operator == (const Vector3 &vector) const // bonus
	{
		return ((x == static_cast<int>(vector.x)) && (y == static_cast<int>(vector.y)) && (z == static_cast<int>(vector.z))) ? true : false;
	}	
	//////////////////
    bool operator != (const Vector3i &vector) const
	{
		return ((x != vector.x) || (y != vector.y) || (z != vector.z)) ? true : false;
	}
	//////////////////
    bool operator != (const Vector3 &vector) const // bonus
	{
		return ((x != static_cast<int>(vector.x)) || (y != static_cast<int>(vector.y)) || (z != static_cast<int>(vector.z))) ? true : false;
	}		
	//////////////////
	int& operator [] (unsigned int number) 
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
		}
		return x;
	}
	//////////////////
	const int& operator [] (unsigned int number) const 
	{
		switch(number)
		{
			case 0: return x; break;
			case 1: return y; break;
			case 2: return z; break;
		}		
		return x;
	}	
	//////////////////
	friend std::ostream& operator << (std::ostream& os, const Vector3i & vector)
	{
		os << vector.x << " " << vector.y << " " << vector.z;
		return os;		
	}	
    // properties
	int x;
    int y;
    int z;
    Vector2i xy;
	// no need for xyz (the whole Vector3i can be used)
};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector4i
{};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector2b
{};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector3b
{};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
struct Vector4b
{};
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////

/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////

/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
/////////////////////
#endif
// coming soon iVector2, 3, 4(integer), bVector2, 3, 4(boolean), fVector2, 3, 4(float) 
#endif
