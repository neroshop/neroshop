#include "../include/db.hpp"

neroshop::DB::DB()
{}
////////////////////
neroshop::DB::DB(const std::string& file_name) : DB() // delegating constructor
{
	if(!open(file_name)) {
		NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR << "Could not open db - " + String::to_string(sqlite3_errmsg(db)) + " (" + file_name + ")" << std::endl;
        std::exit(1);
    }
}
////////////////////
neroshop::DB::~DB() {
    close();
}
////////////////////
lua_State * neroshop::DB::lua_state (luaL_newstate()); // static lua_state
////////////////////
bool neroshop::DB::open(const std::string& file_name) // create a data base; works!
{
	if(sqlite3_open(file_name.c_str(), &db)) {
		close();
		return false;
	}
	return true;
}
////////////////////
void neroshop::DB::close() {
    if(!db) return;
	sqlite3_close(db);
	db = nullptr; // set to null after deletion (to confirm that it has been properly deleted)
}
////////////////////
////////////////////
void neroshop::DB::execute(const std::string& sql) // execute an sql statement; works !
{
    if(!db) throw std::runtime_error("database is not connected");
	char * errmsg = 0;
	int error = sqlite3_exec(db, sql.c_str(), neroshop::DB::callback, 0, &errmsg);
	if (error != SQLITE_OK)
	{
		NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(errmsg) << "\033[0m" << std::endl;
		sqlite3_free(errmsg);
	}
	if(error == SQLITE_LOCKED) {NEROSHOP_TAG_OUT std::cout << "dammit, this database is locked wtf!!!" << std::endl;} // remove this LOL
}
////////////////////
void neroshop::DB::table(const std::string& table, bool auto_inc) // creates a new table; works !
{
	std::string sql = "CREATE TABLE IF NOT EXISTS table_name(id  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT);";
	// set table name
    sql = String::swap_first_of(sql, "table_name", table);
	if(auto_inc == false)
		sql = String::remove_first_of(sql, "AUTOINCREMENT");
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute sql
	execute(sql.c_str());
}
////////////////////
void neroshop::DB::column(const std::string& table, const std::string& method, const std::string& column_name, std::string column_type) // creates, deletes, or modifies a column; works!
{
	std::string sql = "ALTER TABLE table_name method column_name column_type;";
	// add a column to table 
	sql = String::swap_first_of(sql, "table_name", table);
	sql = String::swap_first_of(sql, "method", method   ); // ADD, DROP, MODIFY
	sql = String::swap_first_of(sql, "column_name", column_name );
	// if method is DROP, remove column_type (not necessary when DROPing a column)
	if(String::lower(method) == "drop")
	    sql = String::remove_first_of(sql, "column_type");
	// if method is not DROP, then swap the "column_type" with the appropriate type
	if(String::lower(method) != "drop")
	    sql = String::swap_first_of(sql, "column_type",   column_type ); // NULL by default
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute 
	execute(sql.c_str());	
} 
// db->column("Player", "ADD", "name", "TEXT"); 
// db->column("Player", "ADD", "level", "INTEGER(3)");
// db->column("Users", "DROP", "pw_hash"); // no column datatype required for DROP
// use UPDATE to update columns! INSERT only adds a new row and id!!
////////////////////
void neroshop::DB::insert(const std::string& table, const std::string& variable, const std::string& value) // sets the value of a column (column must exist first); works!
{  
    std::string sql = "INSERT INTO table_name (variable)" 
	" VALUES (value);";
    // set table variable
  	sql = String::swap_first_of(sql, "table_name", table   );
	sql = String::swap_first_of(sql, "variable"  , variable);
	sql = String::swap_first_of(sql, "value"     , /* "'" + */ value /* + "'" */);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif	
	// execute
	execute(sql.c_str());
} // db->insert("Player", "name, level", "'Jack', 1"); -> sets the column 'name' to 'Jack'
////////////////////
void neroshop::DB::select(const std::string& table, std::string variable, std::string condition) // selects variable from a table // works!
{
	std::string sql = "SELECT * " // "SELECT * "
	"FROM table_name "
	"WHERE condition;";
	// select from table
	sql = String::swap_first_of(sql, "table_name", table); // Users
	if (!variable.empty()) 
	    sql = String::swap_first_of(sql, "*",  variable); // name
	if(condition.empty())
		sql = String::remove_first_of(sql, "WHERE condition");
	if(!condition.empty())
	    sql = String::swap_first_of(sql, "condition",  condition);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute
	execute(sql.c_str());
} // db->select("Users", "name", "id=2"); // shows user's name where the id=2
////////////////////
void neroshop::DB::update(const std::string& table, const std::string& variable, const std::string& value, std::string condition) // updates a variable; works!
{
	std::string sql = "UPDATE table_name "
	"SET variable = value " 
	"WHERE condition;     ";
	// update variable 
	sql = String::swap_first_of(sql, "table_name", table   );
	sql = String::swap_first_of(sql, "variable"  , variable);
	sql = String::swap_first_of(sql, "value"     , value   );
	if(condition.empty())
		sql = String::remove_first_of(sql, "WHERE condition");
	if(!condition.empty())
	    sql = String::swap_first_of(sql, "condition",  condition);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute
	execute( sql.c_str() );
} // db->update("Users", "name", "'jack'", "id = 1"); // UPDATE Users SET name='jack' WHERE id=1
////////////////////
void neroshop::DB::drop(const std::string& table, std::string condition) // works!
{
	std::string sql;
	if(condition.empty())
	{
		// delete table
	    sql = "DROP TABLE IF EXISTS table_name;";
	    sql = String::swap_first_of(sql, "table_name", table);
	}
	if(!condition.empty())
	{
		// delete column or whatever
		sql = "DELETE FROM table_name "
		"WHERE condition;";
		sql = String::swap_first_of(sql, "table_name", table);
		sql = String::swap_first_of(sql, "condition", condition);
	}
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute
	execute(sql.c_str());
} 
// db->drop("Users"); // drops table Users
// db->drop("Users", "id=2"); // deletes column where id=2 
////////////////////
////////////////////
void neroshop::DB::index(const std::string& index_name, const std::string& table_name, const std::string& column_list) // ensures that columns will not be duplicated // works!
{
    std::string sql = "CREATE UNIQUE INDEX index_name ON table_name (column_list);";
    sql = String::swap_first_of(sql, "index_name", index_name  );
    sql = String::swap_first_of(sql, "table_name", table_name  );
    sql = String::swap_first_of(sql, "column_list", column_list);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
    // execute
    execute(sql.c_str());
} 
// db->index("idx_users_hash", "Users", "pw_hash"); // like an index of a book // SQLite will issue an error message indicating that the unique index has been violated if a row is not unique when inserting new data to the table
// db->index("idx_users_name", "Users", "first_name, last_name"); // multiple unique columns
////////////////////
void neroshop::DB::drop_index(const std::string& index_name) // works!
{
    std::string sql = "DROP INDEX IF EXISTS index_name;"; // drops an index if it exists
    sql = String::swap_first_of(sql, "index_name", index_name  );
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
    // execute
    execute(sql.c_str());    
} // db->drop_index("idx_users_hash");
////////////////////
////////////////////
void neroshop::DB::rename_table(const std::string& table, const std::string& new_name) // works!
{
	std::string sql = "ALTER TABLE table_name " 
	"RENAME TO new_name;";
    // rename table
	sql = String::swap_first_of(sql, "table_name", table   );
	sql = String::swap_first_of(sql, "new_name"  , new_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute
	execute(sql.c_str());
} // db->rename_table("Users", "Sellers");
////////////////////
////////////////////
void neroshop::DB::delete_from(const std::string& table, std::string condition) { // not tested yet
    std::string sql = "DELETE FROM table_name"
    "WHERE condition;";
    sql = String::swap_first_of(sql, "table_name", table   );
    if(condition.empty())
        sql = String::remove_first_of(sql, "WHERE condition");
    if(!condition.empty())
        sql = String::swap_first_of(sql, "condition", condition);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
	// execute
	execute(sql.c_str());    
}
////////////////////
void neroshop::DB::truncate(const std::string& table_name) { // primary_key id will stay unique so if you make a table with 9 rows then truncate it, when you insert into the table again, the id will start from 10 // https://www.tutorialspoint.com/sqlite/sqlite_truncate_table.htm
    std::string sql = "DELETE FROM table_name;";
    sql = String::swap_first_of(sql, "table_name", table_name   );
    //sql.append(" VACUUM;");// to clear unused space
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
    execute(sql.c_str());
}
////////////////////
void neroshop::DB::vacuum() { // reduces db size by clearing empty spaces
    std::string sql = "VACUUM;";
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
    execute(sql.c_str());    
}
////////////////////
////////////////////
std::string neroshop::DB::to_sql_string(const std::string& value) {
    return "\"" + value + "\"";
}
////////////////////
////////////////////
////////////////////
////////////////////
sqlite3 * neroshop::DB::get_handle() const {
    return db;
}
////////////////////
//sqlite3_stmt * neroshop::DB::get_stmt() const {
//    return nullptr;//stmt;
//}
////////////////////
////////////////////
std::string neroshop::DB::get_sqlite_version() {
    return sqlite3_libversion();
} // NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG << "sqlite3 v" << neroshop::DB::get_sqlite_version() << std::endl;
////////////////////
////////////////////
std::string neroshop::DB::get_select(const std::string& table, std::string variable, std::string condition) const 
{
	std::string sql = "SELECT * FROM table_name WHERE condition;";
	// select from table
	sql = String::swap_first_of(sql, "table_name", table); // Users
	if (!variable.empty()) 
	    sql = String::swap_first_of(sql, "*",  variable); // name
	if(condition.empty())
		sql = String::remove_first_of(sql, "WHERE condition");
	if(!condition.empty())
	    sql = String::swap_first_of(sql, "condition",  condition);
	// if variable is empty, remove everything, but SELECT
	if(variable.empty())
	    sql = String::remove_first_of(sql, "* FROM"); // SELECT  SQLITE_VERSION() ;
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif	    
    return sql; // return sql string (result)
    // don't execute anything, just getting the result, that's all ...
}
////////////////////
////////////////////
std::string neroshop::DB::get_column_name(const std::string& table) const 
{
    if(!db) throw std::runtime_error("database is not connected");
    sqlite3_stmt * stmt;
    std::string sql = get_select(table);
    sql = String::swap_first_of(sql, "table_name", table);
    // ...
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); // db, const char *zSql, sql.length()/int nByte, sqlite3_stmt **ppStmt, const char **pzTail // https://www.sqlite.org/c3ref/prepare.html
    if(result != SQLITE_OK) {
        NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(sqlite3_errmsg(db)) << "\033[0m" << std::endl;
    }
    // step
    result = sqlite3_step(stmt);
    // print column text
    if (result == SQLITE_ROW) {
        return (const char *)sqlite3_column_name(stmt, 0/*int N*/);
    }
    return "";
}
////////////////////
void * neroshop::DB::get_column_null(const std::string& table, std::string variable, std::string condition) const {
    return nullptr;
}
////////////////////
int neroshop::DB::get_column_integer(const std::string& table, std::string variable, std::string condition) const {
    if(!db) throw std::runtime_error("database is not connected");
    sqlite3_stmt * stmt;
    std::string sql = get_select(table, variable, condition);
    //NEROSHOP_TAG_OUT std::cout << sql << std::endl; // debug
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); // prepare // db, const char *zSql, sql.length()/int nByte, sqlite3_stmt **ppStmt, const char **pzTail // https://www.sqlite.org/c3ref/prepare.html
    if(result != SQLITE_OK) {
        NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(sqlite3_errmsg(db)) << "\033[0m" << std::endl;
        //close();
    }
    result = sqlite3_step(stmt);// step
    // return column text
    if (result == SQLITE_ROW) {
        int column_type = sqlite3_column_type(stmt, 0);
        if(column_type != SQLITE_INTEGER) {NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR << "column type is not an INTEGER" << "\033[0m" << std::endl;}
        return (int)sqlite3_column_int(stmt, 0);/*sqlite3_stmt*, int iCol*/
    }
    // destroy
    sqlite3_finalize(stmt);//sqlite3_stmt *pStmt        
    return 0;
}
////////////////////
double neroshop::DB::get_column_real(const std::string& table, std::string variable, std::string condition) const {
    if(!db) throw std::runtime_error("database is not connected");
    sqlite3_stmt * stmt;
    std::string sql = get_select(table, variable, condition);
    //NEROSHOP_TAG_OUT std::cout << sql << std::endl; // debug
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); // prepare // db, const char *zSql, sql.length()/int nByte, sqlite3_stmt **ppStmt, const char **pzTail // https://www.sqlite.org/c3ref/prepare.html
    if(result != SQLITE_OK) {
        NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(sqlite3_errmsg(db)) << "\033[0m" << std::endl;
        //close();
    }
    result = sqlite3_step(stmt);// step
    // return column text
    if (result == SQLITE_ROW) {
        int column_type = sqlite3_column_type(stmt, 0);
        if(column_type != SQLITE_FLOAT) {NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR << "column type is neither a FLOAT OR DOUBLE" << "\033[0m" << std::endl;}
        return (double)sqlite3_column_double(stmt, 0);/*sqlite3_stmt*, int iCol*/
    }
    // destroy
    sqlite3_finalize(stmt);//sqlite3_stmt *pStmt       
    return 0.0;
}
////////////////////
// https://zetcode.com/db/sqlitec/
//////////////////// //int sqlite3_column_type(stmt, 0); // returns a SQLite datatype: SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or SQLITE_NULL
std::string neroshop::DB::get_column_text(const std::string& table, std::string variable, std::string condition) const // works!
{
    sqlite3_stmt * stmt;
    std::string sql = get_select(table, variable, condition);
    //NEROSHOP_TAG_OUT std::cout << sql << std::endl; // debug
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); // prepare // db, const char *zSql, sql.length()/int nByte, sqlite3_stmt **ppStmt, const char **pzTail // https://www.sqlite.org/c3ref/prepare.html
    if(result != SQLITE_OK) {
        NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(sqlite3_errmsg(db)) << "\033[0m" << std::endl;
        //close();
    }
    result = sqlite3_step(stmt);// step
    // return column text
    if (result == SQLITE_ROW) {
        int column_type = sqlite3_column_type(stmt, 0);
        if(column_type != SQLITE_TEXT) {NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR << "column type is not a TEXT" << "\033[0m" << std::endl;}
        return (const char *)sqlite3_column_text(stmt, 0);/*sqlite3_stmt*, int iCol*/
    }
    // destroy
    sqlite3_finalize(stmt);//sqlite3_stmt *pStmt    
    return "";
}
////////////////////
void * neroshop::DB::get_column_blob(const std::string& table, std::string variable, std::string condition) const 
{
    sqlite3_stmt * stmt;
    std::string sql = get_select(table, variable, condition);
    //NEROSHOP_TAG_OUT std::cout << sql << std::endl; // debug
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); // prepare // db, const char *zSql, sql.length()/int nByte, sqlite3_stmt **ppStmt, const char **pzTail // https://www.sqlite.org/c3ref/prepare.html
    if(result != SQLITE_OK) {
        NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(sqlite3_errmsg(db)) << "\033[0m" << std::endl;
        //close();
    }
    result = sqlite3_step(stmt);// step
    // return column text
    if (result == SQLITE_ROW) {
        int column_type = sqlite3_column_type(stmt, 0);
        if(column_type != SQLITE_BLOB) {NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR << "column type is not a BLOB" << "\033[0m" << std::endl;}
        return (void *)sqlite3_column_blob(stmt, 0);/*sqlite3_stmt*, int iCol*/
    }
    // destroy
    sqlite3_finalize(stmt);//sqlite3_stmt *pStmt      
    return nullptr;
}
////////////////////
unsigned int neroshop::DB::row_count(const std::string& table_name) const {
    return get_column_integer(table_name, "COUNT(*)");
}
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::DB::table_exists(const std::string& table_name) const {
    std::string sql_cond = "type='table' AND name='table_name';";//SELECT count(*) FROM sqlite_master WHERE type='table' AND name='table_name';
    sql_cond = String::swap_first_of(sql_cond, "table_name", table_name);
    bool exists = get_column_integer("sqlite_master", "count(*)", sql_cond);
    return exists;
}
////////////////////
// will print [sqlite3]: no such column: role_id" if column does not exist
// find a proper way to check if column exists
bool neroshop::DB::column_exists(const std::string& table_name, const std::string& column_name) const {
    bool exists = get_column_integer(table_name, "count(" + column_name + ")");
    return exists;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
int neroshop::DB::callback(void *not_used, int argc, char **argv, char **azcolname)
{
    int i;
    for(i = 0; i < argc; i++) {
	    std::cout << SQLITE3_TAG << azcolname[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;  // printf("%s = %s\n", azcolname[i], argv[i] ? argv[i] : "nullptr");
    }
    std::cout << std::endl;
    return 0;	
}
////////////////////
// PostgreSQL
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// Lua
////////////////////
bool neroshop::DB::create_config() { // good!
    std::string user = System::get_user();
    std::string text = "-- config.lua\n"
    "localhost = \"127.0.0.1\"\n"
    "neroshop = {\n"
    //"    --general = {\n"
    "    currency = \"usd\",\n" // currency that will be displayed alongside xmr: 0.045000000000 XMR ($10 USD)
    //"    theme = \"dark\",\n" // dark, light, default
    //"    --}\n"
    "    window = {\n"
    "        width = 1280,\n"
    "        height = 720,\n"
    //"        x = 200,\n"
    //"        y = 200,\n"
    "        mode = 0, -- 0 = window_mode or 1 = fullscreen\n"
    "    },\n"
    "    daemon = {\n"
    "        data_dir = \"/home/user/.bitmonero\", -- /path/to/lmdb\n" // change this
    "        network_type = \"stagenet\",\n"
    "        ip = localhost, --\"0.0.0.0\" means that other wallets can connect to your node\n"
    "        port = \"38081\",\n" // 127.0.0.1:38081
    "        confirm_external_bind = false, -- set to true if binding to 0.0.0.0 -- confirms you want to allow connections from wallets that are not on this system\n"
    "        restricted_rpc = true, -- restricts what information can be requested from the daemon by any client\n"//"        daemon_address = \"\",--\"node.moneroworld.com:18089\", -- connect to a remote node (in case you are unable to sync the entire blockchain)\n"
    "        remote = false, -- set this true to connect to a remote node\n"
    "    },\n"
    "    wallet = {\n"
    "        file = \"\", -- include '.keys' ext\n"
    "        restore_height = 0, -- block height or date (YYYY-MM-DD)\n"    
    "    },\n"
    "}\n";
    //"    \n"
    // swap data_dir with user
#ifdef __gnu_linux__ // works!    
    text = String::swap_first_of(text, "/home/user/.bitmonero", ("/home/" + user + "/.bitmonero"));
#endif    
    // "/home/<user>/.config/neroshop"
    std::string neroshop_config_path = "/home/" + user + "/.config/neroshop"; // System::get_user()
    // "/home/<user>/.config/neroshop/config.lua"
    std::string neroshop_config_name = neroshop_config_path + "/config.lua";
    // if file already exists, no need to create it again
    if(File::exists(neroshop_config_name)) return false; // false because it will not be created
    // check if script works before saving
    if(luaL_dostring(lua_state, text.c_str()) != 0) {
		NEROSHOP_TAG_OUT std::cout << LUA_TAG << "\033[0;31m" << "invalid Lua code" << "\033[0m" << std::endl;
		lua_error(lua_state);
		return false; // exit function so it does not save text
	}
    // if path does not exist
    if(!File::is_directory(neroshop_config_path)) 
    {   // create the path
        NEROSHOP_TAG_OUT std::cout << "\033[0;33m" << "path \"" << neroshop_config_path << "\" does not exist, but we'll create it for you :)" << "\033[0m" << std::endl;
        if(!File::make_dir(neroshop_config_path)) {NEROSHOP_TAG_OUT std::cout << "\033[0;31m" << "failed to make the path :(" << "\033[0m" << std::endl; return false;}
        NEROSHOP_TAG_OUT std::cout << "\033[1;97;49m" << "created path \"" << neroshop_config_path << "\"" << "\033[0m" << std::endl;
    }
    // if path exists, but the file is missing or deleted
    if(!File::exists(neroshop_config_name)) {
        // create config file (one time)
        std::ofstream cfg;
        cfg.open (neroshop_config_name, std::ios::out | std::ios::trunc);
        cfg << text << "\n"; // write to file
        cfg.close();
        NEROSHOP_TAG_OUT std::cout << "\033[1;97;49m" << "created file \"" << neroshop_config_name << "\"" << "\033[0m" << std::endl;  
    }
    return true;
}
////////////////////
bool neroshop::DB::load_config(lua_State *L)
{
    std::string user = System::get_user();
    // "/home/<user>/.config/neroshop"
    std::string neroshop_config_path = "/home/" + user + "/.config/neroshop";
    // "/home/<user>/.config/neroshop/config.lua"
    std::string neroshop_config_name = neroshop_config_path + "/config.lua";
    Script script;
    if(!script.load(lua_state, neroshop_config_name)) {return false;}
    NEROSHOP_TAG_OUT std::cout << "\033[1;94;49m" << "loaded script \"" << script.get_file() << "\"" << "\033[0m" << std::endl;
    return true; // default return-value
}
////////////////////
void neroshop::DB::edit_config(const std::string& old_str, const std::string& new_str) { // bad - needs some fixing :/
    ///////////////////////////////// get config file path and name
    std::string user = System::get_user();
    // "/home/<user>/.config/neroshop"
    std::string neroshop_config_path = "/home/" + user + "/.config/neroshop";
    // "/home/<user>/.config/neroshop/config.lua"
    std::string neroshop_config_name = neroshop_config_path + "/config.lua";
    ///////////////////////////////// dump contents
	std::ifstream file(neroshop_config_name); // file_name.c_str()
	if(!file.is_open()) {
	    std::cout << "Could not open " << neroshop_config_name << std::endl;
		return;// false;
	}	
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents
	file.close(); // close file when done retrieving contents    
    ///////////////////////////////// look for then edit specific file content
	if(String::contains(stream.str(), old_str)) {   
		 std::string contents = String::swap_first_of(stream.str(), old_str, new_str);
        // rewrite config file
        std::ofstream cfg;
        cfg.open (neroshop_config_name, std::ios::out | std::ios::trunc);
        cfg << contents << "\n"; // write to file
        cfg.close();
        NEROSHOP_TAG_OUT std::cout << "\033[1;97;49m" << "rewrote file \"" << neroshop_config_name << "\"" << "\033[0m" << std::endl;      
    }
    /////////////////////////////////
}
// 1. dump file contents into string
// 2. use String to add or remove specific contents
// 3. re-write config
////////////////////
lua_State * neroshop::DB::get_lua_state()// const 
{
    return lua_state;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::DB::text_to_binary(const std::string& input, const std::string& output) // (input)file_to_convert_to_binary, (output)binary_file
{
	std::ifstream rfile(input.c_str());
	if(!rfile.is_open())
	{
	    return false;
	}	
	std::stringstream stream;
	stream << rfile.rdbuf(); // dump file contents	
	
	std::string content = stream.str();	// store file content in string
    rfile.close();
	// ----
	    std::ofstream file(output.c_str()); // open file for writing
        if(!file.is_open())
        {
	        return false;
        }	
        for(int i = 0; i < content.size(); i++)
		{
			file << dokun::char_to_binary(content.at(i)) << "\n";
		}
	file.close();	
	return true;
}
////////////////////
bool neroshop::DB::binary_to_text(const std::string& input, const std::string& output)
{
	std::ifstream rfile(input.c_str());
	if(!rfile.is_open())
	{
	    return false;
	}	
	std::string line;
	std::vector<std::string> binary;
    while(std::getline(rfile, line))
	{
		binary.push_back(line);
	}
    rfile.close();
	// ----
	    std::ofstream file(output.c_str()); // open file for writing
        if(!file.is_open())
        {
	        return false;
        }	
        for(int i = 0; i < binary.size(); i++)
		{
			file << dokun::binary_to_char(binary[i]);
			//std::cout << dokun::binary_to_char(binary[i]);
		}
	file.close();	
	return true;
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
