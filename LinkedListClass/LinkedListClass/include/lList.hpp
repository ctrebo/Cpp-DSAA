#ifndef LLIST_HPP
#define LLIST_HPP

#include "node.hpp"
#include <cstddef> 
#include <algorithm> 

class lList {
private:
    Node* head_ref_ {};
    std::size_t length_ {};

    void deepCopy(const lList& lL);
    void deleteNodes();
public:
    lList();
    lList(const lList& lL);
    
    std::size_t length() const;

    void addAtBeginning(int value);
    void append(int value);
    void deleteKey(int key);
    void deleteAtPosition(int position);
    void pop();
    void print();
    void reverse();
    bool keyExists(int key);
    int& at(int index);
    Node* nodeAt(int index);
    void swapNodes(int index1, int index2);
    void sort();

    int& operator[] (int index);
    const int& operator[] (int index) const;
    lList& operator= (const lList& lL);
	friend lList operator+(lList &lL1, const lList &lL2);
};

#endif // LLIST_HPP
