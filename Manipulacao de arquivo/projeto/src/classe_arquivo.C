#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_arquivo.h"

_arq::_arq()
: new_file(NULL), /* vetor_de_variaveis(NULL),*/ reg(NULL), vet_num(0) {
    //vetor_de_variaveis = new _var[20];
    this->vetor_de_variaveis.resize(VET_NUM);
    this->aspas = new _aspas;
    reg = new _reg;
}

void _arq::open(string abrir) {
    char *nome_do_arquivo;
    string linha; //linha lida do arquivo
    int var_teste, cont_vetor = 0;
    new_file.open(abrir.c_str(), ifstream::out);
    while (new_file.good()) {
        this->aspas->call_clear();
        getline(new_file, linha);
        remove_comments(linha);
        //aspas_duplas(linha);
        //aspas_simples (linha);
        //cout<<"IMPRESSAO DA LINHA:"<<linha<<endl;
        aux.get_line_to_analyse(linha, vetor_de_variaveis, vet_num, *reg);
        linha.clear();
    }
    vetor_de_variaveis[0].imprime_vetor(vetor_de_variaveis, vet_num);
}

int _arq::aspas_duplas(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nova_linha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspasd(linha);
    if (inicio != FALSE_VALUE){
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
                linha += aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_duplas(linha);
    }
    return 1;
}

int _arq::aspas_simples(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nova_linha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspass(linha);
    if (inicio != FALSE_VALUE){
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
                linha += aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_simples(linha);
    }
    return 1;
}

int _arq::remove_comments(string &line) {
    int type, comments, remove_primeiro, retorno;
    string comentario;
    retorno = -1;
    //if (verifica_comentario_dentro_de_aspas (line) == -1)
    //    return 0;
    type = reg->reg_comments(line);
    if (type != FALSE_VALUE) {
        line = line.substr(0, type);
        retorno = 1;
    }
    type = reg->reg_comments_(line, REG_COMMENTS_INI);
    //cout<<"comentario grande"<<type<<endl;
    if (type != FALSE_VALUE) {
        remove_primeiro = type-2;//utilizado o remove_rpimeiro para remover a primeira forma de comentario ja que a reg_comments retorna a primeira posicao apos a cadeia de caracteres
        line = line.substr(0, remove_primeiro);
        comentario = line.substr(remove_primeiro, line.length());
        type = FALSE_VALUE;
        while (type == FALSE_VALUE) {
            type = reg->reg_comments_(comentario, REG_COMMENTS_FIM);
            if (type != FALSE_VALUE){
                line += comentario.substr(type, comentario.length());
                //cout <<"IMPRIME PORRA"<<comentario.substr(type, comentario.length())<<endl;
            }else {
                getline(new_file, comentario);
            }
        }
    }
    return 0;
}

int _arq::verifica_comentario_dentro_de_aspas(string line) {
    int comentario, comentario2, aspas, retorno;
        comentario = reg->reg_comments(line); //verifica a linha possui comentario
        if (comentario !=FALSE_VALUE)
            comentario = reg->reg_comments(line.substr(comentario, line.length()));
        comentario2 =reg->reg_comments_(line, REG_COMMENTS_INI);
        if (comentario2 !=FALSE_VALUE)
            comentario2 = reg->reg_comments(line.substr(comentario2, line.length()));
        if (comentario != FALSE_VALUE || comentario2 != FALSE_VALUE) {
            aspas = reg->reg_verifica_aspasd(line);
            //cout<<aspas<<"!="<<comentario<<comentario2<<endl;
            if ((aspas < comentario || aspas < comentario2) && aspas != FALSE_VALUE && (comentario != FALSE_VALUE || comentario2!= FALSE_VALUE))
                return -1;
        }

        return 1;
}

/*
int _arq::verifica_se_a_aspas_esta_dentro_de_comentario(){
}//NECESSARIO//
 */

int main(int argc, char **argv) {
    _arq *A;
    A = new _arq;
    A->open("/home/evandro/Dropbox/TCC/TCC/Manipulacao de arquivo/projeto/test/index.php");
    delete A;
    return 0;
}
