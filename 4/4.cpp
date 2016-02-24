#include "main.h"
#include "parser.h"


void read() {
    string s;
    getline(cin, s);
    s = removeSpace(s);
}


int main() {
    freopen("in", "r", stdin);


    //read();


    for (int t = 0; t < 2; t++) {
        string s;
        getline(cin, s);
        Parser parser(s);
        Node * v = parser.parseExpr();
        db(v->hash.val);
    }

    


}


