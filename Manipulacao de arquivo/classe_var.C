#include <iostream>
#include "classe_var.h"
#include "variavel.h"

/*=====================================OPERADORES E CONSTRUTOR===============================*/
_var::_var()//construtor padrao
: n_var(0), pilha(NULL) {
    this->vect.resize(VET_NUM);
    this->variavel.clear();
}

_var::_var(string x)//construtor com string da var passada.
: pilha(NULL), n_var(0), variavel(x) {
    this->vect.resize(VET_NUM);
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
    vet[size_array].variavel = new_string;
    return 0;
}

/*FORMA A STRING DE VARIAVEIS PARA FORMACAO DE EXPRESSAO REGULAR*/
string _var::string_of_var_to_reg(_var *vet, int size_array) {//construcao de expressao para verificar se as variaveis estao na linha
    string ret;
    int i = 0;
    ret.clear();
    if (size_array == 0)
        ret = "(\\$_POST*)|(\\$_GET*)";
    else
        ret = "(\\$_POST*)|(\\$_GET*)|";
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
int _var::analyse_line(string line, _var *vetor_de_variaveis, int &vet_num, _reg reg) {
    int n, pos, operator_type;
    string array_of_vars_in_array;
    n = pos = operator_type = 0;
    array_of_vars_in_array.clear();
    if (vet_num > 0)
        array_of_vars_in_array = string_of_var_to_reg(vetor_de_variaveis, vet_num); //forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas
    n = reg.mount_reg_get_or_post(line, array_of_vars_in_array); //compila e verifica se alguma variavel tem get ou post
    if (n == TRUE_VALUE) {
        remove_space(line);
        n = reg.reg_exec_first_string(line, pos); //pega a primeira string
        if (n != FALSE_VALUE) {
            n = new_variable(line, pos, vetor_de_variaveis, vet_num, n, reg);

            remove_space(line);

            operator_type = type_of_operator(line, reg);
            while (n != FALSE_VALUE) {//totalmente mal projetado
                n = reg.reg_exec_to_line(line, pos); //verificacoes posteriores verificar depois pra documentar corretamente
                remove_space(line);
                n = new_variable(line, pos, vetor_de_variaveis, vet_num, n, reg);
            }
        } else {//quando for objeto e estiver sendo chamado uma funcao do mesmo
            cout << "E uma funcao de um objeto ou variavel" << endl;
        }
    }
    return 1;
}

/*ADICIONA UMA NOVA VARIAVEL AO VETOR DE VARIAVEIS*/
int _var::new_variable(string &line, int &pos, _var *vetor_de_variaveis, int &vet_num, int n, _reg reg) {
    string var, cclear, subline;
    int aux;
    cclear.clear();
    line = line.substr(pos, line.length());
    var = line.substr(0, n);
    subline = line.substr(n, line.length());
    aux = reg.mount_reg_get_or_post(line.substr(n, line.length()), cclear);
    if (aux == FALSE_VALUE) {//restante da string nao possui get ou post entao essa string esta recebendo qualquer outra coisa. tirar ela do vetor
        cout << "implementarei um metodo para ranca-la do vetor"<< endl;
        return FALSE_VALUE;
    } else {
        aux = reg.mount_reg_get_or_post(var, cclear);
        /*if (aux == 1) {verifica se a primeira variavel ja possui um get ou post
             fazer alguma outra verificacao aqui que nao esta visivel pra mim no momento
               algo relacionado a operadores para mudar a variavel ou adcionar nela e tals.
               
        }*/
        if (!if_var_array(vetor_de_variaveis, vet_num, var)) {
            vet_num++;
        }
    }
    line = line.substr(n, line.length()); //essa linha esta pulando operadores remocao futura!!
    return TRUE_VALUE;
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

    line = line.substr((int) found, line.length());
}

/*VERIFICACAO DO TIPO DE OPERACAO*/
int _var::type_of_operator(string line, _reg reg) {
    int type;
    type = reg.reg_to_operator(line);
    return type;
}

