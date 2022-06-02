#include "../include/menubar.hpp"

Menubar::Menubar() : color(64, 64, 64, 1.0), submenu_color(color),
// fill
fill(true), // true by default // fills up any extra space within the menubar
// spacing
spacing(0),//(0),
// outline
outline(false),
outline_width(1.0),
outline_color(255, 255, 255, 1.0),
outline_antialiased(false),
// highlight
highlight(true),
highlight_color(0, 51, 102, 1.0),
// gradient
gradient(false),
gradient_color(color)
{
	set_position(0, 0);
	set_size(Renderer::window_width, 20); //dokun::Logger("Menubar width is " + String::to_string(get_width()) + " on creation");// menubar_width / menu_width = how_many_menu_can_fit  
	set_orientation(0);	                  // If you want a vertical menubar then just use a combo_box
	
	bar = new Box();
	// set bar properties (to match Menubar)
	bar->set_position(this->get_x(), this->get_y());
	bar->set_size(this->get_width(), this->get_height());
	bar->set_color(this->color);
	bar->set_orientation(this->get_orientation());
	// outline
	bar->set_outline(true);
	bar->set_outline_width(1.0);
    bar->set_outline_color(0, 0, 0, 1.0);
    bar->set_outline_antialiased(false);
    // gradient
    bar->set_gradient(this->gradient);
    bar->set_gradient_color(this->gradient_color);
}
///////////////
Menubar::Menubar(int x, int y) : Menubar()
{
	set_position(x, y);
}
///////////////
Menubar::Menubar(int x, int y, int width, int height) : Menubar()
{
	set_position(x, y);
	set_size(width, height);
}
///////////////
int Menubar::menu_new(lua_State *L)
{
    return 1;
}
///////////////
Menubar::~Menubar()
{}
///////////////
Box * Menubar::selected_menu (nullptr);
///////////////		
void Menubar::draw() // because Menubar is parent to all the menus, its menu children will be automatically drawn unless "visible" is set to false
{
    //on_draw(); // set position relative to parent, regardless of visibility
	if(is_visible())
	{
	    if(!fill) // we will not be filling any extra spaces in the bar
	    {
		    // update bar properties (to match Menubar)
		    bar->set_position(this->get_x(), this->get_y());
		    bar->set_size(this->get_width(), this->get_height());
		    bar->set_color(this->color); //
	        bar->set_outline(true);
	        bar->set_outline_width(1.0);
            bar->set_outline_color(0, 0, 0, 1.0);
            bar->set_outline_antialiased(false);
		    // Draw bar
		    bar->draw();
		}
		// Do not draw menubar - Imaginary menubar that holds menus and does not actually exist. It is just a concept (only the menus are visible)
        for(int i = 0; i < menu_list.size(); i++)
	    {
		    Box * menu = menu_list[i];
		    // On hover, highlight
		    if(highlight) {if(Mouse::is_over(menu->get_rect()) && (selected_menu != menu)) menu->set_color(highlight_color); else { if(selected_menu != menu) menu->set_color(color); } } // no menu must be active when you hover
			// On hover but this time there is an active_menu, show menu of hovered_menu - if(((active) && (active !=menu)) && Mouse::is_over(menu->get_rect()))
			if(get_orientation() == 0) 
			{  // HORIZONTAL MENUBAR
			    // adjust and update size of each menu according to menubar size
			    // if fill_entire_bar, fill extra spaces within menubar
			    if(fill) menu->set_width(this->get_width() / menu_list.size()); // 0 - when horizontal each menu's width is equal to the menubar's width divided by the number of menus // menu_bar_width / number_of_menus =  1280 / 1 = 1280 (1 menu means the single menu will be the entire width of the screen;) | 1280 / 2 =  640 (2 menus would mean each menu would be half the width of the screen) 
			    // if not fill_entire_bar, maintain its original size
			    // calculate position and size of previous menus
			    if(i == 0) menu->set_position(this->get_x(), this->get_y()); // first menu item
		        if(i != 0) {menu->previous = menu_list[i - 1];  menu->set_position((menu->previous->get_x() + spacing) + menu->previous->get_width(), get_y());} // menu[0] is the first menu so it won't have any previous
		        if(i != menu_list.size()-1) menu->next = menu_list[i + 1]; // assign next_menu if not last
		        // Draw menus (children of parent menubar will be automatically drawn with the 'on_draw' function)
            }
		    if(get_orientation() == 1) 
			{ 	// VERTICAL MENUBAR
			    // adjust and update size of each menu according to menubar size
			    menu->set_height(this->get_height() / menu_list.size()); // 1 - when horizontal each menu's height is equal to the menubar's height divided by the number of menus
			    // calculate position and size of previous menus
		        if(i != 0) {menu_list[i]->previous = menu_list[i - 1];  menu_list[i]->set_position(get_position().x, (menu_list[i]->previous->get_position().y + spacing) + menu_list[i]->previous->get_height());} // menu[0] is the first menu so it won't have any previous
		        if(i != menu_list.size()-1) menu_list[i]->next = menu_list[i + 1]; // assign next_menu if not last
		        // Draw menus (children of parent menubar will be automatically drawn with the 'on_draw' function)
            }
			// Draw label (for menus - automatically drawn if object is a widget)
			// Draw image (for menus - automatically drawn if object is a widget)
			// Draw menus
			menu->draw(); // if menus parent is set to menubar, remove this line! on_draw will draw the menus automatically. Also draws labels and images
			/////////////////////
			for(int j = 0; j < menu->sub.size(); j++)
			{
				Box * sub = menu->sub[j];
				// on focus lost (both)
				if((!Mouse::is_over(sub->get_rect()) || !Mouse::is_over(menu->get_rect())) && Mouse::is_pressed(1)) 
				{
				    selected_menu = nullptr; 
				    menu->set_color(color); 
				    if(sub->is_visible()) sub->hide();
				}// mouse pressed, but mouse not over box, hide sub if visible. Also set active menu to nullptr and restore menu color
				// On press (menu)
				if(Mouse::is_over(menu->get_rect()) && Mouse::is_pressed(1)) 
				{
				    selected_menu = menu; 
				    menu->set_color(highlight_color);
				    if(!sub->is_visible()) sub->show();
				} // menu is pressed, show sub, if not visible. Also, set menu as the active menu
			    // set sub sizes
			    // if fill empty spaces
			    if(fill) sub->set_width( this->get_width() / menu_list.size() );
			    // if not fill, maintain its original size
	            // set sub geometry
	            if(j == 0) {sub->set_position( menu->get_x(), menu->get_y() + menu->get_height() );}// first sub, set position to first menubar item
				if(j != 0) {sub->previous = menu_list[i]->sub[j - 1];sub->set_position(sub->previous->get_position().x, sub->previous->get_position().y + sub->previous->get_height());}
			    if(j != menu->sub.size()-1) sub->next = menu->sub[j + 1]; // if not last element, set its "next"	       
			    // if sub is visible      
				if(sub->is_visible())
		        {
		            // User input stuff here first ...
				    // On hover (sub )
				    if(highlight)
				    {
				        if(Mouse::is_over(sub->get_rect()))
	                    {
						    sub->set_color(Vector4(highlight_color.xyz, sub->get_color().w));
 						    if(Mouse::is_pressed(1)) 
					        {
							#ifdef DOKUN_DEBUG0	
						        std::cout <<"Submenu " << j << " at index " << i << " selected.\n";
					        #endif
							}
					    }
					    else sub->set_color(Vector4(submenu_color.xyz, sub->get_color().w));
				    }		   
				    /////////////////     
					// draw sub_menu
					sub->draw(); // Draw sub (Box). Also draws labels and images
				    // Draw image (for sub - automatically drawn if object is a widget)
			        // Draw label (for subs - automatically drawn if object is a widget)				    
				}
			}
		}	
		// DO NOT call on_draw since Menubar calls Box::draw which also calls on_draw!!
	}
}    
/////////////
void Menubar::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Menubar::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}      
/////////////                 
int Menubar::draw(lua_State *L)
{
    return 0;
}
void Menubar::add(const Box& menu)
{
	menu_list.push_back(&const_cast<Box&>(menu));                     //std::cout << "number of menus = " << menu_list.size() << std::endl; // get number of menus after creation
	if(fill ) const_cast<Box&>(menu).set_size(get_width() / menu_list.size(), get_height()); // window_width / num_of_menu, 20 
	if(!fill) const_cast<Box&>(menu).set_size(100, get_height()); // default width of each menu will be 100
	const_cast<Box&>(menu).set_color(get_color());
}          
void Menubar::add(const std::string& menu_name)
{
	Box * menu = new Box();
	// create a new label for menu (in case it does not have one and so it does not crash when trying to access its label)
	if(!menu->get_label()) 
	{ 
	    dokun::Logger("Menu: " + menu_name + " does not have a label so I will create one for it.");
	    dokun::Label * menu_label = new dokun::Label(); 
	    menu->set_label(*menu_label); // also sets label parent to sub
	    dokun::Logger("dokun::Label has been created for Menu: " + menu_name, "info");
	}	
	// menu_label -  make sure parent is set (which it should be) so label can be drawn
	menu->get_label()->set_string   (menu_name); // set menu's label string
	menu->get_label()->set_alignment("center" );
	// add menu to menubar
	add(*menu);
}
int Menubar::add(lua_State * L)
{
    return 0;
}
void Menubar::sub(const Box& submenu, int index)
{
    if(menu_list.size() < index + 1) // if menu at index does not exist
		dokun::Logger("Attempt to access invalid location in sub() | menubar.cpp (180)");
	if(fill ) const_cast<Box&>(submenu).set_size(menu_list[index]->get_width(), menu_list[index]->get_height());
	if(!fill) const_cast<Box&>(submenu).set_height(menu_list[index]->get_height()); // only change the height, leave the width alone (if not filling entire bar)
	const_cast<Box&>(submenu).set_position(menu_list[index]->get_position().x, menu_list[index]->get_position().y + menu_list[index]->get_height());
	const_cast<Box&>(submenu).set_color(Vector4(submenu_color.xyz, 230/*204*/)); // rgba:200 / 255 = glsl:0.784313725 | glsl:0.784313725 * 255 = rgba:200 // rgba=0-255, float=0.0-1.0
	const_cast<Box&>(submenu).set_visible(false);
	const_cast<Box&>(submenu).previous = menu_list[index]; // set menu as its previous
	menu_list[index]->sub.push_back(&const_cast<Box&>(submenu)); // &const_cast<Box&>(submenu)
	
	/*
    if(sub_list.size() < index + 1) // if row at index does not exist, create it
	{
        sub_list.push_back(std::vector<Box *>()); // create empty row
	}
    if(menu_list.size() < index + 1) // if menu at index does not exist
		dokun::Logger("Attempt to access invalid location in sub() | menubar.cpp (180)");	
	sub_list[index].push_back(&const_cast<Box&>(submenu));
	const_cast<Box&>(submenu).set_size(50, get_height());
	const_cast<Box&>(submenu).set_position(menu_list[index]->get_position().x, menu_list[index]->get_position().y + menu_list[index]->get_height());
	const_cast<Box&>(submenu).set_color(submenu_color);
	const_cast<Box&>(submenu).set_visible(false);
	if(sub_list[index].size() == 1) // if first sub_item has been added
	{
		const_cast<Box&>(submenu).previous = menu_list[index]; // set menu as its previous
	}*/
}
void Menubar::sub(const std::string& menu_name, const std::string& sub_name) // menu must exist
{
	// sub_menu
	Box * submenu = new Box();
	// create a new label for sub (in case it does not have one)
	if(!submenu->get_label()) 
	{ 
	    dokun::Logger("Submenu: " + sub_name + " does not have a label so I will create one for it.");
	    dokun::Label * sub_label = new dokun::Label();
	    submenu->set_label(*sub_label); // also sets label parent to sub
	    dokun::Logger("dokun::Label has been created for Submenu: " + sub_name, "info");
	}
	// set existing sub->label's properties (some Boxes are created with a pre-installed label)
	// be sure label_parent is set to submenu
	submenu->get_label()->set_string   (sub_name);
	//if(fill ) submenu->get_label()->set_alignment("center");
	submenu->get_label()->set_relative_position(5, 5);//if(!fill) submenu->get_label()->set_relative_position(5, 5);
	// add submenu to sub_list ...
	for(int j = 0; j < menu_list.size(); j++)
	{
		if(menu_list[j]->get_text() == menu_name)
		{
			std::cout << "Submenu: " << sub_name << " added at index [" << j << "]\n";
			sub(*submenu, j);
			break;
		}
	}
}
////////////
////////////
void Menubar::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    color = Vector4(red, green, blue, alpha);
}
////////////
void Menubar::set_color(const Vector3& color)
{
    set_color(color.x, color.y, color.z);
}
////////////
void Menubar::set_color(const Vector3& color, double alpha) {
    set_color(color.x, color.y, color.z, alpha);
}
////////////
void Menubar::set_color(const Vector4& color)
{
    set_color(color.x, color.y, color.z, color.w);
}
////////////
void Menubar::set_submenu_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
    submenu_color = Vector4(red, green, blue, alpha);
}
////////////
void Menubar::set_submenu_color(const Vector3& color)
{
    set_submenu_color(color.x, color.y, color.z);
}
////////////
void Menubar::set_submenu_color(const Vector4& color)
{
    set_submenu_color(color.x, color.y, color.z, color.w);
}
////////////
// outline
void Menubar::set_outline(bool outline)
{} 
int Menubar::set_outline(lua_State *L)
{
    return 0;
}
void Menubar::set_outline_width(double width)
{} 
int Menubar::set_outline_width(lua_State *L)
{
    return 0;
}
void Menubar::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{} 
int Menubar::set_outline_color(lua_State *L)
{
    return 0;
}
void Menubar::set_outline_color(const Vector3& color)
{}
void Menubar::set_outline_color(const Vector4& color)
{}
void Menubar::set_outline_antialiased(bool antialised)
{} 
int Menubar::set_outline_antialiased(lua_State *L)
{
    return 0;
}
////////////
void Menubar::set_fill(bool fill)
{
    this->fill = fill;
}
int Menubar::set_fill(lua_State * L)
{
    return 0;
}
////////////
////////////
Box * Menubar::get_menu(int index)
{
#ifdef DOKUN_DEBUG	
	if(menu_list.size() < index + 1) dokun::Logger("Attempt to access invalid location | menubar.cpp (234)", "error");
#endif	
	return menu_list[index];
}
////////////
Box * Menubar::get_menu(const std::string& menu_name)
{
    for(int i = 0; i < menu_list.size(); i++)
    {
        Box * menu = menu_list[i];
        if(!menu->get_label()) continue; // menu has no label, skip it
        if(menu->get_label()->get_string() == menu_name)
        {
            return menu;
        }
    }
    return nullptr;
}
////////////
int Menubar::get_menu(lua_State * L)
{
    return 1;
}
////////////
Box * Menubar::get_submenu(int menu_index, int sub_index)
{
#ifdef DOKUN_DEBUG	
	if(menu_list.size() < menu_index + 1) dokun::Logger("Attempt to access invalid location | menubar.cpp (243)", "error");
	if(sub_list.size() < sub_index + 1) dokun::Logger("Attempt to access invalid location | menubar.cpp (244)", "error");
#endif	
	return sub_list[menu_index][sub_index];
}
int Menubar::get_submenu(lua_State * L)
{
    return 1;
}
int Menubar::get_menu_count() // returns number of menus
{
	return menu_list.size();
}
int Menubar::get_menu_count(lua_State * L)
{
    return 1;
}
int Menubar::get_submenu_count(int index) // returns number of submenus at index
{
#ifdef DOKUN_DEBUG		
	if(sub_list.size() < index + 1) dokun::Logger("Attempt to access invalid location | menubar.cpp (229)");
#endif	
	return sub_list[index].size();
}
int Menubar::get_submenu_count(lua_State * L)
{
    return 1;
}
Vector4 Menubar::get_color()
{
	return color;
}
////////////
////////////
bool Menubar::has_sub(int menu_index)
{
	return (sub_list.size() > menu_index);
}
void Menubar::on_hover(void)
{
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	/////////////////////////
}
void Menubar::on_menu_press(Box * menu, int index)
{
	if(!is_visible()) return;
	if(is_disabled()) return;
	if(!is_active()) return;
	/////////////////////////
	if(Mouse::is_over(menu->get_rect()))
	{
		//if(Mouse::is_pressed(1)) {sub->set_visible(true);} 
		//else if(Mouse::is_pressed(1)) sub->set_visible(false);
	}
	/*
	if(sub_list.size() > index)
	{	
		for(int j = 0; j < sub_list[index].size(); j++) // number of menus with subs
		{
			Box * sub  = sub_list[index][j];
			
			if(menu->is_pressed()) {sub->set_visible(true);} 
			else 
			{
				if(Mouse::is_pressed(1)) 
				{
				    sub->set_visible(false);
				}
			}
		}
	}*/
}
