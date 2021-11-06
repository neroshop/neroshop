#include "../include/slider.hpp"

Slider::Slider() : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 255), background_color(160, 160, 160, 255),
    ball_size(10), ball_color(32, 32, 32, 255), ball_radius(50), label(nullptr),
	// outline
	outline (false),
    outline_color(0, 0, 0, 255),
    outline_width(1.0),
    outline_antialiased(false),
    // gradient
    gradient(false),
	gradient_color(background_color) // gradient.color1 = backgound_color, gradient.color0 = foreground_color
{
	set_position(0, 0);
	set_size(200, 20);
	set_orientation(0);
}
/////////////            
Slider::Slider(int x, int y) : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 255), background_color(160, 160, 160, 255),
    ball_size(10), ball_color(32, 32, 32, 255), ball_radius(50), label(nullptr),
	// outline
	outline (false),
    outline_color(0, 0, 0, 255),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(x, y);
	set_size(200, 20);
	set_orientation(0);
}
/////////////
Slider::Slider(int x, int y, int width, int height) : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 255), background_color(160, 160, 160, 255),
    ball_size(10), ball_color(32, 32, 32, 255), ball_radius(50), label(nullptr),
	// outline
	outline (false),
    outline_color(0, 0, 0, 255),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
}
/////////////
Slider::~Slider()
{}
/////////////		
void Slider::draw()
{
	if(is_visible())  // is it visible? 
	{
		if(!is_active()) // not disabled?
		{}		
		if(is_active()) // is it disabled?
		{}
	    double min_val = get_range().x;
	    double max_val = get_range().y;
		double value = get_value();
	    double x = get_position().x;
		double y = get_position().y;
		double angle = get_angle();
		double scale_x = get_scale().x;
		double scale_y = get_scale().y;
		int width  = get_width();
		int height = get_height();
        int red    = get_color().x;
        int green  = get_color().y;
        int blue   = get_color().z;		
		int alpha  = get_color().w;
		// mouse over slider
		//WINDOW * window = WINDOW::get_active();
        if(Mouse::is_over(get_ball_x(), get_ball_y(), get_ball_width(), get_ball_height()))
		{
            std::cout << "Mouse over ball\n";
			if(Mouse::is_pressed(1)) {
			    //if(!window) return;
			    //int x = fabs(round((double)Mouse::get_position(*window).x - (double)(get_x() + get_ball_x())));
			    set_value(get_value() + 2 * 1 / 1); // ?? not sure how to increment it with mouse movement//set_value(get_value() + x);
			    // if mouse is moved, slide it to the direction the mouse is moving to ...
			}
		}
		// Draw slider				
		if(get_orientation() == 0) { // horizontal
		    int ball_width = ball_size;
		    Renderer::draw_slider(x, y, width, height, 0, scale_x, scale_y, red, green, blue, alpha,
			// beam properties
			    min_val, max_val, value, background_color,
			// ball properties
			    ball_width, ball_color
			// outline
			    );
		}
		if(get_orientation() != 0) { // vertical 
		    int ball_height = ball_size;
		    Renderer::draw_slider_vertical(x, y, width, height, 0, scale_x, scale_y, red, green, blue, alpha,
			    min_val, max_val, value, background_color, ball_height, ball_color);
		}	
		// Label
		if(label) // as long as label is not nullptr. Does not matter if text is empty, still need to set proper x and y positions
		{
            // set label_position relative to progressbar_position
			if(label->get_alignment() == "left"  ) {label->set_relative_position(0                                     , (get_height() - 10/*label->get_height()*/) / 2);} // keep label_y positioned at center of progressbar at all times
			if(label->get_alignment() == "center") {label->set_relative_position((get_width() - label->get_width()) / 2, (get_height() - 10/*label->get_height()*/) / 2);} // keep label_y positioned at center of progressbar at all times						
			if(label->get_alignment() == "right" ) {label->set_relative_position( get_width() - label->get_width()     , (get_height() - 10/*label->get_height()*/) / 2);} // keep label_y positioned at center of progressbar at all times	
            if(label->get_alignment() == "none"  ) {}
			label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y());
            // NO need to draw label since child GUI are automatically drawn //label->draw();//Renderer::draw_label(get_text(), x, y, angle, 0.5, 0.5, get_label()->get_font()->get_data(), get_label()->get_color().x, get_label()->get_color().y, get_label()->get_color().z, get_label()->get_color().w);
		}		
	}
    on_draw(); // callback for all gui	
}    
/////////////
void Slider::draw(double x, double y)
{
    set_position(x, y);
    draw();
}
/////////////
void Slider::draw(const Vector2& position)
{
    set_position(position.x, position.y);
    draw();
}
///////////// 
int Slider::draw(lua_State *L)
{
	return 0;
}
/////////////
void Slider::reset()
{
    value = get_minimum_value();//0;
}
/////////////
int Slider::reset(lua_State *L)
{
	return 0;
}
/////////////
/////////////
// SETTERS
/////////////		
void Slider::set_color(int red, int green, int blue, int alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}  
/////////////
int Slider::set_color(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_range(double max, double min)
{
	range = Vector2(min, max);
} 
/////////////
int Slider::set_range(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_minimum_value(double minimum_value)
{
	range.x = minimum_value;
} 
/////////////
int Slider::set_minimum_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_maximum_value(double maximum_value)
{
    range.y = maximum_value;
} 
/////////////
int Slider::set_maximum_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_value(double value)
{
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value	
	this->value = value;
} 
/////////////
int Slider::set_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_ball_color(int layer, int red, int green, int blue)
{
	switch(layer)
	{
		case 0: // inner
		break;
		case 1: // outer
		break;
	}
}
/////////////
int Slider::set_ball_color(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_ball_inner_color(int red, int green, int blue, int alpha)
{}  
/////////////
int Slider::set_ball_inner_color(lua_State *L)
{
	return 0;
}// ball and beam parts of slider
/////////////
void Slider::set_ball_outer_color(int red, int green, int blue, int alpha)
{}  
/////////////
int Slider::set_ball_outer_color(lua_State *L)
{
	return 0;
}// ball and beam parts of slider
/////////////
void Slider::set_ball_size(int ball_size)
{
	this->ball_size = ball_size;
}
/////////////
void Slider::set_radius(double radius)
{
	//(this)->radius = radius;
}  
/////////////
int Slider::set_radius(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_label(const Label& label)
{
    this->label = &const_cast<Label&>(label);
    this->label->set_parent(* this);
}
/////////////
int Slider::set_label(lua_State *L)
{
    return 0;
}
/////////////
/////////////
// GETTERS
/////////////
double Slider::get_value() const
{
	return value;
} 
/////////////
int Slider::get_value(lua_State *L)
{
	return 1;
}
/////////////
Vector2 Slider::get_range() const
{
	return range;
} 
/////////////
int Slider::get_range(lua_State *L)
{
	return 2;
}
/////////////
double Slider::get_minimum_value() const
{
	return range.x;
} 
/////////////
int Slider::get_minimum_value(lua_State *L)
{
	return 1;
}
/////////////
double Slider::get_maximum_value() const
{
	return range.y;
} 
/////////////
int Slider::get_maximum_value(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Slider::get_color() const
{
	return foreground_color;
}
int Slider::get_color(lua_State *L)
{
	return 1;
}
/////////////
Label * Slider::get_label() const
{
    return label;
}
/////////////
int Slider::get_label(lua_State *L)
{
    return 1;
}
/////////////
/////////////
/////////////
/////////////
int Slider::get_ball_x()
{
    if(get_orientation() != 0) return get_x() - (get_width() - get_width() / 2) + 0; // vertical
	return get_x() + (get_value() / get_maximum_value()) * static_cast<float>(get_width()); // horizontal (default)
}
int Slider::get_ball_x(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_y() // should return the ball's actual (real) y_position // y=-5, h=1.5  or  y=-10, h=2
{
    if(get_orientation() != 0) return get_y() + (get_value() / get_maximum_value()) * static_cast<float>(get_height()); // vertical
	return get_y() - (get_height() - get_height() / 2) + 0;//get_y() + 1; // horizontal (default)
}
int Slider::get_ball_y(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_width()
{
    if(get_orientation() != 0) return (get_width() * 2) - 0; // vertical
	return ball_size; // horizontal (default)
}
int Slider::get_ball_width(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_height() // ball_height is half of double the beam's height (height x 1.5) minus the bottom_padding (2) // y=-5, h=1.5  or  y=-10, h=2
{
    if(get_orientation() != 0) return ball_size; // vertical
	return (get_height() * 2) - 0;//get_height() - 2; // horizontal (default)
}
int Slider::get_ball_height(lua_State *L)
{
	return 1;
}
/////////////
/////////////
/////////////
/////////////
/////////////
// BOOLEAN
/////////////		
bool Slider::is_moved()
{
	return false;
} 
int Slider::is_moved(lua_State *L)
{
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/////////////
int Slider::new_(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}
