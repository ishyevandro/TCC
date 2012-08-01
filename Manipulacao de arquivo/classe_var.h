#ifndef _CLASSE_VAR_H_
#define _CLASSE_VAR_H_
#include "classe_reg.h"
using namespace std;

class _var{
public:
    string variavel;
    string functions;
    int n_var;
    _var *pilha;
    _var ();
    _var(string);
    _var & operator =(const _var &outro);
    int if_var_array(_var *, int , string);
    int get_line_to_analyse(string,_var *,int &, _reg);//analise de regex
    int add_function ();//fara parte da regex!
    string string_of_var_to_reg(_var*, int);
    void new_variable(string &, int &, _var *,int &, int); 
    int type_of_operator(string, _reg);
    void remove_space(string&);
    
    void imprime_vetor(_var *, int);
    
};

class _method{
public:
    string method;
    char type_of_method;
    string functions;
    _method *fila;
};
#endif
