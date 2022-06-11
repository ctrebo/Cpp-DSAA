#include "Ds/list.hpp"
#include "Ds/hashtable.hpp"
#include "custom_matchers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <array>
#include <functional>
#include <initializer_list>
#include <utility>
#include <exception>


TEST_CASE("Test own implemented Hashtable constructors") {
    //HashTable(size_type size);
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

    //HashTable(const HashTable& ht);
    SECTION("Initalize HashTable with copy of 'ht'") {
        ds::HashTable<int, int> hs {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};

        ds::HashTable<int, int> ht {hs};
        REQUIRE(ht.size() == hs.size());
        REQUIRE(ht.capacity() == hs.capacity());
    }

    //template<class InputIt>
    //HashTable(InputIt first, InputIt last);
    SECTION("Input values between first and last") {
        using hashtable = ds::HashTable<int, int>;
        std::array<std::pair<int, int>, 6> arr {{{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}}};
        hashtable ht(arr.begin(), arr.end());
        REQUIRE(arr.size() == ht.size());
        REQUIRE(ht[1] == 2);

        std::array arr1 {1, 2, 3, 4, 5, 6, 7};
        CHECK_THROWS_AS(hashtable(arr1.begin(), arr1.end()),  std::invalid_argument);

    }

    //HashTable(const HashTable&& other) noexcept;
    SECTION("Move construct HashTable from 'other'") {
        ds::HashTable<int, int> hs {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};

        ds::HashTable<int, int> ht {std::move(hs)};
        REQUIRE(ht.size() == 6);
        REQUIRE(hs.size() == 0);

    }
}

TEST_CASE("Test own implemented Hashtable size functions") {

    //constexpr size_type size() const;
    SECTION("Return size of HashTable") {
        ds::HashTable<int, int> hs {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};
        REQUIRE(hs.size() == 6);
    }
    //sconstexpr size_type capacity() const;
    SECTION("Return capacity of HashTable") {
        ds::HashTable<int, int> ht(20000);
        REQUIRE(ht.capacity()==20000);
    }

    //constexpr bool empty() const;
    SECTION("Return true if HashTable is empty") {
        ds::HashTable<int, int> ht {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};
        REQUIRE(!ht.empty());

        ds::HashTable<int, int> ht1;
        REQUIRE(ht1.empty());
    }

    //constexpr bool exists(const key_type& key);
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
        CHECK(ht.size() == 7);
        CHECK(ht[7] == 8);


    }

    //const T& operator[](const key_type& key) const;
    SECTION("Return const reference to element") {
        const ds::HashTable<int, int> const_ht {{0, 1}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};
        
        CHECK(const_ht[0] == 1);
        CHECK_THROWS_AS(const_ht[100],  std::invalid_argument);
    }

    //void erase(const key_type& key);
    SECTION("Erase element from HashTable") {
        ht.erase(0);
        CHECK(ht.size() == 5);
        CHECK(!ht.exists(0));
    }

    //HashTable<Key, T>& operator=(HashTable<Key, T> other) noexcept;
    SECTION("Assing value of 'other' to '*this'") {
        ds::HashTable<int, int> ht1;
        ht1 = ht;
        CHECK(ht.size() == ht1.size());
        CHECK(ht[3] == ht1[3]);

        ht1 = std::move(ht);
        CHECK(ht.size() != ht1.size());
    }

}
