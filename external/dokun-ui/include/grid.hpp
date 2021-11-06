#ifndef _GRID
#define _GRID

#include "ui.hpp"
#include "box.hpp"
class Grid : public GUI
{
public:
    Grid();                               static int grid_new(lua_State *L);
    Grid(int row, int column);
	~Grid();
	
	void draw();                               static int draw(lua_State *L);
	void draw(double x, double y);
	void draw(const Vector2& position);	
	void update();                             static int update(lua_State *L);
	
	void set_row(int rows);                    static int set_row(lua_State *L);
	void set_column(int columns);              static int set_column(lua_State *L);
	void set_block(int rows, int columns);     static int set_block(lua_State *L);
	void set_color(int red, int green, int blue, int alpha = 225);
	void set_color(const Vector3& color);
	void set_color(const Vector4& color);	   static int set_color(lua_State *L);
	
	Box * get_block(int row, int column);           static int get_block(lua_State *L);
	int get_row_count()const;                     static int get_row_count(lua_State *L);
	int get_column_count()const;                  static int get_column_count(lua_State *L);	
	Vector4 get_color()const;                     static int get_color(lua_State *L); //static int _(lua_State *L);
private:
	std::vector< std::vector<Box *> > block;
    int rows, columns;
    std::vector<Box *> trash;	
	Vector4 color;
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;	
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
	grid->get_block(0, 0)->set_color(255, 51, 51);
	std::cout << "Grid: " << grid->get_row_count() << " x " << grid->get_column_count() << std::endl;
*/
