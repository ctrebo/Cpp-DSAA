#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <iterator>

namespace ds {

template<class T>
concept is_it = std::is_base_of_v< std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category >;

}
#endif //CONCEPTS_HPP
