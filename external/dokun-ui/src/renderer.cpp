#include "../include/renderer.hpp"

Renderer::Renderer() // don't initialize anything in constructor or Renderer will crash
{}
////////////
Renderer::~Renderer()
{}
////////////
////////////
Renderer * Renderer::render_ptr (new Renderer());
////////////
Vector4 Renderer::color(32, 32, 32, 255.0);
////////////
unsigned int Renderer::window_width (DOKUN_DEFAULT_WIDTH );
unsigned int Renderer::window_height(DOKUN_DEFAULT_HEIGHT);	
////////////
bool Renderer::status     (false); 
////////////
std::string Renderer::API ("OpenGL");
////////////
unsigned int Renderer::sprite_vertex_array_obj   (0);
unsigned int Renderer::sprite_vertex_buffer_obj  (0);
unsigned int Renderer::sprite_texcoord_buffer_obj(0);
unsigned int Renderer::sprite_element_buffer_obj (0);

unsigned int Renderer::model_vertex_array_obj    (0);
////////////
////////////
////////////
////////////
////////////
////////////
////////////
void Renderer::start(void){ // initialize default values for renderer
}
void Renderer::destroy (void){ // call this function to deallocate attached renderer objects before deleting renderer
}
void Renderer::generate_vertex_array(float x, float y, unsigned int width, unsigned int height) // sprites not showing up when using this :(
{
#ifdef DOKUN_OPENGL
    if(!glIsVertexArray(sprite_vertex_array_obj)) // if vao has not been generated
	{
	// vertex array obj -
    glGenVertexArrays(1, &sprite_vertex_array_obj);
    // vertex buffer obj
	glBindVertexArray(sprite_vertex_array_obj); // bind	
	    GLuint sprite_vertex_buffer_obj;
	    glGenBuffers(1, &sprite_vertex_buffer_obj);
		std::vector<GLfloat> vertex_array; // vertex array (C++) 
		vertex_array.push_back(x);
		vertex_array.push_back(y);
		vertex_array.push_back(x + static_cast<GLfloat>(width));
		vertex_array.push_back(y);
		vertex_array.push_back(x + static_cast<GLfloat>(width));
		vertex_array.push_back(y + static_cast<GLfloat>(height));
		vertex_array.push_back(x);
		vertex_array.push_back(y + static_cast<GLfloat>(height));
		/*GLfloat vertex_array[] ={ // vertex array (C)
			static_cast<float>(x)                                     , static_cast<float>(y),
		    static_cast<float>(x) + static_cast<GLfloat>(width)       , static_cast<float>(y),
	 	    static_cast<float>(x) + static_cast<GLfloat>(width)       , static_cast<float>(y) + static_cast<GLfloat>(height),
		    static_cast<float>(x)                                     , static_cast<float>(y) + static_cast<GLfloat>(height),
		};*/
        glBindBuffer(GL_ARRAY_BUFFER, sprite_vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, vertex_array.size()/*sizeof(vertex_array)*/ * sizeof(GLfloat), /*vertex_array*/vertex_array.data(), GL_DYNAMIC_DRAW); // DYNAMIC cause it will update often
		glEnableVertexAttribArray(0); // vertex attribute ptr
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));	
	glBindVertexArray(0); // unbind
	// texcoord buffer obj
	glBindVertexArray(sprite_vertex_array_obj); // bind again  
	    GLuint sprite_texcoord_buffer_obj;
        glGenBuffers(1, &sprite_texcoord_buffer_obj);
		GLfloat tex_coord[] = {0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0,};
        glBindBuffer(GL_ARRAY_BUFFER, sprite_texcoord_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord) * sizeof(GLfloat), tex_coord, GL_STATIC_DRAW); 
		glEnableVertexAttribArray(1); // vertex attribute ptr
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));			
	glBindVertexArray(0); // unbind	
	// element buffer 
	glBindVertexArray(sprite_vertex_array_obj); // bind
	    GLuint sprite_element_buffer_obj;
		glGenBuffers(1, &sprite_element_buffer_obj);
		GLuint element[] = {0, 1, 3,  1, 2, 3};//{0,1,2, 2,3,0};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_element_buffer_obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element) * sizeof(GLuint), element, GL_STATIC_DRAW);
	glBindVertexArray(0); // unbind	
	dokun::Logger("Sprite vertex array and buffer objects generated");
	}
#endif	
}
////////////
void Renderer::update_vertex_array(const std::vector<float>& vertex_array)
{
#ifdef DOKUN_OPENGL
	// Update vertex_buffer_obj (each frame) NOT the VAO!!
	// This code is equivalent to glNamedBufferSubData
    glBindBuffer(GL_ARRAY_BUFFER, sprite_vertex_buffer_obj);
	    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_array.size() * sizeof(GLfloat), vertex_array.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glNamedBufferSubData(sprite_vertex_buffer_obj, 0, vertex_array.size() * sizeof(GLfloat), vertex_array.data()); //dokun::Logger("Sprite vertex data updated");// updates buffer_obj without having to bind it	
#endif	
}
////////////
void Renderer::destroy_vertex_array (void) // do not destroy vertex array until object is no longer in use or the endgine closes
{
#ifdef DOKUN_OPENGL	
if(glIsVertexArray(sprite_vertex_array_obj)) {// if vao has been generated
	// delete the buffers
	glDeleteBuffers(1, &sprite_vertex_buffer_obj  );	dokun::Logger("Sprite vertex_buffer_object deleted");
	glDeleteBuffers(1, &sprite_texcoord_buffer_obj);	dokun::Logger("Sprite texture_coordinate_buffer_object deleted");	
	glDeleteBuffers(1, &sprite_element_buffer_obj );	dokun::Logger("Sprite element_buffer_object deleted");
    // delete the arrays
	glDeleteVertexArrays(1, &sprite_vertex_array_obj);	dokun::Logger("Sprite vertex_array_object deleted");
}
#endif	
}
////////////
////////////
void Renderer::context_check(void)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
    if(get_current_API() != "OpenGL") // but you don't set it as the current API, perhaps Vulkan is the current API?
	{
		dokun::Logger("Context check failed : \
		    OpenGL not set as current API");
		return;
	}
#ifdef __windows__
    if(!wglGetCurrentContext())
	{
		dokun::Logger("Context check failed : \
		    No OpenGL Context was found.");
		return;
	}
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		dokun::Logger("Context check failed : \
		    No OpenGL Context was found.");
		return;
	}	
#endif
#endif
#endif	
}
////////////
////////////
void Renderer::device_check (void)
{
#ifdef DOKUN_VULKAN
	if(get_current_API() == "Vulkan")
	{/*
	    // Query how many devices are present in the system
	    uint32_t device_count = 0;
        VkResult result = vkEnumeratePhysicalDevices(get_pointer()->instance, &device_count, nullptr);
        if(result != VK_SUCCESS) {
            fprintf(stderr, "Failed to query the number of physical devices present: %d\n", result);
            abort();
        }
        // There has to be at least one device present
        if (device_count == 0) {
            fprintf(stderr, "Couldn't detect any device present with Vulkan support: %d\n", result);
            abort();
        }
        // Get the physical devices
        std::vector<VkPhysicalDevice> physical_devices(device_count);
        result = vkEnumeratePhysicalDevices(instance, &deviceCount, &physical_devices[0]);
        if (result != VK_SUCCESS) {
            fprintf(stderr, "Faied to enumerate physical devices present: %d\n", result);
            abort();
        }*/
	/*
		// look for devices and gpus that support vulkan
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, & extension_count, nullptr);

        std::cout << extension_count << " extensions supported" << std::endl;	
		
		VkDevice device;
		VkPhysicalDevice physical_device;
		uint32_t physical_device_count = 0;
		std::vector<VkPhysicalDevice> physical_device_list(physical_device_count);
		// check how many devices we have
		VkResult result = vkEnumeratePhysicalDevices(get_pointer()->instance, & physical_device_count, & physical_device);
		assert(result == VK_SUCCESS);
		std::cout << physical_device_count << " devices found" << std::endl;
	    // device properties
		//VkPhysicalDeviceProperties device_prop = {};
		vkGetPhysicalDeviceProperties(physical_device, & get_pointer()->device_prop);   // for single device
		*/
	}
#endif	
}
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
void Renderer::draw_image(const unsigned int buffer, int width, int height, int depth, float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, int channel, Shader* shader,
    bool outline, float outline_thickness, const Vector4& outline_color, float outline_threshold) // good !
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
	// texture
	// vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // State
	glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);
	///////////////////
	//glBlendEquation(GL_FUNC_ADD);	
    //glEnable(GL_LINE_SMOOTH); glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	// Shader
	shader->use();
#ifdef use_glm	
	// uniform	
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x, y, -1));//model = glm::translate(model, glm::vec3(x + width/2, y + height/2, -1));//
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x, -y, 1));//model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 1));
    // projection and view (camera)
	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0)); // uses its own camera (not subject to the Renderer's camera)
	// pass matrix data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "view") , 1, GL_FALSE, glm::value_ptr(view) );
#endif	
    // Draw border
	/*/////// temp ////////
	shader->set_integer("has_texture", false);//(buffer != 0));
	shader->set_float("color", (255 / 255.0), (102 / 255.0), (0 / 255.0), alpha);
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture
    /////////////////////*/        	
	// Draw image
    shader->set_integer("has_texture", (buffer != 0));
    // Draw outline
    shader->set_integer("outline", outline);
    shader->set_float("outline_threshold", outline_threshold);//0.5);
    shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, buffer);  // bind texture
	shader->set_integer("base", 0);
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	// Disable shader
	shader->disable();
#endif
}
////////////
////////////
////////////
/*
void Renderer::draw_camera(double eye_x, double eye_y, double eye_z, double center_x, double center_y,double center_z, double up_x, double up_y,double up_z, const Shader& shader)
{
#ifdef DOKUN_OPENGL
    context_check();
    center_x = eye_x + center_x;
	center_y = eye_y + center_y;
	center_z = eye_z + center_z;	
#ifdef use_glm
	glm::mat4 view = glm::lookAt(glm::vec3(eye_x, eye_y, eye_z), glm::vec3(center_x, center_y, center_z), glm::vec3(up_x, up_y, up_z));		
#endif
#ifndef use_glm
    Matrix4 view;//;
#endif
    glUseProgram(shader.get_program());
	    //glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view"),  1, false, glm::value_ptr(view));
	glUseProgram(0);
#endif
}*/
//////////////////////////////	
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/*
    const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "void main()\n"
        "{\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
        "#version 330\n"
        "\n"
        "void main()\n"
        "{\n"
        "}\n"
	}	
*/
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////

void Renderer::draw_box(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    float radius, bool iconify,
    // title bar
	bool title_bar,
	int title_bar_height,
	const Vector4& title_bar_color,
	bool title_bar_button_iconify,
    bool title_bar_button_maximize,
    bool title_bar_button_close, const Vector4& title_bar_button_close_color,
	// outline
    bool outline,
    float outline_width,
    const Vector4& outline_color,
    bool outline_antialiased,
    // border
    bool border,
    double border_radius,
    const Vector4& border_color,
	// gradient
	bool gradient,
	const Vector4& gradient_color,
	double gradient_value,
	// shadow
	bool shadow
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
	//--------------------------
	// tex_coord_array
    GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0,                0.0, // top-left (y = 0 = top)
        1.0,                0.0, // top-right (y = 0 = top)
        1.0,                1.0, // bottom-right (y = 1 = bot)
        0.0,                1.0, // bottom-left (y = 1 = bot)
    };
	// element_array
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	//--------------------------
	// TITLE_BAR
    // vertex array obj  - stores vertices
    GLuint title_bar_vertex_array_obj;
    glGenVertexArrays(1, &title_bar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(title_bar_vertex_array_obj); // bind again  
	    GLuint title_bar_tex_coord_buffer_obj;
        glGenBuffers( 1, &title_bar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, title_bar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(title_bar_vertex_array_obj); // bind vertex array obj
        GLuint title_bar_vertex_buffer_obj;
        glGenBuffers(1, &title_bar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, title_bar_vertex_buffer_obj);
		// set title_bar position relative to widget
		float title_bar_x = 0;
		float title_bar_y = -title_bar_height; //-30
		int title_bar_width  = width; // is set automatically
		//std::cout << y + title_bar_y << " title_bar_y\n"; 
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x)                                          , static_cast<GLfloat>(y + title_bar_y),
            static_cast<GLfloat>(x + title_bar_x) + static_cast<GLfloat>(title_bar_width), static_cast<GLfloat>(y + title_bar_y),
            static_cast<GLfloat>(x + title_bar_x) + static_cast<GLfloat>(title_bar_width), static_cast<GLfloat>(y + title_bar_y) + static_cast<GLfloat>(title_bar_height),
            static_cast<GLfloat>(x + title_bar_x)                                          , static_cast<GLfloat>(y + title_bar_y) + static_cast<GLfloat>(title_bar_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(title_bar_vertex_array_obj); // (vao start 1)
	    GLuint title_bar_element_buffer_obj;
        glGenBuffers(1, &title_bar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, title_bar_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// State
	glDisable(GL_DEPTH_TEST);                         // Disable 3d for User interface
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);
	// Shader       - This is the part where the shader is going to start being used (for the purpose of keeping the code organized and in place) ******
	shader->use ();    
    // set shader properties ************** Always check to see if a uniform exists (or not) before applying values to it, using "if(shader.get_uniform("") != -1)"
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0)); //model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
    // projection (ortho2d) (no 'view' because GUI should not be affected by camera)          //float window_width  = get_display_width ();//float window_height = get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f); // make sure window_width and window_height are always updated and equal to the window client's size
	// pass data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif
    // set uniforms (if uniform cannot be found or does not exist, the function will return)
	if(shader->get_uniform("resolution") != -1) shader->set_float("resolution", static_cast<double>(Renderer::get_display_width()), static_cast<double>(Renderer::get_display_height()));
	// if has_title_bar
	if(title_bar)
	{
	//glEnable(GL_DEPTH_TEST);           // for removing hidden line
	//glEnable(GL_POLYGON_OFFSET_FILL);  // for removing hidden line
	// Draw title_bar
	// title_bar : size (I don't think size is even used in shader :O)
	shader->set_vector2  ("size", Vector2(title_bar_width, title_bar_height));
	// title_bar : radius
	shader->set_float("radius", radius); // set rounded_corner	
	// title_bar : gradient
	shader->set_integer("gradient.enabled", gradient); // set gradient on
	shader->set_float  ("gradient.value"  , gradient_value); // set gradient value
	shader->set_float("gradient.color", (title_bar_color.x/255.0), (title_bar_color.y/255.0), (title_bar_color.z/255.0), title_bar_color.w);
	// title_bar : color
	shader->set_float("color", (title_bar_color.x / 255.0), (title_bar_color.y / 255.0), (title_bar_color.z / 255.0), title_bar_color.w);	
	glBindVertexArray(title_bar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDisable(GL_POLYGON_OFFSET_FILL); // for removing hidden line
	//glDisable(GL_DEPTH_TEST);          // for removing hidden line
	// What is drawn last should appear top, according to openGL, so outline will be drawn last
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	/*glLineWidth(outline_width);
	glBindVertexArray(title_bar_vertex_array_obj); // use same vao data as title_bar but this time in a line loop
	    glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/
	}      	
	//--------------------------
	// CLOSE_BUTTON
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// vertex array obj  - stores vertices
	GLuint close_button_vertex_array_obj;
    glGenVertexArrays(1, &close_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(close_button_vertex_array_obj); // bind again  
	    GLuint close_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &close_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, close_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(close_button_vertex_array_obj); // bind vertex array obj
        GLuint close_button_vertex_buffer_obj;
        glGenBuffers(1, &close_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, close_button_vertex_buffer_obj);
		// button width and height should be equal in order to create a perfect circle
		int close_button_height = title_bar_height / 2; // half of titlebar_height //title_bar_height - 5; // 5 is bottom_padding        
        int close_button_width = close_button_height;//10;//title_bar_button_width;
		float close_button_x = (title_bar_width  - close_button_width ) - 5; // 5 is the right_padding (space between close_button and titlebar's right edge)
		float close_button_y = (title_bar_height - close_button_height) / 2;//2; // 2 is top_padding // (title_bar_height - close_height) / 2
		GLfloat vertices2[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x + close_button_x)                                           , static_cast<GLfloat>(y + title_bar_y + close_button_y),
            static_cast<GLfloat>(x + title_bar_x + close_button_x) + static_cast<GLfloat>(close_button_width), static_cast<GLfloat>(y + title_bar_y + close_button_y),
            static_cast<GLfloat>(x + title_bar_x + close_button_x) + static_cast<GLfloat>(close_button_width), static_cast<GLfloat>(y + title_bar_y + close_button_y) + static_cast<GLfloat>(close_button_height),
            static_cast<GLfloat>(x + title_bar_x + close_button_x)                                           , static_cast<GLfloat>(y + title_bar_y + close_button_y) + static_cast<GLfloat>(close_button_height),   
        };		
        //std::cout << "close button position (renderer): " << Vector2(x, y) + Vector2(title_bar_x, title_bar_y) + Vector2(close_button_x, close_button_y) << std::endl;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(close_button_vertex_array_obj); // (vao start 1)
	    GLuint close_button_element_buffer_obj;
        glGenBuffers(1, &close_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, close_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw close_button
	if(title_bar) {
	if(title_bar_button_close) {
	// close_button : radius
	shader->set_float("radius", 100); // 100%
	// close_button : gradient
	////shader->set_integer("gradient.enabled", false); // disable gradient
	shader->set_float("gradient.color", title_bar_button_close_color.x / 255.0, title_bar_button_close_color.y / 255.0, title_bar_button_close_color.z / 255.0, title_bar_button_close_color.w);
	// close_button : color	- close_button has its own unique color while the other buttons copy the box's color because the close_button changes to red, on hover
	shader->set_float("color", title_bar_button_close_color.x / 255.0, title_bar_button_close_color.y / 255.0, title_bar_button_close_color.z / 255.0, title_bar_button_close_color.w);
    glBindVertexArray(close_button_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//--------------------------
	// MAXIMIZE_BUTTON
    // vertex array obj  - stores vertices
	GLuint maximize_button_vertex_array_obj;
    glGenVertexArrays(1, &maximize_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(maximize_button_vertex_array_obj); // bind again  
	    GLuint maximize_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &maximize_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, maximize_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(maximize_button_vertex_array_obj); // bind vertex array obj
        GLuint maximize_button_vertex_buffer_obj;
        glGenBuffers(1, &maximize_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, maximize_button_vertex_buffer_obj); 
		int maximize_button_width  = close_button_width;//10;
		int maximize_button_height = close_button_height;//title_bar_height / 2; // half of titlebar_height        
		float maximize_button_x = (title_bar_width - (maximize_button_width * 2)) - (5 * 2); // 10 is the right_padding
		float maximize_button_y = (title_bar_height - maximize_button_height) / 2;	
		GLfloat vertices3[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x + maximize_button_x)                                                , static_cast<GLfloat>(y + title_bar_y + maximize_button_y),
            static_cast<GLfloat>(x + title_bar_x + maximize_button_x) + static_cast<GLfloat>(maximize_button_width), static_cast<GLfloat>(y + title_bar_y + maximize_button_y),
            static_cast<GLfloat>(x + title_bar_x + maximize_button_x) + static_cast<GLfloat>(maximize_button_width), static_cast<GLfloat>(y + title_bar_y + maximize_button_y) + static_cast<GLfloat>(maximize_button_height),
            static_cast<GLfloat>(x + title_bar_x + maximize_button_x)                                                , static_cast<GLfloat>(y + title_bar_y + maximize_button_y) + static_cast<GLfloat>(maximize_button_height),   
        };      		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3)* sizeof(GLfloat), vertices3, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(maximize_button_vertex_array_obj); // (vao start 1)
	    GLuint maximize_button_element_buffer_obj;
        glGenBuffers(1, &maximize_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, maximize_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw maximize_button
	if(title_bar) {
	if(title_bar_button_maximize) {
    // maximize_button : gradient
	shader->set_float("gradient.color", (gradient_color.x/255.0), (gradient_color.y/255.0), (gradient_color.z/255.0), gradient_color.w); // color1 will be a shade (bottom)
	// maximize_button : color
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);	
    glBindVertexArray(maximize_button_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
	}
    //--------------------------	
	// MINIMIZE_BUTTON
    // vertex array obj  - stores vertices
	GLuint iconify_button_vertex_array_obj;
    glGenVertexArrays(1, &iconify_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(iconify_button_vertex_array_obj); // bind again  
	    GLuint iconify_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &iconify_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, iconify_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(iconify_button_vertex_array_obj); // bind vertex array obj
        GLuint iconify_button_vertex_buffer_obj;
        glGenBuffers(1, &iconify_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, iconify_button_vertex_buffer_obj);
		int iconify_button_width = close_button_width;//10;
		int iconify_button_height = close_button_height;//title_bar_height / 2; // half of titlebar_height       
		float iconify_button_x = (title_bar_width - (iconify_button_width * 3)) - (5 * 3); // 15 is the right_padding
		float iconify_button_y = (title_bar_height - iconify_button_height) / 2;
		GLfloat vertices4[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x + iconify_button_x)                                               , static_cast<GLfloat>(y + title_bar_y + iconify_button_y),
            static_cast<GLfloat>(x + title_bar_x + iconify_button_x) + static_cast<GLfloat>(iconify_button_width), static_cast<GLfloat>(y + title_bar_y + iconify_button_y),
            static_cast<GLfloat>(x + title_bar_x + iconify_button_x) + static_cast<GLfloat>(iconify_button_width), static_cast<GLfloat>(y + title_bar_y + iconify_button_y) + static_cast<GLfloat>(iconify_button_height),
            static_cast<GLfloat>(x + title_bar_x + iconify_button_x)                                               , static_cast<GLfloat>(y + title_bar_y + iconify_button_y) + static_cast<GLfloat>(iconify_button_height),   
        };      		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4)* sizeof(GLfloat), vertices4, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(iconify_button_vertex_array_obj); // (vao start 1)
	    GLuint iconify_button_element_buffer_obj;
        glGenBuffers(1, &iconify_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iconify_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw iconify_button
	if(title_bar) {
	if(title_bar_button_iconify) {	
    // iconify_button : gradient (will copy gradient color from maximize_button)
	//shader->set_float("gradient.color", (gradient_color.x/255.0), (gradient_color.y/255.0), (gradient_color.z/255.0), gradient_color.w); // color1 will be a shade (bottom)	
	// iconify_button : color
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha/*(alpha / 255.0)*/);	
    glBindVertexArray(iconify_button_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
    }
	//--------------------------
	// BOX (widget)
    // vertex array obj  - stores vertices
    GLuint box_vertex_array_obj;
    glGenVertexArrays(1, &box_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(box_vertex_array_obj); // bind again  
	    GLuint box_tex_coord_buffer_obj;
        glGenBuffers(1, &box_tex_coord_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, box_tex_coord_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(box_vertex_array_obj); // bind vertex array obj
        GLuint box_vertex_buffer_obj;
        glGenBuffers(1, &box_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, box_vertex_buffer_obj);
        GLfloat vertices[] = {
		    //static_cast<GLfloat>(x + title_bar_x)                                          , static_cast<GLfloat>(y + title_bar_y),
            //static_cast<GLfloat>(x + title_bar_x) + static_cast<GLfloat>(title_bar_width), static_cast<GLfloat>(y + title_bar_y),
            //static_cast<GLfloat>(x + title_bar_x) + static_cast<GLfloat>(title_bar_width), static_cast<GLfloat>(y + title_bar_y) + static_cast<GLfloat>(title_bar_height),
            //static_cast<GLfloat>(x + title_bar_x)                                          , static_cast<GLfloat>(y + title_bar_y) + static_cast<GLfloat>(title_bar_height),          
        
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };   		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(box_vertex_array_obj); // (vao start 1)
	    GLuint box_element_buffer_obj;
        glGenBuffers(1, &box_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw box outline
	//glEnable(GL_DEPTH_TEST); // for removing hidden line
	//glEnable(GL_POLYGON_OFFSET_FILL);  // for removing hidden line
	//glPolygonOffset(1.0, 1.0);
	if(!iconify)
	{
	// What is drawn last should appear on top, according to openGL, but we want to HIDE the diagonal line shown by the outline, so outline should appear at the bottom
	// Modern OpenGL defines only points, lines or triangles; there are no 4-vertex primitive types, so you can't draw a quad without triangles, which is why you see the diagonal line across the box
	// Draw outline
	// temporary
	/*if(outline) {
	    glLineWidth(outline_width);
	    shader->set_float("color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w); // top color (tint)
        glBindVertexArray(box_vertex_array_obj);
            glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	    glBindVertexArray(0);
	}*/
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// restore gradient color (so outline does not copy gradient of close_button, in the case of close_button being the only button in the title_bar)
	// outline, box : gradient
	////shader->set_float("gradient.color", (gradient_color.x/255.0), (gradient_color.y/255.0), (gradient_color.z/255.0), gradient_color.w); // color1 will be a shade (bottom)
	//shader->set_float("radius", radius); // set rounded_corner
	// Draw box (last: on top of outline so diagonal lines do not show up)
	// box : size (I don't think size is even used in the shader :O)
	shader->set_vector2("size", Vector2(width, height));
	// box : radius
	// if box has outline then it automatically has a radius
	////if(outline && radius <= 0.0) radius = 1.0; 
	shader->set_float("radius", radius); // set rounded_corner	
	// box : gradient
	shader->set_integer("gradient.enabled", gradient); // set gradient on
	shader->set_float("gradient.value"  , gradient_value); // set gradient value
	shader->set_float("gradient.color", gradient_color.x / 255.0, gradient_color.y / 255.0, gradient_color.z / 255.0, gradient_color.w); // bottom color (shade)
	// box : color
	shader->set_float("color", red / 255.0, green / 255.0, blue / 255.0, alpha); // top color (tint)
    glBindVertexArray(box_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //shader->set_float("color", (title_bar_color.x / 255.0), (title_bar_color.y / 255.0), (title_bar_color.z / 255.0), title_bar_color.w);	
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)2);
	glBindVertexArray(0);                // (vao end 2  )
	//glDisable(GL_POLYGON_OFFSET_FILL); // for removing hidden line
	//glDisable(GL_DEPTH_TEST); // for removing hidden line
	// restore shader defaults
	if(shader->get_uniform("radius") != -1) shader->set_float("radius", 0.0);
	shader->set_integer("gradient.enabled", false);
	shader->set_integer("outline", false);
	}
	//--------------------------
	// Clean textures
	// Clean buffers
	// : box (widget main body)
	glDeleteBuffers(1, &box_tex_coord_buffer_obj);
	glDeleteBuffers(1, &box_element_buffer_obj  );
	glDeleteBuffers(1, &box_vertex_buffer_obj   );
	// :title_bar
	glDeleteBuffers(1, &title_bar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &title_bar_element_buffer_obj  );
	glDeleteBuffers(1, &title_bar_vertex_buffer_obj   );	
	// : close_button
	glDeleteBuffers(1, &close_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &close_button_element_buffer_obj  );
	glDeleteBuffers(1, &close_button_vertex_buffer_obj   );	
	// : maximize_button
	glDeleteBuffers(1, &maximize_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &maximize_button_element_buffer_obj  );
	glDeleteBuffers(1, &maximize_button_vertex_buffer_obj   );
    // : iconify_button	
	glDeleteBuffers(1, &iconify_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &iconify_button_element_buffer_obj  );
	glDeleteBuffers(1, &iconify_button_vertex_buffer_obj   );
	// :
	//glDeleteBuffers(1, &_tex_coord_buffer_obj);
	//glDeleteBuffers(1, &_element_buffer_obj  );
	//glDeleteBuffers(1, &_vertex_buffer_obj   );	
	// Clean arrays
	glDeleteVertexArrays(1, &box_vertex_array_obj      );
	glDeleteVertexArrays(1, &title_bar_vertex_array_obj);
	glDeleteVertexArrays(1, &close_button_vertex_array_obj);
	glDeleteVertexArrays(1, &maximize_button_vertex_array_obj);
	glDeleteVertexArrays(1, &iconify_button_vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif
}
////////////////////////////
void Renderer::draw_quad_instanced(int x, int y, int width, int height) {
#ifdef DOKUN_OPENGL	// OpenGL is defined
/*	context_check();
    // Draw
    //shader.set_float("color", (255 / 255.0), (108 / 255.0), (50 / 255.0), 1.0);//glEnable(GL_LINE_SMOOTH);
	glBindVertexArray(box_vertex_array_obj); // use same vao data as box but this time in a line loop
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
	        100);//GLsizei primcount);        
	glBindVertexArray(0);*/
#endif // endif DOKUN_OPENGL	
}


////////////////////////////
////////////
void Renderer::draw_text_old (const std::string& text, float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, const dokun::Font& font, Shader* shader)
{
#ifdef DOKUN_OPENGL	
    context_check();
	if(font.get_face() == nullptr) // check font
	{
		dokun::Logger("Font contains no data");
		return;
	}
    /////////////////////////////////////
	/////////////////////////////////////
	// vertex array obj
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj);
	    GLuint vertex_buffer_obj;
		glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
	// background rect - NEW! ---------------------------
	GLuint background_vertex_array_obj;
	glGenVertexArrays(1, &background_vertex_array_obj);
	// bg vertex_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_vertex_buffer_obj;
		glGenBuffers(1, &background_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_vertex_buffer_obj);
        GLfloat vertices[] = {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };   		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
	// bg texcoord_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_tex_coord_buffer_obj;
        glGenBuffers(1, &background_tex_coord_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1);		
    glBindVertexArray(0);	
	// bg element_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_element_buffer_obj;
        glGenBuffers(1, &background_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background_element_buffer_obj);
        GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 		
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 		
    glBindVertexArray(0);	
    //---------------------------------------	
    // enable
	glEnable(GL_BLEND); // transparent background
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	
	// Shader
	shader->use ();
#ifdef use_glm	
	// model (object)
	glm::mat4 model = glm::mat4(1.0);
 	model = glm::translate(model, glm::vec3(-x, -y, 1.0));//model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, -1));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(x, y, -1.0));//model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	// projection and view (camera)            //float window_width = get_display_width ();//float window_height= get_display_height();
	glm::mat4 proj = glm::ortho(0.0f,  static_cast<float>(window_width), static_cast<float>(window_height), 0.0f);// glm::ortho(0.0f,  window_width, 0.0f, window_height); // flip y when changed ortho
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0)); // label uses a camera of its own
	// pass matrix data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"),  1, false, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj" ),  1, false, glm::value_ptr(proj) );
	if(shader->get_uniform("view") != -1) glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "view" ),  1, false, glm::value_ptr(view) );
#endif
	// Draw text
	glUniform3f(glGetUniformLocation(shader->get_program(), "color"), (red / 255.0), (green / 255.0), (blue / 255.0)); // alpha
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertex_array_obj);
    for (std::string::const_iterator c = text.begin(); c != text.end(); c++) // scan through all characters in string  // Iterate through all characters
	    {
		    dokun::Font::Character character = const_cast<dokun::Font&>(font).character_array[*c];
            // how high or low character is
            GLfloat xpos = x + character.bearing_x * scale_x;
            GLfloat ypos = y + (/*character*/const_cast<dokun::Font&>(font).character_array['H'].bearing_y - character.bearing_y) * scale_y;
            GLfloat glyph_width  = character.width  * scale_x;
            GLfloat glyph_height = character.height * scale_y;
            // Update vertex_buffer_obj for each character
            GLfloat vertices[6][4] = {
            { xpos              , ypos + glyph_height,   0.0, 0.0 },            
            { xpos              , ypos,       0.0, 1.0 },
            { xpos + glyph_width, ypos,       1.0, 1.0 },

            { xpos              , ypos + glyph_height,   0.0, 0.0 },
            { xpos + glyph_width, ypos,       1.0, 1.0 },
            { xpos + glyph_width, ypos + glyph_height,   1.0, 0.0 }     // use custom height      
            };
        // Update content
		GLuint texture = character.id;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Draw text
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (character.advance_x >> 6) * scale_x; // Bitshift by 6 to get value in pixels (2^6 = 64)
        //if(*c == '\n') y += (character.advance >> 6) * scale_y;// TEMPORARY
	    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // restore original pixel_store
	// Draw background ------------ NEW!!
	shader->set_float("color", (0.0f / 255.0), (51.0f / 255.0), (102.0f / 255.0)); // alpha
	glBindVertexArray(background_vertex_array_obj);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);    
	// Clean textures
	// Clean buffers
	// : text
	glDeleteBuffers(1, & vertex_buffer_obj);
	// : background
	glDeleteBuffers(1, & background_vertex_buffer_obj);
	glDeleteBuffers(1, & background_tex_coord_buffer_obj);
	glDeleteBuffers(1, & background_element_buffer_obj);
	// Clean arrays
	glDeleteVertexArrays(1, & vertex_array_obj);
	glDeleteVertexArrays(1, & background_vertex_array_obj);
	// disable
	glDisable(GL_BLEND);
	// program
	shader->disable();
#endif
} 
////////////////////////////
// new version of draw_text (2019-07-29)
void Renderer::draw_text(const std::string& text, float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, const dokun::Font& font, Shader* shader)
{
    dokun::Font * font_ptr = &const_cast<dokun::Font&>(font); // std::cout << const_cast<dokun::Font&>(font).get_bitmap(*c)->get_size() << "; character: " << *c << " = size of bitmap" << std::endl; // prints the characters in the text (e.g 'S')
    /////////////////////////////////////////
#ifdef DOKUN_OPENGL	
    context_check();
	if(font.get_face() == nullptr) // check font
	{
		dokun::Logger("Font contains no data");
		return;
	}
    /////////////////////////////////////
	/////////////////////////////////////
	// vertex array obj
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj);
	    GLuint vertex_buffer_obj;
		glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); /*
	// background rect - NEW! ---------------------------
	GLuint background_vertex_array_obj;
	glGenVertexArrays(1, &background_vertex_array_obj);
	// bg vertex_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_vertex_buffer_obj;
		glGenBuffers(1, &background_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_vertex_buffer_obj);
        GLfloat vertices[] = {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };   		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
	// bg texcoord_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_tex_coord_buffer_obj;
        glGenBuffers(1, &background_tex_coord_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1);		
    glBindVertexArray(0);	
	// bg element_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_element_buffer_obj;
        glGenBuffers(1, &background_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background_element_buffer_obj);
        GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 		
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 		
    glBindVertexArray(0);	*/
    //---------------------------------------	
    // enable
	glEnable(GL_BLEND); // transparent background
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	
	// Shader
	shader->use ();
#ifdef use_glm	
	// model (object)
	glm::mat4 model = glm::mat4(1.0);
 	model = glm::translate(model, glm::vec3(-x, -y, 1.0));//model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, -1));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(x, y, -1.0));//model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	// projection and view (camera)            //float window_width = get_display_width ();//float window_height= get_display_height();
	glm::mat4 proj = glm::ortho(0.0f,  static_cast<float>(window_width), static_cast<float>(window_height), 0.0f);// glm::ortho(0.0f,  window_width, 0.0f, window_height); // flip y when changed ortho
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0)); // label uses a camera of its own
	// pass matrix data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"),  1, false, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj" ),  1, false, glm::value_ptr(proj) );
	if(shader->get_uniform("view") != -1) glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "view" ),  1, false, glm::value_ptr(view) );
#endif	    
    /////////////////////////////////////////
	// Draw text
	glUniform3f(glGetUniformLocation(shader->get_program(), "color"), (red / 255.0), (green / 255.0), (blue / 255.0)); // alpha
	glUniform1f(glGetUniformLocation(shader->get_program(), "alpha"), alpha);
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertex_array_obj); // font_ptr->get_bitmap(*c)->get_name()
    for (std::string::const_iterator c = text.begin(); c != text.end(); c++) // scan through all characters in string  // Iterate through all characters
	{
            // how high or low character is
            GLfloat xpos = x +  (int)font_ptr->get_bearing(*c).x * scale_x; // 
            GLfloat ypos = y + ((int)font_ptr->get_bearing('H').y - (int)font_ptr->get_bearing(*c).y) * scale_y;
            GLfloat glyph_width  = font_ptr->get_width (*c) * scale_x;//font_ptr->get_bitmap(*c)->get_width () * scale_x;
            GLfloat glyph_height = font_ptr->get_height(*c) * scale_y;//font_ptr->get_bitmap(*c)->get_height() * scale_y;
            // Update vertex_buffer_obj for each character
            GLfloat vertices[6][4] = {
            { xpos              , ypos + glyph_height,   0.0, 0.0 },            
            { xpos              , ypos,       0.0, 1.0 },
            { xpos + glyph_width, ypos,       1.0, 1.0 },

            { xpos              , ypos + glyph_height,   0.0, 0.0 },
            { xpos + glyph_width, ypos,       1.0, 1.0 },
            { xpos + glyph_width, ypos + glyph_height,   1.0, 0.0 }     // use custom height      
            };
        // Update content
		GLuint texture = font_ptr->get_buffer(*c);//font_ptr->get_bitmap(*c)->get_buffer(); // get GL_texture_buffer
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Draw text
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (static_cast<long int>( font_ptr->get_advance(*c).x ) >> 6) * scale_x; // Bitshift by 6 to get value in pixels (2^6 = 64)//std::cout << "x is equal to : " << x << " glyph: " << *c << std::endl;
        if(*c == '\n'){ y += (static_cast<long int>( font_ptr->get_advance(*c).x ) >> 6) * scale_y; // new line ... - newline counts as part of a string
	        x = ((long int)font_ptr->get_advance(*c).x >> 6) * scale_x;
	    }
	}
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // restore original pixel_store
    /////////////////////////////////////////
    /*// Draw background ------------ NEW!!
	shader->set_float("color", (0.0f / 255.0), (51.0f / 255.0), (102.0f / 255.0)); // alpha
	glBindVertexArray(background_vertex_array_obj);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/
    /////////////////////////////////////////
	// Clean textures
	// Clean buffers
	// : text
	glDeleteBuffers(1, & vertex_buffer_obj);
	// : background
	/*glDeleteBuffers(1, & background_vertex_buffer_obj);
	glDeleteBuffers(1, & background_tex_coord_buffer_obj);
	glDeleteBuffers(1, & background_element_buffer_obj);*/
	// Clean arrays
	glDeleteVertexArrays(1, & vertex_array_obj);
	//glDeleteVertexArrays(1, & background_vertex_array_obj);
	// disable
	glDisable(GL_BLEND);
	// program
	shader->disable();
#endif    
}
////////////////////////////
void Renderer::draw_glyph (unsigned char glyph, float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, const dokun::Font& font, Shader* shader)
{
#ifdef DOKUN_OPENGL	
    context_check();
	if(font.get_face() == nullptr) // check font
	{
		dokun::Logger("Font contains no data");
		return;
	}
    /////////////////////////////////////
	/////////////////////////////////////
	// vertex array obj
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj);
	    GLuint vertex_buffer_obj;
		glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);      
    // enable
	glEnable(GL_BLEND); // transparent background
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	
	// Shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);; 
	int width  = const_cast<dokun::Font&>(font).character_array[glyph].width;
	int height = const_cast<dokun::Font&>(font).character_array[glyph].height;
 	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, -1));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(static_cast<float>(scale_x), static_cast<float>(scale_y), 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
    // projection and view (camera)
	glm::mat4 proj = glm::ortho(0.0f,  static_cast<float>(window_width), static_cast<float>(window_height), 0.0f); //glm::ortho(0.0f,  window_width, 0.0f, window_height); // flip y when changed ortho
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0));
		
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"),  1, false, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj" ),  1, false, glm::value_ptr(proj) );
	if(shader->get_uniform("view") != -1) glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "view" ),  1, false, glm::value_ptr(view) );
#endif	
	glUniform3f(glGetUniformLocation(shader->get_program(), "color"), (red / 255.0), (green / 255.0), (blue / 255.0)); // alpha
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertex_array_obj);
    // character (glpyh)
	dokun::Font::Character character = const_cast<dokun::Font&>(font).character_array[glyph];
            // how high or low character is
            GLfloat xpos = x +  const_cast<dokun::Font&>(font).character_array[glyph].bearing_x * scale_x;
            GLfloat ypos = y - (const_cast<dokun::Font&>(font).character_array[glyph].height - const_cast<dokun::Font&>(font).character_array[glyph].bearing_y) * scale_y;
            GLfloat w = const_cast<dokun::Font&>(font).get_width(glyph);//const_cast<dokun::Font&>(font).character_array[glyph].width  * scale_x;
            GLfloat h = const_cast<dokun::Font&>(font).get_height(glyph);//const_cast<dokun::Font&>(font).character_array[glyph].height * scale_y;
            // Update vertex_buffer_obj for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
        // Update content
		GLuint texture = const_cast<dokun::Font&>(font).character_array[glyph].id;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Draw text
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (const_cast<dokun::Font&>(font).character_array[glyph].advance_x >> 6) * scale_x; // Bitshift by 6 to get value in pixels (2^6 = 64)
    
	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	// Clean textures
	// Clean buffers
	glDeleteBuffers(1, & vertex_buffer_obj);
	// Clean arrays
	glDeleteVertexArrays(1, & vertex_array_obj);
	// disable
	glDisable(GL_BLEND);
	// program
	shader->disable();
#endif	
}
////////////////////////////
////////////
void Renderer::draw_button(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader, 
	// outline
	bool outline, 
	float outline_width, 
	const Vector4& outline_color,
	bool outline_antialiased,
	// border
	//bool border,
	//const Vector4& border_color,
	// radius
	float radius,
	// gradient
    bool gradient,
	const Vector4& gradient_color
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// use shader
	shader->use ();
	// uniform
#ifdef use_glm	
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	//float window_width  = Renderer::get_display_width ();
	//float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
    // set gradient    if(shader->get_uniform("") != -1)
	if(shader->get_uniform("resolution") != -1) shader->set_float("resolution", static_cast<double>(Renderer::get_display_width()), static_cast<double>(Renderer::get_display_height()));
    // --------------------
	// BUTTON   
    // vertex array obj  - stores vertices
    GLuint button_vertex_array_obj;
    glGenVertexArrays(1, &button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind again  
	    GLuint button_tex_coord_buffer_obj;
        glGenBuffers( 1, &button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, button_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind vertex array obj
        GLuint button_vertex_buffer_obj;
        glGenBuffers(1, &button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, button_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(button_vertex_array_obj); // (vao start 1)
	    GLuint button_element_buffer_obj;
        glGenBuffers(1, &button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw button_outline
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw button
	// button : radius
	if(shader->get_uniform("radius") != -1) shader->set_float("radius", radius);
    // button : gradient
    if(shader->get_uniform("gradient.enabled") != -1) shader->set_integer("gradient.enabled", static_cast<int>(gradient)); // set gradient on
	// button : color and gradient colors
	shader->set_float("gradient.color", (gradient_color.x / 255.0), (gradient_color.y / 255.0), (gradient_color.z / 255.0), gradient_color.w); // bottom color (shade)
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// restore defaults
	if(shader->get_uniform("radius") != -1) shader->set_float("radius", 0.0);
	if(shader->get_uniform("gradient.enabled") != -1) shader->set_integer("gradient.enabled", false);
	shader->set_integer("outline", false);
	// Clean textures
	// Clean buffers
	glDeleteBuffers(1, &button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &button_element_buffer_obj);
	glDeleteBuffers(1, &button_vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &button_vertex_array_obj);
	// Restore defaults
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif
}
////////////
void Renderer::draw_progressbar(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader, 
    double min_value, double max_value, double value, const Vector4& background_color,
	// outline
	bool outline, 
	float outline_width, 
	const Vector4& outline_color,
	bool outline_antialiased,
	// radius
	float radius
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
    // use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	//float window_width  = Renderer::get_display_width ();
	//float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	// texcoord array
	GLfloat tex_coords[] = 
	{ 
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	// element array
	GLuint indices[] = {0, 1, 3,  1, 2, 3,};
	//------------------------
    // BORDER - at the far back of all other elements; can also add border from image
/*
    // vertex array obj  - stores vertices
    GLuint border_vertex_array_obj;
    glGenVertexArrays(1, &border_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(border_vertex_array_obj); // bind again  
	    GLuint border_tex_coord_buffer_obj;
        glGenBuffers( 1, &border_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, border_tex_coord_buffer_obj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(border_vertex_array_obj); // bind vertex array obj
        GLuint border_vertex_buffer_obj;
        glGenBuffers(1, &border_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, border_vertex_buffer_obj);
		float border_x = 0;
		float border_y = 0;
		float border_width  = width  * 2;
		float border_height = height * 2;
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + border_x)                                     , static_cast<GLfloat>(y + border_y),
            static_cast<GLfloat>(x + border_x) + static_cast<GLfloat>(border_width), static_cast<GLfloat>(y + border_y),
            static_cast<GLfloat>(x + border_x) + static_cast<GLfloat>(border_width), static_cast<GLfloat>(y + border_y) + static_cast<GLfloat>(border_height),
            static_cast<GLfloat>(x + border_x)                                     , static_cast<GLfloat>(y + border_y) + static_cast<GLfloat>(border_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(border_vertex_array_obj); // (vao start 1)
	    GLuint border_element_buffer_obj;
        glGenBuffers(1, &border_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_element_buffer_obj);
		GLuint indices2[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2)* sizeof(GLuint), indices2, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw border
	int border_red = 128; int border_green = 128; int border_blue = 128; int border_alpha = 1.0;
    shader->set_float("color", (border_red / 255.0), (border_green / 255.0), (border_blue / 255.0), border_alpha);
    glBindVertexArray(border_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	std::cout << dokun_opengl_error() << "\n";
	*/
	//------------------------
    // STATIC BAR (background) -  drawn underneath moving bar
    // vertex array obj  - stores vertices
    GLuint background_vertex_array_obj;
    glGenVertexArrays(1, &background_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(background_vertex_array_obj); 
	    GLuint background_tex_coord_buffer_obj;
        glGenBuffers( 1, &background_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, background_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0);
	// vertex buffer obj
    glBindVertexArray(background_vertex_array_obj);
        GLuint background_vertex_buffer_obj;
        glGenBuffers(1, &background_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_vertex_buffer_obj); 
        GLfloat vertices[] = 
	    {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };  		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); 
	// element buffer obj
	glBindVertexArray(background_vertex_array_obj);
	    GLuint background_element_buffer_obj;
        glGenBuffers(1, &background_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);
    // Draw static_bar (background) outline
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// progressbar radius
	if(shader->get_uniform("radius") != -1) shader->set_float("radius", radius);
	// Draw static_bar (background)
	shader->set_float("color", (background_color.x / 255.0), (background_color.y / 255.0), (background_color.z / 255.0), background_color.w);
    glBindVertexArray(background_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//std::cout << dokun_opengl_error() << "\n";
	// ----------------------
	// MOVING BAR (foreground) - drawn on top of static bar (background)
    // vertex array obj  - stores vertices
    GLuint foreground_vertex_array_obj;
    glGenVertexArrays(1, &foreground_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(foreground_vertex_array_obj); // bind again  
	    GLuint foreground_tex_coord_buffer_obj;
        glGenBuffers( 1, &foreground_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, foreground_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(foreground_vertex_array_obj); // bind vertex array obj
        GLuint foreground_vertex_buffer_obj;
        glGenBuffers(1, &foreground_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, foreground_vertex_buffer_obj);
		GLfloat progress = value / max_value; // 1.0 = max_value reached !
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                                           , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress * static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress * static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                           , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(foreground_vertex_array_obj); // (vao start 1)
	    GLuint foreground_element_buffer_obj;
        glGenBuffers(1, &foreground_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, foreground_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);
	// Draw moving_bar(foreground) outline
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw moving_bar (foreground)
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha); //  foreground_color
    glBindVertexArray(foreground_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//std::cout << dokun_opengl_error() << "\n";
	// Clean buffers
	// :static bar (background)
	glDeleteBuffers(1, &background_tex_coord_buffer_obj);
	glDeleteBuffers(1, &background_element_buffer_obj  );
	glDeleteBuffers(1, &background_vertex_buffer_obj   );
	// :moving bar (foreground)
	glDeleteBuffers(1, &foreground_tex_coord_buffer_obj);
	glDeleteBuffers(1, &foreground_element_buffer_obj  );
	glDeleteBuffers(1, &foreground_vertex_buffer_obj   );
	// : border
	//glDeleteBuffers(1, &border_tex_coord_buffer_obj);
	//glDeleteBuffers(1, &border_element_buffer_obj  );
	//glDeleteBuffers(1, &border_vertex_buffer_obj   );		
	// Clean arrays
	//glDeleteVertexArrays(1, &border_vertex_array_obj    );
	glDeleteVertexArrays(1, &foreground_vertex_array_obj);
	glDeleteVertexArrays(1, &background_vertex_array_obj);
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth (1.0);
	// Disable program
	shader->disable();
#endif
}
////////////
void Renderer::draw_edit(const std::string& text, int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	bool multilined,
	// cursor
	bool cursor,
	double cursor_x,
	double cursor_y, int cursor_width, int cursor_height, const Vector4& cursor_color)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
    //if(!text.empty()) draw_label(text,);
	shader->use ();
	if(!shader->is_linked()) {throw std::runtime_error("\033[0;91m SHADER NOT LINKED TO A PROGRAM!!");}
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	//float window_width  = Renderer::get_display_width (); // maybe remove?
	//float window_height = Renderer::get_display_height(); // maybe remove?
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//-------------------------------
    // set uniforms (if uniform cannot be found or does not exist, the function will return)
    shader->set_vector2  ("size", Vector2(width, height));
	if(shader->get_uniform("resolution") != -1) shader->set_float("resolution", static_cast<double>(Renderer::get_display_width()), static_cast<double>(Renderer::get_display_height()));
	//-------------------------------
	// TEXT EDIT
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers(1, &tex_coord_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw edit outline
	/*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// edit : radius
	shader->set_float  ("radius", 0/*radius*/); // set rounded_corner	
	// edit : gradient
	shader->set_integer("gradient.enabled", false/*gradient*/); // set gradient on
	shader->set_float  ("gradient.value"  , 0.00/*gradient_value*/); // set gradient value	
	// Draw edit
	////shader->set_float("gradient.color", gradient_color.x / 255.0, gradient_color.y / 255.0, gradient_color.z / 255.0, gradient_color.w); // bottom color (shade)
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Restore defaults
	shader->set_integer("gradient.enabled", false);
	shader->set_float  ("radius", 0);
	////shader->set_integer("outline", false);
    //-----------------------------------
    // CURSOR
	if(cursor) {    
    // vertex array obj  - stores vertices
    GLuint cursor_vertex_array_obj;
    glGenVertexArrays(1, &cursor_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(cursor_vertex_array_obj); // bind vertex array obj
        GLuint cursor_vertex_buffer_obj;
        glGenBuffers(1, &cursor_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, cursor_vertex_buffer_obj);
		int top_padding    = 3;//1;
		int bottom_padding = top_padding * 2; // makes bottom_padding look equal to top_padding
		cursor_y = cursor_y + top_padding;
		int cursor_left  = (cursor_width / 2) + 1;//0; // more like left padding (should be half of line_width, or 0-0.5 if the line_with is 1.0)
		cursor_height = ((!multilined) ? height - bottom_padding : cursor_height - bottom_padding); // bottom_padding // edit height
        //if(multilined) cursor_height = (height/2);
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + cursor_x) + static_cast<float>(cursor_left), static_cast<GLfloat>(y + cursor_y),
            static_cast<GLfloat>(x + cursor_x) + static_cast<float>(cursor_left), static_cast<GLfloat>(y + cursor_y) + static_cast<float>(cursor_height),
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// Draw cursor
	shader->set_float("color", (cursor_color.x / 255.0), (cursor_color.y / 255.0), (cursor_color.z / 255.0), cursor_color.w);
    //glEnable(GL_LINE_SMOOTH);
	glLineWidth(static_cast<GLfloat>(cursor_width));//(1.0); // *real* width of the cursor
	glBindVertexArray(cursor_vertex_array_obj);
        glDrawArrays(GL_LINES, 0,  2);
	glBindVertexArray(0);
	// Cleanup time 
	// vertex_buffer_objs : cursor
	glDeleteBuffers(1, &cursor_vertex_buffer_obj);	  
	// vertex_array_objs : cursor
	glDeleteVertexArrays(1, &cursor_vertex_array_obj);	    
	} // end of if(cursor)
	//-----------------------------------
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean vertex_buffers : edit
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean vertex_arrays : edit
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	// Disable program
	shader->disable(); // un-use shader
#endif
}
////////////
////////////
void Renderer::draw_cursor(double cursor_x, double cursor_y, int cursor_width, int cursor_height, const Vector4& cursor_color, Shader* shader,
    int parent_x, int parent_y, int parent_width, int parent_height) {
    ///////////////
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
    //if(!text.empty()) draw_label(text,);
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(/*parent_*/cursor_x + cursor_width/2, /*parent_*/cursor_y + cursor_height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(0/*angle*/ * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1/*scale_x, scale_y*/, 1));
	model = glm::translate(model, glm::vec3(/*parent_*/-cursor_x - cursor_width/2, /*parent_*/-cursor_y - cursor_height/2, 0));
	//float window_width  = Renderer::get_display_width (); // maybe remove?
	//float window_height = Renderer::get_display_height(); // maybe remove?
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	    
    ///////////////
    // CURSOR
    // vertex array obj  - stores vertices
    GLuint cursor_vertex_array_obj;
    glGenVertexArrays(1, &cursor_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(cursor_vertex_array_obj); // bind vertex array obj
        GLuint cursor_vertex_buffer_obj;
        glGenBuffers(1, &cursor_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, cursor_vertex_buffer_obj);
		int top_padding    = 3;//1;
		int bottom_padding = top_padding * 2; // makes bottom_padding look equal to top_padding
		cursor_y = cursor_y + top_padding;
		int cursor_left  = (cursor_width / 2) + 1;//0; // more like left padding (should be half of line_width, or 0-0.5 if the line_with is 1.0)
		cursor_height = parent_height - bottom_padding;//((!multilined) ? parent_height - bottom_padding : cursor_height - bottom_padding); // bottom_padding // edit height
        //if(multilined) cursor_height = (height/2);
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(parent_x + cursor_x) + static_cast<float>(cursor_left), static_cast<GLfloat>(parent_y + cursor_y),
            static_cast<GLfloat>(parent_x + cursor_x) + static_cast<float>(cursor_left), static_cast<GLfloat>(parent_y + cursor_y) + static_cast<float>(cursor_height),
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// Draw cursor
	shader->set_float("color", (cursor_color.x / 255.0), (cursor_color.y / 255.0), (cursor_color.z / 255.0), cursor_color.w);
    //glEnable(GL_LINE_SMOOTH);
	glLineWidth(static_cast<GLfloat>(cursor_width));//(1.0); // *real* width of the cursor
	glBindVertexArray(cursor_vertex_array_obj);
        glDrawArrays(GL_LINES, 0,  2);
	glBindVertexArray(0);
    ///////////////
    // Cleanup
    // vertex_buffers
    glDeleteBuffers(1, &cursor_vertex_buffer_obj);
    // vertex_arrays
	glDeleteVertexArrays(1, &cursor_vertex_array_obj);
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	// Disable program
	shader->disable();
#endif	
}
////////////
////////////
void Renderer::draw_slider(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    // beam
	double min_value, double max_value, double value, const Vector4& background_color,
	// radius (for beam)
	float radius, 
	// ball
	int ball_width, const Vector4& ball_color, float ball_radius)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// vertex shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//----------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };	
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	//----------------------------
	// BEAM (BACKGROUND)
    // vertex array obj  - stores vertices
    GLuint slider_vertex_array_obj;
    glGenVertexArrays(1, &slider_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(slider_vertex_array_obj); // bind again  
	    GLuint slider_tex_coord_buffer_obj;
        glGenBuffers( 1, &slider_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, slider_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(slider_vertex_array_obj); // bind vertex array obj
        GLuint slider_vertex_buffer_obj;
        glGenBuffers(1, &slider_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, slider_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(slider_vertex_array_obj); // (vao start 1)
	    GLuint slider_element_buffer_obj;
        glGenBuffers(1, &slider_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, slider_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw slider(beam) outline
	/*	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw slider (beam)
	shader->set_float("radius", radius);
	shader->set_float("color", (background_color.x / 255.0), (background_color.y / 255.0), (background_color.z / 255.0), background_color.w);
    glBindVertexArray(slider_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //----------------------------
    // BEAM_BAR (foreground)
    // vertex array obj  - stores vertices
    GLuint bar_vertex_array_obj;
    glGenVertexArrays(1, &bar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bar_vertex_array_obj); // bind again  
	    GLuint bar_tex_coord_buffer_obj;
        glGenBuffers( 1, &bar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bar_vertex_array_obj); // bind vertex array obj
        GLuint bar_vertex_buffer_obj;
        glGenBuffers(1, &bar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bar_vertex_buffer_obj);
		// controls where the bar goes
		// to get x position of pivot = x + (value * 2). If value is 100 then pivot_x is 200
		// to get y position of pivot = y
		// position relative to the slider
		float progress = value / max_value; // value changes when pivotx is moved
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                                       , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress*static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress*static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                       , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bar_vertex_array_obj); // (vao start 1)
	    GLuint bar_element_buffer_obj;
        glGenBuffers(1, &bar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bar_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	/* // Draw beam_bar outline
		// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw beam_bar (foreground)
	////shader->set_float("radius", radius);
	shader->set_float("color", (red/ 255.0), (green / 255.0), (blue / 255.0), alpha); //  foreground_color
    glBindVertexArray(bar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //-------------------------------------------
    //---------BALL / HANDLE
    // vertex array obj  - stores vertices
    GLuint ball_vertex_array_obj;
    glGenVertexArrays(1, &ball_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(ball_vertex_array_obj); // bind again  
	    GLuint ball_tex_coord_buffer_obj;
        glGenBuffers( 1, &ball_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, ball_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(ball_vertex_array_obj); // bind vertex array obj
        GLuint ball_vertex_buffer_obj;
        glGenBuffers(1, &ball_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, ball_vertex_buffer_obj);
		// pivot size = 10 x height = ball_width x ball_height
		// position relative to slider       // padding is only used when the ball is inside the beam rather than it sticking out of the beam
		int ball_top_padding    = 0;//1;    // space between handle and top part of slider
		int ball_bottom_padding = 0;//2     // space between handle and bottom of slider
		int ball_height = (height * 2) - ball_bottom_padding;//height - ball_bottom_padding; // adjust the height a bit                                                                // height (slider_height)
		float ball_y    = -(height - height / 2) + ball_top_padding;//0 + ball_top_padding;             // move down 1 unit away from slider (bottom padding???)               // 0      (slider_y + ball_y) 
		float ball_x    = (value / max_value) * static_cast<GLfloat>(width - ball_width); // value starts at 0	(width - ball_width)  //std::cout << "Renderer ball_height: " << ball_height << std::endl;	std::cout << "Renderer ball_y: " << y + ball_y << std::endl;	
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + ball_x)                                   , static_cast<GLfloat>(y + ball_y),
            static_cast<GLfloat>(x + ball_x) + static_cast<GLfloat>(ball_width), static_cast<GLfloat>(y + ball_y),
            static_cast<GLfloat>(x + ball_x) + static_cast<GLfloat>(ball_width), static_cast<GLfloat>(y + ball_y) + static_cast<GLfloat>(ball_height),
            static_cast<GLfloat>(x + ball_x)                                   , static_cast<GLfloat>(y + ball_y) + static_cast<GLfloat>(ball_height),   
        };      //std::cout << x + ball_x << " pivot_x\n";
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(ball_vertex_array_obj); // (vao start 1)
	    GLuint ball_element_buffer_obj;
        glGenBuffers(1, &ball_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ball_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw ball / handle
	shader->set_float("radius", ball_radius);
	shader->set_float("color", (ball_color.x/ 255.0), (ball_color.y / 255.0), (ball_color.z / 255.0), ball_color.w);
    glBindVertexArray(ball_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	// Restore default radius
	shader->set_float("radius", 0.0);
	shader->set_integer("outline", false);
	// Clean textures
	// Clean buffers
	// :slider
	glDeleteBuffers(1, &slider_tex_coord_buffer_obj);
	glDeleteBuffers(1, &slider_element_buffer_obj  );
	glDeleteBuffers(1, &slider_vertex_buffer_obj   );
	// :bar
	glDeleteBuffers(1, &bar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bar_element_buffer_obj  );
	glDeleteBuffers(1, &bar_vertex_buffer_obj   );
	// :ball
	glDeleteBuffers(1, &ball_tex_coord_buffer_obj);
	glDeleteBuffers(1, &ball_element_buffer_obj  );
	glDeleteBuffers(1, &ball_vertex_buffer_obj   );
	// Clean arrays
	glDeleteVertexArrays(1, &slider_vertex_array_obj);
	glDeleteVertexArrays(1, &bar_vertex_array_obj   );
	glDeleteVertexArrays(1, &ball_vertex_array_obj  );
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	// Disable program
	shader->disable();
	//std::cout << dokun_opengl_error() << std::endl;
#endif
}
////////////
void Renderer::draw_slider_vertical(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    // beam
	double min_value, double max_value, double value, const Vector4& background_color,
	// ball
	int ball_height, const Vector4& ball_color)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// vertex shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//----------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };	
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	//----------------------------
	// BEAM (BACKGROUND)
    // vertex array obj  - stores vertices
    GLuint slider_vertex_array_obj;
    glGenVertexArrays(1, &slider_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(slider_vertex_array_obj); // bind again  
	    GLuint slider_tex_coord_buffer_obj;
        glGenBuffers( 1, &slider_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, slider_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(slider_vertex_array_obj); // bind vertex array obj
        GLuint slider_vertex_buffer_obj;
        glGenBuffers(1, &slider_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, slider_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                              , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                              , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(slider_vertex_array_obj); // (vao start 1)
	    GLuint slider_element_buffer_obj;
        glGenBuffers(1, &slider_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, slider_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw slider(beam) outline
	/*	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw slider (beam)
	shader->set_float("color", (background_color.x / 255.0), (background_color.y / 255.0), (background_color.z / 255.0), background_color.w);
    glBindVertexArray(slider_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //----------------------------
    // BEAM_BAR (foreground)
    // vertex array obj  - stores vertices
    GLuint bar_vertex_array_obj;
    glGenVertexArrays(1, &bar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bar_vertex_array_obj); // bind again  
	    GLuint bar_tex_coord_buffer_obj;
        glGenBuffers( 1, &bar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bar_vertex_array_obj); // bind vertex array obj
        GLuint bar_vertex_buffer_obj;
        glGenBuffers(1, &bar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bar_vertex_buffer_obj);
		// controls where the bar goes
		// to get x position of pivot = x + (value * 2). If value is 100 then pivot_x is 200
		// to get y position of pivot = y
		// position relative to the slider
		float progress = value / max_value; // value changes when pivotx is moved
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                              , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + progress * static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                              , static_cast<GLfloat>(y) + progress * static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bar_vertex_array_obj); // (vao start 1)
	    GLuint bar_element_buffer_obj;
        glGenBuffers(1, &bar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bar_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	/* // Draw beam_bar outline
		// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw beam_bar (foreground)
	shader->set_float("color", (red/ 255.0), (green / 255.0), (blue / 255.0), alpha); //  foreground_color
    glBindVertexArray(bar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //-------------------------------------------
    //---------BALL / HANDLE
    // vertex array obj  - stores vertices
    GLuint ball_vertex_array_obj;
    glGenVertexArrays(1, &ball_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(ball_vertex_array_obj); // bind again  
	    GLuint ball_tex_coord_buffer_obj;
        glGenBuffers( 1, &ball_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, ball_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(ball_vertex_array_obj); // bind vertex array obj
        GLuint ball_vertex_buffer_obj;
        glGenBuffers(1, &ball_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, ball_vertex_buffer_obj);
		// pivot size = 10 x height = ball_width x ball_height
		// position relative to slider       // padding is only used when the ball is inside the beam rather than it sticking out of the beam
		int ball_left_padding  = 0;//1;    // space between handle and top part of slider
		int ball_right_padding = 0;//2     // space between handle and bottom of slider
		int ball_width  = (width * 2) - ball_right_padding;//height - ball_bottom_padding; // adjust the height a bit                                                                // height (slider_height)
		float ball_x    = -(width - width / 2) + ball_left_padding;
		float ball_y    = (value / max_value) * static_cast<GLfloat>(height);//0 + ball_top_padding;             // move down 1 unit away from slider (bottom padding???)               // 0      (slider_y + ball_y) 
		// value starts at 0	//std::cout << "Renderer ball_width: " << ball_width << std::endl;	std::cout << "Renderer ball_x: " << x + ball_x << std::endl;	
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + ball_x)                                   , static_cast<GLfloat>(y + ball_y),
            static_cast<GLfloat>(x + ball_x) + static_cast<GLfloat>(ball_width), static_cast<GLfloat>(y + ball_y),
            static_cast<GLfloat>(x + ball_x) + static_cast<GLfloat>(ball_width), static_cast<GLfloat>(y + ball_y) + static_cast<GLfloat>(ball_height),
            static_cast<GLfloat>(x + ball_x)                                   , static_cast<GLfloat>(y + ball_y) + static_cast<GLfloat>(ball_height),   
        };      //std::cout << x + ball_x << " pivot_x\n";
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(ball_vertex_array_obj); // (vao start 1)
	    GLuint ball_element_buffer_obj;
        glGenBuffers(1, &ball_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ball_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw ball / handle
	shader->set_float("color", (ball_color.x/ 255.0), (ball_color.y / 255.0), (ball_color.z / 255.0), ball_color.w);
    glBindVertexArray(ball_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	// Clean textures
	// Clean buffers
	// :slider
	glDeleteBuffers(1, &slider_tex_coord_buffer_obj);
	glDeleteBuffers(1, &slider_element_buffer_obj  );
	glDeleteBuffers(1, &slider_vertex_buffer_obj   );
	// :bar
	glDeleteBuffers(1, &bar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bar_element_buffer_obj  );
	glDeleteBuffers(1, &bar_vertex_buffer_obj   );
	// :ball
	glDeleteBuffers(1, &ball_tex_coord_buffer_obj);
	glDeleteBuffers(1, &ball_element_buffer_obj  );
	glDeleteBuffers(1, &ball_vertex_buffer_obj   );
	// Clean arrays
	glDeleteVertexArrays(1, &slider_vertex_array_obj);
	glDeleteVertexArrays(1, &bar_vertex_array_obj   );
	glDeleteVertexArrays(1, &ball_vertex_array_obj  );
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	// Disable program
	shader->disable();
	//std::cout << dokun_opengl_error() << std::endl;
#endif
}
////////////
void Renderer::draw_switch(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	int value,
	const Vector4& background_color,
	// border
	bool outline,
	float outline_width,
	const Vector4& outline_color,
	bool outline_antialiased,
	// radius
	float radius	
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
    // use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	//float window_width  = Renderer::get_display_width ();
	//float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	// SWITCH (background / body)
    // vertex array obj  - stores vertices 
    GLuint switch_vertex_array_obj;
    glGenVertexArrays(1, &switch_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(switch_vertex_array_obj); // bind again  
	    GLuint switch_tex_coord_buffer_obj;
        glGenBuffers( 1, &switch_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, switch_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(switch_vertex_array_obj); // bind vertex array obj
        GLuint switch_vertex_buffer_obj;
        glGenBuffers(1, &switch_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, switch_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(switch_vertex_array_obj); // (vao start 1)
	    GLuint switch_element_buffer_obj;
        glGenBuffers(1, &switch_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, switch_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw switch(background) outline
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw switch (background)
	shader->set_float("radius", radius);
	shader->set_float("color", (background_color.x / 255.0), (background_color.y / 255.0), (background_color.z / 255.0), background_color.w);
    glBindVertexArray(switch_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// Restore defaults
	////shader->set_float("radius", 0.0);
	////shader->set_integer("outline", false);
	//------------------------------
	// HANDLE (foreground)
    // vertex array obj  - stores vertices
    GLuint handle_vertex_array_obj;
    glGenVertexArrays(1, &handle_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind again  
	    GLuint handle_tex_coord_buffer_obj;
        glGenBuffers( 1, &handle_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, handle_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind vertex array obj
        GLuint handle_vertex_buffer_obj;
        glGenBuffers(1, &handle_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, handle_vertex_buffer_obj);
		// set position relative to switch
		float top_padding    = 1;    // space between the top_part of switch and the handle 
		float bottom_padding = 2; // space between bottom_part of switch and the handle
		float handle_x = 0;
		float handle_y = 0 + top_padding;  // move handle down a bit by 1 unit (for padding purposes)
        int handle_width  = (width / 2); // handle is half of the switch's width
        int handle_height =	height - bottom_padding;  // reduce the height by 2 pixels (for padding purposes)
		if(value == 1) // if turned on
			handle_x = handle_x + (width / 2); // move handle to the other half of the switch
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + handle_x)                                , static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),
            static_cast<GLfloat>(x + handle_x)                                , static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),   
        };		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(handle_vertex_array_obj); // (vao start 1)
	    GLuint handle_element_buffer_obj;
        glGenBuffers(1, &handle_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw handle outline
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw handle switch
	shader->set_float("radius", (radius >= 50.0) ? 100.0 : radius);// when the switch toggle starts to look more round-ish, set radius to 100 %
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(handle_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Restore defaults
	shader->set_float("radius", 0.0);
	shader->set_integer("outline", false);
	// Clean textures
	// Clean buffers
	// :toggle
	glDeleteBuffers(1, &switch_tex_coord_buffer_obj);
	glDeleteBuffers(1, &switch_element_buffer_obj);
	glDeleteBuffers(1, &switch_vertex_buffer_obj );
	// :switch
	glDeleteBuffers(1, &handle_tex_coord_buffer_obj);
	glDeleteBuffers(1, &handle_element_buffer_obj);
	glDeleteBuffers(1, &handle_vertex_buffer_obj );	
	// Clean arrays
	glDeleteVertexArrays(1, &handle_vertex_array_obj);
	glDeleteVertexArrays(1, &switch_vertex_array_obj);
	// Restore defaults
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif
}
//////////// // Usage: Renderer::draw_tooltip("Hello", 750, 500, 100, 50, 0.0, 1, 1, 106, 106, 106, 255.0);
void Renderer::draw_tooltip(const std::string& text, int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	bool outline, const Vector4& outline_color,
	float radius, 
    std::string direction, int arrow_width, int arrow_height, double arrow_offset)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// set outline
	//glLineWidth(10.0);
		//float radius = 50.0f;
	float angle_rad = 3.14159265359f * angle / 180.f;
	float x_rad = sin(angle) * radius;
	float y_rad = cos(angle) * radius;
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// use shader
	shader->use ();
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x/* + x_rad*/)                              , static_cast<GLfloat>(y/* + y_rad*/),
            static_cast<GLfloat>(x/* + x_rad*/) + static_cast<GLfloat>(width), static_cast<GLfloat>(y/* + y_rad*/),
            static_cast<GLfloat>(x/* + x_rad*/) + static_cast<GLfloat>(width), static_cast<GLfloat>(y/* + y_rad*/) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x/* + x_rad*/)                              , static_cast<GLfloat>(y/* + y_rad*/) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);	
	shader->set_float("radius", radius);
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    // Restore defaults
    shader->set_float("radius", 0.0);
//---------------------------------------------
	//////////////////////
    // TRIANGLE	
    // vertex array obj  - stores vertices
    GLuint arrow_vertex_array_obj;
    glGenVertexArrays(1, &arrow_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(arrow_vertex_array_obj); // bind vertex array obj
        GLuint arrow_vertex_buffer_obj;
        glGenBuffers(1, &arrow_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, arrow_vertex_buffer_obj);
        std::vector<GLfloat> verticestri;
        if(String::lower(direction) == "down") {
            // set arrow geometry       //int arrow_width  = 10; // should be custom (always bigger than arrow_height) //width / 2 - would look like a house //int arrow_height = 5;  // should be custom (always smaller than arrow_width)
            double arrow_x = (arrow_offset < 0.0) ? width / 2 : arrow_offset + arrow_width; // arrow_x at center of box_x
            double arrow_y = height + arrow_height; // at bottom of box_height + self_height
            // but what if user wants to change the position of arrow_x        
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width ));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(-arrow_width));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height));
        }
        if(String::lower(direction) == "up") {
            // set arrow geometry
            double arrow_x = (arrow_offset < 0.0) ? width / 2 : arrow_offset + arrow_width; // arrow_x at center of box_x
            double arrow_y = -arrow_height; // subtract arrow_y by its own height
            // pass arrow vertices
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(-arrow_width));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width ));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ));
        }
        if(String::lower(direction) == "right") 
        {}
        if(String::lower(direction) == "left") 
        {}
        glBufferData(GL_ARRAY_BUFFER, verticestri.size() * sizeof(GLfloat), verticestri.data(), GL_STATIC_DRAW);//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestri)* sizeof(GLfloat), verticestri, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(arrow_vertex_array_obj); // (vao start 1)
	    GLuint arrow_element_buffer_obj;
        glGenBuffers(1, &arrow_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrow_element_buffer_obj);
		GLuint indicestri[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicestri)* sizeof(GLuint), indicestri, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw triangle
	// arrow will copy the color of tooltip. Make sure color is a float/double!//Vector4 arrow_color = Vector4(red, green, blue, alpha);//shader->set_float("color", (arrow_color.x / 255.0), (arrow_color.y / 255.0), (arrow_color.z / 255.0), arrow_color.w); //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(arrow_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 points to form a triangle
	glBindVertexArray(0);                // (vao end 2  ) //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	// Restore defaults
	shader->set_integer("outline", false);
	//////////////////////
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// triangle
	//glDeleteBuffers(1, &arrow_tex_coord_buffer_obj);
	glDeleteBuffers(1, &arrow_element_buffer_obj);
	glDeleteBuffers(1, &arrow_vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj      );
	glDeleteVertexArrays(1, &arrow_vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader->disable();
	//if(!text.empty()) {
		//draw_label(text, x, y);
	//}
#endif	
}
////////////
void Renderer::draw_radio(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    int value, const Vector4& inner_color,
		// outline
		bool outline,
		float outline_width,
		const Vector4& outline_color,
		bool outline_antialiased,
		// radius
		float radius
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//----------------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 	
	// RADIO (background)
    // vertex array obj  - stores vertices
    GLuint radio_vertex_array_obj;
    glGenVertexArrays(1, &radio_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(radio_vertex_array_obj); // bind again  
	    GLuint radio_tex_coord_buffer_obj;
        glGenBuffers( 1, &radio_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, radio_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(radio_vertex_array_obj); // bind vertex array obj
        GLuint radio_vertex_buffer_obj;
        glGenBuffers(1, &radio_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, radio_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(radio_vertex_array_obj); // (vao start 1)
	    GLuint radio_element_buffer_obj;
        glGenBuffers(1, &radio_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, radio_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw radio outline
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw radio
	shader->set_float("radius", radius); // radius should be 100%
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(radio_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//----------------------------
	// INNER QUAD (foreground)
	 // vertex array obj  - stores vertices
    GLuint inner_vertex_array_obj;
    glGenVertexArrays(1, &inner_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(inner_vertex_array_obj); // bind again  
	    GLuint inner_tex_coord_buffer_obj;
        glGenBuffers( 1, &inner_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, inner_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(inner_vertex_array_obj); // bind vertex array obj
        GLuint inner_vertex_buffer_obj;
        glGenBuffers(1, &inner_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, inner_vertex_buffer_obj);
		// set position relative to radio 
		// set size to half of width and height
		int fill_x = width  / 4;         
		int fill_y = height / 4;
		int fill_width  = width  / 2;
		int fill_height = height / 2;
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + fill_x)                                     , static_cast<GLfloat>(y + fill_y),
            static_cast<GLfloat>(x + fill_x) + static_cast<GLfloat>(fill_width), static_cast<GLfloat>(y + fill_y),
            static_cast<GLfloat>(x + fill_x) + static_cast<GLfloat>(fill_width), static_cast<GLfloat>(y + fill_y) + static_cast<GLfloat>(fill_height),
            static_cast<GLfloat>(x + fill_x)                                     , static_cast<GLfloat>(y + fill_y) + static_cast<GLfloat>(fill_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(inner_vertex_array_obj); // (vao start 1)
	    GLuint inner_element_buffer_obj;
        glGenBuffers(1, &inner_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inner_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw inner outline
	/*
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    //shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);	
	if(value == 0) shader->set_float("outline_color", (64.0f / 255.0), (64.0f / 255.0), (64.0f / 255.0), (255.0f / 255.0)); 
	if(value == 1) shader->set_float("outline_color", (outline_color.x / 255.0), (outline_color.y / 255.0), (outline_color.z / 255.0), outline_color.w);*/
	// Draw inner quad
	if(value == 1) { // only draw inner quad when value is 1
	shader->set_float("color", (inner_color.x / 255.0), (inner_color.y / 255.0), (inner_color.z / 255.0), inner_color.w);
    glBindVertexArray(inner_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// Restore defaults
	shader->set_float("radius", 0.0);
	shader->set_integer("outline", false);
	}
	//----------------------------
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &radio_tex_coord_buffer_obj);
	glDeleteBuffers(1, &radio_element_buffer_obj);
	glDeleteBuffers(1, &radio_vertex_buffer_obj );
	// inner
	glDeleteBuffers(1, &inner_tex_coord_buffer_obj);
	glDeleteBuffers(1, &inner_element_buffer_obj);
	glDeleteBuffers(1, &inner_vertex_buffer_obj );	
	// Clean arrays
	glDeleteVertexArrays(1, &radio_vertex_array_obj);
	glDeleteVertexArrays(1, &inner_vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif
}
////////////
void Renderer::draw_checkbox(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    bool value, const Vector4& checkmark_color,
	// outline or border
	bool outline,
	float outline_width,
	const Vector4& outline_color,
	bool outline_antialiased,
	// radius
	float radius
) {
#ifdef DOKUN_OPENGL	// OpenGL is defined    
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//----------------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 	
	// CHECKBOX (background)
    // vertex array obj  - stores vertices
    GLuint checkbox_vertex_array_obj;
    glGenVertexArrays(1, &checkbox_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(checkbox_vertex_array_obj); // bind again  
	    GLuint checkbox_tex_coord_buffer_obj;
        glGenBuffers( 1, &checkbox_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, checkbox_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(checkbox_vertex_array_obj); // bind vertex array obj
        GLuint checkbox_vertex_buffer_obj;
        glGenBuffers(1, &checkbox_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, checkbox_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(checkbox_vertex_array_obj); // (vao start 1)
	    GLuint checkbox_element_buffer_obj;
        glGenBuffers(1, &checkbox_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, checkbox_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw checkbox outline or border
    // Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw checkbox (background)
	shader->set_float("radius", radius);
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(checkbox_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// Restore defaults
	shader->set_float("radius", 0.0);
	shader->set_integer("outline", false);
	//----------------------------
#ifdef use_glm	// TEMPORARY
	// uniform
	glm::mat4 model1 = glm::mat4(1.0);;
	model1 = glm::translate(model1, glm::vec3(x + width/2, y + height/2, 0));//model1 = glm::translate(model1, glm::vec3(x, y, 0));
	model1 = glm::rotate(model1, static_cast<float>(45/*angle*/ * 0.0174533), glm::vec3(0, 0, 1));
	model1 = glm::scale(model1, glm::vec3(1, 1, 1));
	model1 = glm::translate(model1, glm::vec3(-x - width/2, -y - height/2, 0));
	//float window_width  = Renderer::get_display_width ();
	//float window_height = Renderer::get_display_height();
	glm::mat4 proj1  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model1));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj1) );
#endif
	// CHECKMARK
	 // vertex array obj  - stores vertices
    GLuint checkmark_vertex_array_obj;
    glGenVertexArrays(1, &checkmark_vertex_array_obj);
	/*// tex_coord buffer obj
    glBindVertexArray(checkmark_vertex_array_obj); // bind again  
	    GLuint checkmark_tex_coord_buffer_obj;
        glGenBuffers( 1, &checkmark_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, checkmark_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0);*/ // unbind
	// vertex buffer obj
    glBindVertexArray(checkmark_vertex_array_obj); // bind vertex array obj
        GLuint checkmark_vertex_buffer_obj;
        glGenBuffers(1, &checkmark_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, checkmark_vertex_buffer_obj);
		// set position relative to checkbox
		int checkmark_width  = width / 4;//5; // make vertical line longer than horz line //width  / 2;
		int checkmark_height = (checkmark_width * 2);
		int checkmark_x = (width - checkmark_width) / 2;//(width / 2) - (checkmark_width / 2);//width  / 4;         
		int checkmark_y = (height - checkmark_height - 2) / 2;//(height / 2) - (checkmark_height / 2);//height / 4;
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
            // horizontal line
		    static_cast<GLfloat>(x + checkmark_x) + static_cast<float>(checkmark_width), static_cast<GLfloat>(y + checkmark_y) + static_cast<float>(checkmark_height),
            static_cast<GLfloat>(x + checkmark_x)                                      , static_cast<GLfloat>(y + checkmark_y) + static_cast<float>(checkmark_height),
            // vertical line
		    static_cast<GLfloat>(x + checkmark_x) + static_cast<float>(checkmark_width), static_cast<GLfloat>(y + checkmark_y),                                   
            static_cast<GLfloat>(x + checkmark_x) + static_cast<float>(checkmark_width), static_cast<GLfloat>(y + checkmark_y) + static_cast<float>(checkmark_height),
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(checkmark_vertex_array_obj); // (vao start 1)
	    GLuint checkmark_element_buffer_obj;
        glGenBuffers(1, &checkmark_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, checkmark_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw inner outline
	/*// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);	
	if(value == 0) shader->set_float("outline_color", (64.0f / 255.0), (64.0f / 255.0), (64.0f / 255.0), (255.0f / 255.0)); 
	if(value == 1) shader->set_float("outline_color", (outline_color.x / 255.0), (outline_color.y / 255.0), (outline_color.z / 255.0), outline_color.w);*/
	// Draw checkmark (foreground)
	if(value == 1) { // only draw mark when box value is true (box is on)
	shader->set_float("color", (checkmark_color.x / 255.0), (checkmark_color.y / 255.0), (checkmark_color.z / 255.0), checkmark_color.w);
    glLineWidth(2.0);//glEnable(GL_LINE_SMOOTH);
    glBindVertexArray(checkmark_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_LINES, 0, 2); // start at index 0, 2 rects// draw 3 points to form a triangle
	    glDrawArrays(GL_LINES, 2, 2); // start at index 2, 2 rects
	glBindVertexArray(0);                // (vao end 2  )
	}
	//----------------------------
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &checkbox_tex_coord_buffer_obj);
	glDeleteBuffers(1, &checkbox_element_buffer_obj);
	glDeleteBuffers(1, &checkbox_vertex_buffer_obj );
	// mark
	//glDeleteBuffers(1, &checkmark_tex_coord_buffer_obj);
	glDeleteBuffers(1, &checkmark_element_buffer_obj);
	glDeleteBuffers(1, &checkmark_vertex_buffer_obj );	
	// Clean arrays
	glDeleteVertexArrays(1, &checkbox_vertex_array_obj);
	glDeleteVertexArrays(1, &checkmark_vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif            
}
////////////
////////////
void Renderer::draw_scrollbar(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	double value, double min_value, double max_value, 
	// handle
	double handle_y, int handle_height, const Vector4& handle_color,
	// button
	bool button, int button_height, const Vector4& button_color,
	// arrow
	bool arrow, const Vector4& arrow_color,
	// outline
	bool outline, 
	float outline_width, 
	const Vector4& outline_color,
	bool outline_antialiased,
	// border
	bool border)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
    // use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
    //------------------------
	// tex_coord array
	GLfloat tex_coords[] =  // texture coordinates range from (0,0) to (1, 1)
	{
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	// vertex array - all unique
	// element array
	GLuint indices[] = {0, 1, 3,  1, 2, 3,};
	//--------------------
	// BORDER - not sure If scrollbars need a border
	//--------------------
    // SCROLLBAR
    // vertex array obj  - stores vertices
    GLuint scrollbar_vertex_array_obj;
    glGenVertexArrays(1, &scrollbar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(scrollbar_vertex_array_obj); // bind again  
	    GLuint scrollbar_tex_coord_buffer_obj;
        glGenBuffers( 1, &scrollbar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, scrollbar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(scrollbar_vertex_array_obj); // bind vertex array obj
        GLuint scrollbar_vertex_buffer_obj;
        glGenBuffers(1, &scrollbar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, scrollbar_vertex_buffer_obj);  
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),    
        }; 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(scrollbar_vertex_array_obj); // (vao start 1)
	    GLuint scrollbar_element_buffer_obj;
        glGenBuffers(1, &scrollbar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scrollbar_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw scrollbar
    shader->set_float("color", (red/ 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(scrollbar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //------------------------
	// TOP BUTTON - optional
    // vertex array obj  - stores vertices
    GLuint top_button_vertex_array_obj;
    glGenVertexArrays(1, &top_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind again  
	    GLuint top_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &top_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, top_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind vertex array obj
        GLuint top_button_vertex_buffer_obj;
        glGenBuffers(1, &top_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, top_button_vertex_buffer_obj);
		int top_x      = 0;
		int top_y      = -button_height; // move up 20 units away from scrollbar
		int top_width  = width;
		int top_height = button_height;
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + top_x)                                    , static_cast<GLfloat>(y + top_y),
            static_cast<GLfloat>(x + top_x) + static_cast<GLfloat>(top_width), static_cast<GLfloat>(y + top_y),
            static_cast<GLfloat>(x + top_x) + static_cast<GLfloat>(top_width), static_cast<GLfloat>(y + top_y) + static_cast<GLfloat>(top_height),
            static_cast<GLfloat>(x + top_x)                                    , static_cast<GLfloat>(y + top_y) + static_cast<GLfloat>(top_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(top_button_vertex_array_obj); // (vao start 1)
	    GLuint top_button_element_buffer_obj;
        glGenBuffers(1, &top_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, top_button_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	if(button)
	{
    // Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw top_button
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);
    glBindVertexArray(top_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	}
	//-------------------------
	// UP ARROW
	// up_arrow_color
	//shader->set_float("color", (32.0f / 255.0), (32.0f / 255.0), (32.0f / 255.0), (255 / 255.0));
    //-------------------------
	// BOTTOM BUTTON - optional
    // vertex array obj  - stores vertices
    GLuint bottom_button_vertex_array_obj;
    glGenVertexArrays(1, &bottom_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind again  
	    GLuint bottom_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &bottom_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bottom_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind vertex array obj
        GLuint bottom_button_vertex_buffer_obj;
        glGenBuffers(1, &bottom_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bottom_button_vertex_buffer_obj);
		// set position relative to scrollbar
		int bottom_x      = 0;
		int bottom_y      = height; // move 'height' units down past the scrollbar
		int bottom_width  = width; // dont worry about this
		int bottom_height = button_height;
		GLfloat vertices3[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + bottom_x)                                       , static_cast<GLfloat>(y + bottom_y),
            static_cast<GLfloat>(x + bottom_x) + static_cast<GLfloat>(bottom_width), static_cast<GLfloat>(y + bottom_y),
            static_cast<GLfloat>(x + bottom_x) + static_cast<GLfloat>(bottom_width), static_cast<GLfloat>(y + bottom_y) + static_cast<GLfloat>(bottom_height),
            static_cast<GLfloat>(x + bottom_x)                                       , static_cast<GLfloat>(y + bottom_y) + static_cast<GLfloat>(bottom_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3)* sizeof(GLfloat), vertices3, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 1)
	    GLuint bottom_button_element_buffer_obj;
        glGenBuffers(1, &bottom_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottom_button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw bottom_button outline
	if(button)
	{
    // Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw bottom_button
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);
    glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	}
	// -------------------------------
	// DOWN ARROW 
	// down_arrow_color
	//shader->set_float("color", (32.0f / 255.0), (32.0f / 255.0), (32.0f / 255.0), (255 / 255.0));
    // -------------------------------
	// HANDLE 
    // vertex array obj  - stores vertices
    GLuint handle_vertex_array_obj;
    glGenVertexArrays(1, &handle_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind again  
	    GLuint handle_tex_coord_buffer_obj;
        glGenBuffers( 1, &handle_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, handle_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind vertex array obj
        GLuint handle_vertex_buffer_obj;
        glGenBuffers(1, &handle_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, handle_vertex_buffer_obj);
		// controls where the bar goes
		// to get x position of pivot = x + (value * 2). If value is 100 then pivot is 200
		// to get y position of pivot = y
		// position relative to the scrollbar
		//numScrollPositions = (numItemsInList - numItemsVisible) + 1
		double step = (value / max_value) * static_cast<GLfloat>(height - handle_height); // if(button)  (value / max_value) * static_cast<GLfloat>(height - handle_height - button_height);  // move handle until it reaches end of scrollbar_height 
		// position and size
		double handle_x  = 0;//(button == true) ? top_x - top_height : 0; // always 0 for now
		handle_y         = handle_y + step; // step // value starts at 0 // 5 is the max value
		int handle_width = width; // same width as bar
		//std::cout << "handle_y: " << handle_y << std::endl;
		//float step = value / max_value; // value changes when pivotx is moved  		
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + handle_x)                                     , static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),
            static_cast<GLfloat>(x + handle_x)                                     , static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),   
        };      //std::cout << x + ball_x << " pivot_x\n";
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(handle_vertex_array_obj); // (vao start 1)
	    GLuint handle_element_buffer_obj;
        glGenBuffers(1, &handle_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw handle_outline
	/*// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/	
	// Draw handle
    shader->set_float("color", (handle_color.x/ 255.0), (handle_color.y/ 255.0), (handle_color.z / 255.0), handle_color.w); //  foreground_color	
    glBindVertexArray(handle_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// Clean textures
	// Clean buffers
	// :scrollbar
	glDeleteBuffers(1, &scrollbar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &scrollbar_element_buffer_obj  );
	glDeleteBuffers(1, &scrollbar_vertex_buffer_obj   );
	// :handle
	glDeleteBuffers(1, &handle_tex_coord_buffer_obj);
	glDeleteBuffers(1, &handle_element_buffer_obj  );
	glDeleteBuffers(1, &handle_vertex_buffer_obj   );
	// : top_button
	glDeleteBuffers(1, &top_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &top_button_element_buffer_obj  );
	glDeleteBuffers(1, &top_button_vertex_buffer_obj   );	
	// : bottom_button
	glDeleteBuffers(1, &bottom_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bottom_button_element_buffer_obj  );
	glDeleteBuffers(1, &bottom_button_vertex_buffer_obj   );
    // : top_arrow
    // : bottom_arrow	
	// Clean arrays
	// :scrollbar
	glDeleteVertexArrays(1, &scrollbar_vertex_array_obj);
	// :handle
	glDeleteVertexArrays(1, &handle_vertex_array_obj   );
	// : top_button [<]
	glDeleteVertexArrays(1, &top_button_vertex_array_obj);
	// : bottom_button [>]
	glDeleteVertexArrays(1, &bottom_button_vertex_array_obj);
    // : top_arrow
    // : bottom_arrow	
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif
}
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
void Renderer::draw_spinner0(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    double value,		
	// button
    int button_width, const Vector4& button_color, 
    // shape (arrow)
    const Vector2& arrow_size, const Vector4& arrow_color,
	// border
	bool outline,
	float outline_width,
	const Vector4& outline_color,
	bool outline_antialiased)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//---------------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };	
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	// -------------------------------
	// SPINNER
    // vertex array obj  - stores vertices
    GLuint spinner_vertex_array_obj;
    glGenVertexArrays(1, &spinner_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(spinner_vertex_array_obj); // bind again  
	    GLuint spinner_tex_coord_buffer_obj;
        glGenBuffers( 1, &spinner_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, spinner_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(spinner_vertex_array_obj); // bind vertex array obj
        GLuint spinner_vertex_buffer_obj;
        glGenBuffers(1, &spinner_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, spinner_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(spinner_vertex_array_obj); // (vao start 1)
	    GLuint spinner_element_buffer_obj;
        glGenBuffers(1, &spinner_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spinner_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw _
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(spinner_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// ------------------------------
	// TOP_BUTTON
    // vertex array obj  - stores vertices
    GLuint top_button_vertex_array_obj;
    glGenVertexArrays(1, &top_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind again  
	    GLuint top_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &top_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, top_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind vertex array obj
        GLuint top_button_vertex_buffer_obj;
        glGenBuffers(1, &top_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, top_button_vertex_buffer_obj);
		float top_button_x = width;
		float top_button_y = 0;
		float top_button_width  = button_width;
		float top_button_height = (height / 2);
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + top_button_x)                                           , static_cast<GLfloat>(y + top_button_y),
            static_cast<GLfloat>(x + top_button_x) + static_cast<GLfloat>(top_button_width), static_cast<GLfloat>(y + top_button_y),
            static_cast<GLfloat>(x + top_button_x) + static_cast<GLfloat>(top_button_width), static_cast<GLfloat>(y + top_button_y) + static_cast<GLfloat>(top_button_height),
            static_cast<GLfloat>(x + top_button_x)                                           , static_cast<GLfloat>(y + top_button_y) + static_cast<GLfloat>(top_button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(top_button_vertex_array_obj); // (vao start 1)
	    GLuint top_button_element_buffer_obj;
        glGenBuffers(1, &top_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, top_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw _
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(top_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// ------------------------------
	// BOTTOM_BUTTON
    // vertex array obj  - stores vertices
    GLuint bottom_button_vertex_array_obj;
    glGenVertexArrays(1, &bottom_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind again  
	    GLuint bottom_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &bottom_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bottom_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind vertex array obj
        GLuint bottom_button_vertex_buffer_obj;
        glGenBuffers(1, &bottom_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bottom_button_vertex_buffer_obj);
		float bottom_button_x = width;
		float bottom_button_y = (height / 2);
		float bottom_button_width  = button_width;
		float bottom_button_height = (height / 2);
		//std::cout << Vector4(x+bottom_button_x, y+bottom_button_y, bottom_button_width, bottom_button_height) << " in Renderer button_rect\n";
		GLfloat vertices2[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + bottom_button_x)                                              , static_cast<GLfloat>(y + bottom_button_y),
            static_cast<GLfloat>(x + bottom_button_x) + static_cast<GLfloat>(bottom_button_width), static_cast<GLfloat>(y + bottom_button_y),
            static_cast<GLfloat>(x + bottom_button_x) + static_cast<GLfloat>(bottom_button_width), static_cast<GLfloat>(y + bottom_button_y) + static_cast<GLfloat>(bottom_button_height),
            static_cast<GLfloat>(x + bottom_button_x)                                              , static_cast<GLfloat>(y + bottom_button_y) + static_cast<GLfloat>(bottom_button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 1)
	    GLuint bottom_button_element_buffer_obj;
        glGenBuffers(1, &bottom_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottom_button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw _
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//////////////////
    // TOP_TRIANGLE
    // vertex array obj  - stores vertices
    GLuint top_arrow_vertex_array_obj;
    glGenVertexArrays(1, &top_arrow_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(top_arrow_vertex_array_obj); // bind vertex array obj
        GLuint top_arrow_vertex_buffer_obj;
        glGenBuffers(1, &top_arrow_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, top_arrow_vertex_buffer_obj);
        // set properties
        double arrow_x = width + (top_button_width / 2);//width;
        double arrow_y = top_button_height / 2;
        int arrow_width  = arrow_size.x;//5;
        int arrow_height = arrow_size.y;//3;//top_button_height / 4;
		GLfloat verticestri[] = { // when x and y are 0 then from wwidth-wheight
            // normal triangle // if down
            static_cast<GLfloat>(x + arrow_x)                                     , static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height),
            static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(-arrow_width), static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ),
            static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width ), static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ),            
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticestri)* sizeof(GLfloat), verticestri, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(top_arrow_vertex_array_obj); // (vao start 1)
	    GLuint top_arrow_element_buffer_obj;
        glGenBuffers(1, &top_arrow_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, top_arrow_element_buffer_obj);
		GLuint indicestri[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicestri)* sizeof(GLuint), indicestri, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw triangle
	shader->set_float("color", (arrow_color.x / 255.0), (arrow_color.y / 255.0), (arrow_color.z / 255.0), arrow_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(top_arrow_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 points to form a triangle
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	//////////////////////    
	// BUTTOM_TRIANGLE
    // vertex array obj  - stores vertices
    GLuint bottom_arrow_vertex_array_obj;
    glGenVertexArrays(1, &bottom_arrow_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(bottom_arrow_vertex_array_obj); // bind vertex array obj
        GLuint bottom_arrow_vertex_buffer_obj;
        glGenBuffers(1, &bottom_arrow_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bottom_arrow_vertex_buffer_obj);
        // set properties
        double arrow_xx = width + (bottom_button_width / 2);//width;
        double arrow_yy = (height + bottom_button_height) / 2;
		GLfloat verticestrii[] = { // when x and y are 0 then from wwidth-wheight
            // upside-down triangle // if up
            static_cast<GLfloat>(x + arrow_xx)                                     , static_cast<GLfloat>(y + arrow_yy) + static_cast<GLfloat>(arrow_height ),
            static_cast<GLfloat>(x + arrow_xx) + static_cast<GLfloat>(arrow_width ), static_cast<GLfloat>(y + arrow_yy) + static_cast<GLfloat>(-arrow_height),
            static_cast<GLfloat>(x + arrow_xx) + static_cast<GLfloat>(-arrow_width), static_cast<GLfloat>(y + arrow_yy) + static_cast<GLfloat>(-arrow_height),           
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticestrii)* sizeof(GLfloat), verticestrii, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bottom_arrow_vertex_array_obj); // (vao start 1)
	    GLuint bottom_arrow_element_buffer_obj;
        glGenBuffers(1, &bottom_arrow_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottom_arrow_element_buffer_obj);
		GLuint indicestrii[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicestrii)* sizeof(GLuint), indicestrii, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	// Draw triangle
	shader->set_float("color", (arrow_color.x / 255.0), (arrow_color.y / 255.0), (arrow_color.z / 255.0), arrow_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(bottom_arrow_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 points to form a triangle
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	//////////////////////	
	//////////////////
	// SEPARATOR (LINE)
    // vertex array obj  - stores vertices
    GLuint separator_vertex_array_obj;
    glGenVertexArrays(1, &separator_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(separator_vertex_array_obj); // bind vertex array obj
        GLuint separator_vertex_buffer_obj;
        glGenBuffers(1, &separator_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, separator_vertex_buffer_obj);
        double separator_x = top_button_x;
		double separator_y = top_button_height;//separator_y + top_padding;
		int separator_width  = top_button_width;
		int separator_height = 1;//10; // bottom_padding // edit height
		GLfloat vertices11[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + separator_x) + static_cast<float>(separator_width), static_cast<GLfloat>(y + separator_y),
            static_cast<GLfloat>(x + separator_x)                                      , static_cast<GLfloat>(y + separator_y),
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices11)* sizeof(GLfloat), vertices11, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// Draw seperator
	shader->set_float("color", (0 / 255.0), (0 / 255.0), (0 / 255.0), (255 / 255.0));
    glEnable(GL_LINE_SMOOTH); // makes line a little less darker and more smooth
	glLineWidth(static_cast<float>(separator_height)); // thickness of the seperator (will act as height since separator is horz)
	glBindVertexArray(separator_vertex_array_obj);
        glDrawArrays(GL_LINES, 0,  2); // 2 points make up a line
	glBindVertexArray(0);
	//-----------------------------------
	//////////////////
	// Clean buffers
	glDeleteBuffers(1, &spinner_tex_coord_buffer_obj);
	glDeleteBuffers(1, &spinner_element_buffer_obj);
	glDeleteBuffers(1, &spinner_vertex_buffer_obj );
	// : top_
	glDeleteBuffers(1, &top_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &top_button_element_buffer_obj);
	glDeleteBuffers(1, &top_button_vertex_buffer_obj );
    // bottom_
	glDeleteBuffers(1, &bottom_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bottom_button_element_buffer_obj);
	glDeleteBuffers(1, &bottom_button_vertex_buffer_obj );	
	// : line_seperator
	glDeleteBuffers(1, &separator_vertex_buffer_obj);
	// : top_triangle
	glDeleteBuffers(1, &top_arrow_vertex_buffer_obj);
	glDeleteBuffers(1, &top_arrow_element_buffer_obj);
	// : bottom triangle
	glDeleteBuffers(1, &bottom_arrow_vertex_buffer_obj);
	glDeleteBuffers(1, &bottom_arrow_element_buffer_obj);
	// Clean arrays
	glDeleteVertexArrays(1, &spinner_vertex_array_obj);
	glDeleteVertexArrays(1, &top_button_vertex_array_obj);
	glDeleteVertexArrays(1, &bottom_button_vertex_array_obj);
	glDeleteVertexArrays(1, &separator_vertex_array_obj);
	glDeleteVertexArrays(1, &top_arrow_vertex_array_obj);
	glDeleteVertexArrays(1, &bottom_arrow_vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
#endif
}
////////////
void Renderer::draw_spinner(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    int button_width, const Vector4& button_color,        // shape (+, -)
	int shape_size, const Vector4& shape_color, double shape_depth,
	bool separator, int separator_size) {
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//---------------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };	
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	// -------------------------------
	// SPINNER
    // vertex array obj  - stores vertices
    GLuint spinner_vertex_array_obj;
    glGenVertexArrays(1, &spinner_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(spinner_vertex_array_obj); // bind again  
	    GLuint spinner_tex_coord_buffer_obj;
        glGenBuffers( 1, &spinner_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, spinner_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(spinner_vertex_array_obj); // bind vertex array obj
        GLuint spinner_vertex_buffer_obj;
        glGenBuffers(1, &spinner_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, spinner_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(spinner_vertex_array_obj); // (vao start 1)
	    GLuint spinner_element_buffer_obj;
        glGenBuffers(1, &spinner_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spinner_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw _
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(spinner_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// ------------------------------
	//////////////////////
	// LEFT_BUTTON
    // vertex array obj  - stores vertices
    GLuint left_button_vertex_array_obj;
    glGenVertexArrays(1, &left_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(left_button_vertex_array_obj); // bind again  
	    GLuint left_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &left_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, left_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(left_button_vertex_array_obj); // bind vertex array obj
        GLuint left_button_vertex_buffer_obj;
        glGenBuffers(1, &left_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, left_button_vertex_buffer_obj);
		// set properties
		double left_button_x = (!separator) ? x - button_width : x - button_width - separator_size; // -1 is the gap between left-button and spinner-center
		double left_button_y = y;
		int left_button_width = button_width;//width / 2;//-5;
		int left_button_height = height;
		GLfloat verticeslef[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(left_button_x)                                          , static_cast<GLfloat>(left_button_y),
            static_cast<GLfloat>(left_button_x) + static_cast<GLfloat>(left_button_width), static_cast<GLfloat>(left_button_y),
            static_cast<GLfloat>(left_button_x) + static_cast<GLfloat>(left_button_width), static_cast<GLfloat>(left_button_y) + static_cast<GLfloat>(left_button_height),
            static_cast<GLfloat>(left_button_x)                                          , static_cast<GLfloat>(left_button_y) + static_cast<GLfloat>(left_button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticeslef)* sizeof(GLfloat), verticeslef, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(left_button_vertex_array_obj); // (vao start 1)
	    GLuint left_button_element_buffer_obj;
        glGenBuffers(1, &left_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, left_button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw _
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(left_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	//////////////////////
	// RIGHT_BUTTON
    // vertex array obj  - stores vertices
    GLuint right_button_vertex_array_obj;
    glGenVertexArrays(1, &right_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(right_button_vertex_array_obj); // bind again  
	    GLuint right_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &right_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, right_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(right_button_vertex_array_obj); // bind vertex array obj
        GLuint right_button_vertex_buffer_obj;
        glGenBuffers(1, &right_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, right_button_vertex_buffer_obj);
		// set properties
		double right_button_x = (!separator) ? x + width : x + width + separator_size; // +1 is the gap between right-button and spinner-center
		double right_button_y = y;
		int right_button_width = button_width;
		int right_button_height = height;		
		GLfloat verticesrig[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(right_button_x)                                           , static_cast<GLfloat>(right_button_y),
            static_cast<GLfloat>(right_button_x) + static_cast<GLfloat>(right_button_width), static_cast<GLfloat>(right_button_y),
            static_cast<GLfloat>(right_button_x) + static_cast<GLfloat>(right_button_width), static_cast<GLfloat>(right_button_y) + static_cast<GLfloat>(right_button_height),
            static_cast<GLfloat>(right_button_x)                                           , static_cast<GLfloat>(right_button_y) + static_cast<GLfloat>(right_button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesrig)* sizeof(GLfloat), verticesrig, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(right_button_vertex_array_obj); // (vao start 1)
	    GLuint right_button_element_buffer_obj;
        glGenBuffers(1, &right_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, right_button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw _
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(right_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//////////////////////
    // PLUS_SHAPE
    // vertex array obj  - stores vertices
    GLuint plus_vertex_array_obj;
    glGenVertexArrays(1, &plus_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(plus_vertex_array_obj); // bind vertex array obj
        GLuint plus_vertex_buffer_obj;
        glGenBuffers(1, &plus_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, plus_vertex_buffer_obj);
        // set properties
        int plus_width  = shape_size;//10;
        int plus_height = shape_size;//10; // height        
        double plus_x = (right_button_width  / 2) - (plus_width  / 2);
        double plus_y = (right_button_height / 2) - (plus_height / 2);
		GLfloat verticesplu[] = { // when x and y are 0 then from wwidth-wheight
            // horizontal line
		    static_cast<GLfloat>(right_button_x + plus_x) + static_cast<float>(plus_width), static_cast<GLfloat>(right_button_y + plus_y) + static_cast<float>(plus_height / 2), // move y axis to middle of vertical line
            static_cast<GLfloat>(right_button_x + plus_x)                                 , static_cast<GLfloat>(right_button_y + plus_y) + static_cast<float>(plus_height / 2), // move y axis to middle of vertical line               
            // vertical line
		    static_cast<GLfloat>(right_button_x + plus_x) + static_cast<float>(plus_width / 2), static_cast<GLfloat>(right_button_y + plus_y),                                   // move x axis to middle of horizontal line
            static_cast<GLfloat>(right_button_x + plus_x) + static_cast<float>(plus_width / 2), static_cast<GLfloat>(right_button_y + plus_y) + static_cast<float>(plus_height), // move x axis to middle of horizontal line        
        };              
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesplu)* sizeof(GLfloat), verticesplu, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(plus_vertex_array_obj); // (vao start 1)
	    GLuint plus_element_buffer_obj;
        glGenBuffers(1, &plus_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plus_element_buffer_obj);
		//GLuint indicestri[] = {1, 2, 3,  0};
		GLuint indicesplu[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesplu)* sizeof(GLuint), indicesplu, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw lines
	shader->set_float("color", (shape_color.x / 255.0), (shape_color.y / 255.0), (shape_color.z / 255.0), shape_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glLineWidth(static_cast<GLfloat>(shape_depth)); // thickness // range from 0.0 -> 10.0
    glBindVertexArray(plus_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_LINES, 0, 2); // start at index 0, 2 rects// draw 3 points to form a triangle
	    glDrawArrays(GL_LINES, 2, 2); // start at index 2, 2 rects
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	//////////////////////
    // MINUS_SHAPE
    // vertex array obj  - stores vertices
    GLuint minus_vertex_array_obj;
    glGenVertexArrays(1, &minus_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(minus_vertex_array_obj); // bind vertex array obj
        GLuint minus_vertex_buffer_obj;
        glGenBuffers(1, &minus_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, minus_vertex_buffer_obj);
        // set properties
        int minus_width = shape_size;//10;
        int minus_top   = shape_size;//10; // height or top_padding
        double minus_x = (left_button_width  / 2) - (minus_width / 2);
        double minus_y = (left_button_height / 2) - (minus_top   / 2);
		GLfloat verticesmin[] = { // when x and y are 0 then from wwidth-wheight
            // horizontal line
		    static_cast<GLfloat>(left_button_x + minus_x) + static_cast<float>(minus_width), static_cast<GLfloat>(left_button_y + minus_y) + static_cast<float>(minus_top / 2), // move y axis vertically
            static_cast<GLfloat>(left_button_x + minus_x)                                  , static_cast<GLfloat>(left_button_y + minus_y) + static_cast<float>(minus_top / 2), // move y axis vertically        
        };              
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesmin)* sizeof(GLfloat), verticesmin, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(minus_vertex_array_obj); // (vao start 1)
	    GLuint minus_element_buffer_obj;
        glGenBuffers(1, &minus_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, minus_element_buffer_obj);
		//GLuint indicestri[] = {1, 2, 3,  0};
		GLuint indicesduo[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesduo)* sizeof(GLuint), indicesduo, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw lines
	shader->set_float("color", (shape_color.x / 255.0), (shape_color.y / 255.0), (shape_color.z / 255.0), shape_color.w);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	//already set above//glLineWidth(2.0); // thickness (real height) // range from 0.0 -> 10.0
    glBindVertexArray(minus_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_LINES, 0, 2); // 2 points make up a line
	    //glDrawArrays(GL_LINES, 2, 2);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	////////////////////// 	
	//////////////////
	// Clean buffers
	// spinner
	glDeleteBuffers(1, &spinner_tex_coord_buffer_obj);
	glDeleteBuffers(1, &spinner_element_buffer_obj  );
	glDeleteBuffers(1, &spinner_vertex_buffer_obj   );
	// : left_button
	glDeleteBuffers(1, &left_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &left_button_element_buffer_obj  );
	glDeleteBuffers(1, &left_button_vertex_buffer_obj   );
    // right_button
	glDeleteBuffers(1, &right_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &right_button_element_buffer_obj);
	glDeleteBuffers(1, &right_button_vertex_buffer_obj );	
	// plus_
	glDeleteBuffers(1, &plus_vertex_buffer_obj );
	glDeleteBuffers(1, &plus_element_buffer_obj);
	// minus_
	glDeleteBuffers(1, &minus_vertex_buffer_obj );
	glDeleteBuffers(1, &minus_element_buffer_obj);
	//----------------------------------------------------
	// Clean arrays
	glDeleteVertexArrays(1, &spinner_vertex_array_obj     );
	glDeleteVertexArrays(1, &left_button_vertex_array_obj );
	glDeleteVertexArrays(1, &right_button_vertex_array_obj);
	// use +1 and -1 as x_pos seperator gap 
	glDeleteVertexArrays(1, &plus_vertex_array_obj        );
	glDeleteVertexArrays(1, &minus_vertex_array_obj       );
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
}
////////////
////////////
////////////
////////////
void Renderer::draw_combobox(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    const Vector4& button_color, int button_width, bool button_on)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
		
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif		
	///////////////////////
	///////////////////////
	// COMBOBOX
    // vertex array obj  - stores vertices
    GLuint combo_vertex_array_obj;
    glGenVertexArrays(1, &combo_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(combo_vertex_array_obj); // bind again  
	    GLuint combo_tex_coord_buffer_obj;
        glGenBuffers( 1, &combo_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, combo_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(combo_vertex_array_obj); // bind vertex array obj
        GLuint combo_vertex_buffer_obj;
        glGenBuffers(1, &combo_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, combo_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(combo_vertex_array_obj); // (vao start 1)
	    GLuint combo_element_buffer_obj;
        glGenBuffers(1, &combo_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, combo_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw combobox
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(combo_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//----------------------------------
	// BUTTON
    // vertex array obj  - stores vertices
    GLuint button_vertex_array_obj;
    glGenVertexArrays(1, &button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind again  
	    GLuint button_tex_coord_buffer_obj;
        glGenBuffers( 1, &button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind vertex array obj
        GLuint button_vertex_buffer_obj;
        glGenBuffers(1, &button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, button_vertex_buffer_obj);
		// set position relative to combobox
		float button_x = width;
		float button_y = 0;
		int button_height = height;
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + button_x)                                       , static_cast<GLfloat>(y + button_y),
            static_cast<GLfloat>(x + button_x) + static_cast<GLfloat>(button_width), static_cast<GLfloat>(y + button_y),
            static_cast<GLfloat>(x + button_x) + static_cast<GLfloat>(button_width), static_cast<GLfloat>(y + button_y) + static_cast<GLfloat>(button_height),
            static_cast<GLfloat>(x + button_x)                                       , static_cast<GLfloat>(y + button_y) + static_cast<GLfloat>(button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(button_vertex_array_obj); // (vao start 1)
	    GLuint button_element_buffer_obj;
        glGenBuffers(1, &button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw button
	shader->set_float("color", (button_color.x / 255.0), (button_color.y / 255.0), (button_color.z / 255.0), button_color.w);//shader.set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//////////////////////
    // TRIANGLE	
    // vertex array obj  - stores vertices
    GLuint arrow_vertex_array_obj;
    glGenVertexArrays(1, &arrow_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(arrow_vertex_array_obj); // bind vertex array obj
        GLuint arrow_vertex_buffer_obj;
        glGenBuffers(1, &arrow_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, arrow_vertex_buffer_obj);
        // set properties
        double arrow_x = width + (button_width / 2);//width;
        double arrow_y = height / 2;
        int arrow_width  = 5; // wide - looks more smooth when width is bigger than height
        int arrow_height = 2;//2.5; // thin
        std::vector<GLfloat> verticestri;
        if(!button_on) {
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width ));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(-arrow_width));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height));
        }
        if(button_on) {
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(-arrow_height));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(-arrow_width));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ));
            verticestri.push_back(static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width ));
            verticestri.push_back(static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height ));
        }     
        glBufferData(GL_ARRAY_BUFFER, verticestri.size() * sizeof(GLfloat), verticestri.data(), GL_STATIC_DRAW);//glBufferData(GL_ARRAY_BUFFER, sizeof(verticestri)* sizeof(GLfloat), verticestri, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(arrow_vertex_array_obj); // (vao start 1)
	    GLuint arrow_element_buffer_obj;
        glGenBuffers(1, &arrow_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrow_element_buffer_obj);
		GLuint indicestri[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicestri)* sizeof(GLuint), indicestri, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw triangle
	shader->set_float("color", (255.0 / 255.0), (255.0 / 255.0), (255.0 / 255.0), (255.0 / 255.0));
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(arrow_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 points to form a triangle
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	//////////////////////
	// Clean buffers
	glDeleteBuffers(1, &combo_tex_coord_buffer_obj);
	glDeleteBuffers(1, &combo_element_buffer_obj  );
	glDeleteBuffers(1, &combo_vertex_buffer_obj   );
	// : button
	glDeleteBuffers(1, &button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &button_element_buffer_obj  );
	glDeleteBuffers(1, &button_vertex_buffer_obj   );	
	// : triangle
	//glDeleteBuffers(1, &arrow_tex_coord_buffer_obj); // texture not used on triangle
	glDeleteBuffers(1, &arrow_element_buffer_obj  );
	glDeleteBuffers(1, &arrow_vertex_buffer_obj   );
	// Clean arrays
	glDeleteVertexArrays(1, &combo_vertex_array_obj );
	glDeleteVertexArrays(1, &button_vertex_array_obj);
	glDeleteVertexArrays(1, &arrow_vertex_array_obj );
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
	//////////////////
	//draw_triangle(x, y, 32, 32, -45.0f * 0.0174533, 1, 1, 255, 255, 255.0);
#endif
}
////////////
void Renderer::draw_tab(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    int tab_count, bool visible)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// use shader
	shader->use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
		
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader->get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif		
	///////////////////////
	///////////////////////
	// TAB_HEAD
    // vertex array obj  - stores vertices
    GLuint tab_vertex_array_obj;
    glGenVertexArrays(1, &tab_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(tab_vertex_array_obj); // bind again  
	    GLuint tab_tex_coord_buffer_obj;
        glGenBuffers( 1, &tab_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tab_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); //int instAttrib1 = 1;
	    //glVertexAttribDivisor(instAttrib1, 1); // works with glDrawElementsInstanced 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(tab_vertex_array_obj); // bind vertex array obj
        GLuint tab_vertex_buffer_obj;
        glGenBuffers(1, &tab_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, tab_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                              , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                              , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0); //int instAttrib0 = 0;
	    //glVertexAttribDivisor(instAttrib0, 1); // works with glDrawElementsInstanced
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(tab_vertex_array_obj); // (vao start 1)
	    GLuint tab_element_buffer_obj;
        glGenBuffers(1, &tab_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tab_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);//shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);
	shader->set_float("outline_color", (32.0 / 255.0), (32.0 / 255.0), (32.0 / 255.0), (255.0 / 255.0));*/
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(1.0); // outline_width
	glBindVertexArray(tab_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Draw tab (head)	
	double current_x = x + tab_count * width;
	double current_y = y + tab_count * height;
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(tab_vertex_array_obj); // (vao start 2)  
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4); // (as we have 4 instances specified in array)
	glBindVertexArray(0);                // (vao end 2  )
	//----------------------------------
	//////////////////////
	//////////////////////
	// TAB BODY
    // vertex array obj  - stores vertices
    GLuint tab_body_vertex_array_obj;
    glGenVertexArrays(1, &tab_body_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(tab_body_vertex_array_obj); // bind again  
	    GLuint tab_body_tex_coord_buffer_obj;
        glGenBuffers( 1, &tab_body_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tab_body_tex_coord_buffer_obj);
		GLfloat tab_body_tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tab_body_tex_coords)* sizeof(GLfloat), tab_body_tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// set size then set position relative to tab
	int tab_body_width  = width  * 5;
	int tab_body_height = height * 5;
	double tab_body_x = 0;
	double tab_body_y = 0 + height; // 0 + tab_head_height // bring down the tab_body
	// vertex buffer obj
    glBindVertexArray(tab_body_vertex_array_obj); // bind vertex array obj
        GLuint tab_body_vertex_buffer_obj;
        glGenBuffers(1, &tab_body_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, tab_body_vertex_buffer_obj);
		GLfloat tab_body_vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + tab_body_x)                                       , static_cast<GLfloat>(y + tab_body_y),
            static_cast<GLfloat>(x + tab_body_x) + static_cast<GLfloat>(tab_body_width), static_cast<GLfloat>(y + tab_body_y),
            static_cast<GLfloat>(x + tab_body_x) + static_cast<GLfloat>(tab_body_width), static_cast<GLfloat>(y + tab_body_y) + static_cast<GLfloat>(tab_body_height),
            static_cast<GLfloat>(x + tab_body_x)                                       , static_cast<GLfloat>(y + tab_body_y) + static_cast<GLfloat>(tab_body_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(tab_body_vertices)* sizeof(GLfloat), tab_body_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(tab_body_vertex_array_obj); // (vao start 1)
	    GLuint tab_body_element_buffer_obj;
        glGenBuffers(1, &tab_body_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tab_body_element_buffer_obj);
		GLuint tab_body_indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tab_body_indices)* sizeof(GLuint), tab_body_indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
	shader->set_float("color", (32.0 / 255.0), (32.0 / 255.0), (32.0 / 255.0), (32.0 / 255.0));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(5.0); // outline_width
	glBindVertexArray(tab_body_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Draw tab (body)
	shader->set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    glBindVertexArray(tab_body_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	//////////////////
	//////////////////
	// SEPERATOR (LINE)
    // vertex array obj  - stores vertices
/*    
    GLuint separator_vertex_array_obj;
    glGenVertexArrays(1, &separator_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(separator_vertex_array_obj); // bind vertex array obj
        GLuint separator_vertex_buffer_obj;
        glGenBuffers(1, &separator_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, separator_vertex_buffer_obj);
        double separator_x = 0;
		double separator_y = height;//separator_y + top_padding;
		int separator_width  = width;
		int separator_height = 0;//10; // bottom_padding // edit height
		GLfloat vertices11[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + separator_x) + static_cast<float>(separator_width), static_cast<GLfloat>(y + separator_y) + static_cast<float>(separator_height),
            static_cast<GLfloat>(x + separator_x)                                      , static_cast<GLfloat>(y + separator_y) + static_cast<float>(separator_height),
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices11)* sizeof(GLfloat), vertices11, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// Draw seperator
	shader->set_float("color", (0 / 255.0), (0 / 255.0), (0 / 255.0), (255 / 255.0));
    //glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0); // width of the seperator
	glBindVertexArray(separator_vertex_array_obj);
        glDrawArrays(GL_LINES, 0,  2); // 2 points make up a line
	glBindVertexArray(0);     
*/	    
	//-----------------------------------	
	// Clean buffers
	glDeleteBuffers(1, &tab_tex_coord_buffer_obj);
	glDeleteBuffers(1, &tab_element_buffer_obj  );
	glDeleteBuffers(1, &tab_vertex_buffer_obj   );
	// : tab_body
	glDeleteBuffers(1, &tab_body_tex_coord_buffer_obj);
	glDeleteBuffers(1, &tab_body_element_buffer_obj  );
	glDeleteBuffers(1, &tab_body_vertex_buffer_obj   );
	// : seperator
	//glDeleteBuffers(1, &separator_vertex_buffer_obj);
	// Clean arrays
	glDeleteVertexArrays(1, &tab_vertex_array_obj );
	glDeleteVertexArrays(1, &tab_body_vertex_array_obj);
	//glDeleteVertexArrays(1, &separator_vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader->disable();
	//////////////////	
#endif
}
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
void Renderer::draw_point()
{}
void Renderer::draw_line(float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
    //if(!text.empty()) draw_label(text,);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// State
	glDisable(GL_DEPTH_TEST);                         // Disable 3d for User interdata
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
		
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif			
    //-----------------------------------
    // LINE
    // vertex array obj  - stores vertices
    GLuint line_vertex_array_obj;
    glGenVertexArrays(1, &line_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(line_vertex_array_obj); // bind vertex array obj
        GLuint line_vertex_buffer_obj;
        glGenBuffers(1, &line_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, line_vertex_buffer_obj);

		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x) + static_cast<float>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x)                            , static_cast<GLfloat>(y),
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// Draw line
	shader.set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha); //glEnable(GL_LINE_SMOOTH);
	glLineWidth(height); // line thickness (not width, but will act as height since the line is horizontal)
	glBindVertexArray(line_vertex_array_obj);
        glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
    shader.disable();
    // Clean
    // : buffers
    glDeleteBuffers(1, &line_vertex_buffer_obj );
    // : arrays
	glDeleteVertexArrays(1, &line_vertex_array_obj);
	// : shaders
	shader.destroy();
#endif	
}
/////////////
void Renderer::draw_circle()
{/*
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Enable point size mode
	glEnable(GL_PROGRAM_POINT_SIZE);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//=glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	int width = 100; int height = 100; float x = 0.0; float y = 0.0; float angle = 15*(double)(clock()/CLOCKS_PER_SEC);float scale_x = 1.0; float scale_y = 1.0;
	int red = 255; int green = 255; int blue = 100; int alpha = 255;
	// vertex shader	
	Shader vertex_shader;
	vertex_shader.create(DOKUN_SHADER_VERTEX);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	vertex_shader.set_source(vertex_source);
	vertex_shader.compile();
	// fragment shader
	Shader fragment_shader;
	fragment_shader.create(DOKUN_SHADER_FRAGMENT);
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
    const char * circle_fragment_source[] =
	{
		"\n"
		"out vec4 out_color;\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
			"vec2 pos = mod(out_color.xy, vec2(50.0)) - vec2(25.0);"
			"float dist_squared = dot(pos, pos);"
			"out_color = (dist_squared < 400.0)\n"
			    "? vec4(.90, .90, .90, 1.0)\n"
			    ": vec4(.20, .20, .40, 1.0);"
        "}\n"		
	};	
	fragment_shader.set_source(fragment_source);
	fragment_shader.compile();
	// program
	Shader program;
	program.create();
    program.attach(vertex_shader  );
	program.attach(fragment_shader);
	program.link();
	// destroy shaders after linking
	program.detach(vertex_shader  );
	program.detach(fragment_shader);
	vertex_shader.destroy  ();
	fragment_shader.destroy();
	program.use ();
	// uniform
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	shader.set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		
	    float radius = 50.0;
		float circle_points = 100.0;
	    float angle_rad = 2.0 * 3.14159265359 * angle / 180.f;
	    float x_rad = x + cos(angle) * radius;
	    float y_rad = y + sin(angle) * radius;
		for(int i = 0; i < circle_points; i++)
		{
			float theta = 2 * 3.14159265359 * (float)(i) / 100;
			
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + cos(theta) * radius)                                , static_cast<GLfloat>(y + sin(theta) * radius),
            static_cast<GLfloat>(x + cos(theta) * radius) + static_cast<GLfloat>(width), static_cast<GLfloat>(y + sin(theta) * radius),
            static_cast<GLfloat>(x + cos(theta) * radius) + static_cast<GLfloat>(width), static_cast<GLfloat>(y + sin(theta) * radius) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x + cos(theta) * radius)                                , static_cast<GLfloat>(y + sin(theta) * radius) + static_cast<GLfloat>(height),   
        };
		/
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };/      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 2, 2 * circle_points * sizeof(float), vertices);
		}
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLE_FAN, 100, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader.disable();
	shader.destroy();
#endif			*/
}
//////////// //Renderer::draw_triangle(700, 500, 5, 5, 0.0, 1, 1, 255, 255, 255, 255.0);
void Renderer::draw_triangle(float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	// fragment shader
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// program
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
	// destroy shaders after linking
	//program.detach(vertex_shader  );
	//program.detach(fragment_shader);
	//vertex_shader.destroy  ();
	//fragment_shader.destroy();
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	// _
    // vertex array obj  - stores vertices
    GLuint triangle_vertex_array_obj;
    glGenVertexArrays(1, &triangle_vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(triangle_vertex_array_obj); // bind vertex array obj
        GLuint triangle_vertex_buffer_obj;
        glGenBuffers(1, &triangle_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, triangle_vertex_buffer_obj);
		GLfloat verticestri[] = { // when x and y are 0 then from wwidth-wheight
            /*// upside-down triangle
            static_cast<GLfloat>(x)                               , static_cast<GLfloat>(y) + static_cast<GLfloat>(height ),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width ), static_cast<GLfloat>(y) + static_cast<GLfloat>(-height),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(-width), static_cast<GLfloat>(y) + static_cast<GLfloat>(-height),   
            */
            // normal triangle
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(-height),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(-width ), static_cast<GLfloat>(y) + static_cast<GLfloat>(height ),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width)  , static_cast<GLfloat>(y) + static_cast<GLfloat>(height ),            
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticestri)* sizeof(GLfloat), verticestri, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(triangle_vertex_array_obj); // (vao start 1)
	    GLuint triangle_element_buffer_obj;
        glGenBuffers(1, &triangle_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_element_buffer_obj);
		GLuint indices[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
    /*shader->set_integer("outline", outline);
    ////shader->set_float("outline_threshold", outline_threshold);//0.5);
    ////shader->set_float("outline_thickness", outline_thickness);//0.2);
    shader->set_float("outline_color", outline_color.x / 255.0f, outline_color.y / 255.0f, outline_color.z / 255.0f, outline_color.w);*/
	// Draw _
	shader.set_float("color", (red / 255.0), (green / 255.0), (blue / 255.0), alpha);
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(triangle_vertex_array_obj); // (vao start 2)
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 points to form a triangle
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &triangle_element_buffer_obj);
	glDeleteBuffers(1, &triangle_vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &triangle_vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader.disable();
	shader.destroy();
#endif	
}
////////////
void Renderer::draw_quad()
{}
////////////	
void Renderer::draw_cube()
{}
void Renderer::draw_sphere()
{}
void Renderer::draw_torus()
{}
void Renderer::draw_pyramid()
{}
void Renderer::draw_teapot()
{}
////////////
////////////
void Renderer::set_viewport(int x, int y, int width, int height)
{
#ifdef DOKUN_OPENGL
    if(height == 0) 
		height = 1;
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
#endif
#ifdef DOKUN_VULKAN
	VkViewport viewport; // up to 10 viewports
	memset(&viewport, 0, sizeof(viewport));
	viewport.x        =	0;
	viewport.y        =	0;
	viewport.width    = static_cast<float>(width);
	viewport.height   = static_cast<float>(height);
	viewport.minDepth = static_cast<float>(0.0);
	viewport.maxDepth = static_cast<float>(1.0);
	//vkCmdSetViewport(cmd_buf, 0, 1, &viewport);
#endif	
}
////////////
////////////
void Renderer::set_clear(unsigned int red, unsigned int green, unsigned int blue, float alpha, float depth, int stencil)
{
	color = Vector4(red, green, blue, alpha);
#ifdef DOKUN_OPENGL
	context_check();
	glClearColor(static_cast<GLclampf>(color.x) / 255, static_cast<GLclampf>(color.y) / 255, static_cast<GLclampf>(color.z) / 255, static_cast<GLclampf>(color.w) / 255.0); 
    glClearDepth(depth); 
    glClearStencil(stencil);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif
#ifdef DOKUN_VULKAN
	if(Renderer::get_current_API() == "Vulkan")
	{
        /*
		VkClearValue clear_value;
        clear_value.color.float32[0] = static_cast<float>(red);
        clear_value.color.float32[1] = static_cast<float>(green);
        clear_value.color.float32[2] = static_cast<float>(blue);
        clear_value.color.float32[3] = static_cast<float>(alpha); // 1.0f;
		*/
	}
#endif	
}
////////////
void Renderer::set_vertical_synchronization(bool v_sync)
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	wglSwapIntervalEXT(v_sync);
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
	//glXSwapIntervalEXT(display, window, v_sync); // 
#endif
#endif
#endif	
}
////////////
void Renderer::set_display_size(int width, int height) // renderable area (use window_client_size rather than the usual window_size)
{
	Renderer::window_width  = width;
	Renderer::window_height = height;
}
////////////
void Renderer::set_current_API(const std::string& API)
{
	if(dokun::lower(API) == "opengl")
	{
		Renderer::API = "OpenGL";
	}
    if(dokun::lower(API) == "vulkan")
    {
		Renderer::API = "Vulkan";
	}
}
////////////
int Renderer::set_current_API(lua_State *L)
{
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		Renderer::set_current_API(lua_tostring(L, -1));
	}
    return 0;
}
////////////
int Renderer::get_display_width () // returns the width of display area (client area), excluding the titlebar
{
#ifdef __windows__
	RECT rect;
    ::GetClientRect(::GetActiveWindow(), &rect);
    window_width  = rect.right - rect.left;
#endif
    // NOTE: for Linux, the display size is automatically updated in the "Window::update" function
	return window_width;
}
int Renderer::get_display_height() // returns the height of display area (client area), excluding the titlebar
{
#ifdef __windows__
		RECT rect;
        ::GetClientRect(::GetActiveWindow(), &rect);
        window_height = rect.bottom - rect.top;
#endif
    // NOTE: for Linux, the display size is automatically updated in the "Window::update" function
	return window_height;
}
Vector2i Renderer::get_display_size() // returns the size of display area (client area), excluding the titlebar
{
	return Vector2i(get_display_width(), get_display_height());
}
////////////
Renderer * Renderer::get_pointer()
{
	return render_ptr;
}

bool Renderer::get_status()
{
	return status;
}

#ifdef DOKUN_VULKAN
    VkInstance Renderer::get_instance()
    {
	    return get_pointer()->instance;
    }
#endif
//////////////////////
////////////
	// api
std::string Renderer::get_current_API(void)
{
#ifdef DOKUN_OPENGL
    if(String::lower(API) == "opengl") {
	    return "OpenGL";
	}
#endif
#ifdef DOKUN_VULKAN
    if(String::lower(API) == "vulkan") {
	    return "Vulkan";
	}    
#endif
    return "";
}
////////////
std::string Renderer::get_current_API_version(void)
{
    int major, minor, patch;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    glGetIntegerv(GL_MAJOR_VERSION, & major);         
        glGetIntegerv(GL_MINOR_VERSION, & minor);
	    std::pair<int, int> version = std::make_pair (major, minor);		
	    return std::to_string(version.first) + "." + std::to_string(version.second) + ".0";
    }	
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    major = VK_VERSION_MAJOR(Renderer::get_pointer()->device_prop.apiVersion);
	    minor = VK_VERSION_MINOR(Renderer::get_pointer()->device_prop.apiVersion);
	    patch = VK_VERSION_PATCH(Renderer::get_pointer()->device_prop.apiVersion);
	    return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    }	
#endif	
    return "";
}
////////////
int Renderer::get_current_API_version_major(void)
{
    int major;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    glGetIntegerv(GL_MAJOR_VERSION, &major);
        return major;	
    }    
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    major = VK_VERSION_MAJOR(Renderer::get_pointer()->device_prop.apiVersion);
	    return major;
    }	
#endif
    return 0;
}
////////////
int Renderer::get_current_API_version_minor(void)
{
    int minor;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    glGetIntegerv(GL_MINOR_VERSION, &minor);
	    return minor;
    }
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    minor = VK_VERSION_MINOR(Renderer::get_pointer()->device_prop.apiVersion);
	    return minor;
    }
#endif
    return 0;
}
////////////
int Renderer::get_current_API_version_patch(void)
{
    int patch;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    patch = VK_VERSION_PATCH(Renderer::get_pointer()->device_prop.apiVersion);
	    return patch;
	}
#endif
    return 0;
}
////////////
	// shader
std::string Renderer::get_shader_language(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") return "GLSL";
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") return "SPIR-V";
#endif
    return "";
}
////////////
std::string Renderer::get_shader_language_version(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
    }
#endif
    return "";
}
////////////
int Renderer::get_shader_language_version_major(void)
{
    int major;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    std::string shader_version = reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	    std::vector<std::string> major_minor = String::split(shader_version, ".");
	    std::string major_ = major_minor[0];    
	    major = static_cast<int>(String::extract(major_));
	    return major;
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {}
#endif
    return 0;
}
////////////
int Renderer::get_shader_language_version_minor(void)
{
    int minor;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    std::string shader_version = reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	    std::vector<std::string> major_minor = String::split(shader_version, ".");
	    std::string minor_ = major_minor[1];
	    if(minor_.length() > 1) 
		    minor_ = minor_.at( minor_.find_first_of(minor_));
	    minor = static_cast<int>(String::extract(minor_));
	    return minor;
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {}
#endif
    return 0;
}
////////////
int Renderer::get_shader_language_version_patch(void)
{
    int patch;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    std::string shader_version = reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	    std::vector<std::string> major_minor = String::split(shader_version, ".");
	    std::string patch_ = major_minor[1];
	    patch_ = patch_.at(patch_.find_last_of(patch_));
	    patch = static_cast<int>(String::extract(patch_));
	    return patch;
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {}
#endif
    return 0;
}
////////////
	// gpu
std::string Renderer::get_gpu(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_RENDERER)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check();
	    return Renderer::get_pointer()->device_prop.deviceName;
	}
#endif
    return "";
}
////////////
std::string Renderer::get_gpu_vendor(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_VENDOR)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check();
        //return Renderer::get_pointer()->device_prop.vendorID; // uint32_t??
    }
#endif
    return "";
}
////////////
std::string Renderer::get_gpu_version(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_VERSION)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check();
    }
#endif
    return "";
}
////////////
void Renderer::print_vulkan_info()
{
#ifdef DOKUN_VULKAN
	if(get_current_API() == "Vulkan")
	{
		// look for devices and gpus that support vulkan
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, & extension_count, nullptr);

        std::cout << extension_count << " extensions supported" << std::endl;	
		
		VkDevice device;
		VkPhysicalDevice physical_device;
		uint32_t physical_device_count = 0;
		std::vector<VkPhysicalDevice> physical_device_list( physical_device_count);
		// check how many devices we have
		VkResult result = vkEnumeratePhysicalDevices(get_pointer()->instance, & physical_device_count, & physical_device);
		assert(result == VK_SUCCESS);
		std::cout << physical_device_count << " devices found" << std::endl;
	    // device properties
		VkPhysicalDeviceProperties device_prop = {};
		vkGetPhysicalDeviceProperties(physical_device, & device_prop);   // for single device

		// API information
		std::string API_name = "Vulkan";
		int major = VK_VERSION_MAJOR(device_prop.apiVersion);
		int minor = VK_VERSION_MINOR(device_prop.apiVersion);
		int patch = VK_VERSION_PATCH(device_prop.apiVersion);
		
		std::string shader_lang = "GLSL";
		std::string shader_version = "??";
		// device information
			std::cout << "GPU: " << device_prop.deviceName << std::endl; 
			std::cout << "Vendor: " << device_prop.vendorID << std::endl; 
			std::cout << "Device : " << device_prop.deviceID << std::endl;
            std::cout << "Type : " << device_prop.deviceType << std::endl;
			std::cout << "Driver version: " << device_prop.driverVersion << std::endl; 
			std::cout << "API: " << API_name << std::endl; 
			std::cout << "API version ID : " << device_prop.apiVersion << std::endl;
			std::cout << "API Version: " << major << "." << minor << "." << patch << std::endl; 
			std::cout << "Shader: " << shader_lang << std::endl;
            std::cout << "Shader Version: " << shader_version << std::endl;				
	}
#endif
}	
////////////
////////////
