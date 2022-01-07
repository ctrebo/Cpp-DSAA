#ifndef NODE_HPP
#define NODE_HPP

template <class T>
class Node {
public:
    T value_ {};
    Node<T>* next_ {};

    Node();
    Node(T value);
};


template <class T>
Node<T>::Node(): next_ {nullptr} {}

template <class T>
Node<T>::Node(T value): value_ {value}, next_ {nullptr} {}

#endif // NODE_HPP
