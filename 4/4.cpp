#include "main.h"
#include "parser.h"


vector < Node * > hypoth;
vector < Node * > proof;
vector < Node * > axiom;
string mainHyp;

void read() {
    freopen("correct12.in", "r", stdin);
    //freopen("incorrect11.in", "r", stdin);
    cin.clear();
    string s;
    getline(cin, s);
    s = removeSpace(s);
    auto tmp = split(s, "|-");
    assert(tmp.size() == 2 || tmp.size() == 1);
    if (tmp.size() == 2) {
        auto hyp = split(tmp[0], ",");
        assert(!hyp.empty());
            mainHyp = hyp.back();
        for (auto s: hyp) {
            Parser p(s);
            hypoth.pb(p.parseExpr());
        }
    }

    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        proof.pb(p.parseExpr());
    }
    db(proof.size());
}

void init() {
    freopen("axiom.txt", "r", stdin);
    cin.clear();
    string s;
    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        axiom.pb(p.parseExpr());
    }
    db(axiom.size());
}

map < int, ull > matchPart;

bool fit(Node * v, Node * ax) {
    if (ax->type == "A" || ax->type == "a") {
        assert(ax->s.size() == 1);
        int key = ax->s[0];
        if (matchPart.count(key) == 0) 
            matchPart[key] = v->hash;
        else if (matchPart[key] != v->hash)
            return 0;
    }
    if (ax->type != v->type) return 0;
    bool flag = 1;
    assert(ax->l != NULL);
    if (ax->l != NULL) flag &= fit(v->l, ax->l);
    if (ax->r != NULL) flag &= fit(v->r, ax->r);

    return flag;
}

void solve() {

    for (int i = 0; i < (int)proof.size(); i++) {
        bool flag = 0;
        for (auto v: axiom) {
            matchPart.clear();
            if (fit(proof[i], v)) {
                flag = 1;
                
            }
        }


    




    }
}


int main() {
    init();
    read();
    solve();


    //for (int t = 0; t < 2; t++) {
        //string s;
        //getline(cin, s);
        //Parser parser(s);
        //Node * v = parser.parseExpr();
        //db(v->hash.val);
    //}

}


