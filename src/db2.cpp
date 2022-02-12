#include "../include/db2.hpp"

neroshop::DB2::DB2() : conn(nullptr)
{}
////////////////////
neroshop::DB2::DB2(const std::string& file_url) : DB2() // delegating constructor (will call default constructor)
{
    if(!connect(file_url)) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string("Connection to database failed: ") + std::string(PQerrorMessage(conn)));//fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
    }
}
////////////////////
neroshop::DB2::~DB2() {
    finish();
}
////////////////////
neroshop::DB2 * neroshop::DB2::db_obj (nullptr);
////////////////////
bool neroshop::DB2::connect(const std::string& conninfo) {
    // If you ever get this warning then it means
    // database connections are being called within each other like this:
    // db->connect("...");
    // db->connect("...");  // <- this is where the warning appears
    // db->close();         // <- we close the second database here
    // db.close();          // <- then we call close on the same database again, when it has already been closed :|, but thankfully, DB2::close() exits (returns) when the database is nullptr and does not double close a connection
    if(conn) {neroshop::print(POSTGRESQL_TAG + std::string("\033[1;33mWARNING: connection already exists! Please, close pre-existing connection before reconnecting\033[0m")); return true;}// temporary (to make sure database is not always opened during the entire session)
    conn = PQconnectdb(conninfo.c_str());   
    if (PQstatus(conn) == CONNECTION_BAD) { 
        finish();
        //exit(1);
        return false;
    }
    return true;
}
////////////////////
void neroshop::DB2::finish() {
    if(!conn) return; // there's no need to close twice so its better to just exit function
    PQfinish(conn);
    conn = nullptr; // set to null after deletion (to confirm that it has been properly deleted :])
    //neroshop::print("connection to database server is now closed");
}
////////////////////
void neroshop::DB2::execute(const std::string& command, ExecStatusType status_type) {
    if(!conn) throw std::runtime_error("database is not connected");
    PGresult * result = PQexec(conn, command.c_str());
    // PGRES_COMMAND_OK = Successful completion of a command returning no data.
    // PGRES_TUPLES_OK  = Successful completion of a command returning data (such as a SELECT or SHOW).
    if(PQresultStatus(result) != status_type/*PGRES_COMMAND_OK*/) { 
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(/*command + " failed: " + */PQerrorMessage(conn)), 1); // NEROSHOP_TAG_OUT std::cout << POSTGRESQL_TAG_ERROR + String::to_string(PQerrorMessage(conn)) << "\033[0m" << std::endl;
        PQclear(result);
        //finish();
        //exit(1);
        return; // to prevent double freeing of "result"
    }
#ifdef NEROSHOP_DEBUG
    if(status_type == PGRES_TUPLES_OK) neroshop::print(POSTGRESQL_TAG + std::string(PQgetvalue(result, 0, 0)));
#endif    
    PQclear(result); // Frees the storage associated with a PGresult. Every command result should be freed via PQclear when it is no longer needed, to avoid memory leaks.
}
////////////////////
void neroshop::DB2::execute_params(const std::string& command, const std::vector<std::string>& args, ExecStatusType status_type) {
    if(!conn) throw std::runtime_error("database is not connected");
    std::vector<const char*> params = {};
    for (int i = 0; i < args.size(); i++) params.push_back(args[i].c_str());//std::cout << "args: " << values[i] << std::endl; // '' <= quotes should be removed
    ///////////////////////    
    PGresult * result = PQexecParams(conn, command.c_str(),
        params.size(), nullptr, params.data(), nullptr, nullptr, 0);
    if(PQresultStatus(result) != status_type) { 
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQerrorMessage(conn)), 1); // NEROSHOP_TAG_OUT std::cout << POSTGRESQL_TAG_ERROR + String::to_string(PQerrorMessage(conn)) << "\033[0m" << std::endl;
        PQclear(result);
        //finish();
        //exit(1);
        return;
    }
#ifdef NEROSHOP_DEBUG
    //neroshop::print(POSTGRESQL_TAG + command);
    if(status_type == PGRES_TUPLES_OK) neroshop::print(POSTGRESQL_TAG + std::string(PQgetvalue(result, 0, 0)));
#endif        
    PQclear(result);    
} // db2.execute_params("INSERT INTO users (name, pw_hash, opt_email) VALUES ($1, $2, $3)", {"son", "no pw", "null"});
////////////////////
// an execute function that returns the result
////////////////////
////////////////////
void neroshop::DB2::print_database_info(void) {
    if(!conn) throw std::runtime_error("database is not connected");
    char * db_name = PQdb(conn);
    char * user_name = PQuser(conn); 
    char * pass = PQpass(conn);
    char * host_name = PQhost(conn);
    char * host_addr = PQhostaddr(conn);
    char * port_number = PQport(conn);
    std::cout << "db_name: " << db_name << std::endl;
    std::cout << "user: " << user_name << std::endl;
    std::cout << "pass: " << pass << std::endl;
    std::cout << "host: " << host_name << std::endl;
    std::cout << "host_addr: " << host_addr << std::endl;
    std::cout << "port: " << port_number << std::endl;
}
////////////////////
void neroshop::DB2::create_table(const std::string& table_name) {
    std::string command = "CREATE TABLE IF NOT EXISTS table_name(id  SERIAL PRIMARY KEY);"; // SMALLSERIAL, SERIAL, and BIGSERIAL are the same as AUTO_INCREMENT // bigserial should be used if you anticipate the use of more than 2^31 identifiers over the lifetime of the table // serial (4 bytes) is an autoincrementing integer, ranging from 1 to 2147483647 (2.15 billion)
    command = String::swap_first_of(command, "table_name", table_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
}
////////////////////
void neroshop::DB2::rename_table(const std::string& table_name, const std::string& new_name) {
    std::string command = "ALTER TABLE table_name RENAME TO new_name;";
    // rename table
	command = String::swap_first_of(command, "table_name", table_name);
	command = String::swap_first_of(command, "new_name"  , new_name  );
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    execute(command);	    
}
////////////////////
void neroshop::DB2::alter_table(const std::string& table_name, const std::string& action, const std::string& action_arg, std::string extra_args) {
    std::string command = "ALTER TABLE table_name action action_arg extra_args;";
	// add a column to table 
	command = String::swap_first_of(command, "table_name", table_name );
	command = String::swap_first_of(command, "action", action         ); // ADD, DROP, MODIFY, RENAME (columns), RENAME TO (tables)
	command = String::swap_first_of(command, "action_arg", action_arg );    
    // set extra args whether empty string or not (empty by default)
    command = String::swap_first_of(command, "extra_args",   extra_args); // extra_args can be the column datatype (ex. text, integer)
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
} // alter_table("users", "ADD", "name", "text");
////////////////////
void neroshop::DB2::add_column(const std::string& table_name, const std::string& column_name, std::string column_type) {
    std::string command = "ALTER TABLE table_name ADD COLUMN column_name column_type;";//"ALTER TABLE table_name action COLUMN column_name column_type;";
	// add a column to table 
	command = String::swap_first_of(command, "table_name", table_name  );
	command = String::swap_first_of(command, "column_name", column_name);    
    command = String::swap_first_of(command, "column_type", column_type);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
} // add_column("users", "name", "text");
////////////////////
void neroshop::DB2::insert_into(const std::string& table_name, const std::string& column_names, const std::string& values) {
    std::string command = "INSERT INTO table_name (column_names) "
	                      "VALUES (values);";
    // set column value
  	command = String::swap_first_of(command, "table_name", table_name    );
	command = String::swap_first_of(command, "column_names", column_names);
	command = String::swap_first_of(command, "values", values            );    
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
}
////////////////////
void neroshop::DB2::update(const std::string& table_name, const std::string& column_name, const std::string& value, std::string condition) {
    std::string command = "UPDATE table_name SET column_name = value " 
	                      "WHERE condition;";
	                      //"RETURNING *;";
    /////////////////
	// update column value
	command = String::swap_first_of(command, "table_name", table_name  );
	command = String::swap_first_of(command, "column_name", column_name);
	command = String::swap_first_of(command, "value", value            );
	// if condition is empty then remove "WHERE condition", else replace "condition"
	command = (condition.empty()) ? String::remove_first_of(command, "WHERE condition") : String::swap_first_of(command, "condition",  condition);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    /////////////////
    execute(command);
}
////////////////////
void neroshop::DB2::update_replace(const std::string& table_name, const std::string& column_name, const std::string& old_str, const std::string& new_str, std::string condition) {
    std::string command = "UPDATE table_name "
    "SET column_name = REPLACE (column_name, 'old_str', 'new_str') "
    "WHERE condition;";
    command = String::swap_first_of(command, "table_name", table_name);
	command = String::swap_all(command, "column_name", column_name   );
	command = String::swap_first_of(command, "old_str", old_str      );
	command = String::swap_first_of(command, "new_str", new_str      );
	// if condition is empty then remove "WHERE condition", else replace "condition"
	command = (condition.empty()) ? String::remove_first_of(command, "WHERE condition") : String::swap_first_of(command, "condition",  condition);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    execute(command);	
}
////////////////////
////////////////////
void neroshop::DB2::create_index(const std::string& index_name, const std::string& table_name, const std::string& indexed_columns) {
    std::string command = "CREATE UNIQUE INDEX index_name ON table_name (indexed_columns);";//USING index_type (indexed_columns)//PostgreSQL has several index types: B-tree, Hash, GiST, SP-GiST, GIN, and BRIN //PostgreSQL uses B-tree index type by default because it is best fit the most common queries
    ////////////////
    command = String::swap_first_of(command, "index_name", index_name  );
    command = String::swap_first_of(command, "table_name", table_name  );    
    command = String::swap_first_of(command, "indexed_columns", indexed_columns);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    execute(command);
}
////////////////////
void neroshop::DB2::drop_index(const std::string& index_name) {
    std::string command = "DROP INDEX IF EXISTS index_name;";
    command = String::swap_first_of(command, "index_name", index_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    execute(command);
}
////////////////////
////////////////////
void neroshop::DB2::truncate(const std::string& table_name) {
    std::string command = "DELETE FROM table_name;";
    // set table_name
    command = String::swap_first_of(command, "table_name", table_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);    
}
////////////////////
void neroshop::DB2::delete_from(const std::string& table_name, const std::string& condition) {
    std::string command = "DELETE FROM table_name WHERE condition;";
    // set table_name and condition
    command = String::swap_first_of(command, "table_name", table_name);
    command = String::swap_first_of(command, "condition", condition  );
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
}
////////////////////
void neroshop::DB2::drop_column(const std::string& table_name, const std::string& column_name) {
    std::string command = "ALTER TABLE table_name DROP COLUMN IF EXISTS column_name;";
    // set table_name and column_name
    command = String::swap_first_of(command, "table_name", table_name);
    command = String::swap_first_of(command, "column_name", column_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);        
}
////////////////////
// use CASCADE to drop the dependent objects (tables) too, if other objects (tables) depend on the table (ex. "DROP TABLE item CASCADE;" <- will also drop table inventory, which depends on table item)
void neroshop::DB2::drop_table(const std::string& table_name) {
    std::string command = "DROP TABLE IF EXISTS table_name;";
    // set table_name
    command = String::swap_first_of(command, "table_name", table_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
}
////////////////////
void neroshop::DB2::drop_database(const std::string& database_name) {
    std::string command = "DROP DATABASE IF EXISTS database_name;";
    // set database_name
    command = String::swap_first_of(command, "database_name", database_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);    
}
////////////////////
// vacuum should be called periodically or when a certain number of records have been updated/deleted
void neroshop::DB2::vacuum(std::string opt_arg) {
    std::string command = "VACUUM opt_arg;";
    command = String::swap_first_of(command, "opt_arg", opt_arg); // VACUUM FULL returns unused space to the operating system, reducing the db size while plain VACUUM reserves the space for the table and does not return it to the operating system, leaving the db size to remain the same
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
} //db2.vacuum(), db2.vacuum("FULL")// frees up space in all tables in db // db2.vacuum("users"), db2.vacuum("FULL users"), db2.vacuum("FULL VERBOSE users") // frees a specific table, instead of the entire database // VERBOSE displays an activity report of the vacuum process
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
std::string neroshop::DB2::to_psql_string(const std::string& str) {
    //return "'" + str + "'";//"\'" + str + "\'";//"&apos" + str + "&apos";//"$" + str + "$"; // double quotes are not allowed to be used on strings in postgresql unfortunately :( // double quotes are reserved for identifiers (ex. CREATE TABLE "mytable")
    std::string temp_str = String::swap_all(str, "'", "\""); // replace all apostrophes to string quotes //str;//"REGEXP_REPLACE ('" + str + "', '" + apostrophe + "', '" + new_str + "');";
    return "'" + temp_str + "'";
}
////////////////////
// getters
////////////////////
int neroshop::DB2::get_lib_version() {
    return PQlibVersion(); // 140001 = 14.1
}
////////////////////
int neroshop::DB2::get_server_version() const {
    if(!conn) throw std::runtime_error("database is not connected");
    return PQserverVersion(conn);
}
////////////////////
PGconn * neroshop::DB2::get_handle() const {
    return conn;
}
////////////////////
int neroshop::DB2::get_integer(const std::string& select_stmt) const {
    if(!conn) throw std::runtime_error("database is not connected");
    PGresult * result = PQexec(conn, select_stmt.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string("No data retrieved"));        
        PQclear(result);
        //finish();
        return 0; // default  
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    int number_int = std::stoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_int;    
}
////////////////////
int neroshop::DB2::get_integer_params(const std::string& select_stmt, const std::vector<std::string>& values) const { // not tested yet
    if(!conn) throw std::runtime_error("database is not connected");
    std::vector<const char*> params = {};
    for (int i = 0; i < values.size(); i++) params.push_back(values[i].c_str());//std::cout << "args: " << values[i] << std::endl; // '' <= quotes should be removed
    ///////////////////////
    PGresult * result = PQexecParams(conn,
                       select_stmt.c_str(),
                       params.size(),//int nParams,
                       nullptr,//const Oid *paramTypes,
                       params.data(),//args//values.data(),//const char * const *paramValues,
                       nullptr,//const int *paramLengths,
                       nullptr,//const int *paramFormats,
                       0);//int resultFormat);*/ // 1=binary result  0=text result
    // This is to prevent the error (row number 0 is out of range 0..-1) and a crash if result is null :}
    if(PQntuples(result) < 1) {PQclear(result); return 0;} // Returns the number of rows (tuples) in the query result
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQresultErrorMessage(result))); // No data retrieved
        PQclear(result);
        //finish();
        return 0;    
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    int number_int = std::stoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_int;        
}
////////////////////
float neroshop::DB2::get_real(const std::string& select_stmt) const {
    if(!conn) throw std::runtime_error("database is not connected");
    PGresult * result = PQexec(conn, select_stmt.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string("No data retrieved"));        
        PQclear(result);
        //finish();
        return 0.0f;  
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    float number_float = std::stof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_float;        
}
////////////////////
float neroshop::DB2::get_real_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
    if(!conn) throw std::runtime_error("database is not connected");
    std::vector<const char*> params = {};
    for (int i = 0; i < values.size(); i++) params.push_back(values[i].c_str());//std::cout << "args: " << values[i] << std::endl; // '' <= quotes should be removed
    ///////////////////////
    PGresult * result = PQexecParams(conn,
                       select_stmt.c_str(),
                       params.size(),//int nParams,
                       nullptr,//const Oid *paramTypes,
                       params.data(),//args//values.data(),//const char * const *paramValues,
                       nullptr,//const int *paramLengths,
                       nullptr,//const int *paramFormats,
                       0);//int resultFormat);*/ // 1=binary result  0=text result
    // This is to prevent the error (row number 0 is out of range 0..-1) and a crash if result is null :}
    if(PQntuples(result) < 1) {PQclear(result); return 0.0f;} // Returns the number of rows (tuples) in the query result
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQresultErrorMessage(result))); // No data retrieved
        PQclear(result);
        //finish();
        return 0.0f;    
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    float number_float = std::stof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_float;
}
////////////////////
float neroshop::DB2::get_float(const std::string& select_stmt) const {
    return get_real(select_stmt);
}
////////////////////
float neroshop::DB2::get_float_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
    return get_real_params(select_stmt, values);
}
////////////////////
double neroshop::DB2::get_double(const std::string& select_stmt) const {
    if(!conn) throw std::runtime_error("database is not connected");
    PGresult * result = PQexec(conn, select_stmt.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string("No data retrieved"));        
        PQclear(result);
        //finish();
        return 0.0;  
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    double number_double = std::stod(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_double;
}
////////////////////
double neroshop::DB2::get_double_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
    if(!conn) throw std::runtime_error("database is not connected");
    std::vector<const char*> params = {};
    for (int i = 0; i < values.size(); i++) params.push_back(values[i].c_str());//std::cout << "args: " << values[i] << std::endl; // '' <= quotes should be removed
    ///////////////////////
    PGresult * result = PQexecParams(conn,
                       select_stmt.c_str(),
                       params.size(),//int nParams,
                       nullptr,//const Oid *paramTypes,
                       params.data(),//args//values.data(),//const char * const *paramValues,
                       nullptr,//const int *paramLengths,
                       nullptr,//const int *paramFormats,
                       0);//int resultFormat);*/ // 1=binary result  0=text result
    // This is to prevent the error (row number 0 is out of range 0..-1) and a crash if result is null :}
    if(PQntuples(result) < 1) {PQclear(result); return 0.0;} // Returns the number of rows (tuples) in the query result
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQresultErrorMessage(result))); // No data retrieved //neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQerrorMessage(conn)));
        PQclear(result);
        //finish();
        return 0.0;    
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    double number_double = std::stod(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_double;    
}
////////////////////
////////////////////
std::string neroshop::DB2::get_text(const std::string& select_stmt) const {
    if(!conn) throw std::runtime_error("database is not connected");
    PGresult * result = PQexec(conn, select_stmt.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQresultErrorMessage(result))); // No data retrieved            
        PQclear(result);
        //finish();
        return "";    
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    std::string text = std::string(PQgetvalue(result, 0, 0));
    // convert all double quotes (") back to single quotes/apostrophes (')
    if(String::contains(text, "\"")) text = String::swap_all(text, "\"", "'");
    PQclear(result);
    return text;
}
////////////////////
std::string neroshop::DB2::get_text_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
    if(!conn) throw std::runtime_error("database is not connected");
    std::vector<const char*> params = {};
    for (int i = 0; i < values.size(); i++) params.push_back(values[i].c_str());//std::cout << "args: " << values[i] << std::endl; // '' <= quotes should be removed
    ///////////////////////
    PGresult * result = PQexecParams(conn,
                       select_stmt.c_str(),
                       params.size(),//int nParams,
                       nullptr,//const Oid *paramTypes,
                       params.data(),//args//values.data(),//const char * const *paramValues,
                       nullptr,//const int *paramLengths,
                       nullptr,//const int *paramFormats,
                       0);//int resultFormat);*/ // 1=binary result  0=text result
    // This is to prevent the error (row number 0 is out of range 0..-1) and a crash if result is null :}
    if(PQntuples(result) < 1) {PQclear(result); return "";} // Returns the number of rows (tuples) in the query result
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQresultErrorMessage(result))); // No data retrieved //neroshop::print(POSTGRESQL_TAG_ERROR + std::string(PQerrorMessage(conn)));
        PQclear(result);
        //finish();
        return "";    
    }
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + select_stmt);
#endif        
    std::string text = std::string(PQgetvalue(result, 0, 0));
    // convert all double quotes (") back to single quotes/apostrophes (')
    if(String::contains(text, "\"")) text = String::swap_all(text, "\"", "'");
    PQclear(result);
    return text;        
}
////////////////////
////////////////////
unsigned int neroshop::DB2::get_row_count(const std::string& table_name) const {
    std::string command = "SELECT COUNT(*) FROM table_name"; // number of rows in a table
    // replace table_name
    command = String::swap_first_of(command, "table_name", table_name);         
    return get_integer(command);
}
////////////////////
int neroshop::DB2::get_last_id(const std::string& table_name) const {
    // if table has no rows (empty table) then exit function
    //if(get_row_count(table_name) < 1) return 0; // no need for this, its better if the error is displayed so we know what the problem is
    std::string command = "SELECT * FROM table_name ORDER BY id DESC LIMIT 1;";
    // replace table_name
    command = String::swap_first_of(command, "table_name", table_name);
    return get_integer(command);//std::stoi(get_text(command));
}
////////////////////
////////////////////
int neroshop::DB2::get_connections_count() const { // works! :D
    std::string command = "SELECT sum(numbackends) FROM pg_stat_database;";
    return get_integer(command);
}
////////////////////
////////////////////
std::string neroshop::DB2::localtimestamp_to_utc(const std::string& localtimestamp) const {
    return get_text_params("SELECT $1::timestamptz AT TIME ZONE 'UTC'", { localtimestamp });
}
// localtimestamp_to_utc("2022-02-11 07:00:00.339934-05")
////////////////////
std::string neroshop::DB2::localtime_to_utc(const std::string& localtime) const {
    return get_text_params("SELECT $1::timetz AT TIME ZONE 'UTC'", { localtime });
}
////////////////////
neroshop::DB2 * neroshop::DB2::get_singleton() {
    if(!db_obj) db_obj = new DB2(); // conn is null by default
    return db_obj;
}
////////////////////
////////////////////
////////////////////
// boolean
////////////////////
bool neroshop::DB2::table_exists(const std::string& table_name) const {
    // pg_tables view contains information about each table in the database. // pg_tables ia a lot more faster than information_schema.tables and allows you to access tables without being a tableowner // to get current scheme: "SELECT current_schema();"
    std::string command = "SELECT EXISTS ("
                          "SELECT FROM pg_tables " 
                          "WHERE schemaname = 'public' AND tablename = 'table_name'"
                          ");";
    // replace table_name
    command = String::swap_first_of(command, "table_name", table_name);
    bool exists = (get_text(command) == "t") ? true : false; // f=false
    return exists;
}
////////////////////
bool neroshop::DB2::column_exists(const std::string& table_name, const std::string& column_name) const {
    std::string command = "SELECT COUNT(column_name) FROM table_name;"; // this actually counts the number of rows in a column (if column is empty then it will return 0)
    // replace table_name and column_name
    command = String::swap_first_of(command, "table_name", table_name);
    command = String::swap_first_of(command, "column_name", column_name);
    bool exists = (get_integer(command) > 0) ? true : false;
    return exists;
}
////////////////////
////////////////////
/*
    std::string command = "";
    /////////////////
    
#ifdef NEROSHOP_DEBUG
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    /////////////////
    execute(command);
*/
////////////////////
