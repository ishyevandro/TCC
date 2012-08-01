#include <iostream>
#include "classe_var.h"

/*=====================================OPERADORES E CONSTRUTOR===============================*/
_var::_var()//construtor padrao
: n_var(0), pilha(NULL) {
    this->variavel.clear();
    this->functions.clear();
}

_var::_var(string x)//construtor com string da var passada.
: pilha(NULL), n_var(0), variavel(x) {
}

_var& _var::operator=(const _var &recebe)//recebe uma variavel para ir para o vetor de funcao ou variavel
{
    _var *aux = new _var;
    _var *percorre_pilha;
    aux->variavel = recebe.variavel;
    aux->pilha = NULL;
    percorre_pilha = this->pilha;
    while (percorre_pilha->pilha != NULL) {
        percorre_pilha = percorre_pilha->pilha;
    }
    percorre_pilha->pilha = aux;
    this->n_var++;
    return *this;
}

/*====================================MANIPULACAO PRINCIPAL=========================*/

/*VERIFICACAO DE VARIAVEL, VERIFICA SE A VARIAVEL JA ESTA NO VETOR DE VARIAVEIS*/
int _var::if_var_array(_var *vet, int size_array, string new_string) {//insercao de variavel no vetor de variaveis
    int i = 0;

    for (i = 0; i < size_array; i++) {
        if (!vet[i].variavel.compare(new_string))
            return 1;
    }
    //cout << new_string<< " if_var"<< endl;
    vet[size_array].variavel = new_string;
    // cout << vet[size_array].variavel<< endl;
    return 0;
}

/*FORMA A STRING DE VARIAVEIS PARA FORMACAO DE EXPRESSAO REGULAR*/
string _var::string_of_var_to_reg(_var *vet, int size_array) {//construcao de expressao para verificar se as variaveis estao na linha
    string ret;
    ret.clear();
    int i = 0;
    //imprime_vetor(vet, size_array);
    for (i = 0; i < size_array; i++) {
        ret += "(\\";
        ret += vet[i].variavel;
        if (i + 1 != size_array)
            ret += ")|";
        else
            ret += ")";
    }
    return ret;
}

/*RECEBE A LINHA PARA ANALISE*/
int _var::get_line_to_analyse(string line, _var *vetor_de_variaveis, int &vet_num,_reg reg) {
    int n, pos;
    string array_of_vars_in_array;
    array_of_vars_in_array = string_of_var_to_reg(vetor_de_variaveis, vet_num); //forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas

    n = reg.mount_reg_get_or_post(line, array_of_vars_in_array); //compila e verifica se alguma variavel tem get ou post

    if (n == TRUE_VALUE) {
        n = reg.reg_exec_first_string(line, pos);//pega a primeira string
        if (n != FALSE_VALUE) {
            new_variable(line, pos, vetor_de_variaveis, vet_num, n);
            remove_space(line);
            type_of_operator(line, reg);
            while (n != FALSE_VALUE) {
                //cout << "%" << line << "%" << endl;
                n = reg.reg_exec(line, pos); //verificacoes posteriores verificar depois pra documentar corretamente
                remove_space(line);
                if (n != FALSE_VALUE)
                    new_variable(line, pos, vetor_de_variaveis, vet_num, n);
            }
        }
    }
    return 1;
}

/*ADICIONA UMA NOVA VARIAVEL AO VETOR DE VARIAVEIS*/
void _var::new_variable(string &line, int &pos, _var *vetor_de_variaveis, int &vet_num, int n) {
    string var;
    line = line.substr(pos, line.length());
    var = line.substr(0, n);
    if (!if_var_array(vetor_de_variaveis, vet_num, var)) {
        vet_num++;
    }
    line = line.substr(n, line.length());
}

/*IMPRESSAO DO VETOR PARA TESTE*/
void _var::imprime_vetor(_var *vet, int tam) {
    int i;
    for (i = 0; i < tam; i++)
        cout << vet[i].variavel << "." << endl;
    cout << "??????" << endl;

}

void _var::remove_space(string &line) {
    size_t found;
    found = line.find_first_not_of(" ");
    line = line.substr((int)found, line.length());
}

/*VERIFICACAO DO TIPO DE OPERACAO*/
int _var::type_of_operator(string line, _reg reg) {
    int type;
    type = reg.reg_to_operator(line);
    cout<<type<<endl;
    return type;
}



