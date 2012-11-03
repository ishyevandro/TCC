#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_codigo.h"

void display_errors(string prnt) {
    if (DISPLAY_ERROR == 1)
        cout << prnt << endl;
}

_codigo::_codigo()
: reg(NULL), aspas(NULL), no_corrente(NULL), id(0), tag_php(0) {
    aspas = new _aspas;
    no_corrente = &aux;
    reg = new _reg;
    debug.clear();
}

int _codigo::linha(string codigo_todo) {
    string linha;
    int pos;
    display_errors("metodo LINHA inicio");
    this->codigo = codigo_todo;
    aux.id = this->id;
    id++;
    apenas_php();
    while (!this->codigo.empty()) {
        //cout << "linha" << this->codigo << endl;
        if (nova_linha(linha) == TRUE_VALUE)
            verifica_tag(linha);
        else
            cout << "Improvavel entrar aqui" << endl;
    }
    display_errors("metodo LINHA inicio");
    cout << this->debug << endl << endl;
    cout << no_corrente->no << endl;
}

int _codigo::aspas_duplas(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nlinha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspasd(linha, 0);
    if (linha.empty())
        return FALSE_VALUE;
    if (inicio != FALSE_VALUE) {
        tipo = "d";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspasd(inicio_aspas, 1);
            if (fim == FALSE_VALUE) {
                cout <<"ja esta dentro de aspas duplas"<< linha << endl;
                nova_linha(nlinha);
                cout << "DENTRO DE ASPAS DUPLAS" << endl;
                inicio_aspas += nlinha;
                cout <<"ESSA PORRA TAVA CERTA" << inicio_aspas << endl;
                
            } else if (inicio_aspas[fim - 1] != '\\') {
                retorno += inicio_aspas.substr(0, fim);
                cout << "FOI VERIFICADO QUE ESTA COM COMENTARIO" << retorno << endl;
                //exit (1);
                linha = linha.substr(0, inicio);
                //2 linha += this->no_corrente->aspas->novo_valor(retorno, tipo);
                linha += this->aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            } else {
                retorno += inicio_aspas.substr(0, fim);
                inicio_aspas = inicio_aspas.substr(fim + 1, inicio_aspas.length());
                fim = FALSE_VALUE;
            }
        }
        aspas_duplas(linha);
        //cout << "linha aqui: " << linha << endl;
        // exit(1);
    }
    // cout << "ASPAS DUPLAS" <<linha << endl;
    return 1;
}

int _codigo::remove_comments(string &line) {
    int type, comments, remove_primeiro, retorno;
    string comentario, verifica_xss;
    retorno = -1;
    if (verifica_comentario_dentro_de_aspas(line) == -1) {
        cout << line << endl;
        exit(1);
        return 0;
    }
    type = reg->reg_comments(line);
    if (type != FALSE_VALUE) {
        verifica_xss = line.substr(type+2, line.length());
        this->xss = reg->xss(verifica_xss);
        //if (this->xss == TRUE_VALUE)exit (1);
        line = line.substr(0, type);
        retorno = 1;
    }
    type = reg->reg_comments_(line, REG_COMMENTS_INI);
    if (type != FALSE_VALUE) {
        remove_primeiro = type - 2; //utilizado o remove_rpimeiro para remover a primeira forma de comentario ja que a reg_comments retorna a primeira posicao apos a cadeia de caracteres
        line = line.substr(0, remove_primeiro);
        comentario = line.substr(remove_primeiro, line.length());
        type = FALSE_VALUE;
        while (type == FALSE_VALUE) {
            type = reg->reg_comments_(comentario, REG_COMMENTS_FIM);
            if (type != FALSE_VALUE) {
                line += comentario.substr(type, comentario.length());
                ///cout <<"IMPRIME PORRA"<<comentario.substr(type, comentario.length())<<endl;
            } else {
                nova_linha(comentario);
                cout << "DENTRO DE remove comments" << endl;
            }
        }
    }
    return 0;
}

int _codigo::verifica_comentario_dentro_de_aspas(string line) {
    int comentario, comentario2, aspas, retorno;
    comentario = reg->reg_comments(line); //verifica a linha possui comentario
    if (comentario != FALSE_VALUE)
        comentario = reg->reg_comments(line.substr(comentario, line.length()));
    comentario2 = reg->reg_comments_(line, REG_COMMENTS_INI);
    if (comentario2 != FALSE_VALUE)
        comentario2 = reg->reg_comments(line.substr(comentario2, line.length()));
    if (comentario != FALSE_VALUE || comentario2 != FALSE_VALUE) {
        aspas = reg->reg_verifica_aspasd(line, 0);
        //cout<<aspas<<"!="<<comentario<<comentario2<<endl;
        if ((aspas < comentario || aspas < comentario2) && aspas != FALSE_VALUE && (comentario != FALSE_VALUE || comentario2 != FALSE_VALUE))
            return FALSE_VALUE;
    }

    return TRUE_VALUE;
}

int _codigo::aspas_simples(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nlinha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspass(linha, 0);
    //cout <<"INICIO ASPAS_SIMPLES: "<< linha << endl;
    if (inicio != FALSE_VALUE) {
        tipo = "s";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspass(inicio_aspas, 1);
            if (fim == FALSE_VALUE) {
                // cout << "ASpas simples" << inicio_aspas << endl;
                nova_linha(nlinha);
                cout << "DENTRO DE ASPAS simples" << endl;
                inicio_aspas += nlinha;
            } else {
                retorno = inicio_aspas.substr(0, fim);
                linha = linha.substr(0, inicio);
                linha += this->aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_simples(linha);
    }
    //  cout <<"ASPAS_SIMPLES:" <<linha << endl;
    return 1;
}

/*
int main(int argc, char **argv) {
    _codigo *A;
    A = new _codigo;
    A->open("/home/evandro/Dropbox/TCC/TCC/Manipulacao de arquivo/projeto/test/index.php");
    delete A;
    return 0;
}*/



string _codigo::procura_fim_de_linha(string linha) {
    int pos;
    string nlinha;
    nlinha.clear();
    display_errors("Procura_fim_de_linha inicio: ");
    do {
        pos = linha.find_first_of(';');
        if (pos == string::npos) {
            //  cout << "procura_fim_de_linha" << endl;
            nova_linha(nlinha);
            cout << "DENTRO DE procura fim de linha" << no_corrente->no << endl;
            cout << no_corrente->no << "linha zero: " << linha << endl;
            remove_comments(nlinha);
            aspas_duplas(nlinha);
            aspas_simples(nlinha);
            cout << no_corrente->no << "nlinha: " << nlinha << endl;
            linha += nlinha;
            cout << no_corrente->no << "linha: " << linha << endl;
        }
    } while (pos == string::npos && !this->codigo.empty());
    display_errors("Procura_fim_de_linha fim: ");
    return linha;
}

string _codigo::replace_aspas(string linha) {
    string nlinha, aspas, retorno, verifica;
    int pos, tipo, analise;
    analise = 1;
    verifica = linha;
    nlinha = linha;
    display_errors("Replace_aspas inicio: ");
    while (analise == 1) {
        cout << "entrou no replace!" << endl;
        pos = this->reg->reg_retorna_variavel_aspas(verifica, 1);
        //cout << pos << endl;
        if (pos != FALSE_VALUE) {
            nlinha = verifica.substr(0, pos);
            verifica = verifica.substr(pos, verifica.length());
            pos = this->reg->reg_retorna_variavel_aspas(verifica, 0);
            if (pos != FALSE_VALUE) {
                aspas = verifica.substr(0, pos);
                cout << "SAIDA DAS ASPAS: " << aspas << endl;
                if (aspas[aspas.length() - 1] == 'd') {
                    nlinha += '"';
                    tipo = 0;
                } else {
                    nlinha += '\'';
                    tipo = 1;
                }
                verifica = verifica.substr(pos, verifica.length());
                //2retorno = this->no_corrente->aspas->busca_valor(aspas);
                retorno = this->aspas->busca_valor(aspas);
                cout << retorno << endl;
                if (!retorno.empty())
                    nlinha += retorno;
                else {
                    cout << "EXIT REPLACE_ASPAS";
                    exit(-1);
                }
                if (tipo == 1)
                    nlinha += '\'';
                else
                    nlinha += '"';
                nlinha += verifica;
                verifica = nlinha;
            }
        } else {
            analise = 0;
        }
    }
    cout << "saiu do replace" << endl;
    this->aspas->call_clear();
    display_errors("Replace_aspas fim: ");
    return nlinha;
}

int _codigo::verifica_tag(string &linha) {
    int pos, tag, concatena, analisa_ou_nao, aux_cond;
    string auxiliar, retorno;
    tag = pos = 0;
    concatena = FALSE_VALUE;
    auxiliar.clear();
    display_errors("verifica_tag inicio: ");
    remove_comments(linha);
    cout<<"removeu o comentario"<<endl;
    aspas_duplas(linha);
    aspas_simples(linha);
    linha = procura_fim_de_linha(linha);
    cout << "completa: " << linha << endl;
    do {
        no_corrente->remove_space(linha);
        pos = linha.find_first_of(";");
        aux_cond = fim_condicao(linha);
        cout << "COUT ANTES DO ERRO ALI NA FRENTE" << auxiliar << endl;
        if (pos != string::npos && aux_cond != TRUE_VALUE) {
            auxiliar = linha.substr(0, pos + 1);
            cout << "COUT DENTRO DO IF" << auxiliar << endl;

            linha = linha.substr(pos + 1, linha.length());
            //  cout << "ANTES DE CONDICAO E REPLACE" << endl;
            auxiliar = replace_aspas(auxiliar);
            verifica_condicional(auxiliar);
            cout << this->no_corrente->colchete << endl;
            retorno.clear();
            this->no_corrente->verifica_linha(auxiliar, retorno, 0, this->xss);
            this->xss = FALSE_VALUE;
            this->debug += retorno;
        }
        this->no_corrente->remove_space(linha);
        aux_cond = fim_condicao(linha);
        if (this->no_corrente->colchete == 0 && this->no_corrente->pai != NULL  /*&& 1&& this->no_corrente->tipo_de_condicional != REG_ELSE*/) {
            concatena = compara_pai(this->no_corrente); //reaproveitamento da variavel concatena
            if (concatena == FALSE_VALUE)
                cout << "SOU DIFERENTE DO MEU PAI: " << this->no_corrente->id << this->no_corrente->no << endl;
            else {
                cout << "SOU IGUAL AO MEU PAI: " << this->no_corrente->id << this->no_corrente->no << endl;
                //1 experimental remove_objeto_da_arvore(this->no_corrente->id, NULL);
            }
            if (this->no_corrente->tipo_de_condicional == REG_ELSE/*2 remove_obje da arvore == FALSE_VALUE*/) {//1
                this->no_corrente->copia_para_pai();
            }
            if (this->no_corrente->pai != NULL)
                this->no_corrente = this->no_corrente->pai;

        }
        cout << "PASSOU DOS IF`S" << endl;
    } while (pos != string::npos);
    display_errors("verifica_tag fim: ");
}

int _codigo::nova_linha(string &linha) {
    int pos;
    display_errors("nova_linha: ");
    pos = this->codigo.find_first_of("\n");
    if (pos != string::npos) {
        linha = this->codigo.substr(0, pos);
        this->codigo = this->codigo.substr(pos + 1, this->codigo.length());
        return TRUE_VALUE;
    } else if (this->codigo.length() != 0) {
        linha = this->codigo.substr(0, this->codigo.length());
        this->codigo.clear();
        return TRUE_VALUE;
    } else {
        cout << "fim_de arquivo" << endl;
        this->codigo.clear();
    }
    return FALSE_VALUE;
    display_errors("nova_linha fim: ");
}

int _codigo::verifica_condicional(string &linha) {
    int apos_cond, tipo, pos, colchete;
    string if_completo, auxiliar_nome;
    apos_cond = pos = tipo = colchete = 0;
    no_corrente->remove_space(linha);
    auxiliar_nome = "->";
    apos_cond = this->reg->reg_condicional_if(linha, tipo);
    verifica_laco(linha);
    if (apos_cond != FALSE_VALUE) {
        if_completo = no_corrente->no;
        if (tipo == REG_IF || tipo == REG_ELSEIF || tipo == REG_ELSE) {
            if_completo += auxiliar_nome;
            if_completo += linha.substr(0, apos_cond);
            linha = linha.substr(apos_cond, linha.length());
            pos = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
            if (pos != FALSE_VALUE) {
                if_completo += linha.substr(0, pos);
                linha = linha.substr(pos, linha.length());
                colchete = verifica_pos_condicional(linha);
                this->no_corrente->novo_no(if_completo, colchete, this->id, tipo);
                this->id++;
                this->no_corrente = this->no_corrente->filho;
            } else {
                colchete = verifica_pos_condicional(linha);
                this->no_corrente->novo_no(if_completo, colchete, this->id, tipo);
                this->id++;
                this->no_corrente = this->no_corrente->filho;
                this->no_corrente->tipo_de_condicional = REG_ELSE;
                cout << "continuacao do else" << endl;
            }
        } else if (tipo == REG_SWITCH) {
            cout << "SWITCH ACHADO" << endl;
            cout << linha << endl;
            if_completo += auxiliar_nome;
            if_completo += linha.substr(0, apos_cond);
            linha = linha.substr(apos_cond, linha.length());
            pos = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
            if (pos != string::npos) {
                if_completo += linha.substr(0, pos);
                linha = linha.substr(pos, linha.length());
                colchete = verifica_pos_condicional(linha);
                this->no_corrente->novo_no(if_completo, colchete, this->id, tipo);
                this->id++;
                this->no_corrente = this->no_corrente->filho;
            }
            //     cout << if_completo << endl;
            this->no_corrente->remove_space(linha);
            apos_cond = this->reg->reg_condicional_if(linha, tipo);
        }
        if (tipo == REG_CASE && no_corrente->tipo_de_condicional == REG_SWITCH) {
            if_completo += auxiliar_nome;
            cout << "CASE ACHADO" << endl;
            cout << linha << endl;
            do {//cout << linha << endl << linha.substr(0, apos_cond - 1);
                apos_cond = linha.find_first_of(":");
                if (apos_cond != string::npos) {
                    if_completo += linha.substr(0, apos_cond + 1);
                    linha = linha.substr(apos_cond + 1, linha.length());
                } else {
                    cout << "Nao possui iniciador de CASE \":\"" << endl;
                    return ERROR;
                }
                no_corrente->remove_space(linha);
                apos_cond = this->reg->reg_condicional_if(linha, tipo);
            } while (apos_cond != FALSE_VALUE && tipo);
            //   cout << if_completo << endl << linha << endl;
            this->no_corrente->novo_no(if_completo, 1, this->id, REG_CASE);
            this->id++;
            cout << this->no_corrente->no<<endl;
            this->no_corrente = this->no_corrente->filho;
            this->no_corrente->tipo_de_condicional = REG_CASE;
            cout <<"NAO VAI DAR MERDANAO VAI DAR MERDANAO VAI DAR MERDANAO VAI DAR MERDANAO VAI DAR MERDA" << this->no_corrente->no << this->no_corrente->tipo_de_condicional<<endl;

        }
        verifica_condicional(linha);
        return tipo;
    } else
        return FALSE_VALUE;
}

int _codigo::verifica_pos_condicional(string &linha) {
    this->no_corrente->remove_space(linha);
    if (linha[0] == '{') {
        linha = linha.substr(1, linha.length());
        cout << linha << endl;
        return 1;
    } else if (linha[0] == ':') {
        linha = linha.substr(1, linha.length());
        return -1;
    }
    cout << "NAO POSSUI COLCHETES" << endl;
    return 0;
}

int _codigo::compara_pai(_linha *corrente) {
    _linha *pai;
    int i, j;
    if (corrente->pai == NULL)
        return TRUE_VALUE;
    pai = corrente->pai;
    if (pai->vet_num != corrente->vet_num)
        return FALSE_VALUE;
    for (i = 0; i < pai->vet_num; i++) {
        if (pai->vetor_de_variaveis[i].variavel.compare(corrente->vetor_de_variaveis[i].variavel) != 0)
            return FALSE_VALUE;
        for (j = 0; j < pai->vetor_de_variaveis[i].n_var; j++) {
            if (pai->vetor_de_variaveis[i].vect[j].variavel.compare(corrente->vetor_de_variaveis[i].vect[j].variavel) != 0)
                return FALSE_VALUE;
            if (pai->vetor_de_variaveis[i].vect[j].funcao.compare(corrente->vetor_de_variaveis[i].vect[j].funcao) != 0)
                return FALSE_VALUE;
            if (pai->vetor_de_variaveis[i].vect[j].cast != corrente->vetor_de_variaveis[i].vect[j].cast)
                return FALSE_VALUE;
        }
    }
    return TRUE_VALUE;
}

int _codigo::remove_objeto_da_arvore(int procura, _linha *raiz) {
    int removido;
    _linha *pai, *auxiliar, *irmao, *remove;
    remove = this->no_corrente;
    pai = this->no_corrente->pai;
    if (remove->irmao != NULL) {
        pai->filho = remove->irmao;
        irmao = remove->irmao;
        if (remove->filho != NULL) {
            auxiliar = remove->filho;
            while (auxiliar->irmao != NULL) {
                auxiliar = auxiliar->irmao;
            }
            auxiliar->irmao = irmao->filho;
            irmao->filho = remove->filho;
            delete remove;
        } else
            delete remove;
    } else if (remove->filho != NULL) {
        pai->filho = remove->filho;
        auxiliar = remove->filho;
        if (auxiliar->irmao != NULL) {
            irmao = auxiliar;
            do {
                irmao->pai = pai;
                irmao = irmao->irmao;
            } while (irmao->irmao != NULL);
        }
        delete remove;
    } else {
        // cout << "TEM QUE ENTRAR AQUI" << remove->no << endl;
        pai->filho = NULL;
        delete remove;
    }
    return TRUE_VALUE;
}

int _codigo::fim_condicao(string &line) {
    int retorno;
    no_corrente->remove_space(line);
    cout << line << endl;
    if (line[0] == '}' && no_corrente->colchete > 0) {
        this->no_corrente->colchete--;
        line = line.substr(1, line.length());
        cout << line << endl << "true value" << endl;
        fim_condicao(line);
        return TRUE_VALUE;
    } else if (no_corrente->dois_pontos > 0 && no_corrente->tipo_de_condicional != REG_CASE) {
        cout << "ENTROU AQUI dois pontos" << no_corrente->dois_pontos << endl;

    } else if (no_corrente->tipo_de_condicional == REG_CASE) {
        cout <<"ENTROU NO LANCE DO CASE" <<line <<endl;
        retorno = reg->reg_break_condicionais(line, REG_CASE);
        cout << line << endl << retorno << endl;
        if (retorno != FALSE_VALUE) {
            line = line.substr(retorno, line.length());
            cout << line << endl;
            this->no_corrente->colchete = 0;
            return TRUE_VALUE;
        }
        else return FALSE_VALUE;
    }else
        cout<< "DEU MERDADEU MERDADEU MERDADEU MERDADEU MERDADEU MERDADEU MERDADEU MERDADEU MERDA"<<endl;
    cout << "FALSE VALUE" << line<< no_corrente->no << no_corrente->tipo_de_condicional << endl;
    return FALSE_VALUE;
}

int _codigo::apenas_php() {
    string aux, auxiliar;
    int pos, tag, concatena;
    pos = tag = concatena = 0;
    aux = this->codigo;
    auxiliar.clear();
    do {
        tag = pos;
        pos = reg->reg_tag_php(aux, tag_php);
        if (pos != FALSE_VALUE) {
            if (tag_php == 0) {
                aux = aux.substr(pos, aux.length());
                tag_php = 1;
                if (concatena != FALSE_VALUE)
                    concatena = 1;
            } else if (tag_php == 1) {//falta uma variavel aqui PARA ANALISE FUTURA!!!
                auxiliar += aux.substr(0, pos);
                aux = aux.substr(pos, aux.length());
                concatena = 0;
                tag_php = 0;
            }
        }
    } while (tag != FALSE_VALUE);
    this->codigo = auxiliar;
    return TRUE_VALUE;
}

int _codigo::verifica_laco(string &linha) {
    int ret_reg, tipo;
    this->no_corrente->remove_space(linha);
    ret_reg = this->reg->reg_laco(linha, tipo);
    if (ret_reg != FALSE_VALUE) {
        linha = linha.substr(ret_reg, linha.length());
        if (tipo == REG_LOOP) {
            ret_reg = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
            if (ret_reg != FALSE_VALUE) {
                linha = linha.substr(ret_reg, linha.length());
                cout << linha.substr(0, linha.length()) << endl;
            }
        }
        this->no_corrente->remove_space(linha);
        if (linha[0] == '{') {
            if (this->no_corrente->colchete > 0)
                this->no_corrente->colchete++;
        }
    }
}