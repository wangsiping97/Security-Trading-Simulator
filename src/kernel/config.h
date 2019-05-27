#ifndef _CONFIG_H
#define _CONFIG_H

#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <map> 
#include <ctime>
#include <dirent.h>
#include <unistd.h>

#if defined _WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

#define MAX_PRICE 100000000

using std::fstream;
using std::vector;
using std::string;
using std::map;

// 时间
string getCurrentTime () {
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    string year = std::to_string(t_tm->tm_year + 1900);
    string month = std::to_string(t_tm->tm_mon + 1).length() == 1 ? "0" + std::to_string(t_tm->tm_mon + 1) : std::to_string(t_tm->tm_mon + 1);
    string day = std::to_string(t_tm->tm_mday).length() == 1 ? "0" + std::to_string(t_tm->tm_mday) : std::to_string(t_tm->tm_mday);
    string hour = std::to_string(t_tm->tm_hour).length() == 1 ? "0" + std::to_string(t_tm->tm_hour) : std::to_string(t_tm->tm_hour);
    string min = std::to_string(t_tm->tm_min).length() == 1 ? "0" + std::to_string(t_tm->tm_min) : std::to_string(t_tm->tm_min);
    string sec = std::to_string(t_tm->tm_sec).length() == 1 ? "0" + std::to_string(t_tm->tm_sec) : std::to_string(t_tm->tm_sec);
    return year + month + day + hour + min + sec;
}

// 路径
string thisPath = getcwd(NULL, 0);
string stockPath = thisPath + SLASH + (string)".." + SLASH + (string)"data" + SLASH + (string)"Stock";
string userPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "User";
string dataPath = thisPath + SLASH + ".." + SLASH + "data";

// 买家投标
struct Buy {
    double price;
    int num_of_shares;
    string userName;
    string time;
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
    string time;
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