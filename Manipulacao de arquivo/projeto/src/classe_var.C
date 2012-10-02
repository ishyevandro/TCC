#include <iostream>
#include "classe_var.h"
#include "variavel.h"

void display_error(string prnt) {
    if (DISPLAY_ERROR == 1)
        cout << prnt << endl;
}

/*#NECESSARIO ALTERACAO DA BUSCA DE POST E GET... BUSCANDO POR EXPRESSAO REGULAR PARA RETIRAR
 O ESPCAO ENTRE O $_GET   (ESTE ESPACO)[*/

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
    display_error("add_var_array");
    vet[size_array].variavel = new_string;
    size_array++;
    return size_array - 1;
}

/*FORMA A STRING DE VARIAVEIS PARA FORMACAO DE EXPRESSAO REGULAR*/
string _var::string_of_var_to_reg(vector<_var> &vet, int size_array) {//construcao de expressao para verificar se as variaveis estao na linha
    string ret, caracter_especiais, aux_esp1, aux_esp2;
    int i = 0;
    size_t encontrado;
    ret.clear();
    display_error("string_of_var_to_reg ini");
    if (size_array == 0)
        ret = "(\\$_POST*)|(\\$_GET*)";
    else
        ret = "(\\$_POST*)|(\\$_GET*)|";
    for (i = 0; i < size_array; i++) {
        ret += "(\\";
        //ret += vet[i].variavel;
        caracter_especiais = vet[i].variavel;
        encontrado = caracter_especiais.find("[");
        if (encontrado != string::npos) {
            aux_esp1 = "\\[";
            aux_esp2 = "\\]";
            caracter_especiais.replace(caracter_especiais.find("["), 1, aux_esp1);
            caracter_especiais.replace(caracter_especiais.find("]"), 1, aux_esp2);
            ret += caracter_especiais;
        } else
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
    display_error("string_of_var_to_reg ini+par");
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
    string array_of_vars_in_array, nova_variavel;
    n = operator_type = 0;
    array_of_vars_in_array.clear();
    display_error("analyse_line");
    //cout<<line<<endl;
    /*MONTA A EXPRESSAO REGULAR PARA INCLUIR AS VARIAVEIS QUE ESTAO COM POST E GET*/
    if (vet_num > 0)
        array_of_vars_in_array = string_of_var_to_reg(vetor_de_variaveis, vet_num); //forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas
    n = reg.mount_reg_get_or_post(line, array_of_vars_in_array); //compila e verifica se alguma variavel tem get ou post
    /*IF true caso tenha alguma variavel no vetor que possui get ou post*/
    cout<<n<<"()"<<line<<endl;
    if (n == TRUE_VALUE) {
        nova_variavel = primeira_string(line, reg);
        //pega a primeira string e retorna onde ela comeca em pos e retorna onde comeca operador em N
        if (!nova_variavel.empty()) {//##PORCARIA TODA ERRADA. ALTERAR A EXPRESSAO REGULAR PARA PROCURAR PELO $
            //cout<<"<<"<<line<<endl;
            operator_type = type_of_operator(line, reg); /*verifica qual o tipo de operador*/
            switch (operator_type) {/*acoes a serem feitas com cada operador*/
                case REG_OPERATOR_NORMAL:
                    //cout << "QUE ERRO DO DEMONIO  E ESSE?"<<line<<endl;
                    operador_normal(vetor_de_variaveis, vet_num, reg, line, array_of_vars_in_array, nova_variavel);
                    break;
                case REG_OPERATOR_CAT:
                    cout << "??concatena??" << endl;
                    break;
                    //case REG_OPERATOR_SOMA REG_OPERATOR_SUB REG_OPERATOR_DIV REG_OPERATOR_RES:;
                default:
                    cout << "operadores de some aprender a usar switch" << endl;
                    break;
            }
            remove_space(line);
        } else {//quando for objeto e estiver sendo chamado uma funcao do mesmo
            cout << "E UMA FUNCAO E AINDA NAO TRATEI NA PRIMEIRA_STRING" << endl;
        }
    }
    return 1;
}

void _var::remove_space(string &line) {
    size_t found;
    display_error("remove_space");
    if (!line.empty()) {
        found = line.find_first_not_of(" ");
        if (found>=0 && found<= line.length())
                line = line.substr((int) found, line.length());
    }
}

void _var::remove_aspa_simples(string &subline) {
    size_t found;
    display_error("remove_aspas_simples");
    do {
        found = subline.find_first_not_of(" ");
        subline = subline.substr((int) found, subline.length());
    } while (1 != 1);
}

/*VERIFICACAO DO TIPO DE OPERACAO*/
int _var::type_of_operator(string &line, _reg reg) {
    display_error("type_of_operator");
    int retorno;
    remove_space(line);
    retorno = reg.reg_to_operator(line);
    line = line.substr(1, line.length());
    return retorno;
}

/*IMPRESSAO DO VETOR PARA TESTE*/
void _var::imprime_vetor(vector<_var> vet, int tam) {
    int i, j;
    cout << "IMPRESSAO DO VETOR" << endl << endl;
    for (i = 0; i < tam; i++) {
        cout << vet[i].variavel << " N parametros: " << vet[i].n_var << endl;
        for (j = 0; j < vet[i].n_var; j++) {
            cout << "===" << vet[i].vect[j].variavel << "." << endl;
            if (!vet[i].vect[j].funcao.empty())
                cout << "\t===" << vet[i].vect[j].funcao << "." << endl;
        }
    }
    cout << "??????" << endl;

}

int _var::procura_variavel_no_vetor(string var, vector<_var> &vetor_de_variaveis, int &vet_num) {
    int i;
    display_error("procura_variavel_no_vetor");
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
    display_error("remove_variavel_do_vetor");
    pos = procura_variavel_no_vetor(var, vetor_de_variaveis, vet_num);
    if (pos == FALSE_VALUE) {
        cout << "variavel nao encontrada no vetor" << endl;
        return -1;
    }
    for (i = pos; i < vet_num - 1; i++) {
        vetor_de_variaveis[i].variavel = vetor_de_variaveis[i + 1].variavel;
        vetor_de_variaveis[i].vect = vetor_de_variaveis[i + 1].vect;
    }
    vetor_de_variaveis[vet_num - 1].variavel.clear();
    vetor_de_variaveis[vet_num - 1].n_var = 0;
    vet_num--;
    return 1;
}

int _var::adiciona_get_ou_post(_var &add, string &subline, _reg reg, string vet_var_p_g, vector<_var> procura, int vet_num) {//vai ser adicionado a excessao de aspas simples aqui
    int pos, n;
    string string_guardar;
    display_error("adiciona_get_ou_post");
    //remove_space(subline);
    if (1 == 1) {//implementar a diferenca entre get direto e htmlspecial(get);
        if (reg.reg_segunda_parte_linha(subline) == TRUE_VALUE) {//verifica se o primeiro elemento 'e um get ou post
            /*#COM MUDANCAS FUTURAS NAO SERA MAIS NECESSARIO USAR ESSA PARTE*/
            // cout << "_________======" << subline << endl;
            string_guardar = subline; //retorna_variavel(subline, reg);
            //       string_guardar += " ";
            add.vect[add.n_var].variavel = string_guardar;
            add.n_var++;
            return 1;
            //subline = subline.substr((int) subline.find_first_of("]") + 1, subline.length());
        } else if (!vet_var_p_g.empty() && reg.mount_reg_get_or_post(subline, vet_var_p_g) != FALSE_VALUE) {
            string_guardar = subline; //retorna_variavel(subline, reg);
            pos = procura_variavel_no_vetor(string_guardar, procura, vet_num);
            for (n = 0; n < procura[pos].n_var; n++) {
                add.vect[add.n_var].variavel = procura[pos].vect[n].variavel;
                add.vect[add.n_var].funcao = procura[pos].vect[n].funcao;
                add.n_var++;
            }
            return 1;
        } else {//SE TA LOCAO?!
            cout << "Se nao tem get nem variavel sei lah!!" << endl;
            cout << subline << endl;
            return -1;
        }
    }
}

int _var::operador_normal(vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg, string subline, string reg_vetor_de_variaveis, string nova_variavel) {
    int auxiliar, tipo, operacao, cast, ultima_variavel;
    string reg_string_de_var_com_g_p;
    string variavel_atual; //##implementar para futuramente passar a string e nao a linha toda para analise futura.
    _var auxiliar_sublinha; //pega todas as iteracoes posterior ao operador e armazena aqui para no fim colocar no vetor. Feito dessa forma para nao cair em recursao de objetos quando uma variavel receber ela mesmo depois de receber alguma outra.
    auxiliar_sublinha.variavel = nova_variavel;
    auxiliar_sublinha.n_var = 0;
    display_error("operador_normal");
    remove_space(subline);
    auxiliar = TRUE_VALUE;
    //cout <<subline<<endl;
    variavel_atual = primeira_variavel(subline, reg);
    /*auxiliar = reg.mount_reg_get_or_post(variavel_atual, reg_vetor_de_variaveis); /*#ESTA ERRADO!
    QUANDO A VARIAVEL QUE RECEBE E UM GET OU POST DA ERRO, VERIFICAR NA E2 SE GET OU POST CONSEGUEM RECEBER
    TAMBEM HA A NECESSIDADE DE ALTERAR A FORMA QUE ESTA SENDO FEITO. A VERIFICACAO DEVE SER DE AVARIAVEL A VARIAVEL */
    ultima_variavel = 1;
    if (auxiliar != FALSE_VALUE) {
        while (subline[0] != ';' || ultima_variavel != 0) {
            //sera necessario implementar para verificar se essa variavel e concatenada... ou ate remover ela da posicao onde esta e caso tenha outra no vetor apagar a outra!
            /*#NECESSARIO ARMAZENAR A PRIMEIRA STRING, APOS O OPERADOR, PARA FAZER AS FUTURAS ANALISES COM BASE APENAS NA VARIAVEL
             QUE ESTARA SENDO VERIFICADA DESSAA VEZ.*/
            //cout << "VAR ATUAL " << variavel_atual<<"NA SUBLINHA"<<subline << endl;
            auxiliar = reg.mount_reg_get_or_post(variavel_atual, reg_vetor_de_variaveis); //SE HOUVER UM WHILE AQUI DARA ERRO

            if (auxiliar != FALSE_VALUE) {//verificar se ainda ha algum elemento com get ou post esta errado, precisa ser feito verificacao de variavel por variavel ate o final
                tipo = reg.what_is_first_string(variavel_atual); //verifica se e variavel ou funcao
                //cout<<tipo<<subline<<endl;
                if (tipo == REG_VARIABLE) {
                    //cout<<variavel_atual<<endl;
                    operador_normal_variavel(vetor_de_variaveis, vet_num, reg, auxiliar_sublinha, variavel_atual);
                } else if (tipo == REG_FUNCTION) {
                  //  cout << "AQUI" << endl;
                    operador_normal_funcao(vetor_de_variaveis, vet_num, reg, auxiliar_sublinha, variavel_atual);
                    //remove_space(subline);
                    //subline = subline.substr(1, subline.length());
                    //remove_space(subline);
                }

                //#
                if (subline[0] != ';') {//##OPERADOR DE PRECEDENCIA "( )" NECESSARIO VERIFICACAO QUANDO
                    /*QUANDO DENTRO... POLONESA REVERSA?*/
                    // cout << "&&&&&&" << subline << endl;
                    operacao = operador_intermediario(subline, reg); /*COMPLEMENTAR ESTA PARTE.*/
                    if (operacao == REG_POS_OPERATOR_MAT)
                        cast = operacao;
                    variavel_atual = primeira_variavel(subline, reg);
                    remove_space(subline);
                    ultima_variavel = 1;

                } else
                    ultima_variavel = 0;
            } else {//NAO SUBSTITUIR POR ";" CAMINHAR PARA A PROXIMA VARIAVEL para verificar os opeeradores.
                ultima_variavel = 0;
                subline = ";";
            }
        }
    } else {//remove a variavel da lista #existe o problema de ele remover quando um $_get ou post recebe algo verificar isso
        cout << "Removendo variavel: " << nova_variavel << endl;
        remove_variavel_do_vetor(nova_variavel, vetor_de_variaveis, vet_num);
    }
    if (auxiliar_sublinha.n_var > 0) {
        copia_var(auxiliar_sublinha, vetor_de_variaveis, vet_num);
    }
    return 0;
}

/*
int _var::operador_concatena(vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg, string subline, string reg_vetor_de_variaveis, string nova_variavel){

}
int _var::operador_matematico(vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg, string subline, string reg_vetor_de_variaveis, string nova_variavel){

}*/
int _var::operador_intermediario(string &subline, _reg reg) {
    int aux;
    display_error("operador_intermediario");
    remove_space(subline);
    aux = reg.reg_operador_cat_ou_aritmetico(subline);
    if (aux == REG_POS_OPERATOR_CAT) {
        //  cout << "concatena _" << subline << endl;
        subline = subline.substr(1, subline.length());
        remove_space(subline);
        return REG_POS_OPERATOR_CAT;
    } else if (aux == REG_POS_OPERATOR_MAT) {
        //  cout << "matematico _" << subline << endl;
        subline = subline.substr(1, subline.length());

        remove_space(subline);
        return REG_POS_OPERATOR_MAT;
    } else {
        cout << "NAO E MATEMATICO NEM CONCATENA: " << subline << endl;
        remove_space(subline);
        return -1;
    }
}

int _var::operador_normal_variavel(vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg, _var &nova_variavel, string &subline) {
    string reg_string_de_var_com_g_p;
    display_error("operador_normal_variavel");
    remove_space(subline);
    reg_string_de_var_com_g_p = string_of_var_to_reg(vetor_de_variaveis, vet_num, 0);
    return adiciona_get_ou_post(nova_variavel, subline, reg, reg_string_de_var_com_g_p, vetor_de_variaveis, vet_num);
    //ALTERAR ESSE RETURN
}
//POSSIVEL ERRO
//Alteracao nessa funcao para implementacoes futuras uma vez que neste projeto so esta sendo averiguado as funcoes basicas do proprio php
int _var::operador_normal_funcao(vector<_var>&vetor, int&vet_num, _reg reg, _var &nova_variavel, string &subline) {
    int found, aux, pos, retorno;
    string string_func, funcao;
    display_error("operador_normal_funcao");
    remove_space(subline);
    retorno = 0;
    if (subline[0] == '(')
        subline = subline.substr(1, subline.length());
    //cout << subline << ": O.N.F" << endl;
    remove_space(subline);
    aux = reg.what_is_first_string(subline);
    if (aux == REG_FUNCTION) {//doidera total rever essa coisa louca
        funcao = retorna_funcao(subline, reg);
        pos = funcao.find_first_of("(");
        if (pos >= 0 && pos <= funcao.length()) {
            string_func = funcao.substr(0, pos);
           // cout << string_func << endl;
           // cout << funcao.substr(pos, funcao.length()) << endl;
            subline = funcao.substr(pos, funcao.length());
        }
        retorno = operador_normal_funcao(vetor, vet_num, reg, nova_variavel, subline);
       // cout<<retorno<<"> RETORNO"<<endl;
        if (retorno ==1){
            if (!nova_variavel.vect[n_var].funcao.empty())//no na cabeca preciso acessar o local correto para adicionar a funcao e agora?
                nova_variavel.vect[n_var].funcao += "+";
          //  cout<< nova_variavel.vect[n_var].funcao<<endl;
            nova_variavel.vect[n_var].funcao += string_func;
          //  cout << string_func << endl;
        display_error("operador_normal_funcao fim");
        return retorno;
        }
    } else if (aux == REG_VARIABLE) {//##adicao de aspas nesse ponto. e perigoso a forma que esta sendo usada ate o momento
        subline = primeira_variavel(subline, reg);
        retorno = operador_normal_variavel(vetor, vet_num, reg, nova_variavel, subline);
        display_error("operador_normal_funcao fim");
        return retorno;
    }
    display_error("operador_normal_funcao fim");
    return retorno;
}

/*#FUNCAO UTILIZADA PARA PEGAR A STRING ANTES DE QUALQUER OPERADOR
  POSSIVELMENTE SERA UMA VARIAVEL POREM E NECESSARIO ALTERAR O METODO ATUAL
 PARA VERIFICAR SE E UMA VARIAVEL OU NAO*/
string _var::primeira_string(string &line, _reg reg) {
    string nova_string;
    int n, pos, tipo_de_string;
    display_error("primeira_string");
    nova_string.clear();
    remove_space(line);
    tipo_de_string = reg.what_is_first_string(line);
    if (tipo_de_string == REG_VARIABLE) {
        return retorna_variavel(line, reg);
    } else if (tipo_de_string == REG_FUNCTION)
        return retorna_funcao(line, reg);
}

string _var::primeira_variavel(string &line, _reg reg) {
    string nova_string;
    int n, pos, tipo_de_string;
    display_error("primeira_variavel");
    nova_string.clear();
    remove_space(line);
    tipo_de_string = reg.what_is_first_string(line);
    //cout << endl << endl << line << endl << endl;
    if (tipo_de_string == REG_VARIABLE) {//#ERRADDO
        display_error("primeira_variavel fim VARIAVEL");
        return retorna_variavel(line, reg);
    } else if (tipo_de_string == REG_FUNCTION) {
        display_error("primeira_variavel fim FUNCAO");
        nova_string = retorna_funcao(line, reg);
    } else {//####RETORNOU UM NUMERO E AGORA??
        display_error("primeira_variavel fim NUMERO?");
        return nova_string;
    }// nova_string += "(";
    // nova_string += retorna_variavel(line, reg);
    // line = line.substr(1, line.length());
    // nova_string += ") ";
   // cout << nova_string << "P.V" << endl;
   // cout << line << "P.V" << endl;

    return nova_string;
}

int _var::copia_var(_var nova_variavel, vector<_var> &vetor_de_variaveis, int &vet_num) {
    int pos_var, i, j;
    display_error("copia_var");
    pos_var = procura_variavel_no_vetor(nova_variavel.variavel, vetor_de_variaveis, vet_num);
    if (pos_var == FALSE_VALUE)
        pos_var = add_var_array(vetor_de_variaveis, vet_num, nova_variavel.variavel);

    vetor_de_variaveis[pos_var].variavel = nova_variavel.variavel;
    vetor_de_variaveis[pos_var].n_var = nova_variavel.n_var;
    for (i = 0; i < nova_variavel.n_var; i++) {
        vetor_de_variaveis[pos_var].vect[i].variavel = nova_variavel.vect[i].variavel;
        vetor_de_variaveis[pos_var].vect[i].funcao = nova_variavel.vect[i].funcao;
    }
    return 1;
}

/*#NAO E USADO*/
string _var::retorna_gp(string &line) {//RETORNA A PRIMEIRA STRING QUANDO ELA E GET OU POST
    size_t found;
    string retorno;
    display_error("retorna_gp");
    found = line.find_first_of("]");
    retorno = line.substr(0, (int) found + 1);
    line = line.substr((int) found + 1, line.length());
    return retorno;
}

string _var::retorna_variavel(string &line, _reg reg) {//RETORNA A PRIMEIRA STRING QUANDO E VARIAVEL
    int n, pos;
    string retorno;
    display_error("retorna_variavel");
    n = reg.reg_exec_first_string(line, pos);
    retorno = line.substr(pos, n);
    line = line.substr(n, line.length());
    remove_space(line);
    if (line[0] == '[') {
        //reg.count_string(line, "[*]");
        retorno += "['";
        line = line.substr(1, line.length());
        n = (int) line.find_first_of("'");
        line = line.substr(n + 1, line.length());
        n = (int) line.find_first_of("'");
        retorno += line.substr(0, n);
        retorno += "']";
        line = line.substr(line.find_first_of("]") + 1, line.length());
    }
    //cout<<"saiu "<<line.length()<<endl;
    if (!line.empty())
        remove_space(line);
    // cout<<line<<endl;
    display_error("retorna_variavel fim");
    return retorno;
}

/*## FAZER VERIFICACAO DO POS POIS DARA ERRO DESSA FORMA*/
string _var::retorna_funcao(string &line, _reg reg) {
    string retorno;
    int pos_ini, pos_fim;
    display_error("retorna_funcao");
    remove_space(line);
    retorno.clear();
    //cout << line << "R.F line inicio" << endl;
    pos_ini = reg.reg_verifica_parentese(line, REG_PARENTESE_I, 0);
    if (pos_ini != FALSE_VALUE) {
        pos_fim = retorna_elementos_dentro_de_parentese(line, reg);
        retorno = line.substr(0, pos_fim);
      //  cout << line << ": R.F" << endl;
        line = line.substr(pos_fim, line.length());
    }
    /*cout<<"resto: "<<line<<endl;
    cout<<"RETORNO: "<<retorno<<endl;
    exit(1);
    if (pos_fim > 0)
        retorno = line.substr(0, pos_fim);
    else
        return NULL;
    line = line.substr(pos_fim, line.length());
    remove_space(line);
    if (line[0] == '(')
        line = line.substr(1, line.length());
    else
        return NULL;*/
    if (!line.empty())
        remove_space(line);
    display_error("retorna_funcao fim");
    return retorno;
}

int _var::retorna_elementos_dentro_de_parentese(string line, _reg reg) {
    int pos_ini, pos_fim, i, j, pos, count;
    string retorno, auxiliar;
    retorno.clear();
    display_error("retorna_elementos_dentro_de_parentese");
    pos_ini = reg.reg_verifica_parentese(line, REG_PARENTESE_I, 0);
    if (pos_ini == FALSE_VALUE)
        return FALSE_VALUE;
    //line = line.substr(pos_ini, line.length());
    pos_fim = reg.reg_verifica_parentese(line, REG_PARENTESE_F, 1);
    ///////cout<<pos_fim<<line[pos_fim]<<endl;
    if (pos_fim == FALSE_VALUE)
        return FALSE_VALUE;
    auxiliar = line; //.substr(0, pos_fim);
    count = count_char(auxiliar, '(');
    //cout<<count<<endl;
    i = j = 0;
    while (i < count) {
        if (line[j] == ')')
            i++;
        j++;
        if (j > line.length()) {
            // cout<<"Err"<<endl;
            return FALSE_VALUE;
        }
    }
    auxiliar = line.substr(0, j);
    // cout<<auxiliar<<endl;
    display_error("retorna_elementos_dentro_de_parentese fim");
    return j;
}

int _var::count_char(string line, char count) {
    int i, contador;
    contador = 0;
    //cout <<"ERROR"<<endl;
    for (i = 0; i < line.length(); i++) {
        if (line[i] == count)
            contador++;
    }
    return contador;
}