#include <FL/Fl.H>       
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Window.H> 
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <string>
#include <mariadb/mysql.h>
#include <iostream>
#include "admin.hpp"


struct Inputs {
    Fl_Input* username;
    Fl_Input* password;
    Fl_Window* window;
};
void button_callback(Fl_Widget* widget, void* data) {
    Fl_Button* button = (Fl_Button*)widget;

    Inputs* inputs = (Inputs*)data;
    const char *user=inputs->username->value();
    const char *password=inputs->password->value();

    MYSQL *conn;

    const char *host = "localhost";
    const char *dbname = "lista5";

    conn = mysql_init(NULL);

    if (mysql_real_connect(conn, host, user, password, dbname, 0, NULL, 0)) {
        inputs->window->hide();
        if(user=="admin")
            adminWindow(conn);
    } else {
        std::cerr << "Błąd połączenia: " << mysql_error(conn) << std::endl;
    }
    mysql_close(conn);
}

int loginWindow()
{
    Fl_Window *window = new Fl_Window(400, 300, "login");
    Fl_Input *username = new Fl_Input(100,50,200,50,"username");
    Fl_Secret_Input *password = new Fl_Secret_Input(100,120,200,50,"password");
    Fl_Button *button = new Fl_Button(160, 220, 80, 40, "Click me");

    Inputs inputs = { username, password,window};
    button->callback(button_callback,&inputs);

    window->end();
    window->show(); 
    Fl::run();
    return 0;
}