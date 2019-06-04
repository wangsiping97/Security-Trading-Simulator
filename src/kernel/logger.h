#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include "../shell/user_shell.cpp"
#include "../shell/stock_shell.cpp"

#define KEY 4962873

using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::endl;

class Logger {
private: 
    string type;
    static string path;
    string userName;
    string userPath;
    string encrypt(string src_pass, int key);
public: 
    Logger (string const& _type, string const& _userName);
    bool exist();
    void reg(string _password);
    bool login (string _passward);
    User* getNewUser();
    Stock* getNewStock();
};

#endif 