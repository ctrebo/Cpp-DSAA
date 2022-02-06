#ifndef VECTORCLASS_HPP
#define VECTORCLASS_HPP

#include <iostream>
#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <exception>
#include <utility>
#include <memory_resource>

template<class T>
class TD;

template<class T, class Allocator = std::allocator<T>>
class VectorClass {
// Type aliases
public:
    using value_type = T;
    using allocator = Allocator;
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

    void destroyAndDealloc();

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
    VectorClass<T, Allocator>& operator=(const VectorClass& other);
    VectorClass<T, Allocator>& operator=(VectorClass&& other) noexcept;
    VectorClass<T, Allocator>& operator=(std::initializer_list<T> l);

    // Assign
    void assign(size_type count, const T& value);
    void assign(iterator first, iterator last);
    void assign(std::initializer_list<T> l);

    // Length and capacity
    constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type capacity() const noexcept;
    void reserve(size_type new_cap);

    // Modifiers
    constexpr void push_back(const T& value);
    // T&& is r-value reference because no type deduction takes place
    constexpr void push_back(T&& value);
    void clear();
    void insert(const_iterator pos, const T& value);
    void insert(const_iterator pos, T&& value);
    void insert(const_iterator pos, size_type count, const T& value);
    void insert(const_iterator pos, iterator first, iterator last);
    void insert( const_iterator pos, std::initializer_list<T> ilist );


    // Pointers to first and last element
    constexpr iterator begin() noexcept;
    constexpr const_pointer begin() const noexcept;
    constexpr const_pointer cbegin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_pointer end() const noexcept;
    constexpr const_pointer cend() const noexcept;

    // Element access
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;
    pointer data() noexcept;
    reference operator[] (const size_type index);
    const_reference operator[] (const size_type index) const;
    reference at(const size_type index);
    const_reference at(const size_type index) const;
};

template<class T,class ... Args>
decltype(auto) makeVector(T&& t, Args&&... args) {
    return VectorClass<T>{ std::forward<T>(t), std::forward<Args>(args)... };
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::destroyAndDealloc() {
    iterator it{ &(array_[capacity_]) };
    while (it != array_) {
        traits_t::destroy(alloc_, --it);
    }
    traits_t::deallocate(alloc_,array_ , capacity_);
    array_ = nullptr;
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(size_type size): capacity_ {size}, size_ {size}, array_ { traits_t::allocate(alloc_, capacity_) } {
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, T());
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(size_type size, const T& value): capacity_ {size}, size_ {size}, array_ { traits_t::allocate(alloc_, capacity_) } {
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, value);
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(const VectorClass& other): size_ {other.size_}, capacity_{other.capacity_}, array_ {traits_t::allocate(alloc_, capacity_)} {
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, other[i]);
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(VectorClass&& other): size_ { other.size_ }, capacity_ {other.capacity_}, array_ {other.array_} {
    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<class T, class Allocator>
VectorClass<T, Allocator>::~VectorClass() {
    destroyAndDealloc();
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(std::initializer_list<T> l): capacity_ {l.size() * 2}, size_ { l.size()}, array_ { traits_t::allocate(alloc_, capacity_)} {
    size_type counter {0};
    for(auto& element: l) {
        traits_t::construct(alloc_, array_ + counter, element);
        ++counter;
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>& VectorClass<T, Allocator>::operator=(const VectorClass& other) {
    if (&other == this)
        return *this;

    destroyAndDealloc();
    size_ = other.size_;
    capacity_ = other.capacity_;
    array_ = traits_t::allocate(alloc_, capacity_);

    for(size_type i {0}; i <size_; ++i) {
        traits_t::construct(alloc_, array_ + i, other[i]);
    }
    return *this;
}

template<class T, class Allocator>
VectorClass<T, Allocator>& VectorClass<T, Allocator>::operator=(VectorClass&& other) noexcept {
    if (&other == this)
        return *this;

    destroyAndDealloc();
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
    array_ = other.array_;
    other.array_ = nullptr;

    return *this;
}

template<class T, class Allocator>
VectorClass<T, Allocator>& VectorClass<T, Allocator>::operator=(std::initializer_list<T> l) {
    destroyAndDealloc();
    size_ = l.size();
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);

    size_type counter {0};
    for(auto& element: l) {
        traits_t::construct(alloc_, array_ + counter, element);
        ++counter;
    }
    return *this;
}


template<class T, class Allocator>
void VectorClass<T, Allocator>::assign(size_type count, const T& value) {
    destroyAndDealloc();
    size_ = count;
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, value);
    }
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::assign(iterator first, iterator last) {
    destroyAndDealloc();
    size_ = std::distance(first, last);
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);
    size_type counter {0};
    for(auto i {first}; i != last; ++i) {
        traits_t::construct(alloc_, array_ + counter, *i);
        ++counter;
    }
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::assign(std::initializer_list<T> l) {
    destroyAndDealloc();
    size_ = l.size();
    capacity_ = size_ * 2;
    array_ = traits_t::allocate(alloc_, capacity_);
    size_type counter {0};
    for(auto& element: l) {
        traits_t::construct(alloc_, array_ + counter, element);
        ++counter;
    }
}

template<class T, class Allocator>
constexpr bool VectorClass<T, Allocator>::empty() const noexcept {
    return (size_ == 0);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::size_type VectorClass<T, Allocator>::size() const noexcept {
    return size_;
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::reserve(size_type new_cap) {
    if(new_cap <= capacity_) return;
    pointer new_arr  {traits_t::allocate(alloc_, new_cap)};
    for (size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, new_arr + i, array_[i]);
    }
    destroyAndDealloc();
    capacity_ = new_cap;
    array_ = new_arr;
    new_arr = nullptr;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::size_type VectorClass<T, Allocator>::capacity() const noexcept {
    return capacity_;
}

template<class T, class Allocator>
constexpr void VectorClass<T, Allocator>::push_back(const T& value) {
    if((size_ + 1) > capacity_) {
        size_type new_cap {capacity_ == 0 ? 1 : capacity_ * 2};
        pointer new_arr {traits_t::allocate(alloc_, new_cap)};
        for (size_type i {0}; i < size_; ++i) {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[i]));
        }
        destroyAndDealloc();
        capacity_ = new_cap;
        array_ = new_arr;
        new_arr = nullptr;
    }
    traits_t::construct(alloc_, array_ + (size_++), value);
}

template<class T, class Allocator>
constexpr void VectorClass<T, Allocator>::push_back(T&& value) {
    if((size_ + 1) > capacity_) {
        size_type new_cap {capacity_ == 0 ? 1 : capacity_ * 2};
        pointer new_arr {traits_t::allocate(alloc_, new_cap)};
        for (size_type i {0}; i < size_; ++i) {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[i]));
        }
        destroyAndDealloc();
        capacity_ = new_cap;
        array_ = new_arr;
        new_arr = nullptr;
    }
    // Increment after inserting because next free element is
    // size_ not ++size_
    traits_t::construct(alloc_, array_ + (size_++), value);
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::clear() {
    destroyAndDealloc();
    array_ = traits_t::allocate(alloc_, capacity_);
    size_ = 0;
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::insert(const_iterator pos,const T& value) {
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    size_type new_capacity {(size_ + 1>capacity_)?capacity_: capacity_* 2, capacity_};
    pointer new_arr {traits_t::allocate(alloc_, new_capacity)};
    ++size_;
    size_type start = std::distance(cbegin(), pos);
    size_type j {0};
    for(size_type i {0}; i < size_; ++i) {
        if(i == start) {
            traits_t::construct(alloc_, new_arr + i, value);
        } else {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[j]));
            ++j;
        }
    }
    destroyAndDealloc();
    capacity_ = new_capacity;
    array_ = new_arr;
    new_arr = nullptr;
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::insert(const_iterator pos,T&& value) {
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    size_type new_capacity {(size_ + 1> capacity_)?capacity_ * 2: capacity_};
    pointer new_arr {traits_t::allocate(alloc_, new_capacity)};
    ++size_;
    size_type start = std::distance(cbegin(), pos);
    size_type j {0};
    for(size_type i {0}; i < size_; ++i) {
        if(i == start) {
            traits_t::construct(alloc_, new_arr + i, std::move(value));
        } else {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[j]));
            ++j;
        }
    }
    destroyAndDealloc();
    capacity_ = new_capacity;
    array_ = new_arr;
    new_arr = nullptr;

}

template<class T, class Allocator>
void VectorClass<T, Allocator>::insert(const_iterator pos, size_type count, const T& value) {
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    size_type new_capacity {(size_ + count > capacity_)?(capacity_ +count) * 2: capacity_};
    pointer new_arr {traits_t::allocate(alloc_, new_capacity)};
    size_ += count;
    size_type start = std::distance(cbegin(), pos);
    size_type j {0};
    for(size_type i {0}; i < size_; ++i) {
        if(i >= start && i < start + count) {
            traits_t::construct(alloc_, new_arr + i, value);
        } else {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[j]));
            ++j;
        }
    }
    destroyAndDealloc();
    capacity_ = new_capacity;
    array_ = new_arr;
    new_arr = nullptr;
}
template<class T, class Allocator>
void VectorClass<T, Allocator>::insert(const_iterator pos, iterator first, iterator last) {
    const size_type count = std::distance(first, last);
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    size_type new_capacity {(size_ + count > capacity_)?(capacity_ +count) * 2: capacity_};
    pointer new_arr {traits_t::allocate(alloc_, new_capacity)};
    size_ += count;
    size_type start = std::distance(cbegin(), pos);
    size_type j {0};
    iterator iter {first};
    for(size_type i {0}; i < size_; ++i) {
        if(i >= start && i < start + count) {
            traits_t::construct(alloc_, new_arr + i, (*iter));
            ++iter;
        } else {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[j]));
            ++j;
        }
    }
    destroyAndDealloc();
    capacity_ = new_capacity;
    array_ = new_arr;
    new_arr = nullptr;
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> iList) {
    const size_type count {iList.size()};
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    size_type new_capacity {(size_ + count > capacity_)?(capacity_ +count) * 2: capacity_};
    pointer new_arr {traits_t::allocate(alloc_, new_capacity)};
    size_ += count;
    size_type start = std::distance(cbegin(), pos);
    size_type j {0};
    for(size_type i {0}; i < size_; ++i) {
        if(i == start) {
            for(auto& element: iList) {
                traits_t::construct(alloc_, new_arr + i, element);
                if (i != +count - 1) {
                    ++i;
                }
            }
        } else {
            traits_t::construct(alloc_, new_arr + i, std::move(array_[j]));
            ++j;
        }
    }
    destroyAndDealloc();
    capacity_ = new_capacity;
    array_ = new_arr;
    new_arr = nullptr;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::iterator VectorClass<T, Allocator>::begin() noexcept {
    return array_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_pointer VectorClass<T, Allocator>::begin() const noexcept {
    return array_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_pointer VectorClass<T, Allocator>::cbegin() const noexcept {
    return array_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::iterator VectorClass<T, Allocator>::end() noexcept {
    return &(array_[size_]);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_pointer VectorClass<T, Allocator>::end() const noexcept {
    return &(array_[size_]);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_pointer VectorClass<T, Allocator>::cend() const noexcept {
    return &(array_[size_]);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::reference VectorClass<T, Allocator>::front() {
    return array_[0];
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_reference VectorClass<T, Allocator>::front() const {
    return array_[0];
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::reference VectorClass<T, Allocator>::back() {
    return array_[size_ - 1];
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_reference VectorClass<T, Allocator>::back() const {
    return array_[size_ - 1];
}

template<class T, class Allocator>
VectorClass<T, Allocator>::pointer VectorClass<T, Allocator>::data() noexcept {
    return array_;
}

template<class T, class Allocator>
VectorClass<T, Allocator>::reference VectorClass<T, Allocator>::operator[] (const size_type index) {
    return array_[index];
}

template<class T, class Allocator>
VectorClass<T, Allocator>::const_reference VectorClass<T, Allocator>::operator[] (const size_type index) const {
    return array_[index];
}

template<class T, class Allocator>
VectorClass<T, Allocator>::reference VectorClass<T, Allocator>::at(const size_type index) {
    if(!(index < size_))
        throw std::out_of_range("Index out of range");
    return array_[index];
}

template<class T, class Allocator>
VectorClass<T, Allocator>::const_reference VectorClass<T, Allocator>::at(const size_type index) const {
    if(!(index < size_))
        throw std::out_of_range("Index out of range");
    return array_[index];
}

#endif // VECTORCLASS_HPP
