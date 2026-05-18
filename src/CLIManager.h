#pragma once
#include <bits/stdc++.h>
#include "AuthManager.h"
#include "Account.h"
#include "Repository.h"
#include "Student.h"
#include "Teacher.h"
#include "DataManager.h"
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