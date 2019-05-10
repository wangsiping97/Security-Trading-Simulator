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

void Shell_Base::getPassword(string &password) {
    int i = 0;
    int c;

    /*saving the old settings of STDIN_FILENO and copy settings for resetting*/
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;

    /*setting the approriate bit in the termios struct*/
    newt.c_lflag &= ~(ECHO);          

    /*setting the new bits*/
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*reading the password from the console*/
    getline(std::cin, password);

    /*resetting our old STDIN_FILENO*/ 
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}