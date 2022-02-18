#include "vectorclass.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <cstddef>
#include <algorithm>
#include <functional>

using size_type = VectorClass<int>::size_type;



// Every test in this TESTCASE will be required because
// ctors are the base of the class. If they do not work, nothing will.
// Set up ctors in each section seperate because there won't
// be a common starting point.

TEST_CASE("Test own implemented vectors Constructors", "[VectorClass]") {
   SECTION("Default ctor construct size and capacity to 0. No memory allocation happens") {
        VectorClass<int> v;
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        // If array hasny been allocated it 'bool(v.data())' is false
        REQUIRE_FALSE(bool(v.data()));
        
   }

   SECTION("Ctor that takes number constructs size and capacity to number and allocates memory") {
       size_type count {5};
       int value {1};
       VectorClass<int> v(count, 1);

        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() == count);
        // If array hasny been allocated it 'bool(v.data())' is false
        REQUIRE(bool(v.data()));
        std::vector<int> v1(count, 1);
        SECTION("Vector got constructed with the right value") {
        }
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

