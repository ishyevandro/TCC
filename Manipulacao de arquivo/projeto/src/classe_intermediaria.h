#ifndef __INTERMEDIARE_
#define __INTERMEDIARE_
#include "classe_reg.h"
#include "classe_var.h"
#include "classe_aspas.h"
#include <vector>
//#include "classe_funcao.h"

using namespace std;

class _intermediaria {
public:
    vector <_var> vetor_de_variaveis;
    _reg *reg;
    _aspas *aspas;
    int vet_num, n_funcao;
    _intermediaria();
    //int get_line_to_analyse(string , vector<_var>&, int &, _reg);
    //int get_line_to_analyse(string , int &, _reg);
    int verifica_linha(string);
    void remove_space(string&);
    int analisa_linha(string);
    string string_of_var_to_reg();
    string primeira_string(string &);
    string primeira_variavel(string &);
    string retorna_variavel(string &);
    string retorna_funcao(string &);
    string aspas_duplas(string &);
    string aspas_simples(string &);
    int type_of_operator(string &);
    int operador_normal(string, string, string);
    int operador_cat(string, string, string);
    int pos_operador(string &, string, _var &);
    int retorna_elementos_dentro_de_parentese(string);
    int proximo_operador(string);
    int remove_operador_intermediario(string &);
    int operador_intermediario(string &);
    int operador_aspas(_var &, string&, int);
    int operador_normal_variavel(_var &, string&, int);
    int operador_normal_funcao(_var &, string&, int);
    int remove_variavel_do_vetor(string);
    int copia_var(_var);
    int count_char(string, char);
    int adiciona_get_ou_post(_var &, string, int);
    int procura_variavel_no_vetor(string);
    int add_var_array(string);
    int verifica_funcao(string);
    void imprime_vetor();
    void imprime_elemento(_var);
    //int remove_comments(string&, _reg);
};

#endif