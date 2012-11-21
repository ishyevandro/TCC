#include <iostream>
#include <cstdio>
#include "classe_linha.h"

_linha::_linha() : irmao(NULL), pai(NULL), filho(NULL), reg(NULL), vet_num(0), colchete(0), dois_pontos(0) {
    this->vetor_de_variaveis.resize(VET_NUM);
    this->reg = new _reg;
    this->id_irmao_original = 0;
    this->no = "Raiz";
    this->conexao = new BANCOSL(DB_NAME);
}

_linha::~_linha() {
    delete this->reg;
    this->no.clear();
}

void _linha::display_error(string prnt) {//funcao para o desenvolvimento, simples amostragem de funcoes que entrou e que nao entrou
    if (DISPLAY_ERROR == 1)
        cout << this->no << prnt << endl;
}

int _linha::verifica_linha(string line, string &debug, int irmao, int verifica_xss) {//funcao chamada pela classe_codigo para analise de caracteres
    int n;
    this->xss = verifica_xss; //variavel que verifica XSS
    if (this->filho != NULL)//quando um no pai e atualizado, todos os filhos devem ser atualizados dessa forma e feito recursao nesse ponto para todos os filhos fazerem analises distintas.
        this->filho->verifica_linha(line, debug, 1, verifica_xss);
    if (irmao == 1) {//quando o objeto codigo chama essa funcao e passado o valor 0, indicando que os irmaos nao devem ser analisados pois sao iraosdo pai
        if (this->irmao != NULL)//dessa os irmaos do filho devem ser analisados entao e feito essa verificacao para entao chamar os irmaos
            this->irmao->verifica_linha(line, debug, 1, verifica_xss);
    }
    remove_space(line); //remove espacos
    n = reg->what_is_first_string(line); //verifica o que e a primeira string
    if (n == REG_VARIABLE) {//se for uma variavel entao ocorre analise
        analisa_linha(line);
    } else {//se nao for verifica, e considerado como funcao e o valor, linha, passado para analise e encaminhado para o verificador de funcao
        verifica_funcao(line);
        return -1;
    }
    debug += this->seguranca_retorno; //adiciona ao debug os valores de resultado da analise
    this->seguranca_retorno.clear(); //limpa a variavel de retorno para futuras analises
    return 1;
}

void _linha::remove_space(string &line) {//remove espacos para analise
    size_t found;
    found = line.find_first_not_of(" \t");
    if (found != string::npos)
        line = line.substr((int) found, line.length());
    else
        line.clear();
}

/*Corpo principal, analisa uma linha para ver se foi passado algum valor do usuario para a variavel php*/
int _linha::analisa_linha(string line) {
    int operator_type;
    string /*2array_of_vars_in_array,*/ nova_variavel;
    operator_type = 0;

    display_error("analyse_line");

    nova_variavel = primeira_string(line);

    if (!nova_variavel.empty()) {
        operator_type = type_of_operator(line); /*verifica qual o tipo de operador*/
        switch (operator_type) {/*acoes a serem feitas com cada operador*/
            case REG_OPERATOR_NORMAL:
                operador_normal(line, nova_variavel); //cria um objeto var e faz o devido processamento
                break;
            case REG_OPERATOR_CAT:
                operador_cat(line, nova_variavel); //cria um objeto var, copia o conteudo do objeto que possuir a nova_variavel e faz o devido processamento
                break;
            case REG_OPERATOR_MAT:
                operador_mat(line, nova_variavel); //cria um objeto var, copia o conteudo do objeto que possuir a nova_variavel, da o valor de cast para todos os elementos dentro e faz o devido processamento
                break;
        }
        remove_space(line);
    }
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
        // //cout << line << endl;
        if (tipo_de_string == REG_VARIABLE) {//#ERRADDO
            display_error("primeira_variavel fim VARIAVEL");
            return retorna_variavel(line);
        } else if (tipo_de_string == REG_FUNCTION) {
            display_error("primeira_variavel fim FUNCAO");
            nova_string = retorna_funcao(line);
            if (nova_string.empty()) {
                return nova_string;
            }
            //1 verifica_funcao(nova_string);
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
                return primeira_variavel(line, cast, nova_variavel);
            } else {
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
                    //     //cout << "Chama pos operador aqui: " << endl << line << endl;
                    //       //cout << nova_string << endl << line << endl;
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
    // //cout<<pos<<"::::"<<n<<endl;
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
        //cout << line << endl;
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
    //cout << line << endl;
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
        //   //cout << line << endl;
        exit(1);
    }
    line = line.substr(n + 1, line.length());
    //  //cout << line << endl;
    n = this->reg->reg_verifica_aspasd(line, 1);
    if (n == FALSE_VALUE) {
        //cout << line << endl;
        //cout << "Nao possui outra aspas" << endl;
        exit(1);
    }
    retorno = '"';
    retorno += line.substr(0, n);
    retorno += '"';
    ////cout<<line<<endl;
    line = line.substr(n + 1, line.length());
    //  //cout << "aspas duplas line:" << line << endl;
    // //cout << "ASPAS DUPLAS " << retorno << endl;

    return retorno;
}

string _linha::aspas_simples(string &line) {
    int n;
    string retorno;
    remove_space(line);
    n = this->reg->reg_verifica_aspass(line, 0);
    line = line.substr(n + 1, line.length());
    ////cout<<"aspas duplas:"<<line<<endl;
    n = this->reg->reg_verifica_aspass(line, 1);
    if (n == FALSE_VALUE) {
        //cout << "Nao possui outra aspas" << endl;
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

//cria um objeto _var novo para possivel insercao do mesmo no vetor de variaveis e chama a analise apos o sinal de atribuicao

int _linha::operador_normal(string subline, string nova_variavel) {
    _var nova_var; //pega todas as iteracoes posterior ao operador e armazena aqui para no fim colocar no vetor. Feito dessa forma para nao cair em recursao de objetos quando uma variavel receber ela mesmo depois de receber alguma outra.
    display_error("operador_normal INICIO");
    nova_var.variavel = nova_variavel; //atribui o nome a esse objeto variavel
    nova_var.n_var = 0; //declara que ele esta sem variavel
    pos_operador(subline, nova_var, 0); //chama o pos_operador para analise do restante da linha
    adiciona_ou_remove_nova_variavel_ao_vetor(nova_var); //decide se a variavel sera adicionada ou removida do vetor de objetos
    display_error("operador_normal FIM");
    return 0;
}

int _linha::operador_cat(string subline, string nova_variavel) {
    _var nova_var;
    string reg_string_de_var_com_g_p;
    display_error("Operador_cat inicio");
    nova_var.variavel = nova_variavel;
    nova_var.n_var = 0;
    adiciona_get_ou_post(nova_var, nova_var.variavel, 0); //concatena os objetos ja existentes a essa variavel
    pos_operador(subline, nova_var, 0); //chama pos operador para terminar a analise da linha
    adiciona_ou_remove_nova_variavel_ao_vetor(nova_var); //adiciona ao vetor de variaveis caso tenha algum valor passado pelo usuario
    return 0;
}

int _linha::operador_mat(string subline, string nova_variavel) {
    _var nova_var;
    display_error("Operador_cat inicio");
    nova_var.variavel = nova_variavel;
    nova_var.n_var = 0;
    adiciona_get_ou_post(nova_var, nova_var.variavel, 1); //copia todos os elementos do objeto, caso exista, e adiciona o valor de cast para todos
    pos_operador(subline, nova_var, 1); //chama pos operador e o valor um indica que todos os valores receberao cast
    adiciona_ou_remove_nova_variavel_ao_vetor(nova_var); //adiciona ou remove do vetor de variaveis desse objeto
    return 0;
}

int _linha::pos_operador(string &subline, _var &nova_variavel, int cast_total) {//comeca apos o sinal de atribuicao
    int tipo, operacao, prox_operador, cast, ultima_variavel;
    string variavel_atual;
    remove_space(subline);
    tipo = operacao = prox_operador = cast = ultima_variavel = 0;
    cast = cast_total;
    variavel_atual = primeira_variavel(subline, cast, nova_variavel); //retorna o primeiro elemento
    display_error("POS operador inicio");
    ultima_variavel = 1;
    do {
        ////cout << subline << endl;
        prox_operador = proximo_operador(subline); //verifica o proximo elemento, caso seja um + e atribuido cast para o atual, caso seja de comparacao ignora o atual elemento e o proximo e vai em busca do terceiro elemento
        if (prox_operador == REG_OPERATOR_COMPARE) {//retorno compare remove os elementos
            remove_operador_intermediario(subline); //remove o elemento atual, o operador intermediario e o proximo elemento
            remove_space(subline);
            if (subline[0] != ';') {
                operacao = operador_intermediario(subline); //pega o operador
                variavel_atual = primeira_variavel(subline, cast, nova_variavel); //pega o proximo elemento
            } else {
                variavel_atual.clear();
            }
        } else if (prox_operador == REG_POS_OPERATOR_MAT) {//se operador matematico cast 1, um simboliza que possui cast
            cast = 1;
        } else {
            if (cast != 1)//verifica se o valor cast ja possui algum valor, se nao possuir ele recebe o valor passado pelo metodo anterior, metodo que diz se todos receberao cast ou nao
                cast = cast_total;
        }
        if (variavel_atual[0] == '"') {//se comecar com aspas duplas ele verifica todos os elementos dentro de aspas dupals em busca de uma variavel. essa e uma forma de concatenacao do php
            operador_aspas(nova_variavel, variavel_atual, cast); //faz toda a analise de aspas duplas
        } else if (!variavel_atual.empty()) {//verifica o tipo de string
            tipo = reg->what_is_first_string(variavel_atual); //verifica se e variavel ou funcao
            if (tipo == REG_VARIABLE) {//se for variavel chama o operador_normal_variavel... este adiciona a variavel ao objeto _var provisorio.
                operador_normal_variavel(nova_variavel, variavel_atual, cast);
            } else if (tipo == REG_FUNCTION) {//se for funcao chama o metodo abaixo.
                operador_normal_funcao(nova_variavel, variavel_atual, cast);
            }
        }
        if (subline[0] != ';') {//se nao for fim de linha procura o operador intermediario
            operacao = operador_intermediario(subline); /*COMPLEMENTAR ESTA PARTE.*/
            if (operacao == REG_POS_OPERATOR_MAT)
                cast = 1;
            else
                cast = cast_total;
            variavel_atual = primeira_variavel(subline, cast, nova_variavel); //busca o proximo elemento
            remove_space(subline); //remove espacos
            ultima_variavel = 1; //numero que indica que nao chegou ao final da linha
        } else
            ultima_variavel = 0; //numero que indica que chegou ao final d alinha
    } while (ultima_variavel != 0);
    display_error("POS operador FIM");
    return 0;
}

/*Verifica se o objeto _var que analisou a linha recebida recebeu algum valor passado pelo usuario*/
int _linha::adiciona_ou_remove_nova_variavel_ao_vetor(_var nova_variavel) {
    if (nova_variavel.n_var == 0) {//se nao recebeu nenhum valor esse objeto e removido do vetor.
        remove_variavel_do_vetor(nova_variavel.variavel);
    } else if (nova_variavel.n_var > 0) {//se recebeu algum valor esse objeto e copiado para o vetor.
        remove_variavel_do_vetor(nova_variavel.variavel); //inicialmente e tentado remover, caso exista algum objeto antigo e removido.
        return copia_var(nova_variavel); //depois e adicionado novamente ao vetor porem com os valores novos
    }
    return 0;
}

int _linha::retorna_elementos_dentro_de_parentese(string line) {//retorna todos os elementos dentro de parenteses
    int pos_ini, pos_fim, i, j, pos, count, aspas_d, aspas_s;
    string retorno, auxiliar;
    retorno.clear();
    pos_ini = pos_fim = i = j = pos = count = aspas_d = aspas_s = 0;
    display_error("retorna_elementos_dentro_de_parentese");
    pos_ini = this->reg->reg_verifica_parentese(line, REG_PARENTESE_I, 0); //verifica o inicio de parenteses
    if (pos_ini == FALSE_VALUE)
        return FALSE_VALUE;
    pos_fim = this->reg->reg_verifica_parentese(line, REG_PARENTESE_F, 1); //verifica o fim dos parenteses
    if (pos_fim == FALSE_VALUE)
        return FALSE_VALUE;
    auxiliar = line.substr(0, pos_fim);
    count = count_char(auxiliar, '('); //verifica se existem outros parenteses dentro deles... se houver e procurado mais parenteses... Dessa forma e englobado todos o paranteses corretamente
    aspas_s = aspas_d = 0;
    do {
        if (j > auxiliar.length()) {
            return FALSE_VALUE;
        }
        if (line[j] == '\'') {//engenhosidade criada para nao contar os parenseteses dentro de aspas simples ou duplas
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
    } while (i < count); //loop ate retornar o conteudo do parentese
    display_error("retorna_elementos_dentro_de_parentese fim");
    return j;
}

int _linha::proximo_operador(string line) {//verifica qual e o proximo operador, para saber se deve ser atribuido cast para variavel atual ou nao
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

int _linha::remove_operador_intermediario(string &line) {//remove o operador intermediario em casos que o operador e uma comparacao
    int n;
    string remove;
    _var nova_variavel;
    n = this->reg->reg_remove_operador_compara(line);
    line = line.substr(n, line.length());
    primeira_variavel(line, n, nova_variavel);

    return 1;
}

int _linha::operador_intermediario(string &subline) {//verifica qual e o operador antes do proximo elemento
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

/*Este metodo verifica todos os valores dentro de aspas duplas em busca de variaveis*/
int _linha::operador_aspas(_var &nova_variavel, string &subline, int cast) {
    int n, k, aux, retorno, retorno_operador_normal;
    n = 1;
    retorno = 0;
    string variavel, reg_vetor_de_variaveis;
    display_error("operador_aspas");
    while (n != string::npos) {
        n = subline.find_first_of("$"); //procura o inicio de uma variavel
        if (n != string::npos) {
            subline = subline.substr(n + 1, subline.length());
            k = subline.find_first_of(" }\"\'$@%^&*(),./\\"); //procura pelo primeiro sinal de um fim de variavel. Este fim incui espaco, aspas duplas, aspas simples, cifrao e colchete
            if (k != string::npos) {
                variavel = '$';
                variavel = variavel + subline.substr(0, k);
                reg_vetor_de_variaveis = string_of_var_to_reg(); //pega as atuais variaveis no vetor de variaveis e transforma em uma string para expressao regular
                aux = this->reg->mount_reg_get_or_post(variavel, reg_vetor_de_variaveis); //verifica se a variavel encontrada no meio das aspas sao compativeis com algum valor do vetor
                if (aux != FALSE_VALUE) {
                    retorno_operador_normal = operador_normal_variavel(nova_variavel, variavel, cast); //caso seja compativel e adicionado essa variavel ao vetor de var da classe_var temporario

                    if (retorno_operador_normal != FALSE_VALUE)
                        retorno = retorno_operador_normal;
                }
            }
            subline = subline.substr(k, subline.length()); //move a variavel subline para o ponto apos a variavel encontrada
        }
    }
    return retorno;
}

/*Adiciona uma variavel ao vetor de objetos do tipo classe_var, do objeto _var temporario*/
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

/*adiciona uma funcao ao objeto do tipo variavel do objeto _var temporario*/
int _linha::operador_normal_funcao(_var &nova_variavel, string &string_atual, int cast) {//ALTERAR ESTE METODO PARA VERIFICAR TODAS AS STRINGS DA FUNCAO
    int aux, pos, retorno, i, aux_cast, retorno_absoluto;
    int *numero_de_variaveis;
    string string_func, analisa_string;
    numero_de_variaveis = new (int);
    *numero_de_variaveis = nova_variavel.n_var;
    display_error("operador_normal_funcao");
    remove_space(string_atual);
    if (verifica_funcao(string_atual) == TRUE_VALUE)//verifica se e uma funcao de analise. Caso seja simplesmente encerra nesse ponto. Pois o retorno dessas funcoes nao sao valores passados pelo usuario
        return TRUE_VALUE;
    retorno_absoluto = retorno = aux_cast = 0;
    if (string_atual[0] == '(')
        string_atual = string_atual.substr(1, string_atual.length()); //comeca a vasculhar a partir do parentese
    do {
        remove_space(string_atual);
        aux = reg->what_is_first_string(string_atual); //verifica o que e a primeira string
        if (aux == REG_NUMBER) {//se for um numero apenas procura a proxima string
            analisa_string = primeira_variavel(string_atual, aux_cast, nova_variavel);
        } else if (aux == REG_FUNCTION) {//se for uma funcao ele guarda a funcao e procura pelos valores dentro dessa funcao
            analisa_string = retorna_funcao(string_atual);
            remove_space(string_atual);
            pos = analisa_string.find_first_of("(");
            if (pos > 0 && pos < analisa_string.length()) {//analise recursiva
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
        } else if (aux == REG_VARIABLE) {//#adiciona a variavel e com a recursao e adicionado todas as funcoes pela qual a variavel passou
            analisa_string = primeira_variavel(string_atual, aux_cast, nova_variavel);
            if (aux_cast == TRUE_VALUE)
                retorno = operador_normal_variavel(nova_variavel, analisa_string, aux_cast);
            else
                retorno = operador_normal_variavel(nova_variavel, analisa_string, cast);
            aux_cast = 0;
            display_error("operador_normal_funcao fim");
            if (retorno_absoluto != 1)
                retorno_absoluto = retorno;
        } else if (aux == REG_ASPAS) {//analisa com o operador de aspas
            analisa_string = primeira_variavel(string_atual, aux_cast, nova_variavel);
            retorno = operador_aspas(nova_variavel, analisa_string, aux_cast);
            aux_cast = 0;
            if (retorno_absoluto != 1)
                retorno_absoluto = retorno;
        }
        if (string_atual.empty() || string_atual[0] == ')') {//condicao de saida do loop, simboliza o final do elemento analisado
            aux = -3;
        } else if (string_atual[0] == ',') {
            string_atual = string_atual.substr(1, string_atual.length()); //se for virgula, simplesmente procura a proximo elemento valido
        } else if (operador_intermediario(string_atual) == REG_POS_OPERATOR_MAT) {//adiciona cast caso seja um operador matematico
            aux_cast = 1;
        }
        remove_space(string_atual);
    } while (aux != -3);
    display_error("operador_normal_funcao fim");

    return retorno_absoluto;
}

/*remove objeto do tipo _var do vetor do objeto linha*/
int _linha::remove_variavel_do_vetor(string var) {
    int i, pos;
    display_error("remove_variavel_do_vetor");
    pos = procura_variavel_no_vetor(var);
    if (pos == FALSE_VALUE) {//proura de variavel no vetor, caso nao tenha sido encontrado nao ha variavel a remover
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

/*copia objeto temporario para o vetor de objetos da classe_linha*/
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
//conta quantos caracteres ocorreram em um espaco da linha

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
//remove aspas duplas e simples, de acordo com o parametro passado retorna uma linha sem aspas simples e aspas duplas

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
//adiciona ao objeto temporario um objeto do tipo variavel caso ele seja um valor passado pelo usuario ou seja uma variavel que possui um valor passado pelo usuario

int _linha::adiciona_get_ou_post(_var &add, string subline, int cast) {
    int pos, n;
    string string_guardar, vet_var_p_g;
    display_error("adiciona_get_ou_post");
    remove_space(subline);
    vet_var_p_g = string_of_var_to_reg();
    if (reg->reg_segunda_parte_linha(subline) == TRUE_VALUE) {//verifica se o primeiro elemento 'e um get ou post
        string_guardar = subline;
        add.vect[add.n_var].variavel = string_guardar;
        add.vect[add.n_var].cast = cast;
        add.n_var++;
        display_error("adiciona_get_ou_post FIM GET");
        return TRUE_VALUE;
    } else if (!vet_var_p_g.empty() && reg->mount_reg_get_or_post(subline, vet_var_p_g) != FALSE_VALUE) {//verifica se o elemento a ser guardado esta dentro do vetor de objeto do tipo _Var
        string_guardar = subline; //caso esteja esse valor e copiado para o objeto var temporario para prosseguir com a analise
        pos = procura_variavel_no_vetor(string_guardar);
        if (pos == FALSE_VALUE)
            return FALSE_VALUE;
        for (n = 0; n < this->vetor_de_variaveis[pos].n_var; n++) {//passa dos as caracteristicas do objeto encontrado para o objeto temporario
            add.vect[add.n_var].cast = this->vetor_de_variaveis[pos].vect[n].cast == 1 ? this->vetor_de_variaveis[pos].vect[n].cast : cast;
            add.vect[add.n_var].variavel = this->vetor_de_variaveis[pos].vect[n].variavel;
            add.vect[add.n_var].funcao = this->vetor_de_variaveis[pos].vect[n].funcao;
            add.n_var++;
        }
        display_error("adiciona_get_ou_post FIM VARIAVEL");
        return TRUE_VALUE;
    } else {
        display_error("adiciona_get_ou_post FIM NAO POSSUI");
        return FALSE_VALUE;
    }
}
//procura uma variavel PHP no vetor de objetos do tipo _var

int _linha::procura_variavel_no_vetor(string var) {
    int i;
    display_error("procura_variavel_no_vetor");
    for (i = 0; i < this->vet_num; i++) {
        if (var.compare(this->vetor_de_variaveis[i].variavel) == 0)
            return i;
    }
    return FALSE_VALUE;
}
//adiciona um objeto variavel ao vetor de objetos _var. Este add e utilizado pelo objeto temporario
int _linha::add_var_array(string new_string) {
    display_error("add_var_array");
    this->vetor_de_variaveis[this->vet_num].variavel = new_string;
    this->vet_num++;

    return this->vet_num - 1;
}
//verifica se uma funcao e de algum tipo de acesso ao banco ou uma funcao da lista negra

int _linha::verifica_funcao(string funcao) {
    string func, variavel_a_analisar, funcao_impressao;
    string sql;
    _linha *pont_pai;
    _var analisa;
    char *SQL;
    vector <string> linha;
    char buffer[64];
    vector<vector<string> >::iterator resultado; //iterador utilizado para armazenar o resultado da consulta.
    _var nova_variavel;
    funcao_impressao = funcao;
    int retorno = FALSE_VALUE, cast;
    cast = 0;
    int i, n, n_parametros, auxiliar, pos;
    display_error("inicio verifica_funcao");
    n = funcao.find_first_of(" (");
    if (n != string::npos) {
        func = funcao.substr(0, n); //pega apenas o nome da funcao
        funcao = funcao.substr(n + 1, funcao.length()); //fica com o restante da funcao, (os valores)
        remove_space(funcao);
    }
    this->conexao->executar("SELECT * FROM func_analisa", 2); //consulta quais sao as funcoes que necessitam de analise
    if (this->conexao->query_result != 3) {//caso o resultado seja verdadeiro
        for (resultado = this->conexao->resultados.begin(); resultado < this->conexao->resultados.end(); resultado++) {//verifica as funcoes retornadas do sqlite
            linha = *resultado;
            if (func.compare(linha.at(1)) == 0) {//compara uma a uma com a func atual
                n_parametros = count_char(funcao, ','); //conta o numero de parametros
                n_parametros++;
                if (n_parametros == 1) {//se o parametro for igual  e analisado o primeiro parametro
                    pos = funcao.find_last_of(")"); //substitui o fim por ponto e virgula para utilizar o metodo pos_operador para ver todos os valores que possui
                    if (pos != string::npos) {
                        variavel_a_analisar = funcao.substr(0, pos);
                        variavel_a_analisar += ";";
                    }
                } else {
                    n = 1;
                    while (n < atoi(linha.at(2).c_str())) {//procura o parametro correto que deve ser analisados
                        n++; //loop chegar ao argumento que deve ser analisado
                        while (funcao[0] != ',') {
                            variavel_a_analisar = primeira_variavel(funcao, cast, nova_variavel); //"retirando" elementos da linha ate ser encontrado uma virgula
                            remove_space(funcao);
                            if (funcao[0] != ',')//retira o elemento intermediario que possivelmente sera um operador
                                funcao = funcao.substr(1, funcao.length());
                        }
                        auxiliar = funcao.find_first_not_of(',');
                        if (auxiliar != string::npos)//retira a virgula
                            funcao = funcao.substr(auxiliar, funcao.length());
                    }
                    variavel_a_analisar.clear();
                    while (funcao[0] != ')' && funcao[0] != ',') {//loop para pegar todos os valores passados no argumento X da funcao, geralmente sendo 2 ou 1.
                        variavel_a_analisar += primeira_variavel(funcao, cast, nova_variavel);
                        remove_space(funcao);
                        if (funcao[0] != ',' && funcao[0] != ')') {
                            variavel_a_analisar += funcao.substr(0, 1);
                            funcao = funcao.substr(1, funcao.length());
                        }
                    }
                    variavel_a_analisar += ";";
                }
                pos_operador(variavel_a_analisar, analisa, 0); //armazena as variaveis em objeto _var.
                analisa.variavel = "Passada para a funcao";
                imprime_vetor();
                if (analisa.n_var > 0) {
                    this->seguranca_retorno = "Funcao analisada:";
                    this->seguranca_retorno += funcao_impressao;
                    sql = funcao_mysql(analisa, linha.at(0));
                    retorno = 1;
                    break;
                }
            }
        }
    }
    this->conexao->executar("SELECT * FROM black_list", 2);//black_list
    if (this->conexao->query_result != 3) {//caso o resultado seja verdadeiro
        for (resultado = this->conexao->resultados.begin(); resultado < this->conexao->resultados.end(); resultado++) {
            linha = *resultado;
            if (func.compare(linha.at(0))== 0)
                retorno = 1;
        }
    }
    funcao = funcao_impressao;
    remove_space(funcao);
    if(this->reg->reg_include(funcao)==TRUE_VALUE){
        pos = funcao.find_first_of ("(");
        if (pos !=string::npos){
            funcao = funcao.substr(pos, funcao.length());
            cout << funcao << endl;
            analisa.n_var = 0;
            variavel_a_analisar = primeira_variavel (funcao, pos,analisa);
            if (analisa.n_var > 0){
                this->seguranca_retorno += "No: " + this->no + "\n";
                this->seguranca_retorno += "O " + funcao_impressao + "esta vulneravel a leitura de arquivos do servidor e PHP injection.\n";
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
    //cout << "??????" << endl;

}

void _linha::imprime_elemento(_var elemento) {
    int j, k;
    char buffer[10];
    k = elemento.n_var;

    this->seguranca_retorno += "\n\nIMPRESSAO DE ELEMENTO: ";
    sprintf(buffer, "%d", k);
    this->seguranca_retorno += elemento.variavel + " N parametros: " + *buffer + ".\n";
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
        auxiliar->id_irmao_original = 0;
        auxiliar = NULL;
    } else {
        auxiliar = this->filho;
        novo_no = new _linha;
        novo_no->irmao = auxiliar;
        novo_no->tipo_de_condicional = tipo;
        this->filho = novo_no;
        novo_no->pai = this;
        auxiliar = novo_no; //1
        ////cout << "ADD NOVO NO"<<endl<<tipo<<endl;
        if (tipo == REG_ELSEIF || tipo == REG_ELSE) {//1VERIFICAR ESSA IDEIA LOUCA AQUI
            //cout << "ADD NOVO NO" << endl << tipo << endl;
            while (auxiliar->tipo_de_condicional != REG_IF && auxiliar->irmao != NULL) {
                //cout << "ADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NOADD NOVO NO" << endl;
                //cout << auxiliar->irmao->no << endl;
                auxiliar = auxiliar->irmao;
            }
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
        //cout << "DEU ERRADO O COPIA PARA PAI" << endl;
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

string _linha::funcao_mysql(_var valor_analisado, string db_num) {
    int i, pos, retorno, retorno_absoluto = 2;
    vector <string> linha;
    vector<vector<string> >::iterator resultado;
    string aux_retorno, sql;
    char *SQL;
    sql = "SELECT funcao FROM func_sec WHERE id_bd = 0 OR id_bd = ";
    sql += db_num;
    SQL = new char[sql.length() + 1];
    strcpy(SQL, sql.c_str());
    cout << SQL << endl;
    this->conexao->executar(SQL, 2);
    delete SQL;

    this->seguranca_retorno += "\nNo: ";
    this->seguranca_retorno += this->no;
    this->seguranca_retorno += "\n";
    cout << "ANTES DO FIM" << this->seguranca_retorno << endl;
    for (i = 0; i < valor_analisado.n_var; i++) {
        retorno_absoluto = 0;
        for (resultado = this->conexao->resultados.begin(); resultado < this->conexao->resultados.end(); resultado++) {
            retorno = 0;
            linha = *resultado;
            pos = valor_analisado.vect[i].funcao.find(linha.at(0));
            if (pos != string::npos && retorno == 0 && retorno_absoluto != 1) {
                aux_retorno = "O valor passado para o banco possui " + valor_analisado.vect[i].variavel + " que foi tratado por " + linha.at(0) + ", e esta seguro contra SQL injection.\n";
                retorno = 1;
                retorno_absoluto = 1;
            } else if (valor_analisado.vect[i].cast == 1) {
                aux_retorno += " Foi realizado cast no valor " + valor_analisado.vect[i].variavel + ".\n";
                retorno = 1;
                retorno_absoluto = 1;
            }
        }
        if (retorno == 0 && retorno_absoluto == 0) {
            aux_retorno = "O valor passado para o banco possui " + valor_analisado.vect[i].variavel + " e pode estar inseguro contra SQL injection.\n";
        }
        this->seguranca_retorno += aux_retorno;
    }
    if (this->xss == TRUE_VALUE) {
        this->conexao->executar("SELECT funcao from func_sec WHERE tipo = 2", 2);
        for (i = 0; i < valor_analisado.n_var; i++) {
            retorno = 0;
            for (resultado = this->conexao->resultados.begin(); resultado < this->conexao->resultados.end(); resultado++) {
                linha = *resultado;
                pos = valor_analisado.vect[i].funcao.find(linha.at(0));
                if (pos != string::npos && retorno == 0) {
                    retorno = 1;
                    this->seguranca_retorno += "Foi utilizado funca anti armazenamento de XSS. (" + linha.at(0) + " na variavel: " + valor_analisado.vect[i].variavel + ").\n";
                }
            }
            if (retorno == 0)
                this->seguranca_retorno += "A variavel " + valor_analisado.vect[i].variavel + " esta vulneravel a XSS.\n";
        }
    }
    this->seguranca_retorno += "\n\n\n";
    cout << this->seguranca_retorno << endl;
    return this->seguranca_retorno;

}