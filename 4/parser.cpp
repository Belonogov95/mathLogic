#include "main.h"
#include "parser.h"




Node::Node(string type, Node * l, Node * r): type(type), l(l), r(r) { }
Node::Node(string type, string val): type(type), val(val) { }

string removeSpace(string s) {
    string res;
    for (auto ch: s)
        if (!isspace(ch))
            res += ch;
    return res;
}


Parser::Parser(string s) {
    data = removeSpace(s);
    cur = 0;
}

string Parser::nextToken() {

    assert(false);
}

void Parser::shiftCur() {

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








