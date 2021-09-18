#ifndef STACK_H
#define STACK_H

#include <string>
#include <array>


template<class T, int size>
class Stack {
private:
    int top_ {};
    std::array<T, size> arr_ {};
   
    // Private because these functions should only be
    // used inside other public functions.
    void insertAtBottom(int data);
    void sortedInsert(int data);
public:
    Stack();
    bool push(T data);
    T pop();
    T peek();
    bool isEmpty();

    void reverse();
    void sort();
    void print();
    
};

#include "stack.tpp"

int prec(char c);
void infixToPostfix(std::string& s);
void reverseString(std::string& str); 
bool balancedBrackets(std::string& str);
void calculateSpan(int price[], int n, int S[]);
#endif
