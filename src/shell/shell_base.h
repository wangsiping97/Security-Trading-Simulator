#ifndef _SHELL_BASE_H
#define _SHELL_BASE_H

#include <iostream> 
#include <unistd.h>  
#include <sstream>
#include <string>
#include <vector>

#if defined __APPLE__ || defined __linux__
#include <termios.h>
#else
#include <conio.h>
#endif

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
    void cutOut (string const& input, const char flag); 
public: 
    Shell_Base (istream& _in, ostream& _out);
    void clearScreen ();
    void wrong ();
    void getPassword (string &password);
};

#endif // shell_base.h