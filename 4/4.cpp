#include "main.h"
#include "parser.h"


void read() {


}


int main() {


    read();


    for (int t = 0; t < 2; t++) {
        string s;
        getline(cin, s);
        Parser parser(s);
        Node * v = parser.parseExpr();
        db(v->hash.val);
    }

    


}


