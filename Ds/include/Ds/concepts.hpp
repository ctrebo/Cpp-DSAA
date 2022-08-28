#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <iterator>
#include <type_traits>

namespace ds {

template<class T>
concept is_it = std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category >;

template<class T>
concept Hashable = std::is_copy_constructible_v<T> && requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
    
};

template<class T>
concept MappedConcept = std::is_default_constructible_v<T> && std::is_copy_constructible_v<T>;

}
#endif //CONCEPTS_HPP
