#include <iostream>
#include "classe_intermediaria.h"

int _intermediaria::get_line_to_analyse(string line, vector<_var> &vetor_de_variaveis, int &vet_num, _reg reg) {
    int n, pos;
   // n = remove_comments(line, reg);
    remove_space(line);
    n = reg.what_is_first_string(line);
    //cout << n<<"()" <<line<<endl;
    if (n == REG_VARIABLE)
        vetor_de_variaveis[0].analyse_line(line, vetor_de_variaveis, vet_num, reg);
    else
        return -1;
    return 1;
}

void _intermediaria::remove_space(string &line) {
    size_t found;
    found = line.find_first_not_of(" ");
    if ((int) found < line.length())
         line = line.substr((int) found, line.length());
}
/*
int _intermediaria::remove_comments(string &line, _reg reg) {
    int type;
    type = reg.reg_comments(line);
    if (type != -1){

        line = line.substr(0, type);
    }return 0;
}
*/