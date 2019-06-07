#pragma once
#include "../kernel/config.h"
#include "shell_base.h"

Shell_Base::Shell_Base (istream& _in, ostream& _out): in(_in), out(_out) {
    vcmd.clear();
}

void Shell_Base::clearScreen() { 
    #if defined _WIN32  
    system("cls");
    #else
    system("clear");
    #endif
}

void Shell_Base::wrong () {
    out << "Invalid command.Please use \"help\" to see our COMMAND LIST. " << endl;
}

void Shell_Base::cutOut (string const& input, const char flag) {
    vcmd.clear();
    std::istringstream iss(input);
    string temp;
    while (getline(iss, temp, flag)) 
        vcmd.push_back(temp);
}

void Shell_Base::getPassword(string &password) {
    #if defined __APPLE__ || defined __linux__
    struct termios oldt, newt;
    // 将原有设置保存下来并且将原有设置赋值给新的设置
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // 将新的设置设置为输入不回显
    newt.c_lflag &= ~(ECHO);          
    // 设置新的设置
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    // 从键盘输入密码
    getline(in, password);
    // 设置回原有设置
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #else
    getline(in, password);
    #endif
}