#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int updateBook(sql::Connection *conn)
{
    int bookId;

    std::cout << "Wprowadź ID książki, którą chcesz edytować: ";
    std::cin >> bookId;

    std::string firstName, lastName, title, year, isbn;

    // Prośba o wprowadzenie danych książki
    std::cout << "===================================" << std::endl;
    std::cout << "Dodawanie książki do systemu:" << std::endl;
    std::cout << "Wprowadź tytuł książki: ";
    std::cin >> title;

    std::cout << "Wprowadź imię autora: ";
    std::cin >> firstName;

    std::cout << "Wprowadź nazwisko autora: ";
    std::cin >> lastName;

    std::cout << "Wprowadź rok wydania książki: ";
    std::cin >> year;

    std::cout << "Wprowadź numer ISBN książki: ";
    std::cin >> isbn;
    std::cout << "===================================" << std::endl;

    // Przygotowanie zapytania SQL do edytowania książki
    std::string query = "CALL UpdateBook(?, ?, ?, ?, ?, ?)";

    try {
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setInt(1, bookId);  // Przekazanie ID książki
        stmt->setString(2, title);
        stmt->setString(3, firstName);
        stmt->setString(4, lastName);
        stmt->setInt(5, atoi(year.c_str()));  // Konwersja roku na int
        stmt->setString(6, isbn);

        stmt->executeUpdate();

        delete stmt;  // Zwolnienie zasobów
    } catch (sql::SQLException &e) {
        std::cout << "Błąd SQL: " << e.what() << std::endl;
        return -1;  // Zwróć błąd, jeśli wystąpił błąd SQL
    }

    return 0;  // Sukces
}
