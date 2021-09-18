#ifndef TWO_STACK_H
#define TWO_STACK_H

#include <memory>
#include <array>
#include <cstddef>

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

#include "twoStack.tpp"

#endif //TWO_STACK_H
