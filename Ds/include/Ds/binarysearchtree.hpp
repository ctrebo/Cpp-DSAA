#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <cstddef>
#include <iostream>
#include <stack>
#include <utility>

namespace ds {

template<class T>
class BST {
private:
    class Node;
    class Iterator;
public:
    using value_type = T;
    using node_pointer = Node*;
    using const_node_pointer = const Node*;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    
public:
    BST();
    ~BST();
    
private:
    node_pointer root_;
private:

    void deallocNode(Node* node);
    // Returns boolean that tells if insertion was successful
    std::pair<BST<T>::Iterator, bool> exists(const T& value) const;
    template<class Type>
    std::pair<BST<T>::Iterator, bool> insertPriv(Type&& value);
    void destruct(Node* node);
};

template<class T>
class BST<T>::Node {
public:
    Node(T data, Node* left=nullptr, Node* right=nullptr): data_{data}, left_ {left}, right_ {right}
    {}

    static Node* createNode(T value, Node* left=nullptr,Node* right=nullptr) {
        Node* new_node {new Node {value, left, right}};     
        return new_node;
    }
public:
    T data_;
    Node* left_;
    Node* right_;
};

template<class T>
BST<T>::BST(): root_ {nullptr} {}

template<class T>
BST<T>::~BST() {
    destruct(root_);
}

template<class T>
class BST<T>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Node;
    using pointer           = value_type*; 
    using reference         = value_type&;  

    Iterator(pointer node): ptr_ {node} {
        fillStack(node);
    }
    
    T& operator*() const {
        return ptr_->data_;
    }

    pointer operator->() { return ptr_; }

    // Prefix increment
    Iterator& operator++() { next(); return *this; }
    // Postfix increment
    Iterator operator++(T) { Iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr_ == b.ptr_; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr_ != b.ptr_; };

private:
    std::stack<pointer> stack_;
    pointer ptr_;

    void fillStack(Node* node) {
        while(node) {
            stack_.push(node);
            node = node->left_;
        }
    }

    void next() {
        pointer curr {stack_.top()};
        stack_.pop();
        if(curr->right_) {
            fillStack(curr->right_);
        }
        ptr_ = stack_.top();
    }
    
    friend class BST;
};

template<class T>
void BST<T>::deallocNode(Node* node) {
    delete node; 
    node = nullptr;
}

template<class T>
std::pair<typename BST<T>::Iterator, bool> BST<T>::exists(const T& value) const {
    Node* curr {root_};
    
    while(curr) {
        if(curr->data_ == value) {
            return std::pair<Iterator, bool>(Iterator(curr), true); 
        }

        if(curr->data_ < value) {
            curr = curr->left_;
        } else {
            curr = curr->right_;
        }
    }
    return std::pair<Iterator, bool>(Iterator(curr), false);
}

template<class T>
template<class Type>
std::pair<typename BST<T>::Iterator, bool> BST<T>::insertPriv(Type&& value) {
    const auto& pair = exists(value);
    if(pair.second) {
        return std::pair<Iterator, bool>(pair.first, false);
    }
    if(!root_) {
        root_ = BST<T>::getNewNode(value);
    }
}

template<class T>
void BST<T>::destruct(Node* node) {
    if(!node) {
        return;
    }
    if (node->left_) {
        destruct(node->left_);
    }

    if(node->right_) {
        destruct(node->right_);
    }

    delete node;
}


}

#endif // BINARY_SEARCH_TREE_HPP
