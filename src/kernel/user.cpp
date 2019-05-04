#ifndef _USER_H
#define _USER_H

#include "trading.cpp"

struct Value {
    int numFloats;
    double price;
    double cost;
    double yield;
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
    string name;
public: 
    User (string const& _name);
    string search (string const& item, string const& id);
    struct Account getAccount();
};

User::User (string const& _name): name(_name) {
    myPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "User" + SLASH + name;
    myAccount.asset.clear();
    myAccount.available = 0; // 最初情况
    myAccount.total = 0; // 最初情况
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
    myAccount.asset.clear();
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
        v.yield = (v.price / v.cost - 1) * 100;
        myAccount.asset.insert(std::pair<string, struct Value>(id, v));
        myAccount.total += v.price * v.numFloats;
    }
    return myAccount;
}

#endif