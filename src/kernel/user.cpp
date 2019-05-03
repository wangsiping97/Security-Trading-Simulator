#ifndef _USER_H
#define _USER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <unistd.h>

using std::string;
using std::map;
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

struct Value {
    int numFloats;
    double price;
    double cost;
    double yeild;
};

struct Account {
    map<string, struct Value> asset; // id, 持股数
    double available; // 可用
    double total; // 总资产
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
    struct Account getAccount();
};

string User::stockPath = thispath + SLASH + ".." + SLASH + "data" + SLASH + "Stock";

User::User (string const& _name): name(_name) {
    myPath = thispath + SLASH + ".." + SLASH + "data" + SLASH + "User" + SLASH + name;
    myAccount.asset.clear();
}

string User::search (string const& item, string const& id) {
    fstream file(stockPath + SLASH + id);
    if (!file) return "";
    string line;
    getline(file, line); // password
    getline(file, line); // price
    if (item == "price") return line;
    getline(file, line); // floats available
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

struct Account User::getAccount () {
    fstream file (myPath);
    string line;
    getline(file, line); // password
    getline(file, line); // available
    myAccount.available = atof(line.data());
    myAccount.total = myAccount.available;
    string id, nf, cost;
    while (getline(file, id) && getline(file, nf) && getline(file, cost)) {
        struct Value v;
        v.numFloats = atoi(nf.data());
        v.cost = atof(cost.data());
        v.price = atof(search("price", id).data());
        v.yeild = (v.price / v.cost - 1) * 100;
        myAccount.asset.insert(std::pair<string, struct Value>(id, v));
        myAccount.total += v.price * v.numFloats;
    }
    return myAccount;
}

#endif