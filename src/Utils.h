#pragma once
#include <iostream>
#include <string>

template <typename Iter>
void printRange(Iter begin, Iter end, const std::string &sep = "\n") {
    for (; begin != end; ++begin) {
        std::cout << *begin;
        if (std::next(begin) != end) std::cout << sep;
    }
    std::cout << std::endl;
}
