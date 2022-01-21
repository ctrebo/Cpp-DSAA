#ifndef VECTORCLASS_HPP
#define VECTORCLASS_HPP

#include <iostream>
#include <cstddef>
#include <iterator>

template<class T>
class VectorClass {
// Type aliases
public:
    using value_type = T;
    using allocator = std::allocator<T>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type =  std::size_t;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    allocator alloc_ {};
    pointer array_ {};
public:
   explicit VectorClass(size_type size);


};

template<class T>
VectorClass<T>::VectorClass(size_type size) {
    std::cout << "hi";
}

#endif // VECTORCLASS_HPP
