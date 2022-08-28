#ifndef HASHTABLE_W_LLIST
#define HASHTABLE_W_LLIST

#include "concepts.hpp"
#include "list.hpp"

#include <cassert>
#include <list>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <iterator>

namespace ds {

template <Hashable Key, MappedConcept T>
class HT;

template <Hashable Key, MappedConcept T>
void swap(HT<Key, T>& first, HT<Key, T>& second) noexcept;

template<Hashable Key, MappedConcept T>
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
    HT(const HT& other);
    HT(HT&& other) noexcept;

    constexpr size_type capacity() const noexcept;
    constexpr size_type size() const noexcept;

    constexpr std::pair<T*, bool> exists(const key_type& key) const;

    // Modifiers
    bool insert(const key_type& key, const mapped_type& value);
    bool insert(const value_type& pair);
    template<class Type>
    bool insert_or_assign(const Key& k, Type&& obj);
    template<class Type>
    bool insert_or_assign(Key&& k, Type&& obj);
    HT<Key, T>& operator=(HT<Key, T> other);
    template<class...Args>
    bool emplace(Args&&... args);
    
    // Element access
    mapped_type& operator[](const key_type& key);
    mapped_type& operator[](key_type&& key);
    mapped_type& at(const key_type& key);
    const mapped_type& at(const key_type& key) const;


    friend void swap<Key, T>(HT<Key, T>& first, HT<Key, T>& second) noexcept;


private:
    size_type capacity_;   
    size_type size_;   
    std::vector<std::list<value_type>> arr_;   

private:
    static size_type addUpNums(size_type num);
    size_type hashFunction(const Key& key) const noexcept;

    template<class KType, class Type>
    bool insertPriv(KType&& key, Type&& val);
    template<class K>
    mapped_type& _lookup(K&& key);
    template<class KType,class Type>
    bool _insert_or_assign(KType&& key, Type&& val);
    template<class Pair>
    bool _emplace(Pair&& pair);

};

template<Hashable Key, MappedConcept T>
HT<Key, T>::HT(): capacity_ {0}, size_ {0}, arr_(0) {};

template<Hashable Key, MappedConcept T>
HT<Key, T>::HT(size_type size): capacity_ {size}, size_ {0}, arr_(capacity_) {};

template<Hashable Key, MappedConcept T>
HT<Key, T>::HT(std::initializer_list<value_type> iList): HT(iList.size() < 10000 ? 10000 : iList.size() * 2) {
   for(const auto& elem: iList) {
        insertPriv(elem.first, elem.second);
   }
}

template<Hashable Key, MappedConcept T>
template<is_it InputIt>
HT<Key, T>::HT(InputIt first, InputIt last): HT(std::distance(first, last) < 10000 ? 10000 : std::distance(first, last) * 2) {
    for(auto it=first; it != last; ++it) {
        insertPriv((*it).first, (*it).second);
    }
}

template<Hashable Key, MappedConcept T>
HT<Key, T>::HT(const HT& other):
    capacity_ {other.capacity_},
    size_ {other.size_},
    arr_ {other.arr_}
{
}

template<Hashable Key, MappedConcept T>
HT<Key, T>::HT(HT&& other) noexcept: HT(){
    swap(*this, other);
}

template<Hashable Key, MappedConcept T>
constexpr HT<Key, T>::size_type HT<Key, T>::capacity() const noexcept {
    assert(size_ <= capacity_ && "Size can not be bigger than the capacity");
    return capacity_;
}

template<Hashable Key, MappedConcept T>
constexpr HT<Key, T>::size_type HT<Key, T>::size() const noexcept {
    assert(size_ <= capacity_ && "Size can not be bigger than the capacity");
    return size_;
}

template<Hashable Key, MappedConcept T>
constexpr std::pair<T*, bool> HT<Key, T>::exists(const key_type& key) const {
    size_type index {hashFunction(key)};
    
    const auto& list = arr_[index];
    for(auto it=list.begin(); it != list.end(); ++it) {
        if((*it).first == key) {
            return std::make_pair(const_cast<T*>(&((*it).second)), true);
        }
    }

    return std::make_pair(nullptr, false);
}
template<Hashable Key, MappedConcept T>
bool HT<Key, T>::insert(const key_type& key, const mapped_type& value) {
    return insertPriv(key, value);
}

template<Hashable Key, MappedConcept T>
bool HT<Key, T>::insert(const value_type& pair) {
    return insertPriv(pair.first, pair.second);
}

template<Hashable Key, MappedConcept T>
template<class Type>
bool HT<Key, T>::insert_or_assign(const Key& key, Type&& obj) {
    return _assign_or_insert(key, std::forward<Type>(obj));
}

template<Hashable Key, MappedConcept T>
template<class Type>
bool HT<Key, T>::insert_or_assign(Key&& key, Type&& obj) {
    return _insert_or_assign(std::move(key), std::forward<Type>(obj));
}


template<Hashable Key, MappedConcept T>
HT<Key, T>& HT<Key, T>::operator=(HT<Key, T> other) {
    swap(*this, other);
    
    return *this;
}

template<Hashable Key, MappedConcept T>
template<class...Args>
bool HT<Key, T>::emplace(Args&&... args) {
    return _emplace(value_type(std::forward<Args>(args)...));
}

template<Hashable Key, MappedConcept T>
HT<Key, T>::mapped_type& HT<Key, T>::operator[](const key_type& key) {
    return _lookup(key);
}

template<Hashable Key, MappedConcept T>
HT<Key, T>::mapped_type& HT<Key, T>::operator[](key_type&& key) {
    return _lookup(std::move( key ));
}

template<Hashable Key, MappedConcept T>
HT<Key, T>::mapped_type& HT<Key, T>::at(const key_type& key) {
    auto pair = exists(key);
    if(!(pair.second)) {
        throw std::out_of_range("Element does not exist!");
    }
    return *(pair.first);
}

template<Hashable Key, MappedConcept T>
const HT<Key, T>::mapped_type& HT<Key, T>::at(const key_type& key) const {
    auto pair = exists(key);
    if(!(pair.second)) {
        throw std::out_of_range("Element does not exist!!");
    }
    return *(pair.first);
}

template<Hashable Key, MappedConcept T>
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

template<Hashable Key, MappedConcept T>
HT<Key, T>::size_type HT<Key, T>::hashFunction(const Key& key) const noexcept {
    assert(capacity_ > 0 && "Capacity has to be greater than 0");
    return (HT<Key, T>::addUpNums(std::hash<Key>{}(key)) % capacity_);
}

template<Hashable Key, MappedConcept T>
template<class KType, class Type>
bool HT<Key, T>::insertPriv(KType&& key, Type&& val) {
    size_type index {hashFunction(key)};
    if(exists(key).second) {
        return false;
    }

    if(size_ >= capacity_) {
        throw std::length_error("Cant insert element into full hashtable");
    }

    arr_[index].push_back(std::make_pair(std::forward<KType>(key), std::forward<Type>(val)));
    ++size_;
    return true;
}

template<Hashable Key, MappedConcept T>
template<class K>
HT<Key, T>::mapped_type& HT<Key,T>::_lookup(K&& key) {
    size_type index {hashFunction(key)};
    
    auto pair = exists(key);
    if(!(pair.second)) {
        if(size_ >= capacity_) {
            throw std::length_error("Cant insert element into full hashtable");
        }

        arr_[index].push_back(std::make_pair(std::forward<K>(key), T()));
        ++size_;

        auto& list = arr_[index];

        auto it = list.begin();
        std::advance(it, list.size() - 1);
        return (*it).second;
    }
    return *(pair.first);
}

template <Hashable Key, MappedConcept T>
template<class KType,class Type>
bool HT<Key, T>::_insert_or_assign(KType&& key, Type&& val) {
    auto pair = exists(key);
    if(pair.second) {
        *(pair.first) = std::forward<Type>(val);
        return false;
    } else {
        size_type index {hashFunction(key)};
        if(size_ >= capacity_) {
            throw std::length_error("Cant insert element into full hashtable");
        }

        arr_[index].push_back(std::make_pair(std::forward<KType>( key ), std::forward<Type>(val)));
        ++size_;
        return true;
    }

}

template <Hashable Key, MappedConcept T>
void swap(HT<Key, T>& first, HT<Key, T>& second) noexcept {
    using std::swap;

    swap(first.arr_, second.arr_);
    swap(first.size_, second.size_);
    swap(first.capacity_, second.capacity_);
}

template <Hashable Key, MappedConcept T>
template<class Pair>
bool HT<Key, T>::_emplace(Pair&& pair) {
    if (exists(pair.first).second) {
        return false;
    }
    if(size_ >= capacity_) {
        throw std::length_error("Cant insert element into full hashtable");
    }
    size_type index {hashFunction(pair.first)};
    arr_[index].push_back(std::forward<Pair>(pair));
    ++size_;
    return true;
}

}
#endif // HASHTABLE_W_LLIST
