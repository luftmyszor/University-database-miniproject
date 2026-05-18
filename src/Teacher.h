#pragma once
// Teacher.h
// Purpose: model for teachers; stores login, subjects and can assign grades.
#include "Person.h"
#include "Student.h"
#include <bits/stdc++.h>

class Teacher : public Person {
private:
    std::string login;
    std::vector<std::string> subjects;

public:
    Teacher();
    Teacher(const std::string &login, const std::string &name, const std::string &surname,
            int day, int month, int year, const std::string &pesel);
    ~Teacher();

    const std::string &getLogin() const;
    void addSubject(const std::string &s);
    const std::vector<std::string> &getSubjects() const;

    void assignGrade(Student &student, const std::string &subject, double value);

    std::string toString() const override;
    static Teacher fromString(const std::string &line);
    void display() const override;
};
