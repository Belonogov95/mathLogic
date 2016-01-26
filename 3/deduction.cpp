#include "main.h"
#include "parser.h"

struct Deduction {
    vector < Line > * lines;
    string term;

    vector < Line > * answer;

    Deduction(vector < Line > * lines, string term):lines(lines), term(term) { }




    void makeAA() {
        vector < string > tmp;
        tmp.pb("(A)->(A)->(A)");
        tmp.pb("(A)->((A)->(A))->(A)");
        tmp.pb("((A)->(A)->(A))->((A)->((A)->(A))->(A))->((A)->(A))");
        tmp.pb("((A)->((A)->(A))->(A))->((A)->(A))");
        for (auto s: tmp)
            answer->pb(
            answer[(int)answer.size() - i] = replace(answer[(int)answer.size() - i], 'A', A);
    }


    vector < Line > * go() {
        vector < string > nAssumption;
        for (auto x: (*lines)[0].assumption)
            if (x != term)
                nAssumption.pb(x);
         answer = new vector < Line >();
    
        makeAA();
        for (int i = 0; i < (int)lines->size(); i++) {


        }

            

        for (int i = 0; i < (int)answer->size(); i++)
            (*answer)[0].assumption = nAssumption;
        return answer;
    }
};








