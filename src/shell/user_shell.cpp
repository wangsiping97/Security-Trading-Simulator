#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "../kernel/user.cpp"

using std::string;
using std::vector;
using std::istream;
using std::ostream;
using std::endl;

class User_Shell {
private: 
    User* user;
    istream& in;
    ostream& out;
    static const char HELP[];
    vector<string> vcmd;
private: 
    void cutOut (string const& input); 
    bool parseCommand (string const& command);
public: 
    User_Shell (User* _user, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
    void showStockList();
    void wrong ();
    void run ();
}; 

const char User_Shell::HELP[] = 
    "---------------------- COMMAND LIST ----------------------\n"
    "help                                     show command list\n"
    "t+ [SecuCode] [Number of Shares] [price]   long a security\n"
    "t- [SecuCode] [Number of Shares] [price]  short a security\n"
    "account                            see your account status\n"
    "stock                                      show stock list\n"
    "select ... from [secu-code]              look up secu-info\n"
    "clear                                         clear screen\n"
    "quit                                               log out\n"
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
    system(("cd " + user->stockPath + " && ls").c_str());
    out << endl;
}

void User_Shell::wrong () {
    out << "Invalid command.Please refer to our COMMANDLIST below: " << endl;
    showCommand();
}

void User_Shell::cutOut (string const& input) {
    vcmd.clear();
    std::istringstream strcin(input);
    string s;
    while(strcin >> s) vcmd.push_back(s);
}

bool User_Shell::parseCommand(string const& command) {
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
        out << "Unimplemented :(" << endl;
        return true;
    }
    cutOut (command);
    if (vcmd.size() == 1 || vcmd.size() > 4) {
        wrong ();
        return true;
    }
    string cmd = vcmd[0];
    if (cmd == "t+") {
        out << "Unimplemented :(" << endl;
    }
    else if (cmd == "t-") {
        out << "Unimplemented :(" << endl;
    }
    else if (cmd == "select") {
        string from = vcmd[2];
        if (from != "from") wrong();
        else {
            string price = user->search("price", vcmd[3]);
            if (price == "") {
                out << vcmd[3] << " is not in the stock list. Select a stock from below: " << endl;
                showStockList();
                return true;
            }
            string industry = user->search("industry", vcmd[3]);
            string floats = user->search("floats", vcmd[3]);
            string roa = user->search("roa", vcmd[3]);
            string roe = user->search("roe", vcmd[3]);
            string item = vcmd[1];
            if (item == "*") {
                out << endl;
                out << "SecuCode\t" <<  vcmd[3] << endl;
                out << "Price\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(price.data()) << endl;
                out << "Industry\t" << industry << endl;
                out << "Floats\t\t" << floats << endl;
                out << "ROA(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(roa.data()) << endl;
                out << "ROE(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< atof(roe.data()) << endl << endl;
            }
            else {
                out << "Unimplemented :(" << endl;
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