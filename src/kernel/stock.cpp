#include <string>
// #include <fstream>

using std::string;
// using std::fstream;
// using std::ofstream;
// using std::ifstream;
// using std::endl;

struct Info {
    double price;
    string industry;
    long floats;
    double roa;
    double roe;
};

class Stock {
public: 
    string id;
    // string myPath;
    // struct Info myInfo;
public: 
    Stock (string const& _id);
};

Stock::Stock (string const& _id): id(_id) {
    
}