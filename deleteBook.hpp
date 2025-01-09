#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

int deleteBook(sql::Connection *conn)
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
