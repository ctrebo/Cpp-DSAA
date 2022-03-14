#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <utility>
#include <type_traits>
#include <cstddef>

namespace ds {

    template <std::size_t index, class T>
    class TupleImpl {
    private:
        T val_;
    public:
        TupleImpl(T& val): val_ {val} {}
        TupleImpl(T&& val): val_ {std::move(val)} {}

        T& get() {
            return val_;
        }
    };
                


    template<std::size_t index, class... types>
    class TupleRecurBase {

    };

    template<std::size_t index, class L, class... types>
    class TupleRecurBase<index, L, types...>
        : public TupleImpl<index, std::remove_reference_t<L>>
        , public TupleRecurBase<index + 1, types...>
    {
    public:
        template<class CL, class... CArgs>
        TupleRecurBase(CL&& arg, CArgs&&... args)
            : TupleImpl<index, std::remove_reference_t<CL>>{std::forward<CL>(arg)}
            , TupleRecurBase<index + 1, types...>{std::forward<CArgs>(args)...} 
        {
        }

    };

    template<class L, class... types>
    class Tuple: public TupleRecurBase<0, L, types...> {
    public:
        //Constructor uses same recursion as inheritance
        template<class... CArgs>
        Tuple(CArgs&&... args): TupleRecurBase<0, L, types...>{std::forward<CArgs>(args)...}
        {
        }

        template<class... Args>
        friend bool operator==(Tuple<Args...>& t1, Tuple<Args...>& t2) {
            return compare_tuple<sizeof...(Args) - 1>(t1, t2);
        }

    };
    
    template<class... Types>
    decltype(auto) make_tuple(Types&&... args) {
        return Tuple<Types...> {std::forward<Types>(args)...};
    }

    // Template deduction guide for Tuple class
    template<class... CArgs>
    Tuple(CArgs... cargs) -> Tuple<CArgs...>;

    template<std::size_t index, class L, class... Args>
    struct ExtractTypeAt {
        using type = typename ExtractTypeAt<index - 1, Args...>::type;
    };
    
    // Provide nicer alias
    template<std::size_t index, class... Args>
    using ExtractTypeAt_t = typename ExtractTypeAt<index, Args...>::type;

    template<class L, class...Args>
    struct ExtractTypeAt<0, L, Args...> {
        using type = L;
    };
    
    template<std::size_t index, class... Args>
    auto& get(Tuple<Args...>& t) {
        return (static_cast<TupleImpl<index, ExtractTypeAt_t<index, Args...>>&>(t)).get();
    }

    template<std::size_t index, class... Args>
    bool compare_tuple(Tuple<Args...>& t1, Tuple<Args...>& t2) {
        if constexpr (index == 0) {
            return (get<0>(t1) == get<0>(t2));
        } else {
            return (get<index>(t1) == get<index>(t2)) && compare_tuple<index -1>(t1, t2);
        }

    }

}
#endif //TUPLE_HPP
