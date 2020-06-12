//
// Created by eliane on 12/06/2020.
//

#ifndef FESUTILS_LOG_UTILS_HPP
#define FESUTILS_LOG_UTILS_HPP

#include <boost/log/trivial.hpp>
#include <iostream>

namespace std {

    template<typename T, typename TT>
    inline basic_ostream<TT>& operator<<(basic_ostream<TT>& p, const std::vector<T>& vector) {
        p << "[ ";
        for (const auto& i: vector) {
            p << " " << i << " ,";
        }
        p << "]";
        return p;
    }


    template<typename T, typename TT, unsigned int N>
    inline basic_ostream<TT>& operator<<(basic_ostream<TT>& p, const std::array<T, N>& vector) {
        p << "[ ";
        for (const auto& i: vector) {
            p << " " << i << " ,";
        }
        p << "]";
        return p;
    }

}

#endif //FESUTILS_LOG_UTILS_HPP
