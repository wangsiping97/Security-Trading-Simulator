#include "user.h"

User::User (string const& _name): name(_name) {
    myPath = userPath + SLASH + name;
    myAccount.asset.clear();
    myAccount.available = 0; // 最初情况
    myAccount.total = 0; // 最初情况
}

string User::search (string const& item, string const& id) {
    fstream file((stockPath + SLASH + id).c_str());
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
    fstream file (myPath.c_str());
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
    map<string, struct Bids>::iterator iter;
    for (iter = Trading::tradingPool.begin(); iter != Trading::tradingPool.end(); iter++) {
        vector<struct Buy>::iterator iterBuy;
        for (iterBuy = iter->second.buysInfo.begin(); iterBuy != iter->second.buysInfo.end(); iterBuy++) {
            if (iterBuy->userName == name) 
                myAccount.total += iterBuy->price * iterBuy->num_of_shares;
        }
        vector<struct Sell>::iterator iterSell;
        for (iterSell = iter->second.sellsInfo.begin(); iterSell != iter->second.sellsInfo.end(); iterSell++) {
            if (iterSell->userName == name)
                myAccount.total += myAccount.asset[iter->first].price * iterSell->num_of_shares;
        }
    }
    return myAccount;
}

struct Bids User::bidList(string const& id) {
    Trading::readFile();
    return Trading::tradingPool[id];
}

bool User::addBuy (string const& id, int num, double cost) {
    return Trading::addBuy(name, id, num, cost);
}

bool User::addSell (string const& id, int num, double cost) {
    return Trading::addSell(name, id, num, cost);
}