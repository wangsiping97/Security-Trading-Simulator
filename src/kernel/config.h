#ifndef _CONFIG_H
#define _CONFIG_H

#include <fstream>
#include <vector>
#include <string>
#include <map> 

#ifdef WIN32  
#include <direct.h>  
#include <io.h>  
#else
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef _WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

using std::fstream;
using std::vector;
using std::string;
using std::map;

// 路径
string thisPath = getcwd(NULL, 0);
string stockPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "Stock";
string userPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "User";
string dataPath = thisPath + SLASH + ".." + SLASH + "data";

// 买家投标
struct Buy {
    double price;
    int num_of_shares;
    string userName;
    Buy() {}
    Buy (double _price, int _num_of_shares);
    Buy (double _price, int _num_of_shares, string _userName);
    // 重载 < 操作符
    bool operator < (const struct Buy& right) const;
    bool operator > (const struct Buy& right) const;
};

// 卖家投标
struct Sell {
    double price;
    int num_of_shares;
    string userName;
    Sell () {}
    Sell (double _price, int _num_of_shares);
    Sell (double _price, int _num_of_shares, string _userName);
    bool operator > (const struct Sell& right) const;
    bool operator < (const struct Sell& right) const;
};

// 每一只股票的挂牌信息
struct Bids { 
    string id;
    vector<struct Buy> buysInfo;
    vector<struct Sell> sellsInfo;
    Bids () {}
    Bids (string _id);
};

// User account
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

// Stock info
struct Info {
    double price;
    int floats_available;
    string industry;
    int floats;
    double roa;
    double roe;
};

#endif