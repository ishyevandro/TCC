#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_var.cpp"

using namespace std;

class _arq{
protected:
	ifstream new_file;
public:
	_arq();
	void read ();
	void open(string);
};

_arq::_arq()
	:new_file(NULL){}

void _arq::open (string abrir)
{
	char *nome_do_arquivo;
	string linha;//linha lida do arquivo
	string sublinha;// ponto secundario para "encontrar" a variavel(final dela)
	int var_teste, cont_vetor = 0;
	regex_t rx_ini, rx_fim;//declaracao de "expressao regular" linha
	_var *vetor_de_variaveis;
	vetor_de_variaveis = new _var[10];
	string var;
	regmatch_t result;
	string Regex;
	Regex = "[\\=\\-\\ \\+\\.\\;]";
	
	if (regcomp (&rx_ini, "\\$", REG_EXTENDED)!= 0){//Expressao regular... descobrir como fazer aqui
	    printf ("Montagem da expressao com erro\n");
	    exit (-1);
	}
	if (regcomp (&rx_fim, Regex.c_str(), REG_EXTENDED)!= 0){//Expressao regular... descobrir como fazer aqui
	    printf ("Montagem da expressao com erro segunda parte\n");
	    exit (-1);
	}
	new_file.open (abrir.c_str(), ifstream::out);
	while (new_file.good())
	{
		getline (new_file, linha);
		var_teste =1;
		while (var_teste ==1)
		{
		    if (regexec(&rx_ini, linha.c_str(),(size_t)1,&result, 0)==0)
		    {
		        //cout << "Matched:" << "at" << (int)result.rm_so << "to"<<(int)result.rm_eo<< endl;// posicao
		        sublinha = linha.substr((int)result.rm_so, linha.length());//pegar a partir do ponto que ha uma variavel
		        if (regexec(&rx_fim, sublinha.c_str(),(size_t)1, &result, 0)==0)//encontrar o final da variavel
		        {
		            var = sublinha.substr(0, (int)result.rm_so);//pegando variavel
		            if (!if_var_array(vetor_de_variaveis,cont_vetor, var)){
		               cont_vetor++;
		            }
		            linha = sublinha.substr((int)result.rm_so, sublinha.length());//passando o restante do codigo para a linha para reanalizar e encontrar outras variaveis
		        }
		    }else{
		        var_teste =0;
		    }
		
		}
		linha.clear();
	}
}

int main (int argc, char **argv)
{
	_arq *A;
	A = new _arq;
	A->open("index.php");
	delete A;
	return 0;
}
