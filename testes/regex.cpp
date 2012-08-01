#include <iostream>
#include <cstdio>
#include <boost/regex.h> //g++ -o teste regex.cpp -lboost_regex

using namespace std;

int main (int argc,char **argv)
{
 char buffer[1024];
 regex_t rx;
 regmatch_t match;
 size_t nmatch = 1;
 if (regcomp (&rx, "([a-z]|[A-Z])", REG_EXTENDED) != 0)
    {
    perror("regcomp");
    return 0;
    }
  if (regexec(&rx, " ANAA MARI A JESUS JOSE",(size_t)1,&match, 0)==0){//achar o inicio da variavel
  cout<<(int)match.rm_so<<endl;

}

return 0;
}
