#include "ordinalParser.h"


Node::Node(string type, shared_ptr < Node > l, shared_ptr < Node > r): type(type), l(l), r(r) { }
Node::Node(string type, long long val): type(type), val(val) { }




string removeSpace(string s) {
    string res;
    for (auto ch: s)
        if (!isspace(ch))
            res += ch;
    return res;
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

string OrdinalParser::nextToken() {
    assert(cur <= (int)data.size());
    if (cur == (int)data.size()) 
        return "";
    return string(1, data[cur]);
}

void OrdinalParser::shiftCur() {
    cur++;
}


shared_ptr < Node > OrdinalParser::parseTerm() {
    if (nextToken() == "w") {
        shiftCur();
        return shared_ptr < Node > (new Node("w", 0));
    }
    if (nextToken() == "(") {
        shiftCur(); 
        auto v = parseExp();
        assert(nextToken() == ")");
        shiftCur();
        return v;
    }
    assert(nextToken().size() == 1 && isdigit(nextToken()[0]));
    long long x = 0;
    while (nextToken().size() == 1 && isdigit(nextToken()[0])) {
        int d = nextToken()[0] - '0';
        x = x * 10 + d;
        shiftCur();
    }
    return shared_ptr < Node > (new Node("0", x));
}

shared_ptr < Node > OrdinalParser::parseMultiplier() {
    auto v = parseTerm();
    while (nextToken() == "^") {
        shiftCur();
        auto u = parseMultiplier();
        v = shared_ptr < Node > (new Node("^", v, u));
    }
    return v;
}

shared_ptr < Node > OrdinalParser::parseSummand() {
    auto v = parseMultiplier();
    while (nextToken() == "*") {
        shiftCur();
        auto u = parseMultiplier();
        v = shared_ptr < Node > (new Node("*", v, u));
    }
    return v;
}

shared_ptr < Node > OrdinalParser::parseExp() {
    auto v = parseSummand();
    while (nextToken() == "+") {
        shiftCur();
        auto u = parseSummand();
        v = shared_ptr < Node > (new Node("+", v, u));
    }
    return v;
}


shared_ptr < Node > parseOrdinal(string s) {
    OrdinalParser p(s);
    return p.parseExp(); 
}




