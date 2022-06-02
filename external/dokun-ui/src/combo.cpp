#include "../include/combo.hpp"

Combobox::Combobox() : color(106, 106, 106, 1.0), list(nullptr), scrollbar(nullptr), image(nullptr), label(nullptr),
// button
button_color(0, 51, 102, 1.0),
button_width(20),
button_activated (false),
// outline
outline(false),
outline_width(2.0),
outline_color(0, 51, 102, 1.0),
outline_antialiased(false),
// highlight
highlight(true),
highlight_color(0, 51, 102, 1.0)
{
	set_position(0, 0);
	set_size(90, 20); // 50, 25
	set_orientation(0);
}
Combobox::~Combobox()
{
    // delete label
    if(label) {
        delete label;
        label = nullptr;
    }
    // delete image
    if(image) {
        delete image;
        image = nullptr;
    }
    /*// delete x
    if(x) {
        delete x;
        x = nullptr;
    }      
    // delete x
    if(x) {
        delete x;
        x = nullptr;
    }      
    // delete x
    if(x) {
        delete x;
        x = nullptr;
    }*/                
}

void Combobox::draw()
{
    on_draw(); // sets position relative to parent, regardless of visibility
	if(is_visible())
	{
        on_button();	
		// update combo properties based on 'selected' widget
	    on_select();
		// Draw combo_box
		Renderer::draw_combobox(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y,
		    get_color().x, get_color().y, get_color().z, get_color().w,
		    (!shader.get()) ? GUI::gui_shader : shader.get(),
			button_color, button_width, button_activated);	
		// Draw arrow		
		if(list != nullptr)
		{
			// adjust width of list items (to equal width of combobox)
			for(int i = 0; i < list->get_item_count(); i++) {
				if(scrollbar == nullptr) list->get_item(i)->set_width(get_width() + get_button_width());
				if(scrollbar != nullptr) list->get_item(i)->set_width(get_width());
			}
			// set list right below combo_box
			list->set_position(get_position().x, get_position().y + get_height()); // item[0] is the first item so i
			list->draw(); // Draw list
		} // list is not null
		if(scrollbar != nullptr) 
		{
			scrollbar->set_size(get_button_width(), scrollbar->get_height()); // height?
			scrollbar->set_position(get_button_x(), get_button_y() + get_button_height());
			scrollbar->draw();
		}
	    if(image != nullptr)
		{
		    if(image->get_alignment() == "left"  ) image->set_relative_position(0, 0);
            if(image->get_alignment() == "center") image->set_relative_position((get_width() - get_height()) / 2, 0);
		    if(image->get_alignment() == "right" ) image->set_relative_position(get_width() - get_height(), 0);		
			image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y()); // update image position
			image->scale_to_fit(get_width(), get_height());
			image->draw();
		}
		if(label != nullptr)
		{
			if(label->get_alignment() == "left"  ) label->set_relative_position(0, 0);
		    if(label->get_alignment() == "center") label->set_relative_position((get_width()-label->get_width())/2, (get_height()-label->get_height())/2);						
		    if(label->get_alignment() == "right" ) label->set_relative_position(get_width() - label->get_width(), 0);	       
            label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // update label position		
		    label->draw();
		}
	} // is visible
}     
/////////////
void Combobox::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Combobox::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////                       
int Combobox::draw(lua_State * L)
{
    return 0;
}
void Combobox::add(const Box& widget){}                   
int Combobox::add(lua_State * L)
{
    return 0;
}
/////////////
/////////////
/////////////
void Combobox::set_list(const List& list)
{
	this->list = &const_cast<List&>(list);
	const_cast<List&>(list).set_visible(false); // list is not visible until combobox is activated
}                      
int Combobox::set_list(lua_State * L)
{
    return 0;
}
/////////////
void Combobox::set_scrollbar(const Scrollbar& scrollbar)
{
	this->scrollbar = &const_cast<Scrollbar&>(scrollbar);
	const_cast<Scrollbar&>(scrollbar).set_visible(false);
}
int Combobox::set_scrollbar(lua_State * L)
{
    return 0;
}
/////////////
void Combobox::set_text(const std::string& text)
{
    if(!label) dokun::Logger("Combo::set_text: attempting to access function of nullptr, label", "error");
	label->set_string(text);
}     
int Combobox::set_text(lua_State *L)
{
    return 0;
}
/////////////
void Combobox::set_label(const dokun::Label& label)
{
	if(!this->label) { this->label = new dokun::Label(); this->label->set_parent(*this); } // if self.label does not exist, create it and set as child
	this->label->copy(label); // copy the label data, instead of setting a new one (that may already have its own parent)
}
int Combobox::set_label(lua_State *L)
{
    return 0;
}
/////////////
void Combobox::set_image(const Image& image)
{
	if(!this->image) { this->image = new Image(); } // if self.image does not exist, create it
	this->image->copy(image); // copy the image data, instead of setting a new one (that may already have its own parent)
}
int Combobox::set_image(lua_State *L)
{
    return 0;
}
/////////////
void Combobox::set_color(unsigned int red, unsigned int green, unsigned int blue)
{
	color = Vector4(red, green, blue, color.w);
}
/////////////
void Combobox::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
}
void Combobox::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}       
void Combobox::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
void Combobox::set_color(const Vector4& color)
{
	this->color = color;
}
int Combobox::set_color(lua_State *L)
{
    return 0;
}	
/////////////
std::string Combobox::get_text()const
{
    return "";
}                
int Combobox::get_text(lua_State *L)
{
    return 1;
}
/////////////
dokun::Label * Combobox::get_label()const
{
	return label;
}
int Combobox::get_label(lua_State *L)
{
    return 1;
}
/////////////
Image * Combobox::get_image()const
{
	return image;
}                   
int Combobox::get_image(lua_State *L)
{
    return 1;
}
/////////////
Vector4 Combobox::get_color()const
{
	return color;
}                   
int Combobox::get_color(lua_State *L)
{
    return 4;
}
/////////////
List * Combobox::get_list() const
{
	return list;
}
int Combobox::get_list(lua_State * L)
{
    return 1;
}
/////////////
Scrollbar * Combobox::get_scrollbar()const
{
	return scrollbar;
}
int Combobox::get_scrollbar(lua_State * L)
{
    return 1;
}
/////////////
/////////////
	// button
int Combobox::get_button_x() const
{
	return get_x() + get_width();
} 
int Combobox::get_button_x(lua_State *L)
{
    return 1;
}
int Combobox::get_button_y() const
{
	return get_y();
}                  
int Combobox::get_button_y(lua_State *L)
{
    return 1;
}
int Combobox::get_button_width() const
{
	return button_width;
}              
int Combobox::get_button_width(lua_State *L)
{
    return 1;
}
int Combobox::get_button_height() const
{
	return get_height();
}             
int Combobox::get_button_height(lua_State *L)
{
    return 1;
}
/////////////
void Combobox::on_button() // show or hide list on buttonpress or buttonrelease
{
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	/////////////////////////
	// if mouse over button ...
	if(Mouse::is_over(get_button_x(), get_button_y(), get_button_width(), get_button_height())) 
	{
		// and if mouse is pressed ..
		if(Mouse::is_pressed(1))
		{
			if(list) list->set_visible(true);
            if(scrollbar) scrollbar->set_visible(true);
            button_activated = true; // button activated				
		}
    } else //if(!Mouse::is_over(get_button_x(), get_button_y(), get_button_width(), get_button_height()))  
	{
		// if mouse is pressed elsewhere ..
		if(list) {
		if(list->is_visible())
		{
		    for(int i = 0; i < list->get_item_count(); i++)
		    {
			    Box * item = list->get_item(i);
	            if(Mouse::is_over(item->get_rect())) 
				{
	                if(Mouse::is_pressed(1))
		            {
				        list->set_selection(*item);
				    #ifdef DOKUN_DEBUG
                        std::cout << "Combo->list item " << i << " selected" << std::endl;
				    #endif	
				    }	
                }		
                if(Mouse::is_pressed(1))
		        {				
					if(list) list->set_visible(false);
		            if(scrollbar) scrollbar->set_visible(false);
		            button_activated = false; // button not activated
				}
		    }
		}
	    }	
	} // mouse over button	
}
/////////////
void Combobox::on_select() // what happens after selecting an item
{
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	/////////////////////////
    if(!list) return; // if no list, exit function
	Box * selection = list->get_selection();
	if(!selection) return; // if no selection, exit function
	// copy Color from selection
	set_color(selection->get_color()); 
	// copy Image from selection
	if(selection->get_image())
	{
		set_image(*selection->get_image()); // copy image
		image->set_visible(true);
	} 
	else if(!selection->get_image()) {
	    if(image) image->set_visible(false);  // what if selection has no image, hide image
	}
    // copy dokun::Label from selection
    if(selection->get_label())
	{
		if(!selection->get_label()->get_string().empty()) set_label(*selection->get_label()); // copy label
    } 
    else if(!selection->get_label()) { 
        if(label) set_text(""); // what if selection has no label, clear string (combobox must have a label first or will crash)
    }
}
