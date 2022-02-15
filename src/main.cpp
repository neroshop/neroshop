#include <iostream>
// neroshop
#include "../include/neroshop.hpp" //#include "../include/wallet.hpp" // aparrently this file is causing the "error: unknown type name"
using namespace neroshop; // namespace comes after including the header files
// dokun
#include <build.hpp>
#include DOKUN_HEADER
using namespace dokun;
// ----------------------
// stagenet faucets: https://melo.tools/faucet/stagenet/
// https://stagenet-faucet.xmr-tw.org/
// code formatter: https://codebeautify.org/cpp-formatter-beautifier
// api: https://moneroecosystem.org/monero-cpp/annotated.html
// https://stackoverflow.com/questions/30017397/error-curl-usr-local-lib-libcurl-so-4-no-version-information-available-requ
// icons taken from: https://www.iconsdb.com/white-icons/
////////////////////
// neromon (daemon-server) functions
Process * server_process;
void start_neromon_server() {
    // on launching neroshop, start the neromon process, if it has not yet been started    
    int neromon = Process::get_process_by_name("neromon");
    if(neromon != -1) {
        neroshop::print("neromon is already running in the background", 4);
        return;
    }
    server_process = new Process(); // don't forget to delete this!
    server_process->create("./neromon", "");
    // show all processes
    Process::show_processes();
}
////////////////////
//DB2 db2;
void connect_database() { // this function is just for testing purposes
    DB2::get_singleton()->/*db2.*/connect("host=127.0.0.1 port=5432 user=postgres password=postgres dbname=neroshoptest");// dbname=mydb");//("host=localhost port=1234 dbname=mydb connect_timeout=10");//("");//("user=sid dbname=neroshoptest");
    // psql -h localhost -p 5432 -U postgres -d neroshoptest       // or psql neroshoptest
    // Password for user postgres: postgres
    // ctrl + z to exit
    /*std::cout << ": " << << std::endl;
    std::cout << ": " << << std::endl;*/
    DB2::get_singleton()->/*db2.*/print_database_info();   
    ////////////
    // testing
    /*if(!db2.table_exists("users")) {
        db2.create_table("users");
        db2.add_column("users", "name", "text");
        db2.alter_table("users", "ADD", "pw_hash", "text");
        db2.add_column("users", "opt_email"); // will be NULL since datatype is not specified
        // make all "name" unique
        db2.create_index("idx_users_name", "users", "name");
        db2.create_index("idx_users_email", "users", "opt_email");
    }*/
    //db2.insert_into("users", "name, pw_hash, opt_email", "'dude', '$2a$05$bvIG6Nmid91Mu9RcmmWZfO5HJIMCT8riNW0hEp8f6/FuA2/mHZFpe', 'null'"); // libpq does not allow double quotes :(
    //db2.insert_into("users", "name, pw_hash", "'mike', 'blank'");
    //db2.insert_into("users", "name, pw_hash", DB2::to_psql_string("sid's bish") + "," + DB2::to_psql_string("nothing to see here"));
    //db2.update("users", "name", DB2::to_psql_string("nameless"), "id = 1");
    ////db2.execute("DROP DATABASE neroshoptest;");
    //////////////
    ////std::cout << db2.get_row_count("users") << " (row count for table users)" << std::endl;
    // get last record from table users (most recent, even if rowids are not in the correct numerical order from smallest to largest)
    ////std::cout << db2.get_last_id("users") << " (lastest row)" << std::endl;
    //////////////
    ////std::cout << db2.get_text("SELECT VERSION()") << std::endl; // PostgreSQL 14.1 (Ubuntu 14.1-2.pgdg20.04+1) on x86_64-pc-linux-gnu, compiled by gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0, 64-bit
    /////////////
    std::cout << DB2::get_singleton()->/*db2.*/get_integer("SELECT sum(numbackends) FROM pg_stat_database;") << " (number of connections - including this app)\n";
    //db2.execute_params("INSERT INTO users (name, pw_hash, opt_email) VALUES ($1, $2, $3)", {});
    //std::cout << "column_exists: " << db2.column_exists("users","role") << "\n";//get_text("SELECT name FROM users WHERE name = 'jack'") << std::endl;
    //db2.drop_table("users");
    ////std::cout << db2.get_text("SELECT sum(numbackends) FROM pg_stat_database;") << " (?)" << std::endl;
    //delete_from("users", "id=1");
    ////std::cout << db2.get_integer("WITH deleted AS (DELETE FROM users WHERE id=1 IS TRUE RETURNING *) SELECT count(*) FROM deleted;") << " (deleted_records_count)" << std::endl;
    ////std::cout << db2.get_text("SELECT name FROM users WHERE id=4;") << std::endl;
    /*db2.truncate("users");
    db2.vacuum("FULL VERBOSE users");*/
    //std::cout << db2.table_exists("users") << " (table_exists)" << std::endl;
    /////////////
    // executing select does not show any output in PostgreSQL like it does with SQLite
    //std::cout << "please select do something: \n";
    //db2.execute_return("SELECT * FROM users;");
    //$ psql neroshoptest -c"SELECT * FROM users;"
    ////////////
    //db2.x();
    ////////////
    // getting multiple rows of data
    /*PGresult *res = PQexec(db2.get_handle(), "SELECT name FROM item");//"SELECT * FROM item LIMIT 5");    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        //do_exit(conn);
        db2.finish();
        exit(1);
    }
    int rows = PQntuples(res);
    for(int i=0; i<rows; i++) {
        // items_weight += quantity * PQgetvalue(res, i, 0); // SELECT weight
        //
        std::cout << PQgetvalue(res, i, 0) << std::endl;
        //printf("%s %s %s\n", PQgetvalue(res, i, 0), 
        //    PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
    }*/  // 0=id, 1=name, 2=desc (if using SELECT * FROM item)
    ////////////
    ////////////
    ////////////
    ////////////
    ////////////
    //PQclear(res); // Should PQclear PGresult whenever it is no longer needed to avoid memory leaks
    DB2::get_singleton()->/*db2.*/finish(); // close the connection to the database and cleanup
    //if(!db2.get_handle()) std::cout << "conn set to nullptr (means connection closed)\n";
}
////////////////////////////////////////////////
void create_local_database() {
    /////////////////////////////////////////
    // cache.db
    //#ifdef DOKUN_LINUX
    std::string cache_file_name = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cache.db";
    if(!File::exists(cache_file_name)) {
        // create "cache.db" if it does not yet exist
        DB db;if(db.open(cache_file_name)) {
            db.execute("PRAGMA journal_mode = WAL;"); // to prevent database from being locked
            // create table "account" if it does not yet exist
            if(!db.table_exists("account")) {
                db.table("account");
                db.column("account", "ADD", "save", "BOOLEAN");
                db.column("account", "ADD", "username", "TEXT");
            }
            // insert ONLY one row to table
            db.insert("account", "save, username", "false, ''");
            // print message then close the database
            neroshop::print("created \"" + cache_file_name + "\"", 3);
            db.close();
        }
    }
    /////////////////////////////////////////
}
////////////////////////////////////////////////
std::string get_libcurl_version() {
    curl_version_info_data * curl_version = curl_version_info(CURLVERSION_NOW);
    std::string curl_version_str = std::to_string((curl_version->version_num >> 16) & 0xff) + 
        "." + std::to_string((curl_version->version_num >> 8) & 0xff) + 
        "." + std::to_string(curl_version->version_num & 0xff);
    //std::cout << "libcurl version: " << curl_version_str << std::endl;
    return curl_version_str;
}
////////////////////
////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////	
namespace neroshop {
    bool status = false; // off (default)
    ////////////////////
    static bool on_open() {
        //////////////////////////////////////////////////
        // neromon
        start_neromon_server();
        //////////////////////////////////////////////////
        // lua       
        if(DB::get_lua_state() == nullptr) {
            neroshop::print("\033[1;94m[lua]:\033[0m lua state failed to initialize");
            return false;
        }
        luaL_openlibs(DB::get_lua_state()); // opens all standard Lua libraries into the given state. 
        //////////////////////////////////////////////////
        // dokun-ui
        if(!Engine::open()) {
            NEROSHOP_TAG_OUT std::cout << DOKUN_UI_TAG std::string("engine failed to initialize");
            return false;
        }
        //////////////////////////////////////////////////
        // config.lua
        if(!DB::create_config()) { // if it fails to create a config file
            if(!DB::load_config(DB::get_lua_state())) {
                return false;
            } // try loading the config file instead
        }
        //////////////////////////////////////////////////
        // neroshop.db
        create_local_database();
        //////////////////////////////////////////////////
        // cart.db (stored locally or offline)
        if(!Cart::get_singleton()->open()) {
            return false;
        }
        //////////////////////////////////////////////////
        // icons
        Icon::load_all(); // must load all icons before using them    
        //////////////////////////////////////////////////
        // success!
        status = true; // turned on
        return true; // default return value
    }
    ////////////////////
    static bool open() { // init neroshop
        return neroshop::on_open();
    }
    ////////////////////
    static void on_close() {
        if(status == 0) return; // neroshop is off (already)
        // close lua
        lua_close(DB::get_lua_state());        
        // close dokun
        Engine::close();//dokun::Engine::close();
        // kill monerod process ??
        // kill neromon process
        delete server_process;
        neroshop::print("neroshop closed");
    }
    ////////////////////
    static void close() {
        on_close();
    }
    ////////////////////
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
///////////////////////////////////////////////////////////////////////
    // start neroshop
    if(!neroshop::open()) {
        std::cout << "\033[1;91m" << "neroshop: failed to start" << "\033[0m" << std::endl;
        exit(0);
    }
    std::atexit(neroshop::close); // call neroshop::close when program is exited
    // /home/dude/.config/neroshop    <- where config files, cart files (temp) will be stored for guests (sellers' cart data will be stored in the db)
    ////////////////////////////////////////////////
    connect_database();
    ////////////////////////////////////////////////
    User * user = nullptr;
    ////////////////////////////////////////////////
    // Register some items into the database entry
    Item ball("Ball", "A round and bouncy play-thing", 10, 0.5, std::make_tuple(0, 0, 0), "new", "0000-0000-0001");
    Item candy("Candy", "O' so sweet and dee-lish!", 2, 0.01, std::make_tuple(0, 0, 0), "", "0000-0000-0002");
    Item ring("Ring", "One ring to rule them all", 99, 0.5, std::make_tuple(0, 0, 0), "new", "0000-0000-0003");
    // Seller will list some items
    
    // which users will be able to add to cart
    //Cart::get_singleton()->add(ball, 1);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////	
    // Monero	
    // get config: network_type, ip, port, data_dir, etc.
    // network-type: --stagenet, --testnet
    std::string network_type = String::lower(Script::get_string(DB::get_lua_state(), "neroshop.daemon.network_type"));
    if(network_type.empty()) {
        std::cout << "failed to get network type\nsetting default network type: stagenet" << std::endl;
        network_type = "stagenet";
    }
    // --rpc-bind-ip and --rpc-bind-port
    std::string ip = Script::get_string(DB::get_lua_state(), "neroshop.daemon.ip");
    if(ip.empty()) {
        std::cout << "failed to get ip\nsetting default ip: 127.0.0.1" << std::endl;
        ip = "127.0.0.1";
    }
    std::string port = Script::get_string(DB::get_lua_state(), "neroshop.daemon.port");
    if(port.empty()) {
        std::cout << "failed to get port\nsetting default port: 38081" << std::endl;
        port = "38081";
    }
    // --data-dir [path]
    std::string data_dir = Script::get_string(DB::get_lua_state(), "neroshop.daemon.data_dir");
    std::string data_dir_default = std::string("/home/") + System::get_user() + std::string("/.bitmonero");
    if(data_dir.empty()) {
        std::cout << "failed to get data directory\nsetting default data directory: " << data_dir_default << std::endl;
        data_dir = data_dir_default;
    }
    // --confirm-external-bind and --restricted-rpc (if you do not use the --restricted-rpc flag, someone might starting mining on your node >.>)
    bool confirm_external_bind = Script::get_boolean(DB::get_lua_state(), "neroshop.daemon.confirm_external_bind");
    if(Script::get_type(DB::get_lua_state(), "neroshop.daemon.confirm_external_bind") == "nil") {
        confirm_external_bind = false;
        if(ip == "0.0.0.0") {
            confirm_external_bind = true;
        }
    } // set default value if nil
    bool restricted_rpc = Script::get_boolean(DB::get_lua_state(), "neroshop.daemon.restricted_rpc");
    if(Script::get_type(DB::get_lua_state(), "neroshop.daemon.restricted_rpc") == "nil") {
        restricted_rpc = true;
    } // set default value if nil
    // --remote-node (custom arg)
    bool is_remote_node = Script::get_boolean(DB::get_lua_state(), "neroshop.daemon.remote");
    if(Script::get_type(DB::get_lua_state(), "neroshop.daemon.remote") == "nil") is_remote_node = false; // set default value if nil
    // check the type of the restore_height first
    unsigned int restore_height = 0;
    std::vector < std::string > restore_date_vector;
    // if restore_height is a string
    if(Script::get_type(DB::get_lua_state(), "neroshop.wallet.restore_height") == "string") { //std::cout << "restore height is a string\n";
        // convert restore_height (date) to number
        std::string restore_date = Script::get_string(DB::get_lua_state(), "neroshop.wallet.restore_height");
        restore_date_vector = String::split(restore_date, "-"); //std::cout << "Y: " << restore_date_vector[0] << " M: " << restore_date_vector[1] << " D: " << restore_date_vector[2] << std::endl;
        //wallet->get_monero_wallet()->get_height_by_date (restore_date_vector[0], restore_date_vector[1], restore_date_vector[2]); // cannot convert since wallet has not been initialized
    }
    // if restore_height is a number
    if(Script::get_type(DB::get_lua_state(), "neroshop.wallet.restore_height") == "number") { //std::cout << "restore height is a number\n";
        restore_height = Script::get_number(DB::get_lua_state(), "neroshop.wallet.restore_height"); //std::cout << "restore_height: " << restore_height << std::endl;
        // use default restore height (0) if not set by user
        if(restore_height <= 0) {
            /*std::cout << "no user-specific restore_height\nsetting default restore_height: 0" << std::endl;*/
            restore_height = 0;
        }
    }
    // --wallet-file
    std::string wallet_file = Script::get_string(DB::get_lua_state(), "neroshop.wallet.file"); //std::cout << "network-type= " << network_type << ", rpc-bind= " << ip << ":" << port << ", data-dir= " << data_dir << ", restore-height= " << restore_height << ", confirmed-external-bind= " << confirm_external_bind << ", restricted-rpc= " << restricted_rpc << ", use-remote-node= " <<  is_remote_node/* << ", " <<  */<< " <-(data retrieved from config)" << std::endl;
    Wallet * wallet = Wallet::get_singleton();//new Wallet();
    // check if wallet is view only
    //if(wallet->get_monero_wallet()->is_view_only ()) {std::cout << "wallet is view-only\n";}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////	
    bool login_menu = true; //bool node_menu = true;
    bool register_menu = false;
    bool home_menu = false;
    bool synced = false;
    bool wallet_opened = false;
    bool wallet_set = false;
    bool connected = false;
    int times_reconnected = 0;
    // window
    dokun::Window window;
    window.create("neroshop", 
        (Script::get_number(DB::get_lua_state(), "neroshop.window.width" ) != -1) ? Script::get_number(DB::get_lua_state(), "neroshop.window.width" ) : 1280, 
        (Script::get_number(DB::get_lua_state(), "neroshop.window.height") != -1) ? Script::get_number(DB::get_lua_state(), "neroshop.window.height") : 720 , 
        (Script::get_number(DB::get_lua_state(), "neroshop.window.mode"  ) != -1) ? Script::get_number(DB::get_lua_state(), "neroshop.window.mode"  ) : 0   /*1280, 720, 0*/ ); //video->texture->get_width(), video->texture->get_height(), 0); // titlebar is 16x39 //window.set_size(1500, 900);
    window.show();
    // ----------------------------------- fonts testing ---------------------------------
    // font
    dokun::Font font;
    //font->set_height(20);//set_pixel_size(0, 18);
    // setting the font crashes the app, but for some reason only certain fonts do not crash the app
    font.load("res/Mecha-GXPg.ttf");//https://github.com/tonsky/FiraCode    
    // -------------------------------- :P -------------------------------------------
    Label * neroshop_label = new Label();
    //neroshop_label->set_font(font);
    neroshop_label->set_string("ner  shop");
    neroshop_label->set_position(20, 30);
    //neroshop_label->set_scale(1.5, 1.5);
    //neroshop_label->get_font()->set_pixel_size(0, 20);
    Box * monero_icon = new Box();
    Image monero_icon_img(Icon::get["monero_symbol"]->get_data(), 64, 64, 1, 4);
    monero_icon->set_image(monero_icon_img); // use image data rather than the image obj (since it causes a crash for some reason)
    monero_icon->set_as_icon(true);
    monero_icon->set_size(20, 20); // label characters are 10 units in both width and height
    monero_icon->set_position(neroshop_label->get_x() + 30, neroshop_label->get_y());// / 2);
    // ---------------------------------- message ----------------------------------------
    Message message_box; //= Message::get_singleton();// hidden by default
    message_box.set_title("message");
    // the hide function was not working because I set their parent to box with GUI::set_parent(parent)
    // which means I probably didn't own it
    // children are hidden by default and must be shown manually
    message_box.add_edit();//(92/*87 or 85*/, 140); // add edit
    message_box.get_edit(0)->set_sensative(true);
    message_box.add_button("OK", 0, 0, 50, 30); // 0
    message_box.add_button("Cancel", 0, 0, 100, 30); // 1
    message_box.get_button(1)->set_color(214, 31, 31, 1.0); // Cancel button
    message_box.add_button("Submit", 0, 0, 100, message_box.get_edit(0)->get_height()); // 2 // add button
    // temporary
    message_box.get_edit(0)->set_text("supersecretpassword123");
    // create a second message box - this will display wallet notification messages
    Message wallet_message_box; //= Message::get_doubleton();
    // tripleton? nah XD
    // -------------------------------- login -------------------------------------------
    // user_edit ***************************************
    Edit * user_edit = new Edit();
    user_edit->set_size(500, 30); // try using an odd number for width=511
    user_edit->set_character_limit(500);//(32);//(64);//(500);//temp - 500 for testing//(user_edit->get_width() / 10); // 50 characters MAX (width / space_cursor_takes_up_each_increment)
    user_edit->set_position((window.get_width() / 2) - (user_edit->get_width() / 2), (window.get_height() / 2) - (user_edit->get_height() / 2));
    Label user_edit_label;//(*font); // = new Label(); // Label causes a segment fault in GLFW!!
    //user_edit_label.set_font(static_cast<dokun::Font&>(*font));
    user_edit_label.set_color(0, 0, 0, 1.0);//(49, 39, 19, 1.0);
    user_edit_label.set_relative_position(0, 4);
    user_edit->set_label(user_edit_label);
    user_edit->focus(); // same as user_edit->set_focus(true);
    ////GUI::clear_all(); // clears all focus from all gui elements (all gui elements lose focus)
    //user_edit->hide();
    //user_edit->set_cursor_advance(9);
    //mecha_font->set_width(18);
    // password_edit ***********************************
    Edit * pw_edit = new Edit();
    pw_edit->set_size(user_edit->get_width(), user_edit->get_height());
    pw_edit->set_character_limit(128);//(256);//(pw_edit->get_width() / 10);
    pw_edit->set_position(user_edit->get_x(), user_edit->get_y() + user_edit->get_height() + 5);
    Label pw_edit_label;
    pw_edit->set_label(pw_edit_label);
    pw_edit->get_label()->set_color(0, 0, 0, 1.0);
    pw_edit->set_sensative(true);
    //pw_edit->hide();	
    //pw_edit->set_text("Password!23");
    // user_button - replace with label "Username"
    //Box * user_edit_icon = new Box();
    // user_edit placeholder image ******************
    Image user_edit_image(Icon::get["user_login"]->get_data(), 64, 64, 1, 4);
    user_edit_image.resize(16, 16);
    user_edit_image.set_color(32, 32, 32, 0.6);
    user_edit_image.set_relative_position(10, (user_edit->get_height() - user_edit_image.get_height_scaled()) / 2);//user_edit_image.set_alignment("left");
    user_edit->set_placeholder_image(user_edit_image);
    user_edit->set_placeholder_text("   Username");
    //user_edit_icon->hide();
    // pw_icon - replace with label "Password"
    //Box * pw_edit_icon = new Box();
    // pw_edit placeholder image ******************
    Image pw_edit_image(Icon::get["padlock"]->get_data(), 64, 64, 1, 4);
    pw_edit_image.resize(16, 16);
    pw_edit_image.set_color(32, 32, 32, 0.6); // grayed-out
    pw_edit_image.set_relative_position(10, (pw_edit->get_height() - pw_edit_image.get_height_scaled()) / 2);//pw_edit_image.set_alignment("left");
    //std::cout << "pw placeholder alignment: " << pw_edit_image.get_alignment() << std::endl;
    //std::cout << "pw placeholder size: " << pw_edit_image.get_size() << std::endl;
    //std::cout << "pw placeholder size (scaled): " << pw_edit_image.get_size_scaled() << std::endl;
    pw_edit->set_placeholder_image(pw_edit_image);
    pw_edit->set_placeholder_text("   Password");
    // save_login (checkbox)
    Toggle * save_toggle = new Toggle(); // left
    //save_toggle->set_radio(); // should actually be a checkbox instead
    // save_login_label
    Label save_user_label; // right
    save_user_label.set_string("Save user");
    //save_toggle->set_position(save_user_label.get_x() - save_user_label.get_width() - 1/*save_toggle->get_width()*/); 
    //pw_edit_icon->hide();
    // get "save" value from config file or database then set it
    // use sqlite instead of lua to save this.
    std::string cache_file_name = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cache.db";
    DB db(cache_file_name);
    bool cached_save = db.get_column_integer("account", "save", "id = 1"); // returns zero (false) by default
    std::string cached_username = db.get_column_text("account", "username", "id = 1"); // returns empty string by default
    db.close();
    std::cout << "save: " << cached_save << ", username: " << cached_username << std::endl;
    // store cached "save" value in save_toggle
    save_toggle->set_value(cached_save);//((Script::get_boolean(DB::get_lua_state(), "neroshop.account.saved") != -1) ? Script::get_boolean(DB::get_lua_state(), "neroshop.account.saved") : false);
    // store cached "username" value in user_edit
    if(save_toggle->get_value() == true) user_edit->set_text(cached_username);//(Script::get_string(DB::get_lua_state(), "neroshop.account.username"));
    /////////////
    // login_button
    Button * login_button = new Button();
    login_button->set_size(500, 40);//(250, 40); // login_width + register_width = must add up to 400 //(login_button->get_width(), 40);
    Label login_label("Login");
    login_label.set_alignment("center"); //login_label.set_color(, , ); // Alpha 0=fully transparent, 0.2, 0.4, 0.6, 0.8, 1.0=fully visible
    login_button->set_label(login_label);
    login_button->set_color(255, 102, 0); //(255, 213, 0);
    //login_button->hide();
    // register_button
    Button * register_button = new Button();
    Label register_label("Register"); // on stack, not heap
    register_label.set_alignment("center"); //register_label.set_color(, , );
    register_button->set_label(register_label); // login_width + register_width = must add up to 400
    register_button->set_size(150, 40); //(guest_button->get_width(), 40);//(register_label.get_width() * 2, 40);//guest button width: 374
    register_button->set_color(0, 174, 191);; //(102, 205, 170); //register_button->set_position();// will be updated in loop
    //register_button->hide();
    // guest_button
    Button * guest_button = new Button();
    Label guest_label("Continue as guest"); // width=207
    guest_label.set_alignment("center"); //guest_label.set_color(, , );
    guest_button->set_label(guest_label);
    guest_button->set_size(350 - 5, 40);//(500, 40); // 5 is button_horz_spacing between login and register buttons //(guest_label->get_width() * 2, 40); //guest button width: 374 // 40 // multiplying the width by 2 makes it easier to center the label properly		//guest_button->set_outline_color(218,165,32);//x=(login_button->get_width() + register_button->get_width()) + 5
    guest_button->set_color(76, 76, 76); //(255, 105, 255);
    //guest_button->hide();
    // ----------------------------------- settings ------------------------------------
    // settings button
    Button * settings_button = new Button();
    //Box * settings_icon = new Box();
    Image settings_icon(Icon::get["settings"]->get_data(), 64, 64, 1, 4);
    settings_icon.resize(32, 32);
    settings_icon.set_alignment("center");
    settings_button->set_image(settings_icon);
    //settings_icon->set_as_icon(true);	
    settings_button->set_size(50, 40);
    settings_button->set_position(20, 20);
    settings_button->set_color(0, 22, 119); //(60, 105, 134);//settings_button->get_image()->set_color(255, 213, 0);
    //settings_button->hide();
    // daemon_button
    Button * daemon_button = new Button();
    //daemon_button->set_size(login_button->get_width(), 40);
    //Label daemon_label("Daemon"); //width=66
    //daemon_button->set_size(daemon_label.get_width() * 2, 40);// multiplying the width by 2 makes it easier to center the label properly
    //daemon_label.set_alignment("center");
    //daemon_button->set_label(daemon_label);
    daemon_button->set_size(50, 40);
    Image daemon_icon(Icon::get["console"]->get_data(), 64, 64, 1, 4);
    daemon_icon.resize(32, 32);
    daemon_icon.set_alignment("center");
    daemon_button->set_image(daemon_icon);
    daemon_button->set_color(128, 128, 128);
    daemon_button->set_position(settings_button->get_x() + settings_button->get_width() + 10, settings_button->get_y());
    //daemon_button->hide();	
    // create_wallet button
    Button * wallet_button = new Button();
    wallet_button->set_size(50, 40); //Label upload_label("Upload");//upload_button->set_label(upload_label);//upload_button->set_size(upload_label.get_width() * 1.5, 30);
    Image wallet_image(Icon::get["wallet"]->get_data(), 64, 64, 1, 4);
    wallet_image.resize(32, 32);
    wallet_button->set_image(wallet_image);
    wallet_button->get_image()->set_alignment("center");
    wallet_button->set_color(186, 85, 211); //(72,61,139);//(240,255,240);//(192, 192, 192);
    wallet_button->set_position(daemon_button->get_x() + daemon_button->get_width() + 10, daemon_button->get_y());
    //wallet_button->hide();	
    // wallet_edit
    Edit * wallet_edit = new Edit();
    wallet_edit->set_readonly(true);
    wallet_edit->set_size(1000, pw_edit->get_height()); //pw_edit->get_width() //2560=width for 256 chars
    wallet_edit->set_character_limit(wallet_edit->get_width() / 10);
    //wallet_edit->set_position(pw_edit->get_x(), pw_edit->get_y() + pw_edit->get_height() + 5);
    wallet_edit->set_color(112, 128, 144);
    Label wallet_label;//(".."); // setting the text crashes the app // wallet_label //wallet_label.get_font()->set_pixel_size(0, 18);// changes the entire font's size//std::cout << "cursor_space: " << wallet_edit->get_cursor_space() << std::endl; // returns the space the cursor is increased by
    wallet_edit->set_label(wallet_label); // length of wallet file was 69
    wallet_edit->set_text_color(32, 32, 32);
    //wallet_label.set_relative_position(wallet_label.get_relative_x() + 10, wallet_label.get_relative_y() + 10);
    //wallet_edit->hide();
    // upload button
    Button * upload_button = new Button();
    upload_button->set_size(50, 30); //Label upload_label("Upload");//upload_button->set_label(upload_label);//upload_button->set_size(upload_label.get_width() * 1.5, 30);
    Image upload_image(Icon::get["upload"]->get_data(), 64, 64, 1, 4);
    upload_button->set_image(upload_image);
    upload_button->get_image()->resize(24, 24); //upload_button->get_image()->scale_to_fit(upload_button->get_width(), upload_button->get_height());
    upload_button->get_image()->set_alignment("center");
    upload_button->set_color(128, 128, 128); //(240,255,240);//(192, 192, 192);
    upload_button->set_position(wallet_edit->get_x() - upload_button->get_width() - 5, wallet_edit->get_y());
    //upload_button->hide();
    // sync_label
    Label sync_label("SYNC_STATUS:");
    sync_label.set_position(0 + 20, window.get_client_height() - 20);
    // sync_box
    Box * sync_box = new Box();//Image sync_ball(Icon::get["circle"]->get_data(), 64, 64, 1, 4);//sync_box->set_image(sync_ball); //sync_box->set_label(sync_label);
    sync_box->set_size(12, 12); //sync_box->get_image()->set_size(16, 16);
    sync_box->set_color(255, 0, 0);//->get_image()->set_color(255, 0, 0);
    sync_box->set_position(sync_label.get_x() + sync_label.get_width(), sync_label.get_y());
    /*// reconnect_button
    Button * reconnect_button = new Button();
    Label reconnect_label;
    reconnect_label.set_string("Reconnect");
    reconnect_label.set_alignment("center");
    reconnect_button->set_label(reconnect_label);
    reconnect_button->set_color(71, 60, 139);
    reconnect_button->set_size(500, 40);
    reconnect_button->set_position(guest_button->get_x(), guest_button->get_y() + guest_button->get_height() + 10);*/
    
    // --------------------------------- registration --------------------------------
    // user edit - registration
    Edit * user_edit_r = new Edit();
    user_edit_r->set_size(500, 30);
    user_edit_r->set_character_limit(user_edit_r->get_width() / 10);
    Label user_edit_label_r; // = new Label();
    user_edit_label_r.set_color(0, 0, 0, 1.0);
    //user_edit_label_r.set_relative_position(0, 4);
    user_edit_r->set_label(user_edit_label_r);
    user_edit_r->set_placeholder_text("Username *"); // doesn't really do anything
    //user_edit_r->hide();	
    // pw_edit - registration
    Edit * pw_edit_r = new Edit();
    pw_edit_r->set_size(250 - 5, 30); // added a gap (+5) between pw_edit_r and pw_confirm_edit
    pw_edit_r->set_character_limit(128);//(pw_edit_r->get_width() / 10);
    Label pw_edit_label_r; // = new Label();
    pw_edit_label_r.set_color(0, 0, 0, 1.0);
    pw_edit_r->set_label(pw_edit_label_r);
    pw_edit_r->set_placeholder_text("Password *");
    pw_edit_r->set_sensative(true);
    // pw_confirm_edit - registration
    Edit * pw_confirm_edit = new Edit();
    pw_confirm_edit->set_size(250, 30);
    pw_confirm_edit->set_character_limit(pw_edit_r->get_character_limit());//(pw_confirm_edit->get_width() / 10); //(pw_edit_r->get_character_limit());
    Label pw_confirm_edit_label; // = new Label();
    pw_confirm_edit_label.set_color(0, 0, 0, 1.0);
    pw_confirm_edit->set_label(pw_confirm_edit_label);
    pw_confirm_edit->set_placeholder_text("Confirm password *");
    pw_confirm_edit->set_sensative(true);
    // opt_email edit - registration
    Edit * opt_email_edit = new Edit();
    opt_email_edit->set_size(500, 30);
    opt_email_edit->set_character_limit(opt_email_edit->get_width() / 10);
    Label opt_email_edit_label; // = new Label();
    opt_email_edit_label.set_color(0, 0, 0, 1.0);
    opt_email_edit->set_label(opt_email_edit_label);
    opt_email_edit->set_placeholder_text("Email (optional)"); // optional, but without it we won't be able to recover your password or send you email notifications
    // confirm email? nah
    //->set_size(700, 30);
    // gender (radio) ?
    Label gender_label("Gender:");
    Label male_label("Male");
    Label female_label("Female");
    Label other_label("Unspecified");
    Toggle * male_toggle = new Toggle();
    male_toggle->set_radio();
    //male_toggle->set_background_color_on();
    male_toggle->set_outline(true);
    // toggle_female
    Toggle * female_toggle = new Toggle();
    female_toggle->set_radio();
    // toggle_other
    Toggle * other_toggle = new Toggle();
    other_toggle->set_radio();
    other_toggle->set_value(true); // default
    // date of birth ?
    // back-to-login-menu button
    Button * back_button = new Button();
    back_button->set_size(150, 40); // login_width + register_width = must add up to 400 //(login_button->get_width(), 40);
    Label back_label("Go Back");
    back_label.set_alignment("center"); //back_label.set_color(, , ); // Alpha 0=fully transparent, 0.2, 0.4, 0.6, 0.8, 1.0=fully visible
    back_button->set_label(back_label);
    back_button->set_color(96, 96, 96);
    //back_button->hide();
    // submit button - registration
    Button * submit_button = new Button();
    submit_button->set_size(350 - 5, 40); // login_width + register_width = must add up to 400 //(login_button->get_width(), 40);
    Label submit_label("Submit");
    submit_label.set_alignment("center"); //submit_label.set_color(, , ); // Alpha 0=fully transparent, 0.2, 0.4, 0.6, 0.8, 1.0=fully visible
    submit_button->set_label(submit_label);
    submit_button->set_color(65, 105, 225);
    //submit_button->hide();		
    // accept terms of service/agreement?
    // forgot your pw? reset here
    //->set_placeholder_text("");
    // --------------------------------- homepage -------------------------------------
    // search_bar
    Edit * search_bar = new Edit(); // crashes when drawn for some reason
    search_bar->set_size(400, 40);//((window.get_client_width() / 4) * 2 , 30); //620 , 30);
    search_bar->set_color(5,5,6);//(177, 190, 195);
    search_bar->set_cursor_color(255, 255, 255);
    search_bar->set_character_limit(1024);//(2048);//(std::numeric_limits<int>::max());//(std::numeric_limits<double>::infinity());//inf
    Label search_bar_label;
    search_bar_label.set_position(0, 10);
    search_bar_label.set_color(255, 255, 255);//(127, 127, 127);//(32, 32, 32);//(242, 100, 17);
    search_bar->set_label(search_bar_label);
    //search_bar->set_position((window.get_client_width() / 2) - (search_bar->get_width() / 2), 60);
    // search_button
    Button * search_button = new Button();//Box();
    search_button->set_color(17,17,24);//(29,29,41);//(255, 102, 0);//255, 102, 0 = monero orange
    Image search_icon(Icon::get["search"]->get_data(), 64, 64, 1, 4);
    search_button->set_image(search_icon);//(* new Image(Icon::get["search"]->get_data(), 64, 64, 1, 4));//search_button->get_image()->set_color(255,191,0);
    search_button->get_image()->resize(24, 24);
    search_button->get_image()->set_alignment("center");
    search_button->set_size(50, search_bar->get_height()); // adjust size
    //search_button->set_position(search_bar->get_x() + search_bar->get_width()/* + 10*/, search_bar->get_y());
    // search_suggestion_list // A search suggest drop-down list is a query feature used in computing to show the searcher shortcuts, while the query is typed into a text box. Before the query is complete, a drop-down list with the suggested completions appears to provide options to select
    List * search_suggestion_list = new List(); // or seach_suggest_list for short
    //search_suggestion_list->set_size(search_bar->get_width(), 20);
    /*// search_query function:
    std::string str = "123456789abc";
    // get first n characters
    //int n = 8;//1 or 2 would be more ideal
    std::string first_eight = str.substr(0, std::min<size_t>(8, str.length()) );
    std::cout << "first 8 characters: " << first_eight << std::endl;
    // if sql row_name starts_with(first_n_characters)
    */
    // upload login_button    
    // date and time
    Label * date_display = new Label();
    date_display->set_string(Validator::get_date("%Y-%m-%d  %H:%M:%S %p"));
    date_display->set_position(window.get_client_width() - date_display->get_width(), window.get_client_height() - date_display->get_height());
    // icon_settings
    Vector4 default_icon_color = Vector4(255, 255, 255, 1.0); // will get image's color instead if box_type is an icon
    // cart_button
    Button * cart_button = new Button();//Box();
    cart_button->set_size(100, 40);
    cart_button->set_color(50, 50, 50);// - dark charcoal
    Label cart_label("0"); // cart_label
    cart_button->set_label(cart_label);
    Image cart_icon(Icon::get["cart"]->get_data(), 64, 64, 1, 4);
    cart_button->set_image(cart_icon); //("res/icons/white/cart-65-32.png");//"res/icons/white/.png"
    cart_button->get_image()->resize(24, 24);
    //cart_button->set_position(search_bar->get_x() + (search_bar->get_width() + search_button->get_width() + 1) + 52, 20);// treat search_bar and search_button as one single GUI //(window.get_width() - cart_button->get_width() - 20, 20);
    // user_button
    Button * user_button = new Button();
    Image user_icon(Icon::get["user"]->get_data(), 64, 64, 1, 4);
    user_button->set_image(user_icon); //user_button->set_size(user_button->get_image()->get_width(), user_button->get_image()->get_height());
    user_button->get_image()->resize(24, 24);
    user_button->get_image()->set_alignment("center");
    user_button->set_size(50, 40);
    user_button->set_color(214, 46, 46);
    // order_button
    Button * order_button = new Button();
    Image order_icon(Icon::get["order"]->get_data(), 64, 64, 1, 4);
    order_button->set_image(order_icon);
    order_button->get_image()->resize(24, 24);
    order_button->get_image()->set_alignment("center");
    order_button->set_size(50, 40);
    order_button->set_color(96, 120, 72);
    //order_button->set_position(user_button->get_x() + user_button->get_width() + 10, 10);
    // login_button
    Button * logout_button = new Button();
    Image logout_icon(Icon::get["door"]->get_data(), 64, 64, 1, 4);
    logout_button->set_image(logout_icon);
    logout_button->get_image()->resize(24, 24);
    logout_button->get_image()->set_alignment("center");
    logout_button->set_size(50, 40);
    //logout_button->set_color(96, 120, 72);    
    /*// status_icon
    Box * status_icon = new Box();
    status_icon->set_image( * new Image(Icon::get["trash"]->get_data(), 64, 64, 1, 4));
    status_icon->set_as_icon(true);
    status_icon->set_size(16, 16); //(32, 32);
    status_icon->set_position(200, 10);
    // test_icon
    Box * test_icon = new Box();
    test_icon->set_image( * new Image(Icon::get["circle_outline"]->get_data(), 64, 64, 1, 4));
    test_icon->set_as_icon(true);
    test_icon->set_size(16, 16);
    test_icon->set_position(10, 250);
    test_icon->get_image()->set_color(0, 35, 102);
    bool test_activated = false;*/
    /*// star_icon
    Box * star_icon = new Box();
    star_icon->set_image( * new Image( * Icon::get["star_outline"]));
    star_icon->set_as_icon(true);
    star_icon->set_size(16, 16);
    star_icon->set_position(test_icon->get_x() + test_icon->get_width() + 5, 250);
    star_icon->get_image()->set_color(255, 255, 0); // yellow
    bool star_activated = false;*/
    // _icon
    /*Box * heart_icon = new Box();
    heart_icon->set_image(*new Image(*Icon::get["heart_outline"]));
    heart_icon->set_as_icon(true);
    heart_icon->set_size(32, 32);
    heart_icon->set_position(star_icon->get_x() + star_icon->get_width() + 1, 250);
    heart_icon->get_image()->set_color(255, 0, 0); // red 
    bool heart_activated = false;*/
    // _icon
    /*Box * _icon = new Box();
    _icon->set_image(*new Image(*Icon::get[""]));
    _icon->set_as_icon(true);
    _icon->set_size(32, 32);
    _icon->set_position(_icon->get_x() + _icon->get_width() + 5, 250);
    bool _activated = false;*/
    // _icon
    /*Box * _icon = new Box();
    _icon->set_image(*new Image(*Icon::get[""]));
    _icon->set_as_icon(true);
    _icon->set_size(32, 32);
    _icon->set_position(_icon->get_x() + _icon->get_width() + 5, 250);
    bool _activated = false;*/
    // _icon
    /*Box * _icon = new Box();
    _icon->set_image(*new Image(*Icon::get[""]));
    _icon->set_as_icon(true);
    _icon->set_size(32, 32);
    _icon->set_position(_icon->get_x() + _icon->get_width() + 5, 250);
    bool _activated = false;*/
    // _icon
    /*Box * _icon = new Box();
    _icon->set_image(*new Image(*Icon::get[""]));
    _icon->set_as_icon(true);
    _icon->set_size(32, 32);
    _icon->set_position(_icon->get_x() + _icon->get_width() + 5, 250);
    bool _activated = false;*/
    //----------------------------------  --------------------------------------	
    // to-do:
    // refresh button
    // convert to seller button
    // wishlist button
    //--------------------------------- USER ----------------------------------------
    //--------------------------------- CLIENT --------------------------------------
    //std::system("./daemon </dev/null &>/dev/null &"); ::sleep(2);
    //////////////////////////////////////
    Client * client = Client::get_main_client();
	int client_port = 1234;//std::stoi(port)//8080
	std::string client_ip = "0.0.0.0";//"localhost";//0.0.0.0 means anyone can connect to your server
	neroshop::print("connecting to " + ((client_ip == "localhost") ? "127.0.0.1" : client_ip) + ":" + std::to_string(client_port) + " ..");
	if(!client->connect(client_port, client_ip)) {
	    //exit(0);
	} else std::cout << client->read() << std::endl; // read from server once
	// on browser, go to: https://127.0.0.1:1234/
	// or https://localhost:1234/
	// brave (chromium): ERR_SSL_PROTOCOL_ERROR
	// firefox: SSL_ERROR_RX_RECORD_TOO_LONG
	///////////////////////////////////////////////
	// temporary
	pw_edit->set_text("Password!23");
	/////////////////////////////////////////////////////////////////////////////////
    // order listener timer - checks for pending transaction every x seconds
    Timer order_listener_timer;
    order_listener_timer.start();
	/////////////////////////////////////////////////////////////////////////////////
    // ------------------------------------------------------------------------------
    Slider * slider = new Slider();
    slider->set_value(30);
    slider->set_position(10, 300);
    //slider->set_value(60);//slider->set_value(2); // value of 2 = ball_x of 4    
    Slider * slider2 = new Slider();
    slider2->set_value(10);
    slider2->set_position(10, 325);    
    ////////////////
    Box * box = new Box();
    box->set_size(100, 50);
    box->set_color(160, 160, 160, 1.0);//(0, 51, 102);
    Label box_label;
    box_label.set_alignment("center");
    box->set_label(box_label);
    //box->set_text("Hello");
    //box->set_as_tooltip(true);
    box->set_position(100, 150);
    box->set_title_bar(true);
    box->set_radius(50.0); // for rounded corner
    ////////////////
    // Spinner - 98% progress
    Spinner * spinner = new Spinner(); // A.K.A number_picker
    spinner->set_position(100, 100);
    spinner->set_button_color(64, 64, 64, 1.0);
    spinner->set_shape_color(0, 255, 255, 1.0); // cyan :O
    spinner->set_separator(true);
    spinner->set_separator_size(5);
    Label spinner_label;
    spinner_label.set_alignment("center");
    spinner_label.set_color(100, 100, 100);
    spinner->set_label(spinner_label); // value is set to 0 by default
    ////////////////
    Toggle * checkbox = new Toggle();
    checkbox->set_checkbox();
    checkbox->set_position(30, 500);
    
    Toggle * radio = new Toggle();
    radio->set_radio();
    radio->set_position(30, 550);
    ////////////////
    ////////////////
    std::cout << "while loop commencing...\n";
    while(window.is_open()) { // main thread
        window.poll_events(); // check for events        
        window.set_viewport(window.get_client_width(), window.get_client_height()); // set to dimensions of render client rather than the entire window (for pixel coordinates)
        window.clear(32, 32, 32);
//std::cout << window.get_client_size() << std::endl;
        //////////////////////////////////registration_menu//////////////////////////////////////////
        register_men:while (register_menu) { // placing this before login_menu, hides the main_application
            window.poll_events(); // check for events
            window.set_viewport(window.get_client_width(), window.get_client_height());
            window.clear(32, 32, 32);
            /////////////////////////
            if(submit_button->is_pressed() && !message_box.is_visible()) {
                if(Validator::register_user(user_edit_r->get_text(), pw_edit_r->get_text(), pw_confirm_edit->get_text(), opt_email_edit->get_text())) {
                    // login user after a successful registration (create user)		
                    std::string username = user_edit_r->get_text();
                    if(!user) user = Buyer::on_login(username); //(user_edit_r->get_text()); // by default, every user starts off as a normal buyer
                    // clear text edits
                    user_edit_r->clear_all();
                    pw_edit_r->clear_all();
                    pw_confirm_edit->clear_all();
                    opt_email_edit->clear_all();
                    // leave the register_menu
                    register_menu = false;
                    // go to the home_menu
                    home_menu = true;
                    // show message
                    message_box.set_text("Welcome to neroshop, " + username, 52, 101, 164);//("You have registered successfully!"); // 52, 101, 164 = tango palette entry 4 (blue)
                    message_box.show();
                }
            }
            if(back_button->is_pressed() && !message_box.is_visible()) {
                //if(client->is_connected()) client->write("back button pressed"); // temporary
                // clear text edits
                user_edit_r->clear_all();
                pw_edit_r->clear_all();
                pw_confirm_edit->clear_all();
                opt_email_edit->clear_all();
                // leave the register_menu
                register_menu = false;
                // return to the login_menu
                login_menu = true;
            }
            // set positions
            // draw - no need to hide the guis since they will not be drawn when this thread ends
            user_edit_r->draw((window.get_client_width() / 2) - (user_edit_r->get_width() / 2), (window.get_client_height() / 4) / 2);
            pw_edit_r->draw(user_edit_r->get_x(), user_edit_r->get_y() + user_edit_r->get_height() + 50);
            pw_confirm_edit->draw(pw_edit_r->get_x() + pw_edit_r->get_width() + 5, pw_edit_r->get_y());
            opt_email_edit->draw(user_edit_r->get_x(), pw_edit_r->get_y() + pw_edit_r->get_height() + 50);
            // gender toggles
            /*//Toggle::group({male_toggle, female_toggle, other_toggle});
            male_toggle->draw(opt_email_edit->get_x(), opt_email_edit->get_y() + opt_email_edit->get_height() + 50);
            male_label.draw(male_toggle->get_x() + male_toggle->get_width() + 5, male_toggle->get_y() + ((male_toggle->get_height() / 2) - (10 / 2)));
            female_toggle->draw(opt_email_edit->get_x() + (opt_email_edit->get_width() / 3), male_toggle->get_y());
            female_label.draw(female_toggle->get_x() + female_toggle->get_width() + 5, female_toggle->get_y() + ((female_toggle->get_height() / 2) - (10 / 2)));
            other_toggle->draw(opt_email_edit->get_x() + (opt_email_edit->get_width() - other_toggle->get_width() - other_label.get_width()), female_toggle->get_y());
            other_label.draw(other_toggle->get_x() + other_toggle->get_width() + 5, other_toggle->get_y() + ((other_toggle->get_height() / 2) - (10 / 2)));
            */// ...
            back_button->draw(opt_email_edit->get_x(), window.get_client_height() - ((window.get_client_height() / 4) / 2)); //, opt_email_edit->get_y() + opt_email_edit->get_height() + 10);
            submit_button->draw(back_button->get_x() + back_button->get_width() + 5, back_button->get_y());
            /*// sync status
            sync_label.draw(0 + 20, window.get_client_height() - 20);
            sync_box->draw(sync_label.get_x() + sync_label.get_width(), sync_label.get_y());
            */////////////////
            if(Keyboard::is_pressed(DOKUN_KEY_ESCAPE) && !message_box.is_visible()) window.destroy();
            message_box.center(window.get_client_width(), window.get_client_height());
            message_box.draw(); // draw if visible
            //////////////// temp
            //std::cout << "pw_edit_r->get_text: " << pw_edit_r->get_text() << std::endl;
            ////////////////
            window.update(); // swap buffers
        } // register_menu		            
        /////////////////////////////////login_menu//////////////////////////////////////
        while (login_menu) // main application shows when register button is pressed since main_app comes after login_menu :|
        {
            ////////////////
            //if(user_edit->has_focus()) user_edit->set_color(64, 64, 64);// set color of focused gui
            //else if(!user_edit->has_focus()) user_edit->set_color(0, 255, 255);
            //if(pw_edit->has_focus()) pw_edit->set_color(64, 64, 64);
            //else if(!pw_edit->has_focus()) pw_edit->set_color(0, 255, 255);
            ///////////////	    
            window.poll_events(); // check for events
            // update viewport (in case window is resized) and clear window
            window.set_viewport(window.get_client_width(), window.get_client_height());
            window.clear(32, 32, 32);
            /////////////////////
            // On enter pressed with focus on user_edit
            //if(user_edit->has_focus() && Keyboard::is_pressed(DOKhttps://www.color-hex.com/color-palette/6591UN_KEY_RETURN)) {
            //    pw_edit->set_focused(true);
            //    std::cout << "pw_edit has focus\n";
            //}
            // On enter pressed with focus on pw_edit
            if(pw_edit->has_focus() && Keyboard::is_pressed(DOKUN_KEY_RETURN)) {
                // attempt a login
                std::cout << "login anaa?\n";
            }            
            /////////////////////////
            // login_button
            if(login_button->is_pressed() && !message_box.is_visible()) {
                bool logged = false;
                // try to login user after connecting to server (daemon)
                if(client->is_connected()) {
                    logged = Validator::login(user_edit->get_text(), pw_edit->get_text());
                }
                if(!client->is_connected()) {
                    message_box.set_text("Server is offline (Please, launch neromon then restart neroshop)");//("Connection to server has been lost"); 
                    message_box.show();
                    logged = false;
                }                
                // connected to the server, but failed to login
                if(!logged && client->is_connected()) {
                    message_box.set_text("Incorrect login credentials. Please, try again", "red"); //neroshop::message_box.center(window.get_client_width(), window.get_client_height());
                    // box text
                    message_box.get_box()->get_label()->set_alignment("none");
                    message_box.get_box()->get_label()->set_relative_position((message_box.get_box()->get_width() - message_box.get_box()->get_label()->get_string().length() * 10/*message_box.get_box()->get_label()->get_width()*/) / 2, 75);
                    // box button
                    message_box.get_button(1)->set_text("Close");//("OK");
                    //message_box.get_button(1)->set_color(0, 51, 102, 1.0);                 
                    message_box.get_button(1)->set_relative_position((message_box.get_box()->get_width() / 2) - (message_box.get_button(1)->get_width() / 2), message_box.get_box()->get_height() - message_box.get_button(1)->get_height() - 20);//50);
                    message_box.get_button(1)->show();                    
                    message_box.show();
                }
                if(logged) //if(Validator::login(user_edit->get_text(), pw_edit->get_text()))
                {
                    // use sql instead of lua for storing the save_user
                    // upon login, update the save_toggle value to whatever the value was set to at the time the user logged in
                    std::string cache_file_name = std::string("/home/" + System::get_user() + "/.config/neroshop/") + "cache.db";
                    DB db(cache_file_name);
                    db.update("account", "save", std::to_string(save_toggle->get_value()), "id = 1");
                    db.update("account", "username", (save_toggle->get_value()) ? DB::to_sql_string(user_edit->get_text()) : "''", "id = 1");
                    if(save_toggle->get_value() == true ) neroshop::print("Username saved", 3);
                    if(save_toggle->get_value() == false) neroshop::print("Username not saved", 2);
                    db.close();
                    // check whether user is a buyer or seller
                    DB2::get_singleton()->connect("host=127.0.0.1 port=5432 user=postgres password=postgres dbname=neroshoptest");
                    unsigned int account_type_id = DB2::get_singleton()->get_integer_params("SELECT account_type_id FROM users WHERE name = $1", { user_edit->get_text() });
                    DB2::get_singleton()->finish();
                    // create user
                    if(account_type_id <= 0) {
                        std::cout << "This user was not found in database";
                        neroshop::close();
                    }
                    if(account_type_id == 1) user = Buyer::on_login(user_edit->get_text());
                    if(account_type_id == 2) user = Seller::on_login(user_edit->get_text());
                    // broadcast messages to server
                    if(client->is_connected()) client->write(user->get_name() + " has logged in "); // temporary
                    // set wallet and check for pending orders
                    if(user->is_seller()) {
                        // set the wallet if it is opened, but it is not yet set
                        if(wallet != nullptr) {//if(wallet_opened && !wallet_set) {
                            static_cast<Seller *>(user)->set_wallet(*wallet);
                            wallet_set = true;
                            neroshop::print("wallet set for seller (id: " + std::to_string(user->get_id()) + ")", 3);
                        }
                        // check for any incoming orders (pending)
                        /*std::vector<int> pending_orders = static_cast<Seller *>(user)->get_pending_customer_orders();
                        if(pending_orders.size() > 0) {
                            if(wallet_opened && synced) {
                                static_cast<Seller *>(user)->on_order_received();
                            }
                        }*/
                    }
                    // print detailed user information
                    std::cout << "**********************************************************\n";
                    std::cout << user->has_email() << " (user->has_email())" << std::endl;
                    std::cout << user->is_registered() << " (user->is_registered())" << std::endl;
                    //std::cout << User::is_registered("jackfrost"/*user->get_name()*/) << " (User::is_registered(name))" << std::endl;
                    std::cout << user->is_logged() << " (user->is_logged())" << std::endl;
                    std::cout << user->is_seller() << " (user->is_seller())" << std::endl;
                    std::cout << user->is_buyer() << " (user->is_buyer())" << std::endl;
                    if(user->is_seller()) {
                        //Seller * seller = static_cast<Seller *>(user);
                        std::cout << "total_seller_ratings: \033[1;93m" << static_cast<Seller *>(user)->get_ratings_count() << "\033[0m" << std::endl;
                        std::cout << "good_ratings: \033[1;32m" << static_cast<Seller *>(user)->get_good_ratings() << "\033[0m" << std::endl;
                        std::cout << "bad_ratings: \033[1;91m" << static_cast<Seller *>(user)->get_bad_ratings() << "\033[0m" << std::endl;
                        std::cout << "reputation: \033[1;34m" << static_cast<Seller *>(user)->get_reputation() << "%\033[0m" << std::endl;
                    }
                    std::cout << "**********************************************************\n";
                    /// 0. Convert to a seller and register an item
                    //user->convert(); // convert to seller
                    //Item::register(...);
                    /// 1. Seller will list some items or increase stock (for already listed items)
                    /*static_cast<Seller *>(user)->set_stock_quantity(1, 250);
                    static_cast<Seller *>(user)->set_stock_quantity(2, 500);
                    static_cast<Seller *>(user)->set_stock_quantity(3, 100);*/
                    static_cast<Seller *>(user)->list_item(ball, 500, 8.50, "USD", 0.50, 2, 1, "2022-02-12 12:05:00", "new"); //adds item to inventory
                    static_cast<Seller *>(user)->list_item(candy, 1070, 2.00, "USD");
                    static_cast<Seller *>(user)->list_item(ring, 200, 101.00, "USD");//, 0.00, 0, "new");
                    //static_cast<Seller *>(user)->list_item(game, 50, 69.99, "USD", 0.50, 2, "new");
                    //static_cast<Seller *>(user)->list_item(game, 10, 8.50, "usd"); // temp
                    //static_cast<Seller *>(user)->list_item(ring, 3, 1444.00, "jpy");//122.00, "usd");
                    //static_cast<Seller *>(user)->list_item(game, 7, 69.00, "usd");
                    /// 2. which users will be able to then add to cart
                    //Cart::get_singleton()->remove(ball, 10);
                    user->add_to_cart(ball, 2);
                    user->add_to_cart(candy, 10);
                    ////Cart::get_singleton()->add(ring, 1);
                    //Cart::get_singleton()->add(game, 1);
                    /// 3. and finally, use the cart to make an order
                    std::string shipping_addr = "Lars Mars\n"
                    "12 Earth St.\n"
                    "Boston MA 02115";
                    Order * order = user->create_order(shipping_addr);//, "larteyoh@protonmail.com");                    
                    // for an online cart, you can retrieve your cart id like this:
                    // cart_id = db.get_integer_params("SELECT user_id FROM cart WHERE user_id = $1", { user->get_id() });
                    // then save by attaching it to the user
                    // user->set_cart(cart_id);
                    // update cart qty everytime add, remove, or change_qty, is called
                    cart_button->get_label()->set_string(std::to_string(Cart::get_singleton()->get_total_quantity()));                    
                    // set the wallet
                    //if(user && wallet_opened) static_cast <Seller *>(user)->set_wallet(*wallet);
                    //std::cout << "is_user_logged_in: " << user->is_logged() << std::endl;
                    //std::cout << "is_user_registered: " << user->is_registered() << std::endl;
                    //std::cout << "is_user_guest: " << user->is_guest() << std::endl;
                    //std::cout << "is_user_buyer: " << user->is_buyer() << std::endl;
                    //std::cout << "is_user_seller: " << user->is_seller() << std::endl;
                    // 5. rate item you've purchased or rate the seller you purchased from
                    // rate item (from 1-5)
                    //user->rate_item(ball.get_id(), 2, "It's meh :|");//(game.get_id(), 1, "It's meh"); // give ball a 5 star rating//user->rate_item(ball.get_id(), 5, "Very bouncy. I love it!");
                    // rate seller (from 0-1)
                    int seller_id = 4;
                    if(user->get_name() == "jack") user->rate_seller(seller_id, 1, "This seller is awesome!");
                    if(user->get_name() == "dude") user->rate_seller(seller_id, 0, "This seller sucks!");
                    if(user->get_name() == "mike") user->rate_seller(seller_id, 1, "This seller is rocks!");
                    std::cout << "**********************************************************\n";
                    // leave the login_menu           
                    login_menu = false;
                    // go to home menu
                    home_menu = true;
                }
            }
            // guest_button
            if(guest_button->is_pressed() && !message_box.is_visible()) {
                // create user		        
                if(!user) user = new Buyer("Guest");
                std::cout << "Hello, " << user->get_name() << std::endl;
                //std::cout << "is_user_logged_in: " << user->is_logged() << std::endl;
                //std::cout << "is_user_registered: " << user->is_registered() << std::endl;
                //std::cout << "is_user_guest: " << user->is_guest() << std::endl;
                //std::cout << "is_user_buyer: " << user->is_buyer() << std::endl;
                //std::cout << "is_user_seller: " << user->is_seller() << std::endl;
                // leave the login_menu
                login_menu = false;
                // go to home_menu
                home_menu = true;
            }
            /////////////////////
            // register_button
            if(register_button->is_pressed() && !message_box.is_visible()) {
                // leave the login_menu
                login_menu = false; // causes to exit program
                // take user to the register_menu ...
                register_menu = true;
                // go to register_menu loop (which comes before this loop), so it does not exit the app
                goto register_men;
            }
            // settings_button		    
            if(settings_button->is_pressed() && !message_box.is_visible()) {
                std::string cfg_file = "/home/" + System::get_user() + "/.config/neroshop/config.lua";
                NEROSHOP_TAG_OUT std::cout << "opening: " << cfg_file << std::endl;
                std::system(std::string("gedit " + cfg_file).c_str()); // ubuntu
            }
            /////////////////////
            // daemon_button
            if(daemon_button->is_pressed() && !message_box.is_visible()) { // && !msg_box.is_visible()
                if(!wallet_opened  ) {//wallet->get_monero_wallet() == nullptr ) {
                    message_box.set_text("wallet has not been opened"); //msg_box.get_label()->set_string("  wallet has not been opened");//msg_box.set_width(msg_box.get_label()->get_width()); // update msg_box width based on label's width//message_box.center(window.get_client_width(), window.get_client_height());//msg_box.set_position((window.get_width() / 2) - (msg_box.get_width() / 2), (window.get_height() / 2) - (msg_box.get_height() / 2)); // update msg_box pos since width changed
                    // box text (label)
                    message_box.get_box()->get_label()->set_alignment("none");
                    message_box.get_box()->get_label()->set_relative_position((message_box.get_box()->get_width() - message_box.get_box()->get_label()->get_string().length() * 10/*message_box.get_box()->get_label()->get_width()*/) / 2, 75);//55);                    
                    // ok button - not being used at the moment
                    // cancel button
                    message_box.get_button(1)->set_text("Close");
                    //message_box.get_button(1)->set_color(214, 31, 31, 1.0);                    
                    message_box.get_button(1)->set_relative_position((message_box.get_box()->get_width() / 2) - (message_box.get_button(1)->get_width() / 2), message_box.get_box()->get_height() - message_box.get_button(1)->get_height() - 20);//200-30(height)-20(bottompadding) = 150(button_y)
                    message_box.get_button(1)->show();
                    message_box.show();
                }
                if(!synced && wallet_opened) { //message_box.set_text("waiting for node to sync");message_box.show();
                    //message_box.set_text("Please wait for daemon to fully sync"); //msg_box.get_label()->set_string("  wallet has not been opened");//msg_box.set_width(msg_box.get_label()->get_width()); // update msg_box width based on label's width//message_box.center(window.get_client_width(), window.get_client_height());//msg_box.set_position((window.get_width() / 2) - (msg_box.get_width() / 2), (window.get_height() / 2) - (msg_box.get_height() / 2)); // update msg_box pos since width changed
                    //message_box.show();
                    // start sync!
                    wallet->daemon_open(ip, port, confirm_external_bind, restricted_rpc, is_remote_node, data_dir, network_type, restore_height);
                    synced = wallet->daemon_connect(ip, port); //synced = true;
                    if(synced) {
                        sync_box->set_color(0, 255, 0);
                        wallet->wallet_info(); // will crash if not synced // to prevent user from spamming daemon continuously
                        //std::cout << "unused addresses:" << std::endl;
                        //std::vector<std::string> unused_addresses = wallet->address_unused();
                        //for(int i = 0; i < 10; i++) {//wallet->address_new();
                        //    std::cout << unused_addresses[i] << std::endl;
                        //}
                    }
                }
            }
            /////////////////////
            // upload button
            if(upload_button->is_pressed() && !message_box.is_visible() && !wallet_opened) {
                // upload just the filename, but do not open it
                std::cout << "wallet upload_button is pressed\n";
                std::string wallet_file = wallet->upload(false);
                if(!wallet_file.empty()) {
                    wallet_edit->set_text(wallet_file); //wallet_edit->show();
                    ////////////////////
                    // message_box
                    message_box.set_text("Please enter your wallet password: ", 0, 0, 0, 1.0);
                    // set relative positions
                    // box_text - center the x
                    message_box.get_box()->get_label()->set_alignment("none");
                    message_box.get_box()->get_label()->set_relative_position((message_box.get_box()->get_width() - message_box.get_box()->get_label()->get_string().length() * 10/*message_box.get_box()->get_label()->get_width()*/) / 2, 75);//(200−10)÷2 = 95 = (200÷2)−(10÷2) //200=box_ht, 10=label_ht, 95=label_y (centered) // 95 - 20(button_y) = 75
                    // box_edit
                    message_box.get_edit(0)->set_relative_position((message_box.get_box()->get_width() / 2) - (message_box.get_edit(0)->get_width() / 2) - ((message_box.get_button(2)->get_width() + 10) / 2), message_box.get_box()->get_height() - message_box.get_edit(0)->get_height() - 20);
                    // box_button
                    message_box.get_button(2)->set_relative_position(message_box.get_edit(0)->get_relative_x() + message_box.get_edit(0)->get_width() + 10, message_box.get_edit(0)->get_relative_y());//message_box.get_edit(0)->get_relative_y());
                    // show edit, button (children must be shown manually), etc.
                    message_box.get_button(2)->show();
                    message_box.get_edit(0)->show();
                    ///////////////////
                    message_box.show(); // show message
                    ///////////////////                 
                }
                else if(wallet_file.empty()) {
                    message_box.set_text("wallet not found");
                    // box text (label)
                    message_box.get_box()->get_label()->set_alignment("none");
                    message_box.get_box()->get_label()->set_relative_position((message_box.get_box()->get_width() - message_box.get_box()->get_label()->get_string().length() * 10/*message_box.get_box()->get_label()->get_width()*/) / 2, 75);                   
                    // ok button - not being used at the moment
                    // cancel button
                    message_box.get_button(1)->set_text("Close");
                    //message_box.get_button(1)->set_color(214, 31, 31, 1.0);                    
                    message_box.get_button(1)->set_relative_position((message_box.get_box()->get_width() / 2) - (message_box.get_button(1)->get_width() / 2), message_box.get_box()->get_height() - message_box.get_button(1)->get_height() - 20);
                    message_box.get_button(1)->show();                    
                    message_box.show();
                }
            }
            /////////////////////
            // wallet_password prompt (Message)(we are still in login_menu thread)
            ////if(message_box.get_button_count() > 0) {//&& !wallet_opened) {         
            // if OK is pressed, open the wallet file
            if(message_box.get_button(2)->is_pressed() && !wallet_opened) { // message_box.restore(); // restore defaults
                std::string wallet_name = wallet_edit->get_text(); // wallet (uploaded) from edit
                std::string password = message_box.get_edit(0)->get_text(); // get pw from edit //std::string password; std::cin >> password;
                message_box.get_edit(0)->clear_all(); // clear pw from edit
                wallet->open(wallet_name.substr(0, wallet_name.find(".")), password); // will crash if password is incorrect // Wallet::open requires that you exclude the ".keys" ext
                wallet_opened = true;
                // hide message box, once wallet is opened
                message_box.hide(); // hide and restore defaults
            }
            ////}  
            /////////////////////
            // while the wallet is not opened, clear the wallet_edit
            if(!wallet_opened && !message_box.is_visible()) wallet_edit->clear_all();            
            /////////////////////
            if(wallet_button->is_pressed() && !message_box.is_visible() && !wallet_opened) {
                message_box.set_text("Enter wallet name:");
                message_box.show();
                message_box.set_text("Enter password:");
                message_box.show();
            }
            /////////////////////
            /*if(reconnect_button->is_pressed() && !message_box.is_visible()) {
                message_box.set_text("Attempting to reconnect ...");
                message_box.show();
                if(!connected) {//while(!reconnected) {
                    if(client->reconnect(client_port, client_ip)) {
                        message_box.set_text("Connected!", "green");
                        message_box.show();
                        connected = true;
                    }
                }
            }*/
            /////////////////////
            // on cancel button pressed
            if(message_box.get_button(1)->is_pressed()) {
                message_box.hide();
            }
            /////////////////////
            if(Keyboard::is_pressed(DOKUN_KEY_P)) Process::show_processes();
            if(Keyboard::is_pressed(DOKUN_KEY_K)) {
                //stop_neromon_server();// close neromon daemon-server
                ////Process::terminate_by_process_name("neromon"); // DOES NOT WORK -.-
                //NEROSHOP_TAG_OUT std::cout << "\033[37mProcess: neromon killed (id: " << Process::get_process_by_name("neromon") << ")\033[0m\n"; // ALWAYS RETURNS -1 -.-
                NEROSHOP_TAG_OUT std::cout << "server_process_id (before termination): " << server_process->get_handle() << std::endl; 
                server_process->terminate();
                NEROSHOP_TAG_OUT std::cout << "server_process_id (after termination): " << server_process->get_handle() << std::endl;
            }
            /////////////////////
            if(Keyboard::is_pressed(DOKUN_KEY_DOWN) && !message_box.is_visible()) {
                //std::cout << "\033[0;32mcopied text to clipboard\033[0m" << std::endl;
                //pw_edit->copy_all();
                //std::cout << "\033[0;32mpasted text from clipboard\033[0m" << std::endl;
                //pw_edit->paste(); // slow probably due to sync, but whatever, at least copy is fast and its what I need
            }
            /////////////////////
            //??->set_??(slider->get_value());
            ////////////////////
            // if esc is pressed
            if(Keyboard::is_pressed(DOKUN_KEY_ESCAPE) && !message_box.is_visible()) {
                // do you wish to exit the program
                window.destroy();
            }
            // update ui positions (in case window is resized)
            wallet_edit->set_width(window.get_client_width() - (wallet_button->get_x() + wallet_button->get_width() + 5) - (upload_button->get_width() + 5) - 20); // 5 is space b/t upload_button and wallet_edit // 5 is space b/t wallet_button and wallet_edit		    
            wallet_edit->set_position(wallet_button->get_x() + wallet_button->get_width() + 5, wallet_button->get_y() - ((wallet_edit->get_height() / 2) - (wallet_button->get_height() / 2))); //wallet_edit->set_position(pw_edit->get_x() - pw_edit->get_width() / 2, pw_edit->get_y() + pw_edit->get_height() + 5);
            user_edit->set_position((window.get_client_width() / 2) - (user_edit->get_width() / 2), (window.get_client_height() / 2) - (user_edit->get_height() / 2) - 60);
            pw_edit->set_position(user_edit->get_x(), user_edit->get_y() + user_edit->get_height() + 10);
            upload_button->set_position(wallet_edit->get_x() + wallet_edit->get_width() + 5, wallet_edit->get_y()); //upload_button->set_position(wallet_edit->get_x() - upload_button->get_width() - 5, wallet_edit->get_y());
            //int button_horz_spacing = 5;
            login_button->set_position(pw_edit->get_x() + ((login_button->get_width() / 2) - (pw_edit->get_width() / 2)), pw_edit->get_y() + pw_edit->get_height() + 70);//10);//set_position(pw_edit->get_x() - (((login_button->get_width() + register_button->get_width() + 5) / 2) - (pw_edit->get_width() / 2)), pw_edit->get_y() + pw_edit->get_height() + 10); //register_button->set_position(login_button->get_x() + login_button->get_width() + 5, login_button->get_y()); // => register and login on same line
            guest_button->set_position(pw_edit->get_x(), login_button->get_y() + login_button->get_height() + 10); //guest_button->set_position(login_button->get_x(), login_button->get_y() + login_button->get_height() + 5);
            register_button->set_position(guest_button->get_x() + guest_button->get_width() + 5, guest_button->get_y());
            ////reconnect_button->set_position(guest_button->get_x(), guest_button->get_y() + guest_button->get_height() + 10);
            // draw ui
            if(!message_box.is_visible()) { // just so you don't enter keys into the pw and username edits (temporary - until I fix the GUI::focus thing)
                user_edit->draw(); //
                pw_edit->draw(); //std::cout << "pw_edit->get_text: " << pw_edit->get_text() << std::endl;
            }
            // upper options
            settings_button->draw();
            daemon_button->draw();
            wallet_button->draw();
            wallet_edit->draw();
            upload_button->draw();
            // login            
            login_button->draw();
            guest_button->draw();
            register_button->draw();
            ////reconnect_button->draw();
            // save user
            save_toggle->draw(pw_edit->get_x(), pw_edit->get_y() + 80);
            save_user_label.draw(save_toggle->get_x() + save_toggle->get_width() + 5, save_toggle->get_y() + (save_toggle->get_height() / 4));
            // sync status
            //sync_label.draw(0 + 20, window.get_client_height() - 20);
            //sync_box->draw(sync_label.get_x() + sync_label.get_width(), sync_label.get_y());
            // temp
            ////slider->draw();
            ////slider2->draw();
            //Renderer::draw_spinner(100, 100, 40, 20, 0, 1, 1, 255, 255, 255, 1.0, 0, 
            //    20, Vector4(64, 64, 64, 1.0), // button_width, button_color
            //    8, Vector4(0, 255, 255, 1.0), 2.0,         // shape_size, shape_color, shape_depth 
            //    false, 5); // separator, separator_gap
            ////spinner->draw();
            //box->draw();
            //box->set_radius(round(slider->get_value()));
            //std::cout << "slider_value_rounded: " << round(slider->get_value()) << std::endl;//slider->get_value()
            ////checkbox->set_size(round(slider->get_value()), round(slider->get_value()));
            ////checkbox->draw();
            //radio->draw(); // causes SEGFAULT
            //Renderer::draw_checkbox(30, 500, slider->get_value(), slider->get_value(), 0, 1, 1, 0, 51, 102, 1.0,
	        //    true, Vector4(255, 255, 255, 1.0)); // value, checkmark_color
            //->set_(round(slider->get_value()));
            // draw message (if visible)
            //if(Keyboard::is_pressed(DOKUN_KEY_M)) message_box.show();
            //if(Keyboard::is_pressed(DOKUN_KEY_H)) message_box.get_button(0)->hide();
            //if(Keyboard::is_pressed(DOKUN_KEY_S)) message_box.get_button(0)->show();
            message_box.center(window.get_client_width(), window.get_client_height());
            message_box.draw();
            // draw second message box - for wallet notifications
            wallet_message_box.center(window.get_client_width(), window.get_client_height());
            wallet_message_box.draw();
            // update window
            window.update();
        }
        //////////////////////////////////////home_menu//////////////////////////////////////
        while (home_menu) {
            window.poll_events(); // check for events
            window.set_viewport(window.get_client_width(), window.get_client_height()); // set to dimensions of render client rather than the entire window (for pixel coordinates)
            window.clear(28, 33, 48);//(42, 44, 49);//(62, 65, 71);//(65, 62, 74);//(35,44,49);//(65, 69, 71); //(32, 32, 32);	    
            neroshop_label->draw();
            monero_icon->draw();
            date_display->set_position(window.get_client_width() - date_display->get_width(), window.get_client_height() - date_display->get_height());
            date_display->set_string(Validator::get_date("%Y-%m-%d  %l:%M:%S %p"));
            date_display->draw();
/*
            if(Mouse::is_over(cart_button->get_image()->get_rect())) cart_button->get_image()->set_color(242, 100, 17);
            else {
                cart_button->get_image()->set_color(default_icon_color);
            }
            if(Mouse::is_over(search_button->get_rect())) search_button->get_image()->set_color(242, 100, 17);
            else search_button->get_image()->set_color(default_icon_color);
            //if(Mouse::is_over(settings_icon->get_rect())) settings_icon->get_image()->set_color(242, 100, 17); else settings_icon->get_image()->set_color(default_icon_color);
            if(Mouse::is_over(user_button->get_rect())) user_button->get_image()->set_color(242, 100, 17);
            else user_button->get_image()->set_color(default_icon_color);
            if(Mouse::is_over(order_button->get_rect())) order_button->get_image()->set_color(242, 100, 17);
            else order_button->get_image()->set_color(default_icon_color);
            */
            //if(Mouse::is_over(test_icon->get_rect())) test_icon->get_image()->copy(*Icon::get["circle"]); else test_icon->get_image()->copy(*Icon::get["circle_outline"]);
            /*if(Mouse::is_over(test_icon->get_rect())) {
                if(Mouse::is_pressed(1) && test_activated == false) {
                    test_icon->get_image()->copy( * Icon::get["circle"]);
                    test_activated = true;
                } else if(Mouse::is_pressed(1) && test_activated == true) {
                    test_icon->get_image()->copy( * Icon::get["circle_outline"]);
                    test_activated = false;
                }
            }
            if(Mouse::is_over(star_icon->get_rect())) {
                if(Mouse::is_pressed(1) && star_activated == false) {
                    star_icon->get_image()->copy( * Icon::get["star"]);
                    star_activated = true;
                } else if(Mouse::is_pressed(1) && star_activated == true) {
                    star_icon->get_image()->copy( * Icon::get["star_outline"]);
                    star_activated = false;
                }
            }*/
            /*if(Mouse::is_over(_icon->get_rect())) {
                if(Mouse::is_pressed(1) && _activated == false) {
                    _icon->get_image()->copy(*Icon::get[""]);
                    _activated = true;
                }
                else if(Mouse::is_pressed(1) && _activated == true) {
                    _icon->get_image()->copy(*Icon::get["_outline"]);
                    _activated = false;
                }
            }
            if(Mouse::is_over(_icon->get_rect())) {
                if(Mouse::is_pressed(1) && _activated == false) {
                    _icon->get_image()->copy(*Icon::get[""]);
                    _activated = true;
                }
                else if(Mouse::is_pressed(1) && _activated == true) {
                    _icon->get_image()->copy(*Icon::get["_outline"]);
                    _activated = false;
                }
            }
            if(Mouse::is_over(_icon->get_rect())) {
                if(Mouse::is_pressed(1) && _activated == false) {
                    _icon->get_image()->copy(*Icon::get[""]);
                    _activated = true;
                }
                else if(Mouse::is_pressed(1) && _activated == true) {
                    _icon->get_image()->copy(*Icon::get["_outline"]);
                    _activated = false;
                }
            }*/
            //if(Mouse::is_over(_icon->get_rect())) _icon->get_image()->set_color(242, 100, 17); else _icon->get_image()->set_color(default_icon_color);
            //cart_button->draw();
            ////////////////
            //unsigned int height = (unsigned int)wallet->get_monero_wallet()->get_height_by_date (2021, 10, 01);
            //std::cout << "Restore height by date: " << height << std::endl;
            // returns the current daemon height
            //std::cout << "Daemon height: " << wallet->get_monero_wallet()->get_daemon_height 	( 		) << std::endl;
            //std::cout << "Last height processed: " << wallet->get_monero_wallet()->get_height 	( 		) << std::endl;
            //std::cout << "balance: " << wallet->get_balance() << std::endl;
            //if(!synced) {
            //    //std::cout << "daemon is not fully synced with the network\n";
            //    if(wallet->get_monero_wallet()->is_daemon_synced ()) { 
            //        std::cout << "daemon is now fully synced with the network\n"; // WTF?
            //        synced = true;    
            //    }
            //}
            /*
            // draw ui
            user_button->draw(); // user
            order_button->draw(); // order history
            
            status_icon->draw();
            //test_icon->draw();
            //star_icon->draw();*/
            /////////////////////////////////////
            /////////////////////////////////////
            // DON'T open database in loop!!!
            // each time an item is added or removed or qty_changed from the cart, update this string 
            // causes crash
            //cart_button->get_label()->set_string(std::to_string(Cart::get_singleton()->get_total_quantity()));
            //cart_button->get_label()->set_color(); // (cart_qty >= 100) = red(255, 0, 0), (cart_qty >= (100 / 2)) = yellow(255,191,0), (cart_qty <= ((100 / 2) - 1)) = white);
            cart_button->get_label()->set_relative_position(20, (cart_button->get_height() - 10/*cart_label.get_height()*/) / 2);
            cart_button->get_image()->set_relative_position(cart_label.get_relative_x() + (cart_label.get_string().length() * 10) + 10, (cart_button->get_height() - cart_icon.get_height_scaled()) / 2);// "(cart_label.get_string().length() * 10)" could be replaced with the entire label's width
            cart_button->draw(window.get_client_width() - cart_button->get_width() - 20, 20);//(search_bar->get_x() + (search_bar->get_width() + search_button->get_width()/* + 1*/) + 20, 20);//std::cout << "cart_button pos: " << cart_button->get_position() << std::endl;//600 + 20
            /////////////////////////////////////
            // if user is a seller, display customer orders
            user_button->draw(cart_button->get_x() - user_button->get_width() - 1, 20); //(600, 250);                
            /////////////////////////////////////
            order_button->draw(user_button->get_x() - order_button->get_width() - 1, 20);            
            /////////////////////////////////////
            // on logout_button pressed, return to the login menu
            if(logout_button->is_pressed()) {
                home_menu = false;
                login_menu = true;
            }
            logout_button->draw(order_button->get_x() - logout_button->get_width() - 1, 20);
            /////////////////////////////////////
            /////////////////////////////////////
            /////////////////////////////////////                        
            /////////////////////////////////////
            ////search_suggestion_list->draw(search_bar->get_x(), search_bar->get_y() + search_bar->get_height());
            //std::cout << "search_bar cursor position: " << search_bar->get_cursor_position() << std::endl;
            /////////////////////////////////////
            // on search_button pressed
            if(search_button->is_pressed()) {
                if(!search_bar->is_empty()) std::cout << "searching for \033[1;93m" << search_bar->get_text() << "\033[0m" << std::endl;
                if(search_bar->has_focus()) search_bar->set_focus(false);
            }
            // on enter pressed (with focus on search_bar)
            // can search for products, users (sellers), etc.
            if(search_bar->has_focus() && Keyboard::is_pressed(DOKUN_KEY_RETURN)) {
                if(!search_bar->is_empty()) std::cout << "searching for \033[1;93m" << search_bar->get_text() << "\033[0m" << std::endl;
                search_bar->set_focus(false);
            }
            // adjust search_bar size on window resize
            // edit has a bug when its resized :/
            // everytime the edit's size changes, the cursor positions gets messed up and everything is ruined
            search_bar->set_size((window.get_client_width() < 1280) ? 400 : (window.get_client_width() / 4) * 2, 40);//search_bar->set_size((window.get_client_width() > 1280) ? (window.get_client_width() / 4) * 2 : 400, 40);//search_bar->set_text(search_bar->get_text());
            search_bar->draw(150, 20);//((window.get_client_width() / 2) - ((search_bar->get_width() + search_button->get_width() + 1) / 2), 60); // center of window // search_bar and search_button will act as one single GUI element
            search_button->draw(search_bar->get_x() + search_bar->get_width() + 1, search_bar->get_y()); // leave a tiny gap between sbar and sbutton
            /////////////////////////////////////
            // check for any incoming orders (pending)
            // use timer to check every x seconds
            int seconds = 60;//300;//600; //1800secs = 30mins; 1200secs = 20mins; 900secs = 15mins; 600secs = 10mins; 300secs = 5mins;
            if(order_listener_timer.increment() >= seconds) { 
                if(user->is_seller()) {
                    std::cout << seconds << " seconds has passed" << std::endl;
                    // maybe add a notification icon
                    // listen for any incoming customer orders (pending)
                    static_cast<Seller *>(user)->update_customer_orders();
                    // reset timer to 0
                    order_listener_timer.reset();
                    // start timer once more
                    order_listener_timer.start();
                }
            }
            ////////////////
            // sync status
            sync_label.draw(0 + 20, window.get_client_height() - 20);
            sync_box->draw(sync_label.get_x() + sync_label.get_width(), sync_label.get_y());
            ////////////////
            // if esc is pressed
            if(Keyboard::is_pressed(DOKUN_KEY_ESCAPE) && !message_box.is_visible()) {
                // do you wish to exit the program
                window.destroy();
            }
            //if(Mouse::is_down()) std::cout << "mouse down\n";//is_pressed => locked = true
            //if(Mouse::is_released(1)) std::cout << "mouse released\n";//locked = false;//is_locked && mouse_down
            message_box.center(window.get_client_width(), window.get_client_height());
            message_box.draw(); // draw msgbox if visible
            ////////////////
            ////////////////
            window.update();
        } // home_menu
        window.update(); // main loop
        //glfwPollEvents();//glfwWaitEvents (void);//glfwPostEmptyEvent();        
    } // main loop/thread
    //wallet->daemon_close(); // kill daemon
    window.destroy();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*db->close();
    delete db;*/    
    neroshop::close(); // close neroshop when done
    return 0;
}
