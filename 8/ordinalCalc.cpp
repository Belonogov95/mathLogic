#include "main.h"
#include "ordinalCalc.h"




Ordinal::Ordinal(Ordinal * l, Ordinal * r):l(l), r(r), type("p"), val(0) { } 

Ordinal::Ordinal(long long val): l(NULL), r(NULL), type("val"), val(val) { }



Ordinal * evaluate(shared_ptr < Node > ptr) {
    if (ptr->type == 
    
}


Ordinal * first(Ordinal * a) {
    return a->l;
}

Ordinal * rest(Ordinal * a) {
    return a->r;
}


Ordinal * firstn(Ordinal * a, int n) {
    if (n == 0) return NULL;
    return new Ordinal(first(a), firstn(rest(a), n - 1));
}

bool atom(Ordinal * a) {
    return a->type != "p"; 
}

Ordinal * restn(Ordinal * a, int n) {
    if (n == 0) return a;
    return restn(rest(a), n - 1);
}

int len(Ordinal * a) {
    if (atom(a)) return 0;
    return 1 + len(rest(a));
}

Ordinal * fe(Ordinal * a) {
    if (atom(a)) return 0;
    return first(first(a));
}

int size(Ordinal * a) {
    if (atom(a)) return 1;
    return size(fe(a)) + size(rest(a));
}

Ordinal * fc(Ordinal * a) {
    if (atom(a)) return a;
    return rest(first(a));
}


Ordinal * append(Ordinal * a, Ordinal * b) {
    if (atom(a)) return b;
    return new Ordinal(first(a), append(rest(a), b));
}

int cmpW(Ordinal * p, Ordinal * q) {
    assert(p->type == "val");
    assert(q->type == "val");
    if (p->val < q->val) return -1;
    if (p->val > q->val) return 1;
    return 0;
}

int cmpO(Ordinal * a, Ordinal * b) {
    if (atom(a) && atom(b)) return cmpW(a, b);
    if (atom(a)) return -1;
    if (atom(b)) return 1;
    int res1 = cmpO(fe(a), fe(b));
    if (res1 != 0) return res1;

    int res2 = cmpW(fc(a), fc(b));
    if (res2 != 0) return res2;
    return cmpO(rest(a), rest(b));
}

bool lessO(Ordinal * a, Ordinal * b) {
    if (cmpO(a, b) == -1) return 1;
    return 0;
}

Ordinal * addW(Ordinal * a, Ordinal * b) {
    assert(a->type == "val");
    assert(b->type == "val");
    return new Ordinal(a->val + b->val);
}

Ordinal * subW(Ordinal * a, Ordinal * b) {
    assert(a->type == "val");
    assert(b->type == "val");
    assert(a->val >= b->val);
    return new Ordinal(a->val - b->val);
}



Ordinal * addO(Ordinal * a, Ordinal * b) {
    if (atom(a) && atom(b)) return addW(a, b);
    int res = cmpO(fe(a), fe(b));
    if (res == -1) return b;
    if (res == 0) 
        return new Ordinal(new Ordinal(fe(a), addW(fc(a), fc(b))), rest(b));
    return new Ordinal(new Ordinal(fe(a), fc(a)), addO(rest(a) , b));
}

bool lessEW(Ordinal * a, Ordinal * b) {
    assert(a->type == "val");
    assert(b->type == "val");
    return a->val <= b->val;
}

bool lessW(Ordinal * a, Ordinal * b) {
    assert(a->type == "val");
    assert(b->type == "val");
    return a->val < b->val;
}

Ordinal * subO(Ordinal * a, Ordinal * b) {
    if (atom(a) && atom(b) && lessEW(a, b)) return new Ordinal(0);
    if (atom(a) && atom(b)) return subW(a, b);
    if (cmpO(fe(a), fe(b)) == -1) return new Ordinal(0);
    if (cmpO(fe(a), fe(b)) == 1) return a;
    if (lessW(fc(a), fc(b))) return 0;
    if (lessW(fc(b), fc(a))) 
        return new Ordinal(new Ordinal(fe(a), addW(fc(a), fc(b))), rest(a));
    return subO(rest(a), rest(b));
}


Ordinal * mulW(Ordinal * a, Ordinal * b) {
    assert(a->type == "val");
    assert(b->type == "val");
    return new Ordinal(a->val * b->val);
}

Ordinal * mulO(Ordinal * a, Ordinal * b) {
    if (atom(a) && a->val == 0) return new Ordinal(0);
    if (atom(b) && b->val == 0) return new Ordinal(0);

    if (atom(a) && atom(b)) return mulW(a, b);
    if (atom(b)) return new Ordinal(new Ordinal(fe(a), mulW(fc(a), b)), rest(a));
    return new Ordinal(new Ordinal(addO(fe(a), fe(b)), fc(b)), mulO(a, rest(b)));
}







