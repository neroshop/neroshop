// filename: db2.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability?

#ifndef DATABASE2_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define DATABASE2_HPP_NEROSHOP
#define POSTGRESQL_TAG "\033[1;94m[postgresql]:\033[0m "
#define POSTGRESQL_TAG_ERROR "\033[1;94m[postgresql]:\033[0;91m "
#include <iostream>
// neroshop
#include "debug.hpp"
// dokun-ui
#include <string.hpp>
// libpq (C interface to PostgreSQL)
#include <postgresql/libpq-fe.h> // Linux // https://www.postgresql.org/download/linux/ubuntu/ // sudo apt install postgresql // sudo apt-get install libpq-dev // Ubuntu headers: /usr/include/postgresql
// https://zetcode.com/db/postgresqlc/
namespace neroshop {
class DB2 { // DB2 is intended for client-server database engine. SQLite does not use a client-server architecture, so unfortunately we cannot solely use SQLite for this as its used for embedding into applications and becomes an integral part of the program
public:
    DB2();
    DB2(const std::string& file_url);
    ~DB2();
    bool connect(const std::string& conninfo); // string can be empty to use all default parameters
    void finish(); // Closes the connection to the server. Also frees memory used by the PGconn object.
    void execute(const std::string& command); // returns no data
    void execute_return(const std::string& command); // returns data (such as a SELECT or SHOW)
    void print_database_info(void);
    void create_table(const std::string& table_name); // https://www.postgresql.org/docs/14/sql-createtable.html
    void rename_table(const std::string& table_name, const std::string& new_name);    
    void alter_table(const std::string& table_name, const std::string& action, const std::string& action_arg, std::string extra_arg = ""); // adds a new column to a table (think of column as a variable) // https://www.postgresql.org/docs/current/sql-altertable.html
    void add_column(const std::string& table_name, const std::string& column_names, std::string column_type = "text DEFAULT NULL");
    void insert_into(const std::string& table_name, const std::string& column_names, const std::string& values); // inserts a new data entry into a table (like assigning a value to a variable)
    void update(const std::string& table_name, const std::string& column_name, const std::string& value, std::string condition = "");
    void update_replace(const std::string& table_name, const std::string& column_name, const std::string& old_text, const std::string& new_text, std::string condition = ""); // replaces all occurences of a character or string//void update_replace_regex(); // replaces regex    
    void create_index(const std::string& index_name, const std::string& table_name, const std::string& indexed_columns); // indexes enhance database performance. An index allows the database server to find and retrieve specific rows much faster than it could do without an index.
    void drop_index(const std::string& index_name);
    void truncate(const std::string& table_name); // deletes all rows from a table 
    void delete_from(const std::string& table_name, const std::string& condition);    
    void drop_column(const std::string& table_name, const std::string& column_name);
    void drop_table(const std::string& table_name);
    void drop_database(const std::string& database_name);
    void vacuum(std::string opt_arg = ""); // garbage collector //VACUUM frees up space within each table but does not return the space to the operating system, therefore, the size of the database file would not be reduced// VACUUM FULL; reduces database size by reclaiming storage occupied by dead tuples
    // conversion
    static std::string to_psql_string(const std::string& str);    
    // setters
    // getters
    static int get_lib_version();
    int get_server_version() const;
    PGconn * get_handle() const;
    int get_integer(const std::string& select_stmt) const; // bool get_boolean(); // atoi("") // string to int
    float get_real(const std::string& select_stmt) const;
    float get_float(const std::string& select_stmt) const;
    double get_double(const std::string& select_stmt) const; // atof (const char* str);
    std::string get_text(const std::string& select_stmt) const;
    unsigned int get_row_count(const std::string& table_name) const; // returns the number of rows in a table
    int get_last_id(const std::string& table_name) const; // returns the id of the latest or most recent record pushed into a table
    // boolean
    bool table_exists(const std::string& table_name);
    //bool column_exists(const std::string& table_name, const std::string& column_name);// get_integer("SELECT COUNT(column_name) FROM table_name;");
private:
    PGconn * conn;
};
}
/*
    # postgresql datatypes:
    all - https://www.postgresql.org/docs/14/datatype.html
    numerals - https://www.postgresql.org/docs/14/datatype-numeric.html
    strings  - https://www.postgresql.org/docs/14/datatype-character.html
    datetime - https://www.postgresql.org/docs/14/datatype-datetime.html
    // text, character varying(n) ( varchar(n) ), money, integer, real (float4), double precision (float8), boolean (bool), date, time, json, xml, etc.

    # open "/etc/postgresql/14/main/pg_hba.conf"
    sudo gedit /etc/postgresql/14/main/pg_hba.conf
    # edit the pg_hba.conf file (change authentication methods from "peer" to "trust"):
    "local   all             postgres                                trust"
    # restart the server (for the changes in "pg_hba.conf" to take effect): 
    sudo service postgresql restart
    # Finally you can login without need of a password as shown in the figure:
    psql user=postgres
    # change the password of postgres or remove superuser (within psql shell)
    sudo -u postgres -c "DROP ROLE superuser;" # ALTER USER postgres PASSWORD 'postgres';
    # leave the psql shell
    \q
    # restore value from trust to md5 or peer then restart again
    
    # where postgresql stores database files:
    sudo -u postgres psql -c "show data_directory;"
    nautilus /var/lib/postgresql/14/main

    # to list all databases on your device (localhost):
    psql -l
    
    # to change limits and other settings: 
    gedit '/etc/postgresql/14/main/postgresql.conf'




$ sudo -u postgres createuser -s $USER
$ createdb mydatabase
$ psql -d mydatabase

OR

sudo -u postgres createuser -s $USER
createdb
psql

# And, then in the psql shell:
CREATE ROLE myuser LOGIN PASSWORD 'mypass';
CREATE DATABASE mydatabase WITH OWNER = myuser;

# login
psql -h localhost -d mydatabase -U myuser -p <port>
psql -h localhost -d neroshoptest -U postgres -p 5432

# if we don't know the port:
SHOW port;

    // sudo service postgresql start
    // sudo service postgresql status
    
    sudo -u postgres -c "DROP ROLE superuser;"
    
    
changing password (within psql shell):    
    ALTER USER postgres PASSWORD 'postgres';
changing password (normally):
    sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'postgres';"

Example usage(s):

    DB2 db2("host=127.0.0.1 port=5432 user=postgres password=postgres dbname=neroshoptest");
    db2.print_database_info();
    // checking if table exists
    std::cout << db2.table_exists("users") << " (table_exists)" << std::endl;
    // creating table with columns
    db2.create_table("users");
    db2.add_column("users", "name", "text");
    db2.alter_table("users", "ADD", "pw_hash", "text");
    db2.add_column("users", "opt_email"); // will be NULL since datatype is not specified
    // table rows
    std::cout << db2.get_row_count("users") << " (row count)" << std::endl;
    std::cout << db2.get_last_id("users") << " (lastest row)" << std::endl;
    // replacing all occurences of a character or string from column at specific row(s)
    db2.update_replace("users", "pw_hash", "k", "co", "id = 2");
    // replacing all occurences of character or string from ALL column x
    db2.update_replace("users", "pw_hash", "k", "co");
    // deleting a specific row from table
    db2.delete_from("users", "opt_email='null'");
    db2.delete_from("users", "id=1");
    // deleting an entire column from table
    db2.drop_column("users", "pw_hash");
*/
#endif
