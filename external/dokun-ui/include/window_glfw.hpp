#ifndef WINDOW_GLFW_HPP_DOKUN
#define WINDOW_GLFW_HPP_DOKUN

namespace dokun {
class Window {
public:
    Window();
    ~Window();
    // normal functions
    void create();
        void create (const std::string& title);    
        void create(int width, int height, int mode = 0);    
        void create(const std::string&, int width, int height, int mode = 0);   
		void create(int x, int y, int left, int top, int mode = 0);
		void create(const std::string&, int x, int y, int left, int top, int mode = 0);	
        void show();
        void hide();
		void update();
        void destroy ();
	    void clear(double red = 0.0, double green = 0.0, double blue = 0.0, double alpha = 1.0, double depth = 1.0, int stencil = 0);
        void iconify();
		void maximize();
	    void restore();// NOTE: when using the loop function, you must use window callback functions//void loop();
		// setters
        void set_title(const std::string& title);
        void set_width (int width);
        void set_height (int height);
        void set_size (int width, int height);
        void set_mode(int mode);
	    void set_position(int x, int y);
		// appearance and looks
		//void set_style(long style);
		void set_icon(const std::string& file_name);
        void set_cursor(int cursor);
        void set_cursor(std::string file_name, int width = 0, int height = 0);		
		// vertical synchronization
		void set_vertical_synchronization(bool);
		static void set_viewport(int width, int height);
		// event processing : peek_event or wait_event
        //void set_event(const Event& event);	          static int set_event(lua_State *L); // 0 = peek_msg(returns message, returns nothing if no messages), 1 = get_msg(waits for message) 
		// context creation : for drawing
        void set_context();
        void set_surface();
		// getters
		std::string get_name();
		int get_id();
        std::string get_title()const;
        int get_width()const;
        int get_height()const;
		Vector2 get_size()const;
        int get_client_width()const;
        int get_client_height()const;
		Vector2 get_client_size()const;
		
		int get_mode()const;
		Vector2 get_position()const;
		int get_x()const;
		int get_y()const;
		static dokun::Window * get_active();
		// boolean
		bool is_open()const;                           static int is_open(lua_State *L);
		bool is_visible()const;                        static int is_visible(lua_State *L);
		bool is_focused()const;                        static int is_focused(lua_State *L);
		bool is_iconified()const;                      static int is_iconified(lua_State *L);
		bool is_maximized()const;
		bool is_window()const;
		bool is_context()const;    
    // setters
    // getters
    GLFWwindow* get_handle() const;
private:
    GLFWwindow* handle;    
};
}
#endif
