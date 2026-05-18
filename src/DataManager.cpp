#include "DataManager.h"
// DataManager.cpp
// Purpose: implement simple file read/write helpers used across the project.

void DataManager::saveToFile(const std::string &filename, const std::vector<std::string> &lines)
{
    std::ofstream ofs(filename);
    if (!ofs)
        throw std::runtime_error("Cannot open file " + filename);
    for (const auto &l : lines)
        ofs << l << "\n";
}

std::vector<std::string> DataManager::readFromFile(const std::string &filename)
{
    std::vector<std::string> out;
    std::ifstream ifs(filename);
    if (!ifs)
        return out; // treat missing file as empty data
    std::string line;
    while (std::getline(ifs, line))
    {
        out.push_back(line);
    }
    return out;
}
