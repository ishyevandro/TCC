/* 
 * File:   variavel.h
 * Author: evandro
 *
 * Created on August 3, 2012, 10:58 PM
 */

#ifndef VARIAVEL_H
#define	VARIAVEL_H
#include <iostream>

using namespace std;

class _variavel {
public:
    string variavel;
    int cast;
    string funcao;

    _variavel():variavel(),funcao() {
    variavel.clear();
    funcao.clear();
    }
};
#endif	/* VARIAVEL_H */

