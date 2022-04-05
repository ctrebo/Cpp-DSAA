#include "List/list.hpp"
#include "custom_matchers.hpp"

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
        REQUIRE_THAT(l, EqualsVal(val));
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

    //template<class InputIt>
    //List(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    SECTION("Construct list with values in range 'first' and 'last'") {
        ds::List<int> arr {1, 2, 3, 4, 5};
        ds::List<int> l (arr.begin(), arr.end());

        REQUIRE_THAT(l, EqualsContainer(arr));
    }

    //List(const List& other);
    SECTION("Construct list with deep copied values from 'other'") {
        ds::List<int> l {1, 2, 3, 4, 5};
        ds::List<int> copied_l (l);

        REQUIRE_THAT(l, EqualsContainer(copied_l));
    }

    //List(List&& other);
    SECTION("Construct list and move nodes from 'other'") {
        ds::List<int> l {1, 2, 3, 4, 5};
        ds::List<int> copied_l (std::move(l));

        REQUIRE_THAT(l, !EqualsContainer(copied_l));
        REQUIRE(l.size() == 0);
    }
}

TEST_CASE("Test own implemented List Class size functions") {
    int length {5};
    ds::List<int> l(length);

    //constexpr size_type size() const noexcept;
    SECTION("Returns length of list") {
        REQUIRE(l.size() == length);
    }

    //constexpr bool empty() const noexcept;
    SECTION("Returns true if list if empty") {
        ds::List<int> l1;
        REQUIRE(l1.empty());
        l1.push_back(2);
        REQUIRE(!(l1.empty()));
    }
}

TEST_CASE("Yest own implemented List Class assign functions") {
    ds::List<int> l;

    //void assign(size_type count, const T& value);
    SECTION("Add 'count' nodes of value 'value' to list") {
        int count {5};
        int val {13};

        l.assign(count, val);
        CHECK_THAT(l, EqualsVal(13));
        CHECK(l.size() == count);
    }

    //template<class InputIt>
    //void assign(InputIt first, InputIt last);
    SECTION("Assign elements between first and last to list") {
        ds::List<int> l1 {1, 2, 3, 4, 5};

        l.assign(l1.begin(), l1.end());

        CHECK_THAT(l1, EqualsContainer(l));
    }

    //void assign(std::initializer_list<T> iList);
    SECTION("Assign elements in 'iList' to list") {
        std::initializer_list<int> iList {1, 2, 3, 4, 5};

        l.assign(iList);

        CHECK_THAT(l, EqualsContainer(iList));
    }

}


TEST_CASE("Test own implemented List Class iterator functions") {
    ds::List<int> l {1, 2, 3, 4, 5};
    const ds::List<int> const_l {1, 2, 3, 4, 5};

    //iterator begin() noexcept; 
    SECTION("Returns iterator to first node") {
        CHECK(*(l.begin()) == 1);
    }


    //iterator end() noexcept; 
    SECTION("Returns iterator to first node") {
        CHECK(l.end() == nullptr);
    }

}

TEST_CASE("Test own implemented List Class access functions") {
    int first_elem {1};
    int last_elem {10};
    ds::List<int> l{first_elem, 2, 3, 4, 5, 6, 7, 8, 9, last_elem};

    //reference operator[](const size_type index);
    SECTION("Return reference to object at index 'index'. No index checking takes place") {
        REQUIRE(l[0] == first_elem);
        REQUIRE(l[9] == last_elem);
    }

    //reference front();
    SECTION("Returns reference to first element") {
        CHECK(l.front() == first_elem);
    }

    //reference back();
    SECTION("Returns reference to last element") {
        CHECK(l.back() == last_elem);
    }
}

TEST_CASE("Test own implemented List Class modifier functions") {
    
    ds::List<int> l;

    //void push_back(const T& value); 
    SECTION("Append l-value reference at end of list") {
        int value {4};
        l.push_back(value);
        CHECK_THAT(l, EqualsVal(value));
    }

    //void push_back(T&& value); 
    SECTION("Append r-value reference at end of list") {
        int value {4};
        l.push_back(std::move(value));
        CHECK_THAT(l, EqualsVal(value));
    }
    
    //void clear() noexcept;
    SECTION("Clear all nodes of list") {
        l = {1, 2, 3, 4, 5, 6};
        l.clear();
        CHECK(l.empty());
    }

    // Make tests Required becasue they are the foundation for the other
    // insert function tests
    
    //iterator insert(iterator pos, const T& value);
    SECTION("Insert Node with l-value 'value' at 'pos'") {
        int first_val {1};
        int second_value {2};
        int third_value {3};
        REQUIRE(*(l.insert(l.begin(), first_val)) == first_val);
        REQUIRE(l[0] == first_val);
        REQUIRE(l.size() == first_val);
        REQUIRE(*(l.insert(l.end(), second_value)) == second_value);
        REQUIRE(l[1] == second_value);
        REQUIRE(l.size() == second_value);
        REQUIRE(*(l.insert(++(l.begin()), third_value)) == third_value);
        REQUIRE(l[1] == third_value);
        REQUIRE(l.size() == third_value);
    }

    //iterator insert(iterator pos, T&& value);
    SECTION("Insert Node with r-value 'value' at 'pos'") {
        int first_val {1};
        CHECK(*(l.insert(l.begin(), std::move(first_val))) == first_val);
        CHECK(l[0] == first_val);
        CHECK(l.size() == first_val);
        
    }

    //iterator insert( iterator pos, size_type count, const T& value );
    SECTION("Insert 'count' Nodes with 'value' at 'pos'") {
        int value {4};
        int count = value;
        CHECK(*(l.insert(l.begin(), count, value)) == value);
        CHECK(l.size() == count);

    }

    //iterator insert(iterator pos, std::initializer_list<T> iList);
    SECTION("Insert elements of 'iList into' List") {
        std::initializer_list<int> iList {1, 2, 3, 4, 5};
        CHECK(*(l.insert(l.begin(), iList)) == 1);
        CHECK(l.size() == iList.size());
    }

    //template<class... Args>
    //iterator emplace(iterator pos, Args&&... args);
    SECTION("Emplace element") {
        int value {4};
        CHECK(*(l.emplace(l.begin(), value)) == value);
        CHECK(l.size() == 1);
    }

    //template<class... Args>
    //reference emplace(Args&&... args);
    SECTION("Emplace element at the end of list. Returns reference to inserted value") {
        int value_to_emplace {4};
        CHECK((l.emplace_back(value_to_emplace)) == value_to_emplace);
        CHECK(l.size() == 1);
    }

    //template<class... Args>
    //reference emplace(Args&&... args);
    SECTION("Emplace element at the front of the list. Returns reference to inserted value") {
        int value_to_insert {4};
        CHECK(l.emplace_front(value_to_insert) == value_to_insert);
        CHECK(l.size() == 1);
    }

    //iterator erase(iterator pos);
    SECTION("Erase element from list. Return iterator to next element") {
        l.assign({1, 2, 3});
        std::size_t old_size = l.size();
        auto it = l.erase(l.begin());
        CHECK(*it == 2);
        CHECK(l[0] == 2);
        CHECK(l.size() == (old_size - 1));
    }

    //iterator erase(iterator first, iterator last);
    SECTION("Erase elements '[first, last)' from list") {
        l.assign({1, 2, 3, 4, 5});
        auto it = l.erase(l.begin(), l.end());
        CHECK(l.size() == 0);
    }

    //void pop_back()
    SECTION("Remove last element of List") {
        int old_size = l.size();
        l.pop_back();
        CHECK(l.size() == old_size);
        l.assign({1});
        old_size = l.size();
        l.pop_back();
        CHECK(l.size() == old_size -1);
        
        int last_element {5};
        l.assign({1, 2, 3, 4, last_element});
        old_size = l.size();
        l.pop_back();
        CHECK(l.size() == old_size -1);
        CHECK(l[l.size()- 1] == 4);
    }

    //void push_front(const T& value) {};
    SECTION("Add element at the beginning of the list") {
        int value {4};
        int value1 {1};
        l.push_front(value);
        CHECK(l[0] == value);
        l.push_front(value1);
        CHECK(l[0] == value1);
    }

    //void push_front(T&& value) {};
    SECTION("Add element at the beginning of the list") {
        l.push_front(4);
        CHECK(l[0] == 4);
        l.push_front(1);
        CHECK(l[0] == 1);
    }

    //void pop_front()
    SECTION("Remove last element of List") {
        int old_size = l.size();
        l.pop_front();
        CHECK(l.size() == old_size);
        l.assign({1});
        old_size = l.size();
        l.pop_front();
        CHECK(l.size() == old_size -1);
        
        int last_element {5};
        l.assign({1, 2, 3, 4, last_element});
        old_size = l.size();
        l.pop_front();
        CHECK(l.size() == old_size -1);
        CHECK(l[0] == 2);
    }

    //void resize(size_type count, const T& value=T());
    SECTION("If count is bigger than 'size_' add nodes with value 'value'. If smaller resize list") {
        int resize_size {5};
        l.assign({1, 2, 3, 4, 5, 6, 7});
        l.resize(resize_size);
        CHECK(l.size() == resize_size);
        resize_size = 8;
        l.resize(resize_size);
        CHECK(l.size() == resize_size);
    }

    //void swap(List& other);
    SECTION("Swap 'tail_', 'head_' and 'size_' of '*this' and 'other'") {
        l.assign({1, 2, 3, 4, 5, 6, 7, 8});
        ds::List<int> copy_l {l};
        ds::List<int> l1 {8, 7, 6, 5, 4, 3, 2, 1};
        ds::List<int> copy_l1{l1};

        l.swap(l1);
        CHECK_THAT(l1, EqualsContainer(copy_l));
        CHECK_THAT(l, EqualsContainer(copy_l1));
    }
    
}

TEST_CASE("Test own implemented List Class assignment operator") {
    ds::List<int> l {1, 2, 3, 4, 5, 6};

    //List& operator=(const List& other);
    SECTION("Assign 'other' to *this") {
        ds::List<int> assign_l {5, 4, 3, 2, 1};
        l = assign_l;
        CHECK_THAT(l, EqualsContainer(assign_l));
    }

    //List& operator=(List&& other);
    SECTION("Move assign 'other' to *this") {
        ds::List<int> assign_l {5, 4, 3, 2, 1};
        l = std::move(assign_l);
        CHECK_THAT(l, !EqualsContainer(assign_l));
        CHECK(assign_l.size() == 0);
    }

    //List& operator=(std::initializer_list<T> iList);
    SECTION("Assign element in 'iList' to *this") {
        int value {5};
        std::initializer_list<int> iList{value, value, value, value, value};
        l = iList;
        CHECK_THAT(l, EqualsVal(value));
        CHECK(l.size() == iList.size());
    }
}


