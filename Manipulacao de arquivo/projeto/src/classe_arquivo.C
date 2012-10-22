#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_arquivo.h"

_arq::_arq()
: new_file(NULL), reg(NULL)/*,  vetor_de_variaveis(NULL), vet_num(0) */ {
    //vetor_de_variaveis = new _var[20];
    //this->vetor_de_variaveis.resize(VET_NUM);
    //this->aspas = new _aspas;
    tag_php = 0;
    reg = new _reg;
}

void _arq::open(string abrir) {
    char *nome_do_arquivo;
    string linha; //linha lida do arquivo
    int var_teste, cont_vetor = 0;
    new_file.open(abrir.c_str(), ifstream::out);
    while (new_file.good()) {
        this->aux.aspas->call_clear();
        getline(new_file, linha);
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
}

int _arq::aspas_duplas(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nova_linha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspasd(linha);
    if (inicio != FALSE_VALUE) {
        tipo = "d";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspasd(inicio_aspas);
            if (fim == FALSE_VALUE) {
                getline(new_file, nova_linha);
                inicio_aspas += nova_linha;
            } else {
                retorno = inicio_aspas.substr(0, fim);
                linha = linha.substr(1, inicio - 1);
                linha += this->aux.aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_duplas(linha);
    }
    return 1;
}

int _arq::remove_comments(string &line) {
    int type, comments, remove_primeiro, retorno;
    string comentario;
    retorno = -1;
    if (verifica_comentario_dentro_de_aspas(line) == -1)
        return 0;
    type = reg->reg_comments(line);
    if (type != FALSE_VALUE) {
        line = line.substr(0, type);
        retorno = 1;
    }
    type = reg->reg_comments_(line, REG_COMMENTS_INI);
    //cout<<"comentario grande"<<type<<endl;
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
                getline(new_file, comentario);
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
        aspas = reg->reg_verifica_aspasd(line);
        //cout<<aspas<<"!="<<comentario<<comentario2<<endl;
        if ((aspas < comentario || aspas < comentario2) && aspas != FALSE_VALUE && (comentario != FALSE_VALUE || comentario2 != FALSE_VALUE))
            return -1;
    }

    return 1;
}

int _arq::aspas_simples(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nova_linha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspass(linha);
    if (inicio != FALSE_VALUE) {
        tipo = "s";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspass(inicio_aspas);
            if (fim == FALSE_VALUE) {
                getline(new_file, nova_linha);
                inicio_aspas += nova_linha;
            } else {
                retorno = inicio_aspas.substr(0, fim);
                linha = linha.substr(1, inicio - 1);
                linha += this->aux.aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_simples(linha);
    }
    return 1;
}

int main(int argc, char **argv) {
    _arq *A;
    A = new _arq;
    A->open("/home/evandro/Dropbox/TCC/TCC/Manipulacao de arquivo/projeto/test/index.php");
    delete A;
    return 0;
}

string _arq::procura_fim_de_linha(string linha) {
    int pos;
    string nova_linha;
    do {
        pos = linha.find_first_of(';');
        if (pos == string::npos) {
            getline(this->new_file, nova_linha);
            aspas_duplas(nova_linha);
            aspas_simples(nova_linha);
            remove_comments(nova_linha);
            linha += nova_linha;
        }
    } while (pos == string::npos && new_file.good());
    return linha;
}

string _arq::replace_aspas(string linha) {
    string nova_linha, aspas, retorno, verifica;
    int pos, tipo, analise;
    analise = 1;
    verifica = linha;
    nova_linha = linha;
    while (analise == 1) {
        pos = this->reg->reg_retorna_variavel_aspas(verifica, 1);
        if (pos != FALSE_VALUE) {
            nova_linha = verifica.substr(0, pos);
            verifica = verifica.substr(pos, verifica.length());
            pos = this->reg->reg_retorna_variavel_aspas(verifica, 0);
            if (pos != FALSE_VALUE) {
                aspas = verifica.substr(0, pos);
                if (aspas[aspas.length() - 1] == 'd') {
                    nova_linha += '"';
                    tipo = 0;
                } else {
                    nova_linha += '\'';
                    tipo = 1;
                }
                verifica = verifica.substr(pos, verifica.length());
                retorno = this->aux.aspas->busca_valor(aspas);
                if (!retorno.empty())
                    nova_linha += retorno;
                else {
                    exit(-1);
                }
                if (tipo == 1)
                    nova_linha += '\'';
                else
                    nova_linha += '"';
                nova_linha += verifica;
                verifica = nova_linha;
            }
        } else {
            analise = 0;
        }
    }
    return nova_linha;
}

int _arq::verifica_tag(string &linha) {
    int pos, tag, concatena, analisa_ou_nao;
    string auxiliar;
    tag = pos = 0;
    concatena = FALSE_VALUE;
    auxiliar.clear();
    if (tag_php == 1)
        analisa_ou_nao = 1;
    else
        analisa_ou_nao = 0;
    do {
        tag = pos;
        pos = reg->reg_tag_php(linha, tag_php);
        // cout<<"Verifica TAG:"<<tag_php<<"E"<<"analisa"<<analisa_ou_nao<<endl;
        //cout<<linha<<endl;
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
        //cout << "empt" << linha << " ^^^^^^" << auxiliar << endl;
        if (concatena == 1)
            auxiliar += linha;
        linha = auxiliar;
    }
    if (analisa_ou_nao == 1) {
        do {
            pos = linha.find_first_of (";");
            if (pos  != string::npos){
                auxiliar = linha.substr(0, pos+1);
                linha = linha.substr(pos+1, linha.length());
                auxiliar = replace_aspas(auxiliar);
                //cout<<auxiliar<<endl;
                this->aux.verifica_linha(auxiliar);
            }
        } while (pos != string::npos);
    }
}