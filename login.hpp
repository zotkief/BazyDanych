#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <iostream>
#include "admin.hpp"
#include "user.hpp"

int loginWindow()
{   

    std::string user, pass;
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;
    try{



        std::cout << "Username: ";
        std::cin >> user;
        std::cout << "Password: ";
        std::cin >> pass;


        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", user, pass);
        conn->setSchema("lista5");

        adminView(conn);
    } catch (sql::SQLException &e) {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", "user", "user");
        conn->setSchema("lista5");


        std::string query = "SELECT userAuth(?,?) AS valid;";
        sql::PreparedStatement *stmt = conn->prepareStatement(query);
        stmt->setString(1, user);
        stmt->setString(2, pass);

        sql::ResultSet *res = stmt->executeQuery();
        if(res->next()&&res->getInt("valid"))
        {
            userView(conn);
        }
    }
    return 0;
}