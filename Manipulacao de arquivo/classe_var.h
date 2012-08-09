#ifndef _CLASSE_VAR_H_
#define _CLASSE_VAR_H_
#include <iostream>
#include <vector>
#include "classe_reg.h"
#include "variavel.h"

#define VET_NUM 10
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
    int if_var_array(vector<_var> &, int &, string);
    int analyse_line(string, vector<_var> &, int &, _reg); //analise de regex
    int add_function(); //fara parte da regex!
    string string_of_var_to_reg(vector<_var> &, int);
    //int new_variable(string &, int &, vector<_var> , int &, int, _reg);
    int type_of_operator(string, _reg);
    void remove_space(string&);
    //int verify_comments(string, _reg);
    void remove_aspa_simples(string &); //implementar assim que a analise inicial for feita
    void imprime_vetor(vector<_var> , int);
    int procura_variavel_no_vetor(string, vector<_var> &, int &);
    int remove_variavel_do_vetor(string, vector<_var> &, int &);
    void adiciona_get_ou_post(_var &, string, _reg, string,vector<_var> , int );
    string string_de_vars_pos_operador(vector<_var> , int);

};
#endif
