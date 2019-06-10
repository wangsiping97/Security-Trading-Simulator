#include "user_shell.h"

const char User_Shell::HELP[] = 
    "---------------------- COMMAND LIST ----------------------\n"
    "help                                     show command list\n"
    "t+ [SecuCode] [Number of Shares] [price]   long a security\n"
    "t- [SecuCode] [Number of Shares] [price]  short a security\n"
    "account                            see your account status\n"
    "stock                                      show stock list\n"
    "bid [SecuCode]                               show bid-list\n"
    "select ?                        show 'select' instructions\n"
    "select ... from [SecuCode]              look up secu-info\n"
    "clear                                         clear screen\n"
    "quit                                               log out\n"
    "----------------------------------------------------------";

const char User_Shell::INSTRUCTION[] = 
    "---------------------- INSTRUCTIONS ----------------------\n"
    "select * from [SecuCode]                  look up all info\n"
    "select price from [SecuCode]                 look up price\n"
    "select price,roa from [SecuCode]       look up price & roa\n"
    "...                                                       \n"
    "Items available: price, industry, floats, roa, roe        \n"
    "NOTICE! NO BLANK between items, only ',' is valid         \n"
    "----------------------------------------------------------";

User_Shell::User_Shell (User* _user, istream& _in, ostream& _out): user(_user), Shell_Base(_in, _out) {}

void User_Shell::hello() {
    clearScreen();
    out << "Welcome, " << user->name << "!" << endl;
}

void User_Shell::showCommand () {
    out << HELP << endl;
}

void User_Shell::showStockList () {
    out << endl;
    #if defined _WIN32
    DIR* dir = opendir(stockPath.data());
    struct dirent *pid;
    while ((pid = readdir(dir)) != NULL) {
        if (pid->d_name[0] == '.') continue;
        out << pid->d_name << endl;
    }
    #else
    system(("cd " + stockPath + " && ls").c_str());
    #endif
    out << endl;
}

void User_Shell::wrongStock(string const& id) {
    out << id << " is not in the stock list. Select a stock from below: " << endl;
    showStockList();
}

bool User_Shell::match(string const& a, string const& b) { // 模糊匹配
    int f[21][21],i,j,n,m;
    for (i = 0; i < 20; ++i) {
        for (j = 0; j < 20; ++j) 
            f[i][j] = 31; // 初始化，将所有的成本都赋值为一个很大的数
    }
    f[0][0] = 0;
    n = a.length();
    m = b.length();
    if (n >= 20 || m >= 20) return false;
    for (i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i < n && j < m) { // a[i + 1] 和 b[j + 1] 匹配
                if (a[i] == b[j]) f[i + 1][j + 1] = std::min(f[i + 1][j + 1], f[i][j]); // 不需要任何修改
                else f[i + 1][j + 1] = std::min(f[i + 1][j + 1], f[i][j] + 1); // 需要修改，强行匹配
            }
            if (i < n) f[i + 1][j] = std::min(f[i + 1][j], f[i][j] + 1); // 删掉 a[i]
            if (j < m) f[i][j + 1] = std::min(f[i][j + 1], f[i][j] + 1); // 删掉 b[j] 
        }
    }
    return f[n][m] <= 2;
}

bool User_Shell::parseCommand(string& command) {
    command.erase(command.find_last_not_of(" ") + 1); // 去掉尾端多余空格
    if (command == "") return true;
    if (command == "quit") {
        clearScreen();
        out << "Successfully quitted " << user->name << ". " << endl;
        return false;
    }
    if (command == "help") {
        showCommand();
        return true;
    }
    if (command == "clear") {
        clearScreen();
        return true;
    }
    if (command == "stock") {
        showStockList ();
        return true;
    }
    if (command == "account") {
        struct Account account = user->getAccount();
        out << endl;
        out << setw(15) << user->name << setw(15) << "Amount" << setw(10) << "Holding" << setw(10) << "Cost" << setw(10) << "Current" << setw(10) << "Yield(%)" << endl;
        if (!account.asset.empty()) {
            map<string, struct Value>::iterator iter;
            for (iter = account.asset.begin(); iter != account.asset.end(); iter++) {
                out << setw(15) << iter->first << setw(15);
                out << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << iter->second.price * iter->second.numFloats << setw(10);
                out << iter->second.numFloats << setw(10);
                out << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << iter->second.cost << setw(10);
                out << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << iter->second.price << setw(10);
                out << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << iter->second.yield << endl;
            }
        }
        out << setw(15) << "Available" << setw(15) << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << account.available << endl;
        out << setw(15) << "Total asset" << setw(15) << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << account.total;
        out << setw(10) << " " << setw(10) << " " << setw(10) << " " << setw(10) << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << (account.total / 200000 - 1) * 100;
        out << endl;
        out << endl;
        return true;
    }
    if (command == "select ?") {
        out << INSTRUCTION << endl;
        return true;
    }
    cutOut (command, ' ');
    if (vcmd.size() == 2 && vcmd[0] == "bid") {
        string id = vcmd[1];
        string price = user->search("price", id);
        if (price == "") {
            wrongStock (id);
            return true;
        }
        struct Bids bids= user->bidList(id);
        out << endl;
        out << setw(15) << "Order" << setw(10) << "Shares" << setw(10) << "Price" << endl;
        out << "[LONG]" << endl;
        for (int i = (int)bids.buysInfo.size() - 1; i >= 1; i--){
            out << setw(15) << (int)bids.buysInfo.size() - i << setw(10) << bids.buysInfo[i].num_of_shares << setw(10) << std::setiosflags(std::ios::fixed) << std::setprecision(2) << bids.buysInfo[i].price <<endl;
        }
        out << endl;
        out << "[SHORT]" << endl;
        for (int j = (int)bids.sellsInfo.size() - 1; j >= 1; j--){
            out << setw(15) << (int)bids.sellsInfo.size() - j << setw(10) << bids.sellsInfo[j].num_of_shares << setw(10) << std::setiosflags(std::ios::fixed)<<std::setprecision(2) << bids.sellsInfo[j].price << endl;
        }
        out << endl;
        return true;
    }
    if (vcmd.size() == 1 || vcmd.size() == 2 || vcmd.size() == 3 || vcmd.size() > 4) {
        wrong ();
        return true;
    }
    string cmd = vcmd[0];
    if (cmd == "t+") {
        string id = vcmd[1];
        string price = user->search("price", id);
        if (price == "") {
            wrongStock (id);
            return true;
        }
        int numBid = atoi(vcmd[2].data());
        if (numBid <= 0) {
            out << "Invalid number of shares." << endl;
            return true;
        }
        double bidPrice = atof(vcmd[3].data());
        if (bidPrice <= 0) {
            out << "Invalid bid price." << endl;
            return true;
        }
        if (!(user->addBuy(id, numBid, bidPrice))) {
            out << "可用资金不足！" << endl;
            return true;
        }
        out << "Request received!" << endl;
    }
    else if (cmd == "t-") {
        string id = vcmd[1];
        string price = user->search("price", id);
        if (price == "") {
            wrongStock (id);
            return true;
        }
        int numBid = atoi(vcmd[2].data());
        if (numBid <= 0) {
            out << "Invalid number of shares." << endl;
            return true;
        }
        double bidPrice = atof(vcmd[3].data());
        if (bidPrice <= 0) {
            out << "Invalid bid price." << endl;
            return true;
        }
        if (!(user->addSell(id, numBid, bidPrice))) {
            out << "持仓不足！" << endl;
            return true;
        }
        out << "Request received!" << endl;
    }
    else if (cmd == "select") {
        string id = vcmd[3];
        string from = vcmd[2];
        if (from != "from") {
            out << "Invalid command.Please refer to our INSTRUCTIONS below: " << endl;
            out << INSTRUCTION << endl;
        }
        else {
            string price = user->search("price", id);
            if (price == "") {
                wrongStock (id);
                return true;
            }
            string industry = user->search("industry", id);
            string floats = user->search("floats", id);
            string roa = user->search("roa", id);
            string roe = user->search("roe", id);
            string item = vcmd[1];
            if (item == "*") {
                out << endl;
                out << "SecuCode\t" <<  id << endl;
                out << "Price\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(price.data()) << endl;
                out << "Industry\t" << industry << endl;
                out << "Floats\t\t" << floats << endl;
                out << "ROA(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(roa.data()) << endl;
                out << "ROE(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(roe.data()) << endl << endl;
            }
            else {
                cutOut(item, ',');
                vector<string>::iterator iter;
                vector<string> vitem, vres;
                vitem.clear(); vres.clear();
                for (iter = vcmd.begin(); iter != vcmd.end(); iter++) {
                    if (match(*iter, "price") || match(*iter, "industry") || match(*iter, "floats") || *iter == "roa" || *iter == "roe") {
                        vitem.push_back(*iter);
                        vres.push_back(user->search(*iter, id));
                    }
                    else {
                        out << "Wrong item. Try select another item, or use \"select * from ...\" to look up all the information." << endl;
                        return true;
                    }
                }
                out << endl;
                out << "SecuCode\t" << id << endl;
                int s = vitem.size();
                for (int i = 0; i < s; i++) {
                    if (match(vitem[i], "price")) out << "Price\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(price.data()) << endl;
                    else if (match(vitem[i], "industry")) out << "Industry\t" << industry << endl;
                    else if (match(vitem[i], "floats")) out << "Floats\t\t" << floats << endl;
                    else if (vitem[i] == "roa") out << "ROA(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(roa.data()) << endl;
                    else out << "ROE(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(roe.data()) << endl;
                }
                out << endl;
            }
        }
    }
    else wrong();
    return true;
}

void User_Shell::run () {
    string command;
    do {
        out << "STS-I:~"<< user->name << "$ ";
        getline (in, command);
    } while (parseCommand(command));
}