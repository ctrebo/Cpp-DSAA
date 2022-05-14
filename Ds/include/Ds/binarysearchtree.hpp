#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include "concepts.hpp"

#include <cstddef>
#include <iostream>
#include <stack>
#include <utility>
#include <initializer_list>
#include <algorithm>

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
    BST(std::initializer_list<T> iList);
    BST(const BST<T>& other);
    BST(BST<T>&& other) noexcept;
    template<class InputIt>
    requires is_it<InputIt>
    BST(InputIt first, InputIt last);
    ~BST();

    constexpr size_type size() const;
    constexpr bool empty() const;

    constexpr BST<T>::Iterator begin() const noexcept;
    constexpr BST<T>::Iterator end() const noexcept;

    std::pair<BST<T>::Iterator, bool> insert(const T& value);
    std::pair<BST<T>::Iterator, bool> insert(T&& value);
    void insert(std::initializer_list<T> iList);
    void clear();
    template<class... Args>
    std::pair<BST<T>::Iterator, bool> emplace(Args&&... args);
    
    void postOrder();
    void inOrder();
    void preOrder();

    // Overloaded '=' operator
    BST<T> operator=(BST<T> other) noexcept;

    friend void swap<T>(BST<T>& first, BST<T>& second) noexcept;
    
private:
    node_pointer root_;
    size_type size_;
private:

    void deallocNode(Node* node);
    // Returns boolean that tells if insertion was successful
    std::pair<BST<T>::Iterator, bool> exists(const T& value) const;
    template<class Type>
    std::pair<BST<T>::Iterator, bool> insertPriv(Type&& value);
    void destruct(Node* node);
    void postOrderRec(Node* node);
    void inOrderRec(Node* node);
    void preOrderRec(Node* node);
};

template<class T>
class BST<T>::Node {
public:
    Node(const T& data, Node* left=nullptr, Node* right=nullptr): data_{data}, left_ {left}, right_ {right}
    {}

    Node(T&& data, Node* left=nullptr, Node* right=nullptr): data_{std::move(data)}, left_ {left}, right_ {right}
    {}

    static Node* getNewNode(T value, Node* left=nullptr,Node* right=nullptr) {
        Node* new_node {new Node {value, left, right}};     
        return new_node;
    }
private:
    T data_;
    Node* left_;
    Node* right_;
    
    friend class BST;
};

template<class T>
BST<T>::BST(): root_ {nullptr}, size_ {0} {}

template<class T>
BST<T>::BST(std::initializer_list<T> iList): BST() {
    for(const auto& elem: iList) {
        if((exists(elem)).second) {
            continue;
        }
        insertPriv(elem);
    }
}

template<class T>
BST<T>::BST(const BST<T>& other): BST() {
    for(const auto& elem: other) {
        insertPriv(elem);
    }
}

template<class T>
BST<T>::BST(BST<T>&& other) noexcept: BST() {
    swap(*this, other);
}

template<class T>
template<class InputIt>
requires is_it<InputIt>
BST<T>::BST(InputIt first, InputIt last) {
    for(auto it=first; it != last; ++it) {
        if(exists(*it).second) {
            continue;
        }
        insertPriv(*it);
    }
}

template<class T>
BST<T>::~BST() {
    destruct(root_);
    root_ = nullptr;
}

template<class T>
class BST<T>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Node;
    using pointer           = value_type*; 
    using reference         = value_type&;  

    Iterator(pointer node) {
        fillStack(node);
        next();
    }
    
    T& operator*() const {
        return ptr_->data_;
    }

    pointer operator->() { return ptr_; }

    // Prefix increment
    Iterator& operator++() {
        next();
        return *this;
    }
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
        if(!stack_.empty()) {
            pointer curr {stack_.top()};
            stack_.pop();
            if(curr->right_) {
                fillStack(curr->right_);
            }
            ptr_ = curr;
            return;
        }
        ptr_ = nullptr;
    }

    
    friend class BST;
};

template<class T>
constexpr BST<T>::size_type BST<T>::size() const {
    return size_;
}

template<class T>
constexpr bool BST<T>::empty() const {
    return size_==0;
}

template<class T>
constexpr BST<T>::Iterator BST<T>::begin() const noexcept {
    return Iterator(root_);
}

template<class T>
constexpr BST<T>::Iterator BST<T>::end() const noexcept {
    return Iterator(nullptr);
}

template<class T>
std::pair<typename BST<T>::Iterator, bool> BST<T>::insert(const T& value) {
    return insertPriv(value);
}

template<class T>
std::pair<typename BST<T>::Iterator, bool> BST<T>::insert(T&& value) {
    return insertPriv(std::move(value));
}

template<class T>
void BST<T>::insert(std::initializer_list<T> iList) {
    for(const auto& elem: iList) {
        if((exists(elem)).second) {
            continue;
        }
        insertPriv(elem);
    }
}

template<class T>
void BST<T>::clear() {
    destruct(root_);
    root_ = nullptr;
}

template<class T>
template<class... Args>
std::pair<typename BST<T>::Iterator, bool> BST<T>::emplace(Args&&... args) {
    return insertPriv(value_type(args...));
}

template<class T>
void BST<T>::deallocNode(Node* node) {
    delete node; 
    node = nullptr;
}

template<class T>
void BST<T>::postOrder() {
    postOrder(root_);
}

template<class T>
void BST<T>::inOrder() {
    inOrderRec(root_);
}

template<class T>
void BST<T>::preOrder() {
    preOrderRec(root_);
}

template<class T>
BST<T> BST<T>::operator=(BST<T> other) noexcept {
    swap(*this, other);

    return *this;
}

template<class T>
std::pair<typename BST<T>::Iterator, bool> BST<T>::exists(const T& value) const {
    Node* curr {root_};
    
    while(curr) {
        if(curr->data_ == value) {
            return std::pair<Iterator, bool>(Iterator(curr), true); 
        }
        if(value < curr->data_) {
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
    Node* new_node {Node::getNewNode(std::forward<Type>(value))};
    if(!root_) {
        root_ = new_node;
    } else {
        Node* temp {root_};
        Node* last_node {root_};
        while(temp) {
            last_node = temp;
            if(value > temp->data_) {
                temp = temp->right_;
            } else {
                temp = temp->left_;
            }
        }
        if(value > last_node->data_) {
            last_node->right_ = new_node;
        } else {
            last_node->left_ = new_node;
        }
    }
    ++size_;
    return std::pair<Iterator, bool>(Iterator(new_node), true);
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

    --size_;
    delete node;
    node->left_ = nullptr;
    node->right_ = nullptr;
    node = nullptr;
}

template<class T>
void BST<T>::postOrderRec(Node* node) {
    if (!node) {
        return;
    }

    postOrderRec(node->left_);
    postOrderRec(node->right_);
    
    std::cout << node->data_ << ' ';
}

template<class T>
void BST<T>::inOrderRec(Node* node) {
    if (!node) {
        return;
    }

    inOrderRec(node->left_);

    std::cout << node->data_ << ' ';

    inOrderRec(node->right_);
    
}

template<class T>
void BST<T>::preOrderRec(Node* node) {
    if (!node) {
        return;
    }

    std::cout << node->data_ << ' ';

    preOrderRec(node->left_);
    preOrderRec(node->right_);
    
}

template<class T>
void swap(BST<T>& first, BST<T>& second) noexcept {
    using std::swap;
    swap(first.size_, second.size_);
    swap(first.root_, second.root_);
}

}

#endif // BINARY_SEARCH_TREE_HPP
