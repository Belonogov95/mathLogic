#include "main.h"
#include "ordinalParser.h"
#include "ordinalCalc.h"

#define equal equalll
#define less lesss
const int N = -1;
const long long INF = 1e9 + 19;

char s[100];

void read() {
    vector < string > testsName; 
    for (int i = 1; i <= 7; i++){
        sprintf(s, "tests/different%d.in", i);
        testsName.pb(string(s));
    }
    for (int i = 1; i <= 7; i++){
        sprintf(s, "tests/equal%d.in", i);
        testsName.pb(string(s));
    }
    for (auto in: testsName) {

        freopen(in.data(), "r", stdin);
        cin.clear();
        string s;
        getline(cin, s);
        s = removeSpace(s);
        auto tmp = split(s, "=");
        auto left = parseOrdinal(tmp[0]);
        auto right = parseOrdinal(tmp[1]);
       
        auto resLeft = evaluate(left); 
        auto resRight = evaluate(right); 

        cerr << in << " = ";
        if (cmpO(resLeft, resRight) == 0)
            puts("Equal");
        else
            puts("Not equal");
    }
}

void solve() {

}

void stress() {

}

int main(){
    read();
    solve();

    return 0;
}

