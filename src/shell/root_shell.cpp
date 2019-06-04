#include "root_shell.h"

// IShell
const char IShell::I_HELP[] = 
    "---------------------- COMMAND LIST ----------------------\n"
    "help                                     show command list\n"
    "reg [username]                              register an id\n"
    "log [username]                                      log in\n"
    "clear                                         clear screen\n"
    "reset                            quit investor environment\n"
    "----------------------------------------------------------";

IShell::IShell (istream& _in, ostream& _out): Shell_Base(_in, _out) {}

void IShell::showCommand() {
    out << I_HELP << std::endl;
}

bool IShell::checkPassword (string str1, string str2) {
    if (str1 == str2) return true;
    else {
        out << "Wrong password! Please reset your password." << endl;
        return false;
    }
}

bool IShell::parseCommand (string& command) {
    command.erase(command.find_last_not_of(" ") + 1); // 去掉尾端多余空格
    if (command == "") return true;
    if (command == "reset") return false; // 退出投资者环境
    if (command == "help") {
        showCommand(); // 查看 COMMAND LLIST
        return true;
    }
    if (command == "clear") {
        clearScreen();
        return true;
    }
    cutOut (command, ' ');
    if (vcmd.size() == 1 || vcmd.size() > 2) {
        out << "Invalid command.Please refer to our COMMAND LIST below: " << endl;
        showCommand();
        return true;
    }
    string cmd = vcmd[0];
    if (cmd == "reg") {
        Logger logger("User", vcmd[1]); // 创建用户名为 vcmd[1] 的用户
        if (logger.exist()) // 如果已经被注册过，则报错
            out << "\"" << vcmd[1] << "\" " << "has already been registered, please use another name." << endl;
        else { // 如果新用户名合法，则继续设置密码
            string password, c_password;
            do {
                out << "password: "; 
                getPassword(password); // 输入密码
                out << endl;
                out << "confirm password: "; 
                getPassword(c_password); // 确认密码
                out << endl;
            } while (!checkPassword(password, c_password)); // 若前后密码不匹配，则报错并要求再次输入密码
            logger.reg(password); // 注册账户
            logger.login(password); // 自动登录
        }
    }
    else if (cmd == "log") {
        Logger logger("User", vcmd[1]);
        if (!logger.exist()) // 若用户名不存在，则报错，并返回初始状态
            out << "\"" << vcmd[1] << "\" " << "does not exist. Please check your username or use 'reg' to register." << endl;
        else {
            out << "password: ";
            string password;
            getPassword(password);
            if (logger.login(password) == false) { // 若密码错误，则报错，并返回初始状态
                out << endl << "Log-in failed! Please check your username or password." << endl;
            }
        }
    }
    else {
        out << "Invalid command. Please refer to our COMMANDLIST below: " << endl;
        showCommand();
    }
    return true;
}

void IShell::run () {
    string command;
    getline (in, command);
    while (parseCommand(command)) {
        out << "STS-I:~ $ ";
        getline (in, command);
    }
}

// SShell
const char SShell::S_HELP[] = 
    "---------------------- COMMAND LIST ----------------------\n"
    "help                                     show command list\n"
    "log [SecuCode]                                      log in\n"
    "clear                                         clear screen\n"
    "reset                                quit secu environment\n"
    "----------------------------------------------------------";

SShell::SShell (istream& _in, ostream& _out): Shell_Base(_in, _out) {}

void SShell::showCommand () {
    out << S_HELP << std::endl;
}

bool SShell::parseCommand (string& command) {
    command.erase(command.find_last_not_of(" ") + 1); // 去掉尾端多余空格
    if (command == "") return true;
    if (command == "reset") return false; // 退出证券环境
    if (command == "help") {
        showCommand(); // 查看 COMMAND LLIST
        return true;
    }
    if (command == "clear") {
        clearScreen();
        return true;
    }
    cutOut(command, ' ');
    if (vcmd.size() == 1 || vcmd.size() > 2) {
        out << "Invalid command.Please refer to our COMMANDLIST below: " << endl;
        showCommand();
        return true;
    }
    string cmd = vcmd[0];
    if (cmd == "log") {
        Logger logger("Stock", vcmd[1]);
        if (!logger.exist()) // 若用户名不存在，则报错，并返回初始状态
            out << "\"" << vcmd[1] << "\" " << "does not exist. Please check your SecuCode." << endl;
        else {
            out << "password: ";
            string password;
            getPassword(password);
            if (logger.login(password) == false) { // 若密码错误，则报错，并返回初始状态
                out << "Log-in failed! Please check your SecuCode or password." << endl;
            }
        }
    }
    else {
        out << "Invalid command.Please refer to our CommandList below: " << endl;
        showCommand ();
    }
    return true;
}

void SShell::run () {
    string command;
    getline (in, command);
    while (parseCommand(command)) {
        out << "STS-S:~ $ ";
        getline (in, command);
    }
}