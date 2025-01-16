#pragma once

#include <string>
#include <iostream>
#include "addBook.hpp"
#include "searchBook.hpp"
#include "deleteBook.hpp"
#include "updateBook.hpp"
#include "addUser.hpp"
#include "deleteUser.hpp"
#include "searchUser.hpp"
#include "updateUser.hpp"
#include "addGenre.hpp"

void adminView(sql::Connection *conn)
{
    while(true)
    {
        // Wyświetlenie menu z opcjami
        std::cout << "===================================" << std::endl;
        std::cout << "Wybierz jedną z opcji:" << std::endl;
        std::cout << "0. Wyjście" << std::endl;
        std::cout << "1. Dodaj książkę do systemu" << std::endl;
        std::cout << "2. Wyszukaj książkę w systemie" << std::endl;
        std::cout << "3. Usuń książkę o podanym id" << std::endl;
        std::cout << "4. Edytuj książkę o podanym id" << std::endl;
        std::cout << "5. Dodaj użytkownika do systemu" << std::endl;
        std::cout << "6. Usuń użytkownika o podanym id" << std::endl;
        std::cout << "7. Wyszukaj użytkownika w systemie" << std::endl;
        std::cout << "8. Edytuj użotkownika o podanym id" << std::endl;
        std::cout << "9. Dodaj gatunek do książki" << std::endl;
        std::cout << "===================================" << std::endl;

        int n;
        std::cin >> n;

        switch (n)
        {
        case 1:
            addBook(conn);
            break;
        case 2:
            searchBook(conn);
            break;
        case 3:
            deleteBook(conn);
            break;
        case 4:
            updateBook(conn);
            break;
        case 5:
            addUser(conn);
            break;
        case 6:
            deleteUser(conn);
            break;
        case 7:
            searchUser(conn);
            break;
        case 8:
            updateUser(conn);
            break;
        case 9:
            addGenre(conn);
            break;
        case 0:
            return;
            break;
        default:
            break;
        }
    }
}
