#ifndef _SHELL_BASE_H
#define _SHELL_BASE_H

#include <iostream> 
#include <sstream>
#include <vector>

using std::istream;
using std::ostream;
using std::vector;
using std::string;
using std::endl;

class Shell_Base {
protected: 
    istream& in;
    ostream& out;
    vector<string> vcmd;
public: 
    Shell_Base (istream& _in, ostream& _out);
    void clearScreen ();
    void wrong ();
    void showStockList();
    void cutOut (string const& input, const char flag); 
};

#endif // shell_base.h