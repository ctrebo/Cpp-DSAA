#include "Ds/binarysearchtree.hpp"
#include "custom_matchers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <array>
#include <functional>
#include <initializer_list>

TEST_CASE("Test own implemented BSTs Constructors") {
    //BST();
    SECTION("Set root to nullptr and size to 0") {
        ds::BST<int> b;
        REQUIRE(b.size() == 0);
    }

    //BST(std::initializer_list<T> iList);
    SECTION("Insert values in initializer list if they are not in the tree yet") {
        std::initializer_list<int> iList {1, 2, 3, 4, 4, 5, 6};
        ds::BST<int> b {iList};
        REQUIRE(b.size() == iList.size() - 1);
    }

    //BST(const BST<T>& other);
    SECTION("Deep copy values from other to '*this'") {
        ds::BST<int> b {1, 2, 3, 4, 4, 5, 6};
        ds::BST<int> b1 {b};
        REQUIRE_THAT(b1, EqualsContainer(b));
    }

    //BST(BST<T>&& other) noexcept;
    SECTION("Deep copy values from other to '*this'") {
        ds::BST<int> b {1, 2, 3, 4, 4, 5, 6};
        ds::BST<int> b1 {b};
        ds::BST<int> b2 {std::move(b)};
        REQUIRE_THAT(b1, EqualsContainer(b2));
        REQUIRE(b.empty());
    }

    //template<class InputIt>
    //BST(InputIt first, InputIt last);
    SECTION("Copy values between first and last in bst") {
        std::array arr {1, 2, 3, 4, 5, 6, 7};
        ds::BST<int> bst {arr.begin(), arr.end()};

        REQUIRE_THAT(arr, EqualsContainer(arr));
    }
}

TEST_CASE("Test own implemented BSTs modifier functions") {

    ds::BST<int> b;
    
    //std::pair<BST<T>::Iterator, bool> insert(const T& value);
    SECTION("Insert element in tree. Return pair with it to node containing val or node that prevented insertion. Bool value tells if insertion was successful") {
        int val {4};
        auto pair = b.insert(val);

        CHECK(b.size() == 1);
        CHECK(pair.second);
        CHECK(*(pair.first) == val);

        auto pair2 = b.insert(val);
        CHECK(b.size() == 1);
        CHECK(pair2.second == false);
        CHECK(*(pair2.first) == val);
    }

    //std::pair<BST<T>::Iterator, bool> insert(T&& value);
    SECTION("Insert element in tree. Return pair with it to node containing val or node that prevented insertion. Bool value tells if insertion was successful") {
        int val {4};
        auto pair = b.insert(std::move( val ));

        CHECK(b.size() == 1);
        CHECK(pair.second);
        CHECK(*(pair.first) == 4);

        int val2 {4};
        auto pair2 = b.insert(std::move( val2 ));
        CHECK(b.size() == 1);
        CHECK(pair2.second == false);
        CHECK(*(pair2.first) == 4);
    }

    //void insert(std::initializer_list<T> iList);
    SECTION("Inserts elements in 'iList' into BST if they do not already exist") {
        std::initializer_list<int> iList {1, 2, 3, 4, 4, 5, 6};

        b.insert(iList);
        CHECK(b.size() == iList.size() - 1);
    }

    //template<class InputIt>
    //void insert(InputIt first, InputIt last);
    SECTION("Insert values between first and last") {

        std::array arr {1, 2, 3, 4, 5, 6, 7};
        ds::BST<int> bst;

        bst.insert(arr.begin(), arr.end());

        REQUIRE_THAT(arr, EqualsContainer(arr));
    }

    //template<class... Args>
    //std::pair<BST<T>::Iterator, bool> emplace(Args&&... args);
    SECTION("Emplace element") {
        auto pair = b.emplace(4);

        CHECK(b.size() == 1);
        CHECK(pair.second);
        CHECK(*(pair.first) == 4);

        auto pair2 = b.emplace(4);
        CHECK(b.size() == 1);
        CHECK(pair2.second == false);
        CHECK(*(pair2.first) == 4);
    }

    //bool erase(const T& value);
    SECTION("Remove value from BST") {
        b.insert({ 5, 3, 1, 8, 2 ,4 , 6 ,99, 123, 424 });
        CHECK(!b.erase(100));
        CHECK(b.size() == 10);

        CHECK(b.erase(1));
        CHECK(b.size() == 9);

        CHECK(b.erase(123));
        CHECK(b.size() == 8);
    }

    //void clear();
    SECTION("Clear BST") {
        b.insert({1, 2, 3, 4, 5, 6, 7});
        b.clear();

        CHECK(b.size() == 0);
    }
}

TEST_CASE("Test own implemented BSTs operator= functions") {
    ds::BST<int> bst {5, 4, 7, 2, 1, 9, 15, -3};

    //BST<T> operator=(BST<T> other) noexcept;
    SECTION("Assign 'other' to '*this'") {
        ds::BST<int> bst1;
        bst1 = bst;

        CHECK_THAT(bst, EqualsContainer(bst1));
    }


}
