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
    if (reg_comp_all("(\\$_POST[\\[ ]*)|(\\$_GET[\\[ ]*)", REG_P_G) != 0) {//Expressao regular... descobrir como fazer aqui
        cout << "POST erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }

    if (reg_comp_all("[\\$]", REG_$) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    
    if (reg_comp_all("^[\\$]", REG_VARIABLE) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    
    if (reg_comp_all("[^ ]", REG_FIRST) != 0) {//Alterar para escapar caracteres whitespace e tals
        cout << "Montagem da expressao com erro\n" << endl; //arrumar essa porcaria pra ler do arquivos as expressoes
        exit(-1);
    }
    /*RETIRAR O MENOS FUTURAMENTE POR CAUSA DE CLASSES verificacao depois da primeira variavel*/
    if (reg_comp_all("(\\=)|(\\-[^\\>])|(\\+)|(\\.)|(\\;)|( [\\-\\+\\=\\&. ])", REG_OPERATOR) != 0) {
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
    if (reg_comp_all("(^\\=[a-z|A-Z| ])", REG_OPERATOR_NORMAL) != 0) {
        cout << "Montagem da expressao com erro segunda parte\n" << endl;
        exit(-1);
    }
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
}

/*executa todas as expressoes regulares*/
int _reg::reg_exec_all(string verify, int type) {
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
            
            return (int) result.rm_so; //retorna posicao final
        }
    } else
        return -1;
}

int _reg::reg_exec_to_line(string line, int &pos) {//primeira string da linha.
    string subline;
    if (reg_exec_all(line, REG_$) == 0) {//achar o inicio da variavel
        subline = line.substr((int) result.rm_so, line.length()); //passa a verificar a partir do inicio da variavel
        pos = (int) result.rm_so; //passagem por referencia de onde comeca a string
        if (reg_exec_all(subline, REG_OPERATOR) == 0)
            return (int) result.rm_so; //retorna posicao final
    } else {
        return -1;
    }
}

int _reg::mount_reg_get_or_post(string line, string variables)//verifica se na linha tem alguma interacao de get ou post ou alguma variavel que recebeu um get ou post.
{
    if (!variables.empty()) {
        this->variables_with_p_or_g += variables;
        regfree(&first[REG_P_G]);
        if (reg_comp_all(this->variables_with_p_or_g, REG_P_G) != 0)
            cout << "Recompilar mount_reg_get_or_post erro"; //this->variables_with_p_or_g<<line<<endl;
    }
    if (reg_exec_all(line, REG_P_G) == 0) {//verifica se tem post ou get
        return 1;
    } else
        return -1;
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
}

int _reg::reg_comments(string line) {
    if (reg_exec_all(line, REG_COMMENTS_SIMPLE) == 0)
    {
        return (int) result.rm_so;
    }return -1;
}

int _reg::what_is_first_string(string line){
    if (reg_exec_all(line, REG_VARIABLE) == 0)
        return REG_VARIABLE;
    else
        return REG_FUNCTION;
}