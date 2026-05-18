
#pragma once
#include "Person.h"
#include "Grade.h"
#include <bits/stdc++.h>

class Student : public Person {
private:
    std::string login;
    std::vector<Grade> grades;

public:
    Student();
    Student(const std::string &login, const std::string &name, const std::string &surname,
            int day, int month, int year, const std::string &pesel);
    ~Student();

    const std::string &getLogin() const;
    void addGrade(const Grade &g);
    double average() const;
    std::string toString() const override;
    static Student fromString(const std::string &line);
    void display() const override;
};
