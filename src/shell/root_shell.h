#ifndef _ROOTSHELL_H
#define _ROOTSHELL_H

#include "shell_base.cpp"
#include "../kernel/logger.cpp"

// Announcement: SShell_Base class (for securtiy environment) and IShell class (for investment environment)

class IShell: public Shell_Base {
private: 
    using Shell_Base::in;
    using Shell_Base::out;
    using Shell_Base::vcmd;
    static const char I_HELP[];
private:
    bool checkPassword(string str1, string str2);
    bool parseCommand (string& command);
public: 
    IShell (istream& _in, ostream& _out);
    void showCommand ();
    void run ();
};

class SShell: public Shell_Base {
private: 
    using Shell_Base::in;
    using Shell_Base::out;
    using Shell_Base::vcmd;
    static const char S_HELP[];
private:
    bool parseCommand (string& command);
public: 
    SShell (istream& _in, ostream& _out);
    void showCommand ();
    void run ();
};

#endif // root_shell.h