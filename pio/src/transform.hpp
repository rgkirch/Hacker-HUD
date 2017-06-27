#ifndef PROJECT_TRANSFORM_HPP
#define PROJECT_TRANSFORM_HPP

#include <vector>

template<class InputIt, class OutputIt, class UnaryOperation>
OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
    while (first1 != last1) {
        *d_first++ = unary_op(*first1++);
    }
    return d_first;
}

//template<typename Collection, typename UnaryOperation>
//Collection transform(Collection collection, UnaryOperation unary_op) {
//    for(auto x : collection) {
//        x = unary_op(x);
//    }
//    return collection;
//}

//template<typename Collection, typename UnaryOperation>
//void transform(Collection collection, UnaryOperation op) {
//    for(auto x : collection) {
//        x = op(x);
//    }
//}

template<typename Collection, typename UnaryOperation>
auto transform(Collection collection, UnaryOperation op)-> std::vector<decltype(op(collection[0]))> {
    std::vector<decltype(op(collection[0]))> vec{};
    vec.reserve(collection.size());
    for(auto x : collection) {
        vec.push_back(op(x));
    }
    return vec;
}

#endif //PROJECT_TRANSFORM_HPP
