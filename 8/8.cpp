#include "main.h"
#include "ordinalParser.h"
#include "ordinalCalc.h"

#define equal equalll
#define less lesss
const int N = -1;
const long long INF = 1e9 + 19;




void read() {
    freopen("in", "r", stdin);
    //db("here");
    string s;
    getline(cin, s);
    s = removeSpace(s);
    auto tmp = split(s, "=");
    db("here");
    auto left = parseOrdinal(tmp[0]);
    db("here");
    auto right = parseOrdinal(tmp[1]);
   
    db("beforeLeft");
    auto resLeft = evaluate(left); 
    db("beforeRight");
    auto resRight = evaluate(right); 

    if (cmpO(resLeft, resRight) == 0)
        puts("Equal");
    else
        puts("Not equal");
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

