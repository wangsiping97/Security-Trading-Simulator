#include "stock.h"

Stock::Stock (string const& _id): id(_id) {
    myPath = thisPath + SLASH + ".." + SLASH + "data" + SLASH + "Stock" + SLASH + id;
}

struct Info Stock::getInfo() {
    fstream file(myPath);
    string line;
    getline(file, line); // password
    getline(file, line); // price
    myInfo.price = atof(line.data()); 
    getline(file, line); // floats available
    myInfo.floats_available = atoi(line.data());
    getline(file, line); // industry
    myInfo.industry = line;
    getline(file, line); // floats
    myInfo.floats = atoi(line.data());
    getline(file, line); // roa
    myInfo.roa = atof(line.data());
    getline(file, line); // roe
    myInfo.roe = atof(line.data());
    file.close();
    return myInfo;
}

bool Stock::setInfo(string const&type, string const& newInfo) {
    getInfo(); // 先从文件中取出最新数据
    if (type == "floats") {
        if (!Trading::isEmpty(id)) {
            return false;
        }
        Trading::changeFloats(id, myInfo.price, myInfo.floats_available, myInfo.floats, atoi(newInfo.data()));
        getInfo(); // 更新数据
        myInfo.floats = atoi(newInfo.data()); // 再更新股数
    }
    else if (type == "industry") {
        myInfo.industry = newInfo;
    }
    else if (type == "roa") {
        myInfo.roa = atof(newInfo.data());
    }
    else {
        myInfo.roa = atof(newInfo.data());
    }
    updateInfo();
    return true;
}

void Stock::updateInfo() {
    fstream file(myPath);
    string password;
    getline(file, password);
    string price;
    getline(file, price);
    string floats_available;
    getline (file, floats_available);
    file.close();
    ofstream out(myPath);
    out << password << endl;
    out << price << endl;
    out << floats_available << endl;
    out << myInfo.industry << endl;
    out << myInfo.floats << endl;
    out << myInfo.roa << endl;
    out << myInfo.roe << endl;
}