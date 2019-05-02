#include <sstream>
#include <string>
#include <vector>
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
private: 
    bool parseCommand (string const& command);
public: 
    Stock_Shell (Stock* _stock, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
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
    if (command == "clear") {
        system("clear");
        return true;
    }
    else {
        out << "Invalid command/Unimplemented.Please refer to our CommandList below: " << endl;
        showCommand();
    }
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