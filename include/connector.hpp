#ifndef Connector_H
#define Connector_H

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

class connector
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;

    int addBook();
    int deleteBook();
    int searchBook();
    int updateBook();

    int updateUser();
    int searchUser();
    int deleteUser();
    int addUser();

    int addGenre();

    void userView(std::string nick);
    void rentBooks(std::string nick);
    void returnBooks(std::string nick);
    int ownSearch(std::string nick);

    int userSearch();
    void adminView();
public:
    void loginWindow();
};

#endif