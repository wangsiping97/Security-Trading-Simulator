#ifndef _USER_H
#define _USER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <unistd.h>

using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::endl;

#ifdef _WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

string thispath = getcwd(NULL, 0);

struct Account {
    double price;
    string industry;
    int floats;
    double roa;
    double roe;
};

class User {
private: 
    string myPath;
    struct Account myAccount;
public: 
    static string stockPath;
    string name;
public: 
    User (string const& _name);
    string search (string const& item, string const& id);
};

string User::stockPath = thispath.substr(0, thispath.length() - 4) + SLASH + "data" + SLASH + "Stock";

User::User (string const& _name): name(_name) {
    myPath = thispath.substr(0, thispath.length() - 4) + SLASH + "data" + SLASH + "User" + SLASH + name;
}

string User::search (string const& item, string const& id) {
    fstream file(stockPath + SLASH + id);
    if (!file) return "";
    string line;
    getline(file, line); // password
    getline(file, line); // price
    if (item == "price") return line;
    getline(file, line); // industry
    if (item == "industry") return line;
    getline(file, line); // floats
    if (item == "floats") return line;
    getline(file, line); // roa
    if (item == "roa") return line;
    getline(file, line); // roe
    if (item == "roe") return line;
    return "";
}

#endif