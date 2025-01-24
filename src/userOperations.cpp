#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "connector.hpp"

#include "../lib/bcrypt/bcrypt.h"

int connector::updateUser()
{
    int userId;

    std::cout << "Wprowadź ID użytkownika, którego dane chcesz edytować: ";
    std::cin >> userId;

    std::string firstName, lastName, password, email, nick;

    // Prośba o wprowadzenie nowych danych użytkownika
    std::cout << "===================================" << std::endl;
    std::cout << "Edycja danych użytkownika:" << std::endl;
    std::cout << "Wprowadź nowe imię użytkownika: ";
    std::cin >> firstName;

    std::cout << "Wprowadź nowe nazwisko użytkownika: ";
    std::cin >> lastName;

    std::cout << "Wprowadź nowy nick użytkownika: ";
    std::cin >> nick;

    std::cout << "Wprowadź nowy email użytkownika: ";
    std::cin >> email;

    std::cout << "Wprowadź nowe hasło użytkownika: ";
    std::cin >> password;
    std::cout << "===================================" << std::endl;

    // Przygotowanie zapytania SQL do edytowania użytkownika
    std::string query = "UPDATE User SET firstName = ?, lastName = ?, password = ?, email = ?, nick = ? WHERE id = ?";

    try {
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1, firstName);
        stmt->setString(2, lastName);
        stmt->setString(3, password);
        stmt->setString(4, email);
        stmt->setString(5, nick);
        stmt->setInt(6, userId);  // Przekazanie ID użytkownika do warunku WHERE

        // Wykonanie zapytania
        stmt->executeUpdate();
        std::cout << "Dane użytkownika zostały zaktualizowane!" << std::endl;

        delete stmt;  // Zwolnienie zasobów
    } catch (sql::SQLException &e) {
        std::cout << "Błąd SQL: " << e.what() << std::endl;
        return -1;  // Zwróć błąd, jeśli wystąpił problem z SQL
    }

    return 0;  // Sukces
}
int connector::searchUser()
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
int connector::deleteUser()
{
    int userId;

    std::cout << "Wprowadź ID użytkownika, którego chcesz usunąć: ";
    std::cin >> userId;

    if (userId <= 0) {
        std::cout << "Błąd: Nieprawidłowe ID!" << std::endl;
        return -1;  // Zwróć błąd, jeśli ID jest nieprawidłowe
    }

    // Przygotowanie zapytania SQL do usunięcia książki
    std::string query = "DELETE FROM User WHERE id = ?";

    try {
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setInt(1, userId);  // Przekazanie ID książki

        // Wykonanie zapytania
        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            std::cout << "Książka o ID " << userId << " została usunięta." << std::endl;
        } else {
            std::cout << "Nie znaleziono książki o podanym ID." << std::endl;
        }

        delete stmt;  // Zwolnienie zasobów
    } catch (sql::SQLException &e) {
        std::cout << "Błąd SQL: " << e.what() << std::endl;
        return -1;  // Zwróć błąd, jeśli wystąpił błąd SQL
    }

    return 0;  // Sukces
}
int connector::addUser()
{
    try {
        std::string firstName, lastName, password, email, nick;

        // Prośba o wprowadzenie danych książki
        std::cout << "===================================" << std::endl;
        std::cout << "Dodawanie użytkownika do systemu:" << std::endl;

        std::cout << "Wprowadź imię użytkownika: ";
        std::cin >> firstName;

        std::cout << "Wprowadź nazwisko użytkownika: ";
        std::cin >> lastName;

        std::cout << "Wprowadź nick użytkownika: ";
        std::cin >> nick;

        std::cout << "Wprowadź email użytkownika: ";
        std::cin >> email;

        std::cout << "Wprowadź hasło użytkownika: ";
        std::cin >> password;
        std::cout << "===================================" << std::endl;

        char hashed[255];
        char salt[255];
        bcrypt_gensalt(10,salt);
        bcrypt_hashpw(password.c_str(), salt,hashed);

        // Przygotowanie zapytania SQL do dodania książki
        std::string query = "INSERT INTO User(firstName,lastName,password,email,nick,salt)";
        query+="VALUES(?,?,?,?,?,?)";

        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1, firstName);
        stmt->setString(2, lastName);
        stmt->setString(3, hashed); 
        stmt->setString(4, email);
        stmt->setString(5, nick);
        stmt->setString(6, salt);


        // Wykonanie zapytania
        stmt->executeUpdate();
        std::cout << "Użytkownik został dodany do systemu!" << std::endl;

        delete stmt;  // Zwolnienie zasobów
    } catch (sql::SQLException &e) {
        std::cout << "Błąd SQL: " << e.what() << std::endl;
        return -1;
    }

    return 0;  // Sukces
}

