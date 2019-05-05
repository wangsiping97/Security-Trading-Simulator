#ifndef _STOCK_H
#define _STOCK_H

#include "trading.cpp"

class Stock {
private: 
    string myPath;
    struct Info myInfo;
public: 
    string id;
public: 
    Stock (string const& _id);
    struct Info getInfo();
    bool setInfo(string const&type, string const& newInfo);
    void updateInfo();
};

#endif // stock.h