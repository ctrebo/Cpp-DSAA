#ifndef OWN_CUSTOM_MATCHERS
#define OWN_CUSTOM_MATCHERS

#include <catch2/matchers/catch_matchers_templated.hpp>
#include <fmt/core.h>



template<typename T>
struct EqualsValueMatcher : Catch::Matchers::MatcherGenericBase {
    EqualsValueMatcher(const T& val): val_{ val }
    {}

    template<typename Type>
    bool match(const Type& container) const {
        // If container is empty it cant match value
        if(container.size() == 0) {
            return false;
        }

        return (std::all_of(container.cbegin(), container.cend(),
                    [&](const T& container_val){
                    return container_val == val_;
                    }));
    }

    std::string describe() const override {
        std::string msg = fmt::format("At least one value from the container doesnt match {}", val_);
        return msg;
    }

private:
    const T& val_;
};

template<typename T>
auto EqualsVal(const T& val) -> EqualsValueMatcher<T> {
    return EqualsValueMatcher<T>{val};
}

template<typename T>
struct EqualsContainerMatcher : Catch::Matchers::MatcherGenericBase {
    EqualsContainerMatcher(const T& container):
        container_{ container }
    {}

    template<typename Type>
    bool match(const Type& other_container) const {
        return ((container_.size() == other_container.size()) && std::equal(container_.begin(), container_.end(), other_container.begin())); 
    }

    std::string describe() const override {
        return "Containers do not match";
    }

private:
    const T& container_;
};

template<typename T>
auto EqualsContainer(const T& container) -> EqualsContainerMatcher<T> {
    return EqualsContainerMatcher<T>{container};
}

#endif // OWN_CUSTOM_MATCHERS
