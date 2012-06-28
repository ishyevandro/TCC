#include <iostream>
#include <istream>
#include <string>
#include <fstream>
#include <boost/regex.h>

using namespace std;


class x{
public:
    int num;
    x(int a)
        :num(a){}
    x& operator =(const x &outro){
     this->num = outro.num;
     return *this;
    }
};

int main()
{
    x *z;
    x s(10);
    cout<<s.num<<endl;
    z = new x(11);
    cout << z->num << endl;
    *z = s;
    cout<<z->num<<endl;
    return 0;
}
