#include "main.h"
#include "parser.cpp"


const int N = -1;
const long long INF = 1e9 + 19;


void read() {
    string s;
    cin >> s;
    Parser p(s);
    Node * v = p.parse();
    db("here");
    p.printTree(v, 0);
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


