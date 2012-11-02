#include "classe_reg.h"

_reg::_reg() : first(NULL), num_reg(0) {
    reg_comp();

}

int _reg::reg_comp_all(string reg, int type) {
    return regcomp(&first[type], reg.c_str(), REG_EXTENDED);
}

void _reg::reg_comp()//verificar se sera assim mesmo
{
    this->variables_with_p_or_g.clear();
    if (this->num_reg == 0)
        this->first = new regex_t[MAX_REG];
    /* if (reg_comp_all("[\\$]", REG_$) != 0) {//encontr
         cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
         exit(-1);
     }*/

    if (reg_comp_all("^[\\$]", REG_VARIABLE) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    if (reg_comp_all("^[^\\$^\"^\'^[:digit:]^\\(]", REG_FUNCTION) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    // if (reg_comp_all("^[^[:digit:]^\"^\"]", REG_NUMBER) != 0) {//Alterar para escapar caracteres whitespace e tals
    if (reg_comp_all("^[[:digit:]]", REG_NUMBER) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    if (reg_comp_all("^[\"|\']", REG_ASPAS) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    if (reg_comp_all("(^\\$_POST[\\[ ]*)|(^\\$_GET[\\[ ]*)", REG_O_P_G) != 0) {//verifica se a string corrente 'e um GET ou POST
        cout << "POST erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    if (reg_comp_all("(\\$_POST[\\[ ]*)|(\\$_GET[\\[ ]*)", REG_P_G) != 0) {//Expressao regular... descobrir como fazer aqui
        cout << "POST erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }

    if (reg_comp_all("[^ ]", REG_FIRST) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    /*#verificacao erronea... fazer multiplas verificacaos no first string para chegar a usar isso*/
    //if (reg_comp_all("(,)|(\\))(\\=)|(\\-[^\\>])|(\\+)|(\\.)|(\\;)|( [\\-\\+\\=\\&. ])", REG_OPERATOR) != 0) {
    if (reg_comp_all("(,)|(\\))|(\\=)|(\\-[^\\>])|(\\+)|(\\.)|(\\;)| |\\[", REG_OPERATOR) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }

    /*OPERADORES TIPO DE OPERACAO*/
    if (reg_comp_all("(^\\.\\=)", REG_OPERATOR_CAT) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }

    if (reg_comp_all("(^\\+\\=)", REG_OPERATOR_SOMA) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(^\\-\\=)", REG_OPERATOR_SUB) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(^\\*\\=)", REG_OPERATOR_MULT) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(^\\/\\=)", REG_OPERATOR_DIV) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(^\\%\\=)", REG_OPERATOR_RES) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(^\\=[a-z|A-Z| |\\$|\\(])", REG_OPERATOR_NORMAL) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
    /*OPERADORES POS OPERADOR DE ATRIBUICAO*/
    if (reg_comp_all("(^[\\.])", REG_POS_OPERATOR_CAT) != 0) {//verifica se a string corrente 'e um GET ou POST
        cout << "POST erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }//\\*\\%\\/
    if (reg_comp_all("(^[\\+|-|\\*|/|%])", REG_POS_OPERATOR_MAT) != 0) {//verifica se a string corrente 'e um GET ou POST
        cout << "POST erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    //OPERADORES DE COMPARACAO
    if (reg_comp_all("^([<>]=)|^(==)|(^===)|^(!=)|^(!==)|^([<>])", REG_OPERATOR_COMPARE) != 0) {
        cout << "Reg operador com erro" << endl;
        exit(-1);
    }
    if (reg_comp_all("^([<>])", REG_OPERATOR_COMPARE_MAIOR_MENOR) != 0) {
        cout << "Reg operador com erro" << endl;
        exit(-1);
    }
    /* if (reg_comp_all ("^(!==)", REG_OPERATOR_COMPARE_DIFERENTE_TIPO) !=0){
         cout <<"Reg operador com erro"<<endl;
         exit(-1);
     }*/
    if (reg_comp_all("^(!=)", REG_OPERATOR_COMPARE_DIFERENTE) != 0) {
        cout << "Reg operador com erro" << endl;
        exit(-1);
    }
    /*if (reg_comp_all ("^(===)", REG_OPERATOR_COMPARE_IGUAL_TIPO) !=0){
        cout <<"Reg operador com erro"<<endl;
        exit(-1);
    }*/
    if (reg_comp_all("^(==)", REG_OPERATOR_COMPARE_IGUAL) != 0) {
        cout << "Reg operador com erro" << endl;
        exit(-1);
    }
    if (reg_comp_all("^([<>]=)", REG_OPERATOR_COMPARE_MAIOR_MENOR_IGUAL) != 0) {
        cout << "Reg operador com erro" << endl;
        exit(-1);
    }


    /*PARTE PARA COMENTARIOS E ASPAS*/

    if (reg_comp_all("(//)", REG_COMMENTS_SIMPLE) != 0) {
        cout << "Montagem da expressao de comentario simples com erro\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(/\\*)", REG_COMMENTS_INI) != 0) {
        cout << "Montagem da expressao de comentario simples com erro\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("(\\*/)", REG_COMMENTS_FIM) != 0) {
        cout << "Montagem da expressao de comentario simples com erro\n" << endl;
        exit(-1);
    }
    if (reg_comp_all("\"", REG_ASPAS_D_I) != 0) {
        cout << "Erro na montagem: Expressao de aspas duplas" << endl;
    }

    if (reg_comp_all("\"", REG_ASPAS_D_F) != 0) {
        cout << "Erro na montagem: Expressao de aspas duplas" << endl;
    }
    if (reg_comp_all("\'", REG_ASPAS_S_I) != 0) {
        cout << "Erro na montagem: Expressao de aspas simples INICIO" << endl;
    }//mais proximo de certo [^\\\\]
    if (reg_comp_all("(\')", REG_ASPAS_S_F) != 0) {
        cout << "Erro na montagem: Expressao de aspas simples FIM" << endl;
    }
    if (reg_comp_all("(\\$[[:digit:]]{1,}[ds])", REG_VAR_ASPAS) != 0) {
        cout << "Erro na montagem: Expressao de aspas simples" << endl;
    }
    if (reg_comp_all("[(]", REG_PARENTESE_I) != 0) {
        cout << "Erro na montagem: Expressao de parentese inicial" << endl;
    }
    if (reg_comp_all("[)]", REG_PARENTESE_F) != 0) {
        cout << "Erro na montagem: Expressao de parentese final" << endl;
    }

    //TAG PHP
    if (reg_comp_all("(<\\?)|(<\\?php)", REG_INI_PHP) != 0) {
        cout << "Erro na montagem: Expressao de parentese final" << endl;
    }
    if (reg_comp_all("(\\?>)", REG_FIM_PHP) != 0) {
        cout << "Erro na montagem: Expressao de parentese final" << endl;
    }

    /*Condicionais*/
    if (reg_comp_all("^([Ii][Ff])", REG_IF) != 0) {
        cout << "Erro na montagem: Expressao de parentese final" << endl;
    }
    if (reg_comp_all("^([Ee][Ll][Ss][Ee][Ii][Ff]|[Ee][Ll][Ss][Ee] *[Ii][Ff])", REG_ELSEIF) != 0) {
        cout << "Erro na montagem: Expressao de parentese final" << endl;
    }
    if (reg_comp_all("^([Ee][Ll][Ss][Ee])", REG_ELSE) != 0) {
        cout << "Erro na montagem: Expressao de parentese final" << endl;
    }

    if (reg_comp_all("^(switch)", REG_SWITCH) != 0) {
        cout << "Erro na montagem: Expressao de switch" << endl;
    }
    if (reg_comp_all("^(case)", REG_CASE) != 0) {
        cout << "Erro na montagem: Expressao de case" << endl;
    }
    if (reg_comp_all("^(break)", REG_BREAK) != 0) {
        cout << "Erro na montagem: expressao break";
    }
    if (reg_comp_all("^(while)|^(for)|^(foreach)", REG_LOOP) != 0) {
        cout << "Erro na montagem: expressao break";
    }
    if (reg_comp_all("^(do)", REG_LOOP_DO) != 0) {
        cout << "Erro na montagem: expressao break";
    }
    /*Deteccao de CAST dentro de aspas*/
    if (reg_comp_all("( *int *\\))|( *integer *\\))|( *float *\\))|( *real *\\))|( *double *\\)) ", REG_INT) != 0)
        cout << "Erro na montagem: Expressao regular cast" << endl;
}

/*executa todas as expressoes regulares*/
int _reg::reg_exec_all(string verify, int type) {
    // cout << "warning" << type << endl;
    return regexec(&first[type], verify.c_str(), (size_t) 1, &result, 0);
}

/*talvez desnecessario*/
int _reg::reg_exec_first_string(string line, int &pos) {//primeira string da linha.
    string subline;
    /*Com funcao remove_space nao e necessario essa primeira expressao regular*/
    if (reg_exec_all(line, REG_FIRST) == 0) {//achar o inicio da variavel
        subline = line.substr((int) result.rm_so, line.length()); //passa a verificar a partir do inicio da variavel
        pos = (int) result.rm_so; //passagem por referencia de onde comeca a string
        /*NECESSARIO ADICIONAR A VERIFICACAO DE FUNCAO AQUI*/
        if (reg_exec_all(line, REG_OPERATOR) == 0)//ARRUMAR AQUI TALVEZ SEJA BOM RETORNAR O TIPO CORRETO DE OPERACAO JA
        {

            return (int) result.rm_so; //retorna posicao final da variavel e o comeco do operador;
        }
    } else
        return -1;
}

int _reg::mount_reg_get_or_post(string line, string variables)//verifica se na linha tem alguma interacao de get ou post ou alguma variavel que recebeu um get ou post.
{
    if (!variables.empty()) {
        this->variables_with_p_or_g = variables;
        regfree(&first[REG_P_G]);
        if (reg_comp_all(this->variables_with_p_or_g, REG_P_G) != 0)
            cout << "Recompilar mount_reg_get_or_post erro"; //this->variables_with_p_or_g<<line<<endl;
    }
    if (reg_exec_all(line, REG_P_G) == 0) {//verifica se tem post ou get
        //  cout << "REG_MOUNT:" << line << endl << this->variables_with_p_or_g << endl;
        return TRUE_VALUE;
    } else {
        // cout << "FALSE_VALUE:" << line <<endl << this->variables_with_p_or_g << endl;
        return FALSE_VALUE;
    }
}

/*RETORNA O TIPO DE OPERADOR QUE ESTA APOS A PRIMEIRA STRING*/
int _reg::reg_to_operator(string line) {
    if (reg_exec_all(line, REG_OPERATOR_NORMAL) == 0)
        return REG_OPERATOR_NORMAL;
    if (reg_exec_all(line, REG_OPERATOR_SOMA) == 0)
        return REG_OPERATOR_SOMA;
    if (reg_exec_all(line, REG_OPERATOR_SUB) == 0)
        return REG_OPERATOR_SUB;
    if (reg_exec_all(line, REG_OPERATOR_MULT) == 0)
        return REG_OPERATOR_MULT;
    if (reg_exec_all(line, REG_OPERATOR_DIV) == 0)
        return REG_OPERATOR_DIV;
    if (reg_exec_all(line, REG_OPERATOR_CAT) == 0)
        return REG_OPERATOR_CAT;
    else
        cout << "ERRO nos operadores" << endl;
    return FALSE_VALUE;
}

int _reg::reg_comments(string line) {
    if (reg_exec_all(line, REG_COMMENTS_SIMPLE) == 0) {
        return (int) result.rm_so;
    }
    return FALSE_VALUE;
}

int _reg::what_is_first_string(string line) {//AQUI ESTA O POSSIVEL ERRO
    if (reg_exec_all(line, REG_VARIABLE) == 0)
        return REG_VARIABLE;
    else if (reg_exec_all(line, REG_FUNCTION) == 0) {
        line = line.substr(result.rm_so, line.length()); //##verificar corretamente sobre os defines em php
        return REG_FUNCTION;
    } else if (reg_exec_all(line, REG_NUMBER) == 0) {
        //cout<<"FAZER UM RETORNO PARA NUMERO: "<<line<<endl;
        return REG_NUMBER;
    } else if (reg_exec_all(line, REG_ASPAS) == 0)
        return REG_ASPAS;
    else if (reg_exec_all(line, REG_OPERATOR_CAT) == 0)
        return REG_POS_OPERATOR_CAT;
    else if (reg_exec_all(line, REG_POS_OPERATOR_MAT) == 0)
        return REG_POS_OPERATOR_MAT;
    else if (reg_exec_all(line, REG_PARENTESE_I) == 0)
        return REG_PARENTESE_I;
}

int _reg::reg_segunda_parte_linha(string subline) {
    if (reg_exec_all(subline, REG_O_P_G) == 0)
        return TRUE_VALUE;
    else
        return FALSE_VALUE;
}

int _reg::reg_operador_cat_ou_aritmetico(string subline) {
    if (reg_exec_all(subline, REG_POS_OPERATOR_CAT) == 0)
        return REG_POS_OPERATOR_CAT;
    else if (reg_exec_all(subline, REG_POS_OPERATOR_MAT) == 0) {//###TEM ALGUM ERRO AQUI ESTRANHO SE ESTIVER ANTES DO PONTO ELE PEGA PONTO COMO OPERADOR MATEMATICO
        // cout << subline << "QUE PUTARIA E ESSA?" << subline[result.rm_so] << endl;
        return REG_POS_OPERATOR_MAT;
    } else
        return FALSE_VALUE;
}

int _reg::reg_verifica_aspasd(string line, int tipo) {
    int execute;
    if (tipo == 0)
        execute = REG_ASPAS_D_I;
    else
        execute = REG_ASPAS_D_F;
    if (reg_exec_all(line, execute) == 0) {
        return result.rm_so;
    }
    return FALSE_VALUE;
}

int _reg::reg_verifica_aspass(string line, int tipo) {
    int execute;
    if (tipo == 0)
        execute = REG_ASPAS_S_I;
    else
        execute = REG_ASPAS_S_F;
    //cout << execute << "(^\\\\)\'" << line << endl;
    if (reg_exec_all(line, execute) == 0) {
        //  cout <<"encontrado "<<execute << endl;
        return result.rm_so;
    }
    return FALSE_VALUE;
}

int _reg::reg_comments_(string line, int type) {
    if (reg_exec_all(line, type) == 0) {
        return result.rm_eo;
    }
    return FALSE_VALUE;
}

int _reg::reg_verifica_parentese(string line, int tipo_paren, int retorno) {
    if (reg_exec_all(line, tipo_paren) == 0) {
        if (retorno == 0)
            return result.rm_so;
        return result.rm_eo;
    }
    return FALSE_VALUE;
}

int _reg::reg_verifica_operador_compara(string line) {
    if (reg_exec_all(line, REG_OPERATOR_COMPARE) == 0) {
        if (reg_exec_all(line, REG_OPERATOR_COMPARE_IGUAL) == 0)
            return REG_OPERATOR_COMPARE_IGUAL;
        else if (reg_exec_all(line, REG_OPERATOR_COMPARE_DIFERENTE) == 0)
            return REG_OPERATOR_COMPARE_DIFERENTE;
        else if (reg_exec_all(line, REG_OPERATOR_COMPARE_MAIOR_MENOR) == 0)
            return REG_OPERATOR_COMPARE_MAIOR_MENOR;
        else if (reg_exec_all(line, REG_OPERATOR_COMPARE_MAIOR_MENOR_IGUAL) == 0)
            return REG_OPERATOR_COMPARE_MAIOR_MENOR_IGUAL;
        else
            return -2;
    } else
        return FALSE_VALUE;
}

int _reg::reg_remove_operador_compara(string line) {
    if (reg_exec_all(line, REG_OPERATOR_COMPARE) == 0)
        return result.rm_eo;
    return FALSE_VALUE;
}

int _reg::reg_retorna_variavel_aspas(string line, int retorno) {
    if (reg_exec_all(line, REG_VAR_ASPAS) == 0) {
        if (retorno == 1)
            return result.rm_so;
        else
            return result.rm_eo;
    }
    return FALSE_VALUE;
}

int _reg::reg_tag_php(string line, int type) {
    if (type == 0) {
        if (reg_exec_all(line, REG_INI_PHP) == 0)
            return result.rm_eo;
    } else {
        if (reg_exec_all(line, REG_FIM_PHP) == 0)
            return result.rm_so;
    }
    return FALSE_VALUE;
}

int _reg::reg_condicional_if(string line, int &tipo) {

    if (reg_exec_all(line, REG_IF) == 0) {
        tipo = REG_IF;
        return result.rm_eo;
    } else if (reg_exec_all(line, REG_ELSEIF) == 0) {
        tipo = REG_ELSEIF;
        return result.rm_eo;
    } else if (reg_exec_all(line, REG_SWITCH) == 0) {
        tipo = REG_SWITCH;
        return result.rm_eo;
    } else if (reg_exec_all(line, REG_CASE) == 0) {
        cout << line << endl << line << endl << endl << endl << endl << endl;
        tipo = REG_CASE;
        return result.rm_eo;
    } else
        return FALSE_VALUE;
}

int _reg::reg_laco(string line, int &tipo) {
    if (reg_exec_all(line, REG_LOOP) == 0) {
        tipo = REG_LOOP;
        return result.rm_eo;
    } else if (reg_exec_all(line, REG_LOOP_DO) == 0) {
        tipo = REG_LOOP_DO;
        return result.rm_eo;
    }
    return FALSE_VALUE;
}

int _reg::reg_cast(string line) {
    if (reg_exec_all(line, REG_INT) == 0)
        return TRUE_VALUE;
    return FALSE_VALUE;
}

int _reg::reg_break_condicionais(string line, int tipo) {
    int procura;
    switch (tipo) {
        case REG_IF:
        case REG_ELSE:
        case REG_ELSEIF:
            procura = REG_ENDIF;
            break;
        case REG_SWITCH:
            procura = REG_ENDSWITCH;
            break;
        case REG_CASE:
            procura = REG_BREAK;
            break;

    }
    if (reg_exec_all(line, procura) == 0)
        return result.rm_so;
    return FALSE_VALUE;
}