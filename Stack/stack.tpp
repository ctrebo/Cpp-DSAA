#ifndef STACK_TPP
#define STACK_TPP

#ifndef STACK_H
#error __FILE__ should only be included from myclass.h.
#endif // STACK_H


#include "stack.h" 
#include <iostream>


template<class T, int size>
Stack<T, size>::Stack(): top_ {-1} {}


template<class T, int size>
void Stack<T, size>::insertAtBottom(int data) {
    if(isEmpty()) {
        push(data);
    } else {
        T popped_element {pop()};
        insertAtBottom(data);
        push(popped_element);
    }
}


template<class T, int size>
void Stack<T, size>::sortedInsert(int data) {
    if(isEmpty() || data>peek()) {
        push(data);
        return;
    }
    T popped_element { pop() };
    sortedInsert(data);
    push(popped_element);
    
}

template<class T, int size>
bool Stack<T, size>::push(T data) {
    if(top_ >= size - 1){
        std::cout << "Stack overflow\n";
        return false;
    } else {
        arr_[++top_] = data;
        std::cout << "Pushed " << data << " to the stack\n";
        return true;
    } 
}


template<class T, int size>
T Stack<T, size>::pop() {
    if(top_ < 0) {
        std::cout << "Stack underflow\n";
        return 0;
    } else
        return arr_[top_--];
}


template<class T, int size>
T Stack<T, size>::peek() {
    if(top_<0) {
        std::cout << "Stack underflow\n";
        return 0;
    } else {
        return arr_[top_];
    }
}


template<class T, int size>
bool Stack<T, size>::isEmpty() {
    return (top_ < 0);    
}

template<class T, int size>
void Stack<T, size>::reverse() { 
   if(isEmpty()) return;

   T popped_element {pop()}; 
   reverse();
   insertAtBottom(popped_element);
}


template<class T, int size>
void Stack<T, size>::sort() {
    if(isEmpty()) return;

    T popped_element { pop()};
    sort();
    sortedInsert(popped_element);
}

template<class T, int size>
void Stack<T, size>::print() {
    for(int i{0}; i <= top_; ++i) {
        std::cout << arr_[i] << ' ';
    }
    std::cout << '\n';

}


#endif //STACK_TPP
