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

template<class T, int size>
Queue<T, size>::Queue(): capacity_ {size}, rear_ {-1}, front_ {-1} {}


template<class T, int size>
void Queue<T, size>::dequeue() {
    if(isEmpty())
        throw std::underflow_error("Underflow occured!");
    else if(front_==rear_) {
        front_, rear_ = -1;
    } else
        front_ = (front_+1)%capacity_;
}


template<class T, int size>
void Queue<T, size>::enqueue(T data) {
    if(isFull()) {
        std::cout << "Overflow";
        throw std::overflow_error("Overflow occured!");
    } else if(isEmpty()) {
        front_= 0;
        rear_ = 0;
    } else {
        rear_ = (rear_+1)%capacity_;
    }
    arr_[rear_] = data;
}

template<class T, int size>
bool Queue<T, size>::isEmpty() {
    return (rear_ == -1 && front_ == -1) ? true: false;
}

template<class T, int size>
bool Queue<T, size>::isFull() {
    return (((rear_ +1) %capacity_)== front_) ? true: false;
}


template<class T, int size>
void Queue<T, size>::print() {
    std::cout << front_ << ' ' << rear_ << "\n\n";
    for(int counter {rear_}; counter != front_ -1; counter = (counter-1)%capacity_) {
        std::cout << counter << ' ';
        std::cout << arr_[counter] << '\n';
    }
    std::cout << '\n';
}

#endif // QUEUE_H
