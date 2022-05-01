#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "list.hpp"

#include <utility>
#include <cstddef>
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <functional>
#include <memory>

namespace ds {

#define MAX_SIZE 5000

template<class Key, class T>
class HashTable {
public:
    using key_type = Key;
    using mapped_type = T; 
    using value_type = std::pair<Key, T>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

public:
    HashTable(size_type size=MAX_SIZE);
    HashTable(std::initializer_list<value_type>);

    T& operator[](const Key& key);

    // Modifiers

private:
    size_type capacity_;
    size_type size_;
    std::vector<value_type> arr_;

private:
    static size_type addUpNums(size_type num);
    size_type hashFunction(const Key& key);
    size_type moduloIndex(size_type index) const;

};

template<class Key, class T>
HashTable<Key, T>::HashTable(size_type size): capacity_ {size}, arr_(capacity_), size_ {0} {}

template<class Key, class T>
HashTable<Key, T>::HashTable(std::initializer_list<value_type>) {}


template<class Key, class T>
T& HashTable<Key, T>::operator[](const Key& key) {
    size_type index = HashTable<Key, T>::hashFunction(key);
    if(key == (arr_[index]).first) {
        return (arr_[index].second);
    } else if(!(arr_[index]).first) {
        arr_[index].first = key;
        return (arr_[index].second);
    }
    else {
        ++index;
        while(true) {
            if(key == (arr_[moduloIndex(index)]).first) {
                break;
            } else if(!(arr_[moduloIndex(index)].first)) {
                arr_[moduloIndex(index)].first = key;
                break;
            }
            ++index;
        }
        return (arr_[moduloIndex(index)].second);
    }
}

template<class Key, class T>
HashTable<Key, T>::size_type HashTable<Key, T>::addUpNums(size_type num) {
    std::size_t sum {0}; 
    size_type new_num = num;
    while(new_num > 0) {
        short d = new_num % 10;
        sum += (d+48);
        new_num /= 10;
    }
    return sum;
}

template<class Key, class T>
HashTable<Key, T>::size_type HashTable<Key, T>::hashFunction(const Key& key) {
    return (HashTable<Key, T>::addUpNums(std::hash<T>{}(key)) % capacity_);
}
template<class Key, class T>
HashTable<Key, T>::size_type HashTable<Key, T>::moduloIndex(size_type index) const {
    return index % capacity_; 
}

};

#endif //HASH_TABLE_HPP
