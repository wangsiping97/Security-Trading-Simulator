#include "global.h"

void Global::init() {
    #if defined _WIN32  
    system("cls");
    #else
    system("clear");
    #endif
    std::cout << string(5, '\n');
    std::cout << "                         Security Trading Simulator                        \n";
    std::cout << "\n                                   (STS)                        \n";
    std::cout << string(3, '\n');
    std::cout << "                            by Siping Wang @ THU                        \n";
    std::cout << string(5, '\n');
    ofstream out ((dataPath + SLASH + "tradingpool").c_str());
    out.close(); // 重置 tradingpool 文件
    Trading::init();
}

bool Global::test (string const& _ans) {
    if (_ans == "I") {
        #if defined _WIN32  
        system("cls");
        #else
        system("clear");
        #endif
        IShell ishell(std::cin, std::cout);
        ishell.showCommand();
        ishell.run();
    }
    else if (_ans == "S") {
        #if defined _WIN32  
        system("cls");
        #else
        system("clear");
        #endif
        SShell sshell(std::cin, std::cout);
        sshell.showCommand();
        sshell.run();
    }
    else if (_ans == "X") {
        #if defined _WIN32  
        system("cls");
        #else
        system("clear");
        #endif
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