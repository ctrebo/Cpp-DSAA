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
#include <cassert>
#include <exception>

namespace ds {

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
    HashTable();
    HashTable(size_type size);
    HashTable(std::initializer_list<value_type> iList);

    // Size functions
    size_type size() const;
    size_type capacity() const;
    bool empty() const;
    bool exists(const key_type& key);


    // Modifiers
    bool insert(const key_type& key, const mapped_type& value);
    bool insert(const key_type& key, const mapped_type&& value);
    T& operator[](const key_type& key);
    const T& operator[](const key_type& key) const;
    

private:
    size_type capacity_;
    size_type size_;
    std::vector<value_type> arr_;

private:
    static size_type addUpNums(size_type num);
    size_type hashFunction(const Key& key);
    size_type moduloIndex(size_type index) const;
    template<class Type>
    bool insertPriv(const Key& key, Type&& value);

};

template<class Key, class T>
HashTable<Key, T>::HashTable(): capacity_ {10000}, arr_(capacity_), size_(0) {}

template<class Key, class T>
HashTable<Key, T>::HashTable(size_type size): capacity_ {size}, arr_(capacity_), size_ {0} {}

template<class Key, class T>
HashTable<Key, T>::HashTable(std::initializer_list<value_type> iList): capacity_ {iList.size() < 10000 ? 10000 : iList.size()*2}, size_(0), arr_(capacity_) {
    for(const auto& elem: iList) {
        insertPriv(elem.first, elem.second);
    }
}

template<class Key, class T>
HashTable<Key, T>::size_type HashTable<Key, T>::size() const {
    assert(size_ <= capacity_ && "Size can not be greater than 200");
    return size_;
}

template<class Key, class T>
HashTable<Key, T>::size_type HashTable<Key, T>::capacity() const {
    assert(size_ <= capacity_ && "Size can not be greater than capacity");
    return capacity_;
}

template<class Key, class T>
bool HashTable<Key, T>::empty() const {
    assert(size_ <= capacity_ && "Size can not be greater than capacity");
    return size_ == 0;
}

template<class Key, class T>
bool HashTable<Key, T>::exists(const key_type& key) {
    size_type index {HashTable<Key, T>::hashFunction(key)};
    size_type old_index {index};
    while(true) {
        if(key == arr_[moduloIndex(index)].first) {
            return true;
        }
        if (index == old_index) {
            break;
        }
        ++index; 
    }
    return false;
}

template<class Key, class T>
T& HashTable<Key, T>::operator[](const key_type& key) {
    std::cout << "in here";
    size_type index = HashTable<Key, T>::hashFunction(key);
    assert(size_ <= capacity_ && size_ >= 0 && "Something went wrong!");
    if (exists(key)) {
        if(key == arr_[index].first) {
            return arr_[index].second;
        } else {
            ++index;
            while(true) {
                if(key == arr_[moduloIndex(index)].first) {
                    return arr_[moduloIndex(index)].second;
                }
                ++index;
            }
        }
    } else {
        while(true) {
            if(!(arr_[moduloIndex(index)].first)) {
                arr_[moduloIndex(index)].first = key;
                ++size_;
                return (arr_[moduloIndex(index)].second);
            }
            ++index;
        }
    }
}

template<class Key, class T>
const T& HashTable<Key, T>::operator[](const key_type& key) const {
    assert(size_ <= capacity_ && size_ >= 0 && "Something went wrong!");
    size_type index = HashTable<Key, T>::hashFunction(key);
    if (!exists(key)) {
        throw std::invalid_argument("Key is not present in the Hashtable");
    }

    if(key == arr_[index].first) {
        return arr_[index].second;
    } else {
        ++index;
        while(true) {
            if(key == arr_[moduloIndex(index)].first) {
                return arr_[moduloIndex(index)].second;
            }
        }
    }
    
}

template<class Key, class T>
bool HashTable<Key, T>::insert(const key_type& key, const mapped_type& value) {
    return insertPriv(key, value);
}

template<class Key, class T>
bool HashTable<Key, T>::insert(const key_type& key, const mapped_type&& value) {
    return insertPriv(key, std::move(value));
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

template<class Key, class T>
template<class Type>
bool HashTable<Key, T>::insertPriv(const Key& key, Type&& value) {
    if(exists(key)) {
        return false;
    }
    assert(size_ < capacity_ && "Array got no more space");
    size_type index {HashTable<Key, T>::hashFunction(key)};
    while(true) {
        if (!(arr_[moduloIndex(index)].first)) {
            arr_[moduloIndex(index)].first = key;  
            arr_[moduloIndex(index)].second = std::forward<Type>(value);  
            ++size_;
            return true;
        }
        ++index;
    }
}

}

#endif //HASH_TABLE_HPP
