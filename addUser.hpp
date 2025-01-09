#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

int addUser(sql::Connection *conn)
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

        // Przygotowanie zapytania SQL do dodania książki
        std::string query = "INSERT INTO User(firstName,lastName,password,email,nick)";
        query+="VALUES(?,?,?,?,?)";

        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1, firstName);
        stmt->setString(2, lastName);
        stmt->setString(3, password); 
        stmt->setString(4, email);
        stmt->setString(5, nick);

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
