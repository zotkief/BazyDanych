#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>
 
void returnBooks(sql::Connection *conn,std::string nick)
{
    std::cout << "Wprowadź nazwy książek, które chcesz oddać (pozostaw pustą linię, aby zatwierdzić): ";
    std::string title,titles;
    std::getline(std::cin,title);
    std::getline(std::cin,title);
    while(!title.empty())
    {
        titles+=title;
        titles+=",";
        std::getline(std::cin,title);
    }
    try {
        std::string query = "CALL returnBooks(?,?)";
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1,nick);
        stmt->setString(2,titles);
        stmt->executeUpdate();
        delete stmt;
    } catch (sql::SQLException &e) {
        std::cout << "Błąd SQL: " << e.what() << std::endl;
        return;
    }
}