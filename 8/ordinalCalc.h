#include "main.h"
#include "ordinalParser.h"

#ifndef ORDINAL_CALC
#define ORDINAL_CALC

struct Ordinal {
    Ordinal * l, * r;
    string type;
    long long val;
    Ordinal(Ordinal * l, Ordinal * r);
    Ordinal(long long val);
};

Ordinal * evaluate(shared_ptr < Node > ptr);


Ordinal * first(Ordinal * a);

Ordinal * rest(Ordinal * a);

Ordinal * firstn(Ordinal * a, int n);

bool atom(Ordinal * a);

Ordinal * restn(Ordinal * a, int n);

int len(Ordinal * a);

Ordinal * fe(Ordinal * a);

int size(Ordinal * a);

Ordinal * fc(Ordinal * a);

Ordinal * append(Ordinal * a, Ordinal * b);

int cmpW(Ordinal * p, Ordinal * q);

Ordinal * subW(Ordinal * a, Ordinal * b);

Ordinal * addW(Ordinal * a, Ordinal * b);

bool lessW(Ordinal * a, Ordinal * b);

bool lessEW(Ordinal * a, Ordinal * b);

int cmpO(Ordinal * a, Ordinal * b);

bool lessO(Ordinal * a, Ordinal * b);

Ordinal * addW(Ordinal * a, Ordinal * b);

Ordinal * subW(Ordinal * a, Ordinal * b);

Ordinal * addO(Ordinal * a, Ordinal * b);

Ordinal * subO(Ordinal * a, Ordinal * b);

Ordinal * mulO(Ordinal * a, Ordinal * b);


#endif


