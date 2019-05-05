#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "shell/root_shell.cpp"

class Global {
private: 
    static bool test(string const& _ans);
public:
    static void init();
    static void run();
};

#endif // global.h