#ifndef LIST_CLASS_HPP
#define LIST_CLASS_HPP

#include <cstddef>
#include <memory>

namespace ds{

template<class T, class Allocator = std::allocator<T>>
class List {
class Node;
public:
    using value_type = T;
    using allocator_type = Allocator;
    using allocator_type_internal = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
    using traits_t = std::allocator_traits<allocator_type>;
    using traits_t_i = std::allocator_traits<allocator_type_internal>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = traits_t::pointer;
    using const_pointer = traits_t::const_pointer;
    using iterator = traits_t::pointer;
    using const_iterator = traits_t::const_pointer;
    using size_type = traits_t::size_type;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    class Node {
    public:
        value_type data_;
        Node* next_;
        Node* prev_;

        Node(const value_type& data, Node* next, Node* prev): data_ {data}, next_ {next}, prev_ {prev}
        {}

        Node(const value_type&& data, Node* next, Node* prev): data_ {std::move(data)}, next_ {next}, prev_ {prev}
        {}
    };

    Node* getNewNode(const value_type& value, const Node* head=nullptr, const Node* tail=nullptr) {
        Node* new_node {allocator_type_internal::allocate(alloc_, 1)};
        traits_t_i::construct(alloc_, new_node, value, head, tail);
        return new_node;
    }

public:
    List();
    ~List();
private:
    Node* head_;
    Node* tail_;
    allocator_type_internal alloc_;
    size_type size_;

private:
    void destroyAndDealloc();

    template<class Type>
    void append(Type&& type);

};

template<class T, class Allocator>
List<T, Allocator>::List(): size_ {0}, head_ {nullptr}, tail_ {nullptr}  {}

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

    head_ = nullptr;
    tail_ = nullptr;
}


template<class T, class Allocator>
template<class Type>
void List<T, Allocator>::append(Type&& type) {
    Node* new_node {getNewNode(std::forward<Type>(type))};
    if(!tail_) {
        tail_ = head_ =new_node;
    }
}

}

#endif //LIST_CLASS_HPP
