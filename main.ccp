#include <iostream>
#include "stack.h"
#include "stack.cpp"

#define DISABLE_PROTECTION

int main() {
    Stack <double> s = {};
    s.initialize(4);
    s.push(1);
    s.push(2);
    s.push(3);
    int a = s.pop();
    s.pop();
    s.pop();
    s.pop();
}
