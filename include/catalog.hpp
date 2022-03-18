// filename: catalog.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef CATALOG_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define CATALOG_HPP_NEROSHOP
// neroshop
#include "debug.hpp"
#include "db.hpp"
#include "icon.hpp"
// dokun-ui
#include <grid.hpp>
// STL
#include <iostream>
#include <memory> // std::shared_ptr, std::make_shared

namespace neroshop {
class Catalog {
public:
    Catalog();//Catalog(int rows, int columns);
    ~Catalog();
    void draw();
    void draw(int x, int y);
    void center(int window_width, int window_height); // centers the current product page
    // setters
    //void set_columns(int columns); // this will be made private. 1 col = list_view | 2+ cols = grid_view
    void set_rows(int rows);
    void set_position(int x, int y);
    // for each individual boxes
    void set_box_width(int box_width);
    void set_box_height(int box_height);
    void set_box_size(int box_width, int box_height);
    void set_box_size(const Vector2i& box_size);
    // getters
    Grid * get_grid() const; // contains all the boxes
    Box * get_box(int row, int column) const;
    Box * get_current() const; // returns current page
    int get_x() const;
    int get_y() const;
    Vector2i get_position() const;
    int get_width() const;
    int get_height() const;
    Vector2i get_size() const;
    //get_capacity(); // number of boxes that can be lined up horizontally
    //get_row_capacity(); // number of boxes that can be lined up vertically
//private:
    std::shared_ptr<Box> current; // shows product_page (you can only view one product page at a time) // make this static
    std::shared_ptr<Grid> view; // shows catalog_page (item listings)
    void initialize();
    void update(); // updates size and width of boxes
    // box contents
    void add_contents(int box_index); // adds content to product page
    void populate(); // fills / populates category view with items in inventory
    //populate_by_category, populate_by_best_seller (check table order_item -> item_id)
    // populate_by_latest,  populate_by_best_deals_and_promo
    // we can show featured items, best sellers, 
    // product data
    void fetch_inventory();
    /*void add_product_image();
    void add_product_price();
    void add_product_stars(); // star ratings
    void add_product_star_label();
    void add_product_quantity_spinner();
    void add_heart(); // favorite or wishlist
    void add_cart_button(); // add-to-cart button*/
    // views: list_view (1 column, multiple rows), grid_view (multiple rows, multiple columns)
};
}
#endif
// https://github.com/kyliau/ShoppingCart/blob/master/Catalog.cpp
