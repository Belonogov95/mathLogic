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

vector < string > axiom;
vector < int > qid;
vector < oper > b;
vector < node * > a;
unordered_map < long long, int > ok;
vector < pair < long long, long long > > q;
bool use[3];
long long c[3];

void read() {

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

void solve() {
    b.pb(oper("->", 4));
    b.pb(oper("&", 3));
    b.pb(oper("|", 2));
    init();
    for (auto s : axiom)
        a.pb(makeTree(s));
    string s;
    bool mainFlag = 1;
    for (int tt = 0; cin >> s != 0; tt++) {
        printf("(%d) ", tt + 1);
        cout << s << " ";

        node * head = makeTree(s);
        bool flag = 0;
        if (head != NULL) {
            for (int i = 0; i < (int)a.size(); i++) {
                auto x = a[i];
                memset(use, 0, sizeof(use));
                if (compAx(head, x)) {
                    flag = 1;
                    printf("(Cx. акс. %d)", i + 1);
                    break; 
                }
            }
            for (int i = 0; !flag && i < (int)q.size(); i++) {
                auto x = q[i];
                if (x.sc == head->hash && ok.find(x.fr) != ok.end()) {
                    flag = 1;
                    printf("(M. P. %d, %d)", qid[i] + 1, ok[x.fr] + 1);
                    break;
                }
            }
            if (flag && head->type == 4) {
                q.pb(mp(head->l->hash, head->r->hash));
                qid.pb(tt);
            }
            if (!flag)
                printf("(Не доказано)");
        }
        else {
            cout << "(incorrect statments)";
        }
        if (flag == 0) {
            mainFlag = 0;
        }
        if (flag)
            ok.insert(mp(head->hash, tt));
        printf("\n");
    }
    if (mainFlag)
        cerr << "OK\n";
    else
        cerr << "WA\n";
}

void printAns() {

}

void stress() {

}


int main(){
#ifdef DEBUG
    freopen("in", "r", stdin);
    freopen("out", "w", stdout);
    //freopen("test.txt", "r", stdin);
#endif
    if (1) {
        while (true) {
            read();
            solve();
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

