#include "../include/grid.hpp"

Grid::Grid() : rows(0), columns(0), gap_horz(0), gap_vert(0), color(64, 64, 64, 1.0),
// radius
radius(0.0),
// outline
outline(false), 
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
	set_size(50, 50); // size of each individual box_list in grid
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
    // "box_list.clear()" will cause a "double free or corruption (fasttop)" error so we must check if objects are not null then delete them individually
    for(auto rows_ : box_list) {
        for(auto boxes : rows_) {
            if(boxes.get()) {
                boxes.reset();
                if(!boxes.get()) std::cout << "grid box deleted" << std::endl; // confirm deletion
            }
        }
    }
    std::cout << "grid deleted\n";
}
////////////////////
////////////////////
void Grid::draw() // COMPLETE! :D
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(!is_visible()) return;
	if(box_list.empty()) return; // 0 rows
	for(int i = 0; i < box_list.size(); i++) // box_list.size() = rows
	{
		for(int j = 0; j < box_list[i].size(); j++) // box_list[i] = items in box_list i
		{		
                on_highlight(i, j);
				// Set geometry
				box_list[i][j]->set_position(get_position().x + j * (get_width() + gap_horz), get_position().y + i * (get_height() + gap_vert));
                // Draw grid
				box_list[i][j]->set_width(get_width());
				box_list[i][j]->set_height(get_height());
				box_list[i][j]->set_angle(get_angle());
				box_list[i][j]->set_scale(get_scale());
				box_list[i][j]->set_color(box_list[i][j]->get_color()); // already the default
				box_list[i][j]->set_radius(radius);
				box_list[i][j]->set_title_bar(false); // grids should NEVER have a title_bar!
				box_list[i][j]->set_outline(outline); // outline is now the same as border
				box_list[i][j]->set_outline_color(outline_color);
				box_list[i][j]->set_outline_width(outline_width);//box_list[i][j]->set_outline_(outline_);
				//box_list[i][j]->set_gradient(box_list[i][j]->gradient);
				//box_list[i][j]->set_gradient_color(box_list[i][j]->gradient_color);
				//box_list[i][j]->set_gradient_value(box_list[i][j]->gradient_value);
				//box_list[i][j]->set_shadow(false);
				// Image properties
				//box_list[i][j]->set_();
				/*if(box_list[i][j]->get_image(0)) {
				    box_list[i][j]->get_image(0)->set_position(box_list[i][j]->get_x(), box_list[i][j]->get_y());
				    box_list[i][j]->get_image(0)->set_scale(box_list[i][j]->get_image(0)->get_aspect_ratio_correction(get_width(), get_height()),
					    box_list[i][j]->get_image(0)->get_aspect_ratio_correction(get_width(), get_height()));	// equivalent to Image::scale_to_fit
				}*/
				// box_list should set image relative position based on image's alignment
				box_list[i][j]->set_position(box_list[i][j]->get_position().x, box_list[i][j]->get_position().y);
				box_list[i][j]->draw(); // each box_list can have its own label, image, etc.
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
    /*for(int r = 0; r < box_list.size(); r++) {
        for(int c = 0; c < box_list[r].size(); c++) {
            //box_list[r].clear(); // clear columns
            ////std::cout << "row size: " << box_list.size() << "\n";
            ////std::cout << "column size: " << box_list[r].size() << "\n"; // this also causes a crash
            ////box_list[r].erase(box_list[r].begin() + r);
            ////std::cout << "use count: " << box_list[r][c].use_count() << "\n";
            ////if(box_list[r][c].use_count() > 0) {
                ////////box_list[r][c].reset(); // delete all columns // causes "Segmentation fault (core dumped)" error
                //box_list[r][c] = nullptr;
            ////}
            ////std::cout << "use count: " << box_list[r][c].use_count() << "\n";
        }
        ////box_list.erase(box_list.begin() + r);
    }*/
    ////////box_list.resize(0); // causes "double free or corruption (fasttop)" error
    ////////////////////////////
	if(!box_list.empty()) box_list.clear(); // clear rows // causes "free(): invalid pointer" error
	for(int i = 0; i < rows; i++)
	{
		// create empty rows
		box_list.push_back(std::vector<std::shared_ptr<Box>>()); // empty vector of boxes
		// store row items based on # of columns
		for(int j = 0; j < columns; j++) {
			box_list[i].push_back(std::make_shared<Box>());//(std::unique_ptr<Box>(new Box()));
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
	update(); // update box_list each time row is changed
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
	update(); // update box_list each time column is changed
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
void Grid::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
////////////////////
void Grid::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
}
////////////////////
void Grid::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
////////////////////
void Grid::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
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
void Grid::set_radius(float radius) {
    this->radius = radius;
}
////////////////////
////////////////////
Box * Grid::get_block(int row, int column) const
{
    return get_box(row, column);
}              
////////////////////
int Grid::get_block(lua_State *L)
{
    return 1;
}
////////////////////
Box * Grid::get_box(int row, int column) const {
	if(box_list.size() < row + 1) throw std::out_of_range("Grid::get_box: invalid or out of range row index"); // or if(row > (box_list.size() - 1))
	if(box_list[row].size() < column + 1) throw std::out_of_range("Grid::get_box: invalid or out of range column index"); // or if(column > (box_list[row].size() - 1))
	return box_list[row][column].get();
}
////////////////////
Box * Grid::get_box(int index) const {
    std::vector<Box *> box_list_1d = {};
    for(auto r : box_list) {//for(int r = 0; r < this->rows/*or box_list.size()*/; r++) { // block.size() = rows
		for(auto c : r) {//for(int c = 0; c < this->columns/*or box_list[r].size()*/; c++) { // block[r] = items in row r	
            box_list_1d.push_back(c.get());//(box_list[r][c].get());  
        }
    }
    if(index > (box_list_1d.size() - 1)) throw std::out_of_range("Grid::get_box: invalid or out of range index");
    return box_list_1d[index];
}
////////////////////
std::vector<Box *> Grid::get_box_list_1d() const {
    std::vector<Box *> box_list_1d = {};
    for(auto r : box_list)//for(int r = 0; r < box_list.size(); r++) 
	{
		for(auto c : r) {//for(int c = 0; c < box_list[r].size(); c++) {
            box_list_1d.push_back(c.get());//box_list_1d.push_back(box_list[r][c].get());
        }
    }
    return box_list_1d;
}
////////////////////
std::vector<std::vector<Box *>> Grid::get_box_list_2d() const { // not tested yet
    std::vector<std::vector<Box *>> box_list_2d = {};
    for(auto r : box_list)//for(int r = 0; r < box_list.size(); r++) {
    {
        box_list_2d.push_back(std::vector<Box *>()); // empty rows
        for(auto c : r) {//for(int c = 0; c < box_list[r].size(); c++) {
            box_list_2d[box_list_2d.size() - 1].push_back(c.get());//box_list_2d[r].push_back(box_list[r][c].get());
        }
    }
    return box_list_2d;
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
int Grid::get_horizontal_gap() const {
    return gap_horz;
}
////////////////////
int Grid::get_vertical_gap() const {
    return gap_vert;
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
	std::cout << "rows " << box_list.size() << "\n"; 
	for(int k = 0; k < box_list.size(); k++)
	    std::cout << "row items in box_list [" << k << "] " << box_list[k].size() << "\n";
*/
////////////////////
float Grid::get_radius() const {
    return radius;
}
////////////////////
////////////////////
////////////////////
// boolean
////////////////////
bool Grid::is_empty() const {
    for(auto grid : box_list) {
        for(auto boxes : grid) {
            if(boxes->get_gui_count() > 0 || boxes->get_image_count() > 0)
                return false;
        }
    }
    return true;
}
////////////////////
////////////////////
////////////////////
// callbacks
////////////////////
void Grid::on_highlight(int rows, int cols) {
	if(!is_visible() || is_disabled() || !is_active()) return;
	///////////////////
	if(!highlight) return;
	// on hover
    if(Mouse::is_over(box_list[rows][cols]->get_position().x + cols, box_list[rows][cols]->get_position().y + rows, get_width(), get_height()))
		box_list[rows][cols]->set_color(highlight_color);
	else box_list[rows][cols]->set_color(get_color());
}
