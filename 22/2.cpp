#include "main.h"
#include "parser.h"
#include "deduction.h"

bool isSpace(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

string cutSpace(string s) {
    string tmp;
    for (int i = 0; i < (int)s.size(); i++)
        if (!isSpace(s[i]))
            tmp += s[i];
    return tmp;
}

void read() {
    string head;
    getline(cin, head);
    head = cutSpace(head);
    auto G = split(head, "|-");
    assert(G.size() == 2);
    Parser p(head);
    Line l(head);

    string s;
    vector < Line > line;
    for (; getline(cin, s) != 0; ) {
        s = cutSpace(s);
        s = G[0] + "|-" + s;
        //db2(s.size(), s);
        line.pb(Line(s));
    }
    Deduction ded(&line, l.assumption[0]);
    vector < Line > * res = ded.go();
    //for (int i = 0; i < (int)(*res)[0].assumption.size(); i++) {
        //cout << (*res)[0].assumption[i];
        //if (i + 1 != (int)(*res)[0].assumption.size())
            //cout << ",";
        //else
            //cout << "|-A\n";
    //}

    for (int i = 0; i < (int)res->size(); i++) {
        (*res)[i].print(0);
    }
}

void solve() {

}

void stress() {

}


int main(){
#ifdef DEBUG
    //freopen("in1", "r", stdin);
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


