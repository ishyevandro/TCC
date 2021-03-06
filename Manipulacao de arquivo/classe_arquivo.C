#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_arquivo.h"

_arq::_arq()
: new_file(NULL),/* vetor_de_variaveis(NULL),*/ reg(NULL), vet_num(0) {
    //vetor_de_variaveis = new _var[20];
    this->vetor_de_variaveis.resize(VET_NUM);
    reg = new _reg;
}

void _arq::open(string abrir) {
    char *nome_do_arquivo;
    string linha; //linha lida do arquivo
    int var_teste, cont_vetor = 0;
    new_file.open(abrir.c_str(), ifstream::out);
    while (new_file.good()) {
        getline(new_file, linha);
        aux.get_line_to_analyse(linha, vetor_de_variaveis, vet_num, *reg);
        linha.clear();
    }
    vetor_de_variaveis[0].imprime_vetor(vetor_de_variaveis, vet_num);
}

int main(int argc, char **argv) {
    _arq *A;
    A = new _arq;
    A->open("index.php");
    delete A;
    return 0;
}
