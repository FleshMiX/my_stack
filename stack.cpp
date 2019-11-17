//
// Created by Fleshmix on 24.10.2019.
//

#include "stack.h"
#include "stack_error.h"

#define MAX(num1, num2) ((num1) > (num2) ? (num1) : (num2))

template <typename T>
void Stack<T>::initialize (size_t capacity) {
    capacity_ = capacity;
    data_ = new T [capacity+2];
    data_[0] = (T)CANARY;
    data_[capacity_+1] = (T)CANARY;
    data_hash_ = hash_counter_();
    stack_hash_ = params_hash_counter_();

    cur_function_ = INITIALIZE;
    OK();
}

template <typename T>
void Stack<T>::push (T val) {
    OK();
    if(size_>capacity_) {
        realloc_();
    }
    data_[size_++] = val;

    data_hash_ = hash_counter_();
    stack_hash_ = params_hash_counter_();
    cur_function_ = PUSH;
    OK();
}


template <typename T>
T Stack<T>:: pop() {
    size_--;
    data_hash_ = hash_counter_();
    stack_hash_ = params_hash_counter_();
    cur_function_ = POP;
    OK();
    return data_[size_];
}

template <typename T>
unsigned long Stack<T>::hash_counter_() {
    unsigned long hash = 142313;
    for (int i = 0; i < size_; i++)
        hash += (int)pow((i*(int)data_[i]), 4*i) % 9395749;
    return hash;
}

template <typename T>
unsigned long Stack<T>::params_hash_counter_() {
    return (10219323+(int)pow(size_, capacity_*32)) % 2748124;
}

template <typename T>
void Stack<T>::OK() {
    if (data_ == nullptr) {
        state_ = NULL_DATA_POINTER;
        Dump();
        exit(NULL_DATA_POINTER);
    }
    if (capacity_ == 0) {
        state_ = ZERO_CAPACITY;
        Dump();
        exit(ZERO_CAPACITY);
    }
    if (size_ < 1) {
        state_ = NEGATIVE_SIZE;
        Dump();
        exit(NEGATIVE_SIZE);
    }
    if (data_hash_ != hash_counter_()) {
        state_ = BAD_DATA_HASH;
        Dump();
        exit(BAD_DATA_HASH);
    }
    if (data_[0] != (T)CANARY || data_[capacity_+1] != (T)CANARY) {
        state_ = DEAD_DATA_CANARY;
        Dump();
        exit(DEAD_DATA_CANARY);
    }
    if (can1_ != CANARY || can2_ != CANARY) {
        state_ = DEAD_STACK_CANARY;
        Dump();
        exit(DEAD_STACK_CANARY);
    }
    if (this == nullptr) {
        state_ = STACK_ZERO_POINTER;
        Dump();
        exit(STACK_ZERO_POINTER);
    }
    if (state_ == BAD_REALLOC) {
        state_ = BAD_REALLOC;
        Dump();
        exit(BAD_REALLOC);
    }
    if (stack_hash_ != params_hash_counter_()) {
        state_ = BAD_STACK_HASH;
        Dump();
        exit(BAD_STACK_HASH);
    }
}

template <typename T>
void Stack<T>::realloc_() {
    OK();
    T* temp = new T [capacity_+4];
    if (&temp == nullptr) state_ = BAD_REALLOC;
    OK();

    memcpy (temp, data_, sizeof(T) * (capacity_+1));
    delete data_;

    data_ = temp;
    capacity_ += 4;
    data_[capacity_+1] = (T)CANARY;
    data_hash_ = hash_counter_();
    cur_function_ = REALLOC;
    OK();
}

template <typename T>
void Stack<T>::Dump() {
    //std::cout.unsetf(std::ios::dec);
    //std::cout.setf(std::ios::hex);
    std::cout << "Stack <" << typeid(T).name() << "> [" << this << "]";
    if (state_ == 0) std::cout << " (OK)\n{" << std::endl;
    else std::cout << " (!!!NOT OK!!!)\n{" << std::endl;
    std::cout << "\tcan1_ = " << can1_ << std::endl;
    std::cout << "\tstate_ = " << state_ << " ";
    switch (state_) {
        case NULL_DATA_POINTER :
            std::cout << "(The data_ pointer is zero. Try to use Stack::initialize)" << std::endl;
            break;
        case ZERO_CAPACITY :
            std::cout << "(Stack capacity is zero. Perhaps Stack::initialize(0) called)" << std::endl;
            break;
        case NEGATIVE_SIZE :
            std::cout << "(Stack size is less then 1. Perhaps bad pop)" << std::endl;
            break;
        case BAD_STACK_HASH :
            std::cout << "(Stack hash was changed out of Stack functions)" << std::endl;
            break;
        case BAD_DATA_HASH:
            std::cout << "(Data hash was changed out of Stack functions)" << std::endl;
            break;
        case DEAD_DATA_CANARY :
            std::cout << "(Canaries in stack are dead)" << std::endl;
            break;
        case DEAD_STACK_CANARY :
            std::cout << "(Stack params canaries are dead)" << std::endl;
            break;
        case STACK_ZERO_POINTER :
            std::cout << "(Pointer to stack is zero)" << std::endl;
            break;
        case BAD_REALLOC :
            std::cout << "(Realloc couldn't find enough place)" << std::endl;
            break;
        default :
            std::cout << "(NO ERROR)" << std::endl;
            break;
    }
    std::cout << "\tcur_function_ = " << cur_function_ << " ";
    switch (cur_function_) {
        case INITIALIZE :
            std::cout << "(Error in initialize func)" << std::endl;
            break;
        case POP :
            std::cout << "(Error in pop func)" << std::endl;
            break;
        case PUSH :
            std::cout << "(Error in push func)" << std::endl;
            break;
        case REALLOC :
            std::cout << "(Error in realloc func)" << std::endl;
            break;
        default:
            std::cout << "(Bad cur_function_ value)" << std::endl;
    }
    size_t size_ = 1;
    size_t capacity_ = 0;
    T* data_ = NULL;
    unsigned long data_hash_ = 0;
    unsigned long stack_hash_ = 0;
    double can2_ = CANARY;
    std::cout << "\tdata_hash_ = " << data_hash_ << std::endl;
    std::cout << "\tstack_hash_ = " << stack_hash_ << std::endl;
    std::cout << "\tsize_ = " << size_ << std::endl;
    std::cout << "\tcapacity_ = " << capacity_ << std::endl;
    std:: cout << "\tdata_[" << data_ << "]" << std::endl;
    if (capacity_ == 0) std::cout << "\tdata_ empty" <<std:: endl;
    for(int i = 0; i < capacity_; i++)
        std::cout << "\tdata_[" << i << "] = " << data_[i] << std::endl;
    std::cout << "\tcan2_ = " << can2_ << std::endl;
    std::cout << "}" << std::endl;
}
