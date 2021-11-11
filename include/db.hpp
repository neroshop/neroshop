// filename: db.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef DATA_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define DATA_HPP_NEROSHOP
#define SQLITE3_TAG "\033[1;36m[sqlite3]:\033[0m "
#define SQLITE3_TAG_ERR "\033[1;36m[sqlite3]:\033[0;91m "
#define LUA_TAG "\033[1;94m[lua]:\033[0m "
#include <iostream>
#include <cstdarg>
#include <fstream>
#include <sstream>
#include <utility> // std::pair
// neroshop
#include "debug.hpp"
// dokun-ui
#include <script.hpp>
#include "utility.hpp"
#include "string.hpp"
#include "logger.hpp"
// sqlite3
extern "C" {      // run as c code
#include <sqlite3.h>
}

namespace neroshop {
class DB { // use sqlite browser for viewing SQLite DBs: https://sqlitebrowser.org/
public:
    DB();
	DB(const std::string& file_name);
	~DB();
	// sqlite
	bool open(const std::string& file_name); // opens and closes database
	void close();
	void execute(const std::string& sql); // executes sql commands
	void table(const std::string& table_name, bool auto_inc = true); // creates a table// table players
	void column(const std::string& table, const std::string& method, const std::string& column_name, std::string column_type="NULL"); // creates, deletes, or modifies a new column property// ADD, DROP, MODIFY
	void insert(const std::string& table, const std::string& variable, const std::string& value); // sets the column values
	void select(const std::string& table, std::string variable = "", std::string condition=""); // select tables to display// selects from table
	void update(const std::string& table, const std::string& variable, const std::string& value, std::string cond=""); // update
    void drop(const std::string& table, std::string condition = ""); // destroy column or table
	void delete_from(const std::string& table, std::string condition = ""); // delete
	void truncate(const std::string& table_name); // delete all rows from table
	void vacuum();
	void rename_table(const std::string& table, const std::string& new_name); // rename table
	void index(const std::string& index_name, const std::string& table_name, const std::string& column_list);
	void drop_index(const std::string& index_name);
	bool table_exists(const std::string& table_name) const;
	bool column_exists(const std::string& table_name, const std::string& column_name) const;
	// configure db int sqlite3_db_config(sqlite3*, int op, ...);
	// shut down sqlite    int sqlite3_shutdown(void);
	void free(); // free memory used by database// int sqlite3_db_release_memory(sqlite3*);
    //sqlite3_free_table();	
    sqlite3 * get_handle() const; // returns database handle
    static std::string to_sql_string(const std::string& value); // returns a string with quotes around it
    //sqlite3_stmt * get_stmt() const;
    static std::string get_sqlite_version(); // printf("%s\n", sqlite3_libversion());	
	std::string get_database_name() const; // const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName);
	// returns connection status of db
	//void get_status(); // int sqlite3_db_status(sqlite3*, int op, int *pCur, int *pHiwtr, int resetFlg);
	unsigned int row_count(const std::string& table_name) const;
	std::string get_select(const std::string& table, std::string variable = "", std::string condition="") const;
	std::string get_column_name(const std::string& table) const; // returns column name // const char *sqlite3_column_name(sqlite3_stmt*, int N);
	void * get_column_null(const std::string& table, std::string variable = "", std::string condition="") const;
	int get_column_integer(const std::string& table, std::string variable = "", std::string condition="") const;
	double get_column_real(const std::string& table, std::string variable = "", std::string condition="") const;
	std::string get_column_text(const std::string& table, std::string variable = "", std::string condition="") const;
	void * get_column_blob(const std::string& table, std::string variable = "", std::string condition="") const;
	//void get_row(); 
	// check if statement is complete
	// int sqlite3_complete(const char *sql);
	// CHECK if database is locked: https://www.sqlite.org/c3ref/db_readonly.html
	// getters
	// useful_links
	// quickstart - https://www.sqlite.org/quickstart.html
	// tutorial - https://www.sqlite.org/cintro.html | https://www.sqlite.org/c3ref/intro.html
	// reference - https://www.sqlite.org/capi3ref.html // function_list - https://www.sqlite.org/c3ref/funclist.html
	// datatypes - https://www.sqlite.org/datatype3.html
	// foreign keys - https://sqlite.org/foreignkeys.html
	//////////////////////////////////
	// Lua
	static bool load_config(lua_State *L);
	static bool create_config();
	static lua_State * get_lua_state();
	//////////////////////////////////
	// TEXT, BINARY
	bool text_to_binary(const std::string& input, const std::string& output);
	bool binary_to_text(const std::string& input, const std::string& output);
	//////////////////////////////////
	// JSON
	void json_init();
	void json_parse(const char * json);
	bool json_load(const std::string& file_name);
	bool json_write(const std::string& output_file);
	std::string json_stringify(); // prints out the json file content
	
	void json_start_object();
	void json_end_object();
	void json_array_start(); // can add a value in between attay_start and array_end
	void json_array_end(); // arr_start()   number(1) number(2) number(3) arr_end()  array with 3 numbers		
	void json_key(const std::string& key_name); // outputs a key
	void json_int(int value); // followed by a value
	void json_uint(unsigned int value);
	void json_double(double value);
	void json_bool(bool value);
	void json_string(const std::string& value);
	void json_string(const String& value);
	void json_null();
	void json_set(const std::string& key, const std::string& value);
	void json_set(const std::string& key, int value, bool is_bool = false); // or bool
	void json_set(const std::string& key, double value);
	void json_set(const std::string& key, const String& value);
	void json_set(const std::string& key, void * value);
	
	void json_get(const std::string& key);
	void json_get(const std::string& key, int * value);
	void json_get(const std::string& key, double * value);
	void json_get(const std::string& key, String& value);
	void json_get(const std::string& key, void * value);
	
	bool json_is_object(); // check if document is object
	bool json_has_member(const std::string& key);
	bool json_is_null(const std::string& key);
	bool json_is_boolean(const std::string& key);
	bool json_is_object(const std::string& key);
	bool json_is_array(const std::string& key);
	bool json_is_string(const std::string& key);
	bool json_is_number(const std::string& key);
	bool json_is_integer(const std::string& key);
	bool json_is_double(const std::string& key);
	//////////////////////////////////
	bool is_text() const; // including binary
	bool is_sql() const;
	bool is_lua() const;
	bool is_json() const;
private:
	sqlite3 * db;
	static lua_State *lua_state;
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
};
}
// sqlite scalability/load: https://stackoverflow.com/questions/1546947/maximum-number-of-rows-in-a-sqlite-table
                            // https://stackoverflow.com/a/21396826 ***
                            // https://stackoverflow.com/a/11145704                                    
                            // https://www.sqlite.org/limits.html (281TB MAX_DB size, 2000 SQLITE_MAX_COLUMN, )
                            
// varchar(255): https://stackoverflow.com/questions/2340639/why-historically-do-people-use-255-not-256-for-database-field-magnitudes
                 // https://stackoverflow.com/a/44145129
                 // https://forum.xojo.com/t/is-there-a-max-character-length-of-a-sqlite-field-text-varchar/40087/2
// uUnlike SQL, SQLite ignores varchar(255) and does not impose any length restrictions. varchar(255) is auto converted to TEXT
// SQLite datatypes: NULL, INTEGER, REAL, TEXT, BLOB
// Primary keys provide a unique ID for every row in a database table. It is advised that every table has one.
/*
usage:
	DB * data = new DB();
	if(!data->open("dokun.out"))
		std::cout << Logger("Could not open sql database", 0) << std::endl;
	// table
	data->table("Player"); // create table Player
	data->column("Player", "ADD", "name", "TEXT");        // add variable 'name'  to table Player
	data->column("Player", "ADD", "level", "INTEGER"); // add variable 'level' to table Player
	// column 1
	data->insert("Player", "name, level", "'Sid', 20" ); // initialize column 1 variables
	// column 2
	data->insert("Player", "name, level", "'Jack', 16"); // initialize column 2 variables
	// SET name WHERE id = 1
	data->update("Player", "name", "'Sid Donut'", "id = 1"); // This will change name if id = 1
	data->update("Player", "name", "'Sid Donut'");           // This will change all names to 'Sid Dokugawa'
	// delete column
	data->drop("Player", "id = 2"); // delete where id = 2
	data->select("Player", "name", "id=2"); // "SELECT name FROM Player WHERE id=2;"// shows player's name where the id=2
	
	data->drop("Player"); // delete table
	data->close();        // close database when done with it
	///////////////////////////////////////
	// add column "address" then update it (where id=1)
    db->column("Users", "ADD", "address", "TEXT");
    db->update("Users", "address", "'56WG1qKFWXtAuLzxB6RqLe17uwENMRgNHDqoc8mp29Q2dCcBZXmBEocMiMBLCd1x3Q2as9KuNCpK2KgBtcTWT1jA8EqkcoT'", "id=1");
    // drop (delete) column "address"
    db->column("Users", "DROP", "address");
*/
#endif
