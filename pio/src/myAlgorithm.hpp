#ifndef PROJECT_MYALGORITHM_HPP
#define PROJECT_MYALGORITHM_HPP

template<typename InputIt>
auto maximum(InputIt first, InputIt last)-> decltype(*first) {
    decltype(*first) maximum = *first;
    auto it = first;
    while(it != last) {
        maximum = (*it > maximum ? *it : maximum);
        it++;
    }
    return maximum;
}


#endif //PROJECT_MYALGORITHM_HPP
