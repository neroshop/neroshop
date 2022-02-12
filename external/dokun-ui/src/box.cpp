#include "../include/box.hpp"

Box::Box() : radius(0.0), color(160, 160, 160, 1.0)/*(64, 64, 64, 1.0)*/, fill(true), image (nullptr), type("box"),
iconified(false),
maximized(false),
restored(true),
max_width(400), 
max_height(300),
old_width(200),
old_height(150),
alignment("left"), // left is default
forbidden_area(0, 0, 0, 0), // forbidden area
// title bar
title_bar(false), 
title_bar_height(30),
title_bar_color(0, 51, 102, 1.0), // I call this color, dokun_blue
title_bar_button_iconify(false),  // cannot have button_iconify  without button_close
title_bar_button_maximize(false), // cannot have button_minimize without button_close
title_bar_button_close(true),     // can have button_close by itself or with other buttons
title_bar_button_width(10),
title_bar_image(nullptr), // if you operate on this and do not assign it to nullptr or any value then the engine will crash (on Linux especially)
title_bar_label(nullptr), // if you operate on this and do not assign it to nullptr or any value then the engine will crash (on Linux especially)
title_bar_button_close_color(64, 64, 64, 1.0), // 235, 26, 34, 1.0
title_bar_horizontal_padding(10), // for label, image and other content inside titlebar
// outline
outline(false),
outline_width(2.0),
outline_color(0, 51, 102, 1.0), // dokun_blue outline to match title_bar_color
outline_antialiased(false),
// border
border(false),
border_size(1, 1),
border_thickness(0),
border_style(0),
border_radius(10.0),
border_color(47, 79, 79, 1.0),
// gradient
gradient(false),
gradient_color(color),
gradient_value(0.25),
gradient_type("linear"), // or radial (circle-like)
// shadow
shadow(false),
// highlight - used when hovered over ui
highlight(true),
highlight_color(0, 51, 102, 1.0),
// separator - horizontal by default
separator(false),
separator_count(1), // add_separator(), separator_count = separator_count + 1 
separator_width(200),
separator_height(1), 
separator_position(0, 10),
separator_color(32, 32, 32, 1.0),
// tooltip
tooltip_arrow_direction("down"),
tooltip_arrow_width(10),
tooltip_arrow_height(5),
tooltip_arrow_position(-1.0), // can be either x or y
tooltip_arrow_color(color),
// other
previous(nullptr),
next(nullptr)
{
	set_position(0, 0);
	set_size(200, 150);
	set_orientation(0);
	set_label(*new dokun::Label()); // create a(n)(initialized) label for widget (You will no longer need to check whether label is nullptr)
}
/////////////
Box::Box(const Box& widget)
{
	set_position(widget.get_position());
	set_size(widget.get_size());
	set_orientation(widget.get_orientation());
	//set_type (widget.get_type());
	set_color(widget.get_color());
	set_label(*widget.get_label());
	set_image(*widget.get_image());
	//set_(widget.get_());
	set_title_bar(widget.has_title_bar());
	set_title_bar_color(widget.title_bar_color);
	set_title_bar_size(widget.title_bar_height);
	set_title_bar_icon(*widget.title_bar_image);
	set_title_bar_text(widget.title_bar_label->get_string());
	set_title_bar_text_color(widget.title_bar_text_color);
	set_title_bar_button_iconify(widget.title_bar_button_iconify);
	set_title_bar_button_maximize(widget.title_bar_button_maximize);
	set_title_bar_button_close(widget.title_bar_button_close);
	// outline
	set_outline(widget.has_outline());
	set_outline_width(widget.outline_width);
	set_outline_color(widget.outline_color);
	set_outline_antialiased(widget.outline_antialiased);
	// border
	set_border(widget.has_border());
	// shadow
	set_shadow(widget.has_shadow());
	// gradient
	set_gradient(widget.has_gradient());
	//set_(widget.has_());
}
/////////////
Box::Box(const std::string& type)
{
	this->type = type;
    set_position(0, 0);
	set_size(200, 150);
	set_orientation(0);		
	set_label(*new dokun::Label());
}
/////////////
Box::Box(int x, int y)
{
    set_position(x, y);
	set_size(200, 150);
	set_orientation(0);
	set_label(*new dokun::Label());
}
/////////////
Box::Box(int x, int y, int width, int height)
{
    set_position(x, y);
	set_width  (width);
	set_height(height);
	set_orientation(0);
	set_label(*new dokun::Label());
}
/////////////
Box::~Box(void)
{
    // delete titlebar image
    if(title_bar_image) {
        delete title_bar_image;
        title_bar_image = nullptr;
    }
    // delete titlebar label
    if(title_bar_label) {
        delete title_bar_label;
        title_bar_label = nullptr;
    }
	//if(label) delete label; // call label destructor
	//if(image) delete image; // call image destructor
	std::cout << "box deleted\n";
}
/////////////
int Box::widget_new(lua_State *L)
{
    lua_settop(L, 0); // clear any value pushed on top
	// create table (object)
	lua_createtable(L, 0, 0);
    // set mt
	lua_getglobal(L, "Box");
	lua_setmetatable(L, 1);
	// set userdata
	Box ** widget = static_cast<Box**>(lua_newuserdata(L, sizeof(Box*)));
	* widget = new Box();
    lua_setfield(L, 1, "udata");
	// return widget
	if(lua_istable(L, -1))
	    return 1;
	lua_pushnil(L); // if not table
	    return 1; // return nil
}
/////////////
void Box::add(const GUI& gui) // for box widgets only
{
	const_cast<GUI&>(gui).set_parent(*this);
	child_list.push_back(&const_cast<GUI&>(gui)); // bad idea since child gui can set_parent without Box::add()
}
/////////////
int Box::add(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
			Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
			widget->add(*gui);
	    }
	}		
	return 0;
}
/////////////
void Box::remove(const GUI& gui) // for box widgets only
{
    if(gui.get_parent() == this) {
        const_cast<GUI&>(gui).set_parent(nullptr);
        // remove child gui from child_list
        // ...
        //auto iter = std::find_if(child_list.begin(), child_list.end(),
        //                     (GUI * child){return child == &gui;});
        //if (iter != child_list.end()) child_list.erase(iter);
        //if(std::find(contents.begin(), contents.end(), &const_cast<neroshop::Item&>(item)) != contents.end());
        std::cout << "child removed from box (parent)" << std::endl;
    }
}
/////////////
void Box::draw(void) 
{
    on_draw(); // set position relative to its parent (if it has one), regardless of whether visible or not
    draw_box();
    draw_icon();
	draw_tooltip();
}
/////////////
void Box::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Box::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
/////////////
int Box::draw(lua_State *L)  
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->draw();
	}
	return 0;
}
/////////////
void Box::draw_box() {
    if(!is_box()) return; 
    if(!is_visible()) return;
	if(!is_active()) {} // add shade to color
	if(is_active()) {} // add tint to color
	on_titlebar();
    on_drag();
	on_resize();
    /////////////////////////////////////////////////////////////////////////
    // Draw box	- at (0, 0) the titlebar will not show as it is the main widget at 0, 0 so the title bar would be hidden by the window's titlebar		
	Renderer::draw_box(get_position().x, get_position().y, get_width(), get_height(), get_angle(), 
			get_scale().x, get_scale().y, color.x, color.y, color.z, color.w,
			    radius, is_iconified(),
				// title_bar
				title_bar, title_bar_height, title_bar_color,
				title_bar_button_iconify, 
				title_bar_button_maximize,
				title_bar_button_close, title_bar_button_close_color,
				// outline
				outline, outline_width, outline_color, outline_antialiased,
				// border
				border, border_radius, border_color,
				// gradient
				gradient, gradient_color, gradient_value,
				// shadow
				shadow
	);
    /////////////////////////////////////////////////////////////////////////
			// Draw titlebar image and text (goes on top of titlebar) *****************************
			if(has_title_bar()) // if GUI has a titlebar
			{   // title_bar image or icon (goes on the titlebar)
				if(title_bar_image) // if title_bar_image is not nullptr
				{
				    // set title_bar_image alignment and position relative to title_bar
					if(title_bar_image->get_alignment() == "left"  ) {title_bar_image->set_relative_position(0/* + title_bar_horizontal_padding*/                                                , 0);} // we are scaling to fit image into titlbar, so no need to mess with image_relative_y // relative: child_position = parent_position + relative_position
                    if(title_bar_image->get_alignment() == "center") {title_bar_image->set_relative_position((get_title_bar_size().x - get_title_bar_size().y) / 2                               , 0);}			
                    if(title_bar_image->get_alignment() == "right" ) {title_bar_image->set_relative_position((get_title_bar_size().x - get_title_bar_size().y)/* - title_bar_horizontal_padding*/, 0);}								
					if(title_bar_image->get_alignment() == "none"  ) {}
				    title_bar_image->set_position(get_title_bar_position().x + title_bar_image->get_relative_x(), get_title_bar_position().y + title_bar_image->get_relative_y());
				    // scale image to fit the title_bar
                    title_bar_image->scale_to_fit(get_title_bar_size().x, get_title_bar_size().y);
                    // and finally, draw the title_bar_image
				    title_bar_image->draw();// will crash engine if not assigned to nullptr or any value
				}
                // title_bar text (goes on the titlebar)
                if(title_bar_label) // if "title_bar_label" is not nullptr
                {
                    // set title_bar_label alignment and position to title_bar_position + title_bar_label_position
			        if(title_bar_label->get_alignment() == "left"  ) { title_bar_label->set_relative_position(0 + title_bar_horizontal_padding                                                      , (get_title_bar_size().y - 10/*title_bar_label->get_height()*/) / 2); } // the 10 here, is used in place of the label's height (for more accurate results) // keep y positioned at center of titlebar at all times
                    if(title_bar_label->get_alignment() == "center") { title_bar_label->set_relative_position((get_title_bar_size().x - title_bar_label->get_width()) / 2                           , (get_title_bar_size().y - 10/*title_bar_label->get_height()*/) / 2); } // the 10 here, is used in place of the label's height (for more accurate results) // keep y positioned at center of titlebar at all times			
                    if(title_bar_label->get_alignment() == "right" ) { title_bar_label->set_relative_position((get_title_bar_size().x - title_bar_label->get_width()) - title_bar_horizontal_padding, (get_title_bar_size().y - 10/*title_bar_label->get_height()*/) / 2); } // the 10 here, is used in place of the label's height (for more accurate results) // keep y positioned at center of titlebar at all times
                    if(title_bar_label->get_alignment() == "none"  ) {} // default
                    title_bar_label->set_position(get_title_bar_position().x + title_bar_label->get_relative_x(), get_title_bar_position().y + title_bar_label->get_relative_y());
                    // and finally, draw the title_bar_label (be sure title_bar_label string is not empty)
                    if(!title_bar_label->get_string().empty()) title_bar_label->draw(); // NOTE: title_bar_label's parent is not the Box but the Box's titlebar // will crash engine if not assigned to nullptr or any value
                }
			} // end of titlebar image *********************************************
    /////////////////////////////////////////////////////////////////////////
			// Draw label (goes inside box) **********************************************************
            if(label) // make sure Box has an "initialized" label beforehand (or else engine will crash)
            {   // set label's alignment and position relative to the Box
		        if(label->get_alignment() == "left"  ) { label->set_relative_position(0                                     , 0); }
				if(label->get_alignment() == "center") { label->set_relative_position((get_width() - label->get_width()) / 2, (get_height() - 10/*label->get_height()*/) / 2); }						
				if(label->get_alignment() == "right" ) { label->set_relative_position((get_width() - label->get_width())    , 0); }	
                if(label->get_alignment() == "none"  ) {} // default - with this you are free to set the label's relative position to whatever you want // relative_position will always be (0, 0) unless you change it    
                label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // set actual position (No need to call since GUI::on_draw auto fixes child position to parent's)
				// NO need to draw label since child GUI are automatically drawn  // child objects are drawn automatically (via GUI::on_draw()). So DO NOT call draw function!!!
            }
             // Draw multiple labels ... horizontal? vertical?
            for(int l = 0; l < label_list.size(); l++)//for(int l = 1; l <= label_list.size()-1; l++)
            {
                label_list[0]->set_position(label->get_x(), (label->get_y() + label->get_height()) + 2);//if(label ) label_list[0]->set_position(label->get_x(), (label->get_y() + label->get_height()) + 2);
                //if(!label) label_list[0]->set_position(get_x() + label_list[0]->get_relative_x(), get_y() + label_list[0]->get_relative_y());
                // label_list[0] is the default label, so exclude it
                if(l != 0) { dokun::Label * prev = label_list[l-1];    label_list[l]->set_position(prev->get_x(), (prev->get_y() + prev->get_height())); }// set the sub labels position based on label0's position. Don't worry about alignments and whatnot       
                // NO need to draw label since child GUI are automatically drawn (via GUI::on_draw()). So DO NOT call draw function!!!
            }
    /////////////////////////////////////////////////////////////////////////
			// Draw image (goes inside box) *******************************************
            if(image != nullptr)
			{
				// get image size whether its original or resized or scaled
				int image_width  = (image->is_resized() == false) ? image->get_width () : image->get_width_scaled ();
			    int image_height = (image->is_resized() == false) ? image->get_height() : image->get_height_scaled();
                // set image alignment and position relative to GUI	
				if(image->get_alignment() == "left"  ) {image->set_relative_position(0, 0);}					
                if(image->get_alignment() == "center") {image->set_relative_position((get_width() - image_width) / 2, (get_height() - image_height) / 2);}
				if(image->get_alignment() == "right" ) {image->set_relative_position(get_width() - image_width, 0);}
                if(image->get_alignment() == "none"  ) {}
				image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
				// if image is larger than box, scale it to fit box
				if(image_width > get_width () || image_height > get_height()) image->scale_to_fit(get_width(), get_height());// if image is wider than widget, make width equal or if image is taller than widget, make height equal
				// and finally, draw the image ...	
				image->draw(); // Image is not a GUI so you cannot set its parent which means it must be drawn manually
			}
			// Draw multiple images ...
			/////////////////////////////////////////////////////////////////////////
			// Draw separator
			if(separator)
			{
			    for(int i = 0; i < separator_count; i++)
			    {
			        // update separator_width
			        //separator_width = get_width();
			        // update seperator_position (centered x, if horz)
			        //separator_position = Vector2(separator_position.x, separator_position.y);
			        Renderer::draw_line(get_x() + separator_position.x, get_y() + separator_position.y, separator_width, separator_height, 0.0, 1, 1, separator_color.x, separator_color.y, separator_color.z, separator_color.w);
			    }
			}
			/////////////////////////////////////////////////////////////////////////
			// on draw callback (if visible)
	        for(int i = 0; i < Factory::get_gui_factory()->get_size(); i++) { // for(int i = 0; i < child_list.size(); i++) {
			    GUI * gui = static_cast<GUI*>(Factory::get_gui_factory()->get_object(i));//static_cast<GUI*>(child_list[i]);
			    if(gui == nullptr) continue; // skip null objs
			    if(!gui->is_visible()) continue; // skip hidden objs
			    if(gui->get_parent() == this) gui->draw(); // draw all children // no need to check if child is visible since draw() calls already do that
		    }
}
/////////////
void Box::draw_tooltip() {
    if(!is_tooltip()) return; // if this box is a tooltip, draw tooltip
    if(!is_visible()) return;
	if(!is_active()) {}// add shade to color
	if(is_active()) {}  // add tint to color    
    // Draw tooltip
	Renderer::draw_tooltip("Hello", get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, get_color().x, get_color().y, get_color().z, get_color().w, tooltip_arrow_direction, tooltip_arrow_width, tooltip_arrow_height, tooltip_arrow_position/*, tooltip_arrow_color*/);
    // Draw label (goes inside box) **********************************************************
    if(label) // make sure Box has an "initialized" label beforehand (or else engine will crash)
    {   // set label's alignment and position relative to the Tooltip (Box)                     // 10 is the space between the text and the Box's edge
		        if(label->get_alignment() == "left"  ) { label->set_relative_position(0 + 10                                , 0 + 10); }
				if(label->get_alignment() == "center") { label->set_relative_position((get_width() - label->get_width()) / 2, (get_height() - label->get_height()) / 2); }
				if(label->get_alignment() == "right" ) { label->set_relative_position((get_width() - label->get_width())    , 0 + 10); }
                if(label->get_alignment() == "none"  ) {} // default - with this you are free to set the label's relative position to whatever you want // relative_position will always be (0, 0) unless you change it
        label->draw();
    }
    // Draw image (goes inside box) *******************************************
    if(image) // make sure Box has an "initialized" image beforehand (or else engine will crash)
	{   // get image size whether its original or resized or scaled
				int image_width  = (image->is_resized() == false) ? image->get_width () : image->get_width_scaled ();
			    int image_height = (image->is_resized() == false) ? image->get_height() : image->get_height_scaled();
                // set image alignment and position relative to GUI	
				if(image->get_alignment() == "left"  ) {image->set_relative_position(0, 0);}					
                if(image->get_alignment() == "center") {image->set_relative_position((get_width() - image_width) / 2, (get_height() - image_height) / 2);}
				if(image->get_alignment() == "right" ) {image->set_relative_position(get_width() - image_width, 0);}
                if(image->get_alignment() == "none"  ) {}
				image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
				// if image is larger than box, scale to fit
				if(image_width > get_width () || image_height > get_height()) image->scale_to_fit(get_width(), get_height());// if image is wider than widget, make width equal or if image is taller than widget, make height equal
				// and finally, draw the image ...	
		image->draw(); // Image is not a GUI so you cannot set its parent which means it must be drawn manually
	}                 
}
/////////////
void Box::draw_icon() {
    if(!is_icon()) return; // If this widget is an icon, draw image (only) // Boxs can be both a box or an icon
    if(!is_visible()) return;
	if(!is_active()) {}// add shade to color
	if(is_active()) {}  // add tint to color
	if(!image) return;
	image->set_position(get_x(), get_y());//(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
    image->resize(get_size());
	image->draw();
}
/////////////
/////////////
///////////// // create a set_size_preserved() function
void Box::maximize(void)
{
    if(!is_maximized() /*&& !is_iconified()*/) // if not maximized yet
	{
	    old_width  = 200;//get_width ();// // save old size
        old_height = 150;//get_height();		

		set_size(max_width, max_height);//(get_width() * 2, get_height() * 2); // set new size
	    maximized = true;	             // maximized is true
		iconified = false;
		restored = false;                // not restored to default size
	}
}
int Box::maximize(lua_State * L)
{
    return 0;
}
/////////////
void Box::iconify(void) // if maximized, keep maximized while iconified at the same time
{
	if(!is_iconified()) { // if not yet iconified (keep current size, whether maximized or minimized)
	    // hide content
	    if(image) image->set_visible(false);
	    if(label) label->set_visible(false);
	    // set iconify to true
	    restored = false; // not restored to default size
	    iconified = true;
	}
}
int Box::iconify(lua_State * L)
{
    return 0;
}
/////////////
void Box::restore(void)
{
	//********************************
	if(!restored) { // if maximized
	    if(is_iconified()) { // if iconified just put it down dont mess with sizes
	        // show content
	        if(image) image->set_visible(true);
	        if(label) label->set_visible(true);
	        iconified = false; // set iconified to false in order to restore
	        if(is_maximized()) {return;} // restore but in maximized mode
	    }
		if(is_maximized()) {
	        set_size(old_width, old_height); //  restore original size
		    maximized = false;
	        restored  = true; // set restored to true
		}	
	}
}
int Box::restore(lua_State * L)
{
    return 0;
}
/////////////
/* // deallocating box
// delete all labels
for(l = 0; l < label_list.size(); l++) {
    dokun::Label * label = label_list[l];
    delete label;
}
// delete all images
for(i = 0; i < image_list.size(); i++) {
    Image * image = image_list[i];
    delete image;
}
*/
/////////////
/////////////
// SETTERS
/////////////
/////////////
// setters
void Box::set_color(unsigned int red, unsigned int green, unsigned int blue) {
    color = Vector4(red, green, blue, color.w);
}
void Box::set_color(unsigned int red, unsigned int green, unsigned int blue, double alpha) {// if box has been converted to an icon, set its image's color instead//if(String::lower(type) == "icon") { image->set_color(red, green, blue, alpha); }
	color = Vector4(red, green, blue, alpha);
}
void Box::set_color(const Vector3& color) {
	set_color(color.x, color.y, color.z);
}
void Box::set_color(const Vector4& color) {
	set_color(color.x, color.y, color.z, color.w);
}		
int Box::set_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "red");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "green");
		lua_pushvalue(L, 4);
		lua_setfield(L, 1, "blue");
		lua_pushvalue(L, 5);
		lua_setfield(L, 1, "alpha");		
	}
	return 0;
}
void Box::set_alpha(double alpha)
{
	color.w = alpha;
}                                                  
int Box::set_alpha(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_alpha(lua_tonumber(L, 2)); // lua_tonumber(L, ), 
	}
	return 0;	
}
void Box::set_fill(bool fill)
{
	this->fill = fill;
}                                                     
int Box::set_fill(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_fill(lua_toboolean(L, 2));
	}
	return 0;	
}
// outline - is line drawn around border?
void Box::set_outline(bool outline)
{
	this->outline = outline;
}                                                
int Box::set_outline(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_outline(lua_toboolean(L, 2));
	}
	return 0;	
}
void Box::set_outline_width(double width)
{
	outline_width = width;
}                                                    
int Box::set_outline_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_outline_width(lua_tonumber(L, 2));
	}
	return 0;	
}
void Box::set_outline_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	outline_color = Vector4(red, green, blue, alpha);
}                        
void Box::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z, outline_color.w);
}
void Box::set_outline_color(const Vector4& color)
{
	set_outline_color(color.x, color.y, color.z, color.w);
}
int Box::set_outline_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_outline_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
void Box::set_outline_style(int style)
{}                                           
int Box::set_outline_style(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_outline_style(lua_tonumber(L, 2));
	}
	return 0;	
}
void Box::set_outline_antialiased(bool antialiased)
{
	outline_antialiased = antialiased;
}		                           
int Box::set_outline_antialiased(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_outline_antialiased(lua_toboolean(L, 2));
	}
	return 0;	
}
// border - does it have a border(frame)?
void Box::set_border(bool border)
{
	this->border = border;
}                             
int Box::set_border(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_border(lua_toboolean(L , 2));
	}
	return 0;	
}
void Box::set_border_size(int width, int height)
{}                                
int Box::set_border_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_border_size(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;	
}
void Box::set_border_width(int width, int side)
{}                               
int Box::set_border_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_border_width(lua_tonumber(L, 2));
	}
	return 0;	
}
void Box::set_border_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{}                          
int Box::set_border_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_border_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
void Box::set_border_color(const Vector3& color)
{}
void Box::set_border_color(const Vector4& color)
{}
void Box::set_border_style(int style, int side)
{}                               
int Box::set_border_style(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_border_style(lua_tonumber(L, 2));
	}
	return 0;	
}
void Box::set_border_radius(int radius, int side)
{}                              
int Box::set_border_radius(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_border_radius(lua_tonumber(L, 2));
	}
	return 0;	
}
// radius - how round are the edges?
void Box::set_radius(double radius, int side)
{
	this->radius = radius;
}                                      
int Box::set_radius(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_radius(lua_tonumber(L, 2));
	}
	return 0;	
}
void Box::set_trim(double trim, int side)
{}                                   
int Box::set_trim(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_trim(lua_tonumber(L, 2));
	}
	return 0;	
}
// shadow -  does it have a shadow around it?
void Box::set_shadow(bool shadow)
{
	this->shadow = shadow;
}                                         
int Box::set_shadow(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_shadow(lua_toboolean(L, 2));
	}
	return 0;	
}
// gradient - do the colors mix?
void Box::set_gradient(bool gradient)
{
	this->gradient = gradient;
}                                         
int Box::set_gradient(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_gradient(lua_toboolean(L, 2));
	}
	return 0;	
}
void Box::set_gradient_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	gradient_color = Vector4(red, green, blue, alpha);
}
void Box::set_gradient_color(const Vector3& color)
{
	set_gradient_color(color.x, color.y, color.z, gradient_color.w);
}
void Box::set_gradient_color(const Vector4& color)
{
	set_gradient_color(color.x, color.y, color.z, color.w);
}
void Box::set_gradient_color(const Vector3& color0, const Vector3& color1)
{
	color = Vector4(color0, color.w);
	gradient_color = Vector4(color1, gradient_color.w);
}
void Box::set_gradient_color(const Vector4& color0, const Vector4& color1)
{
	color = color0;
	gradient_color = color1;
}

void Box::set_forbidden_area(int x, int y, int width, int height) // set forbidden_area within the gui (relative to the gui)
{
	forbidden_area = Vector4(x, y, width, height);
}
void Box::set_forbidden_area(const Vector4& rect)
{
    set_forbidden_area(rect.x, rect.y, rect.z, rect.w);
}
// title_bar - does it have a title_bar
void Box::set_title_bar(bool title_bar)
{
	this->title_bar = title_bar;
}                                      
int Box::set_title_bar(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar(lua_toboolean(L, 2));
	}
	return 0;	
}
void Box::set_title_bar_size(int height)
{
	title_bar_height = height;
}
int Box::set_title_bar_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_size(lua_tonumber(L, 2)); // lua_tonumber(L, ), 
	}
	return 0;	
}
void Box::set_title_bar_text(const std::string& text)
{
	if(!title_bar_label) throw std::runtime_error("title_bar_label is not initialized");
	title_bar_label->set_string(text);
}                                    
int Box::set_title_bar_text(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_text(lua_tostring(L, 2)); // lua_tostring(L, )
	}
	return 0;	
}
/////////////
void Box::set_title_bar_text_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	if(!title_bar_label) throw std::runtime_error("title_bar_label is not initialized");
	title_bar_label->set_color(red, green, blue, alpha);
}  
/////////////
void Box::set_title_bar_text_color(const Vector3& color)
{
	set_title_bar_text_color(color.x, color.y, color.z, title_bar_text_color.w);
}
/////////////
void Box::set_title_bar_text_color(const Vector4& color)
{
	set_title_bar_text_color(color.x, color.y, color.z, color.w);
}
/////////////
int Box::set_title_bar_text_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_text_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
//////////////
void Box::set_title_bar_label(const dokun::Label& label)
{
	title_bar_label = &const_cast<dokun::Label&>(label);
}
int Box::set_title_bar_label(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
            widget->set_title_bar_label(* label);
           // set in Lua as well
           lua_pushvalue(L, 2);
           lua_setfield(L, 1, "title_bar_label");
        }
    }
    return 0;
}
//////////////
void Box::set_title_bar_image(const Image& image)
{
	title_bar_image = &const_cast<Image&>(image);
}                                  
int Box::set_title_bar_image(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		    widget->set_title_bar_icon(*image);
           // set in Lua as well
           lua_pushvalue(L, 2);
           lua_setfield(L, 1, "title_bar_image");
	    }
	}
	return 0;	
}
void Box::set_title_bar_icon(const Image& icon)
{
	set_title_bar_image(icon);
}
void Box::set_title_bar_color(unsigned int red, unsigned int green, unsigned int blue, double alpha)
{
	title_bar_color = Vector4(red, green, blue, alpha);
}
void Box::set_title_bar_color(const Vector3& color)
{
	set_title_bar_color(color.x, color.y, color.z, title_bar_color.w);
}
void Box::set_title_bar_color(const Vector4& color)
{
	set_title_bar_color(color.x, color.y, color.z, color.w);
}
int Box::set_title_bar_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
void Box::set_title_bar_button_iconify(bool button_iconify)
{
	this->title_bar_button_iconify = button_iconify;
}                     
int Box::set_title_bar_button_iconify(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_button_iconify(lua_toboolean(L, 2));
	}
	return 0;	
}
void Box::set_title_bar_button_maximize(bool button_maximize)
{
	this->title_bar_button_maximize = button_maximize;
}                    
int Box::set_title_bar_button_maximize(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_button_maximize(lua_toboolean(L, 2));
	}
	return 0;	
}
void Box::set_title_bar_button_close(bool button_close)
{
	this->title_bar_button_close = button_close;
}                       
int Box::set_title_bar_button_close(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_title_bar_button_close(lua_toboolean(L, 2));
	}
	return 0;	
}
/////////////
/////////////
// icon
void Box::set_as_icon(bool icon)
{
    // adjust size to match it's image size before converting it to an icon
    //if(icon) set_size(image->get_width(), image->get_height());// also, change the color to the image's color//set_color(image->get_color());
	type = (icon == true) ? "icon" : "box";
}
void Box::set_image(const Image& image) // images are not GUIs so they do not require a parent
{
	this->image = &const_cast<Image&>(image); // add image to box 
}
int Box::set_image(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		    widget->set_image(*image);
			// set in Lua as well
			lua_pushvalue(L, 2);
			lua_setfield(L, 1, "image");
	    }
	}
	return 0;	
}
void Box::set_image_list(const Image& image)
{
    image_list.push_back(&const_cast<Image&>(image)); // store image in image_list (first image set will ALWAYS be in index 0)
}
int Box::set_image_list(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		    widget->set_image(*image);
			// set in Lua as well
            lua_pushvalue(L, 2);
			lua_setfield(L, 1, std::string( std::string("image") + std::to_string((int)widget->image_list.size()) ).c_str());
	    }
	}
	return 0;
}
/////////////
/////////////
// label		
void Box::set_text(const std::string& text)
{
    if(!label) throw std::runtime_error("label is not initialized");
	label->set_string(text);
}
int Box::set_text(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		widget->set_text(lua_tostring(L, 2));	
	}
	return 0;
}
/////////////
void Box::set_label(const dokun::Label& label) // Labels are GUI elements so they are drawn automatically once a parent is set
{
    //const_cast<dokun::Label&>(label).set_color(0, 0, 0, 1.0);
	this->label = &const_cast<dokun::Label&>(label); // add label to box
	this->label->set_parent(* this); // set parent to Box
} 
int Box::set_label(lua_State * L)	
{
	luaL_checktype(L, 1, LUA_TTABLE); // box
	luaL_checktype(L, 2, LUA_TTABLE); // label
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		    widget->set_label(*label);
			// set in Lua as well
			lua_pushvalue(L, 2); // label
			lua_setfield(L, 1, "label"); // box.label = label
			// set Box as parent for dokun::Label
			lua_pushvalue(L, 1); // box
			lua_setfield(L, 2, "parent"); // label.parent = box
	    }
	}
    return 0;	
}	
void Box::set_label_list(const dokun::Label& label) // Labels are GUI elements so they are drawn automatically once a parent is set
{
    label_list.push_back(&const_cast<dokun::Label&>(label)); // store label in label_list (first label set will always be in index 0)
    label_list[label_list.size()-1]->set_parent(* this); // set Box as parent - C++ indexes start from 0
}
int Box::set_label_list(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // box
	luaL_checktype(L, 2, LUA_TTABLE); // label
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		dokun::Label * label = *static_cast<dokun::Label **>(lua_touserdata(L, -1));
        lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		    widget->set_label_list(*label);
            // set in Lua
            lua_pushvalue(L, 2);
            lua_setfield(L, 1, std::string( std::string("label") + std::to_string((int)widget->label_list.size()) ).c_str()); // box.label1, box.label2, box.label3, and so on ...        // lua_getfield(L, 1, "label_list");if(!lua_istable(L, -1)){lua_newtable(L); lua_setfield(L, 1, "label_list");} // check is box.label_list exists. If not create it
            // also create a table of labels
            // get global label_list
            lua_getglobal (L, "label_list");
            if(lua_istable(L, -1)) {
            	#ifdef DOKUN_LUA51
			    int size = lua_objlen(L, -1);
			    #endif
			    #ifndef DOKUN_LUA51
			    int size = luaL_len(L, -1);
			    #endif
			    lua_pushvalue(L, 2); // push label
			    lua_rawseti (L, -2, size + 1); // store label in label_list
			    return 0;
            }
            if(lua_isnil(L, -1)) // if label_list is nil
            {
                lua_pop(L, 1); // pop nil value
                lua_newtable (L); // create it
                lua_pushvalue(L, 2); // push label
                lua_rawseti (L, -2, 1); // store label in label_list
                lua_setglobal (L, "label_list"); // set the global name
                lua_getglobal(L, "label_list");
                lua_setfield(L, 1, "label_list");
                return 0;
            }
        }
    }    
    return 0;
}
/////////////
/////////////
void Box::set_alignment(const std::string& alignment)
{
	this->alignment = alignment;
}
int Box::set_alignment(lua_State * L)
{
    return 0;
}
/////////////
// tooltip
/////////////
void Box::set_as_tooltip(bool tooltip)
{
	type = (tooltip == true) ? "tooltip" : "box";
	color.w = 0.6;
}
/////////////
int Box::set_as_tooltip(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // box
    luaL_checktype(L, 2, LUA_TBOOLEAN); // boolean
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Box * box = *static_cast<Box **>(lua_touserdata(L, -1));
	    box->set_as_tooltip(lua_toboolean(L, 2));
	}
	return 0;
}
/////////////
/////////////
// GETTERS
/////////////
/////////////
Vector4 Box::get_color() const
{
    //if(String::lower(type) == "icon") return image->get_color(); // return the image's color instead if box has been converted to an icon
	return color;
} 
int Box::get_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // widget
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Box * widget = *static_cast<Box **>(lua_touserdata(L, -1));
		lua_pushinteger(L, widget->get_color().x);
		lua_pushinteger(L, widget->get_color().y);
		lua_pushinteger(L, widget->get_color().z);
		lua_pushinteger(L, widget->get_color().w);
		return 4;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
//////////////
/*double Box::get_alpha()
{
    return color.w;
}*/
//////////////
GUI * Box::get_child(unsigned int index)const {
    if(index > (child_list.size()-1)) throw std::runtime_error("attempt to access invalid index at Box::child_list");
    return child_list[index];
}
//////////////
//////////////
//////////////
//////////////
Image * Box::get_image() const
{
	return image;
}
int Box::get_image(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "image");
	return 1;
}
Image * Box::get_image_list(int index) const
{
	return image_list[index];
}
int Box::get_image_list(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // box (arg:1)
    luaL_checktype(L, 2, LUA_TNUMBER); // index (arg:2)
    lua_getfield(L, 1, "image_list"); // image_list (arg:3)
    if(lua_istable(L, -1))
    {
	    lua_rawgeti(L, -1, lua_tointeger(L, 2));
        return 1;
    }
    lua_pushnil(L);
	return 1;	
}
/////////////
dokun::Label * Box::get_label() const
{
	return label;
}
int Box::get_label(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "label");
	return 1;	
}
dokun::Label * Box::get_label_list(int index) const
{
	return label_list[index];
}
int Box::get_label_list(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // box (arg:1)
    luaL_checktype(L, 2, LUA_TNUMBER); // index (arg:2)
    lua_getfield(L, 1, "label_list"); // label_list (arg:3)
    if(lua_istable(L, -1))
    {
	    lua_rawgeti(L, -1, lua_tointeger(L, 2));
        return 1;
    }
    lua_pushnil(L);
	return 1;	
}
/////////////
std::string Box::get_text() const
{
	return get_label()->get_string();
}
/////////////
Vector2 Box::get_title_bar_position() const
{
	double x = 0                 + get_x();
	double y = -title_bar_height + get_y();
	return Vector2(x, y);
}
Vector2 Box::get_title_bar_size() const
{
	return Vector2(get_width(), title_bar_height);
}
/////////////
Vector4 Box::get_title_bar_color() const
{
	return title_bar_color;
}
/////////////
dokun::Label * Box::get_title_bar_label() const
{
    return title_bar_label;
}
/////////////
std::string Box::get_title_bar_text() const
{
	if(!title_bar_label) throw std::runtime_error("title_bar_label is not initialized"); //return "";       // return empty string
	return title_bar_label->get_string();//title_bar_text;
}
/////////////
Vector4 Box::get_title_bar_text_color() const
{
	if(!title_bar_label) throw std::runtime_error("title_bar_label is not initialized");// title_bar_label is null//    return Vector4(255, 255, 255, 1.0);
	return title_bar_label->get_color();
}
/////////////
Image * Box::get_title_bar_icon() const
{
	return title_bar_image;
}
/////////////
/////////////
Vector2 Box::get_title_bar_button_iconify_position()const // 3
{
	double x = get_x() + 0                 + (get_title_bar_size().x - (get_title_bar_button_iconify_size().x * 3)) - (5 * 3);
	double y = get_y() + -title_bar_height + (get_title_bar_size().y -  get_title_bar_button_iconify_size().y) / 2;	
	return Vector2(x, y);	
}
Vector2 Box::get_title_bar_button_iconify_size()const
{
	return Vector2(title_bar_button_width, title_bar_height / 2);	
}
/////////////
Vector2 Box::get_title_bar_button_maximize_position()const // 2
{
	double x = get_x() + 0                 + (get_title_bar_size().x - (get_title_bar_button_maximize_size().x * 2)) - (5 * 2);
	double y = get_y() + -title_bar_height + (get_title_bar_size().y -  get_title_bar_button_maximize_size().y) / 2;	
	return Vector2(x, y);	
}
Vector2 Box::get_title_bar_button_maximize_size()const
{
	return Vector2(title_bar_button_width, title_bar_height / 2);	
}
/////////////
Vector2 Box::get_title_bar_button_close_position()const // 1
{
	double x = get_x() + 0                 + (get_title_bar_size().x - get_title_bar_button_close_size().x) - 5;
	double y = get_y() + -title_bar_height + (get_title_bar_size().y - get_title_bar_button_close_size().y) / 2;
	return Vector2(x, y);
}
Vector2 Box::get_title_bar_button_close_size()const
{
	return Vector2(title_bar_button_width, title_bar_height / 2);
}
Vector4 Box::get_title_bar_button_close_color()const
{
	return title_bar_button_close_color;
}
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
double Box::get_radius() const
{
	return radius;
}
int Box::get_radius(lua_State * L)
{
	return 1;
}
/////////////
/////////////
std::string Box::get_alignment()const
{
	return alignment;
}
int Box::get_alignment(lua_State * L)
{
	return 1;
}
/////////////
/////////////
// BOOLEAN
/////////////
bool Box::is_box  ()const
{
	return (String::lower(type) == "box");
}
///////////// 
int Box::is_box(lua_State * L)
{
	return 1;
}
/////////////
bool Box::is_icon ()const
{
	return (String::lower(type) == "icon");
}
/////////////
int Box::is_icon(lua_State * L)
{
	return 1;
}
/////////////
bool Box::is_tooltip()const
{
    return (String::lower(type) == "tooltip");
}
/////////////
int Box::is_tooltip(lua_State * L)
{
    return 1;
}
/////////////
bool Box::has_title_bar()const
{
	return (title_bar == true);
} 
/////////////
int Box::has_title_bar(lua_State * L)
{
	return 1;
}
/////////////
bool Box::has_outline()const
{
	return (outline == true);
}   
/////////////
int Box::has_outline(lua_State * L)
{
	return 1;
}
/////////////
bool Box::has_border()const
{
	return (border == true);
} 
/////////////
int Box::has_border(lua_State * L)
{
	return 1;
}
/////////////
bool Box::has_shadow()const
{
	return (shadow == true);
}
/////////////
int Box::has_shadow(lua_State * L)
{
	return 1;
}
/////////////
bool Box::has_gradient()const
{
	return (gradient == true);
}  
/////////////
int Box::has_gradient(lua_State * L)
{
	return 1;
}
/////////////
bool Box::is_maximized()const
{
	return (maximized == true);
}
/////////////
int Box::is_maximized(lua_State * L)
{
	return 1;
}
/////////////
bool Box::is_iconified()const
{
	return (iconified == true);
}
/////////////
int Box::is_iconified(lua_State * L)
{
	return 1;
}
/////////////
bool Box::is_restored()const
{
	return (restored == true);
}
/////////////
int Box::is_restored(lua_State * L)
{
	return 1;
}
/////////////
/////////////
void Box::on_titlebar(void)
{
	if(!has_title_bar()) return; // if no titlebar, exit function
	// iconify button (on titlebar)
	on_titlebar_button_iconify();
    // maximize button (on titlebar)
	on_titlebar_button_maximize();
	// close button (on titlebar)
	on_titlebar_button_close();
}
/////////////
void Box::on_titlebar_button_close(void) {
    if(!has_title_bar()) return; // if no titlebar, exit function
    if(!title_bar_button_close) return; // if no titlebar close button, exit function
	// on mouse hover, change close button color
    if(Mouse::is_over(get_title_bar_button_close_position(), get_title_bar_button_close_size())) {
        title_bar_button_close_color = Vector4(255, 51, 51, 1.0); // 227,38,54
        // hide if close button is pressed
        if(Mouse::is_pressed(1)) set_visible(false);
    }
	else {
		title_bar_button_close_color.x = title_bar_color.x + (255 - title_bar_color.x) * 0.05;/*tint_factor*/;// make it a tint of title_bar_color //color;
		title_bar_button_close_color.y = title_bar_color.y + (255 - title_bar_color.y) * 0.05;/*tint_factor*/;
		title_bar_button_close_color.z = title_bar_color.z + (255 - title_bar_color.z) * 0.05;/*tint_factor*/;
		//title_bar_button_close_color.w = title_bar_color.w;
	}
}
/////////////
void Box::on_titlebar_button_maximize(void) {
    if(!has_title_bar()) return; // if no titlebar, exit function
    if(!title_bar_button_maximize) return; // if no titlebar maximize button, exit function    
	// on maximize button pressed, either restore or maximize box
	if(Mouse::is_over(get_title_bar_button_maximize_position(), get_title_bar_button_maximize_size())) {
		if(Mouse::is_pressed(1)) {
            if(!restored) restore(); 
            else maximize();
		}
	}
}
/////////////
void Box::on_titlebar_button_iconify(void) {
    if(!has_title_bar()) return; // if no titlebar, exit function
    if(!title_bar_button_iconify) return; // if no titlebar iconify button, exit function    
	// on iconify button pressed, iconify box
	if(Mouse::is_over(get_title_bar_button_iconify_position(), get_title_bar_button_iconify_size())) {
	    if(Mouse::is_pressed(1)) iconify();
	}
}
/////////////
/////////////
void Box::on_drag(void)
{
	if(!is_draggable()) return;
	dokun::Window * window = dokun::Window::get_active();
	if(!window) return;
	if(has_title_bar()) // has title_bar (drag by title_bar)
	{
	    double mouse_x_relative_to_title_bar = fabs(get_title_bar_position().x - Mouse::get_position(*window).x);
	    double mouse_y_relative_to_title_bar = fabs(get_title_bar_position().y - Mouse::get_position(*window).y);
	    //std::cout << "mouse_x_titlebar: " << mouse_x_relative_to_title_bar << std::endl;
	    //std::cout << "mouse_y_titlebar: " << mouse_y_relative_to_title_bar << std::endl;
	    bool captured = false; // captured pos
		if(Mouse::is_over(get_title_bar_position(), get_title_bar_size()))
		{
			dragged = true; // can drag
			if(title_bar_button_close)
			{
			    if(Mouse::is_over(get_title_bar_button_close_position   (), get_title_bar_button_close_size   ()))
				    dragged = false;
		    }
		    if(title_bar_button_iconify)
			{
			    if(Mouse::is_over(get_title_bar_button_iconify_position (), get_title_bar_button_iconify_size ()))
				    dragged = false;
			}
            if(title_bar_button_maximize)
			{				
		        if(Mouse::is_over(get_title_bar_button_maximize_position(), get_title_bar_button_maximize_size()))
			        dragged = false;
		    }
		}
		if(Mouse::is_pressed(1) && dragged && !captured)
		{
		    // capture position where mouse was pressed at
		    //const int mouse_press_position_x_const = Mouse::get_position(*window).x;
		    //const int mouse_press_position_y_const = Mouse::get_position(*window).y;
		    //std::cout << "mouse_position_pressed: " <<  Vector2(mouse_press_position_x_const, mouse_press_position_y_const) << std::endl;
		    captured = true;
		    //const int old_title_bar_x = get_title_bar_position().x;
		    // mouse_position - position_that_was_pressed   -    Mouse::get_position(*window).x - (get_title_bar_size().x - mouse_press_position_x_const)
			set_position(Mouse::get_position(*window).x - get_title_bar_size().x / 2, Mouse::get_position(*window).y + (get_title_bar_size().y / 2)); // y stays the same, x changes
		}		
		if(Mouse::is_released(1)) // on release
		{
			dragged = false;
			captured = false;
			////set_position(get_position());
		}		
	}
	if(!has_title_bar()) // no title_bar
	{
	    double mouse_x_relative_to_box = fabs(get_x() - Mouse::get_position(*window).x);
	    double mouse_y_relative_to_box = fabs(get_y() - Mouse::get_position(*window).y);
	    
		if(Mouse::is_over(get_x(), get_y(), get_width(), 10))//get_width() - forbidden_area.z, get_height() - forbidden_area.w))
		{
			dragged = true;
		}/*
		if(Mouse::is_over(forbidden_area))
		{
			dragged = false;
		}*/
		if(Mouse::is_pressed(1) && dragged)
		{
			set_position(Mouse::get_position(*window).x - get_size().x / 2,//Mouse::get_position(*window).x - (get_size().x + get_x()),
			    Mouse::get_position(*window).y - get_size().y / 2);
		}
		if(Mouse::is_released(1)) // on release
		{
			dragged = false;
			set_position(get_position());
		}
	}
}
void Box::on_resize(void)
{
	if(!is_resizeable())
        return;
	dokun::Window * window = dokun::Window::get_active();
	if(window == nullptr)
		return;	
    if(Mouse::is_over(0,0)) // mouse over side edges
	{
		// change mouse cursor
		// WINDOWS CURSORS
		// IDC_SIZENESW , IDC_SIZENS , IDC_SIZENWSE , IDC_SIZEWE , 
		// LINUX CURSORS
		// XC_right_side , XC_bottom_right_corner , XC_top_right_corner
		// XC_left_side , XC_bottom_left_corner , XC_top_left_corner
		// XC_bottom_side
		// XC_top_side
	#ifdef DOKUN_WIN32
		HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
		//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)cursor);
	#endif
	#ifdef __gnu_linux__	
	#ifdef DOKUN_X11
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_top_left_corner); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
		XFreeCursor(window->get_display(), cursor);
	#endif			
	#endif						
	} 
	// restore original cursor
	else {
	#ifdef DOKUN_WIN32
		HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
		//SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)cursor);
	#endif
	#ifdef __gnu_linux__	
	#ifdef DOKUN_X11
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_left_ptr); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
		XFreeCursor(window->get_display(), cursor);
	#endif
	#endif
	}	
}		
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
