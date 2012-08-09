
#ifndef _CLASSE_REG_H_
#define _CLASSE_REG_H_
#include <boost/regex.h>
#include <iostream>
using namespace std;

#define FALSE_VALUE -1
#define REG_P_G 0
#define REG_O_P_G 6
#define REG_P_G_V 7
#define REG_$ 1
#define TRUE_VALUE 1
#define REG_FIRST 2
#define REG_OPERATOR 3
#define REG_FUNCTION 4
#define REG_VARIABLE 5

#define REG_OPERATOR_NORMAL 20
#define REG_OPERATOR_SOMA 21
#define REG_OPERATOR_SUB 22
#define REG_OPERATOR_MULT 23
#define REG_OPERATOR_DIV 24
#define REG_OPERATOR_RES 25
#define REG_OPERATOR_CAT 26
#define REG_POS_OPERADOR_OPERADOR 27

#define REG_COMMENTS_SIMPLE 40
#define REG_COMMENTS_INI 41
#define REG_COMMENTS_FIM 42
#define MAX_REG 100

class _reg {
public:
    regex_t *first;
    int num_reg;
    string variables_with_p_or_g;
    regmatch_t result;
    _reg();
    void reg_comp();
    int reg_exec_all(string, int);
    int reg_comp_all(string, int);
    int reg_exec_to_line(string, int&);
    int reg_exec_first_string(string, int&);
    int reg_comments(string);
    int mount_reg_get_or_post(string, string);
    int reg_to_operator(string);
    int what_is_first_string(string);
    int reg_segunda_parte_linha(string );
};
#endif

