#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_arquivo.h"

void display_errors(string prnt) {
    if (DISPLAY_ERROR == 1)
        cout << prnt << endl;
}

_arq::_arq()
: reg(NULL), aspas(NULL), no_corrente(NULL), id(0), tag_php(0)/*,new_file(NULL),  vetor_de_variaveis(NULL), vet_num(0) */ {
    //vetor_de_variaveis = new _var[20];
    //this->vetor_de_variaveis.resize(VET_NUM);
    aspas = new _aspas;
    no_corrente = &aux;
    reg = new _reg;
}

int _arq::linha(string codigo_todo) {
    string linha;
    int pos;
    display_errors("metodo LINHA inicio");
    this->codigo = codigo_todo;
    aux.id = this->id;
    id++;
    while (!this->codigo.empty()) {
        // cout << "linha"<<this->codigo<< endl;
        if (nova_linha(linha) == TRUE_VALUE)
            verifica_tag(linha);
        else
            cout << "Improvavel entrar aqui" << endl;
    }
    display_errors("metodo LINHA inicio");
    no_corrente->imprime_vetor();
}

/*
void _arq::open(string abrir) {
    char *nome_do_arquivo;
    string linha; //linha lida do arquivo
    int var_teste, cont_vetor = 0;
    new_file.open(abrir.c_str(), ifstream::out);
    while (new_file.good()) {
        this->aux.aspas->call_clear();
        nova_linha(linha);
        //1        aspas_duplas(linha);
        //1        aspas_simples(linha);
        //1        remove_comments(linha);
        //1        linha = procura_fim_de_linha(linha);
        //1        linha = replace_aspas(linha);
        //cout<<linha<<endl;
        verifica_tag(linha);
        linha.clear();
    }
    aux.imprime_vetor();
    //vetor_de_variaveis[0].imprime_vetor(vetor_de_variaveis, vet_num);
}*/

int _arq::aspas_duplas(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nlinha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspasd(linha, 0);
    //cout << "inicio_linha: " << linha << endl;
    if (inicio != FALSE_VALUE) {
        tipo = "d";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspasd(inicio_aspas, 1);
            if (fim == FALSE_VALUE) {
                //  cout << "aspas_duplas" << endl;
                nova_linha(nlinha);
                inicio_aspas += nlinha;
            } else {
                retorno = inicio_aspas.substr(0, fim);
                linha = linha.substr(0, inicio);
                //2 linha += this->no_corrente->aspas->novo_valor(retorno, tipo);
                linha += this->aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_duplas(linha);
        //cout << "linha aqui: " << linha << endl;
        // exit(1);
    }
    // cout << "ASPAS DUPLAS" <<linha << endl;
    return 1;
}

int _arq::remove_comments(string &line) {
    int type, comments, remove_primeiro, retorno;
    string comentario;
    retorno = -1;
    if (verifica_comentario_dentro_de_aspas(line) == -1)
        return 0;
    //   cout << "ERRO REMOVE_COMENTS" << endl;
    type = reg->reg_comments(line);
    //   cout << type << line << endl;
    if (type != FALSE_VALUE) {
        line = line.substr(0, type);
        retorno = 1;
        //      cout << type << "   " << line << endl;
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
                //  cout << "comentarios_" << endl;
                nova_linha(comentario);
            }
        }
    }
    return 0;
}

int _arq::verifica_comentario_dentro_de_aspas(string line) {
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

    return 1;
}

int _arq::aspas_simples(string &linha) {
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
    _arq *A;
    A = new _arq;
    A->open("/home/evandro/Dropbox/TCC/TCC/Manipulacao de arquivo/projeto/test/index.php");
    delete A;
    return 0;
}*/



string _arq::procura_fim_de_linha(string linha) {
    int pos;
    string nlinha;
    display_errors("Procura_fim_de_linha inicio: ");
    do {
        pos = linha.find_first_of(';');
        if (pos == string::npos) {
            //  cout << "procura_fim_de_linha" << endl;
            nova_linha(nlinha);
            aspas_duplas(nlinha);
            aspas_simples(nlinha);
            remove_comments(nlinha);
            linha += nlinha;
        }
    } while (pos == string::npos && !this->codigo.empty());
    display_errors("Procura_fim_de_linha fim: ");
    return linha;
}

string _arq::replace_aspas(string linha) {
    string nlinha, aspas, retorno, verifica;
    int pos, tipo, analise;
    analise = 1;
    verifica = linha;
    nlinha = linha;
    display_errors("Replace_aspas inicio: ");
    while (analise == 1) {
        cout << "entrou no replace!" << endl;
        pos = this->reg->reg_retorna_variavel_aspas(verifica, 1);
        cout << pos << endl;
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
    //2 this->no_corrente->aspas->call_clear();
    this->aspas->call_clear();
    display_errors("Replace_aspas fim: ");
    return nlinha;
}

int _arq::verifica_tag(string &linha) {
    int pos, tag, concatena, analisa_ou_nao;
    string auxiliar;
    tag = pos = 0;
    concatena = FALSE_VALUE;
    auxiliar.clear();
    display_errors("verifica_tag inicio: ");
    if (tag_php == 1)
        analisa_ou_nao = 1;
    else
        analisa_ou_nao = 0;
    do {
        tag = pos;
        pos = reg->reg_tag_php(linha, tag_php);

        if (pos != FALSE_VALUE) {
            if (tag_php == 0) {
                linha = linha.substr(pos, linha.length());
                tag_php = 1;
                analisa_ou_nao = 1;
                if (concatena != FALSE_VALUE)
                    concatena = 1;
            } else if (tag_php == 1) {//falta uma variavel aqui PARA ANALISE FUTURA!!!
                auxiliar += linha.substr(0, pos);
                linha = linha.substr(pos, linha.length());
                concatena = 0;
                tag_php = 0;
            }
        }
        aspas_duplas(linha);
        aspas_simples(linha);
        remove_comments(linha);
        linha = procura_fim_de_linha(linha);
    } while (tag != FALSE_VALUE);
    if (!auxiliar.empty()) {
        if (concatena == 1)
            auxiliar += linha;
        linha = auxiliar;
    }
    if (analisa_ou_nao == 1) {
        cout << "completa: " << linha << endl;
        do {
            no_corrente->remove_space(linha);
            // cout <<"completa: " linha << endl;

            pos = linha.find_first_of(";");
            //cout << linha[0] << endl;
            if (linha[0] == '}') {
                cout << "ACHOU O COLCHETE" << endl;
                this->no_corrente->colchete = 0;
                linha = linha.substr(1, linha.length());

            } else if (pos != string::npos) {
                auxiliar = linha.substr(0, pos + 1);
                linha = linha.substr(pos + 1, linha.length());
                auxiliar = replace_aspas(auxiliar);
                cout << auxiliar << endl;
                verifica_condicional(auxiliar);
                cout << this->no_corrente->colchete << endl;
                this->no_corrente->verifica_linha(auxiliar, 0);
            }
            // cout << auxiliar<< endl;
            this->no_corrente->remove_space(linha);
            if (linha[0] == '}')
                this->no_corrente->colchete = 0;
            // cout << this->no_corrente->colchete << endl;
            if (this->no_corrente->colchete == 0 && this->no_corrente->pai != NULL && this->no_corrente->tipo_de_condicional != REG_ELSE) {
                concatena = compara_pai(this->no_corrente); //reaproveitamento da variavel concatena
                if (concatena == FALSE_VALUE)
                    cout << "SOU DIFERENTE DO MEU PAI: " << this->no_corrente->id << this->no_corrente->no << endl;
                else {
                    cout << "SOU IGUAL AO MEU PAI: " << this->no_corrente->id << this->no_corrente->no << endl;
                    remove_objeto_da_arvore(0, NULL);
                }
                if (this->no_corrente->pai != NULL)
                    this->no_corrente = this->no_corrente->pai;

            }
        } while (pos != string::npos);
    }
    display_errors("verifica_tag fim: ");
}

int _arq::nova_linha(string &linha) {
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
    } else {
        cout << "fim_de arquivo" << endl;
        this->codigo.clear();
    }
    display_errors("nova_linha fim: ");
}

int _arq::verifica_condicional(string &linha) {
    int apos_cond, tipo, pos, cochete;
    string if_completo, auxiliar_nome;
    apos_cond = pos = tipo = cochete = 0;
    no_corrente->remove_space(linha);
    auxiliar_nome = "->";
    apos_cond = this->reg->reg_condicional_if(linha, tipo);
    if (apos_cond != FALSE_VALUE && tipo != REG_ELSE) {
        if_completo = no_corrente->no;
        if_completo += auxiliar_nome;
        if_completo += linha.substr(0, apos_cond);
        linha = linha.substr(apos_cond, linha.length());
        pos = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
        if (pos != FALSE_VALUE) {
            if_completo += linha.substr(0, pos);
            linha = linha.substr(pos, linha.length());
            cochete = verifica_pos_condicional(linha);
            this->no_corrente->novo_no(if_completo, cochete, this->id);
            this->id++;
            this->no_corrente = this->no_corrente->filho;
            if (tipo == REG_IF) {
                this->no_corrente->tipo_de_condicional = REG_IF;
            } else {
                this->no_corrente->tipo_de_condicional = REG_ELSEIF;
            }
            cout << "continuacao do if" << linha << endl;
        } else
            cout << "if sem sentido" << endl;
    } else if (tipo == REG_ELSE) {
        linha = linha.substr(apos_cond, linha.length());
        cochete = verifica_pos_condicional(linha);
        //this->no_corrente->novo_no(if_completo, cochete, this->id);
        this->id++;
        this->no_corrente->tipo_de_condicional = REG_ELSE;
        cout << "UM ELSE:" << linha.substr(apos_cond, linha.length()) << endl;
    }
}

int _arq::verifica_pos_condicional(string &linha) {
    this->no_corrente->remove_space(linha);
    if (linha[0] == '{') {
        cout << "POSSUI COLCHETES" << endl;
        linha = linha.substr(1, linha.length());
        cout << linha << endl;
        return 1;
    }
    cout << "NAO POSSUI COLCHETES" << endl;
    return 0;
}

int _arq::compara_pai(_intermediaria *corrente) {
    _intermediaria *pai;
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

int _arq::remove_objeto_da_arvore(int procura, _intermediaria *raiz) {
    int removido;
    _intermediaria *pai, *auxiliar, *irmao, *remove;
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
        cout << "TEM QUE ENTRAR AQUI" << remove->no << endl;
        pai->filho = NULL;
        delete remove;
    }
}
