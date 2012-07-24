#include "classe_reg.h"

_reg::_reg(): first(NULL), num_reg(0){}

void _reg::reg_comp ()//verificar se sera assim mesmo
{

    if (this->num_reg == 0)
        first = new regex_t[10];
    if (regcomp (&first[num_reg], "\\$", REG_EXTENDED)!= 0){//Expressao regular... descobrir como fazer aqui
	    cout <<"Montagem da expressao com erro\n"<<endl;//arrumar essa porcaria pra ler do arquivos as expressoes
	    exit (-1);
	}
	num_reg++;
		if (regcomp (&first[num_reg], "[\\=\\-\\ \\+\\.\\;]", REG_EXTENDED)!= 0){//Expressao regular... descobrir como fazer aqui
	    cout <<"Montagem da expressao com erro segunda parte\n"<<endl;
	    exit (-1);
	}
	num_reg++;
}

int _reg::reg_exec(string line, int &pos){
    string subline;
    if (regexec(&first[0], line.c_str(),(size_t)1,&result, 0)==0){//achar o inicio da variavel
      subline = line.substr((int)result.rm_so, line.length());//passa a verificar a partir do inicio da variavel
      pos =(int)result.rm_so;//passagem por referencia de onde comeca a string
      if (regexec(&first[1], subline.c_str(),(size_t)1, &result, 0)==0)//encontrar o final da variavel
       {
          return (int)result.rm_so;//retorna posicao final
       }
   }else
      return -1;
}


s
