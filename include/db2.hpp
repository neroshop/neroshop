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
// postgreSQL
#include <postgresql/libpq-fe.h> // Linux // https://www.postgresql.org/download/linux/ubuntu/ // sudo apt install postgresql // sudo apt-get install libpq-dev // Ubuntu headers: /usr/include/postgresql

namespace neroshop {
class DB2 { // DB2 is intended for client-server database engine. SQLite does not use a client-server architecture, so unfortunately we cannot solely use SQLite for this as its used for embedding into applications and becomes an integral part of the program
public:
    DB2();
    DB2(const std::string& file_url);
    ~DB2();
private:
};
}
#endif
