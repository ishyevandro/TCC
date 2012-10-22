#include "classe_aspas.h"

void _aspas::call_clear() {
    int i;
    for (i = 0; i < MAX_REG; i++) {
        this->nome_atribuido[i].clear();
        this->aspas_valor[i].clear();
    }
}

string _aspas::novo_valor(string aspas, string tipo) {
    int nova_pos;
    ostringstream convert;
    nova_pos = primeiro_vazio();
    convert << nova_pos;
    this->nome_atribuido[nova_pos] = "$";
    this->nome_atribuido[nova_pos].append(convert.str());
    this->nome_atribuido[nova_pos].append(tipo);
    this->aspas_valor[nova_pos] = aspas;
    return this->nome_atribuido[nova_pos];
    cout << nova_pos << endl;
}

int _aspas::primeiro_vazio() {
    int i;
    i = 0;
    while (!this->nome_atribuido[i].empty()) {
        i++;
    }
    return i;
}

void _aspas::imprime_valores() {
    int i = 0;
    while (!this->nome_atribuido[i].empty()) {
        cout << this->nome_atribuido[i] << endl << this->aspas_valor[i] << endl;
        i++;
    }
}

string _aspas::busca_valor(string busca) {
    int loop;
    string vazia;
    vazia.empty();
    if (!this->nome_atribuido[0].empty()) {
        do {
            if (busca.compare(this->nome_atribuido[loop]) == 0) {
                //cout <<busca<<endl <<this->nome_atribuido[i]<<endl<<this->aspas_valor[i]<<endl;
                return this->aspas_valor[loop];
            }
            //cout << this->aspas_valor[i] << endl;
            ++loop;
           // cout << "Loop = " << loop << endl;
        } while (!this->nome_atribuido[loop].empty());
    }
    return vazia;
}