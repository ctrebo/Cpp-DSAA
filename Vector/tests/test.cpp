#include "vectorclass.hpp"
#include "custom_matchers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <array>
#include <utility>

using size_type = VectorClass<int>::size_type;

// Every test in this TESTCASE will be required because
// ctors are the base of the class. If they do not work, nothing will.
// Set up ctors in each section seperate because there won't
// be a common starting point.

TEST_CASE("Test own implemented vectors Constructors", "[VectorClass]") {

    // VectorClass() = default;
    SECTION("Default ctor construct size and capacity to 0. No memory allocation happens") {
        VectorClass<int> v;
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        // If array hasny been allocated it 'bool(v.data())' is false
        REQUIRE_FALSE(bool(v.data()));

    }

    //VectorClass(size_type size, const T& value=T{}, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to given number and array with default values") {
        size_type count {5};
        VectorClass<int> v(count);

        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() == count);
        // If array hasnt been allocated, 'bool(v.data())' is false
        REQUIRE(bool(v.data()));

        SECTION("Vector got constructed with the right values") {
            REQUIRE_THAT(v, EqualsValueMatcher(0));
        }
    }

    //VectorClass(size_type size, const T& value=T{}, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to given number, and constructs array with given value") {
        size_type count {5};
        int value {1};
        VectorClass<int> v(count, 1);

        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() == count);
        // If array hasnt been allocated, 'bool(v.data())' is false
        REQUIRE(bool(v.data()));
        SECTION("Vector got constructed with the right values") {
            REQUIRE_THAT(v, EqualsValueMatcher(value));
        }
    }
    
    //VectorClass(std::initializer_list<T> l, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to size of initializer list") {
        std::initializer_list<int> initL {1, 2, 3, 4, 5};
        VectorClass<int> v {initL};

        REQUIRE(v.size() == initL.size());
        REQUIRE(v.capacity() == initL.size());
        REQUIRE_THAT(v, EqualsContainer(initL));
    }

    //VectorClass(const VectorClass& other);
    SECTION("Copy ctor copys size and capacity from other vector. Values get deep copied") {
        VectorClass<int> v1{1, 2, 3 ,4 , 5};
        VectorClass<int> v2{v1};

        REQUIRE(v2.capacity() == v1.capacity());
        REQUIRE_THAT(v2, EqualsContainer(v1));
    }

    //VectorClass(const VectorClass&& other);
    SECTION("Move ctor moves size, capacity and array from one vector to another") {
        VectorClass<int> v1 {1, 2, 3, 4, 5};
        // Use another Vector v3 because v1 should be empty after move
        VectorClass<int> v3{v1};
        VectorClass<int> v2{std::move(v1)};

        REQUIRE(v2.capacity() == v3.capacity());
        REQUIRE_THAT(v2, EqualsContainer(v3));

        REQUIRE(v1.size() == 0);
        REQUIRE(v1.capacity() == 0);
        REQUIRE_FALSE(bool(v1.data()));
    }
    
    //VectorClass(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to range between first and last. Elements get deep copied") {
        VectorClass<int> v1 {1, 2, 3, 4, 5};
        VectorClass<int> v2{v1.cbegin(), v1.cend()};

        REQUIRE(v2.capacity() == v1.capacity());
        REQUIRE_THAT(v2, EqualsContainer(v1));
    }
}

TEST_CASE("Test own implemented vectors 'operator=' functions") {
    VectorClass<int> v1;
    VectorClass<int> v2 {1, 2, 3, 4, 5};

    //VectorClass<T, Allocator>& operator=(const VectorClass& other);
    SECTION("Copys size and capacity of 'other' and deep copies the values") {
       v1 = v2; 
       CHECK(v1.capacity() == v2.capacity());
       // EqualsContainer checks for same size too!
       CHECK_THAT(v1, EqualsContainer(v2));
    }

    //VectorClass<T, Allocator>& operator=(VectorClass&& other);
    SECTION("Moves size and capacity of 'other' and takes ownership of array of 'other'") {
        // Use third vector because v2 should be empty after move assignment
        VectorClass<int> v3 {v2};
        v1 = std::move(v2);
        CHECK(v1.capacity() == v3.capacity());
        CHECK_THAT(v1, EqualsContainer(v3));

        CHECK(v2.size() == 0);
        CHECK(v2.capacity() == 0);
        REQUIRE_FALSE(bool(v2.data()));
    }

    //VectorClass<T, Allocator>& operator=(std::initializer_list<T> l);
    SECTION("Assigns size of l to capacity and size of vector. Elements get deep copied") {
        std::initializer_list<int> iList {1, 2, 3, 4, 5};
        v1 = iList;
        CHECK(v1.capacity() == iList.size());
        CHECK_THAT(v1, EqualsContainer(iList));
    }
}

TEST_CASE("Test own implemented vectors length and capacity functions", "[VectorClass]") {
    int starting_cap {5};

    VectorClass<int> v(starting_cap);

    REQUIRE(v.size() == starting_cap);
    REQUIRE(v.capacity() == starting_cap);

    SECTION("Resizing bigger changes size and capacity") {
        v.resize(10);
        CHECK(v.size() == 10);
        CHECK(v.capacity() >= starting_cap);
    }

    SECTION("Resizing smaller changes size but not capacity") {
        v.resize(0);
        CHECK(v.size() == 0);
        CHECK(v.capacity() == starting_cap);
    }

    SECTION("Reserverving bigger changes capacity but not size") {
        v.reserve(starting_cap + 10);
        CHECK(v.size() == starting_cap);
        CHECK(v.capacity() >= starting_cap);
    }

    SECTION("Reserving down unused capacity does not change capacity or size") {
        v.reserve( 0 );
        CHECK(v.size() == starting_cap);
        CHECK(v.capacity() == starting_cap);
    }

    SECTION("Shrinking fit changes unused capacity to length of size") {
        // Get unused capacity
        v.reserve(starting_cap + 10);

        v.shrink_to_fit();
        CHECK(v.size() == v.capacity());
    }

    SECTION("Empty returns false if the vector has not size 0") {
        CHECK_FALSE(v.empty());

        SECTION("Empty returns true if the vector has size 0") {
            v.resize(0);
            CHECK(v.empty());
        }
    }
}

