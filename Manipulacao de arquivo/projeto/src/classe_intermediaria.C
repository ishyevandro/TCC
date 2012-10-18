#include <iostream>
#include "classe_intermediaria.h"

_intermediaria::_intermediaria() : reg(NULL), vet_num(0) {
    this->vetor_de_variaveis.resize(VET_NUM);
    this->reg = new _reg;
    this->aspas = new _aspas;
}

void display_error(string prnt) {
    if (DISPLAY_ERROR == 1)
        cout << prnt << endl;
}

//int _intermediaria::get_line_to_analyse(string line, vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg) {
//int _intermediaria::get_line_to_analyse(string line, int &vet_num, _reg reg) {

int _intermediaria::verifica_linha(string line) {
    int n;
    // n = remove_comments(line, reg);
    remove_space(line);
    n = reg->what_is_first_string(line);
    //cout << n<<"()" <<line<<endl;
    if (n == REG_VARIABLE)
        analisa_linha(line);
    else
        return -1;
    return 1;
}

void _intermediaria::remove_space(string &line) {
    size_t found;
    found = line.find_first_not_of(" ");
    if ((int) found < line.length())
        line = line.substr((int) found, line.length());
}

int _intermediaria::analisa_linha(string line) {
    int n, operator_type;
    string array_of_vars_in_array, nova_variavel;
    n = operator_type = 0;
    array_of_vars_in_array.clear();
    display_error("analyse_line");
    /*MONTA A EXPRESSAO REGULAR PARA INCLUIR AS VARIAVEIS QUE ESTAO COM POST E GET*/
    if (this->vet_num > 0)
        array_of_vars_in_array = string_of_var_to_reg(); //forma string para passar para verificacao de post ou get ou variavel que possui uma dessas coisas
    n = reg->mount_reg_get_or_post(line, array_of_vars_in_array); //compila e verifica se alguma variavel tem get ou post
    /*IF true caso tenha alguma variavel no vetor que possui get ou post*/
    //if (n == TRUE_VALUE) {
    nova_variavel = primeira_string(line);
    //pega a primeira string e retorna onde ela comeca em pos e retorna onde comeca operador em N
    if (!nova_variavel.empty()) {//##PORCARIA TODA ERRADA. ALTERAR A EXPRESSAO REGULAR PARA PROCURAR PELO $
        operator_type = type_of_operator(line); /*verifica qual o tipo de operador*/
        switch (operator_type) {/*acoes a serem feitas com cada operador*/
            case REG_OPERATOR_NORMAL:
                operador_normal(line, array_of_vars_in_array, nova_variavel);
                break;
            case REG_OPERATOR_CAT:
                // cout << "cat" << endl;
                operador_cat(line, array_of_vars_in_array, nova_variavel);
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
    //}
    //imprime_vetor();
    return 1;
}

string _intermediaria::string_of_var_to_reg(/*vector<_var> &vet, int size_array*/) {//construcao de expressao para verificar se as variaveis estao na linha
    string ret, caracter_especiais, aux_esp1, aux_esp2;
    int i = 0;
    size_t encontrado;
    ret.clear();
    display_error("string_of_var_to_reg ini");
    if (this->vet_num == 0)
        ret = "(\\$_POST*)|(\\$_GET*)";
    else
        ret = "(\\$_POST*)|(\\$_GET*)|";
    for (i = 0; i < this->vet_num; i++) {
        ret += "(\\";
        //ret += vet[i].variavel;
        caracter_especiais = this->vetor_de_variaveis[i].variavel;
        encontrado = caracter_especiais.find("[");
        if (encontrado != string::npos) {
            aux_esp1 = "\\[";
            aux_esp2 = "\\]";
            caracter_especiais.replace(caracter_especiais.find("["), 1, aux_esp1);
            caracter_especiais.replace(caracter_especiais.find("]"), 1, aux_esp2);
            ret += caracter_especiais;
        } else
            ret += this->vetor_de_variaveis[i].variavel;
        if (i + 1 != this->vet_num)
            ret += ")|";
        else
            ret += ")";
    }
    return ret;
}

string _intermediaria::primeira_string(string &line) {
    string nova_string;
    int n, pos, tipo_de_string;
    display_error("primeira_string");
    nova_string.clear();
    remove_space(line);
    tipo_de_string = this->reg->what_is_first_string(line);
    if (tipo_de_string == REG_VARIABLE) {
        return retorna_variavel(line);
    } else if (tipo_de_string == REG_FUNCTION)
        return retorna_funcao(line);
}

string _intermediaria::primeira_variavel(string &line) {
    string nova_string;
    int n, pos, tipo_de_string;
    display_error("primeira_variavel");
    nova_string.clear();
    remove_space(line);
    tipo_de_string = this->reg->what_is_first_string(line);
    if (tipo_de_string == REG_VARIABLE) {//#ERRADDO
        display_error("primeira_variavel fim VARIAVEL");
        return retorna_variavel(line);
    } else if (tipo_de_string == REG_FUNCTION) {
        display_error("primeira_variavel fim FUNCAO");
        nova_string = retorna_funcao(line);
        verifica_funcao(nova_string); //{analise no lugar errado}
    } else if (line[0] == '"') {
        display_error("primeira variavel fim ASPAS");
        nova_string = aspas_duplas(line);
    } else if (line[0] == '\'') {
        display_error("primeira variavel fim ASPAS");
        nova_string = aspas_simples(line);
    } else {//####RETORNOU UM NUMERO E AGORA??
        display_error("primeira_variavel fim NUMERO?");
        nova_string = "4";
        n = line.find_first_not_of("1234567890.");
        if (n >= 0 && n <= line.length())
            line = line.substr(n, line.length());
        return nova_string;
    }

    return nova_string;
}

string _intermediaria::retorna_variavel(string &line) {//RETORNA A PRIMEIRA STRING QUANDO E VARIAVEL
    int n, pos;
    string retorno;
    display_error("retorna_variavel");
    n = this->reg->reg_exec_first_string(line, pos);
    // cout<<pos<<"::::"<<n<<endl;
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
    if (!line.empty())
        remove_space(line);
    display_error("retorna_variavel fim");
    return retorno;
}

/*## FAZER VERIFICACAO DO POS POIS DARA ERRO DESSA FORMA*/
string _intermediaria::retorna_funcao(string &line) {
    string retorno;
    int pos_ini, pos_fim;
    display_error("retorna_funcao");
    remove_space(line);
    retorno.clear();
    pos_ini = this->reg->reg_verifica_parentese(line, REG_PARENTESE_I, 0);
    if (pos_ini != FALSE_VALUE) {
        pos_fim = retorna_elementos_dentro_de_parentese(line);
        retorno = line.substr(0, pos_fim);
        line = line.substr(pos_fim, line.length());
    }
    if (!line.empty())
        remove_space(line);
    display_error("retorna_funcao fim");
    return retorno;
}

string _intermediaria::aspas_duplas(string &line) {
    int n;
    string retorno;
    remove_space(line);
    n = this->reg->reg_verifica_aspasd(line);
    line = line.substr(n + 1, line.length());
    //cout<<"aspas duplas:"<<line<<endl;
    n = this->reg->reg_verifica_aspasd(line);
    if (n == FALSE_VALUE) {
        cout << "Nao possui outra aspas" << endl;
        exit(1);
    }
    retorno = '"';
    retorno += line.substr(0, n);
    retorno += '"';
    //cout<<line<<endl;
    line = line.substr(n + 1, line.length());
    //  cout << "aspas duplas line:" << line << endl;
    // cout << "ASPAS DUPLAS " << retorno << endl;

    return retorno;
}

string _intermediaria::aspas_simples(string &line) {
    int n;
    string retorno;
    remove_space(line);
    n = this->reg->reg_verifica_aspass(line);
    line = line.substr(n + 1, line.length());
    //cout<<"aspas duplas:"<<line<<endl;
    n = this->reg->reg_verifica_aspass(line);
    if (n == FALSE_VALUE && n == string::npos) {
        cout << "Nao possui outra aspas" << endl;
        exit(1);
    }
    retorno = '\'';
    retorno += line.substr(0, n);
    retorno += '\'';
    line = line.substr(n + 1, line.length());
    remove_space(line);
    return retorno;
}

/*VERIFICACAO DO TIPO DE OPERACAO*/
int _intermediaria::type_of_operator(string &line) {
    display_error("type_of_operator");
    int retorno;
    remove_space(line);
    retorno = this->reg->reg_to_operator(line);
    if (retorno == REG_OPERATOR_NORMAL)
        line = line.substr(1, line.length());
    else
        line = line.substr(2, line.length());
    return retorno;
}

int _intermediaria::operador_normal(string subline, string reg_vetor_de_variaveis, string nova_variavel) {//##reg_vetor excesso
    //##implementar para futuramente passar a string e nao a linha toda para analise futura.
    _var auxiliar_sublinha; //pega todas as iteracoes posterior ao operador e armazena aqui para no fim colocar no vetor. Feito dessa forma para nao cair em recursao de objetos quando uma variavel receber ela mesmo depois de receber alguma outra.
    display_error("operador_normal INICIO");
    auxiliar_sublinha.variavel = nova_variavel;
    auxiliar_sublinha.n_var = 0;
    pos_operador(subline, reg_vetor_de_variaveis, auxiliar_sublinha);
   // imprime_elemento(auxiliar_sublinha);
    display_error("operador_normal FIM");
}

int _intermediaria::operador_cat(string subline, string reg_vetor_de_variaveis, string nova_variavel) {
    _var nova_var;
    string reg_string_de_var_com_g_p;
    display_error("Operador_cat inicio");
    nova_var.variavel = nova_variavel;
    nova_var.n_var = 0;
    adiciona_get_ou_post(nova_var, nova_var.variavel, 0);
    pos_operador(subline, reg_vetor_de_variaveis, nova_var);
}

int _intermediaria::pos_operador(string &subline, string reg_vetor_de_variaveis, _var &nova_variavel) {
    int auxiliar, tipo, operacao, prox_operador, cast, ultima_variavel;
    string variavel_atual, vazia;
    remove_space(subline);
    auxiliar = TRUE_VALUE;
    vazia.empty();
    cast = FALSE_VALUE;
    variavel_atual = primeira_variavel(subline);
   // exit(1);
    ultima_variavel = 1;
    do {
        prox_operador = proximo_operador(subline);
        if (prox_operador == REG_OPERATOR_COMPARE) {
            remove_operador_intermediario(subline);
            remove_space(subline);
            if (subline[0] != ';') {
                operacao = operador_intermediario(subline);
                variavel_atual = primeira_variavel(subline);
            } else {
                variavel_atual.clear();
            }
        } else if (prox_operador == REG_POS_OPERATOR_MAT) {
            cast = 1;
        } else {
            if (cast != 1)
                cast = 0;
        }
        if (variavel_atual[0] == '"') {
            operador_aspas(nova_variavel, variavel_atual, cast);
        } else if (!variavel_atual.empty()) {
            //sera necessario implementar para verificar se essa variavel e concatenada... ou ate remover ela da posicao onde esta e caso tenha outra no vetor apagar a outra!
            auxiliar = reg->mount_reg_get_or_post(variavel_atual, reg_vetor_de_variaveis); //SE HOUVER UM WHILE AQUI DARA ERRO
            //    cout << auxiliar << endl;
            //cout << auxiliar << "variavel: " << variavel_atual << " linha:" << subline << endl;
            //    cout << variavel_atual << endl;
            tipo = reg->what_is_first_string(variavel_atual); //verifica se e variavel ou funcao
            if (auxiliar != FALSE_VALUE) {//verifica se o elemento atual tem get ou post.
                //2 tipo = reg->what_is_first_string(variavel_atual); //verifica se e variavel ou funcao
                if (tipo == REG_VARIABLE) {
                 //   cout << "FUNCIONANDO VARIAVEL!@#!@#!@#!@#!@#" << cast << endl;
                    operador_normal_variavel(nova_variavel, variavel_atual, cast);
                } else if (tipo == REG_FUNCTION) {
                //    cout << "FUNCIONANDO FUNCAO!@#!@#!@#!@#!@#" << cast << endl;
                    operador_normal_funcao(nova_variavel, variavel_atual, cast);
                }
            }
            //        if (tipo == REG_ASPAS) {
            //          variavel_atual = aspas_simples(subline);
            //        remove_space(subline);
            //      cout << subline << endl;
            //    exit(1);
            //}
        }
        //#
        if (subline[0] != ';') {//##OPERADOR DE PRECEDENCIA "( )" NECESSARIO VERIFICACAO QUANDO
            /*QUANDO DENTRO... POLONESA REVERSA?*/
            // cout << "&&&&&&" << subline << endl;
            operacao = operador_intermediario(subline); /*COMPLEMENTAR ESTA PARTE.*/
            if (operacao == REG_POS_OPERATOR_MAT)
                cast = 1;
            else
                cast = 0;
            variavel_atual = primeira_variavel(subline);
            remove_space(subline);
            //    cout << "ultima verificacao do laco" << variavel_atual << endl;
            ultima_variavel = 1;
        } else
            ultima_variavel = 0;
        //} else {//NAO SUBSTITUIR POR ";" CAMINHAR PARA A PROXIMA VARIAVEL para verificar os opeeradores.
        //   ultima_variavel = 0;
        //   subline = ";";
        // }subline[0] != ';' && 
    } while (ultima_variavel != 0);
    if (nova_variavel.n_var == 0) {//remove a variavel da lista #existe o problema de ele remover quando um $_get ou post recebe algo verificar isso
        cout << "Removendo variavel: " << nova_variavel.variavel << endl;
        remove_variavel_do_vetor(nova_variavel.variavel);
    } else if (nova_variavel.n_var > 0) {
        copia_var(nova_variavel);
    }
    return 0;
}

int _intermediaria::retorna_elementos_dentro_de_parentese(string line) {
    int pos_ini, pos_fim, i, j, pos, count;
    string retorno, auxiliar;
    retorno.clear();
    display_error("retorna_elementos_dentro_de_parentese");
    pos_ini = this->reg->reg_verifica_parentese(line, REG_PARENTESE_I, 0);
    if (pos_ini == FALSE_VALUE)
        return FALSE_VALUE;
    pos_fim = this->reg->reg_verifica_parentese(line, REG_PARENTESE_F, 1);
    if (pos_fim == FALSE_VALUE)
        return FALSE_VALUE;
    auxiliar = line;
    count = count_char(auxiliar, '(');
    i = j = 0;
    while (i < count) {
        if (line[j] == ')')
            i++;
        j++;
        if (j > line.length()) {
            return FALSE_VALUE;
        }
    }
    auxiliar = line.substr(0, j);
    display_error("retorna_elementos_dentro_de_parentese fim");
    return j;
}

int _intermediaria::proximo_operador(string line) {
    int tipo_de_operador;
    remove_space(line);
    //  cout<<"a linha antes do proximo:<<>>"<<line<<endl;
    tipo_de_operador = this->reg->reg_verifica_operador_compara(line);
    if (tipo_de_operador != FALSE_VALUE) {
        // cout << "Operador de comparacao" << endl;
        return REG_OPERATOR_COMPARE;
    }
    tipo_de_operador = this->reg->reg_operador_cat_ou_aritmetico(line);
    if (tipo_de_operador != FALSE_VALUE)
        return tipo_de_operador;
    else return FALSE_VALUE;

}

int _intermediaria::remove_operador_intermediario(string &line) {
    int n;
    string remove;
    n = this->reg->reg_remove_operador_compara(line);
    line = line.substr(n, line.length());
    primeira_variavel(line);
    return 1;
}

int _intermediaria::operador_intermediario(string &subline) {
    int aux;
    string aspas;
    display_error("operador_intermediario");
    remove_space(subline);
    aux = this->reg->reg_operador_cat_ou_aritmetico(subline);
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
        return FALSE_VALUE;
    }
}

int _intermediaria::operador_aspas(_var &nova_variavel, string &subline, int cast) {
    int n, k, aux;
    n = 1;
    string variavel, reg_vetor_de_variaveis;
    display_error("operador_aspas");
    //cout << subline;
    while (n != string::npos) {
        n = subline.find_first_of("$");
        if (n != string::npos) {
            subline = subline.substr(n, subline.length());
            k = subline.find_first_of(" }\"");
            if (k != string::npos) {
                variavel = subline.substr(0, k);
                // cout << "variavel aspas:" << variavel << endl;
                reg_vetor_de_variaveis = string_of_var_to_reg();
                aux = this->reg->mount_reg_get_or_post(variavel, reg_vetor_de_variaveis);
                if (aux != FALSE_VALUE) {
                    cout << "operador_normal_variavel pelo operador_aspas" << endl << subline;
                    operador_normal_variavel(nova_variavel, variavel, cast);

                }
            }
            subline = subline.substr(k + 1, subline.length());
            // cout << "OPerador aspas:" << subline << endl;
            // exit(1);
        }
    }
    //ALTERAR ESSE RETURN
}

int _intermediaria::operador_normal_variavel(_var &nova_variavel, string &subline, int cast) {
    string reg_string_de_var_com_g_p;
    display_error("operador_normal_variavel");
    remove_space(subline);
   // cout << "cast" << cast << endl;
    return adiciona_get_ou_post(nova_variavel, subline, cast);
    //ALTERAR ESSE RETURN
}

int _intermediaria::operador_normal_funcao(_var &nova_variavel, string &string_atual, int cast) {
    int aux, pos, retorno, i;
    string string_func, funcao;
    display_error("operador_normal_funcao");
    remove_space(string_atual);
    retorno = 0;
    if (string_atual[0] == '(')
        string_atual = string_atual.substr(1, string_atual.length());

    if (verifica_funcao(string_atual) == FALSE_VALUE) {
        remove_space(string_atual);
        aux = reg->what_is_first_string(string_atual);
        if (aux == REG_FUNCTION) {//doidera total rever essa coisa louca
            funcao = retorna_funcao(string_atual);
            pos = funcao.find_first_of("(");
            if (pos >= 0 && pos <= funcao.length()) {
                string_func = funcao.substr(0, pos);
                string_atual = funcao.substr(pos, funcao.length());
            }
            retorno = operador_normal_funcao(nova_variavel, string_atual, cast);
            if (retorno == 1) {
                string_func = string_func + "+";
                for (i = this->n_funcao ; i < nova_variavel.n_var; i++) {

                    // cout << string_func << endl;
                    nova_variavel.vect[i].funcao += string_func;
                }
                this->n_funcao = 0;
                display_error("operador_normal_funcao fim");
                return retorno;
            }
        } else if (aux == REG_VARIABLE) {//##adicao de aspas nesse ponto. e perigoso a forma que esta sendo usada ate o momento
          //  cout << "FUNCIONANDO!@#!@#!@#!@#!@#" << cast << endl;
            string_atual = primeira_variavel(string_atual);
            this->n_funcao = nova_variavel.n_var;
            retorno = operador_normal_variavel(nova_variavel, string_atual, cast);
            display_error("operador_normal_funcao fim");
            return retorno;
        }
    }
    display_error("operador_normal_funcao fim");
    return retorno;
}

int _intermediaria::remove_variavel_do_vetor(string var) {
    int i, pos;
    display_error("remove_variavel_do_vetor");
    pos = procura_variavel_no_vetor(var);
    if (pos == FALSE_VALUE) {
        cout << "variavel nao encontrada no vetor" << endl;
        return -1;
    }
    for (i = pos; i < vet_num - 1; i++) {
        vetor_de_variaveis[i].variavel = vetor_de_variaveis[i + 1].variavel;
        vetor_de_variaveis[i].n_var = vetor_de_variaveis[i + 1].n_var;
        vetor_de_variaveis[i].vect = vetor_de_variaveis[i + 1].vect;
    }
    vetor_de_variaveis[vet_num - 1].variavel.clear();
    vetor_de_variaveis[vet_num - 1].n_var = 0;
    vet_num--;
    return 1;
}

int _intermediaria::copia_var(_var nova_variavel) {
    int pos_var, i, j;
    display_error("copia_var");
    pos_var = procura_variavel_no_vetor(nova_variavel.variavel);
    if (pos_var == FALSE_VALUE)
        pos_var = add_var_array(nova_variavel.variavel);

    this->vetor_de_variaveis[pos_var].variavel = nova_variavel.variavel;
    this->vetor_de_variaveis[pos_var].n_var = nova_variavel.n_var;
    for (i = 0; i < nova_variavel.n_var; i++) {
        this->vetor_de_variaveis[pos_var].vect[i].variavel = nova_variavel.vect[i].variavel;
        this->vetor_de_variaveis[pos_var].vect[i].funcao = nova_variavel.vect[i].funcao;
        this->vetor_de_variaveis[pos_var].vect[i].cast = nova_variavel.vect[i].cast;
    }
    return 1;
}

int _intermediaria::count_char(string line, char count) {
    int i, contador;
    contador = 0;
    for (i = 0; i < line.length(); i++) {
        if (line[i] == count)
            contador++;
    }
    return contador;
}

int _intermediaria::adiciona_get_ou_post(_var &add, string subline, int cast) {//vai ser adicionado a excessao de aspas simples aqui
    int pos, n;
    string string_guardar, vet_var_p_g;
    display_error("adiciona_get_ou_post");
    remove_space(subline);
    vet_var_p_g = string_of_var_to_reg();
   // cout << "adiciona_get: " << cast << endl;
    if (reg->reg_segunda_parte_linha(subline) == TRUE_VALUE) {//verifica se o primeiro elemento 'e um get ou post
        /*#COM MUDANCAS FUTURAS NAO SERA MAIS NECESSARIO USAR ESSA PARTE*/
        // cout << "_________======" << subline << endl;
        string_guardar = subline; //retorna_variavel(subline, reg);
        //       string_guardar += " ";
        add.vect[add.n_var].variavel = string_guardar;
        add.vect[add.n_var].cast = cast;
        add.n_var++;
        return 1;
        //subline = subline.substr((int) subline.find_first_of("]") + 1, subline.length());
    } else if (!vet_var_p_g.empty() && reg->mount_reg_get_or_post(subline, vet_var_p_g) != FALSE_VALUE) {
        string_guardar = subline; //retorna_variavel(subline, reg);
        pos = procura_variavel_no_vetor(string_guardar);
        for (n = 0; n < this->vetor_de_variaveis[pos].n_var; n++) {//arrumar o cast
            add.vect[add.n_var].cast = this->vetor_de_variaveis[pos].vect[n].cast == 1 ? this->vetor_de_variaveis[pos].vect[n].cast : cast;
            add.vect[add.n_var].variavel = this->vetor_de_variaveis[pos].vect[n].variavel;
            add.vect[add.n_var].funcao = this->vetor_de_variaveis[pos].vect[n].funcao;
            add.n_var++;
        }
        //  cout << "Adiciona_GET ==========" << add.variavel << " " << add.n_var;
        return 1;
    } else {//SE TA LOCAO?!
        cout << "Nao possui get nem post" << endl;
        //  cout << subline << endl;
        return -1;
    }
}

int _intermediaria::procura_variavel_no_vetor(string var) {
    int i;
    display_error("procura_variavel_no_vetor");
    for (i = 0; i < this->vet_num; i++) {
        if (var.compare(this->vetor_de_variaveis[i].variavel) == 0) {
            return i;
        }
    }
    return -1;
}

int _intermediaria::add_var_array(string new_string) {//insercao de variavel no vetor de variaveis
    display_error("add_var_array");
    this->vetor_de_variaveis[this->vet_num].variavel = new_string;
    this->vet_num++;
    return this->vet_num - 1;
}

int _intermediaria::verifica_funcao(string funcao) {
    string func;
    string matriz[10];
    int retorno = FALSE_VALUE;
    int i;
    matriz[1] = "mysql_query";
    matriz[2] = "pg_query";
    matriz[3] = "pg_query_params";
    matriz[4] = "pg_prepare";
    matriz[5] = "exec";
    matriz[6] = "passthru";
    matriz[7] = "shell_exec";
    matriz[8] = "system";
    int n;
    remove_space(funcao);
    n = funcao.find_first_of(" (");
    if (n != string::npos) {
        func = funcao.substr(0, n);
        //  cout << func << endl;
    }
    for (i = 1; i <= 8; i++) {//corrigir. vetores comecam do zero e nao do um
        if (func.compare(matriz[i]) == 0) {
            cout << func << "e uma funcao que necessita de seguranca" << endl;
            retorno = 1;
        }
    }
    return retorno;
}

void _intermediaria::imprime_vetor() {
    int i, j;
    cout << "IMPRESSAO DO VETOR" << endl << endl;
    for (i = 0; i < this->vet_num; i++) {
        cout << this->vetor_de_variaveis[i].variavel << " N parametros: " << this->vetor_de_variaveis[i].n_var << endl;
        for (j = 0; j < this->vetor_de_variaveis[i].n_var; j++) {
            cout << "===" << this->vetor_de_variaveis[i].vect[j].variavel << "." << this->vetor_de_variaveis[i].vect[j].cast << endl;
            if (!this->vetor_de_variaveis[i].vect[j].funcao.empty())
                cout << "\t===" << this->vetor_de_variaveis[i].vect[j].funcao << "." << endl;
        }
    }
    cout << "??????" << endl;

}

void _intermediaria::imprime_elemento(_var elemento) {
    int j;
   
    cout << "IMPRESSAO DE ELEMENTO" << endl << endl;
    cout << elemento.variavel << " N parametros: " << elemento.n_var << endl;
    for (j = 0; j < elemento.n_var; j++) {
        cout << "===" << elemento.vect[j].variavel << "." << elemento.vect[j].cast << endl;
        if (!elemento.vect[j].funcao.empty())
            cout << "\t===" << elemento.vect[j].funcao << "." << endl;
    }
    cout << "??????" << endl;
}