#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include "../shell/user_shell.cpp"
#include "../shell/stock_shell.cpp"

using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::endl;

class Logger {
private: 
    string type;
protected: 
    static string path;
    string userName;
    string userPath;
public: 
    Logger (string const& _type, string const& _userName);
    bool exist();
    void reg(string const& _password);
    bool login (string const& passward);
    User* getNewUser();
    Stock* getNewStock();
};

#endif 