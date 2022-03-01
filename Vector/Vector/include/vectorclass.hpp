#ifndef VECTORCLASS_HPP
#define VECTORCLASS_HPP

#include <iostream>
#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <exception>
#include <utility>
#include <memory_resource>
#include <stdexcept>
#include <limits>
#include <type_traits>

template<class T>
class TD;

template<class T>
concept is_it = std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category >;

template<class T, class Allocator = std::allocator<T>>
class VectorClass {
// Type aliases
public:
    using value_type = T;
    using allocator = Allocator;
    using traits_t = std::allocator_traits<allocator>;
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
    size_type size_ {};
    size_type capacity_ {};
    allocator alloc_ {};
    pointer array_ {};


public:
    // Constructors and Desctructor
    VectorClass() = default;
    explicit VectorClass(const Allocator& alloc) noexcept;
    VectorClass(size_type size, const T& value=T{}, const Allocator& alloc = Allocator());
    VectorClass(const VectorClass& other);
    VectorClass(const VectorClass& other, const Allocator& alloc);
    VectorClass(VectorClass&& other);
    template<class InputIt>
    requires is_it<InputIt>
    VectorClass(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    VectorClass(std::initializer_list<T> l, const Allocator& alloc = Allocator());
    ~VectorClass(); 

    // Overloading assignment operator
    VectorClass<T, Allocator>& operator=(const VectorClass& other);
    VectorClass<T, Allocator>& operator=(VectorClass&& other) noexcept;
    VectorClass<T, Allocator>& operator=(std::initializer_list<T> l);

    // Assign
    void assign(size_type count, const T& value);
    template<class InputIt>
    requires is_it<InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> l);

    // Length and capacity
    constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type capacity() const noexcept;
    void reserve(size_type new_cap);
    void shrink_to_fit();
    void resize(size_type count, const value_type& value=T{});

    // Modifiers
    void push_back(const T& value);
    // T&& is r-value reference because no type deduction takes place
    void push_back(T&& value);
    template<class... Args>
    void emplace_back(Args&&... args) {
        push_back(value_type(std::forward<Args>(args)...));
    }

    template< class... Args >
    void emplace(const_iterator pos, Args&&... args) {
        insert(pos, value_type(std::forward<Args>(args)...));
    }
    void clear();
    void insert(const_iterator pos, const T& value);
    void insert(const_iterator pos, T&& value);
    void insert(const_iterator pos, size_type count, const T& value);
    template<class InputIt>
    requires is_it<InputIt> 
    void insert(const_iterator pos, InputIt first, InputIt last);
    void insert(const_iterator pos, std::initializer_list<T> ilist );
    iterator erase(const_iterator pos);
    void pop_back();
    void swap(VectorClass& other) noexcept;


    // Pointers to first and last element
    constexpr iterator begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr const_pointer cbegin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cend() const noexcept;

    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    // Element access
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;
    pointer data() noexcept;
    const_pointer data() const noexcept;
    reference operator[] (const size_type index);
    const_reference operator[] (const size_type index) const;
    reference at(const size_type index);
    const_reference at(const size_type index) const;

    // Get allocator
    constexpr allocator get_allocator() const noexcept;

private:
    void destroyAndDealloc();
    template<class Type>
    void append(Type&& val);
    
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
VectorClass<T, Allocator>::VectorClass(const Allocator& alloc) noexcept: size_{0}, capacity_{0}, alloc_ {alloc} {}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(size_type size, const T& value, const Allocator& alloc): capacity_ {size}, size_ {size}, alloc_ {alloc}, array_ { traits_t::allocate(alloc_, capacity_) } {
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, value);
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(const VectorClass& other): size_ {other.size_}, capacity_{other.capacity_}, alloc_{traits_t::select_on_container_copy_construction(other.get_allocator())}, array_ {traits_t::allocate(alloc_, capacity_)} {
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, other[i]);
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(const VectorClass& other, const Allocator& alloc): size_ {other.size_}, capacity_{other.capacity_}, alloc_{alloc}, array_ {traits_t::allocate(alloc_, capacity_)} {
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, other[i]);
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(VectorClass&& other): size_ { other.size_ }, capacity_ {other.capacity_}, alloc_ {std::move(other.alloc_)}, array_ {other.array_} {
    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<class T, class Allocator>
template<class InputIt>
requires is_it<InputIt>
VectorClass<T, Allocator>::VectorClass(InputIt first, InputIt last, const Allocator& alloc): size_{static_cast<size_type>(std::distance(first, last))}, capacity_ {size_}, alloc_ {alloc}, array_ {traits_t::allocate(alloc_, capacity_)} {
    int i {0};
    for(InputIt it{first}; it != last; ++it) {
        traits_t::construct(alloc_, array_ + i, *it);
        ++i;
    }
}

template<class T, class Allocator>
VectorClass<T, Allocator>::~VectorClass() {
    destroyAndDealloc();
}

template<class T, class Allocator>
VectorClass<T, Allocator>::VectorClass(std::initializer_list<T> l, const Allocator& alloc): capacity_ {l.size()}, size_ { l.size()}, alloc_ {alloc}, array_ { traits_t::allocate(alloc_, capacity_)} {
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
    capacity_ = size_;
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
    capacity_ = size_;
    array_ = traits_t::allocate(alloc_, capacity_);
    for(size_type i {0}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, value);
    }
}

template<class T, class Allocator>
template<class InputIt>
requires is_it<InputIt>
void VectorClass<T, Allocator>::assign(InputIt first, InputIt last) {
    destroyAndDealloc();
    size_ = std::distance(first, last);
    capacity_ = size_;
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
    capacity_ = size_;
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
        traits_t::construct(alloc_, new_arr + i, std::move(array_[i]));
    }
    destroyAndDealloc();
    capacity_ = new_cap;
    array_ = new_arr;
    new_arr = nullptr;
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::shrink_to_fit() {
    if(size_ == capacity_) return;
    pointer new_arr {traits_t::allocate(alloc_, size_)};
    for(size_type i{0}; i < size_; ++i) 
        traits_t::construct(alloc_, new_arr + i, std::move(array_[i]));
    
    destroyAndDealloc();
    capacity_ = size_;
    array_ = new_arr;
    new_arr = nullptr;

}

template<class T, class Allocator>
void VectorClass<T, Allocator>::resize(size_type count, const value_type& value) {
    size_type old_size {size_};
    if(count <= capacity_) {
        size_ = count; 
    } else {
        reserve(count < capacity_ * 2 ? capacity_ * 2: count);
        size_ = count;
    }
    for(size_type i {old_size}; i < size_; ++i) {
        traits_t::construct(alloc_, array_ + i, value);
    }

}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::size_type VectorClass<T, Allocator>::capacity() const noexcept {
    return capacity_;
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::push_back(const T& value) {
    append(value);
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::push_back(T&& value) {
    append(std::move(value));
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
    size_type new_capacity {(size_ == capacity_)? capacity_* 2: capacity_};
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
    size_type new_capacity {(size_ == capacity_)?capacity_ * 2: capacity_};
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
template<class InputIt>
requires is_it<InputIt>
void VectorClass<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last) {
    const size_type count = std::distance(first, last);
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    size_type new_capacity {(size_ + count > capacity_)?(capacity_ +count) * 2: capacity_};
    pointer new_arr {traits_t::allocate(alloc_, new_capacity)};
    size_ += count;
    size_type start = std::distance(cbegin(), pos);
    size_type j {0};
    InputIt iter {first};
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
VectorClass<T, Allocator>::iterator VectorClass<T, Allocator>::erase(const_iterator pos) {
    if(!((pos >= begin()) and (pos <= end()))) {
        throw std::out_of_range("Iterator is out of bounds!");
    }
    iterator it = begin() + std::distance(cbegin(), pos);
    traits_t::destroy(alloc_, it);
    std::copy(it + 1, end(), it);
    --size_;
    return it; 
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::pop_back() {
    if (size_ == 0) {
        return;
    }
    traits_t::destroy(alloc_, array_ + (--size_));
}

template<class T, class Allocator>
void VectorClass<T, Allocator>::swap(VectorClass& other) noexcept {
    using std::swap;
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
    swap(array_, other.array_);
    swap(alloc_, other.alloc_);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::iterator VectorClass<T, Allocator>::begin() noexcept {
    return array_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_iterator VectorClass<T, Allocator>::begin() const noexcept {
    return array_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_iterator VectorClass<T, Allocator>::cbegin() const noexcept {
    return array_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::iterator VectorClass<T, Allocator>::end() noexcept {
    return array_ + size_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_iterator VectorClass<T, Allocator>::end() const noexcept {
    return array_ + size_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_iterator VectorClass<T, Allocator>::cend() const noexcept {
    return array_ + size_;
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::reverse_iterator VectorClass<T, Allocator>::rbegin() noexcept {
    return reverse_iterator(array_ + size_);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_reverse_iterator VectorClass<T, Allocator>::rbegin() const noexcept {
    return reverse_iterator(array_ + size_);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_reverse_iterator VectorClass<T, Allocator>::crbegin() const noexcept {
    return reverse_iterator(array_ + size_);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::reverse_iterator VectorClass<T, Allocator>::rend() noexcept {
    return reverse_iterator(array_);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_reverse_iterator VectorClass<T, Allocator>::rend() const noexcept {
    return reverse_iterator(array_);
}

template<class T, class Allocator>
constexpr VectorClass<T, Allocator>::const_reverse_iterator VectorClass<T, Allocator>::crend() const noexcept {
    return reverse_iterator(array_);
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
VectorClass<T, Allocator>::const_pointer VectorClass<T, Allocator>::data() const noexcept {
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
constexpr VectorClass<T, Allocator>::allocator VectorClass<T, Allocator>::get_allocator() const noexcept {
    return alloc_;
}

template<class T, class Allocator>
VectorClass<T, Allocator>::const_reference VectorClass<T, Allocator>::at(const size_type index) const {
    if(!(index < size_))
        throw std::out_of_range("Index out of range");
    return array_[index];
}

template<class T, class Allocator>
template<class Type>
void VectorClass<T, Allocator>::append(Type&& val) {
    if(size_ == capacity_) {
        size_type new_cap {capacity_ == 0 ? 1 : capacity_ * 2};
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    // Increment after inserting because next free element is
    // size_ not ++size_
    traits_t::construct(alloc_, array_ + (size_++), std::forward<Type>(val));
}

#endif // VECTORCLASS_HPP
