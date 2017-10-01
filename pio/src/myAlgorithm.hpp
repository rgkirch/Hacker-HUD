#ifndef PROJECT_MYALGORITHM_HPP
#define PROJECT_MYALGORITHM_HPP

#include <vector>

using std::vector;

// only call with non-empty vector
double maximum(std::vector<double> v) {
    if(v.empty()) abort();
    double m = v[0];
    for(double d : v) {
        m = (m < d ? d : m);
    }
    return m;
}

vector<int> normalize(vector<double> data, int height) {
    double max = *std::max_element(begin(data), end(data));
    return transform(data, [=](double x)->int { // is this capturing height or this?
        return (int)(x / max * height);
    });
}



//template<typename ForwardIterator>
//auto maximum(ForwardIterator first, ForwardIterator last)-> ForwardIterator {
//    ForwardIterator retval = first;
//    ++first;
//    while(first != last) {
//        if(*retval > *first) {
//            retval = first;
//        }
//        ++first;
//    }
//    return retval;
//}

#endif //PROJECT_MYALGORITHM_HPP
