#include <iostream>
#include "classe_var.h"

/*=====================================OPERADORES E CONSTRUTOR*/
_var::_var()//construtor padrao
	:n_var(0), pilha(NULL){
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

/*====================================MANIPULACAO PRINCIPAL*/

int _var::if_var_array (_var *vet, int size_array, string new_string){//insercao de variavel no vetor de variaveis
    int i=0;

    for(i = 0 ; i < size_array ; i++){
        if(!vet[i].variavel.compare(new_string))
            return 1;
    }
    //cout << new_string<< " if_var"<< endl;
    vet[size_array].variavel = new_string;
   // cout << vet[size_array].variavel<< endl;
    return 0;
}

string _var::string_of_var_to_reg (_var *vet, int size_array){//construcao de expressao para verificar se as variaveis estao na linha
    string ret;
    ret.clear();
    int i=0;
    //imprime_vetor(vet, size_array);
    for(i = 0 ; i < size_array ; i++){
        ret+= "(\\";
        ret+= vet[i].variavel;
        if (i+1 !=size_array)
          ret+= ")|";
        else
          ret+= ")";
    }
    return ret;
}



int _var::get_line_to_analyse(string line,_var *vetor_de_variaveis, int &vet_num,_reg reg){
int n, pos;
string subline, var;
subline = string_of_var_to_reg(vetor_de_variaveis, vet_num);//forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas
//cout << subline << endl;
n = reg.reg_get_or_post(line, subline);//compila e verifica se alguma variavel tem get ou post
//cout << n << endl;
  if (n == 1){
  //cout << line<< endl;
    while (n != -1){
      n = reg.reg_exec(line, pos);//verificacoes posteriores verificar depois pra documentar corretamente
      if (n != -1){
        line = line.substr(pos, line.length());
        var = line.substr(0, n);
        if (!if_var_array(vetor_de_variaveis, vet_num, var)){
         // cout<<var<<endl;
          vet_num++;
        }
        line = line.substr(n, line.length());
      }
    }
  }
  //imprime_vetor (vetor_de_variaveis,vet_num);
return 1;
}

void _var::imprime_vetor(_var *vet, int tam){
  int i;
  for (i=0;i<tam;i++)
    cout << vet[i].variavel<<endl;
  cout <<"??????"<<endl;

}

