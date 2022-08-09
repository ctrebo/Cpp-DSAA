#ifndef HASHTABLE_W_LLIST
#define HASHTABLE_W_LLIST

#include "concepts.hpp"
#include "list.hpp"

#include <cassert>
#include <list>
#include <initializer_list>
#include <iterator>

namespace ds {

template<Hashable Key, class T>
class HT {
public:
    using key_type = Key;
    using mapped_type = T; 
    using value_type = std::pair<Key, T>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

public:
    HT();
    HT(size_type size);
    HT(std::initializer_list<value_type> iList);
    template<is_it InputIt>
    HT(InputIt first, InputIt last);

    constexpr size_type capacity() const;
    constexpr size_type size() const;

    constexpr bool exists(const key_type& key) const;

    // Modifiers
    bool insert(const key_type& key, const mapped_type& value);
    bool insert(const value_type& pair);
    //value_type& operator[](const key_type& key);

private:
    size_type capacity_;   
    size_type size_;   
    std::vector<std::list<value_type>> arr_;   

private:
    static size_type addUpNums(size_type num);
    size_type hashFunction(const Key& key) const;

    template<class Type>
    bool insertPriv(const Key& key, Type&& val);
};

template<Hashable Key, class T>
HT<Key, T>::HT(): capacity_ {0}, size_ {0}, arr_(0) {};

template<Hashable Key, class T>
HT<Key, T>::HT(size_type size): capacity_ {size}, size_ {0}, arr_(capacity_) {};

template<Hashable Key, class T>
HT<Key, T>::HT(std::initializer_list<value_type> iList): HT(iList.size() < 10000 ? 10000 : iList.size() * 2) {
   for(const auto& elem: iList) {
        insertPriv(elem.first, elem.second);
   }
}

template<Hashable Key, class T>
template<is_it InputIt>
HT<Key, T>::HT(InputIt first, InputIt last): HT(std::distance(first, last) < 10000 ? 10000 : std::distance(first, last) * 2) {
    for(auto it=first; it != last; ++it) {
        insertPriv((*it).first, (*it).second);
    }
}

template<Hashable Key, class T>
constexpr HT<Key, T>::size_type HT<Key, T>::capacity() const {
    assert(size_ <= capacity_ && "Size can not be bigger than the capacity");
    return capacity_;
}

template<Hashable Key, class T>
constexpr HT<Key, T>::size_type HT<Key, T>::size() const {
    assert(size_ <= capacity_ && "Size can not be bigger than the capacity");
    return size_;
}

template<Hashable Key, class T>
constexpr bool HT<Key, T>::exists(const key_type& key) const {
    size_type index {hashFunction(key)};
    
    auto list = arr_[index];
    for(auto it=list.begin(); it != list.end(); ++it) {
        if((*it).first == key) {
            return true;
        }
    }

    return false;
}

template<Hashable Key, class T>
bool HT<Key, T>::insert(const key_type& key, const mapped_type& value) {
    return insertPriv(key, value);
}

template<Hashable Key, class T>
bool HT<Key, T>::insert(const value_type& pair) {
    return insertPriv(pair.first, pair.second);
}

//template<Hashable Key, class T>
//HT<Key, T>::value_type& HT<Key, T>::operator[](const key_type& key) {


//}

template<Hashable Key, class T>
HT<Key, T>::size_type HT<Key, T>::addUpNums(size_type num) {
    std::size_t sum {0}; 
    size_type new_num = num;
    while(new_num > 0) {
        short d = new_num % 10;
        sum += (d+48);
        new_num /= 10;
    }
    return sum;
}

template<Hashable Key, class T>
HT<Key, T>::size_type HT<Key, T>::hashFunction(const Key& key) const {
    assert(capacity_ > 0 && "Capacity has to be greater than 0");
    return (HT<Key, T>::addUpNums(std::hash<T>{}(key)) % capacity_);
}

template<Hashable Key, class T>
template<class Type>
bool HT<Key, T>::insertPriv(const Key& key, Type&& val) {
        size_type index {hashFunction(key)};
        if(exists(key)) {
            return false;
        }

        if(size_ >= capacity_) {
            throw std::length_error("Cant insert element into full hashtable");
        }

        arr_[index].push_back(std::make_pair(key, std::forward<Type>(val)));
        ++size_;
        return true;
}

}
#endif // HASHTABLE_W_LLIST
