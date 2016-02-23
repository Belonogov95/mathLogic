#include "main.h"
#include "parser.h"


Node::Node(string type, Node * l, Node * r): type(type), l(l), r(r) { }
Node::Node(string type, string val): type(type), val(val) { }
Node::Node(string type, string val, vector < Node * > ch): type(type), val(val), ch(ch) { }

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
    cerr << "type val: " << v->type << " " << v->val << endl;
    printTree(v->l, tab + 1);
} 

Parser::Parser(string s) {
    data = removeSpace(s);
    cur = 0;
}

string Parser::nextToken() {
    assert(cur < (int)data.size());
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
        assert(nextToken() == "=");
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
        result = new Node("0", NULL, NULL);
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
    if (nextToken() == "!") {
        shiftCur();
        return new Node("!", parseUnar(), NULL);
    }
    if (nextToken() == "(") {
        shiftCur();
        Node * v = parseExpr();
        assert(nextToken() == ")");
        shiftCur();
        return v;
    }
    if (nextToken() == "@" || nextToken() == "?") {
        Token curToken = nextToken();
        shiftCur();
        Node * v = parseVar();
        Node * u = parseUnar();
        return new Node(curToken, v, u);
    }
    return parsePredicate();
}

Node * Parser::parseConjunct() {
    Node * v = parseUnar();
    for (; nextToken() == "&"; ) {
        shiftCur();
        Node * u = parseUnar();
        v = new Node("&", v, u);
    }
    return v;
}

Node * Parser::parseDisjunct() {
    Node * v = parseConjunct();
    for (; nextToken() == "|"; ) {
        shiftCur();
        Node * u = parseConjunct();
        v = new Node("|", v, u);
    }
    return v;
}


Node * Parser::parseExpr() {
    Node * v = parseDisjunct();
    for (; nextToken() == "->"; ) {
        shiftCur();  
        Node * u = parseExpr();
        v = new Node("->", v, u);
    }
    return v;

}








