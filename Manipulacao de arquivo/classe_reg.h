
#ifndef _CLASSE_REG_H_
#define _CLASSE_REG_H_
#include <boost/regex.h>
#include <iostream>
using namespace std;

#define REG_P_G 0
#define REG_$ 1
#define REG_OPERATOR 2

class _reg{
public:
    regex_t *first;
    int num_reg;
    string variables_with_p_or_g;
    regmatch_t result;
    _reg();
    void reg_comp();
    int reg_exec(string, int&);
    int reg_get_or_post(string, string);
};
#endif

