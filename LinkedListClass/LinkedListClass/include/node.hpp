#ifndef NODE_HPP
#define NODE_HPP

class Node {
public:
    int value_ {};
    Node* next_ {};

    Node(): next_ {nullptr} {}
    Node(int value): value_ {value}, next_ {nullptr} {}

};

#endif
