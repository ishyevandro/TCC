#ifndef _CLASSE_VAR_H_
#define _CLASSE_VAR_H_
#include "classe_reg.h"
using namespace std;

class _var{
public:
    string variavel;
    int n_var;
    _var *pilha;
    _var ();
    _var(string);
    _var & operator =(const _var &outro);
    int if_var_array(_var *, int , string);
    int get_line_to_analyse(string,_var *,int &, _reg);//analise de regex
    int add_function ();//fara parte da regex!
    string string_of_var_to_reg(_var*, int);
    
    void imprime_vetor(_var *, int);
    
};

#endif
