/* 
 * File:   classe_aspas.h
 * Author: evandro
 *
 * Created on September 12, 2012, 10:53 PM
 */

#ifndef _CLASSE_ASPAS_H_
#define	_CLASSE_ASPAS_H_
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "classe_reg.h"

class _aspas{
public:
  string nome_atribuido[MAX_REG];
  string aspas_valor[MAX_REG];
  void call_clear();
  string novo_valor(string, string);
  void busca_valor();
  int primeiro_vazio();
  _aspas():nome_atribuido(),aspas_valor(){
      call_clear();
  }
};

#endif	/* CLASSE_ASPAS_H */

