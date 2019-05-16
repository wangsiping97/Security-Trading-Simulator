#pragma once
#include "trading.h"

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

bool Trading::isEmpty (string const& id) {
    readFile(); // 从文件中读取最新 tradingPool
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
    readFile(); // 从文件中读取最新 tradingPool
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
    std::sort(begin(tradingPool[id].buysInfo), end(tradingPool[id].buysInfo)); // 排队
    while (trading(id)) {
        trading(id);
    }
    return true;
}

bool Trading::addSell(string const& name, string const& id, int num, double new_price) {
    readFile(); // 从文件中读取最新 tradingPool
    // 查看持仓是否充足
    int have = getHave(name, id);
    if (have < num) return false;
    // 以下能够交易 / 入队
    // 减少持仓
    updateHave (name, id, -num); 
    // 入队
    struct Sell bid (new_price, num, name);
    tradingPool[id].sellsInfo.push_back(bid); 
    std::sort(std::begin(tradingPool[id].sellsInfo), std::end(tradingPool[id].sellsInfo), std::less<struct Sell>()); // 排队
    while(trading(id)) {
        trading(id);
    }
    return true;
}

bool Trading::trading (string const& id) {
    if (tradingPool[id].sellsInfo.empty() || tradingPool[id].buysInfo.size() == 1) {
        setFile();
        return false; // 交易失败
    }
    struct Buy buyBid = tradingPool[id].buysInfo.back(); // 取尾端，最高买价
    struct Sell sellBid = tradingPool[id].sellsInfo.back(); // 取尾端，最低卖价
    double buyCost = buyBid.price * buyBid.num_of_shares; // 原先从买家账户中减掉的可用资金
    int sellNum = sellBid.num_of_shares; // 原先从卖家账户中减掉的持仓
    if (buyBid.price < sellBid.price) {
        setFile();
        return false; // 交易失败
    }
    // 以下交易成功
    double new_price = (buyBid.price + sellBid.price) / 2; // 新股价
    int bid_num = std::min(buyBid.num_of_shares, sellBid.num_of_shares); // 成交股数
    // 更新双方账户：持仓、成本、可用
    // 买家
    double cost = getCost(buyBid.userName, id);
    int have = getHave(buyBid.userName, id);
    double new_cost = (cost * have + bid_num * new_price) / (have + bid_num);
    updateHave(buyBid.userName, id, bid_num);
    updateCost (buyBid.userName, id, new_cost);
    updateAvailable (buyBid.userName, bid_num * buyBid.price - bid_num * new_price); // 更新可用
    // 卖家
    if (sellBid.userName != "") { // 是用户而不是股票
        updateHave(sellBid.userName, id, sellNum - bid_num); // 更新卖家持仓
        if (getHave(sellBid.userName, id) == 0) deleteId (sellBid.userName, id);
        updateAvailable (sellBid.userName, bid_num * new_price); // 更新卖家可用资金
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
    setFile(); // 将最新 tradingPool 写入文件
    return true;
}

void Trading::reset() {
    if (tradingPool.empty()) return;
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
    tradingPool.clear();
}

void Trading::setFile() {
    ofstream out(dataPath + SLASH + "tradingpool");
    map<string, struct Bids>::iterator iter;
    for (iter = tradingPool.begin(); iter != tradingPool.end(); iter++) {
        vector<struct Buy>::iterator iterBuy;
        for (iterBuy = iter->second.buysInfo.begin(); iterBuy != iter->second.buysInfo.end(); iterBuy++) {
            if (iterBuy->userName != "") {
                out << iter->first << " Buy " << iterBuy->userName << " " << iterBuy->num_of_shares << " " << iterBuy->price << " " << iterBuy->time << endl;
            }
        }
        vector<struct Sell>::iterator iterSell;
        for (iterSell = iter->second.sellsInfo.begin(); iterSell != iter->second.sellsInfo.end(); iterSell++) {
            if (iterSell->userName != "")
                out << iter->first << " Sell " << iterSell->userName << " " << iterSell->num_of_shares << " " << iterSell->price << " " << iterSell->time << endl;
        }
    }
    out.close();
}

void Trading::readFile() {
    init();
    fstream file(dataPath + SLASH + "tradingpool");
    vector<string> lines;
    lines.clear();
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    vector<string>::iterator iter;
    for (iter = lines.begin(); iter != lines.end(); iter++) {
        vector<string> temp = cutout(*iter);
        if (temp.size() == 0) return;
        if (temp[1] == "Buy") {
            struct Buy buy;
            buy.userName = temp[2];
            buy.num_of_shares = atoi(temp[3].c_str());
            buy.price = atof(temp[4].c_str());
            buy.time = atoi(temp[5].c_str());
            tradingPool[temp[0]].buysInfo.push_back(buy);
            std::sort(begin(tradingPool[temp[0]].buysInfo), end(tradingPool[temp[0]].buysInfo));
        }
        else {
            struct Sell sell;
            sell.userName = temp[2];
            sell.num_of_shares = atoi(temp[3].c_str());
            sell.price = atof(temp[4].c_str());
            sell.time = atof(temp[5].c_str());
            tradingPool[temp[0]].sellsInfo.push_back(sell);
            std::sort(std::begin(tradingPool[temp[0]].sellsInfo), std::end(tradingPool[temp[0]].sellsInfo), std::less<struct Sell>()); 
        }
    }
}

vector<string> Trading::cutout(string const& line) {
    vector<string> temp;
    temp.clear();
    std::istringstream iss(line);
    string ttemp;
    while (getline(iss, ttemp, ' ')) 
        temp.push_back(ttemp);
    return temp;
}