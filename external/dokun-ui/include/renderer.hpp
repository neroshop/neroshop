#ifndef RENDERER_HPP_DOKUN
#define RENDERER_HPP_DOKUN

#include "platform.hpp"
#include "vector.hpp"
//#include "matrix_test.hpp"//#include "matrix.hpp" // will replace glm once completed
#include "shader.hpp"
#include "texture.hpp"
#include "font.hpp"
//------------------
#ifdef __cplusplus // if c++
#include <iostream> // March 1, 2016     12:21 PM
#include <string>
#include <vector>

#define use_glm /* remove comment to enable glm*/
#ifdef use_glm
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif


class Renderer // draws object to screen; NOTE: The renderer does not hold any kind of data values or information(except GPU info) but takes information from an object and uses that information to draw the object
{
public:	
	Renderer();
	~Renderer();
	// --------------------------
	static void start   (void); // initializes renderer
	static void destroy (void); // destroy renderer and all objects
	// entity -------------------
	/*static void draw_sprite(const Texture& texture, float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha,// = 1.0,
	    const std::vector<float>& vertex_array, const Shader& shader, const std::vector<Texture *>& map);*/ // fast (uses 2-7% CPU)
	//static void draw_atlas(const Texture& texture, int width, int height, float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, int frame, const Shader& shader);
	/*static void draw_model (const std::vector<Vector3>& vertex_array, const std::vector<unsigned int>& element_array, const std::vector<Vector2>& uv_array, const std::vector<Vector3>& normal_array, float x, float y, float z, double rx, double ry, double rz, double sx, double sy, double sz, 
		double ox, double oy, double oz, 
		const std::vector<Texture *>& texture_array, const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector4& emission, double shininess, const Shader& shader);*/
	static void draw_image(const unsigned int buffer/*Texture& texture*/, int width, int height, int depth, float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, int channel, Shader* shader,
	    bool outline, float outline_thickness, const Vector4& outline_color, float outline_threshold); // complete!
    // ui -----------------------
	static void draw_box(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader, 
		float radius, bool iconified,
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
	);
	//--------------------------
	static void draw_quad_instanced(int x, int y, int width, int height);
	//--------------------------
	static void draw_text_old (const std::string& text, float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, const dokun::Font& font, Shader* shader);
	static void draw_glyph (unsigned char glyph, float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, const dokun::Font& font, Shader* shader);
	static void draw_text (const std::string& text, float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, const dokun::Font& font, Shader* shader);
	//--------------------------
	static void draw_button(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader, 
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
	);
	//--------------------------
	static void draw_progressbar(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader, 
		double min_value, double max_value, double value, const Vector4& background_color,
		// outline
		bool outline, 
		float outline_width, 
		const Vector4& outline_color,
		bool outline_antialiased,
		// border
		//bool border,
		//const Vector4& border_color,
		// radius
		float radius		
		);
	//--------------------------
	static void draw_edit(const std::string& text, int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    bool multilined,
		// cursor
		bool cursor, double cursor_x, double cursor_y, int cursor_width, int cursor_height, const Vector4& cursor_color
	);
	static void draw_cursor(double cursor_x, double cursor_y, int cursor_width, int cursor_height, const Vector4& cursor_color, Shader* shader,
	    int parent_x, int parent_y, int parent_width, int parent_height);
	//--------------------------
	static void draw_slider(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,  // good!
        // beam
		double min_value, double max_value, double value, const Vector4& background_color, 
		// radius (for beam)
		float radius, 
		// ball
		int ball_width, const Vector4& ball_color, float ball_radius
	);
    static void draw_slider_vertical(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
    // beam
	double min_value, double max_value, double value, const Vector4& background_color,
	// ball
	int ball_height, const Vector4& ball_color);
	//--------------------------
	static void draw_switch(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader, // good!
		int value,
		const Vector4& background_color,
		// border
		bool outline,
		float outline_width,
		const Vector4& outline_color,
		bool outline_antialiased,
		// radius
		float radius		
	);
	//--------------------------
	static void draw_radio(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    int value, const Vector4& inner_color,
		// outline
		bool outline,
		float outline_width,
		const Vector4& outline_color,
		bool outline_antialiased,
		// radius
		float radius
	);
	//--------------------------
	static void draw_checkbox(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    bool value, const Vector4& checkmark_color,
		// outline
		bool outline,
		float outline_width,
		const Vector4& outline_color,
		bool outline_antialiased,
		// radius
		float radius
	);
	//--------------------------
	static void draw_tooltip(const std::string& text, int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha,
	    // shader
	    Shader* shader,
	    // outline-border
	    bool outline, const Vector4& outline_color,
	    // radius
	    float radius, 
	    // arrow - placement("up", "down"=default, "left", "right")
	    std::string placement = "down", int arrow_width = 10, int arrow_height = 5, double arrow_offset = -1.0);
	//--------------------------
	static void draw_scrollbar(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
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
		bool border
	);
	//--------------------------
	static void draw_spinner0(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    double value,
		// button
        int button_width, const Vector4& button_color, 		
        // shape (arrows)
        const Vector2& arrow_size, const Vector4& arrow_color,
		// border
		bool outline,
		float outline_width,
		const Vector4& outline_color,
		bool outline_antialiased);
	//--------------------------
    static void draw_spinner(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    // button
        int button_width, const Vector4& button_color,
        // shape (+, -)
	    int shape_size, const Vector4& shape_color, double shape_depth,
	    // separator (gap)
	    bool separator, int separator_size); // added 2021-12-17		
	//--------------------------
	static void draw_combobox(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    const Vector4& button_color, int button_width, bool button_on);
	//--------------------------
	static void draw_tab(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha, Shader* shader,
	    // tab
	    int tab_count = 1, 
	    // tab body
	    bool visible = true
	);
	//--------------------------
	//use quad_ui for this - static void draw_menubar(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//use quad_ui for this - static void draw_grid(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	//static void draw_(int x, int y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha);
	// levels and maps ---------
	static void draw_level ();
	static void draw_sky ();
	// 2d primitives -----------
	static void draw_point   ();
	static void draw_line    (float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha = 1.0);
	static void draw_circle  ();
	static void draw_triangle(float x, float y, int width, int height, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha = 1.0);
	static void draw_quad    ();
	static void draw_polygon ();
	// 3d primitives -----------
	static void draw_cube    ();
	static void draw_sphere  ();
	static void draw_torus   ();
	static void draw_cylinder();
	static void draw_pyramid ();
	static void draw_teapot  ();
	//--------------------------
	// ...
	void display(); // shows rendering information
	// setters
	static void set_viewport(int x, int y, int width, int height);
	static void set_clear(unsigned int red, unsigned int green, unsigned int blue, float alpha = 1.0, float depth = 1.0, int stencil = 0);
	static void set_vertical_synchronization(bool v_sync);
	static void set_current_API(const std::string& API); static int set_current_API(lua_State *L);
	static void set_background_alpha(bool bg_alpha); // 0 = transparent 1 = visible
	// getters
	void print_vulkan_info();
	void get_GPU_info();	
	static Renderer * get_pointer();
	static bool get_status();
	static int get_display_width ();
	static int get_display_height();
	static Vector2i get_display_size();
    /////////////////////
	// API
	static std::string get_current_API(void);
	static std::string get_current_API_version(void);
	static int get_current_API_version_major(void);
	static int get_current_API_version_minor(void);
	static int get_current_API_version_patch(void);
	// shader_language
	static std::string get_shader_language(void);
	static std::string get_shader_language_version(void);
	static int get_shader_language_version_major(void);
	static int get_shader_language_version_minor(void);
	static int get_shader_language_version_patch(void);
	static bool is_supported_extension(const char *extList, const char *extension);
	// GPU
	static std::string get_gpu(void);
	static std::string get_gpu_vendor(void);
	static std::string get_gpu_version(void);
	/////////////////////
	#ifdef DOKUN_VULKAN
	    static VkInstance get_instance();
	#endif
	// screen dimensions
	static unsigned int window_width ;
	static unsigned int window_height;	
	static void set_display_size(int width, int height);
    // Test
    static void draw_sprite_test(float x, float y, float angle, float scale_x, float scale_y, unsigned int red, unsigned int green, unsigned int blue, float alpha,
    const std::vector<float>& vertex_array, const Shader& shader, const Texture& texture);
    /////////////////////////////////////////
    static void context_check(void);
	static void device_check (void); // NEW! added 9-3-2018
    /////////////////////////////////////////
 	static void generate_vertex_array(float x, float y, unsigned int width, unsigned int height);
	static void update_vertex_array(const std::vector<float>& vertex_array);
	static void destroy_vertex_array (void);
private:
	/////////////////////////////////////////
    static Renderer * render_ptr; // default renderer
	static bool status;
	static std::string API;
	static Vector4 color;
	///////////////OPENGL OBJECTS///////////////////////////////
	static unsigned int sprite_vertex_array_obj;
	static unsigned int sprite_vertex_buffer_obj;
	static unsigned int sprite_texcoord_buffer_obj;
	static unsigned int sprite_element_buffer_obj;
	/////////////////////////////////////////////
	static unsigned int model_vertex_array_obj;
	//////////// SHADER PROGRAM/////////////////////////
    Shader * sprite_shader;
	Shader * model_shader;
	/// gpu data ////////////////////
	struct gpu
	{
		unsigned int count;
		std::string name;
		std::string vendor;
		std::string API_name;
		std::tuple<int, int, int> API_version;
		std::string API_version_str;
		unsigned int API_version_major;
		unsigned int API_version_minor;
		unsigned int API_version_patch;
		std::string shader_version;
		std::string shader_lang;
		unsigned int ext_count;
		std::vector<std::string> ext_list;
	} gpu;	
public:	
#ifdef DOKUN_VULKAN
	VkApplicationInfo app_info;
	VkInstanceCreateInfo instance_info;
	VkInstance instance;
	VkViewport viewport;
	VkPhysicalDeviceProperties device_prop;
#endif
};
#define DOKUN_CURRENT_API_OPENGL (Renderer::get_current_API() == "OpenGL")
#define DOKUN_CURRENT_API_VULKAN (Renderer::get_current_API() == "Vulkan")
#endif
#endif
