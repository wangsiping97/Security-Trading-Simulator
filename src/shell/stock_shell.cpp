#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "../kernel/stock.cpp"

using std::string;
using std::vector;
using std::istream;
using std::ostream;
using std::endl;

class Stock_Shell {
private: 
    Stock* stock;
    istream& in;
    ostream& out;
    static const char HELP[];
    vector<string> vcmd;
private: 
    void cutOut (string const& input); 
    bool parseCommand (string const& command);
public: 
    Stock_Shell (Stock* _stock, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
    void wrong ();
    void run ();
}; 

const char Stock_Shell::HELP[] = 
    "---------------------- COMMAND LIST ----------------------\n"
    "help                                     show command list\n"
    "me                                      see your secu-info\n"
    "floats [new num of floats]                    reset floats\n"
    "industry [new industry]                     reset industry\n"
    "roa [new roa]                                    reset roa\n"
    "roe [new roe]                                    reset roe\n"
    "clear                                         clear screen\n"
    "quit                                               log out\n"
    "----------------------------------------------------------";

Stock_Shell::Stock_Shell (Stock* _stock, istream& _in, ostream& _out): stock(_stock), in(_in), out(_out) {}

void Stock_Shell::hello() {
    system("clear");
    out << "Welcome, " << stock->id << "!" << endl;
}

void Stock_Shell::showCommand () {
    out << HELP << endl;
}

void Stock_Shell::wrong () {
    out << "Invalid command.Please refer to our COMMANDLIST below: " << endl;
    showCommand();
}

void Stock_Shell::cutOut (string const& input) {
    vcmd.clear();
    std::istringstream strcin(input);
    string s;
    while(strcin >> s) vcmd.push_back(s);
}

bool Stock_Shell::parseCommand(string const& command) {
    if (command == "") return true;
    if (command == "quit") {
        system("clear");
        return false;
    }
    if (command == "help") {
        showCommand();
        return true;
    }
    if (command == "me") {
        struct Info target = stock->getInfo();
        out << endl;
        out << "SecuCode\t" <<  stock->id << endl;
        out << "Price\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< target.price << endl;
        out << "Industry\t" << target.industry << endl;
        out << "Floats\t\t" << target.floats << endl;
        out << "ROA(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< target.roa << endl;
        out << "ROE(%)\t\t" << std::setiosflags(std::ios::fixed)<<std::setprecision(2)<< target.roe << endl << endl;
        return true;
    }
    if (command == "clear") {
        system("clear");
        return true;
    }
    cutOut (command);
    if (vcmd.size() == 1 || vcmd.size() > 2) {
        wrong ();
        return true;
    }
    string cmd = vcmd[0], info = vcmd[1];
    if (cmd == "floats" || cmd == "industry" || cmd == "roa" || cmd == "roe") {
        stock->setInfo(cmd, info);
    }
    else wrong ();
    return true;
}

void Stock_Shell::run () {
    string command;
    getline (in, command);
    while (parseCommand(command)) {
        out << "STS-S:~"<< stock->id << "$ ";
        getline (in, command);
    }
}