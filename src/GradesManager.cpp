#include "GradesManager.h"

const std::string GradesManager::GRADES_FILE = "data/grades.txt";

void GradesManager::loadGrades(Repository<Student> &students)
{
    auto lines = DataManager::readFromFile(GRADES_FILE);
    for (const auto &line : lines)
    {
        if (line.empty())
            continue;

        // Format: studentLogin;subject;value;teacherLogin
        std::istringstream iss(line);
        std::string sLogin, subject, vStr, tLogin;

        if (std::getline(iss, sLogin, ';') &&
            std::getline(iss, subject, ';') &&
            std::getline(iss, vStr, ';') &&
            std::getline(iss, tLogin, ';'))
        {

            double value = 0.0;
            try
            {
                value = std::stod(vStr);
            }
            catch (...)
            {
            }

            auto it = students.find_if([&](const Student &s)
                                       { return s.getLogin() == sLogin; });
            if (it != students.getAll().end())
            {
                it->addGrade(Grade(subject, value, tLogin));
            }
        }
    }
}

void GradesManager::saveGrades(Repository<Student> &students)
{
    std::vector<std::string> lines;
    for (const auto &s : students.getAll())
    {
        for (const auto &g : s.getGrades())
        {
            lines.push_back(s.getLogin() + ";" + g.subject + ";" + std::to_string(g.value) + ";" + g.teacherLogin);
        }
    }
    DataManager::saveToFile(GRADES_FILE, lines);
}