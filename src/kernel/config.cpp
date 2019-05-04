#include "config.h"

using std::fstream;

// struct Buy
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

// Struct Sell
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

// struct Bids
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