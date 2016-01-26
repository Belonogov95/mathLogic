#include "main.h"
#include "parser.h"

string inBracket(string s) {
    return "(" + s + ")";
}

bool isLetter(string g) {
    return g.size() == 1 && 'A' <= g[0] && g[0] <= 'Z';
}

vector < string > split(string s, string pattern) {
    vector < string > res;
    int cur = 0;
    for (int i = 0; i + pattern.size() <= s.size(); i++) {
        if (s.substr(i, pattern.size()) == pattern) {
            res.pb(s.substr(cur, i - cur));
            i += pattern.size();
            cur = i;
            i--;
        }
    }
    assert(cur < (int)s.size());
    res.pb(s.substr(cur, s.size() - cur));
    return res;
}

//ull binPow(ull p, int deg) {
    //ull res = 1;
    //for (; deg > 0; deg /= 2) {
        //if (deg % 2 == 1)
            //res *= p;
        //p *= p;
    //}
    //return res;
//}

ull calcHash(string & s) {
    ull hash = 0;
    for (auto ch: s)
        hash = hash * P + ch;
    return hash;
}

Node::Node (string type, Node * l, Node * r): type(type), l(l), r(r) {
    db(type);
    if (isLetter(type)) {
        s = type;
        assert(l == NULL && r == NULL);
    }
    else if (type == "!") {
        s = type + inBracket(l->s);
        assert(r == NULL);
    }
    else {
        s = inBracket(l->s) + type + inBracket(r->s);
    }
    hash = calcHash(s);
}

string removeSpaces(string s) {
    string res;
    for (int i = 0; i < (int)s.size(); i++)
        if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\r')
            res += s[i];
    return res;
}

//struct Token {
    //string type;
//};

//struct Parser {

Parser::Parser(string data): data(data) { }

Token Parser::nextToken() {
    db2(data[cur], cur);
    for (; cur < (int)data.size() && data[cur] == ' '; cur++);        
    if (cur == (int)data.size()) return ""; 
    if (data[cur] == '(') return "(";
    if (data[cur] == ')') return ")";
    if (data[cur] == '&') return "&";
    if (data[cur] == '|') return "|";
    if (data[cur] == '!') return "!";
    if (data[cur] == '-' && cur + 1 < (int)data.size() && data[cur + 1] == '>') return "->";
    if ('A' <= data[cur] && data[cur] <= 'Z') return string(1, data[cur]);

    cerr << "incorrect symbol " << data[cur] << " at position " << cur << endl;
    assert(false);
}

void Parser::shiftCur() {
    assert(cur < (int)data.size());
    if (data[cur] == '(') { cur++; return;} 
    if (data[cur] == ')') { cur++; return;} 
    if (data[cur] == '&') { cur++; return;} 
    if (data[cur] == '|') { cur++; return;} 
    if (data[cur] == '!') { cur++; return;} 
    if (data[cur] == '-' && cur + 1 < (int)data.size() && data[cur + 1] == '>'){ cur += 2; return; }
    if ('A' <= data[cur] && data[cur] <= 'Z') {cur++; return;}
    assert(false);
}

Node * Parser::parseImplication() {
    Node * result = parseConjunct();         
    for (; nextToken() == "|"; ) {
        shiftCur();
        Node * tmp = parseConjunct(); 
        result = new Node("|", result, tmp);
    }
    return result;
}

Node * Parser::parseConjunct() {
    Node * result = parseDisjunct();
    for (; nextToken() == "&"; ) {
        shiftCur();
        Node * tmp = parseDisjunct(); 
        result = new Node("&", result, tmp);
    }
    return result;
}

Node * Parser::parseDisjunct() {
    if (nextToken() == "!") {
        shiftCur();
        Node * v = parseDisjunct();
        return new Node("!", v, NULL);
    }

    if (nextToken() == "(") {
        shiftCur();
        Node * v = parseAll();
        assert(nextToken() == ")");
        shiftCur();
        return v;
    }

    Token t = nextToken();
    if (isLetter(t)) {
        shiftCur();
        return new Node(t, NULL, NULL);
    }
    cerr << "incorrect Token " << t << endl;
    assert(false);
}

Node * Parser::parseAll() {
    vector < Node * > result(1, parseImplication());
    for (; nextToken() == "->"; ) {
        shiftCur();
        result.push_back(parseImplication());
    }
    Node * vert = result.back();
    for (int i = (int)result.size() - 2; i >= 0; i--)
        vert = new Node("->", result[i], vert);
    return vert;
}

Node * Parser::parse() {
    cur = 0;    
    return parseAll();
}

void Parser::printTree(Node * v, int tab) {
    if (v == NULL) return; 
    printTree(v->r, tab + 1);
    for (int i = 0; i < tab; i++)
        cerr << "\t";
    cerr << v->s << endl;
    printTree(v->l, tab + 1);

}


Line::Line (string s) {
    s = removeSpaces(s);
    vector < string > tmp = split(s, "|-"); 
    assert(tmp.size() <= 2 && !tmp.empty()); 
    Parser p(tmp.back());
    head = p.parse();
    if (tmp.size() == 2) 
        assumption = split(tmp[0], ",");
}










