#include "global.h"

void Global::init() {
    system("clear");
    std::cout << string(5, '\n');
    std::cout << "                         Security Trading Simulator                        \n";
    std::cout << "\n                                   (STS)                        \n";
    std::cout << string(3, '\n');
    std::cout << "                            by Siping Wang @ THU                        \n";
    std::cout << string(5, '\n');
    ofstream out (dataPath + SLASH + "tradingpool");
    out.close(); // 重置 tradingpool 文件
    Trading::init();
}

bool Global::test (string const& _ans) {
    if (_ans == "I") {
        system("clear");
        IShell ishell(std::cin, std::cout);
        ishell.showCommand();
        ishell.run();
    }
    else if (_ans == "S") {
        system("clear");
        SShell sshell(std::cin, std::cout);
        sshell.showCommand();
        sshell.run();
    }
    else if (_ans == "X") {
        system("clear");
        Trading::reset();
        return false;
    }
    return true;
}

void Global::run() {
    string ans;
    do {
        std::cout << "Welcome to STS! Are you an investor (I) or a security manager (S)? \nI/S? ";
        std::cin >> ans;
    } while (test(ans));
}