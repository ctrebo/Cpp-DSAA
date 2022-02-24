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
#include <exception>

using size_type = VectorClass<int>::size_type;

// Every test in this TESTCASE will be required because
// ctors are the base of the class. If they do not work, nothing will.
// Set up ctors in each section seperate because there won't
// be a common starting point.

TEST_CASE("Test own implemented vectors Constructors", "[VectorClass]") {

    //VectorClass<T, Allocator>::VectorClass() = default;
    SECTION("Default ctor construct size and capacity to 0. No memory allocation happens") {
        VectorClass<int> v;
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        // If array hasny been allocated it 'bool(v.data())' is false
        REQUIRE_FALSE(bool(v.data()));

    }

    //VectorClass<T, Allocator>::VectorClass(size_type size, const T& value=T{}, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to given number and array with default values") {
        size_type count {5};
        VectorClass<int> v(count);

        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() == count);
        // If array hasnt been allocated, 'bool(v.data())' is false
        REQUIRE(bool(v.data()));

        SECTION("Vector got constructed with the right values") {
            REQUIRE_THAT(v, EqualsVal(0));
        }
    }

    //VectorClass<T, Allocator>::VectorClass(size_type size, const T& value=T{}, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to given number, and constructs array with given value") {
        size_type count {5};
        int value {1};
        VectorClass<int> v(count, 1);

        REQUIRE(v.size() == count);
        REQUIRE(v.capacity() == count);
        // If array hasnt been allocated, 'bool(v.data())' is false
        REQUIRE(bool(v.data()));
        SECTION("Vector got constructed with the right values") {
            REQUIRE_THAT(v, EqualsVal(value));
        }
    }
    
    //VectorClass<T, Allocator>::VectorClass(std::initializer_list<T> l, const Allocator& alloc = Allocator());
    SECTION("Ctor constructs size and capacity to size of initializer list") {
        std::initializer_list<int> initL {1, 2, 3, 4, 5};
        VectorClass<int> v {initL};

        REQUIRE(v.size() == initL.size());
        REQUIRE(v.capacity() == initL.size());
        REQUIRE_THAT(v, EqualsContainer(initL));
    }

    //VectorClass<T, Allocator>::VectorClass(const VectorClass& other);
    SECTION("Copy ctor copys size and capacity from other vector. Values get deep copied") {
        VectorClass<int> v1{1, 2, 3 ,4 , 5};
        VectorClass<int> v2{v1};

        REQUIRE(v2.capacity() == v1.capacity());
        REQUIRE_THAT(v2, EqualsContainer(v1));
    }

    //VectorClass<T, Allocator>::VectorClass(const VectorClass&& other);
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
    
    //VectorClass<T, Allocator>::VectorClass(InputIt first, InputIt last, const Allocator& alloc = Allocator());
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

    //VectorClass<T, Allocator>& VectorClass<T, Allocator>::operator=(const VectorClass& other);
    SECTION("Copys size and capacity of 'other' and deep copies the values") {
       v1 = v2; 
       CHECK(v1.capacity() == v2.capacity());
       // EqualsContainer checks for same size too!
       CHECK_THAT(v1, EqualsContainer(v2));
    }

    //VectorClass<T, Allocator>& VectorClass<T, Allocator>::operator=(VectorClass&& other);
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

    //VectorClass<T, Allocator>& VectorClass<T, Allocator>::operator=(std::initializer_list<T> l);
    SECTION("Assigns size of l to capacity and size of vector. Elements get deep copied") {
        std::initializer_list<int> iList {1, 2, 3, 4, 5};
        v1 = iList;
        CHECK(v1.capacity() == iList.size());
       // EqualsContainer checks for same size too!
        CHECK_THAT(v1, EqualsContainer(iList));
    }
}

TEST_CASE("Test own implemented vectors assignment functions") {
    VectorClass<int> v1;

    //void VectorClass<T, Allocator>::assign(size_type count, const T& value);
    SECTION("Reserves space for 'count' elements and copys 'value' into each spot") {
        size_type count {5};
        int val {3};
        v1.assign(count, val);

       CHECK(v1.size() == count);
       CHECK(v1.capacity() == count);

       CHECK_THAT(v1, EqualsVal(val));
    }

    //template<class InputIt>
    //void VectorClass<T, Allocator>::assign(InputIt first, InputIt last);
    SECTION("Reserves space for'std::distance(first, last) values deep copies the values'") {
        VectorClass<int> v2 {1, 2, 3, 4, 5};
        v1.assign(v2.cbegin(), v2.cend());

        CHECK(v1.capacity() == v2.size());
        // EqualsContainer checks for same size too!
        CHECK_THAT(v1, EqualsContainer(v2));
    }

    //void VectorClass<T, Allocator>::assign(std::initializer_list<T> l);
    SECTION("Reserves space for size of initializer list, and deep copies values") {
        std::initializer_list<int> iList{1, 2, 3, 4, 5};
        v1.assign(iList);

        CHECK(v1.capacity() == iList.size());
        // EqualsContainer checks for same size too!
        CHECK_THAT(v1, EqualsContainer(iList));
    }
}

TEST_CASE("Test own implemented vectors length and capacity functions", "[VectorClass]") {
    int starting_cap {5};

    VectorClass<int> v(starting_cap);

    REQUIRE(v.size() == starting_cap);
    REQUIRE(v.capacity() == starting_cap);

    //void VectorClass<T, Allocator>::resize(size_type count, const value_type& value=T{});
    SECTION("Resizing bigger changes size and capacity") {
        v.resize(10);
        CHECK(v.size() == 10);
        CHECK(v.capacity() >= starting_cap);

        SECTION("Resizing smaller changes size but not capacity") {
            v.resize(0);
            CHECK(v.size() == 0);
            CHECK(v.capacity() == 10);
        }
    }


    //void VectorClass<T, Allocator>::reserve(size_type new_cap);
    SECTION("Reserverving bigger changes capacity but not size") {
        v.reserve(starting_cap + 10);
        CHECK(v.size() == starting_cap);
        CHECK(v.capacity() >= starting_cap);

        SECTION("Reserving down unused capacity does not change capacity or size") {
            v.reserve( 0 );
            CHECK(v.size() == starting_cap);
            CHECK(v.capacity() >= starting_cap);
        }
    }


    //void VectorClass<T, Allocator>::shrink_to_fit();
    SECTION("Shrinking fit changes unused capacity to length of size") {
        // Get unused capacity
        v.reserve(starting_cap + 10);

        v.shrink_to_fit();
        CHECK(v.size() == v.capacity());
    }

    //constexpr bool VectorClass<T, Allocator>::empty() const noexcept;
    SECTION("Empty returns false if the vector has not size 0") {
        CHECK_FALSE(v.empty());

        SECTION("Empty returns true if the vector has size 0") {
            v.resize(0);
            CHECK(v.empty());
        }
    }
}

TEST_CASE("Test own implemented element access functions") {
    int first_element {1};
    int last_element {5};
    
    VectorClass<int> v {first_element, 2, 3, 4, last_element};

    //constexpr reference VectorClass<T, Allocator>::front();
    SECTION("Returns reference to first element") {
        CHECK(v.front() == first_element);
        v.front() = 10;
        CHECK(v.front() != first_element);
    }

    //constexpr const_reference VectorClass<T, Allocator>::front() const;
    SECTION("Returns reference to first element, but that element cannot be changed") {
        const VectorClass<int> const_v {v};
        CHECK(const_v.front() == first_element);
    }

    //constexpr reference VectorClass<T, Allocator>::back();
    SECTION("Returns reference to last element") {
        CHECK(v.back() == last_element);
        v.back() = 10;
        CHECK(v.back() != last_element);
    }

    //constexpr const_reference VectorClass<T, Allocator>::back() const;
    SECTION("Returns reference to last element, but that element cannot be changed") {
        const VectorClass<int> const_v {v};
        CHECK(const_v.back() == last_element);
    }

    //pointer VectorClass<T, Allocator>::data() noexcept;
    SECTION("Return array of Vectorclass") {
        CHECK(&v[0] == v.data());
    }

    //reference VectorClass<T, Allocator>::operator[] (const size_type index);
    SECTION("Returns reference to element at index 'index'. Function does not check if element is in bounds") {
        CHECK(v[0] == first_element);
        CHECK(v[v.size() - 1] == last_element);
        v[0] = 10;
        CHECK(v[0] != first_element);
    }

    //const_reference VectorClass<T, Allocator>::operator[] (const size_type index) const;
    SECTION("Returns const reference to element at index 'index'. Function does not check if element is in bounds") {
        CHECK(v[0] == first_element);
        CHECK(v[v.size() - 1] == last_element);
    }

    //reference VectorClass<t, Allocator>::at(const size_type index);
    SECTION("Returns reference to element at index 'index'") {
        CHECK(v.at(0) == first_element);
        CHECK(v.at(v.size() - 1) == last_element);
        v[0] = 10;
        CHECK(v[0] != first_element);

        SECTION("If index is not in bounds a std::out_of_range error gets thrown") {
            CHECK_THROWS_AS(v.at(-1), std::out_of_range);
            CHECK_THROWS_AS(v.at(v.size() + 1), std::out_of_range);
        }
    }

    //const_reference VectorClass<t, Allocator>::at(const size_type index) const;
    SECTION("Returns const reference to element at index 'index'") {
        const VectorClass<int> const_v{v};
        CHECK(const_v.at(0) == first_element);
        CHECK(const_v.at(const_v.size() - 1) == last_element);

        SECTION("If index is not in bounds a std::out_of_range error gets thrown") {
            CHECK_THROWS_AS(const_v.at(-1), std::out_of_range);
            CHECK_THROWS_AS(const_v.at(const_v.size() + 1), std::out_of_range);
        }
    }
}

TEST_CASE("Test own implemented vectors modifier functions") {
    VectorClass<int> v1 {1, 2, 3, 4, 5};
    size_type old_capacity {v1.capacity()};

    //void VectorClass<T, Allocator>::push_back(const T& value);
    // The same version exists for r-values. There 'value' gets move constructed
    SECTION("Inserts element at the end of Vector. If vector is full, more capacity gets reserved") {
        int value {6};
        v1.push_back(value);
        
        CHECK(v1.back() == value);
        CHECK(v1.capacity() > old_capacity);
    }

    //void VectorClass<T, Allocator>::clear();
    SECTION("Makes the vector empty. Capacity stays the same") {
        v1.clear();

        CHECK(v1.size() == 0);
        CHECK(v1.capacity() == old_capacity);
    }

    //void VectorClass<T, Allocator>::insert(const_iterator pos, const T& value);
    // Overloaded function with r-value exists. This overloaded
    // function move constructs element.
    SECTION("Inserts 'value' at const iterator 'pos'. If size of vector equals capacity of vector before the insert more capacity gets reserved") {
        int val{5};
        v1.insert(v1.cbegin(), val);
        CHECK(v1.front() == val);
        CHECK(v1.capacity() > old_capacity);

        SECTION("If the iterator 'pos' is not inside 'cbegin()' and 'cend' std::out_of_range gets thrown") {
            CHECK_THROWS_AS(v1.insert(v1.cend() + 1, val), std::out_of_range);
            CHECK_THROWS_AS(v1.insert(v1.begin() - 1, val), std::out_of_range);
        }
    }

    //void VectorClass<T, Allocator>::insert(const_iterator pos, size_type count, const T& value);
    SECTION("Inserts 'count' values of 'value' at position 'pos'") {
        int val{5};
        size_type count {5};
        v1.insert(v1.cbegin(), count, val);
        VectorClass<int> v2 {v1.cbegin(), v1.cbegin() + 5};
        CHECK_THAT(v2, EqualsVal(val));
        CHECK(v1.capacity() > old_capacity);

        SECTION("If the iterator 'pos' is not inside 'cbegin()' and 'cend' std::out_of_range gets thrown") {
            CHECK_THROWS_AS(v1.insert(v1.cend() + 1,count, val), std::out_of_range);
            CHECK_THROWS_AS(v1.insert(v1.begin() - 1,count,  val), std::out_of_range);
        }

    }
    //void VectorClass<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last);
    SECTION("Inserts values between 'first' and 'last' at position 'pos'") {
        VectorClass<int> v2 {5, 4, 3, 2, 1};
        v1.insert(v1.cend(), v2.cbegin(), v2.cend());

        CHECK(v1.capacity() > old_capacity);
        CHECK_THAT(VectorClass<int>(v1.cend() - 5, v1.cend()), EqualsContainer(v2));

        SECTION("If the iterator 'pos' is not inside 'cbegin()' and 'cend' std::out_of_range gets thrown") {
            CHECK_THROWS_AS(v1.insert(v1.cend() + 1, v2.cbegin(), v2.cend()), std::out_of_range);
            CHECK_THROWS_AS(v1.insert(v1.begin() - 1, v2.cbegin(), v2.cend()), std::out_of_range);
        }
    }

    //void VectorClass<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> ilist );
    SECTION("Inserts values of initializer list at position 'pos'") {
        std::initializer_list<int> iList {1, 2, 3, 4, 5};
        v1.insert(v1.cbegin(), iList);
        CHECK(v1.capacity() > old_capacity);
        CHECK_THAT(VectorClass<int>(v1.cbegin(), v1.cbegin() + iList.size()), EqualsContainer(iList));

        SECTION("If the iterator 'pos' is not inside 'cbegin()' and 'cend' std::out_of_range gets thrown") {
            CHECK_THROWS_AS(v1.insert(v1.cbegin() -1, iList), std::out_of_range);
            CHECK_THROWS_AS(v1.insert(v1.cend() + 1, iList), std::out_of_range);
        }
    }

    //iterator VectorClass<T, Allocator>::erase(const_iterator pos);
    SECTION("Removes element from position 'pos'") {
       int second_element {v1[1]};
       v1.erase(v1.cbegin()); 

       CHECK(v1.capacity() == old_capacity);
       CHECK(v1.front() == second_element);
    }
}

TEST_CASE("Test own implemented vectors begin, end and rend functions") {
    
    VectorClass<int> v {1, 2, 3, 4, 5};
    const VectorClass<int> const_v {1, 2, 3, 4, 5};
    auto begin_const_it = const_v.begin();
    auto end_it = v.end();
    auto end_const_it = const_v.end();


    //constexpr iterator VectorClass<T, Allocator>::begin() noexcept;
    SECTION("Returns iterator to first element of vector") {
        auto begin_it = v.begin();
        CHECK(&(v[0]) == begin_it);
        ++begin_it;
        CHECK(&(v[1]) == begin_it);
    }

    //constexpr const_pointer VectorClass<T, Allocator>::begin() const noexcept;
    SECTION("Returns const iterator to first element of vector") {
        auto begin_const_it = const_v.begin();
        CHECK(&(const_v[0]) == begin_const_it);
    }

    //constexpr const_pointer VectorClass<T, Allocator>::cbegin() const noexcept;
    SECTION("Returns const iterator to first element of vector") {
        auto begin_const_it = const_v.cbegin();
        CHECK(&(const_v[0]) == begin_const_it);
    }

    //constexpr iterator VectorClass<T, Allocator>::end() noexcept;
    SECTION("Returns iterator to last + 1 element of vector") {
        auto end_it = v.end();
        CHECK(&(v.back()) + 1 == end_it);
        --end_it;
        CHECK(&(v.back()) == end_it);
    }

    //constexpr const_pointer VectorClass<T, Allocator>::end() const noexcept;
    SECTION("Returns const iterator to last + 1 element of vector") {
        auto end_const_it = const_v.end();
        CHECK(&(const_v.back()) + 1 == end_const_it);
    }

    //constexpr const_pointer VectorClass<T, Allocator>::cend() const noexcept;
    SECTION("Returns const iterator to last + 1 element of vector") {
        auto end_const_it = const_v.cend();
        CHECK(&(const_v.back()) + 1 == end_const_it);
    }
}
