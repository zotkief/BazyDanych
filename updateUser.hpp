#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

int updateUser(sql::Connection *conn)
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
