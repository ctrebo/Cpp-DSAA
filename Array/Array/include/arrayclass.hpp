#ifndef ARRAYCLASS
#define ARRAYCLASS

#include <memory>
#include <cstddef>
#include <initializer_list>
#include <exception>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <compare>


using size_type = std::size_t;


// Use to check if all values are of the same type
template<typename T, typename... Ts>
constexpr inline bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

// N equals number of elements
template<class T, size_type N>
class ArrayClass {
private:
    size_type size_ {};
    std::unique_ptr<T[]> array_ {};
public:
    // Constructors
    ArrayClass();
    ArrayClass(std::initializer_list<T> l);
    ArrayClass(const ArrayClass& arr);
    ArrayClass(const ArrayClass&& arr);
    
    constexpr size_type size() const noexcept;
    constexpr size_type length() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr T& front() const;
    constexpr T& back() const;
    T* data() const noexcept;

    // Pointer to last and first element of Array
    constexpr T* begin() noexcept;
    constexpr const T* begin() const noexcept;
    constexpr const T* cbegin() const noexcept;
    //constexpr T* rbegin() noexcept;

    constexpr T* end() noexcept;
    constexpr const T* end() const noexcept;
    constexpr const T* cend() const noexcept;
    //constexpr T* rend() noexcept;

    // Access array 
    T& operator[] (const size_type index);
    const T& operator[] (const size_type index) const;
    T& at(const size_type index);
    const T& at(const size_type index) const;

    // Operations
    void fill(const T& value);
    constexpr void swap(const size_type index1, const size_type index2);
    constexpr void swap(ArrayClass& array);

    auto operator<=>(const ArrayClass&) const=default;

};

template<typename T, typename... Args>
requires are_same_v<T, Args...>
constexpr auto makeArray(T&& t, Args&&... args) -> ArrayClass<T, 1 + sizeof...(Args)> {
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
    for (auto& element : l)
    {
        array_[count] = element;
        ++count;
    }
}

template<class T, size_type N>
ArrayClass<T, N>::ArrayClass(const ArrayClass& arr): array_ {std::make_unique<T[]>(arr.size_)}, size_ {arr.size_} {
    for(size_type i {0}; i < size_; ++i) {
        array_[i] = arr[i];
    }
}

template<class T, size_type N>
ArrayClass<T, N>::ArrayClass(const ArrayClass&& arr): size_ {arr.size_}, array_ {std::move(arr.array_)}{

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
T& ArrayClass<T, N>::operator[] (const size_type index) {
    return array_[index];
}

template<class T, size_type N>
const T& ArrayClass<T, N>::operator[] (const size_type index) const {
    return array_[index];
}

template<class T, size_type N>
T& ArrayClass<T, N>::at(const size_type index) {
    if(!(index < size_))
        throw std::out_of_range("Index out of bounds!");
    return array_[index];
}

template<class T, size_type N>
const T& ArrayClass<T, N>::at(const size_type index) const {
    if(!(index < size_))
        throw std::out_of_range("Index out of bounds!");
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
constexpr T* ArrayClass<T, N>::begin() noexcept {
    return array_.get();
}

template<class T, size_type N>
constexpr const T* ArrayClass<T, N>::begin() const noexcept {
    return array_.get();
}

template<class T, size_type N>
constexpr const T* ArrayClass<T, N>::cbegin() const noexcept {
    return array_.get();
}


template<class T, size_type N>
constexpr T* ArrayClass<T, N>::end() noexcept {
    return &(array_[size_]);
}

template<class T, size_type N>
constexpr const T* ArrayClass<T, N>::end() const noexcept {
    return &(array_[size_]);
}

template<class T, size_type N>
constexpr const T* ArrayClass<T, N>::cend() const noexcept {
    return &(array_[size_]);
}

template<class T, size_type N>
void ArrayClass<T, N>::fill(const T& value) {
    std::fill(begin(), end(), value);
}

template<class T, size_type N>
constexpr void ArrayClass<T, N>::swap(const size_type index1, const size_type index2) {
    if(!(index1 < size_) || !(index2 < size_))
        throw std::out_of_range("At least one index out of bounds!");
    if(index1 == index2)
        return;
    std::swap(array_[index1], array_[index2]);
}

template<class T, size_type N>
constexpr void ArrayClass<T, N>::swap(ArrayClass& src) {
    // Check if passed in element is not the element itself
    if(src == *this) {
        return;
    }
    this->array_.swap(src.array_);
}

#endif // ARRAYCLASS
