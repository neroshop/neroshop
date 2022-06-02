#include "../include/tab.hpp"

Tab::Tab()
{
    set_size(100, 50);//(50, 10);
    set_color(64, 64, 64, 1.0);

	//label = new dokun::Label(); // label must be initialized manually!!
}
////////////
Tab::~Tab()
{}
////////////
void Tab::draw()
{
	if(is_visible())
	{
	    for(int i = 0; i < tab_list.size(); i++)
	    {
	        if(i == 0) tab_list[0]->set_position(get_x(), get_y());
	        if(i != 0) { tab_list[i]->set_position( tab_list[i - 1]->get_x() + tab_list[i - 1]->get_width(), tab_list[i - 1]->get_y() ); }
	        // tab_body position is always equal to get_x()  which is the first tab's (tab_list[0]) x position
	        //Renderer::draw_tab(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, get_color().x, get_color().y, get_color().z, get_color().w,
	        //    tab_list.size(), true);
	        //if(tab_list[i]->get_width())// if tab_width less than label_width, increase tab_width    
	       	Renderer::draw_tab(tab_list[i]->get_x(), tab_list[i]->get_y(), tab_list[i]->get_width(), tab_list[i]->get_height(), get_angle(), get_scale().x, get_scale().y, tab_list[i]->get_color().x, tab_list[i]->get_color().y, tab_list[i]->get_color().z, tab_list[i]->get_color().w,
	            (!shader.get()) ? GUI::gui_shader : shader.get(),
	            5/*tab_list.size()*/, true);     
	        if(label != nullptr)
	        {
		        if(label->get_alignment() == "left"  ) { label->set_relative_position(0                                     , 0); } // default - relative_position will always be (0, 0) unless you change the alignment
				if(label->get_alignment() == "center") { label->set_relative_position((get_width() - label->get_width()) / 2, (get_height() - label->get_height()) / 2); }						
				if(label->get_alignment() == "right" ) { label->set_relative_position( get_width() - label->get_width()     , 0); }	
                if(label->get_alignment() == "none"  ) {} // with this you are free to set the label's relative position to whatever you want         
                label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // set actual position	        
	        }
	    }
	}
	on_draw();	
}           
/////////////
void Tab::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Tab::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}                     
////////////
int Tab::draw(lua_State * L){return 0;}
////////////
void Tab::add(const Box& box)//(const Tab& tab)
{
    // set default values
    const_cast<Box&>(box).set_color(get_color());
    const_cast<Box&>(box).set_size(get_size  ());
    // store box in tab_list
    tab_list.push_back(&const_cast<Box&>(box)); 
}
////////////                   
int Tab::add(lua_State * L){return 0;}
////////////	
void Tab::set_text(const std::string& text)
{
    label->set_string(text);
}     
////////////
int Tab::set_text(lua_State *L){return 0;}
////////////
void Tab::set_label(const dokun::Label& label)
{
    this->label = &const_cast<dokun::Label&>(label);
    this->label->set_parent(* this);
}         
////////////
int Tab::set_label(lua_State *L){return 0;}
////////////
void Tab::set_image(const Image& image)
{
    this->image = &const_cast<Image&>(image);
}
////////////
int Tab::set_image(lua_State *L){return 0;}
////////////
void Tab::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    color = Vector4(red, green, blue, alpha);
}
////////////
void Tab::set_color(const Vector3& color)
{
    set_color(color.x, color.y, color.z, this->color.w);
}       
////////////
void Tab::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
////////////
void Tab::set_color(const Vector4& color)
{
    set_color(color);
}
////////////
int Tab::set_color(lua_State *L){return 0;}
////////////	
std::string Tab::get_text()const{return label->get_string();}                
int Tab::get_text(lua_State *L){return 1;}
////////////
dokun::Label * Tab::get_label()const{return label;}                   
int Tab::get_label(lua_State *L){return 1;}
////////////
Image * Tab::get_image()const{return image;}                   
int Tab::get_image(lua_State *L){return 1;}
////////////
Vector4 Tab::get_color()const{return color;}                   
int Tab::get_color(lua_State *L){return 4;}
////////////
Vector2 Tab::get_body_position() const
{
    return Vector2(get_x() + 0, get_y() + get_height());
}
////////////
int Tab::get_body_position(lua_State *L)
{
    return 2;
}
////////////
Vector2 Tab::get_body_size() const
{
    return Vector2( get_width() * 5, get_height() * 5 );
}
////////////
int Tab::get_body_size(lua_State *L)
{
    return 2;
}
////////////
////////////
////////////
////////////
////////////
