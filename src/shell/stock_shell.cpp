#include "stock_shell.h"

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
    "ATTENTION! Only change floats when there is no bid!       \n"
    "DO RESTART the system after changing the floats!          \n"
    "----------------------------------------------------------";

Stock_Shell::Stock_Shell (Stock* _stock, istream& _in, ostream& _out): stock(_stock), Shell_Base(_in, _out) {}

void Stock_Shell::hello() {
    clearScreen();
    out << "Welcome, " << stock->id << "!" << endl;
}

void Stock_Shell::showCommand () {
    out << HELP << endl;
}

bool Stock_Shell::parseCommand(string& command) {
    command.erase(command.find_last_not_of(" ") + 1); // 去掉尾端多余空格
    if (command == "") return true;
    if (command == "quit") {
        clearScreen();
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
        clearScreen();
        return true;
    }
    cutOut (command, ' ');
    if (vcmd.size() == 1 || vcmd.size() > 2) {
        wrong ();
        return true;
    }
    string cmd = vcmd[0], info = vcmd[1];
    if (cmd == "floats") {
        int new_floats = atoi(info.data());
        if (new_floats <= 0) {
            out << "Invalid settings." << endl;
            return true;
        }
        if (!stock->setInfo(cmd, info)) {
            out << "DO NOT reset floats when there are bids in the trading pool." << endl;
            return true;
        }
        out << "Please RESTART the system." << endl;
    }
    else if (cmd == "industry") {
        stock->setInfo(cmd, info);
    }
    else if (cmd == "roa" || cmd == "roe") {
        double new_info = atof(info.data());
        if (new_info <= 0) {
            out << "Invalid settings." << endl;
            return true;
        }
        stock->setInfo(cmd, info);
    }
    else wrong ();
    return true;
}

void Stock_Shell::run () {
    string command;
    do {
        out << "STS-S:~"<< stock->id << "$ ";
        getline (in, command);
    } while (parseCommand(command));
}