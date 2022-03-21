#include "../include/grid.hpp"

Grid::Grid() : rows(0), columns(0), gap_horz(0), gap_vert(0), color(64, 64, 64, 1.0),
// outline
outline(true), 
outline_width(2.0), 
outline_antialiased(false),
outline_color(255, 255, 255, 1.0),
// highlight
highlight(false),
highlight_color(0, 51, 102, 1.0),
// gradient
gradient(false),
gradient_color(color)
{
	set_position(0, 0);
	set_size(50, 50); // size of each individual block_list in grid
	set_orientation(0);
}
/////////////////////
Grid::Grid(int rows, int columns) : Grid()
{
	set_columns(columns);
	set_rows(rows);
}
/////////////////////
int Grid::grid_new(lua_State *L)
{
	lua_settop(L, 0); // clear stack	
	lua_createtable(L, 0, 0);      // create table
	lua_getglobal(L, "Grid");
	lua_setmetatable(L, 1);        // set metatable
	Grid ** grid = static_cast<Grid **>(lua_newuserdata(L, sizeof(Grid*))); // set userdata
	*grid = new Grid;
	lua_setfield(L, 1, "udata");
	return 1; 	// return the newly created table
}
/////////////////////
Grid::~Grid()
{
    ////block_list.reset();
    block_list.clear(); // clear rows
    std::cout << "grid deleted\n";
}
////////////////////
////////////////////
void Grid::draw() // COMPLETE! :D
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(!is_visible()) return;
	if(block_list.empty()) return; // 0 rows
	for(int i = 0; i < block_list.size(); i++) // block_list.size() = rows
	{
		for(int j = 0; j < block_list[i].size(); j++) // block_list[i] = items in block_list i
		{		
                on_highlight(i, j);
				// Set geometry
				block_list[i][j]->set_position(get_position().x + j * (get_width() + gap_horz), get_position().y + i * (get_height() + gap_vert));
                // Draw grid
				block_list[i][j]->set_width(get_width());
				block_list[i][j]->set_height(get_height());
				block_list[i][j]->set_angle(get_angle());
				block_list[i][j]->set_scale(get_scale());
				block_list[i][j]->set_color(block_list[i][j]->get_color()); // already the default
				block_list[i][j]->set_radius(0.0);
				block_list[i][j]->set_title_bar(false); // grids should NEVER have a title_bar!
				block_list[i][j]->set_outline(outline); // outline is now the same as border
				block_list[i][j]->set_outline_color(outline_color);
				block_list[i][j]->set_outline_width(outline_width);//block_list[i][j]->set_outline_(outline_);
				//block_list[i][j]->set_gradient(block_list[i][j]->gradient);
				//block_list[i][j]->set_gradient_color(block_list[i][j]->gradient_color);
				//block_list[i][j]->set_gradient_value(block_list[i][j]->gradient_value);
				//block_list[i][j]->set_shadow(false);
				// Image properties
				//block_list[i][j]->set_();
				/*if(block_list[i][j]->get_image(0)) {
				    block_list[i][j]->get_image(0)->set_position(block_list[i][j]->get_x(), block_list[i][j]->get_y());
				    block_list[i][j]->get_image(0)->set_scale(block_list[i][j]->get_image(0)->get_aspect_ratio_correction(get_width(), get_height()),
					    block_list[i][j]->get_image(0)->get_aspect_ratio_correction(get_width(), get_height()));	// equivalent to Image::scale_to_fit
				}*/
				// block_list should set image relative position based on image's alignment
				block_list[i][j]->set_position(block_list[i][j]->get_position().x, block_list[i][j]->get_position().y);
				block_list[i][j]->draw(); // each block_list can have its own label, image, etc.
		}
	}
} // nothing will be drawn if rows and columns are not set
////////////////////
void Grid::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
////////////////////
void Grid::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
////////////////////
int Grid::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->draw();
	}
    return 0;
}
////////////////////
void Grid::update() // crashes when called inside a loop
{
    /*for(int r = 0; r < block_list.size(); r++) {
        for(int c = 0; c < block_list[r].size(); c++) {
            //block_list[r].clear(); // clear columns
            ////std::cout << "row size: " << block_list.size() << "\n";
            ////std::cout << "column size: " << block_list[r].size() << "\n"; // this also causes a crash
            ////block_list[r].erase(block_list[r].begin() + r);
            ////std::cout << "use count: " << block_list[r][c].use_count() << "\n";
            ////if(block_list[r][c].use_count() > 0) {
                ////////block_list[r][c].reset(); // delete all columns // causes "Segmentation fault (core dumped)" error
                //block_list[r][c] = nullptr;
            ////}
            ////std::cout << "use count: " << block_list[r][c].use_count() << "\n";
        }
        ////block_list.erase(block_list.begin() + r);
    }*/
    ////////block_list.resize(0); // causes "double free or corruption (fasttop)" error
    ////////////////////////////
	if(!block_list.empty()) block_list.clear(); // clear rows // causes "free(): invalid pointer" error
	for(int i = 0; i < rows; i++)
	{
		// create empty rows
		block_list.push_back(std::vector<std::shared_ptr<Box>>()); // empty vector of boxes
		// store row items based on # of columns
		for(int j = 0; j < columns; j++) {
			block_list[i].push_back(std::make_shared<Box>());
		}
	}
}
int Grid::update(lua_State *L)
{
    return 0;
}
////////////////////
////////////////////
void Grid::set_rows(int rows)
{
	this->rows = rows;
	update(); // update block_list each time row is changed
}            
////////////////////  
int Grid::set_rows(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_rows(lua_tonumber(L, 2));
	}
    return 0;	
}
////////////////////
void Grid::set_columns(int columns)
{
	this->columns = columns;
	update(); // update block_list each time column is changed
}          
////////////////////    
int Grid::set_columns(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_columns(lua_tonumber(L, 2));
	}
    return 0;	
}
////////////////////
void Grid::set_gap(int gap) {
    set_horizontal_gap(gap);
    set_vertical_gap(gap);
}
////////////////////
void Grid::set_horizontal_gap(int gap_horz) {
    this->gap_horz = gap_horz;
}
////////////////////
void Grid::set_vertical_gap(int gap_vert) {
    this->gap_vert = gap_vert;
}
////////////////////
void Grid::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
	// apply color to all blocks or blocks should have their own colors?
}
////////////////////
void Grid::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
////////////////////
void Grid::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
////////////////////
int Grid::set_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	std::cout << "alpha "<< lua_tonumber(L, 5) << "\n";
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
    return 0;	
}
////////////////////
void Grid::set_highlight(bool highlight) {
    this->highlight = highlight;
}
////////////////////
void Grid::set_highlight_color(unsigned int red, unsigned int green, unsigned int blue) {
    highlight_color = Vector4(red, green, blue, highlight_color.w);
}
////////////////////
void Grid::set_highlight_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    highlight_color = Vector4(red, green, blue, alpha);
}
////////////////////
void Grid::set_highlight_color(const Vector3& color) {
    set_highlight_color(color.x, color.y, color.z);
}
////////////////////
void Grid::set_highlight_color(const Vector4& color) {
    set_highlight_color(color.x, color.y, color.z, color.w);
}
////////////////////
void Grid::set_outline(bool outline) {
    this->outline = outline;
}
////////////////////
void Grid::set_outline_color(unsigned int red, unsigned int green, unsigned int blue) {
    outline_color = Vector4(red, green, blue, outline_color.w);
}
////////////////////
void Grid::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
    outline_color = Vector4(red, green, blue, alpha);
}
////////////////////
void Grid::set_outline_color(const Vector3& color) {
    set_outline_color(color.x, color.y, color.z);
}
////////////////////
void Grid::set_outline_color(const Vector4& color) {
    set_outline_color(color.x, color.y, color.z, color.w);
}
////////////////////
////////////////////
Box * Grid::get_block(int row, int column) const
{
    if(block_list.empty()) return nullptr; // if no rows, return nullptr
	if(block_list.size() < row + 1) throw std::runtime_error("Attempt to access invalid location | grid.cpp (114)");
	return block_list[row][column].get();
}              
////////////////////
int Grid::get_block(lua_State *L)
{
    return 1;
}
////////////////////
std::vector<std::vector<std::shared_ptr<Box>>> Grid::get_block_list() const {
    return block_list;
}
////////////////////
Box * Grid::get_box(int row, int column) const {
    return get_block(row, column);
}
////////////////////
Box * Grid::get_box(int index) const {
    std::vector<Box *> box_1d = {};
    for(int r = 0; r < this->rows/*or block_list.size()*/; r++) // block.size() = rows
	{
		for(int c = 0; c < this->columns/*or block_list[r].size()*/; c++) { // block[r] = items in row r	
            box_1d.push_back( block_list[r][c].get() );  
        }
    }
    //std::cout << "box_1d size: " << box_1d.size() << std::endl;
    return box_1d[index];
}
////////////////////
std::vector<std::vector<std::shared_ptr<Box>>> Grid::get_box_list() const {
    return get_block_list();
}
////////////////////
int Grid::get_row_count()const
{
	return rows;
}
////////////////////
int Grid::get_row_count(lua_State *L)
{
    return 1;
}
////////////////////
int Grid::get_column_count()const
{
	return columns;
}
////////////////////
int Grid::get_column_count(lua_State *L)
{
    return 1;
}
////////////////////
int Grid::get_box_count() const {
    return get_row_count() * get_column_count();
}
////////////////////
int Grid::get_full_width() const {
    return columns * (get_width() + gap_horz);
}
////////////////////
int Grid::get_full_height() const {
    return rows * (get_height() + gap_vert);
}
////////////////////
Vector2 Grid::get_full_size() const {
    return Vector2(get_full_width(), get_full_height());
}
////////////////////
Vector4 Grid::get_color()const
{
	return color;
}              
////////////////////
int Grid::get_color(lua_State *L)
{
    return 4;
}
/*
	std::cout << "rows " << block_list.size() << "\n"; 
	for(int k = 0; k < block_list.size(); k++)
	    std::cout << "row items in block_list [" << k << "] " << block_list[k].size() << "\n";
*/
////////////////////
void Grid::on_highlight(int rows, int cols) {
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	///////////////////
	if(!highlight) return;
	// on hover
    if(Mouse::is_over(block_list[rows][cols]->get_position().x + cols, block_list[rows][cols]->get_position().y + rows, get_width(), get_height()))
		block_list[rows][cols]->set_color(highlight_color);
	else block_list[rows][cols]->set_color(get_color());
}
