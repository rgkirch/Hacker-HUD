#ifndef PROJECT_MYALGORITHM_HPP
#define PROJECT_MYALGORITHM_HPP

double maximum(std::vector<double> v) {
    if(v.empty()) abort();
    double m = v[0];
    for(double d : v) {
        m = (m < d ? d : m);
    }
    return m;
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
