#pragma once

#include <string>
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "userSearch.hpp"
#include "rentBooks.hpp"
#include "returnBooks.hpp"
#include "ownSearch.hpp"

void userView(sql::Connection *conn,std::string nick)
{
    while(true)
    {
        // Wyświetlenie menu z opcjami
        std::cout << "===================================" << std::endl;
        std::cout << "Wybierz jedną z opcji:" << std::endl;
        std::cout << "0. Wyjście" << std::endl;
        std::cout << "1. Wyszukaj dostępne książki" << std::endl;
        std::cout << "2. Wypożycz książki" << std::endl;
        std::cout << "3. Oddaj książki" << std::endl;
        std::cout << "4. Wyszukaj swoje książki" << std::endl;
        std::cout << "===================================" << std::endl;

        int n;
        std::cout << "Wprowadź numer opcji: ";
        std::cin >> n;

        switch (n)
        {
        case 1:
            userSearch(conn);
            break;
        case 2:
            rentBooks(conn,nick);
            break;
        case 3:
            returnBooks(conn,nick);
            break;
        case 4:
            ownSearch(conn,nick);
            break;
        case 0:
            return;
            break;
        default:
            break;
        }
    }
}
