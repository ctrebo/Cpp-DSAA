#ifndef LLIST_HPP
#define LLIST_HPP

#include "node.hpp"
#include <iostream>
#include <cstddef> 
#include <stdexcept>
#include <algorithm>

template<class T>
class lList;

template<class T>
lList<T> operator+(lList<T> &lL1, const lList<T> &lL2);

template <class T>
class lList {
private:
    Node<T>* head_ref_ {};
    std::size_t length_ {};

    void deepCopy(const lList& lL);
    void deleteNodes();
public:
    lList();
    lList(const lList& lL);
    
    std::size_t length() const;

    void addAtBeginning(T value);
    void append(T value);
    void deleteKey(T key);
    void deleteAtPosition(int position);
    void pop();
    void print();
    void reverse();
    bool keyExists(T key);
    T& at(int index);
    Node<T>* nodeAt(int index);
    void swapNodes(int index1, int index2);
    void sort();

    T& operator[] (int index);
    const int& operator[] (int index) const;
    lList<T>& operator= (const lList<T>& lL);
	friend lList<T> operator+<>(lList<T> &lL1, const lList<T> &lL2);
};

template <class T>
Node<T>* getNewNode(T value) {
    Node<T>* new_node {new Node<T> {value}};
    return new_node;
}

template <class T>
void lList<T>::deepCopy(const lList<T>& lL) {
    if(head_ref_) delete head_ref_;
    Node<T>* curr_node {lL.head_ref_};
    if (lL.head_ref_) {
        while(curr_node) {
            this->append(curr_node->value_);
            curr_node = curr_node->next_;
        }
    } else {
        head_ref_ = nullptr;
    }
}

template <class T>
void lList<T>::deleteNodes() {
    if(!head_ref_) return;
    Node<T>* node_to_delete {head_ref_};
    Node<T>* next_node {};
    while(node_to_delete) {
       next_node = node_to_delete->next_;
       delete node_to_delete;
       node_to_delete = next_node;
    }
    head_ref_ = nullptr;
    length_ = 0;
}

template <class T>
lList<T>::lList(): length_ {0} {
    head_ref_ = nullptr;
}

template <class T>
lList<T>::lList(const lList& lL) {
    deepCopy(lL);
}

template <class T>
std::size_t lList<T>::length() const {
    return length_;
}

template <class T>
void lList<T>::addAtBeginning(T value){ 
   Node<T>* new_node {getNewNode(value) };
   new_node->next_ = head_ref_;
   head_ref_ = new_node;
   ++length_;
}

template <class T>
void lList<T>::append(T value){ 
    Node<T>* new_node {getNewNode(value) };
   ++length_;
    if (!head_ref_) {
        head_ref_ = new_node;
        return;
    }
    Node<T>* tmp {head_ref_};
    while(tmp->next_) {
        tmp = tmp->next_;
    }
    tmp->next_ = new_node;
}

template <class T>
void lList<T>::deleteKey(T key) {
    if (!head_ref_)
        return;

    // Write '--length' twice because I need it again in while loop
    if (key == head_ref_->value_) {
        head_ref_ = head_ref_->next_;
        --length_; 
        return;
    }
    Node<T>* prev_node {};
    Node<T>* node_to_delete{head_ref_};
    while(node_to_delete->value_ != key && node_to_delete != nullptr) {
        prev_node = node_to_delete;
        node_to_delete = node_to_delete->next_;
    }
    if(!node_to_delete)
        return;
    prev_node->next_ = node_to_delete->next_;
    --length_; 
    delete node_to_delete;
}

template <class T>
void lList<T>::deleteAtPosition(int position) {
    if (position < 0 or position > length_) {
        throw std::out_of_range("Index out of range!");
    }
    if (!head_ref_)
        return;
    --length_; 
    if (position == 0) {
        head_ref_ = head_ref_->next_;
        return;
    }
    Node<T>* prev_node {};
    // Start from curr_position 1 becasue we already checked if
    // position == 0
    Node<T>* node_to_delete{head_ref_->next_};
    int curr_position {1};
    while(curr_position != position) {
        prev_node = node_to_delete;
        node_to_delete = node_to_delete->next_;
        ++curr_position;
    }
    prev_node->next_ = node_to_delete->next_;
    delete node_to_delete;
}

// Remove last node
template <class T>
void lList<T>::pop() {
    if(!head_ref_)
        return;
    Node<T>* node_to_delete {head_ref_};
    if(length_ == 1) {
        delete node_to_delete;
        head_ref_ = nullptr;
        return;
    }
    Node<T>* prev_node {};
    while(node_to_delete->next_){
        prev_node = node_to_delete;
        node_to_delete = node_to_delete->next_;
    }
    prev_node ->next_ = nullptr;
    delete node_to_delete;
}

template <class T>
void lList<T>::print() {
    Node<T>* tmp {head_ref_};
    while(tmp) {
        std::cout << tmp->value_ << ' ';
        tmp = tmp->next_;
    }
    std::cout << '\n';
}

template <class T>
void lList<T>::reverse() {
    if (!head_ref_) {
        return;
    }
    Node<T>* prev_node {nullptr};
    Node<T>* curr_node{head_ref_};
    Node<T>* curr_node_next {};
    while(curr_node) {
        curr_node_next = curr_node->next_;
        curr_node->next_ = prev_node;
        prev_node = curr_node;

        curr_node = curr_node_next;
    }
    head_ref_ = prev_node;
}

template <class T>
bool lList<T>::keyExists(T key) {
    if (!head_ref_)
        return false;
    Node<T>* tmp {head_ref_};
    while(tmp) {
        if(tmp->value_ == key) {
            return true;
        }
        tmp = tmp->next_;
    }
    return false;
}

template <class T>
T& lList<T>::at(int index) {
    if(index < 0 || index >= length_) 
        throw std::out_of_range("Index out of range!");
    if (!head_ref_)
        throw std::length_error("There are no elements in the list");
    Node<T>* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp->value_;
}

template <class T>
Node<T>* lList<T>::nodeAt(int index) {
    if(index < 0 || index >= length_) 
        throw std::out_of_range("Index out of range!");
    if (!head_ref_)
        throw std::length_error("There are no elements in the list");
    Node<T>* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) 
        tmp = tmp->next_;
    return tmp;
}

template <class T>
void lList<T>::swapNodes(int index1, int index2) {
    if(index1 == index2) return;
    Node<T>* first_node {nodeAt(index1)};
    Node<T>* second_node {nodeAt(index2)};
    T a = first_node->value_;
    first_node->value_ = second_node->value_;
    second_node->value_ = a;
}

template <class T>
void lList<T>::sort() {
   for(std::size_t i {1}; i < length_; ++i) {
       std::size_t counter {i};
        while(counter >= 1 && at(counter)< at(counter-1)) {
            swapNodes(counter, counter-1);
            --counter;
        }
   }
}

template <class T>
T& lList<T>::operator[] (int index) {
    if(index < 0 || index >= length_ || !head_ref_) 
        throw std::out_of_range("Index out of range!");
    Node<T>* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp->value_;
}

template <class T>
const int& lList<T>::operator[] (int index) const {
    if(index < 0 || index >= length_ || !head_ref_) 
        throw std::out_of_range("Index out of range!");
    Node<T>* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp->value_;
}

template <class T>
lList<T> operator+(lList<T> &lL1, const lList<T> &lL2) {
    if (!lL2.head_ref_)
        return lL1;
    
    lL1.length_ = lL1.length_ + lL2.length_;
    Node<T>* last_node_in_list {lL1.head_ref_};
    while(last_node_in_list->next_) {
        last_node_in_list = last_node_in_list->next_;
    }
    last_node_in_list->next_ = (lL2.head_ref_); 
    return lL1;
}

template <class T>
lList<T>& lList<T>::operator= (const lList<T>& lL) {
    if (this != &lL) {
        deleteNodes();
        deepCopy(lL);
    } 
    return *this;
}
#endif // LLIST_HPP
