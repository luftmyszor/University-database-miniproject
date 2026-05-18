#pragma once
// CLIManager.h
// Purpose: declare `CLIManager` which provides the interactive menus and
// orchestrates authentication, repositories and user workflows (admin/teacher/student).
#include <bits/stdc++.h>
#include "AuthManager.h"
#include "Account.h"
#include "Repository.h"
#include "Student.h"
#include "Teacher.h"
#include "DataManager.h"
#include "Course.h"
#include "GradesManager.h"
class CLIManager
{
private:
    enum states
    {
        START,
        SIGNUP,
        SIGNIN,
    };
    int _curentState;
    Account _currentUser;
    Repository<Student> students;
    Repository<Teacher> teachers;
    Repository<Course> courses;
    bool _running;

public:
    AuthManager auth = AuthManager();
    CLIManager();
    void CLI();
    bool isRunning() const;
    int getOption();
    void adminMenu();
    void teacherMenu();
    void studentMenu();
};