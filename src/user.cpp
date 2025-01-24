#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

#include "connector.hpp"

void connector::rentBooks(std::string nick)
{
    std::cout << "Wprowadź nazwy książek, które chcesz wyporzyczyć (pozostaw pustą linię, aby zatwierdzić): ";
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
        std::string query = "CALL borrowBooks(?,?)";
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
void connector::returnBooks(std::string nick)
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
int connector::userSearch()
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
int connector::ownSearch(std::string nick)
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
