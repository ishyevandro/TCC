/*#include <iostream>
#include <fstream>
#include <string>
#include "classe_reg.cpp"

using namespace std;

class _var{
public:
    string variavel;
    int n_var;
    _reg *expressoes;
    _var *pilha;
    _var ();
    _var(string);
    _var & operator =(const _var &outro);
    int if_var_array(_var *, int, string);
    int get_line_to_analyse(string, _var *, int &);//analise de regex
    int add_function ();//fara parte da regex!
    
};*/

#include <iostream>
#include "classe_var.h"

_var::_var()//construtor padrao
	:n_var(0), pilha(NULL), expressoes(NULL){
	this->expressoes = new _reg;
	this->expressoes->reg_comp();
	this->variavel.empty();
	}
	
_var::_var(string x)//construtor com string da var passada.
	:pilha(NULL), n_var(0),variavel(x){}

_var& _var::operator= (const _var &recebe)//recebe uma variavel para ir para o vetor de funcao ou variavel
{
    _var *aux = new _var;
    _var *percorre_pilha;
    aux->variavel = recebe.variavel;
    aux->pilha = NULL;
    percorre_pilha = this->pilha;
    while (percorre_pilha->pilha != NULL){
        percorre_pilha = percorre_pilha->pilha;
    }
    percorre_pilha->pilha = aux;
    this->n_var++;
    return *this;
}

int _var::if_var_array (_var *vet, int size_array, string new_string){//insercao de variavel no vetor de variaveis
    int i=0;
    for(i = 0 ; i < size_array ; i++){
        if(!vet[i].variavel.compare(new_string))
            return 1;
    }
    vet[size_array].variavel = new_string;
    return 0;
}

int _var::get_line_to_analyse(string line,_var *vetor_de_variaveis, int &vet_num){
    int n, pos;
    string subline, var;
    while (n != -1){
        n = this->expressoes->reg_exec(line, pos);
        if (n != -1){
        line= line.substr(pos, line.length());
            var = line.substr(0, n);
            if (!if_var_array(vetor_de_variaveis, vet_num, var))
                cout<<var<<endl;
            line = line.substr(n, line.length());
        }
    }
    return 1;
}

