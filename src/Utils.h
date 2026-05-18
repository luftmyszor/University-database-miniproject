#pragma once
#include <iostream>
#include <string>

/*
 * printRange - simple iterator printer
 * -----------------------------------
 * Prints every element from `begin` up to (but not including) `end` to
 * the standard output. Elements are separated by `sep`, and the function
 * prints a final newline at the end.
 *
 * Notes:
 * - Works with any container iterator (vector, set, list, etc.).
 * - Defaults to printing one element per line (`sep = "\n").
 * - Avoids a trailing separator between elements.
 *
 * Example:
 *   std::vector<std::string> v = {"a", "b", "c"};
 *   printRange(v.begin(), v.end(), ", "); // prints: a, b, c\n
 */

template <typename Iter>
void printRange(Iter begin, Iter end, const std::string &sep = "\n") {
    for (; begin != end; ++begin) {
        std::cout << *begin;
        if (std::next(begin) != end) std::cout << sep;
    }
    std::cout << std::endl;
}
