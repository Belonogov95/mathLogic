#include "main.h"
#include "parser.h"


set < string > tmp;
vector < string > var;
Node * head;

void findVar(Node * v) {
    assert(v != NULL);
    if (v->type == "&" || v->type == "|" || v->type == "->") {
        findVar(v->l);
        findVar(v->r);
        return;
    }
    if (v->type == "!") {
        findVar(v->l);
        return;
    }
    tmp.insert(v->type);
}

void read() {
    freopen("in", "r", stdin); 
    string s;
    getline(cin, s);
    Parser p(s);
    head = p.parse();
    findVar(head);
    for (auto x: tmp)
        var.pb(x);
    sort(var.begin(), var.end());
}

const int N =  10;

int n = 5;
int m;
int p[N];
int vmask[N];
vector < int > e[N];

bool calcExpr(Node * expr, int world);

bool dfs(int v, Node * l, Node * r) {
    //db(v);
    //db2(l->s, r->s);
    bool r1 = calcExpr(l, v); 
    bool r2 = calcExpr(r, v);
    if (!(!r1 || r2)) {
        return 0;
    }
    bool flag = 1;
    for (auto u: e[v])
        flag &= dfs(u, l, r);
    return flag;
}

bool dfs2(int v, Node * l) {
    if (calcExpr(l, v)) return 0;
    bool flag = 1;
    for (auto u: e[v])
        flag &= dfs2(u, l);
    return flag;
}

bool calcExpr(Node * expr, int world) {
    //db(expr->s);
    if (expr->type == "&") {
        bool r1 = calcExpr(expr->l, world);
        bool r2 = calcExpr(expr->r, world);
        return r1 && r2;
    } 
    if (expr->type == "|") {
        //db2(world, expr->s);
        bool r1 = calcExpr(expr->l, world);
        bool r2 = calcExpr(expr->r, world);
        return r1 || r2;
    } 
    if (expr->type == "->") {
        return dfs(world, expr->l, expr->r);
    }
    if (expr->type == "!") {
        return dfs2(world, expr->l); 
    }
    int pos = -1; 
    for (int i = 0; i < m; i++)
        if (var[i] == expr->type)
            pos = i;
    //db(var[0]);
    //db(expr->type);
    assert(pos != -1);
    return ((vmask[world] >> pos) & 1);
}

void rec2(int v) {
    int mask = (p[v] == -1)? 0: vmask[p[v]];
    if (v == n) {
        bool res = calcExpr(head, 0);
        if (!res) {
            cout << "WIN\n";
            for (int i = 0; i < n; i++) {
                cout << i << ": ";
                for (int j = 0; j < m; j++)
                    if ((vmask[i] >> j) & 1)
                        cout << var[j];
                cout << endl;
            }
            for (int i = 1; i < n; i++)
                cout << "(" << p[i] << ", " << i << ")\n";
            exit(0);
        }
        return;
    }
    for (int mm = 0; mm < (1 << m); mm++) {
        if ((~mm & mask) != 0) continue;
        vmask[v] = mm;
        rec2(v + 1);
    }
}

void rec(int pos) {
    if (pos == n) {
        for (int i = 0; i < n; i++)
           e[i].clear(); 
        for (int i = 1; i < n; i++) {
            e[p[i]].pb(i);
        }
        rec2(0);
        return;
    }
    for (int i = 0; i < pos; i++) {
        p[pos] = i;
        rec(pos + 1);
    }

}

void solve() {
    m = var.size();
    //string t = "!!P->P";
    //Parser g(t);
    //Node * u = g.parse();
    //db(calcExpr(u, 0));
    //return;
    p[0] = -1;
    for (int i = 1; i <= 5; i++) {
        n = i;
        rec(1);
    }
    cout << "I think it's true\n";
}

int main() {
    read(); 
    solve();
    

}

