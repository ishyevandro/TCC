#ifndef __INTERMEDIARE_
#define __INTERMEDIARE_
#include "classe_reg.h"
#include "classe_var.h"
//#include "classe_funcao.h"

using namespace std;

class _intermediaria{
public:
    int get_line_to_analyse(string , _var *, int &, _reg);
    void remove_space(string&);
    int remove_comments(string&, _reg);
};

#endif