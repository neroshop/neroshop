// filename: address.hpp - part of "neroshop" project
class Address {
    // normal
    void generate_address(); // generates a sub address from main account
    void explore(const std::string& address); // will detect address before opening explorer
    void show_qr_code(); // shows the address qrcode
    void show_qr_code(unsigned int index); // shows a subaddress qrcode    
    // setters
    // getters
    std::string get_string() const; // returns address string
    std::string get_string(unsigned int subaddress_index) const; // returns subaddress string
    Image * get_qr_code() const; // returns address qrcode
    Image * get_qr_code(unsigned int subaddress_index) const; // returns the qrcode of the subaddress at "index"
private:
    std::string detect() const; // detects what coin the address belongs to; return string "coin"
};
