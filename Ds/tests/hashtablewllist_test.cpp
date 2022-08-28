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

using size_type = ds::HT<int, int>::size_type;

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

    //HT(const HT& other);
    SECTION("Copy construct HT") {
        ds::HT<int, int> ht1{{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};

        ds::HT<int, int> ht{ht1};

        REQUIRE(ht.size() == ht1.size());
        REQUIRE(ht.capacity() == ht1.capacity());
        REQUIRE(ht[1] == ht1[1]);
    }

    //HT(HT&& other);
    SECTION("Move construct HT") {
        ds::HT<int, int> ht1{{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};
        ds::HT<int, int> ht1_copy{ht1};
        ds::HT<int, int> ht{std::move( ht1 )};

        REQUIRE(ht1_copy.size() == ht.size());
        REQUIRE(ht1_copy.capacity() == ht.capacity());
        REQUIRE(ht1_copy[1] == ht[1]);

        REQUIRE(ht1.size() == 0);
        REQUIRE(ht1.capacity() == 0);
    }
}

TEST_CASE("Test custom ht size functions", "[ht]") {

    //constexpr bool exists(const key_type& key) const;
    SECTION("Returns true if element exists in ht") {
        std::initializer_list<std::pair<int, int>> iList {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};
        ds::HT<int, int> ht {iList};

        CHECK(ht.exists(1).second);
        CHECK(!(ht.exists(10).second));
    }
}

TEST_CASE("Test custom ht modifier functions", "[ht]") {

    ds::HT<int, int> ht(10);

    //bool insert(const key_type& key, const mapped_type& value);
    SECTION("If 'key' doesnt exist in ht insert it. Return success of insertion") {
        ht.insert(1, 2);
        
        CHECK(ht.exists(1).second);
        CHECK(ht.size() == 1);
    }

    //bool insert(const value_type& pair);
    SECTION("If 'pair.first' doesnt exist in ht insert it. Return success of insertion") {

        ht.insert({1, 3});
        
        CHECK(ht.exists(1).second);
        CHECK(ht.size() == 1);
    }

    //template<class M>
    //bool insert_or_assign(const Key& k, M&& obj);
    SECTION("If element exists, assign std::forward<M>(object) to it, else insert it. Return bool if inserted, false if assigned") {
        CHECK(ht.insert_or_assign(1, 3) == true);
        CHECK(ht.size()==1);
        CHECK(ht.insert_or_assign(1, 3) == false);
        CHECK(ht.size()==1);
    }

    //template<class M>
    //bool insert_or_assign(Key&& k, M&& obj);
    SECTION("If element exists, assign std::forward<M>(object) to it, else insert it. Return bool if inserted, false if assigned") {
        CHECK(ht.insert_or_assign(1, 3) == true);
        CHECK(ht.size()==1);
        CHECK(ht.insert_or_assign(1, 3) == false);
        CHECK(ht.size()==1);
    }

    //HT<Key, T>& operator=(const HT other);
    SECTION("Assign other to *this") {
        ds::HT<int, int> ht1(10);
        ht1[1] = 3;
        ht1[3] = 3;
        ht1[4] = 9;

        ht = ht1;

        CHECK(ht.size() == ht1.size());
        CHECK(ht.capacity() == ht1.capacity());
        CHECK(ht[4] == ht1[4]);

    }

}

TEST_CASE("Test custom ht access elements", "[ht]") {
    ds::HT<int, int> ht(10);

    //value_type& operator[](const key_type& key);
    SECTION("If key doesnt exists in ht, insert it, else return reference to it") {
        int key = 5;
        int val = 3;
        ht[key] = val;
        CHECK(ht.size() == 1);
        CHECK(ht[key] == val);
        CHECK(ht.size() == 1);
    }

    //value_type& operator[](key_type&& key);
    SECTION("If key doesnt exists in ht, insert it, else return reference to it") {
        int key = 5;
        int key_copy = key;
        int val = 3;

        ht[std::move(key)] = 3;
        CHECK(ht.size() == 1);
        CHECK(ht[key_copy] == val);
        CHECK(ht.size() == 1);
    }

    //mapped_type& at(const key_type& key);
    SECTION("If element doesnt exist, throw std::out_of_range, else return pointer to element") {
        ht.insert({3,4});

        CHECK_THROWS_AS(ht.at(4), std::out_of_range);
        CHECK(ht.at(3) == 4);
    }

    //const mapped_type& at(const key_type& key) const;
    SECTION("Function for const class") {
        const ds::HT<int, int> const_ht{ {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6} };

        CHECK_THROWS_AS(const_ht.at(10), std::out_of_range);
        CHECK(const_ht.at(3) == 4);
    }

    //template<class... Args>
    //bool emplace(Args&&... args)
    SECTION("If element exists, return false, else return true and emplace element") {
        CHECK(ht.emplace(std::make_pair(12, 31)));
        CHECK(!ht.emplace(std::make_pair(12, 31)));
        CHECK(ht.size() == 1);
    }
}
