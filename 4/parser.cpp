#include "main.h"
#include "parser.h"


ull calcHash(string & s) {
    ull hash = 0;
    for (auto ch: s)
        hash = hash * P + ch;
    return hash;
}

string addBracket(string s) {
    return "(" + s + ")";
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

Node::Node(string type, Node * l, Node * r): type(type), l(l), r(r) {
    if (l != NULL && r != NULL) 
        s = addBracket(l->s) + type + addBracket(r->s);
    else {
        assert(l != NULL);
        if (type == "'")
            s = addBracket(l->s) + type;
        else
            s = type + addBracket(l->s);
    }
    hash = calcHash(s);
}

Node::Node(string type, string val): type(type), l(NULL), r(NULL) {
    s = val;
    hash = calcHash(s);
}

Node::Node(string type, string val, vector < Node * > ch): type(type), l(NULL), r(NULL), ch(ch) {
    s = val;
    if (!ch.empty())
        s += "(";
    for (int i = 0; i < (int)ch.size(); i++) {
        s += ch[i]->s;
        if (i + 1 != (int)ch.size())
            s += ", ";
    }
    if (!ch.empty())
        s += ")";

    hash = calcHash(s);
}

string removeSpace(string s) {
    string res;
    for (auto ch: s)
        if (!isspace(ch))
            res += ch;
    return res;
}

void printTree(Node * v, int tab) {
    if (v == NULL) return;
    printTree(v->r, tab + 1);
    for (int i = 0; i < tab; i++)
        epr("\t");
    cerr << "type val: " << v->type << " " << v->s << "                     ";
    for (auto t: v->ch)
        cerr << t->type << " ";
    cerr << endl;
    printTree(v->l, tab + 1);
} 

Parser::Parser(string s) {
    data = removeSpace(s);
    cur = 0;
}

string Parser::nextToken() {
    assert(cur <= (int)data.size());
    if (cur == (int)data.size()) return "";
    if (cur + 1 < (int)data.size() && data[cur] == '-' && data[cur + 1] == '>') 
        return "->";
    return string(1, data[cur]);
}

void Parser::shiftCur() {
    if (cur + 1 < (int)data.size() && data[cur] == '-' && data[cur + 1] == '>')
        cur += 2;
    else
        cur++;
}

Node * Parser::parsePredicate() {
    //db("Predicate");
    if (nextToken().size() == 1 && isupper(nextToken()[0])) {
        string name = nextToken();
        shiftCur();
        for (; nextToken().size() == 1 && isdigit(nextToken()[0]); ) {
            name += nextToken();
            shiftCur();
        }
        vector < Node * > ch;
        if (nextToken() == "(") {
            shiftCur();
            ch.pb(parseTerm());
            for (; nextToken() == ",";) {
                shiftCur();
                ch.pb(parseTerm());
            }
            assert(nextToken() == ")");
            shiftCur();
        }
        return new Node("A", name, ch);
    }
    else {
        Node * v = parseTerm();

        //db2(nextToken(), cur);
        //cerr << data[cur - 1] << data[cur] << data[cur + 1] << endl;

        if (nextToken() != "=") {
            return NULL;
        }
        //assert(nextToken() == "=");

        shiftCur();
        Node * u = parseTerm();
        return new Node("=", v, u);
    }

}

Node * Parser::parseTerm() {
    Node * v = parseSummand();        
    for (; nextToken() == "+"; ) {
        shiftCur();
        v = new Node("+", v, parseSummand());
    }
    return v;
}

Node * Parser::parseSummand() {
    Node * v = parseMultiply();
    for (; nextToken() == "*"; ) {
        shiftCur();
        v = new Node("*", v, parseMultiply());
    }
    return v;
}

Node * Parser::parseMultiply() {
    Node * result;
    if (nextToken() == "0") {
        shiftCur();
        result = new Node("0", "0");
    }
    else if (nextToken() == "(") {
        shiftCur();
        result = parseTerm();
        assert(nextToken() == ")");
        shiftCur();
    }
    else {
        string name; 
        assert(nextToken().size() == 1 && islower(nextToken()[0]));
        name += nextToken();
        shiftCur();
        for (; nextToken().size() == 1 && isdigit(nextToken()[0]); ) {
            name += nextToken();
            shiftCur();
        } 
        if (nextToken() == "(") {
            shiftCur();
            vector < Node * > ch;
            ch.pb(parseTerm());
            for (; nextToken() == ","; ) {
                shiftCur();
                ch.pb(parseTerm());
            }
            assert(nextToken() == ")");
            shiftCur();
            result = new Node("f", name, ch);
        }
        else {
            result = new Node("a", name);
        }
    }
    for (; nextToken() == "'"; ) {
        shiftCur();
        result = new Node("'", result, NULL);
    }
    return result;
}


Node * Parser::parseVar() {
    string res;
    assert(nextToken().size() == 1);
    assert(islower(nextToken()[0]));
    res += nextToken();
    shiftCur();
    for (; nextToken().size() == 1 && isdigit(nextToken()[0]); ) {
        res += nextToken();
        shiftCur();
    }
    return new Node("a", res);
}


Node * Parser::parseUnar() {
    //db("unar");
    if (nextToken() == "!") {
        shiftCur();
        Node * v = parseUnar();
        if (v == NULL) return NULL;
        return new Node("!", v, NULL);
    }
    if (nextToken() == "(") {
        int currentCur = cur;
        shiftCur();
        //db("________here");
        Node * v = parseExpr();
        //db2("__________", cur);
        
        if (v != NULL) {
            assert(nextToken() == ")");
            shiftCur();
            return v;
        }
        //else
            //db("work!!!!!!!!!!");
        cur = currentCur; 
    }
    if (nextToken() == "@" || nextToken() == "?") {
        Token curToken = nextToken();
        shiftCur();
        Node * v = parseVar();
        Node * u = parseUnar();
        if (u == NULL) return NULL;
        return new Node(curToken, v, u);
    }
    return parsePredicate();
}

Node * Parser::parseConjunct() {
    Node * v = parseUnar();
    if (v == NULL) return NULL;
    for (; nextToken() == "&"; ) {
        shiftCur();
        Node * u = parseUnar();
        if (u == NULL) return NULL;
        v = new Node("&", v, u);
    }
    return v;
}

Node * Parser::parseDisjunct() {
    Node * v = parseConjunct();
    if (v == NULL) return NULL;
    for (; nextToken() == "|"; ) {
        shiftCur();
        Node * u = parseConjunct();
        if (u == NULL) return NULL;
        v = new Node("|", v, u);
    }
    return v;
}


Node * Parser::parseExpr() {
    Node * v = parseDisjunct();
    if (v == NULL) return NULL;
    for (; nextToken() == "->"; ) {
        shiftCur();  
        Node * u = parseExpr();
        if (u == NULL) return NULL;
        v = new Node("->", v, u);
    }
    return v;

}
