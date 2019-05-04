#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "../kernel/user.cpp"

using std::string;
using std::vector;
using std::istream;
using std::ostream;
using std::endl;
using std::setw;

class User_Shell {
private: 
    User* user;
    istream& in;
    ostream& out;
    static const char HELP[];
    static const char INSTRUCTION[];
    vector<string> vcmd;
private: 
    void cutOut (string const& input, const char flag); 
    bool parseCommand (string& command);
    void wrong ();
    void wrongStock (string const& id);
    void showStockList();
public: 
    User_Shell (User* _user, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
    void run ();
}; 

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
    "Items available: price, industry, floats, roa, roa        \n"
    "NOTICE! NO BLANK between items, only ',' is valid         \n"
    "----------------------------------------------------------";

User_Shell::User_Shell (User* _user, istream& _in, ostream& _out): user(_user), in(_in), out(_out) {}

void User_Shell::hello() {
    system("clear");
    out << "Welcome, " << user->name << "!" << endl;
}

void User_Shell::showCommand () {
    out << HELP << endl;
}

void User_Shell::showStockList () {
    out << endl;
    system(("cd " + stockPath + " && ls").c_str());
    out << endl;
}

void User_Shell::wrong () {
    out << "Invalid command.Please refer to our COMMAND LIST below: " << endl;
    showCommand();
}

void User_Shell::wrongStock(string const& id) {
    out << id << " is not in the stock list. Select a stock from below: " << endl;
    showStockList();
}

void User_Shell::cutOut (string const& input, const char flag) {
    vcmd.clear();
    std::istringstream iss(input);
    string temp;
    while (getline(iss, temp, flag)) 
        vcmd.push_back(temp);
}

bool User_Shell::parseCommand(string& command) {
    command.erase(command.find_last_not_of(" ") + 1); // 去掉尾端多余空格
    if (command == "") return true;
    if (command == "quit") {
        system("clear");
        out << "Successfully quitted " << user->name << ". " << endl;
        return false;
    }
    if (command == "help") {
        showCommand();
        return true;
    }
    if (command == "clear") {
        system("clear");
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
        out << endl;
        out << "[BUYS]" << endl;
        vector<struct Buy>::iterator iter;
        for (iter = Trading::tradingPool[id].buysInfo.begin(); iter != --Trading::tradingPool[id].buysInfo.end(); iter++) {
            out << setw(15) << iter->userName << setw(10) << iter->num_of_shares << setw(10) << iter->price << endl;
        }
        out << endl;
        out << "[SALES]" << endl;
        vector<struct Sell>::iterator iter2;
        for (iter2 = Trading::tradingPool[id].sellsInfo.begin(); iter2 != Trading::tradingPool[id].sellsInfo.end(); iter2++) {
            out << setw(15) << iter2->userName << setw(10) << iter2->num_of_shares << setw(10) << iter2->price << endl;
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
        if (!(Trading::addBuy(user->name, id, numBid, bidPrice))) {
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
        if (!(Trading::addSell(user->name, id, numBid, bidPrice))) {
            out << "持仓不足！" << endl;
            return true;
        }
        out << "Request received!" << endl;
    }
    else if (cmd == "select") {
        string id = vcmd[3];
        string from = vcmd[2];
        if (from != "from") {
            out << "Invalid command.Please refer to our COMMANDLIST below: " << endl;
            out << INSTRUCTION << endl;
        }
        else {
            string price = user->search("price", id);
            if (price == "") wrongStock (id);
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
                    if (*iter == "price" || *iter == "industry" || *iter == "floats" || *iter == "roa" || *iter == "roa") {
                        vitem.push_back(*iter);
                        vres.push_back(user->search(*iter, id));
                    }
                    else {
                        out << "Wrong item. Try select another item, or use \"select * from ...\" to look up all the information." << endl;
                        return true;
                    }
                }
                out << endl;
                out << "SecuCode\t" <<  id << endl;
                int s = vitem.size();
                for (int i = 0; i < s; i++) {
                    if (vitem[i] == "price") out << "Price\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(price.data()) << endl;
                    else if (vitem[i] == "industry") out << "Industry\t" << industry << endl;
                    else if (vitem[i] == "floats") out << "Floats\t\t" << floats << endl;
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
    getline (in, command);
    while (parseCommand(command)) {
        out << "STS-I:~"<< user->name << "$ ";
        getline (in, command);
    }
}