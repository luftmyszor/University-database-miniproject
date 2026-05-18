#pragma once
#include <bits/stdc++.h>
#include "DataManager.h"

template <typename T>
class Repository
{
private:
    std::vector<T> items;

public:
    Repository() = default;
    ~Repository() = default;

    void add(const T &item) { items.push_back(item); }

    typename std::vector<T>::iterator find_if(std::function<bool(const T &)> pred)
    {
        return std::find_if(items.begin(), items.end(), pred);
    }

    void remove_if(std::function<bool(const T &)> pred)
    {
        items.erase(std::remove_if(items.begin(), items.end(), pred), items.end());
    }

    std::vector<T> &getAll() { return items; }

    void sortBy(std::function<bool(const T &, const T &)> cmp)
    {
        std::sort(items.begin(), items.end(), cmp);
    }

    void loadFromFile(const std::string &filename, std::function<T(const std::string &)> parser)
    {
        auto lines = DataManager::readFromFile(filename);
        for (const auto &line : lines)
        {
            if (!line.empty())
            {
                items.push_back(parser(line));
            }
        }
    }

    void saveToFile(const std::string &filename, std::function<std::string(const T &)> serializer) const
    {
        std::vector<std::string> lines;
        for (const T &it : items)
        {
            lines.push_back(serializer(it));
        }
        DataManager::saveToFile(filename, lines);
    }
};