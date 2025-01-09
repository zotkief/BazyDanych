#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int searchBook(sql::Connection *conn)
{
    int argumentNumber=1;
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

    // Budowanie zapytania SQL
    std::string query = "SELECT DISTINCT Book.id, Book.title, Author.firstName, Author.lastName, Book.year, Book.isbn ";
    query += "FROM Book INNER JOIN Author ON Author.id = Book.authorId ";

    if (!genre.empty()) {
        query += "INNER JOIN Category_Book ON Book.id = Category_Book.bookId ";
        query += "INNER JOIN Category ON Category_Book.categoryId = Category.id ";
    }

    query += "WHERE 1=1 ";

    // Dodawanie warunków w zależności od danych
    if (!title.empty()) {
        query += "AND Book.title LIKE ? ";
    }
    if (!isbn.empty()) {
        query += "AND Book.isbn LIKE ? ";
    }
    if (!year.empty()) {
        query += "AND Book.year LIKE ? ";
    }
    if (!firstName.empty()) {
        query += "AND Author.firstName LIKE ? ";
    }
    if (!lastName.empty()) {
        query += "AND Author.lastName LIKE ? ";
    }
    if (!genre.empty()) {
        query += "AND Category.name LIKE ? ";
    }

    // Przygotowanie zapytania i wykonanie
    try {
        sql::PreparedStatement *stmt = conn->prepareStatement(query);

        if (!title.empty()) {
            stmt->setString(argumentNumber,"%"+title+"%");
            argumentNumber++;
        }
        if (!isbn.empty()) {
            stmt->setString(argumentNumber,"%"+isbn+"%");
            argumentNumber++;
        }
        if (!year.empty()) {
            stmt->setString(argumentNumber,"%"+year+"%");
            argumentNumber++;
        }
        if (!firstName.empty()) {
            stmt->setString(argumentNumber,"%"+firstName+"%");
            argumentNumber++;
        }
        if (!lastName.empty()) {
            stmt->setString(argumentNumber,"%"+lastName+"%");
            argumentNumber++;
        }
        if (!genre.empty()) {
            stmt->setString(argumentNumber,"%"+genre+"%");
        }

        sql::ResultSet *res = stmt->executeQuery();
        
        // Przetwarzanie wyników zapytania
        while (res->next()) {
            std::cout << "ID: " << res->getInt("id") << ", Title: " << res->getString("title")
                      << ", Author: " << res->getString("firstName") << " " << res->getString("lastName")
                      << ", Year: " << res->getInt("year") << ", ISBN: " << res->getString("isbn") << std::endl;
        }
    } catch (sql::SQLException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
