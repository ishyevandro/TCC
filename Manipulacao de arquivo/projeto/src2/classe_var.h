#ifndef _CLASSE_VAR_H_
#define _CLASSE_VAR_H_
#include <iostream>
#include <vector>
#include "classe_reg.h"
#include "variavel.h"

#define VET_NUM 200
using namespace std;

class _var {
public:
    vector<_variavel> vect;
    string variavel;
    int n_var;
    //_var *pilha;
    vector<int> first;
    //vector<_variavel> vars;

    _var();
    _var(string);
    _var & operator =(const _var &outro);
};
#endif
