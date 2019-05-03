#ifndef _ROOTSHELL_H
#define _ROOTSHELL_H

#include <sstream>
#include <string>
#include <vector>
#include "../kernel/logger.cpp"

using std::string;
using std::vector;
using std::istream;
using std::ostream;
using std::endl;

// Announcement: Shell_Base class and IShell class (for investment environment)
class Shell_Base {
protected: 
    istream& in;
    ostream& out;
public: 
    void clearScreen ();
    Shell_Base (istream& _in, ostream& _out);
};

class IShell: public Shell_Base {
private: 
    using Shell_Base::in;
    using Shell_Base::out;
    static const char I_HELP[];
    vector<string> vcmd;
private:
    bool checkPassword(string const& str1, string const& str2);
    void cutOut (string const& input); 
    bool parseCommand (string& command);
public: 
    void showCommand ();
    IShell (istream& _in, ostream& _out);
    void run ();
};

class SShell: public Shell_Base {
private: 
    using Shell_Base::in;
    using Shell_Base::out;
    static const char S_HELP[];
    vector<string> vcmd;
private:
    void cutOut (string const& input); 
    bool parseCommand (string& command);
public: 
    void showCommand ();
    SShell (istream& _in, ostream& _out);
    void run ();
};

#endif