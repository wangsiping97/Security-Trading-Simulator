#ifndef _TRADING_H
#define _TRADING_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map> 
#include <algorithm>
#ifdef WIN32  
#include <direct.h>  
#include <io.h>  
#else
#include <unistd.h>
#include <dirent.h>
#endif

using std::string;
using std::map;
using std::vector;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::endl;

#ifdef _WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

string thisPath = getcwd(NULL, 0);
string stockPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "Stock";
string userPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "User";

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

Buy::Buy (double _price, int _num_of_shares): price(_price), num_of_shares(_num_of_shares) {
    userName = "";
}

Buy::Buy (double _price, int _num_of_shares, string _userName): price(_price), num_of_shares(_num_of_shares), userName(_userName) {}

bool Buy::operator < (const struct Buy& right) const {
    return price <= right.price; // 相等时，先来的 > 后来的
}

bool Buy::operator > (const struct Buy& right) const {
    return price > right.price; // 相等时，先来的 > 后来的
}

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

Sell::Sell (double _price, int _num_of_shares): price(_price), num_of_shares(_num_of_shares) {
    userName = "";
}

Sell::Sell (double _price, int _num_of_shares, string _userName): price(_price), num_of_shares(_num_of_shares), userName(_userName) {}

bool Sell::operator > (const struct Sell& right) const {
    return price >= right.price; // 相等时，先来的 < 后来的
}

bool Sell::operator < (const struct Sell& right) const {
    return price < right.price; // 相等时，先来的 < 后来的
}

struct Bids { // 每一只股票的挂牌信息
    string id;
    vector<struct Buy> buysInfo;
    vector<struct Sell> sellsInfo;
    Bids () {}
    Bids (string _id);
    bool operator < (const struct Bids& right) const;
};

Bids::Bids (string _id): id(_id) {
    buysInfo.clear();
    sellsInfo.clear();
    fstream file (stockPath + SLASH + id);
    string line;
    getline(file, line); // password
    getline(file, line); // price
    double openPrice = atof(line.data());
    getline(file, line); // floats_available
    int floats = atoi(line.data());
    struct Sell initSell(openPrice, floats);
    sellsInfo.push_back(initSell);
    std::make_heap(std::begin(sellsInfo), std::end(sellsInfo)); // 卖堆，默认降序
    struct Buy initBuy(-1, -1);
    buysInfo.push_back(initBuy);
    std::make_heap(begin(buysInfo), end(buysInfo), std::greater<struct Buy>()); // 买堆，升序
    file.close();
}

bool Bids::operator < (const struct Bids& right) const {
    return id <= right.id; // 相等时，先来的 < 后来的
}

class Trading {
protected: 
    static bool haveStock (string const& name, string const& id); // 判断仓中是否有这只股票
    static double getAvaliable (string const& name); // 返回可用资金
    static void updateAvailable (string const& name, double diff_available); // 更新可用资金
    static int getHave (string const& name, string const& id); // 返回持仓数
    static void updateHave (string const& name, string const& id, int new_num); // 更新持仓数
    static double getCost (string const& name, string const& id); // 返回原有成本价
    static void updateCost (string const& name, string const& id, double new_cost); // 更新成本价
    static void deleteId (string const& name, string const& id); // 从账户中删除股票
    static void updatePrice (string const& id, double new_price); // 更新股价
    static void updateFloats_available (string const& id, int diff_floats); // 更新 floats_available
    static void trading (string const& id); // 交易
public: 
    static map<string, struct Bids> tradingPool;
    static void init();
    static bool addBuy (string const& name, string const& id, int num, double cost);
    static bool addSell (string const& name, string const& id, int num, double cost);
    static bool isEmpty (string const& id);
    static void changeFloats (string const& id, double old_price, int old_floats_available, int old_floats, int new_floats);
    static void reset(); // 程序结束运行时，还原 tradingPool 中数据
};

map<string, struct Bids> Trading::tradingPool;

void Trading::init() { // 构建 tradingPool
    tradingPool.clear();
    DIR* dir = opendir(stockPath.data());
    struct dirent *pid;
    while ((pid = readdir(dir)) != NULL) {
        if (pid->d_name[0] == '.') continue;
        struct Bids bids(pid->d_name);
        tradingPool[pid->d_name] = bids;
    }
}

bool Trading::haveStock (string const& name, string const& id) {
    fstream userFile(userPath + SLASH + name);
    string line;
    while (getline(userFile, line)) {
        if (line == id) return true;
    }
    return false;
}

double Trading::getAvaliable (string const& name) {
    fstream userFile(userPath + SLASH + name);
    string line;
    getline(userFile, line); // password
    getline(userFile, line); // available
    return atof(line.data());
}

void Trading::updateAvailable (string const& name, double diff_available) {
    fstream userFile(userPath + SLASH + name);
    string line;
    getline(userFile, line); // password
    string password = line; // 暂存 password
    getline(userFile, line); // available
    double new_available = atof(line.data()) + diff_available;
    vector<string> temp;
    temp.clear();
    while (getline(userFile, line)) {
        temp.push_back(line);
    } // 暂存 available 后所有数据
    userFile.close();
    ofstream out(userPath + SLASH + name);
    out << password << endl;
    out << new_available << endl;
    int sz = temp.size();
    for (int i = 0; i < sz; ++i) {
        out << temp[i] << endl;
    }
    out.close();
}

int Trading::getHave (string const& name, string const& id) {
    fstream userFile(userPath + SLASH + name);
    string line;
    while (getline(userFile, line)) {
        if (line == id) break;
    }
    if (line == "") return 0;
    getline(userFile, line); // 持仓数
    userFile.close();
    return atoi(line.data());
}

void Trading::updateHave (string const& name, string const& id, int diff_num) {
    if (!haveStock(name, id)) {
        ofstream out (userPath + SLASH + name, std::ios::app);
        out << id << endl;
        out << diff_num << endl;
        out << 0 << endl; // 暂时写一个 cost
        out.close();
        return;
    }
    fstream userFile(userPath + SLASH + name);
    vector<string> temp;
    temp.clear();
    string line;
    while (getline(userFile, line)) {
        if (line == id) break;
        temp.push_back(line);
    } // 暂存前面的内容
    getline(userFile, line); // 持仓
    int have = atoi(line.data());
    vector<string> temp2;
    temp2.clear();
    while (getline(userFile, line)) {
        temp2.push_back(line);
    } // 暂存后面的内容
    userFile.close();
    ofstream out(userPath + SLASH + name);
    int sz = temp.size();
    for (int i = 0; i < sz; i++) {
        out << temp[i] << endl;
    }
    out << id << endl;
    out << have + diff_num << endl;
    int sz2 = temp2.size();
    for (int i = 0; i < sz2; i++) {
        out << temp2[i] << endl;
    }
    out.close();
}

double Trading::getCost (string const& name, string const& id) {
    fstream userFile(userPath + SLASH + name);
    string line;
    while (getline(userFile, line)) {
        if (line == id) break;
    }
    if (line == "") return 0;
    getline(userFile, line); // 持仓数
    getline(userFile, line); // cost
    userFile.close();
    return atof(line.data());
}

void Trading::updateCost (string const& name, string const& id, double new_cost) {
    if (!haveStock(name, id)) 
        return;
    fstream userFile(userPath + SLASH + name);
    vector<string> temp;
    temp.clear();
    string line;
    while (getline(userFile, line)) {
        temp.push_back(line);
        if (line == id) break;
    } // 暂存前面的内容
    getline(userFile, line); // 持仓
    temp.push_back(line);
    getline(userFile, line); // cost
    vector<string> temp2;
    temp2.clear();
    while (getline(userFile, line)) {
        temp2.push_back(line);
    } // 暂存后面的内容
    userFile.close();
    ofstream out(userPath + SLASH + name);
    int sz = temp.size();
    for (int i = 0; i < sz; i++) {
        out << temp[i] << endl;
    }
    out << new_cost << endl;
    int sz2 = temp2.size();
    for (int i = 0; i < sz2; i++) {
        out << temp2[i] << endl;
    }
    out.close();
}

void Trading::updatePrice (string const& id, double new_price) {
    fstream stockFile(stockPath + SLASH + id);
    string line;
    getline(stockFile, line); // password
    string password = line; // 暂存 password
    getline(stockFile, line); // price
    vector<string> temp;
    temp.clear();
    while (getline(stockFile, line)) {
        temp.push_back(line);
    } // 暂存后面的内容
    stockFile.close();
    ofstream out(stockPath + SLASH + id);
    out << password << endl;
    out << new_price << endl; // 新股价
    int sz = temp.size();
    for (int i = 0; i < sz; i++) {
        out << temp[i] << endl;
    }
    out.close();
}

void Trading::updateFloats_available(string const& id, int diff_floats) {
    fstream stockFile(stockPath + SLASH + id);
    string line;
    getline(stockFile, line); // password
    string password = line; // 暂存 password
    getline(stockFile, line); // price
    string price = line; // 暂存 price
    getline(stockFile, line); // floats_available
    int floats = atoi(line.data());
    vector<string> temp;
    temp.clear();
    while (getline(stockFile, line)) {
        temp.push_back(line);
    } // 暂存后面的内容
    stockFile.close();
    ofstream out(stockPath + SLASH + id);
    out << password << endl;
    out << price << endl; 
    out << floats + diff_floats << endl;
    int sz = temp.size();
    for (int i = 0; i < sz; i++) {
        out << temp[i] << endl;
    }
    out.close();
}

void Trading::deleteId (string const& name, string const& id) {
    if (!haveStock(name, id)) return;
    fstream userFile (userPath + SLASH + name);
    vector<string> temp;
    temp.clear();
    string line;
    while (getline(userFile, line)) {
        if (line == id) break;
        temp.push_back(line);
    } // 暂存前面的内容
    getline(userFile, line); // num_floats
    getline(userFile, line); // cost
    while (getline(userFile, line)) {
        temp.push_back(line);
    }
    userFile.close();
    ofstream out (userPath + SLASH + name);
    int sz = temp.size();
    for (int i = 0; i < sz; i++) {
        out << temp[i] << endl;
    }
    out.close();
}

// public 函数

bool Trading::isEmpty (string const& id) {
    return tradingPool[id].buysInfo.size() == 1 && tradingPool[id].sellsInfo.size() == 1;
}

void Trading::changeFloats (string const& id, double old_price, int old_floats_available, int old_floats, int new_floats) {
    double new_price = old_price * old_floats / new_floats;
    updatePrice(id, new_price);
    int new_floats_available = old_floats_available * new_floats / old_floats;
    updateFloats_available(id, new_floats_available - old_floats_available);
    DIR* dir = opendir(userPath.data());
    struct dirent *pid;
    while ((pid = readdir(dir)) != NULL) {
        if (pid->d_name[0] == '.') continue;
        if (!haveStock(pid->d_name, id)) continue;
        int old_num = getHave(pid->d_name, id);
        int new_num = old_num * new_floats / old_floats;
        updateHave(pid->d_name, id, new_num - old_num);
        double old_cost = getCost(pid->d_name, id);
        double new_cost = old_num * old_cost / new_num;
        updateCost(pid->d_name, id, new_cost);
    }
}

bool Trading::addBuy (string const& name, string const& id, int num, double cost) {
    double totalCost = num * cost;
    // 查看用户账户，判断可用资金是否充足
    double available = getAvaliable(name);
    if (totalCost > available) return false;
    // 以下能够交易 / 入队
    // 可用资金减少
    updateAvailable(name, -totalCost); 
    // 入队
    struct Buy bid(cost, num, name);
    tradingPool[id].buysInfo.push_back(bid); 
    std::push_heap(begin(tradingPool[id].buysInfo), end(tradingPool[id].buysInfo), std::greater<struct Buy>()); // 排队
    trading(id);
    return true;
}

bool Trading::addSell(string const& name, string const& id, int num, double new_price) {
    // 查看持仓是否充足
    int have = getHave(name, id);
    if (have < num) return false;
    // 以下能够交易 / 入队
    // 减少持仓
    updateHave (name, id, -num); 
    // 入队
    struct Sell bid (new_price, num, name);
    tradingPool[id].sellsInfo.push_back(bid); 
    std::push_heap(std::begin(tradingPool[id].sellsInfo), std::end(tradingPool[id].sellsInfo)); // 排队
    trading(id);
    return true;
}

void Trading::trading (string const& id) {
    struct Buy buyBid = tradingPool[id].buysInfo.back(); // 取尾端，最高买价
    struct Sell sellBid = tradingPool[id].sellsInfo.back(); // 取尾端，最低卖价
    double buyCost = buyBid.price * buyBid.num_of_shares; // 原先从买家账户中减掉的可用资金
    int sellNum = sellBid.num_of_shares; // 原先从卖家账户中减掉的持仓
    if (buyBid.price < sellBid.price) return; // 交易失败
    // 以下交易成功
    double new_price = (buyBid.price + sellBid.price) / 2; // 新股价
    int bid_num = std::min(buyBid.num_of_shares, sellBid.num_of_shares); // 成交股数
    // 更新双方账户：持仓、成本、可用
    // 买家
    double cost = getCost(buyBid.userName, id);
    int have = getHave(buyBid.userName, id);
    double new_cost = (cost * have + bid_num * buyBid.price) / (have + bid_num);
    updateHave(buyBid.userName, id, bid_num);
    updateCost (buyBid.userName, id, new_cost);
    // 卖家
    if (sellBid.userName != "") { // 是用户而不是股票
        updateHave(sellBid.userName, id, sellNum - bid_num); // 更新卖家持仓
        if (getHave(sellBid.userName, id) == 0) deleteId (sellBid.userName, id);
        updateAvailable (sellBid.userName, bid_num * sellBid.price); // 更新卖家可用资金
    }
    else { // 直接从股票中购买
        updateFloats_available(id, -bid_num);
    }
    // 更新股票信息：股价 （floats_available）
    updatePrice(id, new_price);
    // 更新挂牌股数，判断是否要出队
    tradingPool[id].buysInfo.back().num_of_shares -= bid_num;
    tradingPool[id].sellsInfo.back().num_of_shares -= bid_num;
    if (tradingPool[id].buysInfo.back().num_of_shares == 0) tradingPool[id].buysInfo.pop_back();
    if (tradingPool[id].sellsInfo.back().num_of_shares == 0) tradingPool[id].sellsInfo.pop_back();
}

void Trading::reset() {
    map<string, struct Bids>::iterator iter;
    for (iter = tradingPool.begin(); iter != tradingPool.end(); iter++) {
        vector<struct Buy>::iterator buyIter;
        vector<struct Sell>::iterator sellIter;
        for (buyIter = iter->second.buysInfo.begin(); buyIter != iter->second.buysInfo.end(); buyIter++) {
            updateAvailable(buyIter->userName, buyIter->price * buyIter->num_of_shares);
        }
        for (sellIter = iter->second.sellsInfo.begin(); sellIter != iter->second.sellsInfo.end(); sellIter++) {
            if (sellIter->userName != "")
                updateHave(sellIter->userName, iter->first, sellIter->num_of_shares);
        }
    }
}




#endif // TRADING_H