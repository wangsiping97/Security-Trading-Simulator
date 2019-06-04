#ifndef _STS_H
#define _STS_H

#include "shell/root_shell.cpp"

class STS {
private: 
    static bool test(string const& _ans);
public:
    static void init();
    static void run();
};

#endif // sts.h