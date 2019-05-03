#ifndef _STOCK_H
#define _STOCK_H

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

struct Info {
    double price;
    string industry;
    int floats;
    double roa;
    double roe;
};

class Stock {
private: 
    string myPath;
    struct Info myInfo;
public: 
    string id;
public: 
    Stock (string const& _id);
    struct Info getInfo();
    void setInfo(string const&type, string const& newInfo);
    void updateInfo();
};

Stock::Stock (string const& _id): id(_id) {
    string thispath = getcwd(NULL, 0);
    myPath = thispath + SLASH + ".." + SLASH + "data" + SLASH + "Stock" + SLASH + id;
}

struct Info Stock::getInfo() {
    fstream file(myPath);
    string line;
    getline(file, line); // password
    getline(file, line); // price
    myInfo.price = atof(line.data()); 
    getline(file, line); // floats available
    getline(file, line); // industry
    myInfo.industry = line;
    getline(file, line); // floats
    myInfo.floats = atoi(line.data());
    getline(file, line); // roa
    myInfo.roa = atof(line.data());
    getline(file, line); // roe
    myInfo.roe = atof(line.data());
    file.close();
    return myInfo;
}

void Stock::setInfo(string const&type, string const& newInfo) {
    if (type == "floats") {
        myInfo.floats = atoi(newInfo.data());
        // 传给全局函数
    }
    else if (type == "industry") {
        myInfo.industry = newInfo;
    }
    else if (type == "roa") {
        myInfo.roa = atof(newInfo.data());
    }
    else {
        myInfo.roa = atof(newInfo.data());
    }
    updateInfo();
}

void Stock::updateInfo() {
    fstream file(myPath);
    string password;
    getline(file, password);
    string price;
    getline(file, price);
    string floats_available;
    getline (file, floats_available);
    file.close();
    ofstream out(myPath);
    out << password << endl;
    out << price << endl;
    out << floats_available << endl;
    out << myInfo.industry << endl;
    out << myInfo.floats << endl;
    out << myInfo.roa << endl;
    out << myInfo.roe << endl;
}

#endif