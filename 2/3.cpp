#include "main.h"
#include "parser.h"
#include "deduction.h"


const int ALF = 26;

Node * head;
vector < char > var;
map < char, int > rev;
vector < string > answer;
vector < Node * > andProof[2][2];
vector < Node * > orProof[2][2]; 
vector < Node * > impProof[2][2];
vector < Node * > notProof[2];
vector < Node * > aOrNotA;

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
    string target;
    getline(cin, target);
    //db2(target, target.size());
    target = cutSpace(target);

    Parser p(target);
    head = p.parse();
    for (int i = 0; i < (int)target.size(); i++) {
        if ('A' <= target[i] && target[i] <= 'Z') {
            char ch = target[i];
            if (rev.count(ch) == 0) {
                rev[ch] = var.size();
                var.pb(ch);
            }
        }
    }
    freopen("and.txt", "r", stdin); 
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            //db2(i, j);
            string s;
            int x, y;
            cin >> x >> y;
            getline(cin, s);
            for (; getline(cin, s); ) {
                if (s == "#") break;
                //db(":" + s + ":");
                Parser p(s);
                 
                andProof[x][y].pb(p.parse());
            }
        }

    freopen("imp.txt", "r", stdin); 
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            //db2(i, j);
            string s;
            int x, y;
            cin >> x >> y;
            getline(cin, s);
            for (; getline(cin, s); ) {
                if (s == "#") break;
                //db(":" + s + ":");
                Parser p(s);
                impProof[x][y].pb(p.parse());
            }
        }
    freopen("or.txt", "r", stdin); 
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            //db2(i, j);
            string s;
            int x, y;
            cin >> x >> y;
            getline(cin, s);
            for (; getline(cin, s); ) {
                if (s == "#") break;
                //db(":" + s + ":");
                Parser p(s);
                orProof[x][y].pb(p.parse());
            }
        }
 
    freopen("not.txt", "r", stdin); 
    for (int i = 0; i < 2; i++) {
        //db(i);
        string s;
        int x;
        cin >> x;
        getline(cin, s);
        for (; getline(cin, s); ) {
            if (s == "#") break;
            //db(":" + s + ":");
            Parser p(s);
            notProof[x].pb(p.parse());
        }
    }
    freopen("aOrNotA.txt", "r", stdin);
    string s;
    for (; getline(cin, s); ) {
        if (s == "#") break;
        Parser p(s);
        aOrNotA.pb(p.parse());
    }
    

 
}

string replace(string s, vector < pair < char, string > > paterns) {
    string g;
    for (int i = 0; i < (int)s.size(); i++) {
        bool flag = 0;
        for (int j = 0; !flag && j < (int)paterns.size(); j++)
            if (s[i] == paterns[j].fr) {
                g += paterns[j].sc;
                flag = 1;
            }
        if (!flag)
            g += s[i];
    }
    return g;
}

vector < Line > * curRes;

int rec(Node * v, vector < int > value) {
    //db(v);
    assert(v != NULL);
    if (isLetter(v->type)) {
        int id = rev[v->type[0]];
        string s = v->type;
        if (value[id] == 0)
            s = "!" + s;
        curRes->pb(s); 
        return value[id];
    }
    int r1 = 0, r2 = 0;
    if (v->l != NULL) r1 = rec(v->l, value); 
    if (v->r != NULL) r2 = rec(v->r, value);

    if (v->type == "&") {
        int res = r1 & r2; 
        string leftExp = v->l->s;
        string rightExp = v->r->s;

        for (int i = 0; i < (int)andProof[r1][r2].size(); i++) {
            vector < pair < char, string > > paterns;
            paterns.pb(mp('A', leftExp));
            paterns.pb(mp('B', rightExp));
            curRes->pb(Line(replace(andProof[r1][r2][i]->s, paterns)));
        }
        return res;
    }

    if (v->type == "|") {
        int res = r1 | r2; 
        string leftExp = v->l->s;
        string rightExp = v->r->s;

        for (int i = 0; i < (int)orProof[r1][r2].size(); i++) {
            vector < pair < char, string > > paterns;
            paterns.pb(mp('A', leftExp));
            paterns.pb(mp('B', rightExp));
            curRes->pb(Line(replace(orProof[r1][r2][i]->s, paterns)));
        }
        return res;
    }

    if (v->type == "->") {
        int res = (!r1) | r2; 
        string leftExp = v->l->s;
        string rightExp = v->r->s;

        for (int i = 0; i < (int)impProof[r1][r2].size(); i++) {
            vector < pair < char, string > > paterns;
            paterns.pb(mp('A', leftExp));
            paterns.pb(mp('B', rightExp));
            curRes->pb(Line(replace(impProof[r1][r2][i]->s, paterns)));
        }
        return res;
    }

    if (v->type == "!") {
        int res = !r1;
        string leftExp = v->l->s;

        for (int i = 0; i < (int)notProof[r1].size(); i++) {
            vector < pair < char, string > > paterns;
            paterns.pb(mp('A', leftExp));
            curRes->pb(Line(replace(notProof[r1][i]->s, paterns)));
        }
        return res;
    }

 
    assert(false);
}

string norm(string s) {
    Parser p(s);
    return p.parse()->s;
}

void solve() {
    curRes = new vector < Line > ();
    for (int mask = 0; mask < (1 << var.size()); mask++) {
        //db(mask);
        vector < int > value(var.size());
        curRes->clear();
        for (int i = 0; i < (int)var.size(); i++)
            value[i] = ((mask >> i) & 1);
        if (rec(head, value) == 0) {
            
            cerr << "fail :\n";
            for (auto ch: var)
                cerr << ch << " ";
            cerr << endl;
            for (auto x: value)
                cerr << x << " ";
            cerr << endl;
            cout << "Высказывание ложно при ";
            for (int g = 0; g < (int)var.size(); g++) {
                //char ch = (value[g] == 1)? 'И': 'Л';
                cout << var[g] << "=";
                if (value[g])
                    cout << "И";
                else
                    cout << "Л";
                if (g + 1 != (int)var.size())
                    cout << ", ";
            }
            cout << endl;
            exit(0);
        }
        //for (int i = 0; i < (int)curRes->size(); i++)
            //cerr << curRes->at(i).head->s << endl;
        //db("here");

        vector < string > ass;
        for (int i = 0; i < (int)var.size(); i++) {
            string g = ((value[i] == 0)? "!": "") + string(1, var[i]);
            //db(g);
            ass.pb(g);
        }
        for (int i = 0; i < (int)curRes->size(); i++)
            curRes->at(i).assumption = ass;

        for (int i = (int)var.size() - 1; i >= 0; i--) {
            //db(i);
            //db(ass[i]);
            Deduction ded(curRes, ass[i]);
            curRes = ded.go();
        } 
        for (int i = 0; i < (int)curRes->size(); i++)
            answer.pb(curRes->at(i).head->s);
    }  

    //db("second part");
    for (int i = 0; i < (int)var.size(); i++) {
        string v = string(1, var[i]);

        for (int j = 0; j < (int)aOrNotA.size(); j++)
            answer.pb(replace(aOrNotA[j]->s, vector < pair < char, string > > (1, mp('A', v))));
        for (int mask = 0; mask < (1 << (var.size() - i - 1)); mask++) {
            string a = head->s;
            for (int j = 0; j < (int)var.size() - i - 1; j++) {
                string tmp(1, var[var.size() - j - 1]);
                int bit = ((mask >> j) & 1); 
                if (bit)
                    tmp = "!" + tmp;
                a = inBracket(tmp) + "->" + a;
            }

            string first = inBracket(v + "->" + a) + "->" + inBracket("!" + v + "->" + a) + "->" + inBracket(v + "|!" + v) + "->" + inBracket(a);
            string second = inBracket("!" + v + "->" + a) + "->" + inBracket(v + "|!" + v) + "->" + inBracket(a);
            string third = inBracket(v + "|!" + v) + "->" + inBracket(a);
            //db("st");
            //db(first);
            answer.pb(norm(first));
            answer.pb(norm(second));
            answer.pb(norm(third));
            answer.pb(norm(a));
        }
    }




    for (auto s: answer)
        cout << s << endl;
}

void stress() {

}


int main(){
#ifdef DEBUG
    freopen("in1", "r", stdin);
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



