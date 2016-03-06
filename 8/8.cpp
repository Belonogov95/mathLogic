#include "main.h"
#include "ordinalParser.h"
#include "ordinalCalc.h"

#define equal equalll
#define less lesss
const int N = -1;
const long long INF = 1e9 + 19;






void read() {
    string s;
    auto tmp = split(s, "=");
    auto left = parseOrdinal(tmp[0]);
    auto right = parseOrdinal(tmp[1]);
   
    auto resLeft = evaluate(left); 
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
#ifdef DEBUG
    freopen("in", "r", stdin);
    //freopen("out", "w", stdout);
#endif
    if (1) {
        int k = 1;
        for (int tt = 0; tt < k; tt++) {
            read();
            solve();
        }
    }
    else {
        stress();
    }

    return 0;
}

