#ifndef _CLASSE_ARQUIVO_H_
#define _CLASSE_ARQUIVO_H_
#include "classe_var.h"
#include "classe_reg.h"
#include "classe_intermediaria.h"
using namespace std;

class _arq{
public:
	ifstream new_file;
        _intermediaria aux;
	_var *vetor_de_variaveis;//mapeamento de todas as variaveis do arquivo
	_reg *reg;
	int vet_num;
public:
	_arq();
	void read ();
	void open(string);
	
};
#endif
