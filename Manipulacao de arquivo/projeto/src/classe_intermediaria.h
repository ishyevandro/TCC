#ifndef __INTERMEDIARE_
#define __INTERMEDIARE_
#include "classe_reg.h"
#include "classe_var.h"
#include "classe_aspas.h"
#include <cstdio>
#include <vector>
//#include "classe_funcao.h"

using namespace std;

class _intermediaria {
public:
    vector <_var> vetor_de_variaveis;
    _reg *reg;
   // _aspas *aspas;
    _intermediaria *irmao, *pai, *filho;
    int vet_num, colchete, id, id_irmao_original;//, n_funcao;
    int tipo_de_condicional;
    string no;
    string seguranca_retorno;
    _intermediaria();
    ~_intermediaria();
    //int get_line_to_analyse(string , vector<_var>&, int &, _reg);
    //int get_line_to_analyse(string , int &, _reg);
    int verifica_linha(string, string &, int);
    void remove_space(string&);
    int analisa_linha(string);
    string string_of_var_to_reg();
    string primeira_string(string &);
    string primeira_variavel(string &);
    string retorna_variavel(string &);
    string retorna_funcao(string &);
    string aspas_duplas(string &);
    string aspas_simples(string &);
    string remove_aspas (string, char);
    int type_of_operator(string &);
    int operador_normal(string/*2, string*/, string);
    int operador_cat(string/*2, string*/, string);
    int pos_operador(string &/*2, string*/, _var &);
    int retorna_elementos_dentro_de_parentese(string);
    int proximo_operador(string);
    int remove_operador_intermediario(string &);
    int operador_intermediario(string &);
    int operador_aspas(_var &, string&, int);
    int operador_normal_variavel(_var &, string&, int);
    int operador_normal_funcao(_var &, string&, int);
    int remove_variavel_do_vetor(string);
    int copia_var(_var);
    int copia_para_pai();
    int count_char(string, char);
    int adiciona_get_ou_post(_var &, string, int);
    int procura_variavel_no_vetor(string);
    int add_var_array(string);
    int verifica_funcao(string);
    void imprime_vetor();
    void imprime_elemento(_var);
    int novo_no (string, int, int, int);
    void display_error(string);
    int funcao_mysql(_var);
    int funcao_postgre(_var);
    int funcao_padrao(_var, int);
    int adiciona_ou_remove_nova_variavel_ao_vetor(_var);
    //int remove_comments(string&, _reg);
};

#endif