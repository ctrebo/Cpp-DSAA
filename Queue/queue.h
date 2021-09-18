#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <array>
#include <exception>

template<class T, int size>
class Queue {
private:
    int front_ {};
    int rear_ {};
    int capacity_ {};
    std::array<T, size> arr_{};
   
    
public:
    Queue();

    void dequeue();
    void enqueue(T data);
    T peek();
    int capacity();
    void print();
    bool isEmpty();
    bool isFull();

    
};

#include "queue.tpp"

#endif // QUEUE_H
