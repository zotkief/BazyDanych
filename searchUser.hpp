#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int searchUser(sql::Connection *conn)
{
    int argumentNumber=1;
    std::string nick, firstName, lastName, email, book;
    std::cout << "Wprowadź nick (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, nick);
    std::getline(std::cin, nick);

    std::cout << "Wprowadź imię użytkownika (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, firstName);

    std::cout << "Wprowadź nazwisko użytkownika (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, lastName);

    std::cout << "Wprowadź email (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, email);

    std::cout << "Wprowadź wyporzyczoną książkę (lub naciśnij Enter, aby pominąć): ";
    std::getline(std::cin, book);

    // Budowanie zapytania SQL
    std::string query = "SELECT DISTINCT User.id,User.email,User.firstName,User.lastName,User.nick ";
    query += "FROM User ";
    query += "LEFT JOIN Borrow ON User.id=Borrow.userId ";
    query += "LEFT JOIN Book ON Borrow.bookId=Book.id ";
    query += "WHERE 1=1 ";

    // Dodawanie warunków w zależności od danych
    if (!nick.empty()) {
        query += "AND User.nick LIKE ? ";
    }
    if (!firstName.empty()) {
        query += "AND User.firstName LIKE ? ";
    }
    if (!lastName.empty()) {
        query += "AND User.lastName LIKE ? ";
    }
    if (!email.empty()) {
        query += "AND User.email LIKE ? ";
    }
    if (!book.empty()) {
        query += "AND Book.name LIKE ? ";
    }

    // Przygotowanie zapytania i wykonanie
    try {
        sql::PreparedStatement *stmt = conn->prepareStatement(query);

        if (!nick.empty()) {
            stmt->setString(argumentNumber,"%"+nick+"%");
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
        if (!email.empty()) {
            stmt->setString(argumentNumber,"%"+email+"%");
            argumentNumber++;
        }
        if (!book.empty()) {
            stmt->setString(argumentNumber,"%"+book+"%");
            argumentNumber++;
        }

        sql::ResultSet *res = stmt->executeQuery();
        
        // Przetwarzanie wyników zapytania
        while (res->next()) {
            std::cout << "ID: " << res->getInt("id") << ", Nick: " << res->getString("nick")
                      << ", Name: " << res->getString("firstName") << " " << res->getString("lastName")
                      << ", Email: " << res->getString("email") << std::endl;
        }
    } catch (sql::SQLException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
