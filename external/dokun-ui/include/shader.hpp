#ifndef SHADER_HPP_DOKUN
#define SHADER_HPP_DOKUN

#include "platform.hpp"
#include "types.hpp"
#include "logger.hpp"
#include "vector.hpp"
//#include "matrix_test.hpp"

#ifdef __cplusplus // if c++ // if c++
#include <iostream>
#include <vector>
#include <lua.hpp>

class Shader { // each shader represents a program containing shaders
public:	
    Shader();
	~Shader();
	// normal
	void use(void);     // enables the program
	void disable(void); // disables the program
	void create(void);  // creates a program with a number of shaders
	void compile(int shader_type);
	void attach(int shader_type);
	void link(void);
	void prepare(void); // compiles, attaches, and links
	void destroy(void); // destroys program (shaders are detached and destroyed as well)
	void bind(const std::string& attribute_name, unsigned int index); // glBindAttribLocation
	bool load(const std::string& file, int index); // load a shader from file
	// setters
	void set_source(const char * const * source, int index);
	void set_integer(const std::string& uniform_name, int integer);
	void set_integer(const std::string& uniform_name, int x, int y);
	void set_integer(const std::string& uniform_name, int x, int y, int z);
	void set_integer(const std::string& uniform_name, int x, int y, int z, int w);
	void set_float(const std::string& uniform_name, float floatp);
	void set_float(const std::string& uniform_name, float x, float y);
	void set_float(const std::string& uniform_name, float x, float y, float z);
	void set_float(const std::string& uniform_name, float x, float y, float z, float w);
	void set_double(const std::string& uniform_name, double doublep);
    void set_double(const std::string& uniform_name, double x, double y);
    void set_double(const std::string& uniform_name, double x, double y, double z);
    void set_double(const std::string& uniform_name, double x, double y, double z, double w);
	void set_vector2(const std::string& uniform_name, const Vector2& vector);
	void set_vector3(const std::string& uniform_name, const Vector3& vector);
	void set_vector4(const std::string& uniform_name, const Vector4& vector);
	//void set_matrix2(const std::string& uniform_name, const Matrix2& matrix);
	//void set_matrix3(const std::string& uniform_name, const Matrix3& matrix);
	//void set_matrix4(const std::string& uniform_name, int count, const Matrix4& matrix);
	//void set_matrix4(const std::string& uniform_name, int count, bool transpose, const Matrix4& matrix);		
	// getters
	unsigned int get_program() const;
	unsigned int get_shader(int index) const; // index is also represents the type of shader
	unsigned int get_shader_count() const;
	std::string get_source(int shader_index)const;                 //static int get_source(lua_State *L);
    // location
	unsigned int get_attribute(const std::string& attribute)const;    //static int get_attribute(lua_State *L); // return attrib location
	unsigned int get_uniform(const std::string& uniform)const;               //static int get_uniform(lua_State *L); // return uniform location
    // boolean
	bool has_program()const;
    bool is_compiled(unsigned int shader) const;
	bool is_linked(void) const;
private:
	void context_check(void);
	unsigned int program;
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	unsigned int geometry_shader;
	unsigned int tess_control_shader;
	unsigned int tess_evaluation_shader;
	unsigned int compute_shader;
	std::vector<std::string> source_list;
};
/*
usage:
  Shader shader;
  shader.create(2); // create vertex and fragment shader
  shader.set_source(vertex_source, 0);
  shader.set_source(fragment_source, 1);
  shader.prepare(); // prepare shader
  
  shader.use(); // use program
  shader.disable(); // disable program
*/
#endif
#endif
