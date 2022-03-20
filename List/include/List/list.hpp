#ifndef LIST_CLASS_HPP
#define LIST_CLASS_HPP

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iostream>
#include <type_traits>


namespace ds{

template<class T>
concept is_it = std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category >;

template<class T, class Allocator = std::allocator<T>>
class List {
class Node;
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
    using iterator = typename traits_t::pointer;
    using const_iterator = typename traits_t::const_pointer;
    using size_type = typename traits_t::size_type;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    // Constructors and desctructors
    List();
    explicit List(const Allocator& alloc);
    List(size_type count, const T& value = T(), const Allocator& alloc = Allocator());
    List(std::initializer_list<T> iList, const Allocator& alloc = Allocator());
    template<class InputIt>
    requires is_it<InputIt>
    List(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    ~List();

    // Size functions
    constexpr size_type size() const noexcept;

    // begin, cbegin...
    Iterator begin() { return Iterator(head_);}
    Iterator end() { return nullptr;}

    // Inserts elements
    void push_back(const T& value); 
    void push_back(T&& value); 

    // Random access specifiers(Even if access time is terrible)
    reference operator[](const size_type index);
    const_reference operator[](const size_type index) const;
        
private:
    Node* head_;
    Node* tail_;
    allocator_type_internal alloc_;
    size_type size_;

private:
    void destroyAndDealloc();

    template<class Type>
    void append(Type&& type);

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

}

template<class T, class Allocator>
List<T, Allocator>::~List() {
    destroyAndDealloc();
}

template<class T, class Allocator>
constexpr typename List<T, Allocator>::size_type List<T, Allocator>::size() const noexcept {
    return size_;
}

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
void List<T, Allocator>::push_back(const T& value) {
    append(value);
}

template<class T, class Allocator>
void List<T, Allocator>::push_back(T&& value) {
    append(std::move(value));
}

template<class T, class Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::operator[](const size_type index) {
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
typename List<T, Allocator>::const_reference List<T, Allocator>::operator[](const size_type index) const {
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
template<class Type>
void List<T, Allocator>::append(Type&& type) {
    Node* new_node {getNewNode(std::forward<Type>(type))};
    ++size_;
    if(!tail_) {
        tail_ = head_ = new_node;
        return;
    }
    tail_->next_ = new_node;
    tail_ = new_node;
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

template<class T, class Allocator>
class List<T, Allocator>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Node;
    using pointer           = value_type*; 
    using reference         = value_type&;  
    
    Iterator(pointer ptr) : ptr_{ptr} {}

    T& operator*() const { return ptr_->data_; }
    pointer operator->() { return ptr_; }

    // Prefix increment
    Iterator& operator++() { ptr_=ptr_->next_; return *this; }  

    // Postfix increment
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr_ == b.ptr_; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr_ != b.ptr_; };

private:
    pointer ptr_;
};

}

#endif //LIST_CLASS_HPP
