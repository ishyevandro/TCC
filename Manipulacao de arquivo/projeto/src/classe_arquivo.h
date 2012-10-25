#ifndef _CLASSE_ARQUIVO_H_
#define _CLASSE_ARQUIVO_H_
#include "classe_var.h"
#include "classe_reg.h"
#include "classe_intermediaria.h"
#include "classe_aspas.h"
#include <vector>
using namespace std;

class _arq{
public:
	//ifstream new_file;
        string codigo;
        int tag_php;
        _intermediaria aux, *no_corrente;
	//vector <_var> vetor_de_variaveis;//mapeamento de todas as variaveis do arquivo
	_reg *reg;
        //_aspas *aspas;
	//int vet_num;
public:
	_arq();
	void read ();
	void open(string);
        int nova_linha (string &);
        int aspas_duplas(string &);
        int aspas_simples(string &);
	int remove_comments(string &);
        int verifica_comentario_dentro_de_aspas(string);
        int verifica_tag(string &);
        int verifica_condicional (string &);
        string procura_fim_de_linha (string);
        string replace_aspas (string);
        int linha(string);
        int verifica_pos_condicional(string&);
};
#endif
