//
// Created by Fleshmix on 24.10.2019.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <typeinfo>

#include "stack_error.h"

#define CANARY 0xDEADBEEF

#define INITIALIZE 201
#define PUSH 202
#define POP 203
#define REALLOC 204

template <typename T>
class Stack {
private:
    double can1_ = CANARY;
    int state_ = 0;
    int cur_function_ = 0;
    unsigned long data_hash_ = 0;
    unsigned long stack_hash_ = 0;
    size_t size_ = 1;
    size_t capacity_ = 0;
    T* data_ = NULL;
    double can2_ = CANARY;

    void OK();
    void realloc_();
    unsigned long hash_counter_();
    unsigned long params_hash_counter_();
public:
    void initialize(size_t capacity);
    void push(T val);
    T pop();
    void Dump();
};


#endif //STACK_STACK_H
