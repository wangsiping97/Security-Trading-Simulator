#ifndef _TRADING_H
#define _TRADING_H

#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "config.cpp"

using std::ofstream;
using std::ifstream;
using std::endl;

class Trading {
    friend class User;
    friend class Stock;
private: 
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
    static bool addBuy (string const& name, string const& id, int num, double cost);
    static bool addSell (string const& name, string const& id, int num, double cost);
    static bool isEmpty (string const& id);
    static void changeFloats (string const& id, double old_price, int old_floats_available, int old_floats, int new_floats);
    static void setFile ();
    static void readFile ();
    static vector<string> cutout (string const& line);
private: 
    static map<string, struct Bids> tradingPool;
public: 
    static void init();
    static void reset(); // 程序结束运行时，还原 tradingPool 中数据
};

#endif // trading.h