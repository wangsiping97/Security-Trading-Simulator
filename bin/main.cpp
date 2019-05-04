#include <iostream>
#include "../src/shell/root_shell.cpp"

bool test (std::string _ans) {
    if (_ans == "I") {
        IShell ishell(std::cin, std::cout);
        ishell.showCommand();
        ishell.run();
    }
    else if (_ans == "S") {
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

void loop() {
    Trading::init();
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    std::string ans;
    do {
        std::cout << "Welcome to STS! Are you an investor (I) or a security manager (S)? \nI/S? ";
        std::cin >> ans;
    } while (test(ans));
}

int main() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    std::cout << string(9, '\n');
    std::cout << "                        Security Trading Simulator                        \n";
    std::cout << string(3, '\n');
    std::cout << "                            by Siping Wang @ THU                           \n";
    loop();
    return 0;
}