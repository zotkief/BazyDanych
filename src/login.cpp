#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>

#include "connector.hpp"

#include "../lib/bcrypt/bcrypt.h"

void connector::loginWindow()
{   

    std::string user, pass;
    try{



        std::cout << "Username: ";
        std::cin >> user;
        std::cout << "Password: ";
        std::cin >> pass;


        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", user, pass);
        conn->setSchema("lista5");

        adminView();
    } catch (sql::SQLException &e) {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", "user", "user");
        conn->setSchema("lista5");
        std::string query = "SELECT salt From salty WHERE nick=?;";
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1, user);
        sql::ResultSet *res = stmt->executeQuery();
        

        char hashed[255];
        std::string salt;
        if(res->next())
        {
            salt=res->getString("salt");
        }

        bcrypt_hashpw(pass.c_str(), salt.c_str(),hashed);

        query = "SELECT userAuth(?,?) AS valid;";
        stmt = conn->prepareStatement(query);
        stmt->setString(1, user);
        stmt->setString(2, hashed);

        res = stmt->executeQuery();
        if(res->next()&&res->getInt("valid"))
        {
            userView(user);
        }
    }
    return;
}
void connector::adminView()
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
            addBook( );
            break;
        case 2:
            searchBook( );
            break;
        case 3:
            deleteBook( );
            break;
        case 4:
            updateBook( );
            break;
        case 5:
            addUser( );
            break;
        case 6:
            deleteUser( );
            break;
        case 7:
            searchUser( );
            break;
        case 8:
            updateUser( );
            break;
        case 9:
            addGenre( );
            break;
        case 0:
            return;
            break;
        default:
            break;
        }
    }
}
void connector::userView(std::string nick)
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
            userSearch();
            break;
        case 2:
            rentBooks(nick);
            break;
        case 3:
            returnBooks(nick);
            break;
        case 4:
            ownSearch(nick);
            break;
        case 0:
            return;
            break;
        default:
            break;
        }
    }
}

