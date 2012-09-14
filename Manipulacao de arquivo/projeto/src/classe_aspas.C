#include "classe_aspas.h"

void _aspas::call_clear(){
    int i;
    for (i=0;i<MAX_REG;i++){
        this->nome_atribuido[i].clear();
        this->aspas_valor[i].clear();
    }
}

string _aspas::novo_valor(string aspas){
    int nova_pos;
    ostringstream convert;
    nova_pos = primeiro_vazio();
    convert <<nova_pos;
    this->nome_atribuido[nova_pos] = "$";
    this->nome_atribuido[nova_pos].append(convert.str());
    this->aspas_valor[nova_pos] = aspas;
    return this->nome_atribuido[nova_pos]; 
    cout <<nova_pos<<endl;
}

int _aspas::primeiro_vazio(){
    int i;
    i = 0;
    while (!this->nome_atribuido[i].empty()){
        i++;
    } 
    return i;
}