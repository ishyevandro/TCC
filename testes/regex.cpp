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
 if (regcomp (&rx, argv[1], REG_EXTENDED)!= 0)
    {
    perror("regcomp");
    return 0;
    }
 
 while (!feof(stdin))
    {
    fgets(buffer,1024,stdin);
     
    if (regexec (&rx,buffer,1,&match,0)==0)
       {
       printf("Matched:(%s) at %d to %i",buffer,(int)match.rm_so, (int)match.rm_eo);
       }
 
    }
regfree (&rx);
return 0;
}
