#include <iostream>
#include <string>
#include <fstream>
#include <boost/regex.h>
#include "classe_codigo.h"

void display_errors(string prnt) {
    if (DISPLAY_ERROR == 1)
        cout << prnt << endl;
}

_codigo::_codigo()
: reg(NULL), aspas(NULL), no_corrente(NULL), id(0), tag_php(0) {
    aspas = new _aspas;
    no_corrente = &aux;
    reg = new _reg;
    debug.clear();
}

int _codigo::linha(string codigo_todo) {//1Metodo chamado pelo editor
    string linha;
    int pos;
    display_errors("metodo LINHA inicio");
    this->codigo = codigo_todo;//Passa todo o codigo para um atributo codigo
    aux.id = this->id;//atribui ao auxiliar o valor 0
    id++;//incrementa para os ids dos outras condicionais
    apenas_php();//Deixa no atributo codigo apenas o PHP
    while (!this->codigo.empty()) {//loop para analisar todas as linhas
        if (nova_linha(linha) == TRUE_VALUE)// se for falso o retorno significa que chegou ao fim do arquivo
            verifica_tag(linha);//inicio das verificacoes
    }
    display_errors("metodo LINHA inicio");
}

int _codigo::aspas_duplas(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nlinha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspasd(linha, 0);
    if (linha.empty())
        return FALSE_VALUE;
    if (inicio != FALSE_VALUE) {
        tipo = "d";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspasd(inicio_aspas, 1);
            if (fim == FALSE_VALUE) {
                nova_linha(nlinha);
                inicio_aspas += nlinha;

            } else if (inicio_aspas[fim - 1] != '\\') {
                retorno += inicio_aspas.substr(0, fim);
                linha = linha.substr(0, inicio);
                linha += this->aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            } else {
                retorno += inicio_aspas.substr(0, fim);
                inicio_aspas = inicio_aspas.substr(fim + 1, inicio_aspas.length());
                fim = FALSE_VALUE;
            }
        }
        aspas_duplas(linha);
    }
    return 1;
}

int _codigo::remove_comments(string &line) {//remove comentarios da linha
    int type, comments, remove_primeiro, retorno;
    string comentario, verifica_xss;
    retorno = -1;
    if (verifica_comentario_dentro_de_aspas(line) == -1) {//verifica se o comentario nao esta dentro de aspas
        exit(1);
        return 0;
    }
    type = reg->reg_comments(line);//procura pelo comentario simples "//"
    if (type != FALSE_VALUE) {
        verifica_xss = line.substr(type + 2, line.length());//verifica se nao existe um XSS nesse ponto
        this->xss = reg->xss(verifica_xss);//verificacao com expressao regular
        line = line.substr(0, type);//retorna os caracteres antes do comentario
        retorno = 1;
    }
    type = reg->reg_comments_(line, REG_COMMENTS_INI);//verifica comentarios com barra asterisco e procura o final do comentario
    if (type != FALSE_VALUE) {
        remove_primeiro = type - 2; //utilizado o remove_rpimeiro para remover a primeira forma de comentario ja que a reg_comments retorna a primeira posicao apos a cadeia de caracteres
        line = line.substr(0, remove_primeiro);
        comentario = line.substr(remove_primeiro, line.length());
        type = FALSE_VALUE;//enquanto o type for falso significa que nao foi encontrado o final do comentario
        while (type == FALSE_VALUE) {
            type = reg->reg_comments_(comentario, REG_COMMENTS_FIM);//procura o final do comentario procurando por asterisco barra
            if (type != FALSE_VALUE) {
                line += comentario.substr(type, comentario.length());
            } else {
                nova_linha(comentario);
            }
        }
    }
    return 0;
}

int _codigo::verifica_comentario_dentro_de_aspas(string line) {
    int comentario, comentario2, aspas, retorno;
    comentario = reg->reg_comments(line); //verifica a linha possui comentario
    if (comentario != FALSE_VALUE)
        comentario = reg->reg_comments(line.substr(comentario, line.length()));
    comentario2 = reg->reg_comments_(line, REG_COMMENTS_INI);
    if (comentario2 != FALSE_VALUE)
        comentario2 = reg->reg_comments(line.substr(comentario2, line.length()));
    if (comentario != FALSE_VALUE || comentario2 != FALSE_VALUE) {
        aspas = reg->reg_verifica_aspasd(line, 0);
        if ((aspas < comentario || aspas < comentario2) && aspas != FALSE_VALUE && (comentario != FALSE_VALUE || comentario2 != FALSE_VALUE))
            return FALSE_VALUE;
    }

    return TRUE_VALUE;
}

int _codigo::aspas_simples(string &linha) {
    int inicio, fim;
    string retorno, inicio_aspas, nlinha, tipo;
    retorno.clear();
    fim = -1;
    inicio = reg->reg_verifica_aspass(linha, 0);
    if (inicio != FALSE_VALUE) {
        tipo = "s";
        inicio_aspas = linha.substr(inicio + 1, linha.length());
        while (fim == FALSE_VALUE) {
            fim = reg->reg_verifica_aspass(inicio_aspas, 1);
            if (fim == FALSE_VALUE) {
                nova_linha(nlinha);
                inicio_aspas += nlinha;
            } else {
                retorno = inicio_aspas.substr(0, fim);
                linha = linha.substr(0, inicio);
                linha += this->aspas->novo_valor(retorno, tipo);
                linha += inicio_aspas.substr(fim + 1, inicio_aspas.length());
            }
        }
        aspas_simples(linha);
    }
    return 1;
}

/*Este metodo procura o final, ponto e virgula, de uma linha*/
string _codigo::procura_fim_de_linha(string linha) {
    int pos;
    string nlinha;
    nlinha.clear();
    display_errors("Procura_fim_de_linha inicio: ");
    do {
        pos = linha.find_first_of(';');
        if (pos == string::npos) {
            nova_linha(nlinha);
            remove_comments(nlinha);
            aspas_duplas(nlinha);
            aspas_simples(nlinha);
            linha += nlinha;
        }
    } while (pos == string::npos && !this->codigo.empty());//Enquanto nao for encontrado o fim de uma linha nao sai desse loop
    display_errors("Procura_fim_de_linha fim: ");
    return linha;
}

string _codigo::replace_aspas(string linha) {
    string nlinha, aspas, retorno, verifica;
    int pos, tipo, analise;
    analise = 1;
    verifica = linha;
    nlinha = linha;
    display_errors("Replace_aspas inicio: ");
    while (analise == 1) {
        pos = this->reg->reg_retorna_variavel_aspas(verifica, 1);
        if (pos != FALSE_VALUE) {
            nlinha = verifica.substr(0, pos);
            verifica = verifica.substr(pos, verifica.length());
            pos = this->reg->reg_retorna_variavel_aspas(verifica, 0);
            if (pos != FALSE_VALUE) {
                aspas = verifica.substr(0, pos);
                if (aspas[aspas.length() - 1] == 'd') {
                    nlinha += '"';
                    tipo = 0;
                } else {
                    nlinha += '\'';
                    tipo = 1;
                }
                verifica = verifica.substr(pos, verifica.length());
                //2retorno = this->no_corrente->aspas->busca_valor(aspas);
                retorno = this->aspas->busca_valor(aspas);
                if (!retorno.empty())
                    nlinha += retorno;
                else {
                    exit(-1);
                }
                if (tipo == 1)
                    nlinha += '\'';
                else
                    nlinha += '"';
                nlinha += verifica;
                verifica = nlinha;
            }
        } else {
            analise = 0;
        }
    }
    this->aspas->call_clear();
    display_errors("Replace_aspas fim: ");
    return nlinha;
}

/*Metodo que faz a distincao de linha comum e linha com condicionais ou lacos*/
int _codigo::verifica_tag(string &linha) {//Faz a verificacao de analise futura
    int pos, tag, concatena, analisa_ou_nao, aux_cond;
    string auxiliar, retorno;
    tag = pos = 0;
    concatena = FALSE_VALUE;
    auxiliar.clear();
    display_errors("verifica_tag inicio: ");
    remove_comments(linha);//remove comentarios da linha
    aspas_duplas(linha);//remove aspas duplas
    aspas_simples(linha);//remove aspas simples
    linha = procura_fim_de_linha(linha);//verifica se a linha tem um ponto e virgula ao final
    do {
        no_corrente->remove_space(linha);
        pos = linha.find_first_of(";");
        aux_cond = fim_condicao(linha);//verifica se e o final de uma condicional
        if (pos != string::npos && aux_cond != TRUE_VALUE) {//se for encontrado o final de uma condicional nao entra aqui, e feito o devido tratamento para o objeto pai voltar a ser o no corrente para entao entrar nessa analise
            auxiliar = linha.substr(0, pos + 1);//copia a linha ate o ponto e virgula
            linha = linha.substr(pos + 1, linha.length());//a linha se torna o restante da linha, caso tenha mais de um codigo em uma unica linha.
            auxiliar = replace_aspas(auxiliar);//retorna todas as aspas simples e duplas para a linha
            verifica_condicional(auxiliar);//verificao se e uma condicional. Caso seja uma condicional e criado um novo objeto nesse metodo.
            retorno.clear();//apaga o retorno antes de envia-lo, passado por referencia dessa forma o conteudo exibido no debug vem essa variavel
            this->no_corrente->verifica_linha(auxiliar, retorno, 0, this->xss);//chamada de verificacao de linha
            this->xss = FALSE_VALUE;//seta XSS como falso para o proximo envio.
            this->debug += retorno;//adiciona o valor retornado, debug, a string que sera usada na aba debug
        }
        this->no_corrente->remove_space(linha);//sempre remove espaco em branco para ser feito a analise correta
        aux_cond = fim_condicao(linha);//verifica se e o final da condicao
        if (this->no_corrente->colchete == 0 && this->no_corrente->pai != NULL ) {//verifica se acabou a condicao e se pode ser retornado para o no pai
            concatena = compara_pai(this->no_corrente); //reaproveitamento da variavel concatena
            if (concatena == FALSE_VALUE)
                cout << "SOU DIFERENTE DO MEU PAI: " << this->no_corrente->id << this->no_corrente->no << endl;
            else {
                cout << "SOU IGUAL AO MEU PAI: " << this->no_corrente->id << this->no_corrente->no << endl;
                //1 experimental remove_objeto_da_arvore(this->no_corrente->id, NULL);
            }
            if (this->no_corrente->tipo_de_condicional == REG_ELSE/*2 remove_obje da arvore == FALSE_VALUE*/) {//1
                this->no_corrente->copia_para_pai();//caso seja else e copiado todo o valor para o pai
            }
            if (this->no_corrente->pai != NULL)
                this->no_corrente = this->no_corrente->pai;

        }
    } while (pos != string::npos);//loop ate chegar ao final da linha, nao encontra ponto e virgula
    display_errors("verifica_tag fim: ");
}

int _codigo::nova_linha(string &linha) {
    int pos;
    display_errors("nova_linha: ");
    pos = this->codigo.find_first_of("\n");//procura pelo primeiro fim de linha
    if (pos != string::npos) {//verifica se e uma posicao valida
        linha = this->codigo.substr(0, pos);//atribui a variavel linha a linha corrente
        this->codigo = this->codigo.substr(pos + 1, this->codigo.length());//anda com o codigo para ficar posterior a linha retirada
        return TRUE_VALUE;//retorna verdadeiro
    } else if (this->codigo.length() != 0) {//se nao encontra o fim de linha ele copia todo o valor dentro da variavel e limpa o codigo simbolizando o fim do codigo
        linha = this->codigo.substr(0, this->codigo.length());
        this->codigo.clear();
        return TRUE_VALUE;
    } else {
        this->codigo.clear();//apenas limpa o codigo
    }
    display_errors("nova_linha fim: ");
    return FALSE_VALUE;//retorna falso dando a entender que nao existe mais codigo
}

/*Original FUNCIONA CORRETAMENTE*/
int _codigo::verifica_condicional(string &linha) {
    int apos_cond, tipo, pos, colchete;
    string if_completo, auxiliar_nome;
    apos_cond = pos = tipo = colchete = 0;
    no_corrente->remove_space(linha);
    auxiliar_nome = "->";
    apos_cond = this->reg->reg_condicional_if(linha, tipo);
    verifica_laco(linha);
    if (apos_cond != FALSE_VALUE) {
        if_completo = no_corrente->no;
        if (tipo == REG_IF || tipo == REG_ELSEIF || tipo == REG_ELSE) {
            if_completo += auxiliar_nome;
            if_completo += linha.substr(0, apos_cond);
            linha = linha.substr(apos_cond, linha.length());
            pos = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
            if (pos != FALSE_VALUE) {
                if_completo += linha.substr(0, pos);
                linha = linha.substr(pos, linha.length());
                colchete = verifica_pos_condicional(linha);
                this->no_corrente->novo_no(if_completo, colchete, this->id, tipo);
                this->id++;
                this->no_corrente = this->no_corrente->filho;
            } else {
                colchete = verifica_pos_condicional(linha);
                this->no_corrente->novo_no(if_completo, colchete, this->id, tipo);
                this->id++;
                this->no_corrente = this->no_corrente->filho;
                this->no_corrente->tipo_de_condicional = REG_ELSE;
            }
        } else if (tipo == REG_SWITCH) {
            if_completo += auxiliar_nome;
            if_completo += linha.substr(0, apos_cond);
            linha = linha.substr(apos_cond, linha.length());
            pos = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
            if (pos != string::npos) {
                if_completo += linha.substr(0, pos);
                linha = linha.substr(pos, linha.length());
                colchete = verifica_pos_condicional(linha);
                this->no_corrente->novo_no(if_completo, colchete, this->id, tipo);
                this->id++;
                this->no_corrente = this->no_corrente->filho;
            }
            this->no_corrente->remove_space(linha);
            apos_cond = this->reg->reg_condicional_if(linha, tipo);
        }
        if (tipo == REG_CASE && no_corrente->tipo_de_condicional == REG_SWITCH) {
            if_completo += auxiliar_nome;
            do {
                apos_cond = linha.find_first_of(":");
                if (apos_cond != string::npos) {
                    if_completo += linha.substr(0, apos_cond + 1);
                    linha = linha.substr(apos_cond + 1, linha.length());
                } else 
                    return ERROR;
                no_corrente->remove_space(linha);
                apos_cond = this->reg->reg_condicional_if(linha, tipo);
            } while (apos_cond != FALSE_VALUE && tipo);
            this->no_corrente->novo_no(if_completo, 1, this->id, REG_CASE);
            this->id++;
            this->no_corrente = this->no_corrente->filho;
            this->no_corrente->tipo_de_condicional = REG_CASE;
        }
        verifica_condicional(linha);
        return tipo;
    } else
        return FALSE_VALUE;
}

int _codigo::verifica_pos_condicional(string &linha) {
    this->no_corrente->remove_space(linha);
    if (linha[0] == '{') {
        linha = linha.substr(1, linha.length());
        return 1;
    } else if (linha[0] == ':') {
        linha = linha.substr(1, linha.length());
        return -1;
    }
    return 0;
}

int _codigo::compara_pai(_linha *corrente) {//verifica se o filho e igual ao pai
    _linha *pai;
    int i, j;
    if (corrente->pai == NULL)
        return TRUE_VALUE;
    pai = corrente->pai;
    if (pai->vet_num != corrente->vet_num)
        return FALSE_VALUE;
    for (i = 0; i < pai->vet_num; i++) {
        if (pai->vetor_de_variaveis[i].variavel.compare(corrente->vetor_de_variaveis[i].variavel) != 0)
            return FALSE_VALUE;
        for (j = 0; j < pai->vetor_de_variaveis[i].n_var; j++) {
            if (pai->vetor_de_variaveis[i].vect[j].variavel.compare(corrente->vetor_de_variaveis[i].vect[j].variavel) != 0)
                return FALSE_VALUE;
            if (pai->vetor_de_variaveis[i].vect[j].funcao.compare(corrente->vetor_de_variaveis[i].vect[j].funcao) != 0)
                return FALSE_VALUE;
            if (pai->vetor_de_variaveis[i].vect[j].cast != corrente->vetor_de_variaveis[i].vect[j].cast)
                return FALSE_VALUE;
        }
    }
    return TRUE_VALUE;
}

int _codigo::remove_objeto_da_arvore(int procura, _linha *raiz) {//nao sendo utilizada, seu intuito era remover um objeto quando fosse igual ao pai apos a analisa.
    int removido;
    _linha *pai, *auxiliar, *irmao, *remove;
    remove = this->no_corrente;
    pai = this->no_corrente->pai;
    if (remove->irmao != NULL) {
        pai->filho = remove->irmao;
        irmao = remove->irmao;
        if (remove->filho != NULL) {
            auxiliar = remove->filho;
            while (auxiliar->irmao != NULL) {
                auxiliar = auxiliar->irmao;
            }
            auxiliar->irmao = irmao->filho;
            irmao->filho = remove->filho;
            delete remove;
        } else
            delete remove;
    } else if (remove->filho != NULL) {
        pai->filho = remove->filho;
        auxiliar = remove->filho;
        if (auxiliar->irmao != NULL) {
            irmao = auxiliar;
            do {
                irmao->pai = pai;
                irmao = irmao->irmao;
            } while (irmao->irmao != NULL);
        }
        delete remove;
    } else {
        pai->filho = NULL;
        delete remove;
    }
    return TRUE_VALUE;
}

int _codigo::fim_condicao(string &line) {//verifica o final da condicao
    int retorno;
    no_corrente->remove_space(line);
    if (line[0] == '}' && no_corrente->colchete > 0) {
        this->no_corrente->colchete--;
        line = line.substr(1, line.length());
        fim_condicao(line);
        return TRUE_VALUE;
    } else if (no_corrente->dois_pontos > 0 && no_corrente->tipo_de_condicional != REG_CASE) {

    } else if (no_corrente->tipo_de_condicional == REG_CASE) {
        retorno = reg->reg_break_condicionais(line, REG_CASE);
        if (retorno != FALSE_VALUE) {
            line = line.substr(retorno, line.length());
            this->no_corrente->colchete = 0;
            return TRUE_VALUE;
        } else return FALSE_VALUE;
    }
    return FALSE_VALUE;
}

int _codigo::apenas_php() {//torna o atributo codigo apenas conteudo PHP. Retirando HTMl e qualquer script cliente side
    string aux, auxiliar;
    int pos, tag, concatena;
    pos = tag = concatena = 0;
    aux = this->codigo;
    auxiliar.clear();
    do {
        tag = pos;
        pos = reg->reg_tag_php(aux, tag_php);
        if (pos != FALSE_VALUE) {
            if (tag_php == 0) {
                aux = aux.substr(pos, aux.length());
                tag_php = 1;
                if (concatena != FALSE_VALUE)
                    concatena = 1;
            } else if (tag_php == 1) {
                auxiliar += aux.substr(0, pos);
                aux = aux.substr(pos, aux.length());
                concatena = 0;
                tag_php = 0;
            }
        }
    } while (tag != FALSE_VALUE);
    this->codigo = auxiliar;
    return TRUE_VALUE;
}

int _codigo::verifica_laco(string &linha) {//se for um laco ele simplesmente "pula o laco"
    int ret_reg, tipo;
    this->no_corrente->remove_space(linha);
    ret_reg = this->reg->reg_laco(linha, tipo);
    if (ret_reg != FALSE_VALUE) {
        linha = linha.substr(ret_reg, linha.length());
        if (tipo == REG_LOOP) {
            ret_reg = this->no_corrente->retorna_elementos_dentro_de_parentese(linha);
            if (ret_reg != FALSE_VALUE)
                linha = linha.substr(ret_reg, linha.length());

        }
        this->no_corrente->remove_space(linha);
        if (linha[0] == '{') {
            if (this->no_corrente->colchete > 0)
                this->no_corrente->colchete++;
        }
    }
}