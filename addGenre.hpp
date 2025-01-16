#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int addGenre(sql::Connection *conn)
{
    std::string title, genre;
    std::cout << "Wprowadź tytuł: ";
    std::getline(std::cin, title);
    std::getline(std::cin, title);

    std::cout << "Wprowadź kategorię: ";
    std::getline(std::cin, genre);

    // Przygotowanie zapytania i wykonanie
    try {
        std::string query = "CALL assignCategory(?,?)";
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1,title);
        stmt->setString(2,genre);
        stmt->executeUpdate();
        delete stmt;
    } catch (sql::SQLException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
