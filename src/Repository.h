#pragma once
// Repository.h
// Purpose: generic in-memory container with load/save helpers for simple persistence.
#include <bits/stdc++.h>

template <typename T>
class Repository {
private:
    std::vector<T> items;

public:
    Repository() = default;
    ~Repository() = default;

    void add(const T &item) { items.push_back(item); }

    typename std::vector<T>::iterator find_if(std::function<bool(const T &)> pred) {
        return std::find_if(items.begin(), items.end(), pred);
    }

    void remove_if(std::function<bool(const T &)> pred) {
        items.erase(std::remove_if(items.begin(), items.end(), pred), items.end());
    }

    std::vector<T> &getAll() { return items; }

    void sortBy(std::function<bool(const T &, const T &)> cmp) { std::sort(items.begin(), items.end(), cmp); }

    void loadFromFile(const std::string &filename, std::function<T(const std::string &)> parser) {
        std::ifstream ifs(filename);
        if (!ifs) throw std::runtime_error("Cannot open file " + filename);
        std::string line;
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            items.push_back(parser(line));
        }
    }

    void saveToFile(const std::string &filename, std::function<std::string(const T &)> serializer) const {
        std::ofstream ofs(filename);
        if (!ofs) throw std::runtime_error("Cannot open file " + filename);
        for (const T &it : items)
            ofs << serializer(it) << "\n";
    }
};
