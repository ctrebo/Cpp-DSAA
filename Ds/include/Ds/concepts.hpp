#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <iterator>

namespace ds {

template<class T>
concept is_it = std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category >;

template<class T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

}
#endif //CONCEPTS_HPP
