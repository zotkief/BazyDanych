#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

int addBook(sql::Connection *conn)
{
    try {
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

        // Przygotowanie zapytania SQL do dodania książki
        std::string query = "CALL AddBook(?, ?, ?, ?, ?)";

        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1, title);
        stmt->setString(2, firstName);
        stmt->setString(3, lastName);
        stmt->setInt(4, atoi(year.c_str()));  // Konwersja roku na int
        stmt->setString(5, isbn);

        // Wykonanie zapytania
        stmt->executeUpdate();
        std::cout << "Książka została dodana do systemu!" << std::endl;

        delete stmt;  // Zwolnienie zasobów
    } catch (const std::invalid_argument& e) {
        std::cout << "Błąd: Nieprawidłowy argument: " << e.what() << std::endl;
        return -1;
    } catch (const std::out_of_range& e) {
        std::cout << "Błąd: Poza zakresem: " << e.what() << std::endl;
        return -1;
    } catch (sql::SQLException &e) {
        std::cout << "Błąd SQL: " << e.what() << std::endl;
        return -1;
    }

    return 0;  // Sukces
}
