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

namespace ds {

#define MAX_SIZE 200

template<class Key, class T>
class HashTable {
public:
    using key_type = Key;
    using mapped_type = T; 
    using value_type = std::pair<Key, T>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using list_type = List<value_type>;

public:
    HashTable();
    template<class InputIt>
    HashTable(InputIt first, InputIt last);
    HashTable(const HashTable& other);
    HashTable(const HashTable& other);

private:
    size_type size_;
    std::array<list_type, MAX_SIZE> arr_;

private:
    static size_type addUpNums(size_type num);
    static size_type hashFunction(const Key& key);
};

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
    return (HashTable<Key, T>::addUpNums(std::hash<T>{}(key)) % MAX_SIZE);
}

template<class Key, class T>
HashTable<Key, T>::HashTable(): size_ {0} { 
}

};

#endif //HASH_TABLE_HPP
