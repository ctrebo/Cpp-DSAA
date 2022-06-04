#include "Ds/list.hpp"
#include "Ds/hashtable.hpp"
#include "custom_matchers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <array>
#include <functional>
#include <initializer_list>
#include <utility>


TEST_CASE("Test own implemented Hashtable constructors") {
    //HashTable(size_type size=10000);
    SECTION("Construct HashTable with size passed to constructor") {
        ds::HashTable<int, int> ht(20000);
        REQUIRE(ht.size()==0);
        REQUIRE(ht.capacity()==20000);

        ds::HashTable<int, int> ht1(10000);
        REQUIRE(ht1.size()==0);
        REQUIRE(ht1.capacity()==10000);
    }

    //HashTable(std::initializer_list<value_type> iList);
    SECTION("Insert values from 'iList' to HashTable") {
        std::initializer_list<std::pair<int, int>> iList {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};
        ds::HashTable<int, int> hs {iList};
        REQUIRE(hs.size() == iList.size());
    }
}

TEST_CASE("Test own implemented Hashtable size functions") {

    //size_type size() const;
    SECTION("Return size of HashTable") {
        ds::HashTable<int, int> hs {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};
        REQUIRE(hs.size() == 6);
    }
    //size_type capacity() const;
    SECTION("Return capacity of HashTable") {
        ds::HashTable<int, int> ht(20000);
        REQUIRE(ht.capacity()==20000);
    }

    //bool empty() const;
    SECTION("Return true if HashTable is empty") {
        ds::HashTable<int, int> ht {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};
        REQUIRE(!ht.empty());

        ds::HashTable<int, int> ht1;
        REQUIRE(ht1.empty());
    }

    //bool exists(const key_type& key);
    SECTION("Returns true if key exists in HashTable") {
        ds::HashTable<int, int> ht {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};

        REQUIRE(ht.exists(1));
        REQUIRE(!ht.exists(10));
    }
}

TEST_CASE("Test own implemented HashTables modifier functions") {
    ds::HashTable<int, int> ht {{0, 1}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};

    //T& operator[](const key_type& key);
    SECTION("Return reference to element") {
        ht[7] = 8;
        REQUIRE(ht.size() == 7);
        REQUIRE(ht[7] == 8);
    }
}
