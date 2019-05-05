#pragma once
#include "../kernel/config.h"
#include "shell_base.h"

Shell_Base::Shell_Base (istream& _in, ostream& _out): in(_in), out(_out) {
    vcmd.clear();
}

void Shell_Base::clearScreen() { 
    system("clear");
}

void Shell_Base::wrong () {
    out << "Invalid command.Please use \"help\" to see our COMMAND LIST. " << endl;
}

void Shell_Base::showStockList () {
    out << endl;
    system(("cd " + stockPath + " && ls").c_str());
    out << endl;
}

void Shell_Base::cutOut (string const& input, const char flag) {
    vcmd.clear();
    std::istringstream iss(input);
    string temp;
    while (getline(iss, temp, flag)) 
        vcmd.push_back(temp);
}