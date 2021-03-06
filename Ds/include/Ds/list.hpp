#ifndef LIST_CLASS_HPP
#define LIST_CLASS_HPP

#include "concepts.hpp"

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <functional>
#include <algorithm>


namespace ds{


template<class T, class Allocator = std::allocator<T>>
class List {
private:
    class Node;
public:
    class Iterator;
public:
    using value_type = T;
    using allocator_type = Allocator;
    using allocator_type_internal = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
    using traits_t = std::allocator_traits<allocator_type>;
    using traits_t_i = std::allocator_traits<allocator_type_internal>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename traits_t::pointer;
    using const_pointer = typename traits_t::const_pointer;
    using iterator = Iterator;
    using size_type = typename traits_t::size_type;
    using reverse_iterator = std::reverse_iterator<iterator>;

public:
    // Constructors and desctructors
    List();
    explicit List(const Allocator& alloc);
    List(size_type count, const T& value = T(), const Allocator& alloc = Allocator());
    template<class InputIt>
    requires is_it<InputIt>
    List(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    List(const List& other);
    List(const List& other, const Allocator& alloc);
    List(List&& other);
    List(List&& other, const Allocator& alloc);
    List(std::initializer_list<T> iList, const Allocator& alloc = Allocator());
    ~List();

    // Size functions
    constexpr size_type size() const noexcept;
    constexpr bool empty() const noexcept;

    // assign functions
    void assign(size_type count, const T& value);
    template<class InputIt>
    requires is_it<InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> iList);


    // begin, cbegin...
    iterator begin() noexcept; 
    iterator begin() const noexcept; 
    iterator end() noexcept;
    iterator end() const noexcept;

    // Modifiers 
    void push_back(const T& value); 
    void push_back(T&& value); 
    void clear() noexcept;
    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos, T&& value);
    iterator insert( iterator pos, size_type count, const T& value );
    template<class InputIt>
    requires is_it<InputIt>
    iterator insert(iterator pos, InputIt first, InputIt last);
    iterator insert(iterator pos, std::initializer_list<T> iList);
    template<class... Args>
    iterator emplace(iterator pos, Args&&... args);
    template<class... Args>
    reference emplace_back(Args&&... args);
    template<class... Args>
    reference emplace_front(Args&&... args);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void pop_back();
    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();
    void resize(size_type count, const T& value=T());
    void swap(List& other);

    // Operation functions
    void merge(const List& other);
    void merge(List&& other);
    template<class Compare>
    void merge(const List& other, Compare comp);
    template<class Compare>
    void merge(List&& other, Compare comp);
    template<class Compare=std::less<T>>
    void sort(Compare comp=Compare{});
    void unique();
    void reverse() noexcept;
    size_type remove(const T& value);
    template<class UnaryPredicate>
    size_type remove_if(UnaryPredicate p);
    

    // Random access specifiers(Even if access time is terrible)
    reference operator[](const size_type index);
    const_reference operator[](const size_type index) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
        
    //assignment operator
    List<T, Allocator>& operator=(const List& other);
    List<T, Allocator>& operator=(List&& other) noexcept;
    List<T, Allocator>& operator=(std::initializer_list<T> iList);


    // Get allocator
    constexpr allocator_type get_allocator() const noexcept;
private:
    Node* head_;
    Node* tail_;
    allocator_type_internal alloc_;
    size_type size_;

private:
    void destroyAndDealloc();
    void deallocNode(Node* node_to_delete);

    template<class Type>
    void append(Type&& value);
    template<class Type>
    void insertAtFront(Type&& value);

    template<class Type>
    iterator insertAtPos(iterator pos, Type&& value);

    Node* getNewNode(const value_type& value, Node* head=nullptr, Node* tail=nullptr);

};


template<class T, class Allocator>
List<T, Allocator>::List(): size_ {0}, head_ {nullptr}, tail_ {nullptr}  {}

template<class T, class Allocator>
List<T, Allocator>::List(const Allocator& alloc): size_ {0}, alloc_ {alloc}, head_ {nullptr}, tail_ {nullptr} {}

// Set size_ to 0 because it gets increased in the append function
template<class T, class Allocator>
List<T, Allocator>::List(size_type count, const T& value, const Allocator& alloc)
    : size_ {0}
    , alloc_ {alloc}
    , tail_ {nullptr}
    , head_ {nullptr}
{
    for(int i {0}; i < count; ++i) {
        append(value);
    }
}

// Set size_ to 0 because it gets increased in the append function
template<class T, class Allocator>
List<T, Allocator>::List(std::initializer_list<T> iList, const Allocator& alloc)
    : size_{0}
    , alloc_ {alloc}
    , tail_ {nullptr}
    , head_ {nullptr}
{
    for(const auto& value: iList) {
        append(value);
    }
}

template<class T, class Allocator>
template<class InputIt>
requires is_it<InputIt>
List<T, Allocator>::List(InputIt first, InputIt last, const Allocator& alloc)
    : size_{0}
    , alloc_ {alloc}
    , tail_ {nullptr}
    , head_ {nullptr}
{
    for(auto it {first}; it != last; ++it) {
        append(*it);
    }
}

template<class T, class Allocator>
List<T, Allocator>::List(const List& other)
    : size_{0}
    , alloc_ {std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())}
    , tail_ {nullptr}
    , head_ {nullptr}
{
    for(auto it = other.begin(); it != other.end(); ++it) {
        append(*it);
    }
}

template<class T, class Allocator>
List<T, Allocator>::List(const List& other, const Allocator& alloc)
    : size_{0}
    , alloc_ {alloc}
    , tail_ {nullptr}
    , head_ {nullptr}
{
    for(auto it = other.begin(); it != other.end(); ++it) {
        append(*it);
    }
}

template <class T, class Allocator>
List<T, Allocator>::List(List&& other)
    : size_{other.size_}
    , alloc_ {std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())}
    , tail_ {other.tail_}
    , head_ {other.head_}
{
    other.size_ = 0;
    other.head_ = nullptr;
    other.tail_ = nullptr;
}

template <class T, class Allocator>
List<T, Allocator>::List(List&& other, const Allocator& alloc)
    : size_{other.size_}
    , alloc_ {alloc}
    , tail_ {other.tail_}
    , head_ {other.head_}
{
    other.size_ = 0;
    other.head_ = nullptr;
    other.tail_ = nullptr;
}



template<class T, class Allocator>
List<T, Allocator>::~List() {
    destroyAndDealloc();
}

template<class T, class Allocator>
class List<T, Allocator>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Node;
    using pointer           = value_type*; 
    using reference         = value_type&;  
    
    Iterator(pointer ptr) : ptr_{ptr} {}
    Iterator(): ptr_ {nullptr} {}

    T& operator*() const {
        return ptr_->data_; 
    }
    pointer operator->() { return ptr_; }

    // Prefix increment
    Iterator& operator++() { ptr_=ptr_->next_; return *this; }  

    // Postfix increment
    Iterator operator++(T) { Iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr_ == b.ptr_; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr_ != b.ptr_; };

private:
    pointer ptr_;
    friend class List;
};

template<class T, class Allocator>
void List<T, Allocator>::destroyAndDealloc() {
    if(!tail_) return;
    Node* node_to_delete {tail_};
    Node* next_node {};
    while(node_to_delete) {
        next_node = node_to_delete->prev_; 
        traits_t_i::destroy(alloc_, node_to_delete);
        traits_t_i::deallocate(alloc_, node_to_delete, 1);
        node_to_delete = next_node;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
}

template<class T, class Allocator>
void List<T, Allocator>::deallocNode(Node* node_to_delete) {
    traits_t_i::destroy(alloc_, node_to_delete);
    traits_t_i::deallocate(alloc_, node_to_delete, 1);
}

template<class T, class Allocator>
constexpr List<T, Allocator>::size_type List<T, Allocator>::size() const noexcept {
    return size_;
}

template<class T, class Allocator>
constexpr bool List<T, Allocator>::empty() const noexcept {
    return (size_ == 0);
}

template<class T, class Allocator>
void List<T, Allocator>::assign(size_type count, const T& value) {
    destroyAndDealloc();
    for(size_type i {0}; i < count; ++i) {
        append(value);
    }
}

template<class T, class Allocator>
template<class InputIt>
requires is_it<InputIt>
void List<T, Allocator>::assign(InputIt first, InputIt last) {
    destroyAndDealloc();
    for(auto it=first; it != last; ++it) {
        append(*it);
    }
}

template<class T, class Allocator>
void List<T, Allocator>::assign(std::initializer_list<T> iList) {
    destroyAndDealloc();
    for(const auto& value: iList) {
        append(value);
    }
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::begin() noexcept {
    return Iterator(head_);
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::begin() const noexcept {
    return Iterator(head_);
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::end() noexcept {
    return Iterator(nullptr);
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::end() const noexcept {
    return Iterator(nullptr);
}

template<class T, class Allocator>
void List<T, Allocator>::push_back(const T& value) {
    append(value);
}

template<class T, class Allocator>
void List<T, Allocator>::push_back(T&& value) {
    append(std::move(value));
}

template<class T, class Allocator>
void List<T, Allocator>::clear() noexcept {
    destroyAndDealloc();
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::insert(iterator pos, const T& value) {
    return insertAtPos(pos, value);
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::insert(iterator pos, T&& value) {
    return insertAtPos(pos, std::move(value));
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::insert( iterator pos, size_type count, const T& value ) {
    if (count == 0) {
        return pos;
    }
    
    auto return_it = insertAtPos(pos, value);
    --count;
    for(size_type i {0}; i < count; ++i) {
        insertAtPos(pos, value);
    }
    return return_it;
}
template<class T, class Allocator>
template<class InputIt>
requires is_it<InputIt>
List<T, Allocator>::iterator List<T, Allocator>::insert(iterator pos, InputIt first, InputIt last) {
    if (first == last) {
        return pos;
    }
    
    auto return_it = insertAtPos(pos, *first);
    ++first;
    for(auto it = first; it != last; ++it) {
        insertAtPos(pos, *it);
    }
    return return_it;
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::insert(iterator pos, std::initializer_list<T> iList) {
    Iterator it;
    for(int i{0};const auto& value: iList) {
       if (i == 0) {
           it = insertAtPos(pos, value);
       } else {
           insertAtPos(pos, value);
       }
       ++i;
    }
    return it;
}

template<class T, class Allocator>
template<class... Args>
List<T, Allocator>::iterator List<T, Allocator>::emplace(iterator pos, Args&&... args) {
    return insertAtPos(pos, value_type(std::forward<Args>(args)...));
}

template<class T, class Allocator>
template<class... Args>
List<T, Allocator>::reference List<T, Allocator>::emplace_back(Args&&... args) {
    append(value_type(std::forward<Args>(args)...));
    return tail_->data_;
}

template<class T, class Allocator>
template<class... Args>
List<T, Allocator>::reference List<T, Allocator>::emplace_front(Args&&... args) {
    insertAtFront(value_type(std::forward<Args>(args)...));
    return head_->data_;
}



template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::erase(iterator pos) {
    Node* temp {pos.ptr_};
    Node* pointer_to_return {nullptr};
    if((pos==begin()) && (pos == end())) {
        deallocNode(temp);
        --size_;
        head_ = tail_ = nullptr; 

        return iterator(nullptr);
    } else if(!(pos->next_)) {
        tail_ = pos->prev_;     
        tail_->next_ = nullptr;
    } else if(pos == begin()) {
        head_ = pos->next_;
        head_->prev_ = nullptr;
        pointer_to_return = head_;
    } else {
        pos->prev_->next_ = pos->next_;
        pos->next_->prev_ = pos->prev_;
        pointer_to_return = pos->next_;
    }
    deallocNode(temp);
    --size_;
    return iterator(pointer_to_return);
}

template<class T, class Allocator>
List<T, Allocator>::iterator List<T, Allocator>::erase(iterator first, iterator last) {
    Node* pointer_to_return {nullptr};

    size_type num_deleted_nodes = std::distance(first, last);
    size_ -= num_deleted_nodes;

    Node* node_to_delete=first.ptr_;
    Node* next_node = node_to_delete;

    if(first == last) {return last;}

    if((first == begin()) && (last == end())) {
        // If Nodes dont get deallocated here we have a dangling pointer
        while (node_to_delete != last.ptr_){
            next_node = node_to_delete->next_;
            deallocNode(node_to_delete);
            node_to_delete = next_node;
        }
        head_ = tail_ = nullptr;
        return iterator(nullptr);
    } else if(first == begin()) {
        head_ = last.ptr_;
        head_->prev_ = nullptr;
        pointer_to_return = head_;
    } else if(!(last->next_)) {
        tail_ = first->prev_;
        tail_->next_ = nullptr;
    } else {
        first->prev_->next_ = last.ptr_;
        last->prev_ = first->prev_;
        pointer_to_return = last.ptr_;
    }

    while (node_to_delete != last.ptr_){
        next_node = node_to_delete->next_;
        deallocNode(node_to_delete);
        node_to_delete = next_node;
    }
    return iterator(pointer_to_return);
}

template<class T, class Allocator>
void List<T, Allocator>::pop_back() {
    if(!tail_) {return;}
    --size_;
    if(tail_ == head_) {
        deallocNode(tail_);
        head_ = tail_ = nullptr;
        return;
    }
    tail_ = tail_->prev_;
    deallocNode(tail_->next_);
    tail_->next_ = nullptr;
}

template<class T, class Allocator>
void List<T, Allocator>::push_front(const T& value) {
   insertAtFront(value); 
}

template<class T, class Allocator>
void List<T, Allocator>::push_front(T&& value) {
   insertAtFront(std::move(value)); 
}

template<class T, class Allocator>
void List<T, Allocator>::pop_front() {
    if(!head_) {return;}
    --size_;
    if(tail_ == head_) {
        deallocNode(head_);
        head_ = tail_ = nullptr;
        return;
    }

    head_ = head_->next_;
    deallocNode(head_->prev_);
    head_->prev_ = nullptr;
}

template<class T, class Allocator>
void List<T, Allocator>::resize(size_type count, const T& value) {
    if(size_ == count) {return;}
    if(count < size_) {
        Node* temp {tail_};
        Node* prev_temp {temp};
        for(size_type i {size_}; i > count; --i) {
            prev_temp = temp->prev_;
            deallocNode(temp);
            temp = prev_temp;
        }
        tail_ = temp;
        tail_->next_ = nullptr;
        size_ = count; 
    } else {
        for(size_type i {size_}; i < count; ++i) {
            append(value);
        }
    }
}

template<class T, class Allocator>
void List<T, Allocator>::swap(List& other) {
    Node* temp_head {head_};
    Node* temp_tail {tail_};
    size_type temp_size {size_};

    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;

    other.head_ = temp_head;
    other.tail_ = temp_tail;
    other.size_ = temp_size;
}

// Operation functions

template<class T, class Allocator>
void List<T, Allocator>::merge(const List& other) {
    if(&other == this) {
        return;
    }
    
    for(auto it=other.begin(); it != other.end(); ++it) {
       append(*it);
    }
}

template<class T, class Allocator>
void List<T, Allocator>::merge(List&& other) {
    if(&other == this) {
        return;
    }
    
    tail_->next_ = other.head_;
    other.head_->prev_ = tail_;
    tail_ = other.tail_;
    size_ += other.size_;

    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
}

template<class T, class Allocator>
template<class Compare>
void List<T, Allocator>::merge(const List& other, Compare comp) {
    if(&other == this) {
        return;
    }
    
    for(auto it=other.begin(); it != other.end(); ++it) {
       append(*it);
    }

    sort(comp);
     
}

template<class T, class Allocator>
template<class Compare>
void List<T, Allocator>::merge(List&& other, Compare comp) {
    if(&other == this) {
        return;
    }
    
    tail_->next_ = other.head_;
    other.head_->prev_ = tail_;
    tail_ = other.tail_;
    size_ += other.size_;

    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;

    sort(comp);
    
}

template<class T, class Allocator>
template<class Compare>
void List<T, Allocator>::sort(Compare comp) {
    if(size_ <= 1) {
        return;
    }
    Node* curr {head_};
    while(curr->next_) {
        Node* index {curr->next_};
        while(index) {
            if(comp(index->data_, curr->data_)) {
                T tmp { std::move(curr->data_) }; 
                curr->data_ = std::move(index->data_); 
                index->data_ = std::move(tmp); 
            }
            index = index->next_;

        }
        curr = curr->next_;
    }
}

template<class T, class Allocator>
void List<T, Allocator>::unique() {
    ds::List<T> sorted_list {std::move(*this)};
    sorted_list.sort();
}

template<class T, class Allocator>
void List<T, Allocator>::reverse() noexcept {
    Node* temp {nullptr};
    Node* current {head_};

    while (current) { 
        temp = current->prev_; 
        current->prev_ = current->next_; 
        current->next_ = temp;               
        current = current->prev_; 
    }       

    /* Before changing head, check for the cases like empty  
       list and list with only one node */
    if(temp) {
        temp = head_;
        head_ = tail_;
        tail_ = temp;
    }
}

template<class T, class Allocator>
List<T, Allocator>::size_type List<T, Allocator>::remove(const T& value) {
    Node* temp {head_};
    Node* next_node {nullptr};
    size_type deleted_nodes {0};
    while(temp) {
        next_node = temp->next_;
        if(temp->data_ == value) {
            erase(iterator(temp));
            ++deleted_nodes;
        }
        temp = next_node;
    }
    return deleted_nodes;
}

template<class T, class Allocator>
template<class UnaryPredicate>
List<T, Allocator>::size_type List<T, Allocator>::remove_if(UnaryPredicate p) {
    Node* temp {head_};
    Node* next_node {nullptr};
    size_type deleted_nodes {0};
    while(temp) {
        next_node = temp->next_;
        if(p(temp->data_)) {
            erase(iterator(temp));
            ++deleted_nodes;
        }
        temp = next_node;
    }
    return deleted_nodes;
}

template<class T, class Allocator>
List<T, Allocator>::reference List<T, Allocator>::operator[](const size_type index) {
    if(index >= size_/2) {
        Node* temp {tail_};
        for(size_type i {size_- 1}; i != index; --i) {
            temp = temp->prev_;
        }
        return temp->data_;
    } else {
        Node* temp {head_};
        for(size_type i {0}; i != index; ++i) {
            temp = temp->next_;
        }
        return temp->data_;

    }
}

template<class T, class Allocator>
List<T, Allocator>::const_reference List<T, Allocator>::operator[](const size_type index) const {
    if(index >= size_/2) {
        Node* temp {tail_};
        for(size_type i {size_ - 1}; i != index; --i) {
            temp = temp->prev_;
        }
        return temp->data_;
    } else {
        Node* temp {head_};
        for(size_type i {0}; i != index; ++i) {
            temp = temp->next_;
        }
        return temp->data_;

    }
}

template<class T, class Allocator>
List<T, Allocator>::reference List<T, Allocator>::front() {
    return head_->data_;
}

template<class T, class Allocator>
List<T, Allocator>::const_reference List<T, Allocator>::front() const {
    return head_->data_;
}

template<class T, class Allocator>
List<T, Allocator>::reference List<T, Allocator>::back() {
    return tail_->data_;
}

template<class T, class Allocator>
List<T, Allocator>::const_reference List<T, Allocator>::back() const {
    return tail_->data_;
}

template<class T, class Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List& other) {
    alloc_ = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator());
    size_ = other.size_;
    destroyAndDealloc();
    for(auto it=other.begin(); it != other.end(); ++it) {
        append(*it);
    }
    return *this;
}

template<class T, class Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List&& other) noexcept {
    alloc_ = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator());
    destroyAndDealloc();

    size_ = other.size_;
    other.size_ = 0;

    head_ = other.head_;
    tail_ = other.tail_;
    other.head_ = nullptr;
    other.tail_ = nullptr;

    return *this;
}

template<class T, class Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(std::initializer_list<T> iList) {
    destroyAndDealloc();
    for(const auto& value: iList) {
        append(value);
    }
    return *this;
}

template<class T, class Allocator>
template<class Type>
void List<T, Allocator>::append(Type&& value) {
    Node* new_node {getNewNode(std::forward<Type>(value))};
    ++size_;
    if(!tail_) {
        tail_ = head_ = new_node;
        return;
    }
    tail_->next_ = new_node;
    new_node->prev_ = tail_;
    tail_ = new_node;
}

template<class T, class Allocator>
template<class Type>
void List<T, Allocator>::insertAtFront(Type&& value) {
    Node* new_node {getNewNode(std::forward<Type>(value))};
    ++size_;
    if(!head_) {
        tail_ = head_ = new_node;
        return;
    }
    new_node->next_ = head_; 
    head_->prev_ = new_node;
    head_ = new_node;
}

template<class T, class Allocator>
template<class Type>
List<T, Allocator>::iterator List<T, Allocator>::insertAtPos(iterator pos, Type&& value) {
    Node* new_node {getNewNode(std::forward<Type>(value))};
    ++size_;
    if((pos == end()) && (pos == begin())) {
        tail_ = head_ = new_node;
    } else if(pos == begin()) {
        head_->prev_ = new_node;
        new_node->next_ = head_;
        head_ = new_node;
    } else if(pos == end()) {
        tail_->next_ = new_node;
        new_node->prev_ = tail_;
        tail_ = new_node;
    } else {
        Node* temp {pos.ptr_};
        temp->prev_->next_ = new_node;
        new_node->prev_ = temp->prev_;
        new_node->next_ = temp;
        temp->prev_ = new_node;
    }
    return Iterator(new_node);
}


template<class T, class Allocator>
constexpr List<T, Allocator>::allocator_type List<T, Allocator>::get_allocator() const noexcept {
    return alloc_;
}

template<class T, class Allocator>
class List<T, Allocator>::Node {
public:
    value_type data_;
    Node* next_;
    Node* prev_;

    Node(const value_type& data, Node* next, Node* prev): data_ {data}, next_ {next}, prev_ {prev}
    {}

    Node( value_type&& data, Node* next, Node* prev): data_ {std::move(data)}, next_ {next}, prev_ {prev}
    {}
    
};

template<class T, class Allocator>
List<T, Allocator>::Node* List<T, Allocator>::getNewNode(const value_type& value, Node* head, Node* tail) {
        Node* new_node {traits_t_i::allocate(alloc_, 1)};
        traits_t_i::construct(alloc_, new_node, value, head, tail);
        return new_node;
    }


}

#endif //LIST_CLASS_HPP
