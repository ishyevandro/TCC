
#ifndef _CLASSE_REG_H_
#define _CLASSE_REG_H_
#include <boost/regex.h>
#include <iostream>
using namespace std;

#define DISPLAY_ERROR 0

#define FALSE_VALUE -1
#define REG_P_G 0
#define REG_P_G_V 7// nao esta sendo utilizado
#define REG_$ 1
#define TRUE_VALUE 1
#define REG_FIRST 2
#define REG_OPERATOR 3
#define REG_FUNCTION 4
#define REG_VARIABLE 5
#define REG_O_P_G 6
#define REG_NUMBER 7
#define REG_ASPAS 8
#define REG_FUNCTION 9

/*operadores utilizados para passar algum valor ha variavel exemplo "="*/
#define REG_OPERATOR_NORMAL 20
#define REG_OPERATOR_SOMA 21
#define REG_OPERATOR_SUB 22
#define REG_OPERATOR_MULT 23
#define REG_OPERATOR_DIV 24
#define REG_OPERATOR_RES 25
#define REG_OPERATOR_CAT 26

/*Operadores de comparacao utilizados geralmente em if ou como retorno de 1 ou 0*/
#define REG_OPERATOR_COMPARE 27
#define REG_OPERATOR_COMPARE_IGUAL 28
#define REG_OPERATOR_COMPARE_DIFERENTE 29
#define REG_OPERATOR_COMPARE_MAIOR_MENOR 30
#define REG_OPERATOR_COMPARE_MAIOR_MENOR_IGUAL 31
#define REG_OPERATOR_COMPARE_DIFERENTE_TIPO 32
#define REG_OPERATOR_COMPARE_IGUAL_TIPO 33


/*operadores utilizados apos um operador anterior para formar um calculo ou concatenacao de string*/
#define REG_POS_OPERATOR_MAT 34
#define REG_POS_OPERATOR_CAT 35

/*Comentarios e Aspas*/
#define REG_COMMENTS_SIMPLE 40
#define REG_COMMENTS_INI 41
#define REG_COMMENTS_FIM 42
#define REG_ASPAS_D 43
#define REG_ASPAS_S 44
#define MAX_REG 100

/*Deteccao de parentese*/
#define REG_PARENTESE_I 50
#define REG_PARENTESE_F 51
#define REG_VAR_ASPAS 52

class _reg {
public:
    regex_t *first;
    int num_reg;
    string variables_with_p_or_g;
    regmatch_t result;
    _reg();
    void reg_comp();
    int reg_exec_all(string, int);
    int reg_comp_all(string, int);
    int reg_exec_to_line(string, int&);
    int reg_exec_first_string(string, int&);
    int reg_comments(string);
    int reg_comments_(string, int);
    int mount_reg_get_or_post(string, string);
    int reg_to_operator(string);
    int what_is_first_string(string);
    int reg_segunda_parte_linha(string );
    int reg_operador_cat_ou_aritmetico(string);
 //   int count_string(string, string);
    int reg_verifica_aspasd (string);
    int reg_verifica_aspass (string);
    int reg_verifica_parentese (string, int, int);
    int reg_verifica_operador_compara(string);
    int reg_remove_operador_compara(string);
    int reg_retorna_variavel_aspas (string, int);
//verificacao de funcoes
    
};
#endif

