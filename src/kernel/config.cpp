#include "config.h"

// struct Buy
Buy::Buy (double _price, int _num_of_shares): price(_price), num_of_shares(_num_of_shares) {
    userName = "";
    time = "0";
}

Buy::Buy (double _price, int _num_of_shares, string _userName): price(_price), num_of_shares(_num_of_shares), userName(_userName) {
    time = getCurrentTime();
}

bool Buy::operator < (const struct Buy& right) const {
    if (price != right.price) return price < right.price; // 相等时，先来的 > 后来的
    else return time > right.time;
}

bool Buy::operator > (const struct Buy& right) const {
    if (price != right.price) return price > right.price;
    else return time < right.time; // 相等时，先来的 > 后来的
}

// struct Sell
Sell::Sell (double _price, int _num_of_shares): price(_price), num_of_shares(_num_of_shares) {
    userName = "";
    time = "0";
}

Sell::Sell (double _price, int _num_of_shares, string _userName): price(_price), num_of_shares(_num_of_shares), userName(_userName) {
    time = getCurrentTime();
}

bool Sell::operator > (const struct Sell& right) const {
    if (price != right.price) return price > right.price; // 相等时，先来的 < 后来的
    else return time > right.time;
}

bool Sell::operator < (const struct Sell& right) const {
    if (price != right.price) return price < right.price; // 相等时，先来的 < 后来的
    else return time < right.time;
}

// struct Bids
Bids::Bids (string _id): id(_id) {
    buysInfo.clear();
    sellsInfo.clear();
    fstream file ((stockPath + SLASH + id).c_str());
    string line;
    getline(file, line); // password
    getline(file, line); // price
    double openPrice = atof(line.data());
    getline(file, line); // floats_available
    int floats = atoi(line.data());
    struct Sell initSell(openPrice, floats);
    sellsInfo.push_back(initSell);
    std::sort(sellsInfo.begin(), sellsInfo.end(), std::less<struct Sell>()); // 卖堆，降序
    struct Buy initBuy(-1, -1);
    buysInfo.push_back(initBuy);
    std::sort(buysInfo.begin(), buysInfo.end()); // 买堆，默认升序
    file.close();
}