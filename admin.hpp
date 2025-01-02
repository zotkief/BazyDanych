#include <FL/Fl.H>       
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Window.H> 
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <string>
#include <mariadb/mysql.h>
#include <iostream>

//users
void addUserCallback(Fl_Widget* widget, void* data)
{
    
}
void searchUserCallback(Fl_Widget* widget, void* data)
{
    
}
void updateUserCallback(Fl_Widget* widget, void* data)
{
    
}
void deleteUserCallback(Fl_Widget* widget, void* data)
{
    
}
//books
void addBookCallback(Fl_Widget* widget, void* data)
{

}
void searchBookCallback(Fl_Widget* widget, void* data)
{
    
}
void updateBookCallback(Fl_Widget* widget, void* data)
{
    
}
void deleteBookCallback(Fl_Widget* widget, void* data)
{
    
}
//main
int adminWindow(MYSQL* conn)
{
    Fl_Window *window = new Fl_Window(200, 400, "admin");

    Fl_Button *addUser=new Fl_Button(0,0,200,45,"add user");
    addUser->callback(addUserCallback,conn);
    Fl_Button *searchUser=new Fl_Button(0,50,200,45,"search user");
    searchUser->callback(searchUserCallback,conn);
    Fl_Button *updateUser=new Fl_Button(0,100,200,45,"update user");
    updateUser->callback(updateUserCallback,conn);
    Fl_Button *deleteUser=new Fl_Button(0,150,200,45,"delete user");
    deleteUser->callback(deleteUserCallback,conn);

    Fl_Button *addBook=new Fl_Button(0,200,200,45,"add book");
    addBook->callback(addBookCallback,conn);
    Fl_Button *searchBook=new Fl_Button(0,250,200,45,"search book");
    searchBook->callback(searchBookCallback,conn);
    Fl_Button *updateBook=new Fl_Button(0,300,200,45,"update book");
    updateBook->callback(updateBookCallback,conn);
    Fl_Button *deleteBook=new Fl_Button(0,350,200,45,"delete book");
    deleteBook->callback(deleteBookCallback,conn);

    window->end();
    window->show(); 

    return Fl::run();
}