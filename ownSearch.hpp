#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int ownSearch(sql::Connection *conn,std::string nick)
{
    try {
        sql::PreparedStatement *stmt = conn->prepareStatement("SELECT id,title,year FROM ownBook WHERE nick= ?;");
        stmt->setString(1,nick);

        sql::ResultSet *res = stmt->executeQuery();
        
        // Przetwarzanie wyników zapytania
        while (res->next()) {
            std::cout << "ID: " << res->getInt("id") << ", Title: " << res->getString("title")
                      <<", Year: "<< res->getInt("year") <<std::endl;
        }
        delete stmt;
        delete res;
    } catch (sql::SQLException &e) {
        std::cout << e.what() << std::endl;
        std::cout << "SQL Error Code: " << e.getErrorCode() << std::endl;
        std::cout << "SQL State: " << e.getSQLState() << std::endl;
    }

    return 0;
}
