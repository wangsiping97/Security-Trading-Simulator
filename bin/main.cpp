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
    std::string ans;
    do {
        std::cout << "Welcome to STS! Are you an investor (I) or a security manager (S)? \nI/S? ";
        std::cin >> ans;
    } while (test(ans));
}

int main() {
    system("clear");
    Trading::init();
    loop();
    return 0;
}