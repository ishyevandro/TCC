#include <iostream>
#include <cstdio>
#include "classe_linha.h"

_linha::_linha() : irmao(NULL), pai(NULL), filho(NULL), reg(NULL), vet_num(0), colchete(0), dois_pontos(0) {
    this->vetor_de_variaveis.resize(VET_NUM);
    this->reg = new _reg;
    this->id_irmao_original = 0;
    // this->aspas = new _aspas;
    this->no = "Raiz";
    this->conexao = new BANCOSL(DB_NAME);
}

_linha::~_linha() {
    delete this->reg;
    this->no.clear();
}

void _linha::display_error(string prnt) {
    if (DISPLAY_ERROR == 1)
        cout << this->no << prnt << endl;
}

int _linha::verifica_linha(string line, string &debug, int irmao) {
    int n;
    cout << this->no << endl;
    cout << line << endl;
    if (this->filho != NULL)
        this->filho->verifica_linha(line, debug, 1);
    if (irmao == 1) {
        if (this->irmao != NULL)
            this->irmao->verifica_linha(line, debug, 1);
    }
    remove_space(line);
    n = reg->what_is_first_string(line);
    if (n == REG_VARIABLE) {
        analisa_linha(line);
    } else {
        cout << "FUNCAO DIRETO" << endl;
        return -1;
    }
    debug += this->seguranca_retorno;
    this->seguranca_retorno.clear();
    return 1;
}

void _linha::remove_space(string &line) {
    size_t found;
    found = line.find_first_not_of(" \t");
    if (found != string::npos)
        line = line.substr((int) found, line.length());
    else
        line.clear();
}

int _linha::analisa_linha(string line) {
    int operator_type;
    string /*2array_of_vars_in_array,*/ nova_variavel;
    operator_type = 0;

    display_error("analyse_line");

    nova_variavel = primeira_string(line);

    if (!nova_variavel.empty()) {//##PORCARIA TODA ERRADA. ALTERAR A EXPRESSAO REGULAR PARA PROCURAR PELO $
        operator_type = type_of_operator(line); /*verifica qual o tipo de operador*/
        switch (operator_type) {/*acoes a serem feitas com cada operador*/
            case REG_OPERATOR_NORMAL:
                //  cout << "nor" << endl;
                operador_normal(line/*2, array_of_vars_in_array*/, nova_variavel);
                break;
            case REG_OPERATOR_CAT:
                //   cout << "cat" << endl;
                operador_cat(line/*2, array_of_vars_in_array,*/, nova_variavel);
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

string _linha::string_of_var_to_reg() {//construcao de expressao para verificar se as variaveis estao na linha
    string ret, caracter_especiais, aux_esp1, aux_esp2;
    int i = 0;
    size_t encontrado;
    ret.clear();
    display_error("string_of_var_to_reg ini");
    if (this->vet_num == 0)
        ret = "(\\$_POST)|(\\$_GET)";
    else
        ret = "(\\$_POST)|(\\$_GET)|";
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
            ret += "$)|";
        else
            ret += "$)";
    }
    return ret;
}

string _linha::primeira_string(string &line) {
    string nova_string;
    int tipo_de_string;
    tipo_de_string = 0;
    display_error("primeira_string");
    nova_string.clear();
    remove_space(line);
    tipo_de_string = this->reg->what_is_first_string(line);
    if (tipo_de_string == REG_VARIABLE) {
        return retorna_variavel(line);
    } else if (tipo_de_string == REG_FUNCTION)
        return retorna_funcao(line);
    else
        return nova_string;
}

string _linha::primeira_variavel(string &line, int &cast, _var &nova_variavel) {
    string nova_string;
    int n, pos, tipo_de_string, parentese, prox;

    n = pos = tipo_de_string = 0;
    display_error("primeira_variavel");
    nova_string.clear();
    do {
        parentese = FALSE_VALUE;
        remove_space(line);
        tipo_de_string = this->reg->what_is_first_string(line);
        cout << line << endl;
        if (tipo_de_string == REG_VARIABLE) {//#ERRADDO
            display_error("primeira_variavel fim VARIAVEL");
            return retorna_variavel(line);
        } else if (tipo_de_string == REG_FUNCTION) {
            display_error("primeira_variavel fim FUNCAO");
            nova_string = retorna_funcao(line);
            if (nova_string.empty()) {
                cout << "####ERRO de sintaxe" << endl;
                cout << line << endl;
                return nova_string;
            }
            verifica_funcao(nova_string); //{analise no lugar errado}
        } else if (line[0] == '"') {
            display_error("primeira variavel fim ASPAS");
            nova_string = aspas_duplas(line);
        } else if (line[0] == '\'') {
            display_error("primeira variavel fim ASPAS");
            nova_string = aspas_simples(line);
        } else if (tipo_de_string == REG_PARENTESE_I) {
            tipo_de_string = this->reg->reg_cast(line);
            pos = retorna_elementos_dentro_de_parentese(line);
            if (tipo_de_string == TRUE_VALUE) {
                cast = 1;
                line = line.substr(pos, line.length());
                cout << "TRUE_CAST" << line << endl;
                return primeira_variavel(line, cast, nova_variavel);
            } else {
                cout << "FALSE_CAST" << endl;
                cout << line << endl;
                pos = retorna_elementos_dentro_de_parentese(line);
                if (pos != string::npos) {
                    nova_string = line.substr(0, pos);
                    line = line.substr(pos, line.length());
                    remove_space(line);
                    nova_string = nova_string.substr(1, nova_string.length());
                    pos = nova_string.find_last_not_of(")");
                    if (pos != string::npos) {
                        nova_string = nova_string.substr(0, pos + 1);
                        nova_string += ";";
                    }
                    prox = proximo_operador(line); //ate aqui eu faco apenas a transformacao do parentese de dentro em um mini linha para analise
                    if (prox == FALSE_VALUE) {
                        cout << line << endl;
                        cout << "linha com erro de sintaxe" << endl;
                        exit(1);
                    } else if (prox == REG_OPERATOR_COMPARE) {
                        remove_operador_intermediario(line);
                        remove_space(line);
                        if (line[0] != ';') {//errado
                            prox = operador_intermediario(line);
                            nova_string = primeira_variavel(line, cast, nova_variavel);
                        } else {
                            nova_string.clear();
                        }
                    } else if (prox == REG_POS_OPERATOR_MAT) {
                        pos_operador(nova_string, nova_variavel, 1);
                    } else
                        pos_operador(nova_string, nova_variavel, 0);
                    cout << "Chama pos operador aqui: " << endl << line << endl;
                    cout << nova_string << endl << line << endl;
                    parentese = TRUE_VALUE;
                } else
                    cout << "comeca colchete mas nao terminar" << endl;

                //  exit(1);
            }
        } else if (line[0] == ';')
            nova_string.clear();
        else {//####RETORNOU UM NUMERO E AGORA??
            display_error("primeira_variavel fim NUMERO?");
            nova_string = "4";
            n = line.find_first_not_of("1234567890.");
            if (n != string::npos)
                line = line.substr(n, line.length());
            return nova_string;
        }
    } while (parentese != FALSE_VALUE);


    return nova_string;
}

string _linha::retorna_variavel(string &line) {//RETORNA A PRIMEIRA STRING QUANDO E VARIAVEL
    int n, pos;
    string retorno;
    display_error("retorna_variavel");
    n = this->reg->reg_exec_first_string(line, pos);
    // cout<<pos<<"::::"<<n<<endl;
    retorno = line.substr(pos, n);
    line = line.substr(n, line.length());
    remove_space(line);
    do {
        if (line[0] == '[') {
            if (line[1] == '\'') {
                retorno += "['";
                line = line.substr(1, line.length());
                n = (int) line.find_first_of("'");
                line = line.substr(n + 1, line.length());
                n = (int) line.find_first_of("'");
                retorno += line.substr(0, n);
                retorno += "']";
                line = line.substr(line.find_first_of("]") + 1, line.length());
            } else if (line[1] == '"') {
                retorno += "[\"";
                line = line.substr(1, line.length());
                n = (int) line.find_first_of('"');
                line = line.substr(n + 1, line.length());
                n = (int) line.find_first_of('"');
                retorno += line.substr(0, n);
                retorno += "\"]";
                line = line.substr(line.find_first_of("]") + 1, line.length());
            } else {
                n = line.find_first_of("]");
                retorno += line.substr(0, n + 1);
                line = line.substr(n + 1, line.length());
            }
        }
        remove_space(line);
        cout << line << endl;
    } while (line[0] == '[');
    if (!line.empty())
        remove_space(line);
    display_error("retorna_variavel fim");
    return retorno;
}

/*## FAZER VERIFICACAO DO POS POIS DARA ERRO DESSA FORMA*/
string _linha::retorna_funcao(string &line) {
    string retorno;
    int pos_ini, pos_fim;
    display_error("retorna_funcao");
    remove_space(line);
    retorno.clear();
    pos_ini = this->reg->reg_verifica_parentese(line, REG_PARENTESE_I, 0);
    cout << line << endl;
    if (pos_ini != FALSE_VALUE) {
        pos_fim = retorna_elementos_dentro_de_parentese(line);
        if (pos_fim == FALSE_VALUE)
            return retorno;
        retorno = line.substr(0, pos_fim);
        line = line.substr(pos_fim, line.length());
    }
    if (!line.empty())
        remove_space(line);
    display_error("retorna_funcao fim");
    return retorno;
}

string _linha::aspas_duplas(string &line) {
    int n;
    string retorno;
    remove_space(line);
    n = this->reg->reg_verifica_aspasd(line, 0);
    if (n == FALSE_VALUE) {
        //   cout << line << endl;
        exit(1);
    }
    line = line.substr(n + 1, line.length());
    //  cout << line << endl;
    n = this->reg->reg_verifica_aspasd(line, 1);
    if (n == FALSE_VALUE) {
        cout << line << endl;
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

string _linha::aspas_simples(string &line) {
    int n;
    string retorno;
    remove_space(line);
    n = this->reg->reg_verifica_aspass(line, 0);
    line = line.substr(n + 1, line.length());
    //cout<<"aspas duplas:"<<line<<endl;
    n = this->reg->reg_verifica_aspass(line, 1);
    if (n == FALSE_VALUE) {
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
int _linha::type_of_operator(string &line) {
    display_error("type_of_operator");
    int retorno;
    remove_space(line);
    retorno = this->reg->reg_to_operator(line);
    if (retorno == REG_OPERATOR_NORMAL)
        line = line.substr(1, line.length());
    else if (retorno != FALSE_VALUE)
        line = line.substr(2, line.length());
    display_error("type_of_operator fim");
    return retorno;
}

int _linha::operador_normal(string subline, string nova_variavel) {//##reg_vetor excesso
    _var nova_var; //pega todas as iteracoes posterior ao operador e armazena aqui para no fim colocar no vetor. Feito dessa forma para nao cair em recursao de objetos quando uma variavel receber ela mesmo depois de receber alguma outra.
    display_error("operador_normal INICIO");
    nova_var.variavel = nova_variavel;
    nova_var.n_var = 0;
    pos_operador(subline, nova_var, 0);
    adiciona_ou_remove_nova_variavel_ao_vetor(nova_var);
    display_error("operador_normal FIM");
    return 0;
}

int _linha::operador_cat(string subline, string nova_variavel) {

    _var nova_var;
    string reg_string_de_var_com_g_p;
    display_error("Operador_cat inicio");
    nova_var.variavel = nova_variavel;
    nova_var.n_var = 0;
    adiciona_get_ou_post(nova_var, nova_var.variavel, 0);
    pos_operador(subline, nova_var, 0);
    adiciona_ou_remove_nova_variavel_ao_vetor(nova_var);
    return 0;
}

int _linha::pos_operador(string &subline, _var &nova_variavel, int cast_total) {
    int tipo, operacao, prox_operador, cast, ultima_variavel;
    string variavel_atual;
    remove_space(subline);
    tipo = operacao = prox_operador = cast = ultima_variavel = 0;
    cast = cast_total;
    variavel_atual = primeira_variavel(subline, cast, nova_variavel);
    display_error("POS operador inicio");
    ultima_variavel = 1;
    do {
        //cout << subline << endl;
        prox_operador = proximo_operador(subline);
        if (prox_operador == REG_OPERATOR_COMPARE) {
            remove_operador_intermediario(subline);
            remove_space(subline);
            if (subline[0] != ';') {
                operacao = operador_intermediario(subline);
                variavel_atual = primeira_variavel(subline, cast, nova_variavel);
            } else {
                variavel_atual.clear();
            }
        } else if (prox_operador == REG_POS_OPERATOR_MAT) {
            cast = 1;
        } else {
            if (cast != 1)
                cast = cast_total;
        }
        if (variavel_atual[0] == '"') {
            operador_aspas(nova_variavel, variavel_atual, cast);
        } else if (!variavel_atual.empty()) {
            tipo = reg->what_is_first_string(variavel_atual); //verifica se e variavel ou funcao
            if (tipo == REG_VARIABLE) {
                operador_normal_variavel(nova_variavel, variavel_atual, cast);
            } else if (tipo == REG_FUNCTION) {
                operador_normal_funcao(nova_variavel, variavel_atual, cast);
            }
        }
        if (subline[0] != ';') {
            operacao = operador_intermediario(subline); /*COMPLEMENTAR ESTA PARTE.*/
            if (operacao == REG_POS_OPERATOR_MAT)
                cast = 1;
            else
                cast = cast_total;
            variavel_atual = primeira_variavel(subline, cast, nova_variavel);
            remove_space(subline);
            ultima_variavel = 1;
        } else
            ultima_variavel = 0;
    } while (ultima_variavel != 0);
    display_error("POS operador FIM");
    return 0;
}

int _linha::adiciona_ou_remove_nova_variavel_ao_vetor(_var nova_variavel) {
    if (nova_variavel.n_var == 0) {//remove a variavel da lista #existe o problema de ele remover quando um $_get ou post recebe algo verificar isso
        cout << "Removendo variavel: " << nova_variavel.variavel << endl;
        remove_variavel_do_vetor(nova_variavel.variavel);
    } else if (nova_variavel.n_var > 0) {
        return copia_var(nova_variavel);
    }
    return 0;
}

int _linha::retorna_elementos_dentro_de_parentese(string line) {
    int pos_ini, pos_fim, i, j, pos, count, aspas_d, aspas_s;
    string retorno, auxiliar;
    retorno.clear();
    pos_ini = pos_fim = i = j = pos = count = aspas_d = aspas_s = 0;
    display_error("retorna_elementos_dentro_de_parentese");
    pos_ini = this->reg->reg_verifica_parentese(line, REG_PARENTESE_I, 0);
    if (pos_ini == FALSE_VALUE)
        return FALSE_VALUE;
    pos_fim = this->reg->reg_verifica_parentese(line, REG_PARENTESE_F, 1);
    cout << pos_fim << endl;
    if (pos_fim == FALSE_VALUE)
        return FALSE_VALUE;
    auxiliar = line.substr(0, pos_fim);
    count = count_char(auxiliar, '(');
    aspas_s = aspas_d = 0;
    cout << count << endl;
    do {
        if (j > auxiliar.length()) {
            return FALSE_VALUE;
        }
        if (line[j] == '\'') {
            if (aspas_s == 0) {
                if (aspas_d == 0)
                    aspas_s = 1;
            } else if (j > 0) {
                if (line[j - 1] != '\\')
                    aspas_s = 0;
            }
        }
        if (line[j] == '"') {
            if (aspas_d == 0) {
                if (aspas_s == 0)
                    aspas_d = 1;
            } else if (j > 0) {
                if (line[j - 1] != '\\')
                    aspas_d = 0;
            }
        }
        if (line[j] == ')' && aspas_s == 0 && aspas_d == 0)
            i++;
        j++;
    } while (i < count);
    cout << j << endl;
    display_error("retorna_elementos_dentro_de_parentese fim");
    return j;
}

int _linha::proximo_operador(string line) {
    int tipo_de_operador;
    remove_space(line);
    tipo_de_operador = this->reg->reg_verifica_operador_compara(line);
    if (tipo_de_operador != FALSE_VALUE) {
        return REG_OPERATOR_COMPARE;
    }
    tipo_de_operador = this->reg->reg_operador_cat_ou_aritmetico(line);
    if (tipo_de_operador != FALSE_VALUE) {
        return tipo_de_operador;
    } else if (line[0] == ';')
        return REG_PONTO_VIRGULA;
    else
        return FALSE_VALUE;

}

int _linha::remove_operador_intermediario(string &line) {
    int n;
    string remove;
    _var nova_variavel;
    n = this->reg->reg_remove_operador_compara(line);
    line = line.substr(n, line.length());
    primeira_variavel(line, n, nova_variavel);

    return 1;
}

int _linha::operador_intermediario(string &subline) {
    int aux;
    string aspas;
    display_error("operador_intermediario");
    remove_space(subline);
    aux = this->reg->reg_operador_cat_ou_aritmetico(subline);
    if (aux == REG_POS_OPERATOR_CAT) {
        subline = subline.substr(1, subline.length());
        remove_space(subline);
        return REG_POS_OPERATOR_CAT;
    } else if (aux == REG_POS_OPERATOR_MAT) {
        subline = subline.substr(1, subline.length());
        remove_space(subline);
        return REG_POS_OPERATOR_MAT;
    } else if (subline[0] == ';') {
        return REG_PONTO_VIRGULA;
    } else
        return FALSE_VALUE;
}

int _linha::operador_aspas(_var &nova_variavel, string &subline, int cast) {
    int n, k, aux, retorno, retorno_operador_normal;
    n = 1;
    retorno = 0;
    string variavel, reg_vetor_de_variaveis;
    display_error("operador_aspas");
    while (n != string::npos) {
        n = subline.find_first_of("$");
        if (n != string::npos) {
            subline = subline.substr(n + 1, subline.length());
            k = subline.find_first_of(" }\"\'$");
            if (k != string::npos) {
                variavel = '$';
                variavel = variavel + subline.substr(0, k);
                reg_vetor_de_variaveis = string_of_var_to_reg();
                aux = this->reg->mount_reg_get_or_post(variavel, reg_vetor_de_variaveis);
                if (aux != FALSE_VALUE) {
                    retorno_operador_normal = operador_normal_variavel(nova_variavel, variavel, cast);

                    if (retorno_operador_normal != FALSE_VALUE)
                        retorno = retorno_operador_normal;
                }
            }
            subline = subline.substr(k, subline.length());
        }
    }
    return retorno;
}

int _linha::operador_normal_variavel(_var &nova_variavel, string &subline, int cast) {
    string reg_vetor_de_variaveis;
    int aux;

    display_error("operador_normal_variavel");
    remove_space(subline);
    reg_vetor_de_variaveis = string_of_var_to_reg();
    aux = reg->mount_reg_get_or_post(subline, reg_vetor_de_variaveis); //1
    if (aux != FALSE_VALUE)//1
        return adiciona_get_ou_post(nova_variavel, subline, cast);
    return FALSE_VALUE; //1
}

int _linha::operador_normal_funcao(_var &nova_variavel, string &string_atual, int cast) {//ALTERAR ESTE METODO PARA VERIFICAR TODAS AS STRINGS DA FUNCAO
    int aux, pos, retorno, i, aux_cast, retorno_absoluto;
    int *numero_de_variaveis;
    string string_func, analisa_string;
    numero_de_variaveis = new (int);
    *numero_de_variaveis = nova_variavel.n_var;
    display_error("operador_normal_funcao");
    remove_space(string_atual);
    retorno_absoluto = retorno = aux_cast = 0;
    if (string_atual[0] == '(')
        string_atual = string_atual.substr(1, string_atual.length());
    do {
        remove_space(string_atual);
        aux = reg->what_is_first_string(string_atual);
        if (aux == REG_NUMBER) {
            analisa_string = primeira_variavel(string_atual, aux_cast, nova_variavel);
        } else if (aux == REG_FUNCTION) {//doidera total rever essa coisa louca
            analisa_string = retorna_funcao(string_atual);
            remove_space(string_atual);
            pos = analisa_string.find_first_of("(");
            if (pos > 0 && pos < analisa_string.length()) {//##ALTERADO AS COMPARACOES DE MAIOR IGUAL PARA MAIOR E MENOR IGUAL PARA MENOR
                string_func = analisa_string.substr(0, pos);
                analisa_string = analisa_string.substr(pos, analisa_string.length());
            }
            if (proximo_operador(string_atual) == REG_POS_OPERATOR_MAT || aux_cast == 1) {
                retorno = operador_normal_funcao(nova_variavel, analisa_string, aux_cast);
                aux_cast = 0;
            } else {
                retorno = operador_normal_funcao(nova_variavel, analisa_string, cast);
            }
            if (retorno == 1) {
                string_func = string_func + "+";
                for (i = *numero_de_variaveis; i < nova_variavel.n_var; i++) {
                    nova_variavel.vect[i].funcao += string_func;
                }
                display_error("operador_normal_funcao fim");
                if (retorno_absoluto != 1)
                    retorno_absoluto = retorno;
            }
        } else if (aux == REG_VARIABLE) {//##adicao de aspas nesse ponto. e perigoso a forma que esta sendo usada ate o momento
            analisa_string = primeira_variavel(string_atual, aux_cast, nova_variavel); //acaba perdendo o resto da string alterar aqui
            if (aux_cast == TRUE_VALUE)
                retorno = operador_normal_variavel(nova_variavel, analisa_string, aux_cast);
            else
                retorno = operador_normal_variavel(nova_variavel, analisa_string, cast);
            aux_cast = 0;
            display_error("operador_normal_funcao fim");
            if (retorno_absoluto != 1)
                retorno_absoluto = retorno;
        } else if (aux == REG_ASPAS) {
            analisa_string = primeira_variavel(string_atual, aux_cast, nova_variavel);
            retorno = operador_aspas(nova_variavel, analisa_string, aux_cast);
            aux_cast = 0;
            if (retorno_absoluto != 1)
                retorno_absoluto = retorno;
        }
        if (string_atual.empty() || string_atual[0] == ')') {
            aux = -3;
        } else if (string_atual[0] == ',') {
            string_atual = string_atual.substr(1, string_atual.length());
        } else if (operador_intermediario(string_atual) == REG_POS_OPERATOR_MAT) {
            aux_cast = 1;
        }
        remove_space(string_atual);
    } while (aux != -3);
    display_error("operador_normal_funcao fim");

    return retorno_absoluto;
}

int _linha::remove_variavel_do_vetor(string var) {
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

int _linha::copia_var(_var nova_variavel) {
    int pos_var, i;
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

int _linha::count_char(string line, char count) {
    int i, contador;
    string sublinha, sublinha_s;
    sublinha = remove_aspas(line, '"');
    if (sublinha.empty())
        sublinha = line;
    sublinha_s = remove_aspas(sublinha, '\'');
    if (!sublinha_s.empty())
        contador = 0;
    for (i = 0; i < sublinha.length(); i++) {
        if (sublinha[i] == count) {
            contador++;
        }
    }
    return contador;
}

string _linha::remove_aspas(string linha, char remove) {
    int i, aspas_i, aspas_f;
    string retorno;
    aspas_i = -1;
    aspas_f = 0;
    retorno.clear();
    for (i = 0; i < linha.length(); i++) {
        if (linha[i] == remove) {
            if (aspas_i == -1) {
                aspas_i = i;
                if (i > 0)
                    retorno += linha.substr(aspas_f, aspas_i - aspas_f);
            } else {
                if (linha[i - 1] != '\\') {
                    aspas_f = i + 1;
                    aspas_i = -1;
                }
            }
        }
    }
    if (aspas_f != 0)
        retorno += linha.substr(aspas_f, linha.length());
    return retorno;
}

int _linha::adiciona_get_ou_post(_var &add, string subline, int cast) {//vai ser adicionado a excessao de aspas simples aqui
    int pos, n;
    string string_guardar, vet_var_p_g;
    display_error("adiciona_get_ou_post");
    remove_space(subline);
    vet_var_p_g = string_of_var_to_reg();
    if (reg->reg_segunda_parte_linha(subline) == TRUE_VALUE) {//verifica se o primeiro elemento 'e um get ou post
        string_guardar = subline; //retorna_variavel(subline, reg);
        add.vect[add.n_var].variavel = string_guardar;
        add.vect[add.n_var].cast = cast;
        add.n_var++;
        display_error("adiciona_get_ou_post FIM GET");
        return 1;
    } else if (!vet_var_p_g.empty() && reg->mount_reg_get_or_post(subline, vet_var_p_g) != FALSE_VALUE) {
        string_guardar = subline; //retorna_variavel(subline, reg);
        pos = procura_variavel_no_vetor(string_guardar);
        if (pos == FALSE_VALUE)
            return FALSE_VALUE;
        for (n = 0; n < this->vetor_de_variaveis[pos].n_var; n++) {//arrumar o cast
            add.vect[add.n_var].cast = this->vetor_de_variaveis[pos].vect[n].cast == 1 ? this->vetor_de_variaveis[pos].vect[n].cast : cast;
            add.vect[add.n_var].variavel = this->vetor_de_variaveis[pos].vect[n].variavel;
            add.vect[add.n_var].funcao = this->vetor_de_variaveis[pos].vect[n].funcao;
            add.n_var++;
        }
        display_error("adiciona_get_ou_post FIM VARIAVEL");
        return 1;
    } else {//SE TA LOCAO?!
        cout << "Nao possui get nem post" << endl;
        display_error("adiciona_get_ou_post FIM NAO POSSUI");
        return FALSE_VALUE;
    }
}

int _linha::procura_variavel_no_vetor(string var) {
    int i;
    display_error("procura_variavel_no_vetor");
    for (i = 0; i < this->vet_num; i++) {
        if (var.compare(this->vetor_de_variaveis[i].variavel) == 0)
            return i;
    }
    return FALSE_VALUE;
}

int _linha::add_var_array(string new_string) {//insercao de variavel no vetor de variaveis
    display_error("add_var_array");
    this->vetor_de_variaveis[this->vet_num].variavel = new_string;
    this->vet_num++;

    return this->vet_num - 1;
}

int _linha::verifica_funcao(string funcao) {
    string func, variavel_a_analisar, funcao_impressao;
    string sql;
    char *SQL;
    vector <string> linha;
    char buffer[64];
    vector<vector<string> >::iterator resultado;
    _var nova_variavel;
    funcao_impressao = funcao;
    int retorno = FALSE_VALUE, cast;
    cast = 0;
    int i, n, n_parametros, auxiliar, pos;
    display_error("inicio verifica_funcao");
    n = funcao.find_first_of(" (");
    if (n != string::npos) {
        func = funcao.substr(0, n);
        funcao = funcao.substr(n + 1, funcao.length());
        remove_space(funcao);
        if (funcao[0] == '(')
            funcao = funcao.substr(1, funcao.length());
    }
    this->conexao->executar("SELECT * FROM func_analisa", 2);
    if (this->conexao->query_result != 3) {
        for (resultado = this->conexao->resultados.begin(); resultado < this->conexao->resultados.end(); resultado++) {
            linha = *resultado;
            cout << linha.at(0) << endl;
            cout << linha.at(1) << endl;
            if (func.compare(linha.at(1)) == 0) {
                cout << this->no << endl << endl << endl;
                n_parametros = count_char(funcao, ',');
                n_parametros++;
                if (n_parametros == 1) {
                    variavel_a_analisar = primeira_variavel(funcao, cast, nova_variavel);
                    cout << variavel_a_analisar << "FIN" << endl;
                } else {
                    n = 0;
                    while (n < atoi(linha.at(0).c_str())) {
                        n++;
                        variavel_a_analisar = primeira_variavel(funcao, cast, nova_variavel);
                        remove_space(funcao);
                        auxiliar = funcao.find_first_not_of(',');
                        if (auxiliar != string::npos)
                            funcao = funcao.substr(auxiliar, funcao.length());
                        else {
                            cout << "FUNCAO COM ERRO" << endl;
                            exit(1);
                        }
                    }
                }
                imprime_vetor();
                pos = procura_variavel_no_vetor(variavel_a_analisar);
                cout << pos << variavel_a_analisar << endl;
                if (pos != FALSE_VALUE) {
                    sprintf(buffer, "%d", this->id_irmao_original);
                    cout << "AQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUIAQUI" << endl << this->no << this->id_irmao_original << endl << buffer << endl << endl << endl << endl << endl;
                    this->seguranca_retorno = "FUNCAO ANALISADA:";
                    this->seguranca_retorno += funcao_impressao;
                    this->seguranca_retorno += "\nNo: ";
                    this->seguranca_retorno += this->no;
                    this->seguranca_retorno += "\n";
                    this->seguranca_retorno += "ID_IRMAO:";
                    this->seguranca_retorno += buffer;
                    this->seguranca_retorno += "\n";

                    sql = "SELECT funcao FROM func_sec WHERE id_bd = 0 OR id_bd = ";
                    sql += linha.at(0);
                    SQL = new char[sql.length()+1];
                    strcpy (SQL, sql.c_str());
                   // cout << sql << endl;
                    this->conexao->executar(SQL, 2);
                    delete SQL;
                    funcao_mysql(this->vetor_de_variaveis[pos]);
                    //    cout <<"SEG FAULT" << endl;
                  /*  if (func[0] == 'm')
                        funcao_mysql(this->vetor_de_variaveis[pos]);
                    else if (func[0] == 'p')
                        funcao_postgre(this->vetor_de_variaveis[pos]);
                    else
                        funcao_padrao(this->vetor_de_variaveis[pos], 1);
                    imprime_elemento(this->vetor_de_variaveis[pos]);
                    */// cout << this->seguranca_retorno << endl;
                }
                retorno = 1;
            }
        }
    }
    display_error("fim verifica_funcao");
    return retorno;
}

void _linha::imprime_vetor() {
    int i, j;
    if (this->filho != NULL)
        this->filho->imprime_vetor();
    if (this->irmao != NULL)
        this->irmao->imprime_vetor();
    cout << "IMPRESSAO DO VETOR DO NO:" << this->no << endl << endl;
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

void _linha::imprime_elemento(_var elemento) {
    int j, k;
    char buffer[10];
    k = elemento.n_var;

    this->seguranca_retorno += "\n\nIMPRESSAO DE ELEMENTO: ";
    sprintf(buffer, "%d", k);
    this->seguranca_retorno += elemento.variavel + " N parametros: " + *buffer + ".\n";
    // sprintf(buffer, "%d", this->id_irmao_original);
    this->seguranca_retorno += *buffer + "\n";
    for (j = 0; j < elemento.n_var; j++) {
        sprintf(buffer, "%d", elemento.vect[j].cast);
        this->seguranca_retorno += "===" + elemento.vect[j].variavel + "." + *buffer + "\n";

        if (!elemento.vect[j].funcao.empty())
            this->seguranca_retorno += "\t===" + elemento.vect[j].funcao + "." + "\n";
    }
    this->seguranca_retorno += "\n\n\n\n";
}

int _linha::novo_no(string nome, int colchete, int id, int tipo) {
    _linha *auxiliar;
    _linha *novo_no;
    int i, j;
    if (this->filho == NULL) {
        this->filho = new _linha;
        auxiliar = this->filho;
        auxiliar->pai = this;
        auxiliar->tipo_de_condicional = tipo;
        auxiliar->id_irmao_original = id;
        auxiliar = NULL;
    } else {
        auxiliar = this->filho;
        novo_no = new _linha;
        novo_no->irmao = auxiliar;
        auxiliar->tipo_de_condicional = tipo;
        this->filho = novo_no;
        novo_no->pai = this;
        auxiliar = novo_no; //1
        //cout << "ADD NOVO NO"<<endl<<tipo<<endl;
        if (tipo == REG_ELSEIF || tipo == REG_ELSE) {//1VERIFICAR ESSA IDEIA LOUCA AQUI
            cout << "ADD NOVO NO" << endl << tipo << endl;
            while (auxiliar->tipo_de_condicional != REG_IF && auxiliar->irmao != NULL) {
                cout << "ADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NO" << endl;
                cout << auxiliar->irmao->no << endl;
                auxiliar = auxiliar->irmao;
            }
            cout << "id do hermano" << endl << auxiliar->id << endl;
            novo_no->id_irmao_original = auxiliar->id;
        }
        auxiliar = NULL;
    }
    //1this->filho->colchete = colchete;
    if (colchete == -1) {//1
        this->filho->dois_pontos = 1;
        this->filho-> colchete = 0;
    } else if (colchete == 1) {
        this->filho->dois_pontos = 0;
        this->filho->colchete = 1;
    } else {
        this->filho->dois_pontos = 0;
        this->filho->colchete = 0;
    }
    this->filho->vet_num = this->vet_num;
    this->filho->id = id;
    this->filho->no = nome;
    for (i = 0; i < this->vet_num; i++) {
        this->filho->vetor_de_variaveis[i].variavel = this->vetor_de_variaveis[i].variavel;
        this->filho->vetor_de_variaveis[i].n_var = this->vetor_de_variaveis[i].n_var;
        for (j = 0; j < this->vetor_de_variaveis[i].n_var; j++) {
            this->filho->vetor_de_variaveis[i].vect[j].variavel = this->vetor_de_variaveis[i].vect[j].variavel;
            this->filho->vetor_de_variaveis[i].vect[j].cast = this->vetor_de_variaveis[i].vect[j].cast;
            this->filho->vetor_de_variaveis[i].vect[j].funcao = this->vetor_de_variaveis[i].vect[j].funcao;
        }
    }
    return 0;
}

int _linha::copia_para_pai() {
    int i, j;
    if (this->pai == NULL) {
        cout << "DEU ERRADO O COPIA PARA PAI" << endl;
        exit(1);
    }
    for (i = 0; i < this->vet_num; i++) {
        this->pai->vetor_de_variaveis[i].variavel = this->vetor_de_variaveis[i].variavel;
        this->pai->vetor_de_variaveis[i].n_var = this->vetor_de_variaveis[i].n_var;
        for (j = 0; j < this->vetor_de_variaveis[i].n_var; j++) {
            this->pai->vetor_de_variaveis[i].vect[j].variavel = this->vetor_de_variaveis[i].vect[j].variavel;
            this->pai->vetor_de_variaveis[i].vect[j].cast = this->vetor_de_variaveis[i].vect[j].cast;
            this->pai->vetor_de_variaveis[i].vect[j].funcao = this->vetor_de_variaveis[i].vect[j].funcao;
        }
    }
}

int _linha::funcao_mysql(_var valor_analisado) {
    int i, pos, retorno, retorno_absoluto = 2;
    vector <string> linha;
    vector<vector<string> >::iterator resultado;
    //cout <<"SEG FAULT" << endl;
    for (i = 0; i < valor_analisado.n_var; i++) {
        retorno = funcao_padrao(valor_analisado, i);
        cout << valor_analisado.vect[i].funcao << endl;
        for (resultado = this->conexao->resultados.begin(); resultado < this->conexao->resultados.end(); resultado++) {
            linha = *resultado;
            pos = valor_analisado.vect[i].funcao.find(linha.at(0));
            if (pos != string::npos) {
                this->seguranca_retorno += "A variavel " + valor_analisado.variavel + " possui o valor de " + valor_analisado.vect[i].variavel + " e foi tratada por " + linha.at(0);
                retorno = 1;
            }
            if (valor_analisado.vect[i].cast == 0 && retorno == 1) {
                this->seguranca_retorno += " porem caso esta variavel nao esteja dentro de aspas e possivel ocorrer injection.\n";
            } else if (retorno == 1)
                this->seguranca_retorno += ".\n";
            if (retorno == 0) {
                this->seguranca_retorno += "A variavel " + valor_analisado.variavel + " possui o " + valor_analisado.vect[i].variavel + " pode estar insegura.\n";
                retorno_absoluto = 0;
            }
        }
        /*pos = valor_analisado.vect[i].funcao.find("mysql_escape_string");
        if (pos != string::npos) {
            this->seguranca_retorno += "A variavel " + valor_analisado.variavel + " possui o valor de " + valor_analisado.vect[i].variavel + " e foi tratada por mysql_escape_string";
            retorno = 1;
        }
        pos = valor_analisado.vect[i].funcao.find("mysql_real_escape_string");
        if (pos != string::npos) {
            this->seguranca_retorno += "A variavel " + valor_analisado.variavel + " possui o valor de " + valor_analisado.vect[i].variavel + " e foi tratada por mysql_real_escape_string";
            retorno = 1;
        }
        if (valor_analisado.vect[i].cast == 0 && retorno == 1) {
            this->seguranca_retorno += " porem caso esta variavel nao esteja dentro de aspas e possivel ocorrer injection.\n";
        } else if (retorno == 1)
            this->seguranca_retorno += ".\n";

        if (retorno == 0) {
            this->seguranca_retorno += "A variavel " + valor_analisado.variavel + " possui o " + valor_analisado.vect[i].variavel + " pode estar insegura.\n";
            retorno_absoluto = 0;
        } */
    }
    return retorno_absoluto;
}

int _linha::funcao_postgre(_var valor_analisar) {
    cout << "oi" << endl;
    return 0;
}

int _linha::funcao_padrao(_var valor_analisado, int i) {
    int pos, retorno = 0;
    pos = valor_analisado.vect[i].funcao.find("addslashes");
    if (pos != string::npos) {
        this->seguranca_retorno += "Variavel " + valor_analisado.vect[i].variavel + "possui addslashes.\n";
        retorno = 1;
    }
    if (valor_analisado.vect[i].cast == 1) {
        this->seguranca_retorno += "Foi realizado cast na variavel " + valor_analisado.vect[i].variavel + "\n";
        retorno = 1;
    }
    return retorno;
}