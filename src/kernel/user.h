#ifndef _USER_H
#define _USER_H

#include "trading.cpp"

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

#endif