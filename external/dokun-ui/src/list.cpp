#include "../include/list.hpp"
// I want list to be a single quad that can have different labels, colors, only on part of / a section of the quad. A line separator can also be added, but its optional
// each section or part of the list (quad) will all share the same width of course
// but each part of the quad can have their own height, say a height of 10 
// so each of their contents will be separated by a unit of 10 - we will call this variable, separator_unit(10)
List::List() : color(106, 106, 106, 1.0), selection(nullptr), //image(nullptr),
// outline
outline(true),
outline_width(1.0),
outline_color(0, 0, 0, 1.0),
outline_antialiased(false),
// gradient
gradient(false),
gradient_color(color)
{
	set_position(0, 0);
	set_size(80, 20);
	set_orientation(0);
}
////////////////////
List::List(int x, int y) : List()
{
	set_position(x, y);
}
////////////////////
List::List(int x, int y, int width, int height) : List()
{
	set_position(x, y);
	set_size(width, height);
}
////////////////////
int List::new_(lua_State *L)
{
    return 1;
}
////////////////////
List::~List()
{}
////////////////////		
void List::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(!is_visible()) return;
	for(int i = 0; i < box_list.size(); i++)
	{
		Box * item = box_list[i].get();
		/////////////////
		if(i == 0) item->set_position(get_position()); // first item is set at position of list (this)
		if(i != 0) {
		    item->previous = box_list[i - 1].get();
		    item->set_position(get_position().x, item->previous->get_position().y + item->previous->get_height());
		} // item[0] is the first item so it won't have any previous
        /////////////////
            on_item(item, i);
			// Draw box (multiple)
		    /*Renderer::draw_box(item->get_x(), item->get_y(), item->get_width(), item->get_height(), 
			    get_angle(), get_scale().x, get_scale().y, 
			    item->get_color().x, item->get_color().y, item->get_color().z, item->get_color().w,
			    (!shader.get()) ? GUI::gui_shader : shader.get(),
			    item->get_radius(), false, false, 0, color, false, false, false, item->get_title_bar_button_close_color(),
			    item->has_outline(), item->outline_width, item->outline_color, item->outline_antialiased,
			    item->has_border(), 0, color,
			    item->has_gradient(), item->gradient_color, item->gradient_value,
			    item->has_shadow()
			);*/
			item->draw();
		/////////////////
		// Draw image
			/*Image * image = item->get_image();
            if(image != nullptr)
			{
				int image_width  = image->get_width();
			    int image_height = image->get_height();					
				if(image->get_alignment() == "left"  ) image->set_relative_position(0, 0);
				if(image->get_alignment() == "center") image->set_relative_position((item->get_width() - item->get_height()) / 2, 0);
				if(image->get_alignment() == "right" ) image->set_relative_position(item->get_width() - item->get_height(), 0);		
				image->set_position(item->get_x() + image->get_relative_x(), item->get_y());
				image->scale_to_fit(get_width(), get_height());
                image->draw();				
			}	
			// Draw label
			dokun::Label * label = item->get_label();
			if(label)
			{
		        if(label->get_alignment() == "left"  ) { label->set_relative_position(0                                          , 0); } // default - relative_position will always be (0, 0) unless you change the alignment
				if(label->get_alignment() == "center") { label->set_relative_position((item->get_width()-label->get_width())/2   , (item->get_height()-label->get_height())/2); }						
				if(label->get_alignment() == "right" ) { label->set_relative_position(item->get_width() - label->get_width()     , 0); }	
                if(label->get_alignment() == "none"  ) {} // with this you are free to set the label's relative position to whatever you want         
                label->set_position(item->get_x() + label->get_relative_x(), item->get_y() + label->get_relative_y()); // set actual position
                label->draw(); // draw label manually since we are drawing from List::draw and NOT Box::draw  
			}*/			
	}
}
/////////////
void List::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void List::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int List::draw(lua_State *L)
{
    return 0;
}
////////////////
void List::add_item(const Box& item)
{
    std::unique_ptr<Box> list_item (&const_cast<Box&>(item));
	box_list.push_back(std::move(list_item));
	//////////////////////////////
	if(box_list.size() == 1) const_cast<Box&>(item).set_position(get_position());
	const_cast<Box&>(item).set_size(get_width(), get_height());
	const_cast<Box&>(item).set_outline(outline);
	const_cast<Box&>(item).set_outline_width(outline_width);
	const_cast<Box&>(item).set_outline_color(outline_color);//const_cast<Box&>(item).set_outline_antialiased(outline_antialiased);
}
////////////////
void List::add_items(int items_count) {
    for(int i = 0; i < items_count; i++) {
        std::unique_ptr<Box> list_item (std::unique_ptr<Box>(new Box()));
        box_list.push_back(std::move(list_item));
        /////////////////////////////////////////
        if(box_list.size() == 1) box_list[0]->set_position(get_position());
        box_list[i]->set_size(get_width(), get_height());
        box_list[i]->set_outline(outline);
        box_list[i]->set_outline_width(outline_width);
        box_list[i]->set_outline_color(outline_color);
    }
}
////////////////
int List::add_item(lua_State *L)
{
    return 0;
}
////////////////
/*void List::set_label(const dokun::Label& label)
{
    std::unique_ptr<dokun::Label> list_label(&const_cast<dokun::Label&>(label));
	this->label = std::move(list_label);
	this->label->set_parent(*this);
}*/
////////////////
void List::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
////////////////
void List::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
}
////////////////
void List::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
////////////////
void List::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
////////////////
void List::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
////////////////
void List::set_selection(const Box& selection)
{
	this->selection = &const_cast<Box&>(selection);
}
int List::set_selection(lua_State * L)
{
    return 0;
}
///////////////
/*dokun::Label * List::get_label() const {
	return label.get();
}*/
///////////////
Vector4 List::get_color() const
{
	return color;
}
///////////////
Box * List::get_item(int index) const
{
    if(box_list.empty()) return nullptr;
	if(box_list.size() < index + 1) throw std::out_of_range("attempt to access invalid index at List::box_list");
	return box_list[index].get();
}
///////////////
int List::get_item_count()const // returns number of items added to list
{
	return box_list.size();
}
///////////////
Box * List::get_selection() const
{
	return selection;
}
///////////////
int List::get_selection(lua_State * L)
{
    return 1;
}
/////////////////
	// outline
void List::set_outline(bool outline)
{
	this->outline = outline;
}
int List::set_outline(lua_State * L)
{
    return 0;
}
void List::set_outline_width(double width)
{
	this->outline_width = width;
}
int List::set_outline_width(lua_State * L)
{
    return 0;
}
void List::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	this->outline_color = Vector4(red, green, blue, alpha);
}
int List::set_outline_color(lua_State * L)
{
    return 0;
}
void List::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z);
}
void List::set_outline_color(const Vector4& color)
{
	set_outline_color(color.x, color.y, color.z, color.w);	
}
void List::set_outline_antialiased(bool antialiased)
{
	outline_antialiased = antialiased;
}
//////////////////
void List::on_item(Box* item, int index) // will not work under list->is_visible
{
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	/////////////////////////
    if(Mouse::is_over(item->get_rect()))
	{
		// highlight item on hover
		item->set_outline_width(3.0);
		//if mouse is pressed, set the selected item
		if(Mouse::is_pressed(1))
		{
			set_selection(*item);
			#ifdef DOKUN_DEBUG	
				std::cout <<"Item " << index << " selected.\n";
			#endif    
		}
	} else {item->set_outline_width(1.0);} // remove highlight	
}
