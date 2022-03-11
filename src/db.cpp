#include "../include/db.hpp"

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// PostgreSQL
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
neroshop::DB::Psql::Psql() : conn(nullptr)
{}
////////////////////
neroshop::DB::Psql::Psql(const std::string& file_url) : Psql() // delegating constructor (will call default constructor)
{
    if(!connect(file_url)) {
        neroshop::print(POSTGRESQL_TAG_ERROR + std::string("Connection to database failed: ") + std::string(PQerrorMessage(conn)));//fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
    }
}
////////////////////
neroshop::DB::Psql::~Psql() {
    finish();
}
////////////////////
neroshop::DB::Psql * neroshop::DB::Psql::db_obj (nullptr);
////////////////////
bool neroshop::DB::Psql::connect(const std::string& conninfo) {
    // If you ever get this warning then it means
    // database connections are being called within each other like this:
    // db->connect("...");
    // db->connect("...");  // <- this is where the warning appears
    // db->close();         // <- we close the second database here
    // db.close();          // <- then we call close on the same database again, when it has already been closed :|, but thankfully, DB::close() exits (returns) when the database is nullptr and does not double close a connection
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
void neroshop::DB::Psql::finish() {
    if(!conn) return; // there's no need to close twice so its better to just exit function
    PQfinish(conn);
    conn = nullptr; // set to null after deletion (to confirm that it has been properly deleted :])
    neroshop::print("connection to database server is now closed");
}
////////////////////
void neroshop::DB::Psql::execute(const std::string& command, ExecStatusType status_type) {
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
void neroshop::DB::Psql::execute_params(const std::string& command, const std::vector<std::string>& args, ExecStatusType status_type) {
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
void neroshop::DB::Psql::print_database_info(void) {
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
void neroshop::DB::Psql::create_table(const std::string& table_name) {
    std::string command = "CREATE TABLE IF NOT EXISTS table_name(id  SERIAL PRIMARY KEY);"; // SMALLSERIAL, SERIAL, and BIGSERIAL are the same as AUTO_INCREMENT // bigserial should be used if you anticipate the use of more than 2^31 identifiers over the lifetime of the table // serial (4 bytes) is an autoincrementing integer, ranging from 1 to 2147483647 (2.15 billion)
    command = String::swap_first_of(command, "table_name", table_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
}
////////////////////
void neroshop::DB::Psql::rename_table(const std::string& table_name, const std::string& new_name) {
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
void neroshop::DB::Psql::alter_table(const std::string& table_name, const std::string& action, const std::string& action_arg, std::string extra_args) {
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
void neroshop::DB::Psql::add_column(const std::string& table_name, const std::string& column_name, std::string column_type) {
    std::string command = "ALTER TABLE table_name ADD COLUMN IF NOT EXISTS column_name column_type;";//"ALTER TABLE table_name action COLUMN column_name column_type;";
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
void neroshop::DB::Psql::insert_into(const std::string& table_name, const std::string& column_names, const std::string& values) {
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
void neroshop::DB::Psql::update(const std::string& table_name, const std::string& column_name, const std::string& value, std::string condition) {
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
void neroshop::DB::Psql::update_replace(const std::string& table_name, const std::string& column_name, const std::string& old_str, const std::string& new_str, std::string condition) {
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
void neroshop::DB::Psql::create_index(const std::string& index_name, const std::string& table_name, const std::string& indexed_columns) {
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
void neroshop::DB::Psql::drop_index(const std::string& index_name) {
    std::string command = "DROP INDEX IF EXISTS index_name;";
    command = String::swap_first_of(command, "index_name", index_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif        
    execute(command);
}
////////////////////
////////////////////
void neroshop::DB::Psql::truncate(const std::string& table_name) {
    std::string command = "DELETE FROM table_name;";
    // set table_name
    command = String::swap_first_of(command, "table_name", table_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);    
}
////////////////////
void neroshop::DB::Psql::delete_from(const std::string& table_name, const std::string& condition) {
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
void neroshop::DB::Psql::drop_column(const std::string& table_name, const std::string& column_name) {
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
void neroshop::DB::Psql::drop_table(const std::string& table_name) {
    std::string command = "DROP TABLE IF EXISTS table_name;";
    // set table_name
    command = String::swap_first_of(command, "table_name", table_name);
#ifdef NEROSHOP_DEBUG0
    neroshop::print(POSTGRESQL_TAG + command);
#endif    
    execute(command);
}
////////////////////
void neroshop::DB::Psql::drop_database(const std::string& database_name) {
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
void neroshop::DB::Psql::vacuum(std::string opt_arg) {
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
std::string neroshop::DB::Psql::to_psql_string(const std::string& str) {
    //return "'" + str + "'";//"\'" + str + "\'";//"&apos" + str + "&apos";//"$" + str + "$"; // double quotes are not allowed to be used on strings in postgresql unfortunately :( // double quotes are reserved for identifiers (ex. CREATE TABLE "mytable")
    std::string temp_str = String::swap_all(str, "'", "\""); // replace all apostrophes to string quotes //str;//"REGEXP_REPLACE ('" + str + "', '" + apostrophe + "', '" + new_str + "');";
    return "'" + temp_str + "'";
}
////////////////////
// getters
////////////////////
int neroshop::DB::Psql::get_lib_version() {
    return PQlibVersion(); // 140001 = 14.1
}
////////////////////
int neroshop::DB::Psql::get_server_version() const {
    if(!conn) throw std::runtime_error("database is not connected");
    return PQserverVersion(conn);
}
////////////////////
PGconn * neroshop::DB::Psql::get_handle() const {
    return conn;
}
////////////////////
int neroshop::DB::Psql::get_integer(const std::string& select_stmt) const {
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
int neroshop::DB::Psql::get_integer_params(const std::string& select_stmt, const std::vector<std::string>& values) const { // not tested yet
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
    std::string number_str = PQgetvalue(result, 0, 0);
    if(number_str.empty()) {PQclear(result); return 0;}
    int number_int = std::stoi(number_str);//std::stoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_int;        
}
////////////////////
float neroshop::DB::Psql::get_real(const std::string& select_stmt) const {
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
float neroshop::DB::Psql::get_real_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
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
    std::string number_str = PQgetvalue(result, 0, 0);
    if(number_str.empty()) {PQclear(result); return 0.0f;}
    float number_float = std::stof(number_str);//std::stof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_float;
}
////////////////////
float neroshop::DB::Psql::get_float(const std::string& select_stmt) const {
    return get_real(select_stmt);
}
////////////////////
float neroshop::DB::Psql::get_float_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
    return get_real_params(select_stmt, values);
}
////////////////////
double neroshop::DB::Psql::get_double(const std::string& select_stmt) const {
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
double neroshop::DB::Psql::get_double_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
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
    std::string number_str = PQgetvalue(result, 0, 0);
    if(number_str.empty()) {PQclear(result); return 0.0;}
    double number_double = std::stod(number_str);//std::stod(PQgetvalue(result, 0, 0));
    PQclear(result);
    return number_double;    
}
////////////////////
////////////////////
std::string neroshop::DB::Psql::get_text(const std::string& select_stmt) const {
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
std::string neroshop::DB::Psql::get_text_params(const std::string& select_stmt, const std::vector<std::string>& values) const {
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
unsigned int neroshop::DB::Psql::get_row_count(const std::string& table_name) const {
    std::string command = "SELECT COUNT(*) FROM table_name"; // number of rows in a table
    // replace table_name
    command = String::swap_first_of(command, "table_name", table_name);         
    return get_integer(command);
}
////////////////////
int neroshop::DB::Psql::get_last_id(const std::string& table_name) const {
    // if table has no rows (empty table) then exit function
    //if(get_row_count(table_name) < 1) return 0; // no need for this, its better if the error is displayed so we know what the problem is
    std::string command = "SELECT * FROM table_name ORDER BY id DESC LIMIT 1;";
    // replace table_name
    command = String::swap_first_of(command, "table_name", table_name);
    return get_integer(command);//std::stoi(get_text(command));
}
////////////////////
////////////////////
int neroshop::DB::Psql::get_connections_count() const { // works! :D
    std::string command = "SELECT sum(numbackends) FROM pg_stat_database;";
    return get_integer(command);
}
////////////////////
////////////////////
std::string neroshop::DB::Psql::localtimestamp_to_utc(const std::string& localtimestamp) const {
    return get_text_params("SELECT $1::timestamptz AT TIME ZONE 'UTC'", { localtimestamp });
}
// localtimestamp_to_utc("2022-02-11 07:00:00.339934-05")
////////////////////
std::string neroshop::DB::Psql::localtime_to_utc(const std::string& localtime) const {
    return get_text_params("SELECT $1::timetz AT TIME ZONE 'UTC'", { localtime });
}
////////////////////
neroshop::DB::Psql * neroshop::DB::Psql::get_singleton() {
    if(!db_obj) db_obj = new DB::Psql(); // conn is null by default
    return db_obj;
}
////////////////////
////////////////////
////////////////////
// boolean
////////////////////
bool neroshop::DB::Psql::table_exists(const std::string& table_name) const {
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
bool neroshop::DB::Psql::column_exists(const std::string& table_name, const std::string& column_name) const {
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
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// sqlite3
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
neroshop::DB::Sqlite3::Sqlite3()
{}
////////////////////
neroshop::DB::Sqlite3::Sqlite3(const std::string& file_name) : Sqlite3() // delegating constructor
{
	if(!open(file_name)) {
		NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR << "Could not open db - " + String::to_string(sqlite3_errmsg(db)) + " (" + file_name + ")" << std::endl;
        std::exit(1);
    }
}
////////////////////
neroshop::DB::Sqlite3::~Sqlite3() {
    close();
}
////////////////////
bool neroshop::DB::Sqlite3::open(const std::string& file_name) // create a data base; works!
{
	if(sqlite3_open(file_name.c_str(), &db)) {
		close();
		return false;
	}
	return true;
}
////////////////////
void neroshop::DB::Sqlite3::close() {
    if(!db) return;
	sqlite3_close(db);
	db = nullptr; // set to null after deletion (to confirm that it has been properly deleted)
}
////////////////////
////////////////////
void neroshop::DB::Sqlite3::execute(const std::string& sql) // execute an sql statement; works !
{
    if(!db) throw std::runtime_error("database is not connected");
	char * errmsg = 0;
	int error = sqlite3_exec(db, sql.c_str(), neroshop::DB::Sqlite3::callback, 0, &errmsg);
	if (error != SQLITE_OK)
	{
		NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG_ERR + String::to_string(errmsg) << "\033[0m" << std::endl;
		sqlite3_free(errmsg);
	}
	if(error == SQLITE_LOCKED) {NEROSHOP_TAG_OUT std::cout << "dammit, this database is locked wtf!!!" << std::endl;} // remove this LOL
}
////////////////////
void neroshop::DB::Sqlite3::table(const std::string& table, bool auto_inc) // creates a new table; works !
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
void neroshop::DB::Sqlite3::column(const std::string& table, const std::string& method, const std::string& column_name, std::string column_type) // creates, deletes, or modifies a column; works!
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
void neroshop::DB::Sqlite3::insert(const std::string& table, const std::string& variable, const std::string& value) // sets the value of a column (column must exist first); works!
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
void neroshop::DB::Sqlite3::select(const std::string& table, std::string variable, std::string condition) // selects variable from a table // works!
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
void neroshop::DB::Sqlite3::update(const std::string& table, const std::string& variable, const std::string& value, std::string condition) // updates a variable; works!
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
void neroshop::DB::Sqlite3::drop(const std::string& table, std::string condition) // works!
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
void neroshop::DB::Sqlite3::index(const std::string& index_name, const std::string& table_name, const std::string& column_list) // ensures that columns will not be duplicated // works!
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
void neroshop::DB::Sqlite3::drop_index(const std::string& index_name) // works!
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
void neroshop::DB::Sqlite3::rename_table(const std::string& table, const std::string& new_name) // works!
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
void neroshop::DB::Sqlite3::delete_from(const std::string& table, std::string condition) { // not tested yet
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
void neroshop::DB::Sqlite3::truncate(const std::string& table_name) { // primary_key id will stay unique so if you make a table with 9 rows then truncate it, when you insert into the table again, the id will start from 10 // https://www.tutorialspoint.com/sqlite/sqlite_truncate_table.htm
    std::string sql = "DELETE FROM table_name;";
    sql = String::swap_first_of(sql, "table_name", table_name   );
    //sql.append(" VACUUM;");// to clear unused space
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
    execute(sql.c_str());
}
////////////////////
void neroshop::DB::Sqlite3::vacuum() { // reduces db size by clearing empty spaces
    std::string sql = "VACUUM;";
#ifdef NEROSHOP_DEBUG0
    neroshop::print(SQLITE3_TAG + sql);
#endif
    execute(sql.c_str());    
}
////////////////////
////////////////////
std::string neroshop::DB::Sqlite3::to_sql_string(const std::string& value) {
    return "\"" + value + "\"";
}
////////////////////
////////////////////
////////////////////
////////////////////
sqlite3 * neroshop::DB::Sqlite3::get_handle() const {
    return db;
}
////////////////////
//sqlite3_stmt * neroshop::DB::Sqlite3::get_stmt() const {
//    return nullptr;//stmt;
//}
////////////////////
////////////////////
std::string neroshop::DB::Sqlite3::get_sqlite_version() {
    return sqlite3_libversion();
} // NEROSHOP_TAG_OUT std::cout << SQLITE3_TAG << "sqlite3 v" << neroshop::DB::Sqlite3::get_sqlite_version() << std::endl;
////////////////////
////////////////////
std::string neroshop::DB::Sqlite3::get_select(const std::string& table, std::string variable, std::string condition) const 
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
std::string neroshop::DB::Sqlite3::get_column_name(const std::string& table) const 
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
void * neroshop::DB::Sqlite3::get_column_null(const std::string& table, std::string variable, std::string condition) const {
    return nullptr;
}
////////////////////
int neroshop::DB::Sqlite3::get_column_integer(const std::string& table, std::string variable, std::string condition) const {
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
double neroshop::DB::Sqlite3::get_column_real(const std::string& table, std::string variable, std::string condition) const {
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
std::string neroshop::DB::Sqlite3::get_column_text(const std::string& table, std::string variable, std::string condition) const // works!
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
void * neroshop::DB::Sqlite3::get_column_blob(const std::string& table, std::string variable, std::string condition) const 
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
unsigned int neroshop::DB::Sqlite3::row_count(const std::string& table_name) const {
    return get_column_integer(table_name, "COUNT(*)");
}
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::DB::Sqlite3::table_exists(const std::string& table_name) const {
    std::string sql_cond = "type='table' AND name='table_name';";//SELECT count(*) FROM sqlite_master WHERE type='table' AND name='table_name';
    sql_cond = String::swap_first_of(sql_cond, "table_name", table_name);
    bool exists = get_column_integer("sqlite_master", "count(*)", sql_cond);
    return exists;
}
////////////////////
// will print [sqlite3]: no such column: role_id" if column does not exist
// find a proper way to check if column exists
bool neroshop::DB::Sqlite3::column_exists(const std::string& table_name, const std::string& column_name) const {
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
int neroshop::DB::Sqlite3::callback(void *not_used, int argc, char **argv, char **azcolname)
{
    int i;
    for(i = 0; i < argc; i++) {
	    std::cout << SQLITE3_TAG << azcolname[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;  // printf("%s = %s\n", azcolname[i], argv[i] ? argv[i] : "nullptr");
    }
    std::cout << std::endl;
    return 0;	
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
////////////////////
////////////////////
////////////////////
////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Text
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/*bool neroshop::DB::Text::text_to_binary(const std::string& input, const std::string& output) // (input)file_to_convert_to_binary, (output)binary_file
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
bool neroshop::DB::Text::binary_to_text(const std::string& input, const std::string& output)
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
}*/
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
