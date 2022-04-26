#include "../include/shader.hpp"

Shader::Shader() : program(0), vertex_shader(0), fragment_shader(0), geometry_shader(0), tess_control_shader(0), tess_evaluation_shader(0), compute_shader(0), source_list({}) {} // it's best to create the shaders manually than in the constructor ;}
////////////////////
Shader::~Shader() {	
	destroy(); // destroy program (shaders are detached and destroyed as well)
}
////////////////////
////////////////////
// normal
////////////////////
void Shader::use() {
#ifdef DOKUN_OPENGL
    context_check();
	if(!glIsProgram(program)) return; // if no program, exit function
	glUseProgram(program);
	//dokun::Logger("Program " + String(static_cast<int>(program)).str() + " is enabled");
#endif	
}
////////////////////
void Shader::disable() {
#ifdef DOKUN_OPENGL
    context_check();
	glUseProgram(0);
	//dokun::Logger("Program is disabled");
#endif	
}
////////////////////
void Shader::create() {
#ifdef DOKUN_OPENGL
    context_check();
    if(glIsProgram(program)) {std::cout << DOKUN_UI_TAG "\033[1;33mShader::create() : shader program already exists\033[0m" << std::endl; return;}// if program already exists, exit function
	// create program
	program = glCreateProgram();
	//dokun::Logger("Program" + String(static_cast<int>(program)).str() + " has been created");
	// create shaders (for this program)
	vertex_shader          = glCreateShader(GL_VERTEX_SHADER         );
	fragment_shader        = glCreateShader(GL_FRAGMENT_SHADER       );
	geometry_shader        = glCreateShader(GL_GEOMETRY_SHADER       );
	tess_control_shader    = glCreateShader(GL_TESS_CONTROL_SHADER   );
	tess_evaluation_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	compute_shader         = glCreateShader(GL_COMPUTE_SHADER        ); // ONLY IN OPENGL 4.3
	//dokun::Logger(String("Shaders created in program ") + String(static_cast<int>(program)).str());
#endif	
}
////////////////////
void Shader::compile(int shader_type)
{
#ifdef DOKUN_OPENGL
    context_check();
    if(!glIsProgram(program)) return; // if no program, exit function
    switch(shader_type) // or type
	{
		    case 0:
		    {
		        if(glIsShader(vertex_shader))
	            {
					glCompileShader(vertex_shader);         // compile shader	
			        if(!is_compiled(vertex_shader)) throw std::runtime_error("\033[1;91mShader::compile(): vertex shader compilation failed\033[0m");           // check for error
				}
		    } break;
		    case 1:
		    {
		        if(glIsShader(fragment_shader))
	            {
					glCompileShader(fragment_shader);         // compile shader	
			        if(!is_compiled(fragment_shader)) throw std::runtime_error("\033[1;91mShader::compile(): fragment shader compilation failed\033[0m");           // check for error
				}
		    } break;
		    case 2:
		    {
		        if(glIsShader(geometry_shader))
	            {
					glCompileShader(geometry_shader);         // compile shader	
			        if(!is_compiled(geometry_shader)) throw std::runtime_error("\033[1;91mShader::compile(): geometry shader compilation failed\033[0m");           // check for error
				}                    
		    } break;			
		    case 3:
		    {
		        if(glIsShader(tess_control_shader))
	            {
					glCompileShader(tess_control_shader);         // compile shader	
			        if(!is_compiled(tess_control_shader)) throw std::runtime_error("\033[1;91mShader::compile(): tess_control shader compilation failed\033[0m");           // check for error
				}                    
		    } break;
		    case 4:
		    {
		        if(glIsShader(tess_evaluation_shader))
	            {
					glCompileShader(tess_evaluation_shader);         // compile shader	
			        if(!is_compiled(tess_evaluation_shader)) throw std::runtime_error("\033[1;91mShader::compile(): tess_evaluation shader compilation failed\033[0m");           // check for error
				}                    
		    } break;
		    case 5:
		    {
		        if(glIsShader(compute_shader))
	            {
					glCompileShader(compute_shader);         // compile shader	
			        if(!is_compiled(compute_shader)) throw std::runtime_error("\033[1;91mShader::compile(): compute shader compilation failed\033[0m");           // check for error
				}                    
		    } break;					
	}
#endif
}
////////////////////
void Shader::attach(int shader_type)
{
#ifdef DOKUN_OPENGL
    context_check();
    if(!glIsProgram(program)) return; // if no program, exit function
    switch(shader_type) // or type
	{
		    case 0:
		    {
		        if(glIsShader(vertex_shader))
                    glAttachShader(program, vertex_shader); // attach shader
		    } break;
		    case 1:
		    {
		        if(glIsShader(fragment_shader))
                    glAttachShader(program, fragment_shader); // attach shader
		    } break;
		    case 2:
		    {
		        if(glIsShader(geometry_shader))
                    glAttachShader(program, geometry_shader); // attach shader
		    } break;			
		    case 3:
		    {
		        if(glIsShader(tess_control_shader))
                    glAttachShader(program, tess_control_shader); // attach shader
		    } break;
		    case 4:
		    {
		        if(glIsShader(tess_evaluation_shader))
                    glAttachShader(program, tess_evaluation_shader); // attach shader
		    } break;
		    case 5:
		    {
		        if(glIsShader(compute_shader))
                    glAttachShader(program, compute_shader); // attach shader
		    } break;			
	}
#endif	
}
////////////////////
void Shader::link(void) {
#ifdef DOKUN_OPENGL
    context_check();
    if(!glIsProgram(program)) return; // if no program, exit function
    glLinkProgram(program); // Requires at least two shaders. // to link a program
    if(!is_linked()) throw std::runtime_error("\033[1;91mShader::link(): shader program linkage failed\033[0m"); // check for error
#endif	
}
////////////////////
void Shader::prepare()
{
#ifdef DOKUN_OPENGL
    context_check();
    if(!glIsProgram(program)) return; // if no program, exit function
	// vertex_shader
	if(glIsShader(vertex_shader) && source_list.size() >= 1) // and source_list.size() >= 1
	{	
		glCompileShader(vertex_shader);	        // compile shader			
        if(!is_compiled(vertex_shader))	throw std::runtime_error("\033[0;91mShader::prepare(): vertex shader compilation failed\033[0m"); // check for error
		glAttachShader(program, vertex_shader); // attach shader				//std::cout << DOKUN_UI_TAG "Shader " + String(static_cast<int>(0)).str() + " has been attached to program " + String(static_cast<int>(program)).str() << std::endl;//);
	}
	// fragment_shader
	if(glIsShader(fragment_shader) && source_list.size() >= 2)
	{
		glCompileShader(fragment_shader);         // compile shader	
		if(!is_compiled(fragment_shader)) throw std::runtime_error("\033[0;91mShader::prepare(): fragment shader compilation failed\033[0m"); // check for error
		glAttachShader(program, fragment_shader); // attach shader				//std::cout << DOKUN_UI_TAG "Shader " + String(static_cast<int>(1)).str() + " has been attached to program " + String(static_cast<int>(program)).str() << std::endl;//);
	}
	// link program
    link();
#endif	
}
////////////////////
void Shader::destroy()
{
#ifdef DOKUN_OPENGL
    context_check();
    if(!glIsProgram(program)) return; // if no program exists, exit function
	if(glIsShader(vertex_shader)) 
	{
		glDetachShader(program, vertex_shader); // detach shader
		glDeleteShader(vertex_shader);          // delete shader
		//dokun::Logger("Vertex Shader of program " + String(static_cast<int>(program)).str() + " has been detached and deleted");
	}
	if(glIsShader(fragment_shader))
	{
		glDetachShader(program, fragment_shader); // detach shader
		glDeleteShader(fragment_shader);          // delete shader
		//dokun::Logger("Fragment Shader of program " + String(static_cast<int>(program)).str() + " has been detached and deleted");
	}
	if(glIsShader(geometry_shader))
	{
		glDetachShader(program, geometry_shader); // detach shader
		glDeleteShader(geometry_shader);          // delete shader
		//dokun::Logger("Geometry Shader of program " + String(static_cast<int>(program)).str() + " has been detached and deleted");
	}		
	if(glIsShader(tess_control_shader)) 
	{
		glDetachShader(program, tess_control_shader); // detach shader
		glDeleteShader(tess_control_shader);          // delete shader
		//dokun::Logger("Tess Control Shader of program " + String(static_cast<int>(program)).str() + " has been detached and deleted");
	}
	if(glIsShader(tess_evaluation_shader))
	{
		glDetachShader(program, tess_evaluation_shader); // detach shader
		glDeleteShader(tess_evaluation_shader);          // delete shader
		//dokun::Logger("Tess Evaluation Shader of program " + String(static_cast<int>(program)).str() + " has been detached and deleted");
	}
	if(glIsShader(compute_shader)) 
	{
		glDetachShader(program, compute_shader); // detach shader
		glDeleteShader(compute_shader);          // delete shader
		//dokun::Logger("Compute Shader of program " + String(static_cast<int>(program)).str() + " has been detached and deleted");
	}
	glDeleteProgram(program); // delete program after detaching and deleting shaders
	//dokun::Logger("Program " + String(static_cast<int>(program)).str() + " is destroyed");
    source_list.clear(); // clear source_list
#endif	
}
////////////////////
void Shader::bind(const std::string& attribute_name, unsigned int index)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(!glIsProgram(program)) return; // program is invalid, exit function
	glBindAttribLocation(program, index, attribute_name.c_str());
#endif
}
////////////////////
bool Shader::load(const std::string& filename, int shader_type)
{
    std::ifstream file(filename.c_str());
	if(!file.is_open()){dokun::Logger("Could not load " + filename);return false;	}
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents into stream
	file.close();           // close file
	const char * content = stream.str().c_str(); // copy string from stream
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) return false;
#endif
#ifdef DOKUN_LINUX
#ifdef DOKUN_X11
    if(!glXGetCurrentContext()) return false;
#endif
#endif	
    switch(shader_type) // or type
	{
		case 0: 
		{
	        if(glIsShader(vertex_shader)) {
	            glShaderSource(vertex_shader, 1, &content, nullptr);
		        source_list.push_back(stream.str().c_str()); // store in string_vector
	        }
		} break;
		case 1: 
		{
	        if(glIsShader(fragment_shader))	{
	            glShaderSource(fragment_shader, 1, &content, nullptr);
		        source_list.push_back(stream.str().c_str()); // store in string_vector
	        }
		} break;
		case 2: 
		{
	        if(glIsShader(geometry_shader))	{
	            glShaderSource(geometry_shader, 1, &content, nullptr);
		        source_list.push_back(stream.str().c_str()); // store in string_vector
	        }
		} break;	/*
		case 0: {
	        if(glIsShader(shader))	
	        {
	            glShaderSource(shader, 1, &content, nullptr);
		        source_list.push_back(stream.str().c_str()); // store in string_vector
	        }
		} break;*/			
    }
#endif	
	return true;	
}
////////////////////
// setters
void Shader::set_source(const char * const * source, int shader_type)
{
#ifdef DOKUN_OPENGL
    context_check();
    switch(shader_type) // or type
	{
		case 0:
		{
		    if(glIsShader(vertex_shader))
	        {
	        glShaderSource(vertex_shader, sizeof(source)/sizeof(source), source, nullptr);
		    const char * content = * source;
		    source_list.push_back(content); // store in string_vector
	        }
		} break;
		case 1:
		{
		    if(glIsShader(fragment_shader))
	        {
	        glShaderSource(fragment_shader, sizeof(source)/sizeof(source), source, nullptr);
		    const char * content = * source;
		    source_list.push_back(content); // store in string_vector
	        }
		} break;
		case 2:
		{
		    if(glIsShader(geometry_shader))
	        {
	        glShaderSource(geometry_shader, sizeof(source)/sizeof(source), source, nullptr);
		    const char * content = * source;
		    source_list.push_back(content); // store in string_vector
	        }
		} break;/*
		case 0:
		{
		    if(glIsShader(shader))
	        {
	        glShaderSource(shader, sizeof(source)/sizeof(source), source, nullptr);
		    const char * content = * source;
		    source_list.push_back(content); // store in string_vector
	        }
		} break;*/			
	}
#endif	
}
////////////////////
void Shader::set_integer(const std::string& uniform_name, int integer)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
	    glUniform1i(uniform_location, integer);		
	}
#endif	
}
void Shader::set_integer(const std::string& uniform_name, int x, int y)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
       glUniform2i(uniform_location, x, y);		
	}
#endif	
}
void Shader::set_integer(const std::string& uniform_name, int x, int y, int z)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
       glUniform3i(uniform_location, x, y, z);		
	}
#endif	
}
void Shader::set_integer(const std::string& uniform_name, int x, int y, int z, int w)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
       glUniform4i(uniform_location, x, y, z, w);		
	}	
#endif	
}
void Shader::set_float(const std::string& uniform_name, float floatp)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform1f(uniform_location, floatp);		
	}	
#endif	
}
void Shader::set_float(const std::string& uniform_name, float x, float y)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform2f(uniform_location, x, y);		
	}		
#endif	
}
void Shader::set_float(const std::string& uniform_name, float x, float y, float z)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform3f(uniform_location, x, y, z);		
	}	
#endif	
}
void Shader::set_float(const std::string& uniform_name, float x, float y, float z, float w)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform4f(uniform_location, x, y, z, w);		
	}		
#endif	
}
void Shader::set_double(const std::string& uniform_name, double doublep)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform1f(uniform_location, static_cast<GLfloat>(doublep));		
	}	
#endif	
}
void Shader::set_double(const std::string& uniform_name, double x, double y)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform2f(uniform_location, static_cast<GLfloat>(x), static_cast<GLfloat>(y));		
	}			
#endif	
}
void Shader::set_double(const std::string& uniform_name, double x, double y, double z)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform3f(uniform_location, static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z));		
	}	
#endif	
}
void Shader::set_double(const std::string& uniform_name, double x, double y, double z, double w)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform4f(uniform_location, static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z), static_cast<GLfloat>(w));		
	}	
#endif	
}
void Shader::set_vector2(const std::string& uniform_name, const Vector2& vector)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform2f(uniform_location, static_cast<GLfloat>(vector.x), static_cast<GLfloat>(vector.y));		
	}
#endif	
}
void Shader::set_vector3(const std::string& uniform_name, const Vector3& vector)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform3f(uniform_location, static_cast<GLfloat>(vector.x), static_cast<GLfloat>(vector.y), static_cast<GLfloat>(vector.z));		
	}	
#endif	
}
void Shader::set_vector4(const std::string& uniform_name, const Vector4& vector)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform4f(uniform_location, static_cast<GLfloat>(vector.x), static_cast<GLfloat>(vector.y), static_cast<GLfloat>(vector.z), static_cast<GLfloat>(vector.w));		
	}		
#endif	
}/*
void Shader::set_matrix2(const std::string& uniform_name, const Matrix2& matrix)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger("Uniform '" + uniform_name + "' is not valid");
		    return;}
       //glUniformMatrixfv(uniform_location, 1, false, static_cast<const GLfloat *>(matrix.data()));		
	}
#endif	
}
void Shader::set_matrix3(const std::string& uniform_name, const Matrix3& matrix)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger("Uniform '" + uniform_name + "' is not valid"); return;}
       //glUniformMatrixfv(uniform_location, 1, false, static_cast<const GLfloat *>(matrix.data()));		
	}
#endif	
}*/
/*void Shader::set_matrix4(const std::string& uniform_name, int count, const Matrix4& matrix)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniform4fv(uniform_location, count, (const GLfloat *)&matrix.value[0][0]);//view0.data());
	}
#endif	
}
void Shader::set_matrix4(const std::string& uniform_name, int count, bool transpose, const Matrix4& matrix)
{
#ifdef DOKUN_OPENGL
    context_check();
	if(glIsProgram(program))
	{
        GLint uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(uniform_location == -1) {dokun::Logger::push("Uniform '" + uniform_name + "' is not valid"); return;}
        glUniformMatrix4fv(uniform_location, count, transpose, (const GLfloat *)&matrix.value[0][0]);		
	}
#endif	
}*/	
////////////////////
// getters
////////////////////
unsigned int Shader::get_program() const
{
	return program;
}
////////////////////
//unsigned int Shader::get_shader(int index) const
//{
	//return shader_list[index];
//}
////////////////////
unsigned int Shader::get_shader_count() const
{
	return source_list.size(); // number of shader sources
}
////////////////////
std::string Shader::get_source(int shader_index)const
{
	if(source_list.empty()) {
	    std::cout << DOKUN_UI_TAG "No source found for shader program (" << program << ")" << std::endl;
	    return ""; //error // if no source, exit function
	}
	if(shader_index > (source_list.size() - 1)) throw std::runtime_error("\033[0;91mShader::get_source(int) : Attempt to access invalid location or index\033[0m");	
	return source_list[shader_index];
}
////////////////////         
//static int get_source(lua_State *L);
////////////////////
unsigned int Shader::get_attribute(const std::string& attribute)const
{
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) return -1;
#endif
#ifdef DOKUN_LINUX
#ifdef DOKUN_X11
    if(!glXGetCurrentContext()) return -1;
#endif
#endif
    if(glIsProgram(program))
        return glGetAttribLocation(program, attribute.c_str());
	return -1;
#endif	
}    //static int get_attribute(lua_State *L); // return attrib location
////////////////////
unsigned int Shader::get_uniform(const std::string& uniform)const
{
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) return -1;
#endif
#ifdef DOKUN_LINUX
#ifdef DOKUN_X11
    if(!glXGetCurrentContext()) return -1;
#endif
#endif	
	if(glIsProgram(program))
		return glGetUniformLocation(program, uniform.c_str());
	return -1; // -1 cannot be unsigned :O!! <- fix this soon by changing function type to a normal int!
#endif	
}               //static int get_uniform(lua_State *L); // return uniform location
////////////////////
// boolean
////////////////////
bool Shader::has_program()const
{
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) return false;
#endif
#ifdef DOKUN_LINUX
#ifdef DOKUN_X11
    if(!glXGetCurrentContext()) return false;
#endif
#endif	
	return (glIsProgram(program) != 0);
#endif	
}
////////////////////
bool Shader::is_compiled(unsigned int shader) const {
#ifdef DOKUN_OPENGL	
    if(!glIsProgram(program)) return false; // if no program, return false
	// check for error (with shader compilation)
    GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
	    GLint infoLogLength;
	    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength );
	    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
	    glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog );
	    std::cout << DOKUN_UI_TAG "\033[1;92m[glsl]: \033[1;37m" << String(strInfoLog) << "\033[0m" << std::endl;
	    delete[] strInfoLog;
	}
	return (status == GL_TRUE);
#endif	
    return false; // default return value (in case no graphics API is being used)
}
////////////////////
bool Shader::is_linked(void) const {
#ifdef DOKUN_OPENGL	
    if(!glIsProgram(program)) return false; // if no program, return false
    // check for error (with linking)
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status != GL_TRUE) {
	    char buffer[512];
        glGetProgramInfoLog(program, 512, nullptr, buffer);
        std::cout << std::string(DOKUN_UI_TAG "\033[1;92m[glsl]: \033[1;37m") + buffer << "\033[0m" << std::endl;
    }
    return (status == GL_TRUE);
#endif	
    return false; // default return value (in case no graphics API is being used)
}
////////////////////
void Shader::context_check(void) {
#ifdef DOKUN_OPENGL
#ifdef DOKUN_WIN32
	if(!wglGetCurrentContext()) {
		throw std::runtime_error("\033[1;91mContext check failed: No OpenGL context found.\033[0m");
	}
#endif
#ifdef DOKUN_LINUX
#ifdef DOKUN_X11
    if(!glXGetCurrentContext()) {
		throw std::runtime_error("\033[1;91mContext check failed: No OpenGL context found.\033[0m");
	}
#endif
#endif	
#endif	
}
