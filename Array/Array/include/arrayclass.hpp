#ifndef ARRAYCLASS
#define ARRAYCLASS

#include <memory>
#include <cstddef>
#include <initializer_list>
#include <exception>
#include <iostream>

using size_type = std::size_t;

// N equals number of elements
template<class T, size_type N>
class ArrayClass {
private:
    std::unique_ptr<T[]> array_ {};
    size_type size_ {};
public:
    ArrayClass();
    ArrayClass(std::initializer_list<T> l);
    
    constexpr size_type size() const noexcept;
    constexpr size_type length() const noexcept;
    constexpr bool empty() const noexcept;
    T& at(size_type index);
    const T& at(size_type index) const;
    constexpr T& front() const;
    constexpr T& back() const;
    T* data() const noexcept;

    constexpr T* begin() const noexcept;
    constexpr T* end() const noexcept;

    const T& operator[] (size_type index) const;
    T& operator[] (size_type index);
};

template<typename T, typename... Args>
constexpr auto makeArray(T&& t, Args&&... args) -> ArrayClass<T, 1 + sizeof...(args)> {
    return {{std::forward<T>(t), std::forward<Args>(args)...}};
}

template<class T, size_type N>
ArrayClass<T, N>::ArrayClass(): array_ {std::make_unique<T[]>(N)} ,size_ { N } {}

template<class T, size_type N>
ArrayClass<T, N>::ArrayClass(std::initializer_list<T> l) {
    if (N != l.size())
        throw std::out_of_range("Template Argument n doesnt match initializer list length!");
    array_ = std::make_unique<T[]>(l.size());
    size_ = l.size(); 
    int count{ 0 };
    for (auto element : l)
    {
        array_[count] = element;
        ++count;
    }
}
template<class T, size_type N>
constexpr size_type ArrayClass<T, N>::size() const noexcept{
    return size_;
}

template<class T, size_type N>
constexpr size_type ArrayClass<T, N>::length() const noexcept{
    return size_;
}

template<class T, size_type N>
constexpr bool ArrayClass<T, N>::empty() const noexcept{
    return (size_ == 0);
}

template<class T, size_type N>
T& ArrayClass<T, N>::operator[] (size_type index) {
    return array_[index];
}

template<class T, size_type N>
const T& ArrayClass<T, N>::operator[] (size_type index) const {
    return array_[index];
}

template<class T, size_type N>
T& ArrayClass<T, N>::at(size_type index) {
    if(!(index < size_))
        throw std::out_of_range("Template Argument n doesnt match initializer list length!");
    return array_[index];
}

template<class T, size_type N>
const T& ArrayClass<T, N>::at(size_type index) const {
    if(!(index < size_))
        throw std::out_of_range("Template Argument n doesnt match initializer list length!");
    return array_[index];
}

template<class T, size_type N>
constexpr T& ArrayClass<T, N>::front() const {
    return array_[0];
}

template<class T, size_type N>
constexpr T& ArrayClass<T, N>::back() const {
    return array_[size_ - 1];
}

template<class T, size_type N>
T* ArrayClass<T, N>::data() const noexcept {
    return array_.get();
}

template<class T, size_type N>
constexpr T* ArrayClass<T, N>::begin() const noexcept {
    return array_.get();
}

template<class T, size_type N>
constexpr T* ArrayClass<T, N>::end() const noexcept {
    return &(array_[size_]);
}

#endif // ARRAYCLASS
