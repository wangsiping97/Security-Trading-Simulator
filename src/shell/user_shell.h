#ifndef _USER_SHELL_H
#define _USER_SHELL_H

#include <iomanip>
#include "shell_base.cpp"
#include "../kernel/user.cpp"

using std::setw;

class User_Shell: private Shell_Base {
private: 
    User* user;
    static const char HELP[];
    static const char INSTRUCTION[];
private: 
    bool parseCommand (string& command);
    void wrongStock (string const& id);
public: 
    User_Shell (User* _user, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
    void run ();
}; 

#endif // user_shell.h