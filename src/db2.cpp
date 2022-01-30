#include "../include/db2.hpp"

neroshop::DB2::DB2() : conn(nullptr)
{}
////////////////////
neroshop::DB2::DB2(const std::string& file_url) : DB2() // delegating constructor
{
    //connect();
}
////////////////////
neroshop::DB2::~DB2()
{}
////////////////////
bool neroshop::DB2::connect(const std::string& conninfo) {
    conn = PQconnectdb(conninfo.c_str());   
    if (PQstatus(conn) == CONNECTION_BAD) {    
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        finish();
        return false;
    }
    return true;
}
void neroshop::DB2::finish() {
    PQfinish(conn);
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
int neroshop::DB2::get_lib_version() {
    return PQlibVersion(); // 140001 = 14.1
}
////////////////////
int neroshop::DB2::get_server_version() const {
    if(!conn) throw std::runtime_error("database is not connected");
    return PQserverVersion(conn);
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
