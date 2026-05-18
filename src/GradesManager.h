#pragma once
#include <bits/stdc++.h>
#include "Repository.h"
#include "Student.h"
#include "DataManager.h"

class GradesManager
{
public:
    static const std::string GRADES_FILE;
    static void loadGrades(Repository<Student> &students);
    static void saveGrades(Repository<Student> &students);
};