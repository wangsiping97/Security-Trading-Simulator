#include <string>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
// #include "user.cpp"
// #include "stock.cpp"
// #include "../shell/user_shell.cpp"
// #include "../shell/stock_shell.cpp"

#ifdef _WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::endl;

string thispath = getcwd(NULL, 0);

class Logger {
protected: 
    static string path;
    string type;
    string userName;
    string userPath;
public: 
    Logger (string const& _type, string const& _userName);
    bool exist();
    void reg(string const& _password);
    bool login (string const& passward);
    // User* getNewUser();
    // Stock* getNewStock();
};

string Logger::path = thispath.substr(0, thispath.length() - 4) + SLASH + "data";

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
    fout << 200000 << endl;
}

// User* Logger::getNewUser() {
//     return new User (userName);
// }

// Stock* Logger::getNewStock() {
//     return new Stock (userName);
// }

bool Logger::login (string const& password) {
    fstream file(userPath);
    string line;
    getline(file, line);
    if (line != password) {
        return false;
    }
    // if (type == "User") {
    //     User* user = getNewUser();
    //     delete user;
    // }
    // else {
    //     Stock* stock = getNewStock();
    //     delete stock;
    // }
    return true;
}