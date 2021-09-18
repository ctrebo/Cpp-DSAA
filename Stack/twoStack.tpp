#ifndef TWO_STACK_TPP
#define TWO_STACK_TPP

#ifndef TWO_STACK_H
#error __FILE__ should only be included from twoClass.h.
#endif // TWO_STACK_H

#include "twoStack.h" 
#include <iostream>



template<class T, int N>
TwoStack<T, N>::TwoStack(): top1_{-1}, top2_{N / 2 -1} {}

template<class T, int N>
bool TwoStack<T, N>::push1(T data) {
    if(top1_ < N/2 -1) {
        arr_[++top1_] = data;
        return true;
    } else {
        std::cout << "Stack Overflow\n";
        return false;
    }
}

template<class T, int N>
bool TwoStack<T, N>::push2(T data) {
    if(top2_ < N -1) {
        arr_[++top2_] = data;
        return true;
    } else {
        std::cout << "Stack Overflow\n";
        return false;
    }
}


template<class T, int N>
T TwoStack<T, N>::pop1() {
    if(top1_ < 0) {
        std::cout << "Stack underflow\n";
        return static_cast<T>(0);
    } else {
        return arr_[top1_--];
    }
}

template<class T, int N>
T TwoStack<T, N>::pop2() {
    if(top2_ < N/2) {
        std::cout << "Stack underflow\n";
        return static_cast<T>(0);
    } else {
        return arr_[top2_--];
    }
}

template<class T, int N>
T TwoStack<T, N>::top1() {
    if(top1_ < 0) {
        std::cout << "Stack underflow\n";
        return static_cast<T>(0);
    } else {
        return arr_[top1_];
    }
}

template<class T, int N>
T TwoStack<T, N>::top2() {
    if(top2_ < N/2) {
        std::cout << "Stack underflow\n";
        return static_cast<T>(0);
    } else {
        return arr_[top2_];
    }
}


#endif //TWO_STACK_TPP
