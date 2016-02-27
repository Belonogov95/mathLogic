#include "main.h"
#include "parser.h"
#include "deduction.h"


Deduction::Deduction(vector < Line > * lines, string term1):lines(lines) {
    Parser p(term1);
    term = p.parse()->s;
}

string Deduction::replace(string s, char u, string t) {
    string ans;
    for (auto ch : s) 
        if (ch == u)
            ans += t;
        else
            ans += ch;
    return ans;
}

void Deduction:: add(string tmp) {
    Parser p(tmp);
    answer->pb(Line(p.parse()));
}

void Deduction :: makeAA() {
    vector < string > tmp;
    tmp.pb("(A)->(A)->(A)");
    tmp.pb("(A)->((A)->(A))->(A)");
    tmp.pb("((A)->(A)->(A))->((A)->((A)->(A))->(A))->((A)->(A))");
    tmp.pb("((A)->((A)->(A))->(A))->((A)->(A))");
    tmp.pb("(A)->(A)");
    for (auto s: tmp) {
        add(replace(s, 'A', term));
    }
}

Node * Deduction :: myParse(string tmp) {
    Parser p(tmp);
    return p.parse();
}

void Deduction :: init() {
    vector < string > tmp;
    tmp.pb("A->B->A");
    tmp.pb("(A->B)->(A->B->C)->(A->C)");
    tmp.pb("A->B->A&B");
    tmp.pb("A&B->A");
    tmp.pb("A&B->B");
    tmp.pb("A->A|B");
    tmp.pb("B->A|B");
    tmp.pb("(A->C)->(B->C)->(A|B->C)");
    tmp.pb("(A->B)->(A->!B)->!A");
    tmp.pb("!!A->A");
    for (auto x: tmp)
        axiom.pb(myParse(x));
}

bool Deduction :: compAx(Node * v, Node * ax, int level = 0) {
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
    if (isLetter(ax->type))
        return 1;
    if (ax->type != v->type) {
        //db2(ax->type, v->type);
        //db("Here");
        return 0;
    }
    bool ans = 1;
    ans &= compAx(v->l, ax->l, level + 1);
    ans &= compAx(v->r, ax->r, level + 1);
    return ans;
}


vector < Line > * Deduction :: go() {
    vector < string > nAssumption;
    set < ull > hypothesis;
    init();
    for (auto x: (*lines)[0].assumption) {
        Parser p(x);
        Node * v = p.parse();
        string nx = v->s;
        if (x != term) {
            nAssumption.pb(x);
            hypothesis.insert(v->hash);
        }
    }
    answer = new vector < Line >();
    map < ull, int > prevLine;

    for (int i = 0; i < (int)lines->size(); i++) {
        Node * head = (*lines)[i].head; 
        //db2(i, head->s);
        prevLine[head->hash] = i;
        if (head->s == term) {
            makeAA();
            continue;
        }
        bool key = 0;
        key |= hypothesis.count(head->hash) == 1;
          
        for (Node * v: axiom) {
            memset(use, 0, sizeof(use));
            //db(v->s);
            if (compAx(head, v)) {
                key = 1; 
            }
            //db(key);
            //exit(0);
        }
        //db(key);

        if (key == 1) {
            add(head->s);
            add(inBracket(head->s) + "->" + inBracket(term) + "->" + inBracket(head->s));
            add(inBracket(term) + "->" + inBracket(head->s));
            continue;
        } 

        bool flag = 0;
        for (int j = 0; j < i; j++) {
            Node * v = (*lines)[j].head;
            if (v->type == "->" && v->r->s == head->s && prevLine.count(v->l->hash) == 1) {
                flag = 1;
                int id = prevLine[v->l->hash];

                string A = inBracket(term);
                string B = inBracket((*lines)[id].head->s);
                string C = inBracket(head->s);
                    
                string first = inBracket(A + "->" + B);
                string second = inBracket(A + "->" + B + "->" + C);
                string third = inBracket(A + "->" + C);

                add(first + "->" + second + "->" + third);
                add(second + "->" + third);
                add(third);
   
                break;
            }
        } 
        assert(flag);

    }


    for (int i = 0; i < (int)answer->size(); i++) 
        (*answer)[i].assumption = nAssumption;
    return answer;
}








