#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int userSearch(sql::Connection *conn)
{
    std::string title, firstName, lastName, isbn, year, genre;
    std::cout << "Wprowadź tytuł (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, title);
    std::getline(std::cin, title);

    std::cout << "Wprowadź imię autora (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, firstName);

    std::cout << "Wprowadź nazwisko autora (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, lastName);

    std::cout << "Wprowadź ISBN (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, isbn);

    std::cout << "Wprowadź rok wydania (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, year);

    std::cout << "Wprowadź kategorię (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, genre);

    // Przygotowanie zapytania i wykonanie
    try {
        sql::PreparedStatement *stmt = conn->prepareStatement("CALL userSearch(?, ?, ?, ?, ?, ?)");
        stmt->setString(1,title);
        stmt->setString(2,firstName);
        stmt->setString(3,lastName);
        stmt->setString(4,isbn);
        stmt->setString(5,year);
        stmt->setString(6,genre);

        sql::ResultSet *res = stmt->executeQuery();
        
        // Przetwarzanie wyników zapytania
        while (res->next()) {
            std::cout << "ID: " << res->getInt("id") << ", Title: " << res->getString("title")
                      << res->getInt("year") <<std::endl;
        }
    } catch (sql::SQLException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
