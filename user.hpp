#pragma once

#include <string>
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "userSearch.hpp"

void userView(sql::Connection *conn)
{
    while(true)
    {
        // Wyświetlenie menu z opcjami
        std::cout << "===================================" << std::endl;
        std::cout << "Wybierz jedną z opcji:" << std::endl;
        std::cout << "0. Wyjście" << std::endl;
        std::cout << "1. Wyszukaj dostępne książki" << std::endl;
        std::cout << "===================================" << std::endl;

        int n;
        std::cout << "Wprowadź numer opcji: ";
        std::cin >> n;

        switch (n)
        {
        case 1:
            userSearch(conn);
            break;
        case 0:
            return;
            break;
        default:
            break;
        }
    }
}
