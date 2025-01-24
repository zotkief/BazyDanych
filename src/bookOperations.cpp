#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "connector.hpp"

int connector::addBook()
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
int connector::deleteBook()
{
    int bookId;

    std::cout << "Wprowadź ID książki, którą chcesz usunąć: ";
    std::cin >> bookId;

    if (bookId <= 0) {
        std::cout << "Błąd: Nieprawidłowe ID!" << std::endl;
        return -1;  // Zwróć błąd, jeśli ID jest nieprawidłowe
    }

    // Przygotowanie zapytania SQL do usunięcia książki
    std::string query = "DELETE FROM Book WHERE id = ?";

    try {
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setInt(1, bookId);  // Przekazanie ID książki

        // Wykonanie zapytania
        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            std::cout << "Książka o ID " << bookId << " została usunięta." << std::endl;
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
int connector::searchBook()
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
int connector::updateBook()
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

