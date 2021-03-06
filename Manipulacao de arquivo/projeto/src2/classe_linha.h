#ifndef __INTERMEDIARE_
#define __INTERMEDIARE_
#include "classe_reg.h"
#include "classe_var.h"
#include "classe_aspas.h"
#include <bancosl.h>
#include <cstdio>
#include <vector>
//#include "classe_funcao.h"
#define DB_NAME "phpsec.db"

using namespace std;

class _linha {
public:
    vector <_var> vetor_de_variaveis;
    _reg *reg;
    _linha *irmao, *pai, *filho;
    int vet_num, colchete, dois_pontos, id, id_irmao_original, xss; //, n_funcao;
    int tipo_de_condicional;
    BANCOSL *conexao;
    string no;
    string seguranca_retorno;
    _linha();
    ~_linha();
    //int get_line_to_analyse(string , vector<_var>&, int &, _reg);
    //int get_line_to_analyse(string , int &, _reg);
    int verifica_linha(string, string &, int, int);
    void remove_space(string&);
    int analisa_linha(string);
    string string_of_var_to_reg();
    string primeira_string(string &);
    string primeira_variavel(string &, int &, _var &);
    string retorna_variavel(string &);
    string retorna_funcao(string &);
    string aspas_duplas(string &);
    string aspas_simples(string &);
    string remove_aspas(string, char);
    int type_of_operator(string &);
    int operador_normal(string, string);
    int operador_cat(string, string);
    int operador_mat(string,string);
    int pos_operador(string &, _var &, int);
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
    int novo_no(string, int, int, int);
    void display_error(string);
    string funcao_mysql(_var, string);
    int adiciona_ou_remove_nova_variavel_ao_vetor(_var);
    string imprime_todas_as_possibilidades (_var, int, string);
};

#endif