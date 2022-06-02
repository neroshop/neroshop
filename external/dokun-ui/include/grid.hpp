#ifndef GRID_HPP_DOKUN
#define GRID_HPP_DOKUN

#include "ui.hpp"
#include "box.hpp"
class Grid : public GUI
{
public:
    Grid();                               static int grid_new(lua_State *L);
    Grid(int row, int column);
    ~Grid();
    // normal functions
    void draw();                               static int draw(lua_State *L);
    void draw(double x, double y);
    void draw(const Vector2& position);	
    void update();                             static int update(lua_State *L);
    // setters
    void set_rows(int rows);                    static int set_rows(lua_State *L);
    void set_columns(int columns);              static int set_columns(lua_State *L);
    void set_gap(int gap);
    void set_horizontal_gap(int gap_horz);
    void set_vertical_gap(int gap_vert);
    void set_color(unsigned int red, unsigned int green, unsigned int blue);
    void set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
    void set_color(const Vector3& color);
    void set_color(const Vector3& color, double alpha);
    void set_color(const Vector4& color);	   static int set_color(lua_State *L);
    void set_highlight(bool highlight);
    void set_highlight_color(unsigned int red, unsigned int green, unsigned int blue);
    void set_highlight_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
    void set_highlight_color(const Vector3& color);
    void set_highlight_color(const Vector3& color, double alpha);
    void set_highlight_color(const Vector4& color);
    void set_outline(bool outline);
    void set_outline_color(unsigned int red, unsigned int green, unsigned int blue);
    void set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
    void set_outline_color(const Vector3& color);
    void set_outline_color(const Vector3& color, double alpha);
    void set_outline_color(const Vector4& color);
    void set_radius(float radius);
    // getters
    Box * get_block(int row, int column) const;           static int get_block(lua_State *L);
    Box * get_box(int row, int column) const;
    Box * get_box(int index) const; // for 1D index positions of grid boxes
    std::vector<Box *> get_box_list_1d() const;
    std::vector<std::vector<Box *>> get_box_list_2d() const;
    int get_row_count()const;                     static int get_row_count(lua_State *L);
    int get_column_count()const;                  static int get_column_count(lua_State *L);
    int get_box_count() const;
    int get_full_width() const;                  static int get_full_width(lua_State *L);
    int get_full_height() const;                 static int get_full_height(lua_State *L);
    int get_horizontal_gap() const;
    int get_vertical_gap() const;
    Vector2 get_full_size() const;               static int get_full_size(lua_State *L);		
    Vector4 get_color()const;                     static int get_color(lua_State *L); //static int _(lua_State *L);
    float get_radius() const;
    // boolean
    bool is_empty() const; // returns true if all grid boxes contain zero content (meaning no guis nor images) otherwise returns false        
private:
    // callbacks
    void on_highlight(int rows, int columns); // rows come first before columns
    std::vector<std::vector<std::shared_ptr<Box>>> box_list;
    int rows, columns;
    int gap_horz; // the horizontal gap between blocks
    int gap_vert; // the vertical gap between blocks
    Vector4 color;
    // outline
    bool outline;
    double outline_width;
    Vector4 outline_color;
    bool outline_antialiased;	
    // radius
    float radius;
    // highlight
    bool highlight;
    Vector4 highlight_color;
    // gradient
    bool gradient;
    Vector4 gradient_color;	
};
#endif
/*
USAGE:
    Grid * grid = new Grid();
	grid->set_row(2);
	grid->set_column(2);
	grid->set_position(100, 400);
	grid->get_box(0, 0)->set_color(255, 51, 51);
	std::cout << "Grid: " << grid->get_row_count() << " x " << grid->get_column_count() << std::endl;
*/
