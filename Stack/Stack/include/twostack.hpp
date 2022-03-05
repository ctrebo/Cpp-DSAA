#ifndef TWO_STACK_H
#define TWO_STACK_H

#include <memory>
#include <array>
#include <cstddef>
#include <iostream>

#define MAX_SIZE_TWOSTACK 10

template<class T, int N>
class TwoStack {
private:
    int top1_ {};
    int top2_ {};
   
    std::array<T, N> arr_ {};
public:
    TwoStack();
    
    bool push1(T data);
    bool push2(T data);

    T top1();
    T top2();

    T pop1();
    T pop2();
};

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

#endif //TWO_STACK_H
