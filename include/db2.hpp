// filename: db2.hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability?

#ifndef DATABASE2_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define DATABASE2_HPP_NEROSHOP
#define POSTGRESQL_TAG "\033[1;94m[postgresql]:\033[0m "
#define POSTGRESQL_TAG_ERR "\033[1;94m[postgresql]:\033[0;91m "
#include <iostream>
// neroshop
#include "debug.hpp"
// dokun-ui
// libpq (C interface to PostgreSQL)
#include <postgresql/libpq-fe.h> // Linux // https://www.postgresql.org/download/linux/ubuntu/ // sudo apt install postgresql // sudo apt-get install libpq-dev // Ubuntu headers: /usr/include/postgresql

namespace neroshop {
class DB2 { // DB2 is intended for client-server database engine. SQLite does not use a client-server architecture, so unfortunately we cannot solely use SQLite for this as its used for embedding into applications and becomes an integral part of the program
public:
    DB2();
    DB2(const std::string& file_url);
    ~DB2();
    bool connect(const std::string& conninfo); // string can be empty to use all default parameters
    void finish();
    // setters
    // getters
    static int get_lib_version();
    int get_server_version() const;
private:
    PGconn * conn;
};
}
/*
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

*/
#endif
