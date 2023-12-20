#include <bits/stdc++.h>

using namespace std;

struct base {
    base();
    virtual void print() = 0;
    void baseFunction();
};

base::base() { cout << "Base" << endl;}

void base::baseFunction() { cout << "Base Function" << endl; }

class inherit : public base {
    public:
        inherit(int);
        void print();
        void inheritFunction();
    private:
        int info;
};

inherit::inherit(int info) { cout << "Inherit with info " << info << endl; }

void inherit::print() { cout << "Pure virtual function used" << endl; }

void inherit::inheritFunction() { cout << "Inherit Function" << endl; }

int main() {
    inherit inheritObj(5);
    inheritObj.print();
    inheritObj.baseFunction();
    inheritObj.inheritFunction();
    return 0;
}