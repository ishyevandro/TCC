#include <iostream>
#include "classe_var.h"
#include "variavel.h"

//void display_error(string prnt) {
  //  if (DISPLAY_ERROR == 1)
    //    cout << prnt << endl;
//}

/*#NECESSARIO ALTERACAO DA BUSCA DE POST E GET... BUSCANDO POR EXPRESSAO REGULAR PARA RETIRAR
 O ESPCAO ENTRE O $_GET   (ESTE ESPACO)[ 
*/
//=====================================OPERADORES E CONSTRUTOR=============================== 
_var::_var()//construtor padrao
: n_var(0) {
    this->vect.resize(VET_NUM);
    this->variavel.clear();
}

_var::_var(string x)//construtor com string da var passada.
: n_var(0), variavel(x) {
    this->vect.resize(VET_NUM);
}

_var& _var::operator=(const _var &recebe)//recebe uma variavel para ir para o vetor de funcao ou variavel
{
    _var *aux = new _var;
    _var *percorre_pilha;
    aux->variavel = recebe.variavel;
    this->n_var++;
    return *this;
}