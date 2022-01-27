#ifndef VECTORCLASS_HPP
#define VECTORCLASS_HPP

#include <iostream>
#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <exception>
#include <utility>

template<class T>
class TD;

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
    using traits_t = std::allocator_traits<allocator>;
private:
    size_type capacity_ {};
    size_type size_ {};
    allocator alloc_ {};
    pointer array_ {};

public:
    // Constructors and Desctructor
    VectorClass(size_type size);
    VectorClass(size_type size, const T& value);
    VectorClass(const VectorClass& other);
    VectorClass(VectorClass&& other);
    VectorClass(std::initializer_list<T> l);
    VectorClass() = default;
    ~VectorClass(); 

    // Overloading assignment operator
    VectorClass<T>& operator=(const VectorClass& other);
    VectorClass<T>& operator=(VectorClass&& other) noexcept;
    VectorClass<T>& operator=(std::initializer_list<T> l);

    // Assign
    void assign(size_type count, const T& value);
    void assign(iterator first, iterator last);
    void assign(std::initializer_list<T> l);

    // Length and capacity
    constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type capacity() const noexcept;
    void reserve(size_type new_cap);

    constexpr void push_back(const T& value);
    // T&& is r-value reference because no type deduction takes place
    constexpr void push_back(T&& value);

    // Pointers to first and last element
    constexpr iterator begin() noexcept;
    constexpr const_pointer begin() const noexcept;
    constexpr const_pointer cbegin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_pointer end() const noexcept;
    constexpr const_pointer cend() const noexcept;
    
    // Access Array
    pointer data() noexcept;

    // Access array
    reference operator[] (const size_type index);
    const_reference operator[] (const size_type index) const;
    reference at(const size_type index);
    const_reference at(const size_type index) const;
};

template<class T,class ... Args>
decltype(auto) makeVector(T&& t, Args&&... args) {
    return VectorClass<T>{ std::forward<T>(t), std::forward<Args>(args)... };
}

template<class T>
VectorClass<T>::VectorClass(size_type size): capacity_ {size}, size_ {size}, array_ { traits_t::allocate(alloc_, capacity_) } {
    for(size_type i {0}; i < size_; ++i) {
        array_[i] = T(); 
    }
}

template<class T>
VectorClass<T>::VectorClass(size_type size, const T& value): capacity_ {size}, size_ {size}, array_ { traits_t::allocate(alloc_, capacity_) } {
    for(size_type i {0}; i < size_; ++i) {
        array_[i] = value; 
    }
}

template<class T>
VectorClass<T>::VectorClass(const VectorClass& other): size_ {other.size_}, capacity_{other.capacity_}, array_ {traits_t::allocate(alloc_, capacity_)} {
    for(size_type i {0}; i < size_; ++i) {
        array_[i] = other[i];
    }
}

template<class T>
VectorClass<T>::VectorClass(VectorClass&& other): size_ { other.size_ }, capacity_ {other.capacity_}, array_ {other.array_} {
    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<class T>
VectorClass<T>::~VectorClass() {
    traits_t::deallocate(alloc_, array_ , capacity_);
}

template<class T>
VectorClass<T>::VectorClass(std::initializer_list<T> l): capacity_ {l.size() * 2}, size_ { l.size()}, array_ { traits_t::allocate(alloc_, capacity_)} {
    size_type counter {0};
    for(auto& element: l) {
        array_[counter] = element;
        ++counter;
    }
}

template<class T>
VectorClass<T>& VectorClass<T>::operator=(const VectorClass& other) {
    if (&other == this)
        return *this;

    traits_t::deallocate(alloc_, array_ , capacity_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    array_ = traits_t::allocate(alloc_, capacity_);

    for(size_type i {0}; i <size_; ++i) {
        array_[i] = other[i];
    }
    return *this;
}

template<class T>
VectorClass<T>& VectorClass<T>::operator=(VectorClass&& other) noexcept {
    if (&other == this)
        return *this;

    traits_t::deallocate(alloc_, array_ , capacity_);
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
    array_ = other.array_;
    other.array_ = nullptr;

    return *this;
}

template<class T>
VectorClass<T>& VectorClass<T>::operator=(std::initializer_list<T> l) {
    traits_t::deallocate(alloc_, array_ , capacity_);
    size_ = l.size();
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);

    size_type counter {0};
    for(auto& element: l) {
        array_[counter] = element;
        ++counter;
    }
    return *this;
}


template<class T>
void VectorClass<T>::assign(size_type count, const T& value) {
    traits_t::deallocate(alloc_, array_ , capacity_);
    size_ = count;
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);
    for(size_type i {0}; i < size_; ++i) {
        array_[i] = value;
    }
}

template<class T>
void VectorClass<T>::assign(iterator first, iterator last) {
    
}

template<class T>
void VectorClass<T>::assign(std::initializer_list<T> l) {
    traits_t::deallocate(alloc_, array_ , capacity_);
    size_ = l.size();
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);
    size_type counter {0};
    for(auto& element: l) {
        array_[counter] = element;
        ++counter;
    }
}

template<class T>
constexpr bool VectorClass<T>::empty() const noexcept {
    return (size_ == 0);
}

template<class T>
constexpr VectorClass<T>::size_type VectorClass<T>::size() const noexcept {
    return size_;
}

template<class T>
void VectorClass<T>::reserve(size_type new_cap) {
    if(new_cap <= capacity_) return;
    pointer new_arr  {traits_t::allocate(alloc_, new_cap)};
    for (size_type i {0}; i < size_; ++i) {
        new_arr[i] = array_[i]; 
    }
    traits_t::deallocate(alloc_, array_, capacity_);
    capacity_ = new_cap;
    array_ = new_arr;
    new_arr = nullptr;
}

template<class T>
constexpr VectorClass<T>::size_type VectorClass<T>::capacity() const noexcept {
    return capacity_;
}

template<class T>
constexpr void VectorClass<T>::push_back(const T& value) {
    if((size_ + 1) > capacity_) {
        size_type new_cap {capacity_ == 0 ? 1 : capacity_ * 2};
        pointer new_arr  {traits_t::allocate(alloc_, new_cap)};
        for (size_type i {0}; i < size_; ++i) {
            new_arr[i] = array_[i]; 
        }
        traits_t::deallocate(alloc_, array_, capacity_);
        capacity_ = new_cap;
        array_ = new_arr;
        new_arr = nullptr;
    }
    array_[size_++] = value;
}

template<class T>
constexpr void VectorClass<T>::push_back(T&& value) {
    if((size_ + 1) > capacity_) {
        size_type new_cap {capacity_ == 0 ? 1 : capacity_ * 2};
        pointer new_arr  {traits_t::allocate(alloc_, new_cap)};
        for (size_type i {0}; i < size_; ++i) {
            new_arr[i] = array_[i]; 
        }
        traits_t::deallocate(alloc_, array_, capacity_);
        capacity_ = new_cap;
        array_ = new_arr;
        new_arr = nullptr;
    }
    // Increment after inserting because next free element is
    // size_ not ++size_
    array_[size_++] = std::move(value);
}

template<class T>
constexpr VectorClass<T>::iterator VectorClass<T>::begin() noexcept {
    return array_;
}

template<class T>
constexpr VectorClass<T>::const_pointer VectorClass<T>::begin() const noexcept {
    return array_;
}

template<class T>
constexpr VectorClass<T>::const_pointer VectorClass<T>::cbegin() const noexcept {
    return array_;
}

template<class T>
constexpr VectorClass<T>::iterator VectorClass<T>::end() noexcept {
    return &(array_[size_]);
}

template<class T>
constexpr VectorClass<T>::const_pointer VectorClass<T>::end() const noexcept {
    return &(array_[size_]);
}

template<class T>
constexpr VectorClass<T>::const_pointer VectorClass<T>::cend() const noexcept {
    return &(array_[size_]);
}

template<class T>
VectorClass<T>::pointer VectorClass<T>::data() noexcept {
    return array_;
}

template<class T>
VectorClass<T>::reference VectorClass<T>::operator[] (const size_type index) {
    return array_[index];
}

template<class T>
VectorClass<T>::const_reference VectorClass<T>::operator[] (const size_type index) const {
    return array_[index];
}

template<class T>
VectorClass<T>::reference VectorClass<T>::at(const size_type index) {
    if(!(index < size_))
        throw std::out_of_range("Index out of range");
    return array_[index];
}

template<class T>
VectorClass<T>::const_reference VectorClass<T>::at(const size_type index) const {
    if(!(index < size_))
        throw std::out_of_range("Index out of range");
    return array_[index];
}

#endif // VECTORCLASS_HPP
