#include "List/list.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <array>

TEST_CASE("Test own implemented List Class constructors") {

    //List();
    SECTION("Default constructor, everything will be set to 0 or nullptr") {
        ds::List<int> l;
        REQUIRE(l.size() == 0);
    }

    //List(size_type count, const T& value = T(), const Allocator& alloc = Allocator());
    SECTION("Construct 'count' nodes with value 'value'") {
        int val {3};
        std::size_t count {6};
        ds::List<int> l(count, val);

        REQUIRE(l.size() == count);
        for(std::size_t i {0}; i < l.size(); ++i) {
            REQUIRE(val == l[0]);
        }
    }

    //List(std::initializer_list<T> iList, const Allocator& alloc = Allocator());
    SECTION("Construct iList.size() nodes with elements from 'iList'") {
        int first_elem {1};
        int last_elem {5};
        ds::List<int> l {1, 2, 3, 4, 5};
        REQUIRE(l.size() == 5);
        REQUIRE(l[0] == first_elem);
        REQUIRE(l[4] == last_elem);
    }

    //template<class T>
    //List(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    SECTION("Construct list with values in range 'first' and 'last'") {
        constexpr int size {5};
        std::array<int, size> arr {1, 2, 3, 4, 5};
        ds::List<int> l(arr.begin(), arr.end());
    }
}

TEST_CASE("Test own implemented List Class access functions") {
    //reference operator[](const size_type index);
    SECTION("Return reference to object at index 'index'. No index checking takes place") {
        int first_elem {1};
        int last_elem {5};
        ds::List<int> l{first_elem, 2, 3, 4, last_elem};
        REQUIRE(l[0] == first_elem);
        REQUIRE(l[4] == last_elem);
    }
}


