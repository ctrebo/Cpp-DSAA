#include "Ds/list.hpp"
#include "Ds/hashtablewllist.hpp"
#include "custom_matchers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <array>
#include <functional>
#include <initializer_list>
#include <utility>
#include <exception>

using size_type = std::size_t;

TEST_CASE("Test custom ht constructors", "[ht]") {
    //HT();
    SECTION("Default construct everything to zero") {
        ds::HT<int, int> ht;
        REQUIRE(ht.size() == 0);
        REQUIRE(ht.capacity() == 0);
    }

    //HT(size_type size);
    SECTION("Initialize HT with a capacity of 'size'") {
        size_type size {10000};
        ds::HT<int, int> ht(size);
        REQUIRE(ht.size() == 0);
        REQUIRE(ht.capacity() == size);
    }

    //HT(std::initializer_list<value_type> iList);
    SECTION("Insert all elements in 'iList' into hashtable") { 
        std::initializer_list<std::pair<int, int>> iList {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};
        ds::HT<int, int> ht {iList};

        REQUIRE(ht.size() == iList.size());
        REQUIRE(ht.capacity() == 10000);
    }

    //template<is_it InputIt>
    //HT(InputIt first, InputIt last);
    SECTION("Insert all elements between 'frist' and 'last' into hashtable") {
        std::vector<std::pair<int, int>> vec {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};
        ds::HT<int, int> ht(vec.begin(), vec.end());

        REQUIRE(ht.size() == vec.size());
    }
}

TEST_CASE("Test custom ht size functions", "[ht]") {

    //constexpr bool exists(const key_type& key) const;
    SECTION("Returns true if element exists in ht") {
        std::initializer_list<std::pair<int, int>> iList {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};
        ds::HT<int, int> ht {iList};

        CHECK(ht.exists(1));
        CHECK(!ht.exists(10));
    }
}

TEST_CASE("Test custom ht modifier functions", "[ht]") {

    //bool insert(const key_type& key, const mapped_type& value);
    SECTION("If 'key' doesnt exist in ht insert it. Return success of insertion") {
        ds::HT<int, int> ht(10);
        ht.insert(1, 2);
        
        CHECK(ht.exists(1));
        CHECK(ht.size() == 1);
    }

    //bool insert(const value_type& pair);
    SECTION("If 'pair.first' doesnt exist in ht insert it. Return success of insertion") {

        ds::HT<int, int> ht(10);
        ht.insert({1, 3});
        
        CHECK(ht.exists(1));
        CHECK(ht.size() == 1);
    }
}
