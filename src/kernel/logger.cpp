#include "logger.h"

string Logger::path = thispath + SLASH + ".." + SLASH + "data";

Logger::Logger(string const& _type, string const& _userName): type(_type), userName(_userName) {
    userPath = path + SLASH + type + SLASH + userName;
}

bool Logger::exist() {
    fstream file;
    file.open(userPath);
    if (!file) return false;
    else return true;
}

void Logger::reg(string const& _password) {
    system(("touch " + userPath).c_str());
    ofstream fout(userPath);
    fout << _password << endl;
    fout << 200000 << endl;
}

User* Logger::getNewUser() {
    return new User (userName);
}

Stock* Logger::getNewStock() {
    return new Stock (userName);
}

bool Logger::login (string const& password) {
    fstream file(userPath);
    string line;
    getline(file, line);
    file.close();
    if (line != password) {
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