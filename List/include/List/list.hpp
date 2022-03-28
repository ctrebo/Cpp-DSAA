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


    // Random access specifiers(Even if access time is terrible)
    reference operator[](const size_type index);
    const_reference operator[](const size_type index) const;
        
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

    template<class Type>
    void append(Type&& value);

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
constexpr typename List<T, Allocator>::size_type List<T, Allocator>::size() const noexcept {
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
    friend class List;
};

}

#endif //LIST_CLASS_HPP
