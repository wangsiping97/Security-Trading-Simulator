#ifndef _USER_SHELL_H
#define _USER_SHELL_H

#include <iomanip>
#include "shell_base.cpp"
#include "../kernel/user.cpp"

using std::setw;

class User_Shell: public Shell_Base {
private: 
    User* user;
    static const char HELP[];
    static const char INSTRUCTION[];
    bool parseCommand (string& command);
    bool match(string const& a, string const& b);
public: 
    User_Shell (User* _user, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
    void showStockList();
    void wrongStock (string const& id);
    void run ();
}; 

#endif // user_shell.h