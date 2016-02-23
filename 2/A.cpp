#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <unordered_set>
#include <unordered_map>


using namespace std;

#define fr first
#define sc second
#define mp make_pair
#define pb push_back

#define epr(...) fprintf(stderr, __VA_ARGS__)
const int N = -1;
const int INF = 1e9;
const int T = 20;
const long long P = 1e9 + 7;

struct node {
    int type; //// 4 -> /// 3 | /// 2 & /// 1 !
    long long hash;
    string s;
    node * l, * r; 
    node() {}
};

struct oper {
    string s;
    int type;
    oper(){ }
    oper(string s, int type): s(s), type(type) { }
};

struct Desc {
    int type; // -1 fail // 0 not proof // 1 - axc // 2 - M. P.
    int id1, id2;
    Desc() {}
    Desc(int type, int id1, int id2): type(type), id1(id1), id2(id2) {}
};

vector < string > axiom;
vector < int > qid;
vector < oper > b;
vector < node * > a;
unordered_map < long long, int > ok;
vector < pair < long long, long long > > q;
bool use[3];
long long c[3];
int withoutProof;
vector < string > expr;
string A, B;
vector < Desc > describe;
vector < string > answer;
vector < string > hypothesis;
vector < node * > hypothesisTree;

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
    string s;
    getline(cin, s);
    s = cutSpace(s);

    int cur = 0;
    for (int i = 0; i < (int)s.size(); i++) 
        if (s[i] == ',') {
            hypothesis.pb(s.substr(cur, i - cur));
            cur = i + 1;
        }
    bool flag = 0;
    for (int i = cur; i < (int)s.size(); i++)
        if (s[i] == '|') {
            A = s.substr(cur, i - cur);
            B = s.substr(i + 2, (int)s.size() - (i + 2));
            hypothesis.pb(A);
            flag = 1;
        }  
    assert(flag);
    withoutProof = expr.size();
    cerr << "withoutProof: " << (withoutProof) << endl;
    for (; getline(cin, s) != 0; ) {
    //for (; cin >> s != 0; ) {
        s = cutSpace(s);
        if (s.empty()) continue;
        expr.pb(s); 
    }
    //cerr << expr.size() << endl;
}

void init() {
    axiom.pb("A->B->A");
    axiom.pb("(A->B)->(A->B->C)->(A->C)");
    axiom.pb("A->B->A&B");
    axiom.pb("A&B->A");
    axiom.pb("A&B->B");
    axiom.pb("A->A|B");
    axiom.pb("B->A|B");
    axiom.pb("(A->C)->(B->C)->(A|B->C)");
    axiom.pb("(A->B)->(A->!B)->!A");
    axiom.pb("!!A->A");
}

node * rec(string & s, vector < int > & p, int l, int r) {
    if (s[l] == '(' && s[r - 1] == ')' && p[l] == r - 1)
        return rec(s, p, l + 1, r - 1); 

    for (auto x : b) {
        for (int i = l; i < r; i++) {
            if (s[i] == '(') {
                i = p[i];
                continue;
            }
            if (i + (int)x.s.size() <= (int)s.size() && s.substr(i, x.s.size()) == x.s) {
                node * v = new node;
                v->l = rec(s, p, l, i);
                v->r = rec(s, p, i + x.s.size(), r);
                v->type = x.type;
                v->hash = (v->l->hash * P + x.type) * P + v->r->hash;
                return v;
            }
        }
    }
    if (s[l] == '!') {
        node * v = new node;
        v->type = 1;
        v->r = NULL;
        v->l = rec(s, p, l + 1, r);
        v->hash = v->l->hash * P + 1;
        return v;
    }
    node * v = new node;
    assert(r - l > 0);
    v->s = s.substr(l, (r - l));
    v->hash = 0;
    v->type = -1;
    for (auto x : v->s)
        v->hash = v->hash * P + x;
    v->l = v->r = NULL;
    return v;
}

node * makeTree(string & s) {
    vector < int > stack(s.size());
    vector < int > p(s.size(), -1);
    int cur = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '(') {
            stack[cur++] = i;
        }
        if (s[i] == ')') {
            if (cur == 0)
                return NULL;
            p[stack[cur - 1]] = i;
            p[i] = stack[cur - 1];
            cur--;
        }
    }
    if (cur != 0)
        return NULL;
    return rec(s, p, 0, s.size());
}


bool compAx(node * v, node * ax, int level = 0) {
    if (v == NULL && ax == NULL) return 1;
    if (v == NULL || ax == NULL) return 0;
    if (ax->l == NULL && ax->r == NULL) {
        assert((int)ax->s.length() == 1);
        int id = ax->s[0] - 'A';
        assert(0 <= id && id <= 2);
        if (!use[id]) {
            use[id] = 1;
            c[id] = v->hash;
        }
        if (c[id] != v->hash) {
            return 0;
        }
    } 
    if (ax->type == -1)
        return 1;
    if (ax->type != v->type) {
        return 0;
    }
    bool ans = 1;
    ans &= compAx(v->l, ax->l, level + 1);
    ans &= compAx(v->r, ax->r, level + 1);
    return ans;
}

void firstSolve() {
    b.pb(oper("->", 4));
    b.pb(oper("&", 3));
    b.pb(oper("|", 2));
    init();
    for (auto s : axiom)
        a.pb(makeTree(s));
    for (auto s : hypothesis)
        hypothesisTree.pb(makeTree(s));
    bool mainFlag = 1;
    for (int tt = 0; tt < (int)expr.size(); tt++) {
        //printf("(%d) ", tt + 1);    cout << expr[i] << " ";

        node * head = makeTree(expr[tt]);
        bool flag = 0;
        if (head != NULL) {
            for (int i = 0; !flag && i < (int)hypothesisTree.size(); i++) {
                auto v = hypothesisTree[i];
                if (v->hash == head->hash) {
                    flag = 1; 
                    describe.pb(Desc(0, i, -1));
                }
            }
            if (tt < withoutProof) {
                describe.pb(Desc(0, 0, 0));
                flag = 1;
            }
            for (int i = 0; !flag && i < (int)a.size(); i++) {
                auto x = a[i];
                memset(use, 0, sizeof(use));
                if (compAx(head, x)) {
                    flag = 1;
                    describe.pb(Desc(1, i, -1));
                    //printf("(Cx. акс. %d)", i + 1);
                    break; 
                }
            }
            for (int i = 0; !flag && i < (int)q.size(); i++) {
                auto x = q[i];
                if (x.sc == head->hash && ok.find(x.fr) != ok.end()) {
                    flag = 1;
                    describe.pb(Desc(2, qid[i], ok[x.fr]));
                    //printf("(M. P. %d, %d)", qid[i] + 1, ok[x.fr] + 1);
                    break;
                }
            }
            if (!flag)
                describe.pb(Desc(-1, -1, -1));
                //printf("(Не доказано)");
        }
        else  {
            cout << "(incorrect statments)";
            describe.pb(Desc(-1, -1, -1));
        }

        mainFlag &= flag;
        if (flag)
            ok.insert(mp(head->hash, tt));
        if (flag && head->type == 4) {
            q.pb(mp(head->l->hash, head->r->hash));
            qid.pb(tt);
        }
        //printf("\n");
    }
    if (mainFlag)
        cerr << "OK\n";
    else
        cerr << "WA\n";
}

string replace(string s, char u, string t) {
    string ans;
    for (auto ch : s) 
        if (ch == u)
            ans += t;
        else
            ans += ch;
    return ans;
}

void makeAA() {
    answer.pb("(A)->(A)->(A)");
    answer.pb("(A)->((A)->(A))->(A)");
    answer.pb("((A)->(A)->(A))->((A)->((A)->(A))->(A))->((A)->(A))");
    answer.pb("((A)->((A)->(A))->(A))->((A)->(A))");
    for (int i = 1; i <= 4; i++)
        answer[(int)answer.size() - i] = replace(answer[(int)answer.size() - i], 'A', A);
}

void secondSolve() {
    assert(expr.size() == describe.size());
    makeAA();   
    for (int i = 0; i < (int)expr.size(); i++) {
        if (describe[i].type == 0 && describe[i].id1 == (int)hypothesis.size() - 1) {
            answer.pb(replace("(A)->(A)", 'A', A));
        }
        else if (describe[i].type == 1 || describe[i].type == 0) {
            answer.pb(expr[i]);
            answer.pb("(" + expr[i] + ")->(" + A + ")->(" + expr[i] + ")");
            answer.pb("(" + A + ")->(" + expr[i] + ")");
        } 
        else {
            assert(describe[i].type == 2);
            string tmp = "(" + A + ")->";
            string first = "(" + tmp + "(" + expr[describe[i].id2] + ")" + ")";
            string second = "(" + tmp + "(" + expr[describe[i].id1] + ")" + ")";
            string third = "(" + tmp + "(" + expr[i] + ")" + ")";
            answer.pb(first + "->" + second + "->" + third);
            answer.pb(second + "->" + third);
            answer.pb(third);
        }
    }
}

string inBracket(string s) {
    return "(" + s + ")";
}
void printAns() {
    for (int i = 0; i < (int)hypothesis.size() - 1; i++) {
        cout << hypothesis[i];
        if (i + 1 != (int)hypothesis.size() - 1)
            cout << ",";
    }
    cout << "|-";
    cout << inBracket(A) << "->" << inBracket(B) << endl;
    for (auto x : answer) 
        cout << x << "\n";
}

void stress() {

}


int main(){
#ifdef DEBUG
    freopen("in1", "r", stdin);
    //freopen("out", "w", stdout);
    //freopen("test.txt", "r", stdin);
#endif
    if (1) {
        while (true) {
            read();
            firstSolve();
            secondSolve();
            printAns();
            cerr << clock() * 1.0 / CLOCKS_PER_SEC << endl;
            return 0;
        }
    }
    else {
        stress();
    }

    return 0;
}

