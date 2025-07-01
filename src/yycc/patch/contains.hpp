#pragma once
#include "../macro/feature_probe.hpp"

namespace yycc::patch::contains {

    /**
     * @brief Checks if there is an element with key equivalent to key in the container.
     * @details
     * The polyfill to \c contains() function of unordered and ordered associative container.
     * Because this function only present after C++ 20.
     * This function will use our custom polyfill if the version of C++ standard you are using lower than C++ 20.
     * Otherwise it will fallback to vanilla standard library function.
     * @tparam TContainer
     * The type of container. This container must have \c find() and \c end() member functions.
     * @tparam TKey
     * The type of key of container.
     * If the container is a set, this type is the type of item in set.
     * If the container is a map, this type is the key type of map.
     * @param[in] container The reference to container to find.
     * @param[in] key Key value of the element to search for
     * @return True if there is such an element, otherwise false.
     * @remarks
     * This template function do not have constraint check.
     * If container type has \c find() and \c end() member functions, this template function will be created without any error.
     * However, this function should be used for standard library associative container according to its original purpose.
     * It means that the type of container usually and should be one of following types:
     * \li \c std::set
     * \li \c std::multiset
     * \li \c std::map
     * \li \c std::multimap
     * \li \c std::unordered_set
     * \li \c std::unordered_multiset
     * \li \c std::unordered_map
     * \li \c std::unordered_multimap
    */
    template<class TContainer, class TKey>
    bool contains(const TContainer& container, const TKey& key) {
#if defined(YYCC_CPPFEAT_CONTAINS)
        return container.contains(key);
#else
        return container.find(key) != container.end();
#endif
    }

} // namespace yycc::patch::container
