#include "logger.h"

string Logger::path = thisPath + SLASH + ".." + SLASH + "data";

Logger::Logger(string const& _type, string const& _userName): type(_type), userName(_userName) {
    userPath = path + SLASH + type + SLASH + userName;
}

string Logger::encrypt(string pass, int key) {
    char skey[100];
    sprintf(skey, "%d", key);
    for (int i = 0, j = 0; pass[j]; j++, i = (i + 1) % 7) {
        pass[j] += skey[i] - '0';
        if(pass[j] > 122) pass[j] -= 90;
    }
    return pass;
}

bool Logger::exist() {
    fstream file;
    file.open(userPath);
    if (!file) return false;
    else return true;
}

void Logger::reg(string _password) {
    system(("touch " + userPath).c_str());
    ofstream fout(userPath.c_str());
    string password = encrypt(_password, KEY);
    fout << password << endl;
    fout << 200000 << endl;
}

User* Logger::getNewUser() {
    return new User (userName);
}

Stock* Logger::getNewStock() {
    return new Stock (userName);
}

bool Logger::login (string password) {
    string enpassword = encrypt(password, KEY);
    fstream file(userPath.c_str());
    string line;
    getline(file, line);
    file.close();
    if (line != enpassword) {
        return false;
    }
    if (type == "User") {
        User* user = getNewUser();
        User_Shell shell(user, std::cin, std::cout);
        shell.hello();
        shell.showCommand();
        shell.run();
        delete user;
    }
    else if (type == "Stock") {
        Stock* stock = getNewStock();
        Stock_Shell shell(stock, std::cin, std::cout);
        shell.hello();
        shell.showCommand();
        shell.run();
        delete stock;
    }
    return true;
}