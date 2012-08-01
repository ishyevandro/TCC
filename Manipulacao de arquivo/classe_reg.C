#include "classe_reg.h"

_reg::_reg(): first(NULL), num_reg(0){
    reg_comp();
    
}

void _reg::reg_comp ()//verificar se sera assim mesmo
{
  this->variables_with_p_or_g.clear();
  if (this->num_reg == 0)
    this->first = new regex_t[10];
  if (regcomp (&first[REG_P_G], "(\\$_POST[\\[ ]*)|(\\$_GET[\\[ ]*)", REG_EXTENDED)!= 0){//Expressao regular... descobrir como fazer aqui
    cout <<"POST erro\n"<<endl;//arrumar essa porcaria pra ler do arquivos as expressoes
	  exit (-1);
	}
	
	 if (regcomp (&first[REG_$], "[\\$]", REG_EXTENDED)!= 0){//Alterar para escapar caracteres whitespace e tals
    cout <<"Montagem da expressao com erro\n"<<endl;//arrumar essa porcaria pra ler do arquivos as expressoes
    exit (-1);
	}
	
  if (regcomp (&first[REG_FIRST], "[^ ]", REG_EXTENDED)!= 0){//Alterar para escapar caracteres whitespace e tals
    cout <<"Montagem da expressao com erro\n"<<endl;//arrumar essa porcaria pra ler do arquivos as expressoes
    exit (-1);
	}
	/*RETIRAR O MENOS FUTURAMENTE POR CAUSA DE CLASSES verificacao depois da primeira variavel*/
	if (regcomp (&first[REG_OPERATOR], "(\\=)|(\\-[^\\>])|(\\+)|(\\.)|(\\;)|( [\\-\\+\\=\\&.])", REG_EXTENDED)!= 0){
    cout <<"Montagem da expressao com erro segunda parte\n"<<endl;
    exit (-1);
	}
	
	if (regcomp (&first[REG_OPERATOR_CAT], "(\\.\\=)", REG_EXTENDED)!= 0){
    cout <<"Montagem da expressao com erro segunda parte\n"<<endl;
    exit (-1);
	}
	
}



int _reg::reg_exec_first_string(string line, int &pos){//primeira string da linha.
    string subline;
    //cout <<line<<endl;
    if (regexec(&first[REG_FIRST], line.c_str(),(size_t)1,&result, 0)==0){//achar o inicio da variavel
      subline = line.substr((int)result.rm_so, line.length());//passa a verificar a partir do inicio da variavel
      pos =(int)result.rm_so;//passagem por referencia de onde comeca a string
      //cout << pos<<endl;
      if (regexec(&first[REG_OPERATOR], subline.c_str(),(size_t)1, &result, 0)==0)//encontrar o final da variavel
       {
          return (int)result.rm_so;//retorna posicao final
       }
   }else
      return -1;
}

int _reg::reg_exec(string line, int &pos){//primeira string da linha.
    string subline;
    if (regexec(&first[REG_$], line.c_str(),(size_t)1,&result, 0)==0){//achar o inicio da variavel
      subline = line.substr((int)result.rm_so, line.length());//passa a verificar a partir do inicio da variavel
      pos =(int)result.rm_so;//passagem por referencia de onde comeca a string
     //cout << pos<<endl;
      if (regexec(&first[REG_OPERATOR], subline.c_str(),(size_t)1, &result, 0)==0)//encontrar o final da variavel
       {
          return (int)result.rm_so;//retorna posicao final
       }
   }else
      return -1;
}


int _reg::mount_reg_get_or_post(string line, string variables)//verifica se na linha tem alguma interacao de get ou post ou alguma variavel que recebeu um get ou post.
{  
if (!variables.empty()){
  if (this->variables_with_p_or_g.empty())
    this->variables_with_p_or_g = "(\\$_POST*)|(\\$_GET*)|";
  this->variables_with_p_or_g += variables;
  regfree(&first[0]);
  if (regcomp (&first[REG_P_G], this->variables_with_p_or_g.c_str(), REG_EXTENDED)!= 0)
      cout<<" ";//this->variables_with_p_or_g<<line<<endl;
  }
if (regexec(&first[REG_P_G], line.c_str(),(size_t)1,&result, 0)==0){//verifica se tem post ou get
  return 1;
  }else
    return -1;
}
