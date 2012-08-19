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
int _var::add_var_array(vector<_var> &vet, int &size_array, string new_string) {//insercao de variavel no vetor de variaveis
    vet[size_array].variavel = new_string;
    size_array++;
    return size_array - 1;
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

string _var::string_of_var_to_reg(vector<_var> &vet, int size_array, int nada) {//construcao de expressao para verificar se as variaveis estao na linha
    string ret;
    int i = 0;
    ret.clear();
    if (size_array == 0)
        ret = "(^\\$_POST*)|(^\\$_GET*)";
    else
        ret = "(^\\$_POST*)|(^\\$_GET*)|";
    if (size_array == 0)
        return ret;
    else {
        for (i = 0; i < size_array; i++) {
            ret += "(^\\";
            ret += vet[i].variavel;
            if (i + 1 < size_array)
                ret += ")|";
            else
                ret += ")";
        }
        return ret;
    }
}

/*RECEBE A LINHA PARA ANALISE*/
int _var::analyse_line(string line, vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg) {
    int n, operator_type;
    string array_of_vars_in_array, procura_operador, subline, nova_variavel;
    n =  operator_type = 0;
    array_of_vars_in_array.clear();
    /*MONTA A EXPRESSAO REGULAR PARA INCLUIR AS VARIAVEIS QUE ESTAO COM POST E GET*/
    if (vet_num > 0)
        array_of_vars_in_array = string_of_var_to_reg(vetor_de_variaveis, vet_num); //forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas
    n = reg.mount_reg_get_or_post(line, array_of_vars_in_array); //compila e verifica se alguma variavel tem get ou post
    /*IF true caso tenha alguma variavel no vetor que possui get ou post*/
    if (n == TRUE_VALUE) {
        remove_space(line);
        nova_variavel = primeira_string(line, reg);
        //n = reg.reg_exec_first_string(line, pos); //pega a primeira string e retorna onde ela comeca em pos e retorna onde comeca operador em N
        if (n != FALSE_VALUE) {//PORCARIA TODA ERRADA. ALTERAR A EXPRESSAO REGULAR PARA PROCURAR PELO $
            cout<<"<<"<<line<<endl;
            //procura_operador = line;//.substr(n, line.length()); /*pega a partir do final da primeira string pra frente*/
            remove_space(line); /*remove espacos para o primeiro caracter ser parte do operador ja*/
            operator_type = type_of_operator(line, reg); /*verifica qual o tipo de operador*/
            cout<<">>"<<line<<"  "<<operator_type<<endl;
            switch (operator_type) {/*acoes a serem feitas com cada operador*/
                case REG_OPERATOR_NORMAL:
                    /*Subline 'e a variavel que possui como inicio o primeiro campo apos o operador*/
                    //subline = line.substr(1, line.length());
                    operador_normal(vetor_de_variaveis, vet_num, reg, line, array_of_vars_in_array, nova_variavel);
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
int _var::type_of_operator(string &line, _reg reg) {
    int retorno;
    retorno = reg.reg_to_operator(line);
    line = line.substr(1, line.length());
    return retorno;
}

/*IMPRESSAO DO VETOR PARA TESTE*/
void _var::imprime_vetor(vector<_var> vet, int tam) {
    int i, j;
    for (i = 0; i < tam; i++) {
        cout << vet[i].variavel << " N parametros: " << vet[i].n_var << endl;
        for (j = 0; j < vet[i].n_var; j++)
            cout << "===" << vet[i].vect[j].variavel << endl;
    }
    cout << "??????" << endl;

}

int _var::procura_variavel_no_vetor(string var, vector<_var> &vetor_de_variaveis, int &vet_num) {
    int i;
    for (i = 0; i < vet_num; i++) {
        if (var.compare(vetor_de_variaveis[i].variavel) == 0) {
            //   cout << "encontrada no vetor" << endl;
            return i;
        }
    }
    return -1;
}

int _var::remove_variavel_do_vetor(string var, vector<_var> &vetor_de_variaveis, int &vet_num) {
    int i, pos;
    pos = procura_variavel_no_vetor(var, vetor_de_variaveis, vet_num);
    for (i = pos; i < vet_num - 1; i++) {
        vetor_de_variaveis[i].variavel = vetor_de_variaveis[i + 1].variavel;
        vetor_de_variaveis[i].vect = vetor_de_variaveis[i + 1].vect;
    }
    vetor_de_variaveis[vet_num - 1].variavel.clear();
    vetor_de_variaveis[vet_num - 1].n_var = 0;
    vet_num--;
    return 1;
}

void _var::adiciona_get_ou_post(_var &add, string &subline, _reg reg, string vet_var_p_g, vector<_var> procura, int vet_num) {//vai ser adicionado a excessao de aspas simples aqui
    int pos, n;
    string string_guardar;
    remove_space(subline);
    size_t found;
   // cout << "==================" << endl << subline << endl;
    if (1 == 1) {//implementar a diferenca entre get direto e htmlspecial(get);
        if (reg.reg_segunda_parte_linha(subline) == TRUE_VALUE) {//verifica se o primeiro elemento 'e um get ou post
            found = subline.find_first_of("]");
            string_guardar = subline.substr(0, (int) found + 1);
            add.vect[add.n_var].variavel = string_guardar;
            add.n_var++;
            subline = subline.substr((int) found + 1, subline.length());
        } else if (!vet_var_p_g.empty() && reg.mount_reg_get_or_post(subline, vet_var_p_g) != FALSE_VALUE) {
            found = subline.find_first_of(" .+-;");
            string_guardar = subline.substr(0, (int) found);
            pos = procura_variavel_no_vetor(string_guardar, procura, vet_num);
            for (n = 0; n < procura[pos].n_var; n++) {
                add.vect[add.n_var].variavel = procura[pos].vect[n].variavel;
                add.n_var++;
            }
            subline = subline.substr((int) found, subline.length());
        } else {//SE TA LOCAO?!
            cout << "Se nao tem get nem variavel sei lah!!" << endl;
            cout << subline << endl;
        }
    }

    //cout << "add var: " << add.vect[add.n_var - 1].variavel <<endl;
}

int _var::operador_normal(vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg, string subline, string reg_vetor_de_variaveis, string nova_variavel) {
    int auxiliar, pos_var, tipo;
    string reg_string_de_var_com_g_p;
    /*verifica se nessa parte tem alguma variavel pra frente que possua get ou post*/
   // cout<<"======ANTES DE PASSAR___"<<subline<<endl;
    remove_space(subline);
    if (auxiliar != FALSE_VALUE) {
        while (subline[0] != ';') {
         //   cout<<"======ANTES DE PASSAR___"<<subline<<endl;//sera necessario implementar para verificar se essa variavel e concatenada... ou ate remover ela da posicao onde esta e caso tenha outra no vetor apagar a outra!
            auxiliar = reg.mount_reg_get_or_post(subline, reg_vetor_de_variaveis); //SE HOUVER UM WHILE AQUI DARA ERRO
            if (auxiliar != FALSE_VALUE) {//verificar se ainda ha algum elemento com get ou post esta errado, precisa ser feito verificacao de variavel por variavel ate o final
                tipo = reg.what_is_first_string(subline); //verifica se e variavel ou funcao
                if (tipo == REG_VARIABLE) {
                    remove_space(subline);
                    operador_normal_variavel(vetor_de_variaveis, vet_num, reg, nova_variavel, subline);
                } else if (tipo == REG_FUNCTION) {
                    cout << "NEM ERA PRA ENTRAR AQUI" << endl; //TA ERRADO POR QUE ESTA ENTRANDO? REGEX ERRADA?
                    remove_space(subline);
                    operador_normal_funcao(vetor_de_variaveis, vet_num, reg, nova_variavel, subline);
                }
                remove_space(subline);
              //  cout<<subline<<endl;
                if (subline[0] != ';')
                    operador_intermediario(subline, reg);
                remove_space(subline);
            }else
                subline = ";";
        }
    } else {//remove a variavel da lista
        cout<<"Removendo variavel: "<<nova_variavel<<endl;
        remove_variavel_do_vetor(nova_variavel, vetor_de_variaveis, vet_num);
    }
    return 0;
}

int _var::operador_intermediario(string &subline, _reg reg) {
    int aux;
    aux = reg.reg_operador_cat_ou_aritmetico(subline);
    if (aux == REG_POS_OPERATOR_CAT) {
        cout << "concatena _" << subline << endl;
    } else if (aux == REG_POS_OPERATOR_MAT) {
        cout << "matematico _" << subline << endl;
    } else {
        cout << "fudeu _" << subline << endl;
    }
    subline = subline.substr(1, subline.length());
    //cout << subline << endl;
    return 0;

}

int _var::operador_normal_variavel(vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg, string nova_variavel, string &subline) {
    int pos_var;
    string reg_string_de_var_com_g_p;
    pos_var = procura_variavel_no_vetor(nova_variavel, vetor_de_variaveis, vet_num);
    if (pos_var == FALSE_VALUE)
        pos_var = add_var_array(vetor_de_variaveis, vet_num, nova_variavel);
    reg_string_de_var_com_g_p = string_of_var_to_reg(vetor_de_variaveis, vet_num, 0);
    adiciona_get_ou_post(vetor_de_variaveis[pos_var], subline, reg, reg_string_de_var_com_g_p, vetor_de_variaveis, vet_num);
    return pos_var; //ALTERAR ESSE RETURN
}
//POSSIVEL ERRO

int _var::operador_normal_funcao(vector<_var>&vetor, int&vet_num, _reg reg, string nova_variavel, string &subline) {
    int found, aux, pos;
    string string_var, funcao;
    remove_space(subline);
    found = (int) subline.find_first_of("("); //alterar por expressao regular pois pode haver constantes no meio do codigo
    funcao = subline.substr(0, found);
    string_var = subline.substr(found, subline.length());
    remove_space(string_var);
    aux = reg.what_is_first_string(string_var);
    if (aux == REG_FUNCTION) {//doidera total rever essa coisa louca
        pos = operador_normal_funcao(vetor, vet_num, reg, nova_variavel, string_var);
        if (vetor[pos].vect[vetor[pos].n_var].funcao.empty())//no na cabeca preciso acessar o local correto para adicionar a funcao e agora?
            vetor[pos].vect[vetor[pos].n_var].funcao = "+";
        vetor[pos].vect[vetor[pos].n_var].funcao = funcao;
        return pos;
    } else if (aux == REG_VARIABLE) {
        pos = operador_normal_variavel(vetor, vet_num, reg, nova_variavel, string_var);
        return pos;
    }

}

string _var::primeira_string (string &line, _reg reg){
    string nova_string;
    int n, pos;
    n = reg.reg_exec_first_string(line, pos);
    nova_string = line.substr(pos, n);
    line = line.substr(n, line.length());
    remove_space(line);
    return nova_string;
}