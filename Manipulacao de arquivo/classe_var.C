#include <iostream>
#include "classe_var.h"
#include "variavel.h"

/*=====================================OPERADORES E CONSTRUTOR===============================*/
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

/*====================================MANIPULACAO PRINCIPAL=========================*/

/*VERIFICACAO DE VARIAVEL, VERIFICA SE A VARIAVEL JA ESTA NO VETOR DE VARIAVEIS*/
int _var::if_var_array(vector<_var> &vet, int &size_array, string new_string) {//insercao de variavel no vetor de variaveis
    int i = 0;
    _var *aux;
    aux = new _var;
    for (i = 0; i < size_array; i++) {
        if (!vet[i].variavel.compare(new_string))
            return 1;
    }
    //vet.push_back(*aux);
    vet[size_array].variavel = new_string;
    size_array++;
    return 0;
}

/*FORMA A STRING DE VARIAVEIS PARA FORMACAO DE EXPRESSAO REGULAR*/
string _var::string_of_var_to_reg(vector<_var> &vet, int size_array) {//construcao de expressao para verificar se as variaveis estao na linha
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
int _var::analyse_line(string line, vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg) {
    int n, pos, operator_type, auxiliar, pos_var;
    string array_of_vars_in_array, procura_operador, subline, variavel, reg_string_de_var_com_g_p;
    n = pos = operator_type = 0;
    array_of_vars_in_array.clear();
    /*MONTA A EXPRESSAO REGULAR PARA INCLUIR AS VARIAVEIS QUE ESTAO COM POST E GET*/
    if (vet_num > 0)
        array_of_vars_in_array = string_of_var_to_reg(vetor_de_variaveis, vet_num); //forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas
    n = reg.mount_reg_get_or_post(line, array_of_vars_in_array); //compila e verifica se alguma variavel tem get ou post

    /*IF true caso tenha alguma variavel no vetor que possui get ou post*/
    if (n == TRUE_VALUE) {
        remove_space(line);
        n = reg.reg_exec_first_string(line, pos); //pega a primeira string e retorna onde ela acaba
        if (n != FALSE_VALUE) {//PORCARIA TODA ERRADA. ALTERAR A EXPRESSAO REGULAR PARA PROCURAR PELO $
            variavel = line.substr(pos, n);
            procura_operador = line.substr(n, line.length()); /*pega a partir do final da primeira string pra frente*/
            remove_space(procura_operador); /*remove espacos para o primeiro caracter ser parte do operador ja*/
            operator_type = type_of_operator(procura_operador, reg); /*verifica qual o tipo de operador*/
            switch (operator_type) {/*acoes a serem feitas com cada operador*/
                case REG_OPERATOR_NORMAL:
                    /*Subline 'e a variavel que possui como inicio o primeiro campo apos o operador*/
                    subline = procura_operador.substr(1, procura_operador.length());
                    /*verifica se nessa parte tem alguma variavel pra frente que possua get ou post*/
                    auxiliar = reg.mount_reg_get_or_post(subline, array_of_vars_in_array); //SE HOUVER UM WHILE AQUI DARA ERRO
                    if (auxiliar != FALSE_VALUE) {
                        reg_string_de_var_com_g_p = string_de_vars_pos_operador(vetor_de_variaveis, vet_num);
                        pos_var = procura_variavel_no_vetor(variavel, vetor_de_variaveis, vet_num);
                        //cout << reg_string_de_var_com_g_p << endl;
                        adiciona_get_ou_post(vetor_de_variaveis[pos_var], subline, reg, reg_string_de_var_com_g_p, vetor_de_variaveis, vet_num);
                    } else {
                        remove_variavel_do_vetor(variavel, vetor_de_variaveis, vet_num);
                        cout << "Nenhuma variavel daqui pra frente tem get ou post" << endl;
                    }

                    break;
                case REG_OPERATOR_CAT:
                    cout << "concatena" << endl;
                    break;
                    //case REG_OPERATOR_SOMA REG_OPERATOR_SUB REG_OPERATOR_DIV REG_OPERATOR_RES:;
                default:
                    cout << "operadores de some aprender a usar switch" << endl;
                    break;
            }
            remove_space(line);
        } else {//quando for objeto e estiver sendo chamado uma funcao do mesmo
            cout << "E uma funcao de um objeto ou variavel" << endl;
        }
    }
    return 1;
}

void _var::remove_space(string &line) {
    size_t found;
    found = line.find_first_not_of(" ");

    line = line.substr((int) found, line.length());
}

void _var::remove_aspa_simples(string &subline) {
    size_t found;
    do {
        found = subline.find_first_not_of(" ");

        subline = subline.substr((int) found, subline.length());
    } while (1 != 1);
}

/*VERIFICACAO DO TIPO DE OPERACAO*/
int _var::type_of_operator(string line, _reg reg) {
    return reg.reg_to_operator(line);
}

/*IMPRESSAO DO VETOR PARA TESTE*/
void _var::imprime_vetor(vector<_var> vet, int tam) {
    int i, j;
    for (i = 0; i < tam; i++){
        cout << vet[i].variavel<< endl;
        for (j = 0; j < vet[i].n_var; j++)
            cout<<"==="<<vet[i].vect[j].variavel<<endl;
    }cout << "??????" << endl;

}

int _var::procura_variavel_no_vetor(string var, vector<_var> &vetor_de_variaveis, int &vet_num) {
    int i;
    for (i = 0; i < vet_num; i++) {
        if (var.compare(vetor_de_variaveis[i].variavel) == 0) {
            cout << "encontrada no vetor" << endl;
            return i;
        }
    }
    if_var_array(vetor_de_variaveis, vet_num, var);
    //cout << "Procura variavel: " << vetor_de_variaveis[vet_num - 1].variavel << endl;

    return vet_num - 1;
}

int _var::remove_variavel_do_vetor(string var, vector<_var> &vetor_de_variaveis, int &vet_num) {
    int i;
    for (i = 0; i < vet_num; i++) {
        if (var.compare(vetor_de_variaveis[i].variavel) == 0) {
            vetor_de_variaveis.erase(vetor_de_variaveis.begin() + i);
            vet_num--;
            return i;
        }
    }
    if_var_array(vetor_de_variaveis, vet_num, var);
    //cout<<var<<endl;
    //cout<<vetor_de_variaveis[vet_num].variavel<<endl;
    //vet_num++;
    return vet_num - 1;
}

void _var::adiciona_get_ou_post(_var &add, string subline, _reg reg, string vet_var_p_g, vector<_var> procura, int vet_num) {//vai ser adicionado a excessao de aspas simples aqui
    int pos, n;
    string string_guardar;
    _variavel *aux;
    remove_space(subline);
    size_t found;
    if (1 == 1) {//implementar a diferenca entre get direto e htmlspecial(get);
        if (reg.reg_segunda_parte_linha(subline) == TRUE_VALUE) {//verifica se o primeiro elemento 'e um get ou post
            found = subline.find_first_of("]");
            string_guardar = subline.substr(0, (int) found + 1);
            add.vect[add.n_var].variavel = string_guardar;
            add.n_var++;
        } else if (!vet_var_p_g.empty() && reg.mount_reg_get_or_post(subline, vet_var_p_g) != FALSE_VALUE) {
            found = subline.find_first_of(" .+-;");
            string_guardar = subline.substr(0, (int) found);

            pos = procura_variavel_no_vetor(string_guardar, procura, vet_num);
            for (n = 0; n < procura[pos].n_var; n++) {
                add.vect[add.n_var].variavel = procura[pos].vect[n].variavel;
                add.n_var++;
            }
        } else {
            cout << "Se nao tem get nem variavel sei lah!!" << endl;
            cout << subline << endl;
        }
    }
    cout << "add var: " << add.vect[add.n_var - 1].variavel << endl;
}

string _var::string_de_vars_pos_operador(vector<_var> vet, int size_array) {//construcao de expressao para verificar se as variaveis estao na linha
    string ret;
    int i = 0;
    ret.clear();

    if (size_array == 0)
        return ret;
    else {
        for (i = 0; i < size_array; i++) {
            ret += "(^\\";
            ret += vet[i].variavel;
            if (i + 1 != size_array)
                ret += ")|";
            else
                ret += ")";
        }
        return ret;
    }
}