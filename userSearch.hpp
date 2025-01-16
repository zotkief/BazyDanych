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
        std::string query="SELECT id,title,year FROM availableBook WHERE 1=1 ";
        if(!title.empty())
            query += "AND title LIKE '%"+title+"%'";
        if(!firstName.empty())
            query += "AND firstName LIKE '%"+firstName+"%'";
        if(!lastName.empty())
            query += "AND lastName LIKE '%"+lastName+"%'";
        if(!isbn.empty())
            query += "AND isbn LIKE '%"+isbn+"%'";
        if(!year.empty())
            query += "AND year LIKE '%"+year+"%'";
        if(!genre.empty())
            query += "AND (genre LIKE '%"+genre+"%' OR genre IS NULL);";
        sql::PreparedStatement *stmt = conn->prepareStatement(query);

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
    }
    return 0;
}
