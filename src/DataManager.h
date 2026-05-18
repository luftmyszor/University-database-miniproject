#pragma once
#include <bits/stdc++.h>

class DataManager
{
public:
    static void saveToFile(const std::string &filename, const std::vector<std::string> &lines);
    static std::vector<std::string> readFromFile(const std::string &filename);
};