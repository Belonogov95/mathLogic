#include "main.h"
#include "parser.h"

vector < Node * > hypoth;
vector < Node * > proof;
vector < Node * > axiom;
vector < Node * > mathAxiom;
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

vector < string > rule2; 
vector < string > rule3_1;
vector < string > rule3_2;

void init() {
    freopen("axiom_scheme.txt", "r", stdin);
    cin.clear();
    string s;
    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        axiom.pb(p.parseExpr());
    }
    db(axiom.size());

    freopen("axiom_math.txt", "r", stdin);
    cin.clear();

    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        mathAxiom.pb(p.parseExpr());
    }

    freopen("rule_2.txt", "r", stdin);
    cin.clear();
    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        rule2.pb(p.parseExpr()->s);
    }

    freopen("ruleAny_part1.txt", "r", stdin);
    cin.clear();
    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        rule3_1.pb(p.parseExpr()->s);
    }


    freopen("ruleAny_part2.txt", "r", stdin);
    cin.clear();
    for (; getline(cin, s); ) {
        s = removeSpace(s);
        if (s.empty()) continue;
        Parser p(s);
        rule3_2.pb(p.parseExpr()->s);
    }
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

void makeAA(string term) {
    vector < string > tmp;
    tmp.pb("(A)->(A)->(A)");
    tmp.pb("(A)->((A)->(A))->(A)");
    tmp.pb("((A)->(A)->(A))->((A)->((A)->(A))->(A))->((A)->(A))");
    tmp.pb("((A)->((A)->(A))->(A))->((A)->(A))");
    tmp.pb("(A)->(A)");
    for (auto s: tmp) {
        cout << replace(s, 'A', term) << endl;
    }
}

map < ull, string > exprFromHash;
set < ull > proved;
map < ull, set < ull > > need;

Node * createExpCopy(Node * v) {
    Parser p(v->s);
    return p.parseExpr();
}

bool flagFail;

pair < Node *, int > makeSubst(Node * v, string from, Node * to, set < string > blocked, bool flag) {
    if (v->type == "?" || v->type == "@") {
        assert(v->l->type == "a");
        if (v->l->s == from) {
            return mp(v, 0);  
        }
        flag &= blocked.count(v->l->s) == 0;
        auto tmp = makeSubst(v->r, from, to, blocked, flag);
        v->r = tmp.fr;
        return mp(v, tmp.sc);
    } 
    if (v->type == "a" && v->s == from)  {
        flagFail |= !flag;
        return mp(createExpCopy(to), 1);
    }
    int sum = 0;
    if (v->l != NULL) {
        auto tmp = makeSubst(v->l, from, to, blocked, flag);
        v->l = tmp.fr;
        sum += tmp.sc;
    }
    if (v->r != NULL) {
        auto tmp = makeSubst(v->r, from, to, blocked, flag);
        v->r = tmp.fr;
        sum += tmp.sc;
    }
    for (int i = 0; i < (int)v->ch.size(); i++) {
        auto tmp = makeSubst(v->ch[i], from, to, blocked, flag);
        v->ch[i] = tmp.fr;
        sum += tmp.sc;
    }
    return mp(v, sum);
}

bool check9(Node * head) {
    if (head->type != "->") return 0;
    Node * fi = head->r;
    if (head->l->type != "&") return 0;
    if (head->l->r->type != "@") return 0;
    string var = head->l->r->l->s;

    auto r1 = makeSubst(createExpCopy(fi), var, new Node("0", "0"), set < string >(), 1);
    auto r2 = makeSubst(createExpCopy(fi), var, new Node("'", new Node("a", var), NULL), set < string > (), 1);

    assert(r1.fr == r2.fr);
    if (r1.fr == 0) return 0;

    string g = addBracket(r1.fr->s) + "&@" + var + addBracket(addBracket(fi->s) + "->" + addBracket(r2.fr->s)) + "->" + addBracket(fi->s);
    Parser p(g);
    Node * v = p.parseExpr();
    if (v->hash == head->hash) {
        //db("wiiiiillll");
        return 1;
    }
    return 0;
}

Node * findTheta(Node * v, Node * u, string var) {
    if (u == NULL) return NULL;
    assert(v != NULL && u != NULL);
    if (v->type == "a" && v->s == var) {
        return u;
    }
    if (v->type != u->type) return NULL;
    if (v->type == "@" || v->type == "?") {
        if (v->l->s == var) return NULL;
    }
    if (v->l != NULL) {
        auto res = findTheta(v->l, u->l, var);
        if (res != NULL) return res;
    }

    if (v->r != NULL) {
        auto res = findTheta(v->r, u->r, var);
        if (res != NULL) return res;
    }

    for (int i = 0; i < (int)min(v->ch.size(), u->ch.size()); i++) {
        auto res = findTheta(v->ch[i], u->ch[i], var);
        if (res != NULL) return res;
    }
    return NULL;
}

set < string > mergeSet(set < string > a, set < string > b) {
    for (auto x: b)
        a.insert(x);
    return a;
}

set < string > findFree(Node * v, set < string > blocked) {
    if (v == NULL) return set < string > ();
    if (v->type == "?" || v->type == "@") {
        blocked.insert(v->l->s);
        return findFree(v->r, blocked);
    }
    if (v->type == "a") {
        if (blocked.count(v->s) == 0) {
            set < string > tmp;
            tmp.insert(v->s);
            return tmp;
        }
    }
    set < string > sum;
    sum = mergeSet(sum, findFree(v->l, blocked));
    sum = mergeSet(sum, findFree(v->r, blocked));
    for (auto ch: v->ch)
        sum = mergeSet(sum, findFree(ch, blocked));

    return sum;
}

bool check11(Node * head) {
    if (head->type != "->") return 0;
    if (head->l->type != "@") return 0;
    string var = head->l->l->s;

    Node * fi = head->l->r;

    Node * theta = findTheta(fi, head->r, var);
    if (theta == NULL) return 0;

    auto sfree = findFree(theta, set < string > ());
    flagFail = 0;
    auto tmp = makeSubst(head->r, var, theta, sfree, 1);
    Node * u = tmp.fr;

    if (flagFail) return 0;
    
    string nExpr = "@" + var + addBracket(fi->s) + "->" + addBracket(u->s); 
    Parser p(nExpr);
    Node * g = p.parseExpr();
    if (g->hash == head->hash) return 1;
    return 0;
}

bool check12(Node * head) {
    if (head->type != "->") return 0;
    if (head->r->type != "?") return 0;
    string var = head->r->l->s;
    Node * fi = head->r->r;

    Node * theta = findTheta(fi, head->l, var);
    if (theta == NULL) return 0;

    auto sfree = findFree(theta, set < string > ());

    flagFail = 0;
    auto tmp = makeSubst(head->l, var, theta, sfree, 1);
    Node * u = tmp.fr;

    if (flagFail) return 0;
    
    //string nExpr = "@" + var + addBracket(fi->s) + "->" + addBracket(u->s); 
    string nExpr = addBracket(u->s) + "->" + "?" + var + addBracket(fi->s);
    Parser p(nExpr);
    Node * g = p.parseExpr();
    if (g->hash == head->hash) return 1;
    return 0;
}

void printRule(vector < string > rule2, map < char, string > q) {
    for (auto s: rule2) {
        for (auto ch: s) {
            if (q.count(ch) == 1) 
                cout << q[ch];
            else
                cout << ch;
        }
        cout << endl;
    }      
}

bool checkRuleExist(Node * head) {
    if (head->type != "->") return 0;
    if (head->l->type != "?") return 0;
    string s = addBracket(head->l->r->s) + "->" + addBracket(head->r->s);
    string var = head->l->l->s;
    Parser p(s);
    Node * u = p.parseExpr();
    if (proved.count(u->hash) == 1) {
        if (!mainHyp.empty()) {
            map < char, string > q;                 
            q['A'] = mainHyp;
            q['B'] = u->l->s;
            q['Y'] = u->r->s;
            printRule(rule2, q);
            cout << addBracket(u->l->s) << "->" << addBracket(mainHyp) << "->" << addBracket(u->r->s) << endl;
            cout << "?" <<  var << addBracket(u->l->s) << "->" << addBracket(mainHyp) << "->" << addBracket(u->r->s) << endl;

            q['A'] = "?" + var + addBracket(u->l->s);
            q['B'] = mainHyp;
            q['Y'] = addBracket(u->r->s);
            printRule(rule2, q);

            cout << addBracket(mainHyp) + "->" + "?" + var + addBracket(u->l->s) + "->" + addBracket(u->r->s) << endl;
        }
        return 1;
    }
    else
        return 0;
}

bool checkRuleAny(Node * head) {
    if (head->type != "->") return 0;
    if (head->r->type != "@") return 0;
    string var = head->r->l->s;
    Node * fi = head->l;
    Node * psi = head->r->r;
    
    string s = fi->s + "->" + psi->s;
    Parser p(s);
    Node * u = p.parseExpr();
    if (proved.count(u->hash) == 0) return 0;

    if (!mainHyp.empty()) {
        map < char, string > q;                 
        q['A'] = mainHyp;
        q['B'] = u->l->s;
        q['Y'] = u->r->s;
        printRule(rule3_2, q);
        cout << addBracket(mainHyp) + "&" + addBracket(u->l->s) + "->" + addBracket(u->r->s) << endl;
        cout << addBracket(mainHyp) + "&" + addBracket(u->l->s) + "->@" + var + addBracket(u->r->s) << endl;

        q['A'] = mainHyp;
        q['B'] = u->l->s;
        q['Y'] = "@" + var + addBracket(u->r->s);
        printRule(rule3_1, q);
        cout << addBracket(mainHyp) + "->" + addBracket(u->l->s) + "->@" + var + addBracket(u->r->s) << endl;
    }

    return 1;
}

void solve() {
    freopen("answer.txt", "w", stdout);

    for (int i = 0; i < (int)proof.size(); i++) {
        bool flag = 0;
        for (auto v: axiom) {
            matchPart.clear();
            if (fit(proof[i], v)) {
                flag = 1;
            }
        }
        for (int j = 0; j < (int)hypoth.size() - 1; j++) 
            if (hypoth[j]->hash == proof[i]->hash) 
                flag = 1;
        for (int j = 0; j < (int)mathAxiom.size(); j++)
            if (mathAxiom[j]->hash == proof[i]->hash) 
                flag = 1;

        flag |= check9(proof[i]);
        flag |= check11(proof[i]);
        flag |= check12(proof[i]);

        if (flag) {
            cout << proof[i]->s << endl; 
            if (!mainHyp.empty()) {
                cout << addBracket(proof[i]->s) << "->" << addBracket(mainHyp) << "->" << addBracket(proof[i]->s) << endl;
                cout << addBracket(mainHyp) << "->" << addBracket(proof[i]->s) << endl;
            }
        }

        if (hypoth.back()->hash == proof[i]->hash && flag == 0) {
            makeAA(proof[i]->s);
            flag = 1;
        }

        if (flag == 0) {
            for (auto x: need[proof[i]->hash]) {
                if (proved.count(x)) {
                    flag = 1;
                    cout << proof[i]->s << endl;
                    if (!mainHyp.empty()) {
                        assert(exprFromHash.count(x) == 1);

                        string A = addBracket(mainHyp);
                        string B = addBracket(exprFromHash[x]);
                        string C = addBracket(proof[i]->s);
                            
                        string first = addBracket(A + "->" + B);
                        string second= addBracket(A + "->" + B + "->" + C);
                        string third = addBracket(A + "->" + C);

                        cout << first + "->" + second + "->" + third << endl;
                        cout << second + "->" + third << endl;
                        cout << third << endl; 
                    }
                }
            }
        } 

        if (!flag) flag |= checkRuleExist(proof[i]);
        if (!flag) flag |= checkRuleAny(proof[i]);



        if (!flag) {
            cout << "incorrect evidence from : " << i + 1 << endl;
            return;
        }
        proved.insert(proof[i]->hash);
        exprFromHash[proof[i]->hash] = proof[i]->s;
        if (proof[i]->type == "->") {
            need[proof[i]->r->hash].insert(proof[i]->l->hash);
        }
    }
}


int main() {
    read();
    init();
    solve();


    //for (int t = 0; t < 2; t++) {
        //string s;
        //getline(cin, s);
        //Parser parser(s);
        //Node * v = parser.parseExpr();
        //db(v->hash.val);
    //}

}


