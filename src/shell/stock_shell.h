#ifndef _STOCK_SHELL_H
#define _STOCK_SHELL_H

#include <iomanip>
#include "shell_base.cpp"
#include "../kernel/stock.cpp"

class Stock_Shell: public Shell_Base {
private: 
    Stock* stock;
    static const char HELP[];
private: 
    bool parseCommand (string& command);
public: 
    Stock_Shell (Stock* _stock, istream& _in, ostream& _out);
    void hello();
    void showCommand ();
    void run ();
}; 

#endif // stock_shell.h