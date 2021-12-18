#include "lList.hpp"

#include <iostream>
#include <cstddef> 
#include <stdexcept>
#include <algorithm>

Node* getNewNode(int value) {
    Node* new_node {new Node {value}};
    return new_node;
}

void lList::deepCopy(const lList& lL) {
    if(head_ref_) delete head_ref_;
    Node* curr_node {lL.head_ref_};
    if (lL.head_ref_) {
        while(curr_node) {
            this->append(curr_node->value_);
            curr_node = curr_node->next_;
        }
    } else {
        head_ref_ = nullptr;
    }

}

void lList::deleteNodes() {
    if(!head_ref_) return;
    Node* node_to_delete {head_ref_};
    Node* next_node {};
    while(node_to_delete) {
       next_node = node_to_delete->next_;
       delete node_to_delete;
       node_to_delete = next_node;
    }
    head_ref_ = nullptr;
    length_ = 0;
}

lList::lList(): length_ {0} {
    head_ref_ = nullptr;
}

lList::lList(const lList& lL) {
    deepCopy(lL);
}

std::size_t lList::length() const {
    return length_;
}

void lList::addAtBeginning(int value){ 
   Node* new_node {getNewNode(value) };
   new_node->next_ = head_ref_;
   head_ref_ = new_node;
   ++length_;
}

void lList::append(int value){ 
    Node* new_node {getNewNode(value) };
   ++length_;
    if (!head_ref_) {
        head_ref_ = new_node;
        return;
    }
    Node* tmp {head_ref_};
    while(tmp->next_) {
        tmp = tmp->next_;
    }
    tmp->next_ = new_node;
}

void lList::deleteKey(int key) {
    if (!head_ref_)
        return;

    // Write '--length' twice because I need it again in while loop
    if (key == head_ref_->value_) {
        head_ref_ = head_ref_->next_;
        --length_; 
        return;
    }
    Node* prev_node {};
    Node* node_to_delete{head_ref_};
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

void lList::deleteAtPosition(int position) {
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
    Node* prev_node {};
    // Start from curr_position 1 becasue we already checked if
    // position == 0
    Node* node_to_delete{head_ref_->next_};
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
void lList::pop() {
    if(!head_ref_)
        return;
    Node* node_to_delete {head_ref_};
    if(length_ == 1) {
        delete node_to_delete;
        head_ref_ = nullptr;
        return;
    }
    Node* prev_node {};
    while(node_to_delete->next_){
        prev_node = node_to_delete;
        node_to_delete = node_to_delete->next_;
    }
    prev_node ->next_ = nullptr;
    delete node_to_delete;
}

void lList::print() {
    Node* tmp {head_ref_};
    while(tmp) {
        std::cout << tmp->value_ << ' ';
        tmp = tmp->next_;
    }
    std::cout << '\n';
}

void lList::reverse() {
    if (!head_ref_) {
        return;
    }
    Node* prev_node {nullptr};
    Node* curr_node{head_ref_};
    Node* curr_node_next {};
    while(curr_node) {
        curr_node_next = curr_node->next_;
        curr_node->next_ = prev_node;
        prev_node = curr_node;

        curr_node = curr_node_next;
    }
    head_ref_ = prev_node;
}

bool lList::keyExists(int key) {
    if (!head_ref_)
        return false;
    Node* tmp {head_ref_};
    while(tmp) {
        if(tmp->value_ == key) {
            return true;
        }
        tmp = tmp->next_;
    }
    return false;
}

int& lList::at(int index) {
    if(index < 0 || index >= length_) 
        throw std::out_of_range("Index out of range!");
    if (!head_ref_)
        throw std::length_error("There are no elements in the list");
    Node* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp->value_;
}

Node* lList::nodeAt(int index) {
    if(index < 0 || index >= length_) 
        throw std::out_of_range("Index out of range!");
    if (!head_ref_)
        throw std::length_error("There are no elements in the list");
    Node* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp;
}

void lList::swapNodes(int index1, int index2) {
    if(index1 == index2) return;
    Node* first_node {nodeAt(index1)};
    Node* second_node {nodeAt(index2)};
    int a = first_node->value_;
    first_node->value_ = second_node->value_;
    second_node->value_ = a;
}

void lList::sort() {
   for(std::size_t i {1}; i < length_; ++i) {
       std::size_t counter {i};
        while(counter >= 1 && at(counter)< at(counter-1)) {
            swapNodes(counter, counter-1);
            --counter;
        }
   }
}

int& lList::operator[] (int index) {
    if(index < 0 || index >= length_ || !head_ref_) 
        throw std::out_of_range("Index out of range!");
    Node* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp->value_;
}

const int& lList::operator[] (int index) const {
    if(index < 0 || index >= length_ || !head_ref_) 
        throw std::out_of_range("Index out of range!");
    Node* tmp {head_ref_};
    for(std::size_t i{0}; i < index; ++i) {
        tmp = tmp->next_;
    }
    return tmp->value_;
}

lList operator+(lList &lL1, const lList &lL2) {
    if (!lL2.head_ref_)
        return lL1;
    
    lL1.length_ = lL1.length_ + lL2.length_;
    Node* last_node_in_list {lL1.head_ref_};
    while(last_node_in_list->next_) {
        last_node_in_list = last_node_in_list->next_;
    }
    last_node_in_list->next_ = (lL2.head_ref_); 
    return lL1;
}

lList& lList::operator= (const lList& lL) {
    if (this != &lL) {
        deleteNodes();
        deepCopy(lL);
    } 
    return *this;
}
